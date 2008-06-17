/*Example schema written in C*/
#include <random_iface.h>
#include <loader.h>
#include <schema.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  Random* interface;
}Pdata;

/*Random callbacks*/
float get_random(Random* const i) {
  fprintf(stderr,"call get_random() inside sB\n");
  return (float)rand();
}

void set_seed(Random* const i, const int seed) {
  fprintf(stderr,"call set_seed(%d) inside sB\n",seed);
  srand(seed);
}

/*schema API*/
int init(Schema* const s) {
  fprintf(stderr,"executing init on schema sB(%d)...\n",s->sid);
  return 0;
}

void* cast(Schema* const s,
	   const char* interface_name) {
  Pdata* my_pdata = (Pdata*)s->private_data;

  if (my_pdata->interface == 0) {
    fprintf(stderr,"call cast() inside sB(%d)\n",s->sid);
    my_pdata->interface = new_Random(&get_random,0,
				     &set_seed,0);
  }
  return my_pdata->interface;
}

/*sB ctor*/
Schema* new_sB(Schema_reg* sr, const int sid, const int father_sid) {
  Pdata* my_pdata;

  my_pdata = (Pdata*)malloc(sizeof(Pdata));
  my_pdata->interface = 0;

  return new_Schema(sid,father_sid,
		    &init,0,
		    &cast,0,
		    my_pdata);
}

/*sB dtor*/
void delete_sB(Schema_reg* sr, Schema* s) {
  if (s) {
    Pdata* my_pdata = (Pdata*)s->private_data;
    
    delete_Random(my_pdata->interface);
    free(my_pdata);
    delete_Schema(s);
  }
}

/*register schema definition*/
ADD_SCHEMA_REG("sB",RANDOM_IFACE_NAME,&new_sB,&delete_sB,0)
