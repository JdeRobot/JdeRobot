%module interface

%{
#include <interface.h>
%}


typedef struct{
  int father_id;
  char interface_name[MAX_NAME];
  %extend{
    Interface(const char* father,
	      const char* interface_name,
	      const int owned=0);
    void run();
    void stop();
  }
} Interface;
