#ifndef ENCODERS_H
#define ENCODERS_H
#include "interface.h"

#ifdef __cplusplus
extern "C" {
#endif

enum robot_enum {ROBOT_X,ROBOT_Y,ROBOT_THETA,ROBOT_COS,ROBOT_SIN,ROBOT_NELEM};

#define Encoders_attr(ATTR,I)					\
  ATTR(I,robot,float,ARRAY,ROBOT_NELEM)				\
  ATTR(I,x,float,SYNTHETIC,self->robot[ROBOT_X])		\
  ATTR(I,y,float,SYNTHETIC,self->robot[ROBOT_Y])		\
  ATTR(I,theta,float,SYNTHETIC,self->robot[ROBOT_THETA])	\
  ATTR(I,cos,float,SYNTHETIC,self->robot[ROBOT_COS])		\
  ATTR(I,sin,float,SYNTHETIC,self->robot[ROBOT_SIN])		\
  ATTR(I,clock,unsigned long int,VARIABLE,0)


INTERFACE_DECLARATION(Encoders,Encoders_attr)

#ifdef __cplusplus
}
#endif
#endif /*ENCODERS_H*/
