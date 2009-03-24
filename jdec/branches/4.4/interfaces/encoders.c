#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "encoders.h"

Encoders* new_Encoders(const char* father,
		       const char* interface_name,
		       const int owned){
  Interface* i = new_Interface(father,interface_name,owned);
  if (owned)
    i->datap = calloc(1,sizeof(Encoders_data));
  return (Encoders*)i;
}

void delete_Encoders(Encoders* e){
  if (e->owned)
    free(e->datap);
  delete_Interface((Interface*)e);
}

float Encoders_x_get(const Encoders* e){
  float* datap;

  assert(e!=0);
  if (e->owned)
    return ((Encoders_data*)e->datap)->x;
  else{
    datap=(float *)myimport(e->interface_name,"jde_robot");
    return (datap?datap[0]:0.0);
  }
}


float Encoders_y_get(const Encoders* e){
  float* datap;

  assert(e!=0);
  if (e->owned)
    return ((Encoders_data*)e->datap)->y;
  else{
    datap=(float *)myimport(e->interface_name,"jde_robot");
    return (datap?datap[1]:0.0);
  }
}

float Encoders_theta_get(const Encoders* e){
  float* datap;

  assert(e!=0);
  if (e->owned)
    return ((Encoders_data*)e->datap)->theta;
  else{
    datap=(float *)myimport(e->interface_name,"jde_robot");
    return (datap?datap[2]:0.0);
  }
}

float Encoders_cos_get(const Encoders* e){
  float* datap;

  assert(e!=0);
  if (e->owned)
    return ((Encoders_data*)e->datap)->cos;
  else{
    datap=(float *)myimport(e->interface_name,"jde_robot");
    return (datap?datap[3]:0.0);
  }  
}

float Encoders_sin_get(const Encoders* e){
float* datap;

  assert(e!=0);
  if (e->owned)
    return ((Encoders_data*)e->datap)->sin;
  else{
    datap=(float *)myimport(e->interface_name,"jde_robot");
    return (datap?datap[4]:0.0);
  }
}

unsigned long int Encoders_clock_get(const Encoders* e){
  unsigned long int* datap;

  assert(e!=0);
  if (e->owned)
    return ((Encoders_data*)e->datap)->clock;
  else{
    datap=(unsigned long int *)myimport(e->interface_name,"clock");
    return (datap?*datap:0);
  }
} 

int Encoders_cycle_get(const Encoders* e){
  int* datap;
   
  assert(e!=0);
  if (e->owned)
    return ((Encoders_data*)e->datap)->cycle;
  else{
    datap=(int *)myimport(e->interface_name,"cycle");
    return (datap?*datap:0);
  }
}

void Encoders_x_set(const Encoders* e, const float new_x){
  assert(e!=0);
  if (e->owned)
    ((Encoders_data*)e->datap)->x = new_x;
}

void Encoders_y_set(const Encoders* e, const float new_y){
  assert(e!=0);
  if (e->owned)
    ((Encoders_data*)e->datap)->y = new_y;
}

void Encoders_theta_set(const Encoders* e, const float new_theta){
  assert(e!=0);
  if (e->owned)
    ((Encoders_data*)e->datap)->theta = new_theta;
}

void Encoders_cos_set(const Encoders* e, const float new_cos){
  assert(e!=0);
  if (e->owned)
    ((Encoders_data*)e->datap)->cos = new_cos;
}

void Encoders_sin_set(const Encoders* e, const float new_sin){
  assert(e!=0);
  if (e->owned)
    ((Encoders_data*)e->datap)->sin = new_sin;
}

void Encoders_clock_set(const Encoders* e, const unsigned long int new_clock){
  assert(e!=0);
  if (e->owned)
    ((Encoders_data*)e->datap)->clock = new_clock;
}

void Encoders_cycle_set(Encoders* e, const int new_cycle){
  int* datap;
   
  assert(e!=0);
  if (e->owned)
    ((Encoders_data*)e->datap)->cycle = new_cycle;
  else{
    datap=(int *)myimport(e->interface_name,"cycle");
    if (datap)
      *datap = new_cycle;
  }
}

