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
    void addref(JDEInterfacePrx* const referral);
    void delref(JDEInterfacePrx* const referral);
    unsigned int refcount();
  }
} JDEInterface;

typedef struct{
  JDEInterface* refers_to;
  JDESchema* user;
  %extend{
    JDEInterfacePrx(const char* interface_name,
		    JDESchema* const user,
		    JDEInterface* const refers_to);
    void run();
    void stop();
  }
} JDEInterfacePrx;
