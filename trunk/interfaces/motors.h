#ifndef MOTORS_H
#define MOTORS_H
#include "interface.h"

#ifdef __cplusplus
extern "C" {
#endif

#define Motors_attr(ATTR,I)			\
  ATTR(I,v,float,VARIABLE,0) /* mm/s */		\
  ATTR(I,w,float,VARIABLE,0) /* deg/s */  

INTERFACE_DECLARATION(Motors,Motors_attr)


#ifdef __cplusplus
}
#endif
#endif /*MOTORS_H*/
