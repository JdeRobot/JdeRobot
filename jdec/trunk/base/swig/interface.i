%module interface

%{
#include <interface.h>
%}


typedef struct{
  char *interface_name;
  struct JDESchema *supplier;
  %extend{
    JDEInterface(const char *interface_name,
		 struct JDESchema *const supplier);
    void refcount_inc();
    void refcount_dec();
  }
} JDEInterface;

typedef struct{
  JDEInterface *refers_to;
  struct JDESchema *user;
  %extend{
    JDEInterfacePrx(const char *interface_name,
		    struct JDESchema *const user);
    int run();
    int stop();
  }
} JDEInterfacePrx;
