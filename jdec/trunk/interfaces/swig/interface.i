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
    void addref(JDEInterfacePrx *const referral);
    void delref(JDEInterfacePrx *const referral);
    unsigned int refcount();
  }
} JDEInterface;

typedef struct{
  JDEInterface *refers_to;
  struct JDESchema *user;
  %extend{
    JDEInterfacePrx(const char *interface_name,
		    struct JDESchema *const user,
		    JDEInterface *const refers_to);
    void run();
    void stop();
  }
} JDEInterfacePrx;
