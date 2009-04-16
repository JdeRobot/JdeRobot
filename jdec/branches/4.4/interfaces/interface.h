#ifndef INTERFACE_H
#define INTERFACE_H
#include <jde.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
  int father_id;
  char interface_name[MAX_NAME];
  //int owned;
  int refs;
  JDESchema* owner;
  void* datap;
} Interface;

/*constructor & destructor*/
Interface* new_Interface(const char* father,
			 const char* interface_name,
			 JDESchema* owner);
			 //const int owned);
void delete_Interface(Interface* i);

void Interface_run(Interface* i);
void Interface_stop(Interface* i);

#ifdef __cplusplus
}
#endif
#endif /*INTERFACE_H*/
