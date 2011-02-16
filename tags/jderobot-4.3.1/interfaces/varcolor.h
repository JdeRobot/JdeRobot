#ifndef VARCOLOR_H
#define VARCOLOR_H
#include "interface.h"

#ifdef __cplusplus
extern "C" {
#endif

#define Varcolor_attr(ATTR,I)					\
  ATTR(I,img,char*,VARIABLE,0) /* RGB order */			\
  ATTR(I,width,int,VARIABLE,0)					\
  ATTR(I,height,int,VARIABLE,0)					\
  ATTR(I,clock,unsigned long int,VARIABLE,0)

INTERFACE_DECLARATION(Varcolor,Varcolor_attr)

#ifdef __cplusplus
}
#endif
#endif /*VARCOLOR_H*/
