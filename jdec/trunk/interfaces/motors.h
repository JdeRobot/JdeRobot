#ifndef MOTORS_H
#define MOTORS_H
#include "interface.h"

#ifdef __cplusplus
extern "C" {
#endif

#define Motors_attr(ATTR,I)			\
  ATTR(I,v,float,VARIABLE,0)			\
  ATTR(I,w,float,VARIABLE,0)			\
  ATTR(I,cycle,int,VARIABLE,0)
  

INTERFACE_DECLARATION(Motors,Motors_attr)


#ifdef __cplusplus
}
#endif
#endif /*MOTORS_H*/
