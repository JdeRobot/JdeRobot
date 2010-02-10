#ifndef INTERFACE_H
#define INTERFACE_H
#include <jde.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
  int father_id;
  char interface_name[MAX_NAME];
  JDESchema* owner;
  int implemented;
  void* datap;
} Interface;

/*constructor & destructor*/
Interface* new_Interface(JDESchema* const owner,
			 const char* interface_name,
			 const int implemented);
void delete_Interface(Interface* const i);

void Interface_run(const Interface* i);
void Interface_stop(const Interface* i);

#ifdef __cplusplus
}
#endif
#endif /*INTERFACE_H*/
