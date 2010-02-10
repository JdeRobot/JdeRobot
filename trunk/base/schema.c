#include "schema.h"
#include "hierarchy.h"
#include "interface.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <simclist.h>


const char *const states_str[] = {
  "slept","active","notready","ready","forced","winner"
};

struct JDESchema_p{
  list_t interface_list;/*FIXME: check thread safe*/
  list_t interfaceprx_list;
};

JDESchema* new_JDESchema(char* name,
			 void (*init)(char *configfile),
			 void (*terminate)(void),
			 void (*stop)(void),
			 void (*run)(int father, int *brothers, arbitration fn),
			 void (*show)(void),
			 void (*hide)(void)){
  JDESchema* s = 0;
  s = (JDESchema*)calloc(1,sizeof(JDESchema));
  assert(s!=0);
  s->handle = 0;
  s->init = init;
  s->terminate = terminate;
  s->stop = stop;
  s->run = run;
  s->show = show;
  s->hide = hide;
  s->id = (int*)malloc(sizeof(int)); /*FIXME: allocated to keep
				       backwards compatibility*/
  *(s->id) = 0;
  strcpy(s->name,name);
  s->fps = 0.;
  s->k =0;
  s->state=slept;
  s->guistate=off;
  pthread_mutex_init(&s->mymutex,NULL);
  pthread_cond_init(&s->condition,NULL);
  s->hierarchy = 0;
  s->version = 4;
  s->revision = 4;
  s->priv = (JDESchema_p*)calloc(1,sizeof(JDESchema_p));
  assert(s->priv!=0);
  list_init(&(s->priv->interface_list));
  list_init(&(s->priv->interfaceprx_list));
  fprintf(stderr,"%s schema loaded\n",name);
  return s;
}

void delete_JDESchema(JDESchema* const self){
  /*FIXME:delete a schema is not implemented yet.We simply terminate it*/
  assert(self!=0);
  self->terminate();
}

/* void* schema_std_thread(void* schema){ */
/*   JDESchema* s = (JDESchema*)schema; */
/*   int state = 0; */
  
/*   while(state>=0){ */
/*     state = JDESchema_get_state(s); */
/*     switch(state){ */
/*     case slept: */
/*       pthread_mutex_lock(&s->mymutex); */
/*       pthread_cond_wait(&s->condition,&s->mymutex); */
/*       pthread_mutex_unlock(&s->mymutex); */
/*       break;; */
/*     case notready: */
/*       state = ready; */
/*       break;; */
/*     case ready: */
/*       state = winner */
/*       ;; */
/*     } */

/*     if (state == winner){ */
/*       JDESchema_speedcounter(s); */
/*       JDESchema_iteration(s); */
/*     } */
/*     JDESchema_set_state(s,state); */
/*     /\*sleep*\/ */
/*   } */
/*   return 0; */
/* } */

int JDESchema_init(JDESchema* const self, char *configfile){
  assert(self!=0);
  self->init(configfile);
  return 1;
}

int JDESchema_terminate(JDESchema* const self){
  assert(self!=0);
  self->terminate();
  return 1;
}

int JDESchema_stop(JDESchema* const self){
  assert(self!=0);
  self->stop();
  return 1;
}

int JDESchema_run(JDESchema* const self, JDESchema* const father){
  assert(self!=0);
  if (father==0)
    self->run(-1,0,0);/*FIXME: value used to run a schema without
		     father*/
  else
    self->run(*(father->id),0,0);
  return 1;
}

int JDESchema_show(JDESchema* const self){
  assert(self!=0);
  self->show();
  return 1;
}

int JDESchema_hide(JDESchema* const self){
  assert(self!=0);
  self->hide();
  return 1;
}

int JDEDriver_init(JDEDriver* const self, char *configfile){
  assert(self!=0);
  self->init(configfile);
  return 1;
}

int JDEDriver_terminate(JDEDriver* const self){
  assert(self!=0);
  self->terminate();
  return 1;
}

int JDESchema_state_get(JDESchema* const self){
  int state;

  assert(self!=0);
  //fprintf(stderr,"lock en get_state\n");
  //pthread_mutex_lock(&s->mymutex);
  state = self->state;
  //pthread_mutex_unlock(&s->mymutex);
  //fprintf(stderr,"unlock en get_state\n");
  return state;
}

