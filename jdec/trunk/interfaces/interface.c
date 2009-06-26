#include "interface.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <jde_private.h>
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
  
  assert(supplier!=0);
  i = (JDEInterface*)calloc(1,sizeof(JDEInterface));/*all set to 0*/
  assert(i!=0);
  strncpy(i->interface_name,interface_name,MAX_NAME);
  i->interface_name[MAX_NAME-1] = '\0';
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
      assert(supplier_id!=0);/*if id not registered we can't guess who
			       is the supplier for this interface*/
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

void delete_JDEInterface(JDEInterface* const this){
  if (this==0)
    return;
  list_destroy(&(this->priv->referral_list));
  free(this->priv);
  free(this);
}
  
void delete_JDEInterfacePrx(JDEInterfacePrx* const this){
  if (this==0)
    return;
  if (this->refers_to){
    if (JDEInterface_refcount(this->refers_to)==1)/*last reference*/
      /*FIXME: delete exported symbols*/
      delete_JDEInterface(this->refers_to);
    else
      JDEInterface_delref(this->refers_to,this);
  }
  free(this);
}

void JDEInterface_addref(JDEInterface* const this,
			 JDEInterfacePrx* const referral){
  assert(this!=0 && referral!=0);
  pthread_mutex_lock(&(this->priv->mutex));
  list_append(&(this->priv->referral_list),referral);
  pthread_mutex_unlock(&(this->priv->mutex));
}

void JDEInterface_delref(JDEInterface* const this,
			 JDEInterfacePrx* const referral){
  unsigned int pos;

  assert(this!=0 && referral!=0);
  pthread_mutex_lock(&(this->priv->mutex));
  pos = list_locate(&(this->priv->referral_list),referral);
  if (pos>0)
    list_delete_at(&(this->priv->referral_list),pos);
  pthread_mutex_unlock(&(this->priv->mutex));
}

unsigned int JDEInterface_refcount(JDEInterface* const this){
  unsigned int s;
  assert(this!=0);
  pthread_mutex_lock(&(this->priv->mutex));
  s = list_size(&(this->priv->referral_list));
  pthread_mutex_unlock(&(this->priv->mutex));
  return s;
}

void JDEInterfacePrx_run(const JDEInterfacePrx* this){
  runFn irun;
  int id;
  
  assert(this!=0);
  assert(PRX_REFERS_TO(this)->supplier != this->user);/*avoid loops*/
  PRX_REFERS_TO(this)->supplier->run(*(this->user->id),NULL,NULL);
  this->user->children[*(PRX_REFERS_TO(this)->supplier->id)] = 1;
}

void JDEInterfacePrx_stop(const JDEInterfacePrx* this){
  int id;

  assert(this!=0);
  assert(PRX_REFERS_TO(this)->supplier != this->user);/*avoid loops*/
  PRX_REFERS_TO(this)->supplier->stop();
  this->user->children[*(PRX_REFERS_TO(this)->supplier->id)] = 0;
}
