#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "motors.h"

Motors* new_Motors(const char* interface_name,
		   JDESchema* const supplier){
  Motors* m;
  
  assert(supplier!=0 && supplier->hierarchy!=0);
  m = (Motors*)calloc(1,sizeof(Motors));
  assert(m!=0);
  SUPER(m) = new_JDEInterface(interface_name,supplier);
  assert(SUPER(m) != 0);
  /*interface export*/
  if (JDEHierarchy_myexport(supplier->hierarchy,interface_name,"Motors",m) == 0){
    delete_JDEInterface(SUPER(m));
    free(m);
    return 0;
  }
  /*backwards compatibility exports*/
  myexport(interface_name,"v",&(m->v));
  myexport(interface_name,"w",&(m->w));
  return m;
}

void delete_Motors(Motors* const self){
  if (self==0)
    return;
  delete_JDEInterface(SUPER(self));
  free(self);/*FIXME: un-export symbols. references?*/
}

/*for old interfaces proxy->refers_to == 0*/
MotorsPrx* new_MotorsPrx(const char* interface_name,
			 JDESchema* const user){
  MotorsPrx* mprx;

  assert(user!=0 && user->hierarchy!=0);
  mprx = (MotorsPrx*)calloc(1,sizeof(MotorsPrx));
  assert(mprx!=0);
  PRX_REFERS_TO(mprx) = (Motors*)JDEHierarchy_myimport(user->hierarchy,interface_name,"Motors");
  if (PRX_REFERS_TO(mprx) == 0){ /*we are connecting with an old
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
      free(mprx);
      return 0;
    }
  }
  SUPER(mprx) = new_JDEInterfacePrx(interface_name,user);
  assert(SUPER(mprx)!=0);
  return mprx;
}

void delete_MotorsPrx(MotorsPrx* const self){
  if (self==0)
    return;
  
  delete_JDEInterfacePrx(SUPER(self));
  free(self);
}


int MotorsPrx_run (MotorsPrx * const self){
  return JDEInterfacePrx_run(self->super);
}

int MotorsPrx_stop (MotorsPrx * const self){
  return JDEInterfacePrx_stop(self->super);
}


float MotorsPrx_v_get(MotorsPrx *const self){
  float* vp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    vp=&(PRX_REFERS_TO(self)->v);
  else
    vp=(float *)myimport(INTERFACEPRX_NAME(self),"v");
  assert(vp!=0);
  return *vp;
}

float MotorsPrx_w_get(MotorsPrx *const self){
  float* wp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    wp=&(PRX_REFERS_TO(self)->w);
  else
    wp=(float *)myimport(INTERFACEPRX_NAME(self),"w");
  assert(wp!=0);
  return *wp;
}

void MotorsPrx_v_set(MotorsPrx* const self, float new_v){
  float* vp = 0;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    vp=&(PRX_REFERS_TO(self)->v);
  else  
    vp=(float *)myimport(INTERFACEPRX_NAME(self),"v");
  assert(vp!=0);
  *vp = new_v;
}

void MotorsPrx_w_set(MotorsPrx* const self, float new_w){
  float* wp;

  assert(self!=0);
  if (PRX_REFERS_TO(self))
    wp=&(PRX_REFERS_TO(self)->w);
  else  
    wp=(float *)myimport(INTERFACEPRX_NAME(self),"w");
  assert(wp!=0);
  *wp = new_w;
}

/*Macro to define all the attr get/set functions*/
Motors_attr(INTERFACE_ATTR_DEFINITION,Motors)
