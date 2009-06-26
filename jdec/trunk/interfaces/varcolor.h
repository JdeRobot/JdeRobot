#ifndef VARCOLOR_H
#define VARCOLOR_H
#include <jde.h>
#include "interface.h"

#ifdef __cplusplus
extern "C" {
#endif

INTERFACEDEF(Varcolor,
	     char *img; 
	     /* RGB order */
	     unsigned long int clock;
	     int width;
	     int height;)

IFPROXYDEF(Varcolor,VarcolorPrx)

/*constructors & destructors*/
Varcolor* new_Varcolor(const char* interface_name,
		       JDESchema* const supplier);
void delete_Varcolor(Varcolor* const this);

VarcolorPrx* new_VarcolorPrx(const char* interface_name,
			     JDESchema* const user,
			     Varcolor* const refers_to);
void delete_VarcolorPrx(VarcolorPrx* const this);


/*get methods*/
char* VarcolorPrx_img_get(const VarcolorPrx* this);
int VarcolorPrx_width_get(const VarcolorPrx* this);
int VarcolorPrx_height_get(const VarcolorPrx* this);
unsigned long int VarcolorPrx_clock_get(const VarcolorPrx* this);

/*set methods*/
void VarcolorPrx_img_set(VarcolorPrx* const this, const char* new_img);
void VarcolorPrx_width_set(VarcolorPrx* const this, const float new_width);
void VarcolorPrx_height_set(VarcolorPrx* const this, const float new_height);
void VarcolorPrx_clock_set(VarcolorPrx* const this, const unsigned long int new_clock);


#ifdef __cplusplus
}
#endif
#endif /*VARCOLOR_H*/
