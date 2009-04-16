#include "schema.h"
#include <stdio.h>
#include <string.h>
#include <Python.h>

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

int JDESchema_get_state(JDESchema* const s){
  assert(s!=0);
  return s->state;
}

void JDESchema_set_state(JDESchema* const s, int newstate){
  assert(s!=0);
  s->state=newstate;
  /* only some changes are relevant. For instance change of one 
     motor schema from active to ready is not, because it happens every iteration */
  if ((newstate==winner) || 
      (newstate==slept) || 
      (newstate==forced)|| 
      (newstate==notready)|| 
      (newstate==ready)|| 
      (newstate==active));
}

void JDESchema_speedcounter(JDESchema* const s){
  assert(s!=0);
  s->k++;
}

void JDEDriver_init(JDEDriver* const d, char *configfile){
  assert(d!=0);
  d->init(configfile);
}

void JDEDriver_terminate(JDEDriver* const d){
  assert(d!=0);
  d->terminate();
}
