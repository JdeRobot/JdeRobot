%module motors

%{
#include <motors.h>
%}

%import "interface.i"

typedef struct{
  %extend{
    Motors(const char* father,
	   const char* interface_name = "motors",
	   JDESchema* owner = 0);
    void run();
    void stop();
    /*modulations*/
    float v; /* mm/s */
    float w; /* deg/s */
    int cycle;
  }
} Motors;
