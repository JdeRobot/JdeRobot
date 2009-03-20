%module motors

%{
#include <motors.h>
%}

typedef struct{
  char interface_name[MAX_NAME];
  %extend{
    Motors(const char* interface_name = "motors");
    /*modulations*/
    float v; /* mm/s */
    float w; /* deg/s */
    int cycle;
  }
} Motors;
