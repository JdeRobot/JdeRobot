/*Example schema written in C*/
#include <schema.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int seed;
}Pdata;

/*schema API*/
int init(Schema* const s) {
  Pdata* my_pdata = (Pdata*)s->private_data;

  fprintf(stderr,"executing init on schema %d...\n",s->sid);
  return 0;
}

int iteration(Schema* const s) {
  static int i = 0;
  Pdata* my_pdata = (Pdata*)s->private_data;

  fprintf(stderr,"executing iteration %d on schema %d...\n",i++,s->sid);
  return 0;
}

void* cast(Schema* const s,
	   const char* interface_name) {
  return 0;/*no interface implemented*/
}

/*sA ctor*/
Schema* new_sA(SFactory* sf, const int sid) {
  Pdata* my_pdata;

  my_pdata = (Pdata*)malloc(sizeof(Pdata));
  my_pdata->seed = 46486422;

  return new_Schema(sid,
		    &init,0,
		    &iteration,0,
		    &cast,0,
		    my_pdata);
}

/*sA dtor*/
void delete_sA(SFactory* sf, Schema* s) {
  if (s) {
    Pdata* my_pdata = (Pdata*)s->private_data;
    
    free(my_pdata);
    delete_Schema(s);
  }
}

/*register schema definition*/
ADD_SFACTORY("sA","null",&new_sA,&delete_sA,0)
