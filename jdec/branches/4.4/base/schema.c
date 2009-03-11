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
  pthread_mutex_init(&s->mymutex,PTHREAD_MUTEX_TIMED_NP);
  pthread_cond_init(&s->condition,NULL);
  fprintf(stderr,"%s schema loaded (id %d)\n",name,(*(s->id)));
  num_schemas++;
  return s;
}
