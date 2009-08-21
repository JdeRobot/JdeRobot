%module encoders
%include "carrays.i"
%array_class(float,floatArray);


%{
#include <encoders.h>
%}

%import "interface.i"

enum robot_enum {ROBOT_X,ROBOT_Y,ROBOT_THETA,ROBOT_COS,ROBOT_SIN,ROBOT_NELEM};
typedef struct{
  /*perceptions*/
  float robot[ROBOT_NELEM];
  unsigned long int clock;
  JDEInterface* super;
  %extend{
    Encoders(const char* interface_name,
	     JDESchema* const supplier);
  }
}Encoders;

typedef struct{
  Encoders* refers_to;
  JDEInterfacePrx* super;
  %extend{
    EncodersPrx(const char* interface_name,
		JDESchema* const user,
		Encoders* const refers_to=0);
    /*perceptions*/
    float robot[ROBOT_NELEM];
    unsigned long int clock;
  }
}EncodersPrx;
