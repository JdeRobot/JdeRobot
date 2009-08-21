#include "interface.h"
#include <jde.h>
#include <schema.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <simclist.h>

struct JDEInterface_p{
  list_t referral_list;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  /*void* datap;*/
};

struct JDEInterfacePrx_p{
  pthread_mutex_t mutex;
};


JDEInterface* new_JDEInterface(const char* interface_name,
			       JDESchema* const supplier){
  JDEInterface* i;
  
  assert(interface_name!=0 && supplier!=0);
  i = (JDEInterface*)calloc(1,sizeof(JDEInterface));/*all set to 0*/
  assert(i!=0);
  i->interface_name= strdup(interface_name);
  i->supplier = supplier;
  i->priv = (JDEInterface_p*)calloc(1,sizeof(JDEInterface_p));
  assert(i->priv!=0);
  list_init(&(i->priv->referral_list));
  pthread_mutex_init(&(i->priv->mutex),NULL);
  pthread_cond_init(&(i->priv->cond),NULL);
  return i;
}

JDEInterfacePrx* new_JDEInterfacePrx(const char* interface_name,
				     JDESchema* const user,
				     JDEInterface* const refers_to){
  JDEInterfacePrx* iprx;
  
  assert(user!=0);
  iprx = (JDEInterfacePrx*)calloc(1,sizeof(JDEInterfacePrx));
  assert(iprx!=0);
  if (refers_to == 0){/*try to get object with myimport*/
    iprx->refers_to = (JDEInterface*)myimport(interface_name,"JDEInterface");
    if (iprx->refers_to == 0){/*if JDEInterface object doesn't exists. It
			  is an old interface.For backward
			  compatibility we try to create one.*/
      JDESchema* supplier;
      int *supplier_id;
      
      supplier_id = (int*)myimport(interface_name,"id");
      assert(supplier_id!=0);/*FIXME:show error: no interface found in
			       the system*/
      supplier = get_schema(*supplier_id);
      iprx->refers_to = new_JDEInterface(interface_name,supplier);
    }
  }else{/*we own the interface instance, we register it*/
    iprx->refers_to = refers_to;
    /*export symbols*/
    myexport(interface_name,"JDEInterface",refers_to);
    myexport(interface_name,"id",refers_to->supplier->id);
    myexport(interface_name,"run",refers_to->supplier->run);
    myexport(interface_name,"stop",refers_to->supplier->stop);
  }
  iprx->user = user;
  iprx->priv = (JDEInterfacePrx_p*)calloc(1,sizeof(JDEInterfacePrx_p));
  assert(iprx->priv!=0);
  pthread_mutex_init(&(iprx->priv->mutex),NULL);
  JDEInterface_addref(iprx->refers_to,iprx);
  return iprx;
}

void delete_JDEInterface(JDEInterface* const self){
  if (self==0)
    return;
  list_destroy(&(self->priv->referral_list));
  free(self->priv);
  free(self->interface_name);
  free(self);
}
  
void delete_JDEInterfacePrx(JDEInterfacePrx* const self){
  if (self==0)
    return;
  if (self->refers_to){
    if (JDEInterface_refcount(self->refers_to)==1)/*last reference*/
      /*FIXME: delete exported symbols*/
      delete_JDEInterface(self->refers_to);
    else
      JDEInterface_delref(self->refers_to,self);
  }
  free(self);
}

void JDEInterface_addref(JDEInterface* const self,
			 JDEInterfacePrx* const referral){
  assert(self!=0 && referral!=0);
  pthread_mutex_lock(&(self->priv->mutex));
  list_append(&(self->priv->referral_list),referral);
  pthread_mutex_unlock(&(self->priv->mutex));
}

void JDEInterface_delref(JDEInterface* const self,
			 JDEInterfacePrx* const referral){
  unsigned int pos;

  assert(self!=0 && referral!=0);
  pthread_mutex_lock(&(self->priv->mutex));
  pos = list_locate(&(self->priv->referral_list),referral);
  if (pos>0)
    list_delete_at(&(self->priv->referral_list),pos);
  pthread_mutex_unlock(&(self->priv->mutex));
}

unsigned int JDEInterface_refcount(JDEInterface* const self){
  unsigned int s;
  assert(self!=0);
  pthread_mutex_lock(&(self->priv->mutex));
  s = list_size(&(self->priv->referral_list));
  pthread_mutex_unlock(&(self->priv->mutex));
  return s;
}

void JDEInterfacePrx_run(const JDEInterfacePrx* self){
  JDESchema *s;
  
  assert(self!=0);
  s = PRX_REFERS_TO(self)->supplier;/*schema implementing interface*/
  assert(s != self->user);/*avoid loops*/
  JDESchema_run(s,self->user);
}

void JDEInterfacePrx_stop(const JDEInterfacePrx* self){
  JDESchema *s;

  assert(self!=0);
  s = PRX_REFERS_TO(self)->supplier;/*schema implementing interface*/
  assert(s != self->user);/*avoid loops*/
  JDESchema_stop(s);
}
