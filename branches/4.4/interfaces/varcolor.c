#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "varcolor.h"

Varcolor* new_Varcolor(const char* interface_name,
		       JDESchema* const supplier){
  Varcolor* v;
  
  assert(supplier!=0 && supplier->hierarchy!=0);
  v = (Varcolor*)calloc(1,sizeof(Varcolor));
  assert(v!=0);
  v->super = new_JDEInterface(interface_name,supplier);
  assert(v->super != 0);
  /*interface export*/
  if (JDEHierarchy_myexport(supplier->hierarchy,interface_name,"Varcolor",v) == 0){
    delete_JDEInterface(v->super);
    free(v);
    return 0;
  }
  /*backwards compatibility exports*/
  myexport(interface_name,"varcolor",v);
  return v;
}

void delete_Varcolor(Varcolor* const self){
  if (self==0)
    return;
  delete_JDEInterface(self->super);
  free(self);
}

VarcolorPrx* new_VarcolorPrx(const char* interface_name,
			     JDESchema* const user){
  VarcolorPrx* vprx;

  assert(user!=0 && user->hierarchy!=0);
  vprx = (VarcolorPrx*)calloc(1,sizeof(VarcolorPrx));
  assert(vprx!=0);
  vprx->refers_to = (Varcolor*)JDEHierarchy_myimport(user->hierarchy,interface_name,"Varcolor");
  if (vprx->refers_to == 0){ /*we are connecting with an old
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
      free(vprx);
      return 0;
    }
  }
  vprx->super = new_JDEInterfacePrx(interface_name,user);
  assert(vprx->super!=0);
  return vprx;
}

void delete_VarcolorPrx(VarcolorPrx* const self){
  if (self==0)
    return;
  
  delete_JDEInterfacePrx(self->super);
  free(self);
}


int VarcolorPrx_run (VarcolorPrx * const self){
  return JDEInterfacePrx_run(self->super);
}

int VarcolorPrx_stop (VarcolorPrx * const self){
  return JDEInterfacePrx_stop(self->super);
}


/*get methods*/
Varcolor* VarcolorPrx_varcolorp_get(VarcolorPrx *const self){
  Varcolor* varcolorp = 0;

  assert(self!=0);
  if (self->refers_to)
    varcolorp=self->refers_to;
  else/*backwards compatibility*/
    varcolorp=(Varcolor *)myimport(INTERFACEPRX_NAME(self),"varcolor");
  assert(varcolorp!=0);
  return varcolorp;
}


char* VarcolorPrx_img_get(VarcolorPrx *const self){
  Varcolor* varcolorp = 0;

  assert(self!=0);
  varcolorp = VarcolorPrx_varcolorp_get(self);
  return varcolorp->img;
}

int VarcolorPrx_width_get(VarcolorPrx *const self){
  Varcolor* varcolorp = 0;

  assert(self!=0);
  varcolorp = VarcolorPrx_varcolorp_get(self);
  return varcolorp->width;
}

int VarcolorPrx_height_get(VarcolorPrx *const self){
  Varcolor* varcolorp;

  assert(self!=0);
  varcolorp = VarcolorPrx_varcolorp_get(self);
  return varcolorp->height;
}

unsigned long int VarcolorPrx_clock_get(VarcolorPrx *const self){
  Varcolor* varcolorp;

  assert(self!=0);
  varcolorp = VarcolorPrx_varcolorp_get(self);
  return varcolorp->clock;
}

/*set methods*/
void VarcolorPrx_img_set(VarcolorPrx* const self, char* new_img){
  Varcolor *vp;

  assert(self!=0);
  vp = VarcolorPrx_varcolorp_get(self);
  memmove(vp->img,new_img,vp->width*vp->height*3);
}

void VarcolorPrx_width_set(VarcolorPrx* const self, int new_width){
  Varcolor *vp;

  assert(self!=0);
  vp = VarcolorPrx_varcolorp_get(self);
  vp->width = new_width;
}

void VarcolorPrx_height_set(VarcolorPrx* const self, int new_height){
  Varcolor *vp;

  assert(self!=0);
  vp = VarcolorPrx_varcolorp_get(self);
  vp->height = new_height;
}

void VarcolorPrx_clock_set(VarcolorPrx* const self, unsigned long int new_clock){
  Varcolor *vp;

  assert(self!=0);
  vp = VarcolorPrx_varcolorp_get(self);
  vp->clock = new_clock;
}

/*Macro to define all the attr get/set functions*/
Varcolor_attr(INTERFACE_ATTR_DEFINITION,Varcolor)
