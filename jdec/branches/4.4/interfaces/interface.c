#include "interface.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <jde_private.h>

/*constructor & destructor*/
Interface* new_Interface(const char* father,
			 const char* interface_name,
			 const int owned){
  JDESchema* fs;
  Interface* i = (Interface*)calloc(1,sizeof(Interface));
  assert(i!=0);
  i->owned = owned;
  fs = find_schema(father);
  if (fs)
    i->father_id = *(find_schema(father)->id);
  else
    i->father_id = SHELLHUMAN;
  strncpy(i->interface_name,interface_name,MAX_NAME);
  i->interface_name[MAX_NAME-1] = '\0';
  return i;
}

void delete_Interface(Interface* i){
  free(i);
}

void Interface_run(Interface* i){
  runFn irun;

  irun = (runFn)myimport(i->interface_name,"run");
  if (irun)
    irun(i->father_id,NULL,NULL);
}

void Interface_stop(Interface* i){
  stopFn istop;

  istop = (stopFn)myimport(i->interface_name,"stop");
  if (istop)
    istop();
}
