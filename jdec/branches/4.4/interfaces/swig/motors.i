%module motors

%{
#include <motors.h>
Interface* Motors_cast(Motors* e){
  return (Interface*)e;
}
%}

%import "interface.i"

typedef struct{
  %extend{
    Motors(const char* father,
	   const char* interface_name = "motors",
	   const int owned = 0);
    Interface* cast();
    /*modulations*/
    float v; /* mm/s */
    float w; /* deg/s */
    int cycle;
  }
} Motors;
