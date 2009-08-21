#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "encoders.h"

Encoders* new_Encoders(const char* interface_name,
		       JDESchema* const supplier){
  Encoders* e;
  
  assert(supplier!=0);
  e = (Encoders*)calloc(1,sizeof(Encoders));
  assert(e!=0);
  SUPER(e) = new_JDEInterface(interface_name,supplier);
  assert(SUPER(e) != 0);
  return e;
}

EncodersPrx* new_EncodersPrx(const char* interface_name,
			     JDESchema* const user,
			     Encoders* const refers_to){
  EncodersPrx* eprx;

  assert(user!=0);
  eprx = (EncodersPrx*)calloc(1,sizeof(EncodersPrx));
  assert(eprx!=0);
  if (refers_to == 0){
    PRX_REFERS_TO(eprx) = (Encoders*)myimport(interface_name,"Encoders");
    SUPER(eprx) = new_JDEInterfacePrx(interface_name,user,0);
  }else{
    PRX_REFERS_TO(eprx) = refers_to;
    SUPER(eprx) = new_JDEInterfacePrx(interface_name,
				      user,
				      SUPER(refers_to));
    myexport(interface_name,"Encoders",refers_to);
    /*backwards compatibility*/
    myexport(interface_name,"jde_robot",refers_to->robot);
    myexport(interface_name,"clock",&(refers_to->clock));
    /* myexport(interface_name,"cycle",&(refers_to->cycle)); */
  }
  return eprx;
}

void delete_Encoders(Encoders* const self){
  if (self==0)
    return;
  free(self);
}

void delete_EncodersPrx(EncodersPrx* const self){
  if (self==0)
    return;
  
  if (PRX_REFERS_TO(self)){
    if (JDEInterface_refcount(PRX_REFERS_TO(SUPER(self)))==1){/*last reference*/
      /*FIXME: delete exported symbols*/
      delete_Encoders(PRX_REFERS_TO(self));
      PRX_REFERS_TO(SUPER(self)) = 0;/*JDEInterface has been destroyed*/
    }
  }
  delete_JDEInterfacePrx(SUPER(self));
  free(self);
}

float* EncodersPrx_robot_get(const EncodersPrx* self){
  float* robot = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    robot=PRX_REFERS_TO(self)->robot;
  else/*backwards compatibility*/
    robot=(float *)myimport(PRX_REFERS_TO(SUPER(self))->interface_name,"jde_robot");
    
  return robot;
}

float EncodersPrx_x_get(const EncodersPrx* self){
  float* robot;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    robot=PRX_REFERS_TO(self)->robot;
  else/*backwards compatibility*/
    robot=(float *)myimport(PRX_REFERS_TO(SUPER(self))->interface_name,"jde_robot");
      
  return (robot?robot[ROBOT_X]:0.0);
}


float EncodersPrx_y_get(const EncodersPrx* self){
  float* robot;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    robot=PRX_REFERS_TO(self)->robot;
  else/*backwards compatibility*/
    robot=(float *)myimport(PRX_REFERS_TO(SUPER(self))->interface_name,"jde_robot");
  return (robot?robot[ROBOT_Y]:0.0);
}

float EncodersPrx_theta_get(const EncodersPrx* self){
  float* robot;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    robot=PRX_REFERS_TO(self)->robot;
  else/*backwards compatibility*/
    robot=(float *)myimport(PRX_REFERS_TO(SUPER(self))->interface_name,"jde_robot");
  return (robot?robot[ROBOT_THETA]:0.0);
}

float EncodersPrx_cos_get(const EncodersPrx* self){
  float* robot;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    robot=PRX_REFERS_TO(self)->robot;
  else/*backwards compatibility*/
    robot=(float *)myimport(PRX_REFERS_TO(SUPER(self))->interface_name,"jde_robot");
  return (robot?robot[ROBOT_COS]:0.0); 
}

float EncodersPrx_sin_get(const EncodersPrx* self){
  float* robot;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    robot=PRX_REFERS_TO(self)->robot;
  else/*backwards compatibility*/
    robot=(float *)myimport(PRX_REFERS_TO(SUPER(self))->interface_name,"jde_robot");
  return (robot?robot[ROBOT_SIN]:0.0);
}

unsigned long int EncodersPrx_clock_get(const EncodersPrx* self){
  unsigned long int* clockp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    clockp=&(PRX_REFERS_TO(self)->clock);
  else/*backwards compatibility*/
    clockp=(unsigned long int *)myimport(PRX_REFERS_TO(SUPER(self))->interface_name,"clock");
  return (clockp?*clockp:0);
} 

void EncodersPrx_robot_set(EncodersPrx* const self, float* new_robot){
  assert(self!=0);
  if (PRX_REFERS_TO(self))
    memmove(PRX_REFERS_TO(self)->robot,new_robot,sizeof(float)*ROBOT_NELEM);
}

void EncodersPrx_clock_set(EncodersPrx* const self, unsigned long int new_clock){
  assert(self!=0);
  if (PRX_REFERS_TO(self))
    PRX_REFERS_TO(self)->clock = new_clock;
}

