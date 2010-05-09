%module motors

%{
#include <motors.h>
%}

%import "interface.i"

typedef struct{
  JDEInterface *super;
  %extend{
    Motors(const char* interface_name,
	   JDESchema* const supplier);
    %extend{
      float v; /* mm/s */
      float w; /* deg/s */
    }
  }
} Motors;

typedef struct{
  %extend{
    MotorsPrx(const char* interface_name,
	      JDESchema* const user);
    void run();
    void stop();
    float v; /* mm/s */
    float w; /* deg/s */
  }
} MotorsPrx;

