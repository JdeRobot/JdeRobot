#include "interface.h"
#include <jde.h>
#include <hierarchy.h>
#include <schema.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <simclist.h>

struct JDEInterface_p{
  unsigned int refcount;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  /*void* datap;*/
};

struct JDEInterfacePrx_p{
  pthread_mutex_t mutex;
};


/*not protected functions. Monitor implementation*/
void JDEInterface_refcount_inc_i(JDEInterface* const self){
  assert(self!=0);
  self->priv->refcount++;
}

void JDEInterface_refcount_dec_i(JDEInterface* const self){
  assert(self!=0);
  assert(self->priv->refcount > 0);
  self->priv->refcount--;
}


JDEInterface* new_JDEInterface(const char* interface_name,
			       JDESchema* const supplier){
  JDEInterface* i;
  int rc;
  
  assert(interface_name!=0 && supplier!=0);
  i = (JDEInterface*)calloc(1,sizeof(JDEInterface));/*all set to 0*/
  assert(i!=0);
  i->interface_name= strdup(interface_name);
  i->supplier = supplier;
  i->priv = (JDEInterface_p*)calloc(1,sizeof(JDEInterface_p));
  assert(i->priv!=0);
  i->priv->refcount = 1;
  pthread_mutex_init(&(i->priv->mutex),NULL);
  pthread_cond_init(&(i->priv->cond),NULL);
  if (supplier->hierarchy==0)/*old schema*/
    rc = myexport(interface_name,"JDEInterface",i);
  else
    rc = JDEHierarchy_myexport(supplier->hierarchy,
			       interface_name,"JDEInterface",i);
  if (rc == 0){/*can't export symbol*/
    free(i->priv);
    free(i->interface_name);
    free(i);
    return 0;
  }
  JDESchema_interface_add(supplier,i);
  return i;
}


JDEInterfacePrx* new_JDEInterfacePrx(const char* interface_name,
				     JDESchema* const user){
  JDEInterfacePrx* iprx;
  JDEInterface *refers_to;
  
  assert(user!=0 && user->hierarchy!=0);
  refers_to = (JDEInterface*)JDEHierarchy_myimport(user->hierarchy,
						   interface_name,"JDEInterface");
  if (refers_to == 0)/*no interface found*/
    return 0;
  iprx = (JDEInterfacePrx*)calloc(1,sizeof(JDEInterfacePrx));
  assert(iprx!=0);
  iprx->refers_to = refers_to;
  iprx->user = user;
  iprx->priv = (JDEInterfacePrx_p*)calloc(1,sizeof(JDEInterfacePrx_p));
  assert(iprx->priv!=0);
  pthread_mutex_init(&(iprx->priv->mutex),NULL);
  JDEInterface_refcount_inc(iprx->refers_to);
  JDESchema_interfaceprx_add(user,iprx);
  return iprx;
}

void delete_JDEInterface(JDEInterface* const self){
  if (self==0)
    return;
  
  pthread_mutex_lock(&(self->priv->mutex));
  if (self->priv->refcount == 0){
    JDESchema_interface_del(self->supplier,self);
    /*FIXME: delete exported symbols to assure any othe prx will have
      a reference to this Interface.*/
    pthread_mutex_unlock(&(self->priv->mutex));
    free(self->priv);
    free(self->interface_name);
    free(self);
  }else
    pthread_mutex_unlock(&(self->priv->mutex));
}
  
void delete_JDEInterfacePrx(JDEInterfacePrx* const self){
  if (self==0)
    return;
  
  JDESchema_interfaceprx_del(self->user,self);
  assert(self->refers_to);
  JDEInterface_refcount_dec(self->refers_to);
  free(self);
}


void JDEInterface_refcount_inc(JDEInterface* const self){
  assert(self!=0);
  pthread_mutex_lock(&(self->priv->mutex));
  JDEInterface_refcount_inc_i(self);
  pthread_mutex_unlock(&(self->priv->mutex));
}

void JDEInterface_refcount_dec(JDEInterface* const self){
  assert(self!=0);
  pthread_mutex_lock(&(self->priv->mutex));
  JDEInterface_refcount_dec_i(self);
  if (self->priv->refcount == 0){
    pthread_mutex_unlock(&(self->priv->mutex));
    delete_JDEInterface(self);
  }else
    pthread_mutex_unlock(&(self->priv->mutex));
}

int JDEInterfacePrx_run(const JDEInterfacePrx* self){
  JDESchema *s;
  
  assert(self!=0);
  s = PRX_REFERS_TO(self)->supplier;/*schema implementing interface*/
  assert(s != self->user);/*avoid loops*/
  return JDESchema_run(s,self->user);
}

int JDEInterfacePrx_stop(const JDEInterfacePrx* self){
  JDESchema *s;

  assert(self!=0);
  s = PRX_REFERS_TO(self)->supplier;/*schema implementing interface*/
  assert(s != self->user);/*avoid loops*/
  return JDESchema_stop(s);
}
