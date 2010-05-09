%module laser
%include "carrays.i"
%array_class(int,intArray);

%{
#include <laser.h>
%}

%import "interface.i"

%constant int MAX_LASER = 720;

typedef struct{
  JDEInterface* super;
  %extend{
    Laser(const char* interface_name,
	  JDESchema* const supplier);
    int laser[MAX_LASER];
    int number;
    int resolution;
    unsigned long int clock;
  }
}Laser;

typedef struct{
  %extend{
    LaserPrx(const char* interface_name,
	     JDESchema* const user);
    void run();
    void stop();
    int laser[MAX_LASER];
    int number;
    int resolution;
    unsigned long int clock;
  }
}LaserPrx;
