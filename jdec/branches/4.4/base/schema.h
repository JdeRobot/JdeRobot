#ifndef SCHEMA_H
#define SCHEMA_H
#include <jde.h>


JDESchema* new_JDESchema(char* name,
			 void (*init)(char *configfile),
			 void (*terminate)(void),
			 void (*stop)(void),
			 void (*run)(int father, int *brothers, arbitration fn),
			 void (*show)(void),
			 void (*hide)(void));

#endif /*SCHEMA_H*/
