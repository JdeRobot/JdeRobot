%module encoders
%include "carrays.i"
%array_class(float,floatArray);


%{
#include <encoders.h>
%}

%import "interface.i"

enum robot_enum {ROBOT_X,ROBOT_Y,ROBOT_THETA,ROBOT_COS,ROBOT_SIN,ROBOT_NELEM};
typedef struct{
  JDEInterface* super;
  %extend{
    Encoders(const char* interface_name,
	     JDESchema* const supplier);
    float robot[ROBOT_NELEM];
    float x;
    float y;
    float theta;
    float cos;
    float sin;
    unsigned long int clock;
  }
}Encoders;

typedef struct{
  %extend{
    EncodersPrx(const char* interface_name,
		JDESchema* const user);
    void run();
    void stop();
    float robot[ROBOT_NELEM];
    float x;
    float y;
    float theta;
    float cos;
    float sin;
    unsigned long int clock;
  }
}EncodersPrx;
