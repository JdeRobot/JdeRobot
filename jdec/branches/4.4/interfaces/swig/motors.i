%module motors

%{
#include <motors.h>
%}

%import "interface.i"

typedef struct{
  %extend{
    Motors(JDESchema* const owner,
	   const char* interface_name = "motors",
	   const int implemented = 0);
    void run();
    void stop();
    /*modulations*/
    float v; /* mm/s */
    float w; /* deg/s */
    int cycle;
  }
} Motors;
