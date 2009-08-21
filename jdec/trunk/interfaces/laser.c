#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "laser.h"

Laser* new_Laser(const char* interface_name,
		 JDESchema* const supplier){
  Laser* l;
  
  assert(supplier!=0);
  l = (Laser*)calloc(1,sizeof(Laser));
  assert(l!=0);
  SUPER(l) = new_JDEInterface(interface_name,supplier);
  assert(SUPER(l) != 0);
  return l;
}

void delete_Laser(Laser* const self){
  if (self==0)
    return;
  free(self);
}

LaserPrx* new_LaserPrx(const char* interface_name,
		       JDESchema* const user,
		       Laser* const refers_to){
  LaserPrx* lprx;

  assert(user!=0);
  lprx = (LaserPrx*)calloc(1,sizeof(LaserPrx));
  assert(lprx!=0);
  if (refers_to == 0){
    PRX_REFERS_TO(lprx) = (Laser*)myimport(interface_name,"Laser");
    SUPER(lprx) = new_JDEInterfacePrx(interface_name,user,0);
  }else{
    PRX_REFERS_TO(lprx) = refers_to;
    SUPER(lprx) = new_JDEInterfacePrx(interface_name,
				      user,
				      SUPER(refers_to));
    myexport(interface_name,"Laser",refers_to);
    /*backwards compatibility*/
    myexport(interface_name,"laser",refers_to->laser);
    myexport(interface_name,"clock",&(refers_to->clock));
    myexport(interface_name,"number",&(refers_to->number));
    myexport(interface_name,"resolution",&(refers_to->resolution));
  }
  return lprx;

}

void delete_LaserPrx(LaserPrx* const self){
  if (self==0)
    return;
  
  if (PRX_REFERS_TO(self)){
    if (JDEInterface_refcount(PRX_REFERS_TO(SUPER(self)))==1){/*last reference*/
      /*FIXME: delete exported symbols*/
      delete_Laser(PRX_REFERS_TO(self));
      PRX_REFERS_TO(SUPER(self)) = 0;/*JDEInterface has been destroyed*/
    }
  }
  delete_JDEInterfacePrx(SUPER(self));
  free(self);
}

int* LaserPrx_laser_get(const LaserPrx* self){
  int* laserp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    laserp=PRX_REFERS_TO(self)->laser;
  else/*backwards compatibility*/
    laserp=(int *)myimport(PRX_REFERS_TO(SUPER(self))->interface_name,"laser");
    
  return laserp;
}


int LaserPrx_number_get(const LaserPrx* self){
  int* numberp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    numberp=&(PRX_REFERS_TO(self)->number);
  else/*backwards compatibility*/
    numberp=(int *)myimport(PRX_REFERS_TO(SUPER(self))->interface_name,"number");
      
  return (numberp?*numberp:0);
}

int LaserPrx_resolution_get(const LaserPrx* self){
  int* resolutionp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    resolutionp=&(PRX_REFERS_TO(self)->resolution);
  else/*backwards compatibility*/
    resolutionp=(int *)myimport(PRX_REFERS_TO(SUPER(self))->interface_name,"resolution");
      
  return (resolutionp?*resolutionp:0);
}

unsigned long int LaserPrx_clock_get(const LaserPrx* self){
  unsigned long int* clockp;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    clockp=&(PRX_REFERS_TO(self)->clock);
  else/*backwards compatibility*/
    clockp=(unsigned long int *)myimport(PRX_REFERS_TO(SUPER(self))->interface_name,"clock");
      
  return (clockp?*clockp:0);
} 

void LaserPrx_laser_set(LaserPrx* const self, int* new_laser){
  assert(self!=0);
  if (PRX_REFERS_TO(self))
    memmove(PRX_REFERS_TO(self)->laser,new_laser,sizeof(int)*MAX_LASER);
}

void LaserPrx_number_set(LaserPrx* const self, int new_number){
  assert(self!=0);
  if (PRX_REFERS_TO(self))
    PRX_REFERS_TO(self)->number = new_number;
}

void LaserPrx_resolution_set(LaserPrx* const self, int new_resolution){
  assert(self!=0);
  if (PRX_REFERS_TO(self))
    PRX_REFERS_TO(self)->resolution = new_resolution;
}

void LaserPrx_clock_set(LaserPrx* const self, unsigned long int new_clock){
  assert(self!=0);
  if (PRX_REFERS_TO(self))
    PRX_REFERS_TO(self)->clock = new_clock;
}

