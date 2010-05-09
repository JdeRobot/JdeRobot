#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "laser.h"

Laser* new_Laser(const char* interface_name,
		 JDESchema* const supplier){
  Laser* l;
  
  assert(supplier!=0 && supplier->hierarchy!=0);
  l = (Laser*)calloc(1,sizeof(Laser));
  assert(l!=0);
  SUPER(l) = new_JDEInterface(interface_name,supplier);
  assert(SUPER(l) != 0);
  /*interface export*/
  if (JDEHierarchy_myexport(supplier->hierarchy,interface_name,"Laser",l) == 0){
    delete_JDEInterface(SUPER(l));
    free(l);
    return 0;
  }
  /*backwards compatibility exports*/
  myexport(interface_name,"laser",l->laser);
  myexport(interface_name,"clock",&(l->clock));
  myexport(interface_name,"number",&(l->number));
  myexport(interface_name,"resolution",&(l->resolution));
  return l;
}

void delete_Laser(Laser* const self){
  if (self==0)
    return;
  delete_JDEInterface(SUPER(self));
  free(self);/*FIXME: un-export symbols. references?*/
}

/*for old interfaces proxy->refers_to == 0*/
LaserPrx* new_LaserPrx(const char* interface_name,
		       JDESchema* const user){
  LaserPrx* lprx;

  assert(user!=0 && user->hierarchy!=0);
  lprx = (LaserPrx*)calloc(1,sizeof(LaserPrx));
  assert(lprx!=0);
  
  PRX_REFERS_TO(lprx) = (Laser*)JDEHierarchy_myimport(user->hierarchy,interface_name,"Laser");
  if (PRX_REFERS_TO(lprx) == 0){/*we are connecting with an old
				   encoders interface*/
    int *idp;
    JDESchema *supplier;

    idp = (int*)myimport(interface_name,"id");/*we have to get the
						supplier*/
    if (idp != 0){
      JDEInterface *i;

      supplier = get_schema(*idp);
      assert(supplier!=0);
      i = new_JDEInterface(interface_name,supplier);/*pointer stored with myimport*/
      assert(i!=0);
    }else{/*no interface found with this name*/
      free(lprx);
      return 0;
    }
  }
  SUPER(lprx) = new_JDEInterfacePrx(interface_name,user);
  assert(SUPER(lprx)!=0);
  return lprx;
}


void delete_LaserPrx(LaserPrx* const self){
  if (self==0)
    return;
  
  delete_JDEInterfacePrx(SUPER(self));
  free(self);
}

int LaserPrx_run (LaserPrx * const self){
  return JDEInterfacePrx_run(self->super);
}

int LaserPrx_stop (LaserPrx * const self){
  return JDEInterfacePrx_stop(self->super);
}


int* LaserPrx_laser_get(LaserPrx *const self){
  int* laserp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    laserp=PRX_REFERS_TO(self)->laser;
  else/*backwards compatibility*/
    laserp=(int *)myimport(INTERFACEPRX_NAME(self),"laser");
  assert(laserp!=0);
  return laserp;
}


int LaserPrx_number_get(LaserPrx *const self){
  int* numberp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    numberp=&(PRX_REFERS_TO(self)->number);
  else/*backwards compatibility*/
    numberp=(int *)myimport(INTERFACEPRX_NAME(self),"number");
  assert(numberp!=0);
  return *numberp;
}

int LaserPrx_resolution_get(LaserPrx *const self){
  int* resolutionp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    resolutionp=&(PRX_REFERS_TO(self)->resolution);
  else/*backwards compatibility*/
    resolutionp=(int *)myimport(INTERFACEPRX_NAME(self),"resolution");
  assert(resolutionp!=0);
  return *resolutionp;
}

unsigned long int LaserPrx_clock_get(LaserPrx *const self){
  unsigned long int* clockp;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    clockp=&(PRX_REFERS_TO(self)->clock);
  else/*backwards compatibility*/
    clockp=(unsigned long int *)myimport(INTERFACEPRX_NAME(self),"clock");
  assert(clockp!=0);
  return *clockp;
} 

void LaserPrx_laser_set(LaserPrx* const self, int* new_laser){
  int *laser;

  assert(self!=0);
  laser = LaserPrx_laser_get(self);
  memmove(laser,new_laser,sizeof(int)*MAX_LASER);
}

void LaserPrx_number_set(LaserPrx* const self, int new_number){
  int* numberp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    numberp=&(PRX_REFERS_TO(self)->number);
  else/*backwards compatibility*/
    numberp=(int *)myimport(INTERFACEPRX_NAME(self),"number");
  assert(numberp!=0);
  *numberp = new_number;
}

void LaserPrx_resolution_set(LaserPrx* const self, int new_resolution){
  int* resolutionp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    resolutionp=&(PRX_REFERS_TO(self)->resolution);
  else/*backwards compatibility*/
    resolutionp=(int *)myimport(INTERFACEPRX_NAME(self),"resolution");
  assert(resolutionp!=0);
  *resolutionp = new_resolution;
}

void LaserPrx_clock_set(LaserPrx* const self, unsigned long int new_clock){
  unsigned long int* clockp;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    clockp=&(PRX_REFERS_TO(self)->clock);
  else/*backwards compatibility*/
    clockp=(unsigned long int *)myimport(INTERFACEPRX_NAME(self),"clock");
  assert(clockp!=0);
  *clockp = new_clock;
}

/*Macro to define all the attr get/set functions*/
Laser_attr(INTERFACE_ATTR_DEFINITION,Laser)
