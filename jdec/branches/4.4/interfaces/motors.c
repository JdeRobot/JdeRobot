#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "motors.h"


Motors* new_Motors(const char* interface_name){
  Motors* m = (Motors*)calloc(1,sizeof(Motors));
  assert(m!=0);
  strncpy(m->interface_name,interface_name,MAX_NAME);
  m->interface_name[MAX_NAME-1] = '\0';
  return m;
}

void delete_Motors(Motors* m){
  free(m);
}

float Motors_v_get(const Motors* m){
  float* motors_v;

  assert(m!=0);
  motors_v=(float *)myimport(m->interface_name,"v");
  return (motors_v?*motors_v:0.0);
}

float Motors_w_get(const Motors* m){
  float* motors_w;

  assert(m!=0);
  motors_w=(float *)myimport(m->interface_name,"w");
  return (motors_w?*motors_w:0);
}

int Motors_cycle_get(const Motors* m){
  int* motors_cycle;
    
  assert(m!=0);
  motors_cycle=(int *)myimport(m->interface_name,"cycle");
  return (motors_cycle?*motors_cycle:0.0);
}

void Motors_v_set(Motors* m, const float v){
  float* motors_v;

  assert(m!=0);
  motors_v=(float *)myimport(m->interface_name,"v");
  if (motors_v)
    *motors_v = v;
  m->v = v;
}

void Motors_w_set(Motors* m, const float w){
  float* motors_w;

  assert(m!=0);
  motors_w=(float *)myimport(m->interface_name,"w");
  if (motors_w)
    *motors_w = w;
  m->w = w;
}

void Motors_cycle_set(Motors* m, const int cycle){
  int* motors_cycle;

  assert(m!=0);
  motors_cycle=(int *)myimport(m->interface_name,"cycle");
  if (motors_cycle)
    *motors_cycle = cycle;
  m->cycle = cycle;
}
