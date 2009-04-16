#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "encoders.h"

Encoders* new_Encoders(const char* father,
		       const char* interface_name,
		       JDESchema* owner){
  Interface* i = new_Interface(father,interface_name,owner);
  if (owner != 0){
    i->datap = calloc(1,sizeof(Encoders_data));
    myexport(i->interface_name,"jde_robot",((Encoders_data*)i->datap)->robot);
    myexport(i->interface_name,"clock",&((Encoders_data*)i->datap)->clock);
    myexport(i->interface_name,"cycle",&((Encoders_data*)i->datap)->cycle);
  }
  return (Encoders*)i;
}

void delete_Encoders(Encoders* e){
  if (e->owner != 0)
    free(e->datap);//FIXME:controlar refs
  delete_Interface((Interface*)e);
}

void Encoders_run(Encoders* e){
  Interface_run(e);
}

void Encoders_stop(Encoders* e){
  Interface_stop(e);
}

float* Encoders_robot_get(const Encoders* e){
  float* robot;

  assert(e!=0);
  if (e->owner != 0)
    robot=((Encoders_data*)e->datap)->robot;
  else
    robot=(float *)myimport(e->interface_name,"jde_robot");
    
  return robot;
}

float Encoders_x_get(const Encoders* e){
  float* robot;

  assert(e!=0);
  if (e->owner != 0)
    robot=((Encoders_data*)e->datap)->robot;
  else
    robot=(float *)myimport(e->interface_name,"jde_robot");
    
  return (robot?robot[ROBOT_X]:0.0);
}


float Encoders_y_get(const Encoders* e){
  float* robot;

  assert(e!=0);
  if (e->owner != 0)
    robot=((Encoders_data*)e->datap)->robot;
  else
    robot=(float *)myimport(e->interface_name,"jde_robot");
  return (robot?robot[ROBOT_Y]:0.0);
}

float Encoders_theta_get(const Encoders* e){
  float* robot;

  assert(e!=0);
  if (e->owner != 0)
    robot=((Encoders_data*)e->datap)->robot;
  else
    robot=(float *)myimport(e->interface_name,"jde_robot");
  return (robot?robot[ROBOT_THETA]:0.0);
}

float Encoders_cos_get(const Encoders* e){
  float* robot;

  assert(e!=0);
  if (e->owner != 0)
    robot=((Encoders_data*)e->datap)->robot;
  else
    robot=(float *)myimport(e->interface_name,"jde_robot");
  return (robot?robot[ROBOT_COS]:0.0); 
}

float Encoders_sin_get(const Encoders* e){
  float* robot;

  assert(e!=0);
  if (e->owner != 0)
    robot=((Encoders_data*)e->datap)->robot;
  else
    robot=(float *)myimport(e->interface_name,"jde_robot");
  return (robot?robot[ROBOT_SIN]:0.0);
}

unsigned long int Encoders_clock_get(const Encoders* e){
  unsigned long int* clock;

  assert(e!=0);
  if (e->owner != 0)
    return ((Encoders_data*)e->datap)->clock;
  else{
    clock=(unsigned long int *)myimport(e->interface_name,"clock");
    return (clock?*clock:0);
  }
} 

int Encoders_cycle_get(const Encoders* e){
  int* cycle;
   
  assert(e!=0);
  if (e->owner != 0)
    return ((Encoders_data*)e->datap)->cycle;
  else{
    cycle=(int *)myimport(e->interface_name,"cycle");
    return (cycle?*cycle:0);
  }
}

void Encoders_robot_set(const Encoders* e, const float* new_robot){
  assert(e!=0);
  if (e->owner != 0)
    memmove(((Encoders_data*)e->datap)->robot,new_robot,sizeof(float)*ROBOT_NELEM);
}

void Encoders_x_set(const Encoders* e, const float new_x){
  assert(e!=0);
  if (e->owner != 0)
    ((Encoders_data*)e->datap)->robot[ROBOT_X] = new_x;
}

void Encoders_y_set(const Encoders* e, const float new_y){
  assert(e!=0);
  if (e->owner != 0)
    ((Encoders_data*)e->datap)->robot[ROBOT_Y] = new_y;
}

void Encoders_theta_set(const Encoders* e, const float new_theta){
  assert(e!=0);
  if (e->owner != 0)
    ((Encoders_data*)e->datap)->robot[ROBOT_THETA] = new_theta;
}

void Encoders_cos_set(const Encoders* e, const float new_cos){
  assert(e!=0);
  if (e->owner != 0)
    ((Encoders_data*)e->datap)->robot[ROBOT_COS] = new_cos;
}

void Encoders_sin_set(const Encoders* e, const float new_sin){
  assert(e!=0);
  if (e->owner != 0)
    ((Encoders_data*)e->datap)->robot[ROBOT_SIN] = new_sin;
}

void Encoders_clock_set(const Encoders* e, const unsigned long int new_clock){
  assert(e!=0);
  if (e->owner != 0)
    ((Encoders_data*)e->datap)->clock = new_clock;
}

void Encoders_cycle_set(Encoders* e, const int new_cycle){
  int* cycle;
   
  assert(e!=0);
  if (e->owner != 0)
    ((Encoders_data*)e->datap)->cycle = new_cycle;
  else{
    cycle=(int *)myimport(e->interface_name,"cycle");
    if (cycle)
      *cycle = new_cycle;
  }
}

