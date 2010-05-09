#include "schema.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

JDESchema* new_JDESchema(char* name,
			 void (*init)(char *configfile),
			 void (*terminate)(void),
			 void (*stop)(void),
			 void (*run)(int father, int *brothers, arbitration fn),
			 void (*show)(void),
			 void (*hide)(void)){
  JDESchema* s = 0;
  
  if (num_schemas>MAX_SCHEMAS) {
    fprintf(stderr, "WARNING: No entry available for %s schema\n",name);
    return 0;
  }

  s = &all[num_schemas];
  s->handle = 0;
  s->init = init;
  s->terminate = terminate;
  s->stop = stop;
  s->run = run;
  s->show = show;
  s->hide = hide;
  s->id = (int*)malloc(sizeof(int));
  *(s->id) = num_schemas;
  strcpy(s->name,name);
  s->fps = 0.;
  s->k =0;
  s->state=slept;
  s->guistate=off;
  pthread_mutex_init(&s->mymutex,NULL);
  pthread_cond_init(&s->condition,NULL);
  fprintf(stderr,"%s schema loaded (id %d)\n",name,(*(s->id)));
  num_schemas++;
  return s;
}

void delete_JDESchema(JDESchema* const s){
  /*delete a schema is not implemented yet. We simply terminate it*/
  assert(s!=0);
  s->terminate();
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

void JDESchema_init(JDESchema* const s, char *configfile){
  assert(s!=0);
  s->init(configfile);
}

void JDESchema_terminate(JDESchema* const s){
  assert(s!=0);
  s->terminate();
}

void JDESchema_stop(JDESchema* const s){
  assert(s!=0);
  s->stop();
}

void JDESchema_run(JDESchema* const s,
		   int father, int *brothers, arbitration fn){
  assert(s!=0);
  s->run(father,brothers,fn);
}

void JDESchema_show(JDESchema* const s){
  assert(s!=0);
  s->show();
}

void JDESchema_hide(JDESchema* const s){
  assert(s!=0);
  s->hide();
}

void JDEDriver_init(JDEDriver* const d, char *configfile){
  assert(d!=0);
  d->init(configfile);
}

void JDEDriver_terminate(JDEDriver* const d){
  assert(d!=0);
  d->terminate();
}

int JDESchema_get_state(JDESchema* const s){
  int state;

  assert(s!=0);
  //fprintf(stderr,"lock en get_state\n");
  //pthread_mutex_lock(&s->mymutex);
  state = s->state;
  //pthread_mutex_unlock(&s->mymutex);
  //fprintf(stderr,"unlock en get_state\n");
  return state;
}

void JDESchema_set_state(JDESchema* const s, int newstate){
  assert(s!=0);
  //fprintf(stderr,"lock en set_state\n");
  //pthread_mutex_lock(&s->mymutex);
  //if (newstate != s->state){
  //  fprintf(stderr,"signal en set_state\n");
  //  pthread_cond_broadcast(&s->condition);
  //}
  s->state=newstate;
  //pthread_mutex_unlock(&s->mymutex);
  //fprintf(stderr,"unlock en set_state\n");
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

void JDESchema_speedcounter(JDESchema* const s){
  assert(s!=0);
  //fprintf(stderr,"lock en speedcounter\n");
  //pthread_mutex_lock(&s->mymutex);
  s->k++;
  //pthread_mutex_unlock(&s->mymutex);
  //fprintf(stderr,"unlock en speedcounter\n");
}