int JDESchema_state_set(JDESchema* const self, int newstate){
  assert(self!=0);
  //fprintf(stderr,"lock en set_state\n");
  //pthread_mutex_lock(&s->mymutex);
  //if (newstate != s->state){
  //  fprintf(stderr,"signal en set_state\n");
  //  pthread_cond_broadcast(&s->condition);
  //}
  self->state=newstate;
  //pthread_mutex_unlock(&s->mymutex);
  //fprintf(stderr,"unlock en set_state\n");
  return 1;
}

//void JDESchema_wait_statechange(JDESchema* const s){
  //assert(s!=0);
  //fprintf(stderr,"lock en wait_statechange\n");
  //pthread_mutex_lock(&s->mymutex);
  //fprintf(stderr,"wait en wait_statechange\n");
  //pthread_cond_wait(&s->condition,&s->mymutex);
  //pthread_mutex_unlock(&s->mymutex);
  //fprintf(stderr,"unlock en wait_statechange\n");
//}

int JDESchema_speedcounter(JDESchema* const self){
  assert(self!=0);
  //fprintf(stderr,"lock en speedcounter\n");
  //pthread_mutex_lock(&s->mymutex);
  self->k++;
  //pthread_mutex_unlock(&s->mymutex);
  //fprintf(stderr,"unlock en speedcounter\n");
  return self->k;
}

int JDESchema_interface_add(JDESchema* const self, JDEInterface *const interface){
  assert(self!=0 && interface!=0);
  if (self->version>=4 && self->revision>=4)/*only from 4.4*/
    return list_append(&(self->priv->interface_list),(void*)interface);
  return -1;
}

int JDESchema_interface_del(JDESchema* const self, JDEInterface *const interface){
  unsigned int pos;

  assert(self!=0 && interface!=0);
  if (self->version>=4 && self->revision>=4){/*only from 4.4*/
    pos = list_locate(&(self->priv->interface_list),(void*)interface);
    if (pos>=0)
      list_delete_at(&(self->priv->interface_list),pos);
    return (pos<0)?pos:1;/*if element not found returns pos (<0),
			   otherwise 1*/
  }
  return -1;
}
  
JDEInterface **JDESchema_interface_list(JDESchema* const self){
  unsigned int size,i;
  JDEInterface **list;
  
  assert(self!=0);
  if (self->version>=4 && self->revision>=4){/*only from 4.4*/
    size = list_size(&(self->priv->interface_list));
    list = (JDEInterface**)calloc(size+1,sizeof(*list));
    for (i=0; i<size; i++)
      list[i] = (JDEInterface*)list_get_at(&(self->priv->interface_list),i);
    list[size] = 0;
    return list;
  }
  return -1;
}

int JDESchema_interfaceprx_add(JDESchema* const self, JDEInterfacePrx *const iprx){
  assert(self!=0 && iprx!=0);
  if (self->version>=4 && self->revision>=4)/*only from 4.4*/
    return list_append(&(self->priv->interfaceprx_list),(void*)iprx);
  return -1;
}

int JDESchema_interfaceprx_del(JDESchema* const self, JDEInterfacePrx *const iprx){
  unsigned int pos;

  assert(self!=0 && iprx!=0);
  if (self->version>=4 && self->revision>=4){/*only from 4.4*/
    pos = list_locate(&(self->priv->interfaceprx_list),(void*)iprx);
    if (pos>0)
      list_delete_at(&(self->priv->interfaceprx_list),pos);
    return (pos<0)?pos:1;/*if element not found returns pos (<0),
			   otherwise 1*/
  }
  return -1;
}

JDEInterfacePrx **JDESchema_interfaceprx_list(JDESchema* const self){
  unsigned int size,i;
  JDEInterfacePrx **list;
  
  assert(self!=0);
  if (self->version>=4 && self->revision>=4){/*only from 4.4*/
    size = list_size(&(self->priv->interfaceprx_list));
    list = (JDEInterfacePrx**)calloc(size+1,sizeof(*list));
    for (i=0; i<size; i++)
      list[i] = (JDEInterfacePrx*)list_get_at(&(self->priv->interfaceprx_list),i);
    list[size] = 0;
    return list;
  }
  return -1;
}
