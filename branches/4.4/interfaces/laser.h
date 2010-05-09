#ifndef LASER_H
#define LASER_H
#include "interface.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LASER 720

#define Laser_attr(ATTR,I)			\
  ATTR(I,laser,int,ARRAY,MAX_LASER)		\
  ATTR(I,number,int,VARIABLE,0)			\
  ATTR(I,resolution,int,VARIABLE,0)		\
  ATTR(I,clock,unsigned long int,VARIABLE,0)	

INTERFACE_DECLARATION(Laser,Laser_attr)

#ifdef __cplusplus
}
#endif
#endif /*LASER_H*/
