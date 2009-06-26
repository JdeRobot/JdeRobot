%module laser
%include "carrays.i"
%array_class(int,intArray);

%{
#include <laser.h>
%}

%import "interface.i"

%constant int MAX_LASER = 720;

typedef struct{
  /*perceptions*/
  int laser[MAX_LASER];
  int number;
  int resolution;
  unsigned long int clock;
  JDEInterface* super;
  %extend{
    Laser(const char* interface_name,
	  JDESchema* const supplier);
  }
}Laser;

typedef struct{
  Laser* refers_to;
  JDEInterfacePrx* super;
  %extend{
    LaserPrx(const char* interface_name,
	     JDESchema* const user,
	     Laser* const refers_to=0);
    /*perceptions*/
    int laser[MAX_LASER];
    int number;
    int resolution;
    unsigned long int clock;
  }
}LaserPrx;
