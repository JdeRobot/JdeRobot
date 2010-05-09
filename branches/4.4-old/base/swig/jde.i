%module jde

%{
#include <jde.h>
#include <jde_private.h>
%}


int jdeinit(const char* cf);
void jdeshutdown(const int sig);
JDESchema* jde_loadschema(const char *name);
JDEDriver* jde_loaddriver(const char *name);

char* get_configfile();
void null_arbitration();
JDESchema* find_schema (const char *name);
//int get_state(const JDESchema* s);
//void set_state(JDESchema* s,const int newstate);
//void speedcounter2(JDESchema* s);
int myexport(const char *schema, const char *name, void *p);
void *myimport(const char *schema, const char *name);
