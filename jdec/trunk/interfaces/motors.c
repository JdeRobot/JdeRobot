#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "motors.h"

Motors* new_Motors(const char* interface_name,
		   JDESchema* const supplier){
  Motors* m;
  
  assert(supplier!=0);
  m = (Motors*)calloc(1,sizeof(Motors));
  assert(m!=0);
  SUPER(m) = new_JDEInterface(interface_name,supplier);
  assert(SUPER(m) != 0);
  return m;
}

void delete_Motors(Motors* const self){
  if (self==0)
    return;
  free(self);
}

MotorsPrx* new_MotorsPrx(const char* interface_name,
			 JDESchema* const user,
			 Motors* const refers_to){
  MotorsPrx* mprx;

  assert(user!=0);
  mprx = (MotorsPrx*)calloc(1,sizeof(MotorsPrx));
  assert(mprx!=0);
  if (refers_to == 0){
    PRX_REFERS_TO(mprx) = (Motors*)myimport(interface_name,"Motors");
    SUPER(mprx) = new_JDEInterfacePrx(interface_name,user,0);
  }else{
    PRX_REFERS_TO(mprx) = refers_to;
    SUPER(mprx) = new_JDEInterfacePrx(interface_name,
				      user,
				      SUPER(refers_to));
    myexport(interface_name,"Motors",refers_to);
    /*backwards compatibility*/
    myexport(interface_name,"v",&(refers_to->v));
    myexport(interface_name,"w",&(refers_to->w));
  }
  return mprx;

}

void delete_MotorsPrx(MotorsPrx* const self){
  if (self==0)
    return;
  
  if (PRX_REFERS_TO(self)){
    if (JDEInterface_refcount(PRX_REFERS_TO(SUPER(self)))==1){/*last reference*/
      /*FIXME: delete exported symbols*/
      delete_Motors(PRX_REFERS_TO(self));
      PRX_REFERS_TO(SUPER(self)) = 0;/*JDEInterface has been destroyed*/
    }
  }
  delete_JDEInterfacePrx(SUPER(self));
  free(self);
}

float MotorsPrx_v_get(const MotorsPrx* self){
  float* vp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    vp=&(PRX_REFERS_TO(self)->v);
  else
    vp=(float *)myimport(INTERFACEPRX_NAME(self),"v");
    
  return (vp?*vp:0.0);
}

float MotorsPrx_w_get(const MotorsPrx* self){
  float* wp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    wp=&(PRX_REFERS_TO(self)->w);
  else
    wp=(float *)myimport(INTERFACEPRX_NAME(self),"w");
    
  return (wp?*wp:0.0);
}

void MotorsPrx_v_set(MotorsPrx* const self, float new_v){
  float* vp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    vp=&(PRX_REFERS_TO(self)->v);
  else  
    vp=(float *)myimport(INTERFACEPRX_NAME(self),"v");
  if (vp)
    *vp = new_v;
}

void MotorsPrx_w_set(MotorsPrx* const self, float new_w){
  float* wp;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    wp=&(PRX_REFERS_TO(self)->w);
  else  
    wp=(float *)myimport(INTERFACEPRX_NAME(self),"w");
  if (wp)
    *wp = new_w;
}


INTERFACEPRX_ATTR_DEFINITION(Motors,cycle,int,VARIABLE,)
