%module encoders

%{
#include <encoders.h>
%}

typedef struct{
  %extend{
    Encoders(const char* interface_name = "encoders");
    /*perceptions*/
    const float x;
    const float y;
    const float theta;
    const float cos;
    const float sin;
    const unsigned long int clock;
    /*modulations*/
    int cycle;
  }
}Encoders;
