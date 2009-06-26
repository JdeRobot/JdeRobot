%module motors

%{
#include <motors.h>
%}

%import "interface.i"

typedef struct{
  /*modulations*/
  float v; /* mm/s */
  float w; /* deg/s */
  JDEInterface* super;
  %extend{
    Motors(const char* interface_name,
	   JDESchema* const supplier);
  }
} Motors;

typedef struct{
  Motors* refers_to;
  JDEInterfacePrx* super;
  %extend{
    MotorsPrx(const char* interface_name,
	      JDESchema* const user,
	      Motors* const refers_to=0);
    /*modulations*/
    float v; /* mm/s */
    float w; /* deg/s */
  }
} MotorsPrx;

