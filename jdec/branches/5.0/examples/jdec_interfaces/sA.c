/*Example schema written in C*/
#include <random_iface.h>
#include <loader.h>
#include <schema.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int seed;
  Schema_reg* child_sr;
  Schema* child;
  Random* child_interface;
}Pdata;

/*schema API*/
int init(Schema* const s) {
  Pdata* my_pdata = (Pdata*)s->private_data;

  fprintf(stderr,"executing init on schema %d...\n",s->sid);

  if (my_pdata->child == 0) {
    if ((my_pdata->child_sr = search_schema_reg(RANDOM_IFACE_NAME)) == 0) {
      printf("can't find child on schema %d...\n",s->sid);
      return 1;
    }
    my_pdata->child = Schema_reg_instance(my_pdata->child_sr,
					  s->sid*10,s->sid);
    my_pdata->child_interface = cast_Random(my_pdata->child);
    fprintf(stderr,"\tset_seed(%d) on child\n",my_pdata->seed);
    Random_set_seed(my_pdata->child_interface,my_pdata->seed);
  }

  fprintf(stderr,"\tget_random() from %s: %f\n",
	 RANDOM_IFACE_NAME,
	 Random_get_random(my_pdata->child_interface));
  return 0;
}

void* cast(Schema* const s,
	   const char* interface_name) {
  return 0;/*no interface implemented*/
}

/*sA ctor*/
Schema* new_sA(Schema_reg* sr, const int sid, const int father_sid) {
  Pdata* my_pdata;

  my_pdata = (Pdata*)malloc(sizeof(Pdata));
  my_pdata->seed = 46486422;
  my_pdata->child = 0;
  my_pdata->child_sr = 0;
  my_pdata->child_interface = 0;

  return new_Schema(sid,father_sid,
		    &init,0,
		    &cast,0,
		    my_pdata);
}

/*sA dtor*/
void delete_sA(Schema_reg* sr, Schema* s) {
  if (s) {
    Pdata* my_pdata = (Pdata*)s->private_data;
    
    if (my_pdata->child)
      Schema_reg_delete_instance(my_pdata->child_sr,
				 my_pdata->child);
    if (my_pdata->child_sr)
      delete_Schema_reg(my_pdata->child_sr);
    free(my_pdata);
    delete_Schema(s);
  }
}

/*register schema definition*/
ADD_SCHEMA_REG("sA","null",&new_sA,&delete_sA,0)
