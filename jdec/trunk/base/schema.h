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
void delete_JDESchema(JDESchema* const s);

void JDESchema_init(JDESchema* const s, char *configfile);
void JDESchema_terminate(JDESchema* const s);
void JDESchema_stop(JDESchema* const s);
void JDESchema_run(JDESchema* const s,
		   int father, int *brothers, arbitration fn);
void JDESchema_show(JDESchema* const s);
void JDESchema_hide(JDESchema* const s);
int JDESchema_get_state(JDESchema* const s);
void JDESchema_set_state(JDESchema* const s, int newstate);
//void JDESchema_wait_statechange(JDESchema* const s);
void JDESchema_speedcounter(JDESchema* const s);

void JDEDriver_init(JDEDriver* const d, char *configfile);
void JDEDriver_terminate(JDEDriver* const d);

#endif /*SCHEMA_H*/
