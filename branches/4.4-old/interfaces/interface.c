#include "interface.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <jde_private.h>

/*constructor & destructor*/
Interface* new_Interface(JDESchema* const owner,
			 const char* interface_name,
			 const int implemented){
  Interface* i;
  
  assert(owner!=0);
  i = (Interface*)calloc(1,sizeof(Interface));/*all set to 0*/
  assert(i!=0);
  strncpy(i->interface_name,interface_name,MAX_NAME);
  i->interface_name[MAX_NAME-1] = '\0';
  i->owner = owner;
  i->implemented = implemented;
  if (implemented){
    myexport(i->interface_name,"id",i->owner->id);
    myexport(i->interface_name,"run",i->owner->run);
    myexport(i->interface_name,"stop",i->owner->stop);
  }
  return i;
}

void delete_Interface(Interface* i){
  free(i);//FIXME: controlar refs
}

void Interface_run(const Interface* i){
  runFn irun;
  
  assert(i!=0 && i->implemented==0);/*if implemented run not allowed to avoid loops*/
  irun = (runFn)myimport(i->interface_name,"run");
  if (irun)
    irun(*(i->owner->id),NULL,NULL);
}

void Interface_stop(const Interface* i){
  stopFn istop;

  assert(i!=0 && i->implemented==0);/*if implemented run not allowed to avoid loops*/
  istop = (stopFn)myimport(i->interface_name,"stop");
  if (istop)
    istop();
}
