#ifndef INTERFACE_H
#define INTERFACE_H
#include <jde.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
  int father_id;
  char interface_name[MAX_NAME];
  int owned;
  void* datap;
} Interface;

/*constructor & destructor*/
Interface* new_Interface(const char* father,
			 const char* interface_name,
			 const int owned);
void delete_Interface(Interface* i);

void run(Interface* i);
void stop(Interface* i);

#ifdef __cplusplus
}
#endif
#endif /*INTERFACE_H*/
