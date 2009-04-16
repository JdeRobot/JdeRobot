%module encoders

%{
#include <encoders.h>
%}

%import "interface.i"

enum robot_enum {ROBOT_X,ROBOT_Y,ROBOT_THETA,ROBOT_COS,ROBOT_SIN,ROBOT_NELEM};
typedef struct{
  char interface_name[MAX_NAME];
  %extend{
    Encoders(const char* father,
	     const char* interface_name = "encoders",
	     JDESchema* owner = 0);
    void run();
    void stop();
    /*perceptions*/
    float robot[ROBOT_NELEM];
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
