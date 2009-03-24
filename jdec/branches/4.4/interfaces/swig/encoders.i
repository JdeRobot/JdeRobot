%module encoders

%{
#include <encoders.h>
Interface* Encoders_cast(Encoders* e){
  return (Interface*)e;
}
%}

%import "interface.i"

typedef struct{
  char interface_name[MAX_NAME];
  %extend{
    Encoders(const char* father,
	     const char* interface_name = "encoders",
	     const int owned = 0);
    Interface* cast();
    /*perceptions*/
    float x;
    float y;
    float theta;
    float cos;
    float sin;
    unsigned long int clock;
    /*modulations*/
    int cycle;
  }
}Encoders;
