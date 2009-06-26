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

void delete_Laser(Laser* const this){
  if (this==0)
    return;
  free(this);
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

void delete_LaserPrx(LaserPrx* const this){
  if (this==0)
    return;
  
  if (PRX_REFERS_TO(this)){
    if (JDEInterface_refcount(PRX_REFERS_TO(SUPER(this)))==1){/*last reference*/
      /*FIXME: delete exported symbols*/
      delete_Laser(PRX_REFERS_TO(this));
      PRX_REFERS_TO(SUPER(this)) = 0;/*JDEInterface has been destroyed*/
    }
  }
  delete_JDEInterfacePrx(SUPER(this));
  free(this);
}

int* LaserPrx_laser_get(const LaserPrx* this){
  int* laserp = 0;

  assert(this!=0);
  if (PRX_REFERS_TO(this))
    laserp=PRX_REFERS_TO(this)->laser;
  else/*backwards compatibility*/
    laserp=(int *)myimport(PRX_REFERS_TO(SUPER(this))->interface_name,"laser");
    
  return laserp;
}


int LaserPrx_number_get(const LaserPrx* this){
  int* numberp = 0;

  assert(this!=0);
  if (PRX_REFERS_TO(this))
    numberp=&(PRX_REFERS_TO(this)->number);
  else/*backwards compatibility*/
    numberp=(int *)myimport(PRX_REFERS_TO(SUPER(this))->interface_name,"number");
      
  return (numberp?*numberp:0);
}

int LaserPrx_resolution_get(const LaserPrx* this){
  int* resolutionp = 0;

  assert(this!=0);
  if (PRX_REFERS_TO(this))
    resolutionp=&(PRX_REFERS_TO(this)->resolution);
  else/*backwards compatibility*/
    resolutionp=(int *)myimport(PRX_REFERS_TO(SUPER(this))->interface_name,"resolution");
      
  return (resolutionp?*resolutionp:0);
}

unsigned long int LaserPrx_clock_get(const LaserPrx* this){
  unsigned long int* clockp;

  assert(this!=0);
  if (PRX_REFERS_TO(this))
    clockp=&(PRX_REFERS_TO(this)->clock);
  else/*backwards compatibility*/
    clockp=(unsigned long int *)myimport(PRX_REFERS_TO(SUPER(this))->interface_name,"clock");
      
  return (clockp?*clockp:0);
} 

void LaserPrx_laser_set(LaserPrx* const this, const int* new_laser){
  assert(this!=0);
  if (PRX_REFERS_TO(this))
    memmove(PRX_REFERS_TO(this)->laser,new_laser,sizeof(int)*MAX_LASER);
}

void LaserPrx_number_set(LaserPrx* const this, const int new_number){
  assert(this!=0);
  if (PRX_REFERS_TO(this))
    PRX_REFERS_TO(this)->number = new_number;
}

void LaserPrx_resolution_set(LaserPrx* const this, const int new_resolution){
  assert(this!=0);
  if (PRX_REFERS_TO(this))
    PRX_REFERS_TO(this)->resolution = new_resolution;
}

void LaserPrx_clock_set(LaserPrx* const this, const unsigned long int new_clock){
  assert(this!=0);
  if (PRX_REFERS_TO(this))
    PRX_REFERS_TO(this)->clock = new_clock;
}

