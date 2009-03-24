%module laser
%include "carrays.i"
%array_class(int,intArray);

%{
#include <laser.h>
Interface* Laser_cast(Laser* l){
  return (Interface*)l;
}
%}

%import "interface.i"

%constant int MAX_LASER = 720;

typedef struct{
  char interface_name[MAX_NAME];
  %extend{
    Laser(const char* father,
	  const char* interface_name = "laser",
	  const int owned = 0);
    Interface* cast();
    /*perceptions*/
    int laser[MAX_LASER];
    int number;
    int resolution;
    unsigned long int clock;
    /*modulations*/
    int cycle;
  }
}Laser;
