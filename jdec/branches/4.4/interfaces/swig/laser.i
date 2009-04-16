%module laser
%include "carrays.i"
%array_class(int,intArray);

%{
#include <laser.h>
%}

%import "interface.i"

%constant int MAX_LASER = 720;

typedef struct{
  char interface_name[MAX_NAME];
  %extend{
    Laser(const char* father,
	  const char* interface_name = "laser",
	  JDESchema* owner = 0);
    void run();
    void stop();
    /*perceptions*/
    int laser[MAX_LASER];
    int number;
    int resolution;
    unsigned long int clock;
    /*modulations*/
    int cycle;
  }
}Laser;
