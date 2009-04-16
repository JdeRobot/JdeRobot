#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "laser.h"

Laser* new_Laser(const char* father,
		       const char* interface_name,
		       JDESchema* owner){
  Interface* i = new_Interface(father,interface_name,owner);
  if (owner != 0){
    i->datap = calloc(1,sizeof(Laser_data));
    myexport(i->interface_name,"laser",((Laser_data*)i->datap)->laser);
    myexport(i->interface_name,"clock",((Laser_data*)i->datap)->clock);
    myexport(i->interface_name,"number",((Laser_data*)i->datap)->number);
    myexport(i->interface_name,"resolution",((Laser_data*)i->datap)->resolution);
  }
  return (Laser*)i;
}

void delete_Laser(Laser* l){
  if (l->owner != 0)
    free(l->datap);//FIXME: controlar refs
  delete_Interface((Interface*)l);
}

void Laser_run(Laser* l){
  Interface_run(l);
}

void Laser_stop(Laser* l){
  Interface_stop(l);
}

int* Laser_laser_get(const Laser* l){
  int* datap;

  assert(l!=0);
  if (l->owner != 0)
    return ((Laser_data*)l->datap)->laser;
  else{
    datap=(int *)myimport(l->interface_name,"laser");
    return datap;
  }
}


int Laser_number_get(const Laser* l){
  int* datap;

  assert(l!=0);
  if (l->owner != 0)
    return ((Laser_data*)l->datap)->number;
  else{
    datap=(int *)myimport(l->interface_name,"number");
    return (datap?*datap:0);
  }
}

int Laser_resolution_get(const Laser* l){
  int* datap;

  assert(l!=0);
  if (l->owner != 0)
    return ((Laser_data*)l->datap)->resolution;
  else{
    datap=(int *)myimport(l->interface_name,"resolution");
    return (datap?*datap:0);
  }
}

unsigned long int Laser_clock_get(const Laser* l){
  unsigned long int* datap;

  assert(l!=0);
  if (l->owner != 0)
    return ((Laser_data*)l->datap)->clock;
  else{
    datap=(unsigned long int *)myimport(l->interface_name,"clock");
    return (datap?*datap:0);
  }
} 

int Laser_cycle_get(const Laser* l){
  int* datap;
   
  assert(l!=0);
  if (l->owner != 0)
    return ((Laser_data*)l->datap)->cycle;
  else{
    datap=(int *)myimport(l->interface_name,"cycle");
    return (datap?*datap:0);
  }
}

void Laser_laser_set(const Laser* l, const int* new_laser){
  assert(l!=0);
  if (l->owner != 0)
    memmove(((Laser_data*)l->datap)->laser,new_laser,sizeof(int)*MAX_LASER);
}

void Laser_number_set(const Laser* l, const int new_number){
  assert(l!=0);
  if (l->owner != 0)
    ((Laser_data*)l->datap)->number = new_number;
}

void Laser_resolution_set(const Laser* l, const int new_resolution){
  assert(l!=0);
  if (l->owner != 0)
    ((Laser_data*)l->datap)->resolution = new_resolution;
}

void Laser_clock_set(const Laser* l, const unsigned long int new_clock){
  assert(l!=0);
  if (l->owner != 0)
    ((Laser_data*)l->datap)->clock = new_clock;
}

void Laser_cycle_set(Laser* l, const int new_cycle){
  int* datap;
   
  assert(l!=0);
  if (l->owner != 0)
    ((Laser_data*)l->datap)->cycle = new_cycle;
  else{
    datap=(int *)myimport(l->interface_name,"cycle");
    if (datap)
      *datap = new_cycle;
  }
}

