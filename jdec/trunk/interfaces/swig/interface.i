%module interface

%{
#include <interface.h>
%}


typedef struct{
  char interface_name[MAX_NAME];
  JDESchema* supplier;
  %extend{
    JDEInterface(const char* interface_name,
		 JDESchema* const supplier);
    addref(JDEInterfacePrx* const referral);
    delref(JDEInterfacePrx* const referral);
    refcount();
  }
} JDEInterface;

typedef struct{
  JDEInterface* refers_to;
  JDESchema* user;
  %extend{
    JDEInterfacePrx(const char* interface_name,
		    JDESchema* const user,
		    JDEInterface* const refers_to);
    run();
    stop();
  }
} JDEInterfacePrx;
