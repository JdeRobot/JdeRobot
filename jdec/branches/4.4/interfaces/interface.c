#include "interface.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <jde_private.h>

/*constructor & destructor*/
Interface* new_Interface(const char* father,
			 const char* interface_name,
			 JDESchema* owner){
			 //const int owned){
  JDESchema* fs;
  Interface* i = (Interface*)calloc(1,sizeof(Interface));
  assert(i!=0);
  strncpy(i->interface_name,interface_name,MAX_NAME);
  i->interface_name[MAX_NAME-1] = '\0';
  //i->owned = owned;
  i->refs = 0;
  i->owner = owner;
  if (owner == 0){
    fs = find_schema(father);
    assert(fs!=0);
    i->father_id = *(fs->id);
  }else{
    myexport(i->interface_name,"id",i->owner->id);
    myexport(i->interface_name,"run",i->owner->run);
    myexport(i->interface_name,"stop",i->owner->stop);
  }
  return i;
}

void delete_Interface(Interface* i){
  free(i);//FIXME: controlar refs
}

void Interface_run(Interface* i){
  runFn irun;

  irun = (runFn)myimport(i->interface_name,"run");
  if (irun){
    irun(i->father_id,NULL,NULL);
    i->refs++;
  }
}

void Interface_stop(Interface* i){
  stopFn istop;

  istop = (stopFn)myimport(i->interface_name,"stop");
  if (istop)
    istop();
}
