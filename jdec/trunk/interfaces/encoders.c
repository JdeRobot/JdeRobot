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

void delete_Encoders(Encoders* const this){
  if (this==0)
    return;
  free(this);
}

void delete_EncodersPrx(EncodersPrx* const this){
  if (this==0)
    return;
  
  if (PRX_REFERS_TO(this)){
    if (JDEInterface_refcount(PRX_REFERS_TO(SUPER(this)))==1){/*last reference*/
      /*FIXME: delete exported symbols*/
      delete_Encoders(PRX_REFERS_TO(this));
      PRX_REFERS_TO(SUPER(this)) = 0;/*JDEInterface has been destroyed*/
    }
  }
  delete_JDEInterfacePrx(SUPER(this));
  free(this);
}

float* EncodersPrx_robot_get(const EncodersPrx* this){
  float* robot = 0;

  assert(this!=0);
  if (PRX_REFERS_TO(this))
    robot=PRX_REFERS_TO(this)->robot;
  else/*backwards compatibility*/
    robot=(float *)myimport(PRX_REFERS_TO(SUPER(this))->interface_name,"jde_robot");
    
  return robot;
}

float EncodersPrx_x_get(const EncodersPrx* this){
  float* robot;

  assert(this!=0);
  if (PRX_REFERS_TO(this))
    robot=PRX_REFERS_TO(this)->robot;
  else/*backwards compatibility*/
    robot=(float *)myimport(PRX_REFERS_TO(SUPER(this))->interface_name,"jde_robot");
      
  return (robot?robot[ROBOT_X]:0.0);
}


float EncodersPrx_y_get(const EncodersPrx* this){
  float* robot;

  assert(this!=0);
  if (PRX_REFERS_TO(this))
    robot=PRX_REFERS_TO(this)->robot;
  else/*backwards compatibility*/
    robot=(float *)myimport(PRX_REFERS_TO(SUPER(this))->interface_name,"jde_robot");
  return (robot?robot[ROBOT_Y]:0.0);
}

float EncodersPrx_theta_get(const EncodersPrx* this){
  float* robot;

  assert(this!=0);
  if (PRX_REFERS_TO(this))
    robot=PRX_REFERS_TO(this)->robot;
  else/*backwards compatibility*/
    robot=(float *)myimport(PRX_REFERS_TO(SUPER(this))->interface_name,"jde_robot");
  return (robot?robot[ROBOT_THETA]:0.0);
}

float EncodersPrx_cos_get(const EncodersPrx* this){
  float* robot;

  assert(this!=0);
  if (PRX_REFERS_TO(this))
    robot=PRX_REFERS_TO(this)->robot;
  else/*backwards compatibility*/
    robot=(float *)myimport(PRX_REFERS_TO(SUPER(this))->interface_name,"jde_robot");
  return (robot?robot[ROBOT_COS]:0.0); 
}

float EncodersPrx_sin_get(const EncodersPrx* this){
  float* robot;

  assert(this!=0);
  if (PRX_REFERS_TO(this))
    robot=PRX_REFERS_TO(this)->robot;
  else/*backwards compatibility*/
    robot=(float *)myimport(PRX_REFERS_TO(SUPER(this))->interface_name,"jde_robot");
  return (robot?robot[ROBOT_SIN]:0.0);
}

unsigned long int EncodersPrx_clock_get(const EncodersPrx* this){
  unsigned long int* clockp = 0;

  assert(this!=0);
  if (PRX_REFERS_TO(this))
    clockp=&(PRX_REFERS_TO(this)->clock);
  else/*backwards compatibility*/
    clockp=(unsigned long int *)myimport(PRX_REFERS_TO(SUPER(this))->interface_name,"clock");
  return (clockp?*clockp:0);
} 

/* int EncodersPrx_cycle_get(const EncodersPrx* this){ */
/*   int* cyclep = 0; */
   
/*   assert(this!=0); */
/*   if (PRX_REFERS_TO(this)) */
/*     cyclep=PRX_REFERS_TO(this)->cycle; */
/*   else/\*backwards compatibility*\/ */
/*     cyclep=(int *)myimport(PRX_REFERS_TO(SUPER(this))->interface_name,"cycle"); */
/*   return (cyclep?*cyclep:0); */
/* } */

void EncodersPrx_robot_set(EncodersPrx* const this, const float* new_robot){
  assert(this!=0);
  if (PRX_REFERS_TO(this))
    memmove(PRX_REFERS_TO(this)->robot,new_robot,sizeof(float)*ROBOT_NELEM);
}

void EncodersPrx_x_set(EncodersPrx* const this, const float new_x){
  assert(this!=0);
  if (PRX_REFERS_TO(this))
    PRX_REFERS_TO(this)->robot[ROBOT_X] = new_x;
}

void EncodersPrx_y_set(EncodersPrx* const this, const float new_y){
  assert(this!=0);
  if (PRX_REFERS_TO(this))
    PRX_REFERS_TO(this)->robot[ROBOT_Y] = new_y;
}

void EncodersPrx_theta_set(EncodersPrx* const this, const float new_theta){
  assert(this!=0);
  if (PRX_REFERS_TO(this))
    PRX_REFERS_TO(this)->robot[ROBOT_THETA] = new_theta;
}

void EncodersPrx_cos_set(EncodersPrx* const this, const float new_cos){
  assert(this!=0);
  if (PRX_REFERS_TO(this))
    PRX_REFERS_TO(this)->robot[ROBOT_COS] = new_cos;
}

void EncodersPrx_sin_set(EncodersPrx* const this, const float new_sin){
  assert(this!=0);
  if (PRX_REFERS_TO(this))
    PRX_REFERS_TO(this)->robot[ROBOT_SIN] = new_sin;
}

void EncodersPrx_clock_set(EncodersPrx* const this, const unsigned long int new_clock){
  assert(this!=0);
  if (PRX_REFERS_TO(this))
    PRX_REFERS_TO(this)->clock = new_clock;
}

/* void EncodersPrx_cycle_set(EncodersPrx* const this, const int new_cycle){ */
/*   int* cyclep = 0; */
   
/*   assert(this!=0); */
/*   if (PRX_REFERS_TO(this)) */
/*     cyclep = PRX_REFERS_TO(this)->cycle; */
/*   else */
/*     cyclep=(int *)myimport(PRX_REFERS_TO(SUPER(this))->interface_name,"cycle"); */
/*   if (cyclep) */
/*     *cyclep = new_cycle; */
/* } */

