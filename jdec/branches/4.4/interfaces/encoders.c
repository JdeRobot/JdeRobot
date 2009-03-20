#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "encoders.h"

Encoders* new_Encoders(const char* interface_name){
  Encoders* e = (Encoders*)calloc(1,sizeof(Encoders));
  assert(e!=0);
  strncpy(e->interface_name,interface_name,MAX_NAME);
  e->interface_name[MAX_NAME-1] = '\0';
  return e;
}

void delete_Encoders(Encoders* e){
  free(e);
}

float Encoders_x_get(const Encoders* e){
  float* data;

  assert(e!=0);
  data=(float *)myimport(e->interface_name,"jde_robot");
  return (data?data[0]:0.0);
}


float Encoders_y_get(const Encoders* e){
  float* data;

  assert(e!=0);
  data=(float *)myimport(e->interface_name,"jde_robot");
  return (data?data[1]:0.0);
}

float Encoders_theta_get(const Encoders* e){
  float* data;

  assert(e!=0);
  data=(float *)myimport(e->interface_name,"jde_robot");
  return (data?data[2]:0.0);
}

float Encoders_cos_get(const Encoders* e){
  float* data;

  assert(e!=0);
  data=(float *)myimport(e->interface_name,"jde_robot");
  return (data?data[3]:0.0);
}

float Encoders_sin_get(const Encoders* e){
  float* data;

  assert(e!=0);
  data=(float *)myimport(e->interface_name,"jde_robot");
  return (data?data[4]:0.0);
}

unsigned long int Encoders_clock_get(const Encoders* e){
  unsigned long int* data;

  assert(e!=0);
  data=(unsigned long int *)myimport(e->interface_name,"clock");
  return (data?*data:0);
} 

int Encoders_cycle_get(const Encoders* e){
  int* data;

  assert(e!=0);
  data=(int *)myimport(e->interface_name,"cycle");
  return (data?*data:0);
}

void Encoders_cycle_set(Encoders* e, const int cycle){
  int* data;

  assert(e!=0);
  data=(int *)myimport(e->interface_name,"cycle");
  if (data)
    *data = cycle;
  e->cycle = cycle;
}

