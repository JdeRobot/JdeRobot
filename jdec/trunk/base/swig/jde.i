%module jde

%{
#include <jde.h>
%}


int jdeinit(int argc, char* argv[], const char* cf);
void jdeshutdown(const int sig);
JDESchema* jde_loadschema(const char *name);
JDEDriver* jde_loaddriver(const char *name);

char* get_configfile();
JDESchema* find_schema (const char *name);
/*void *myimport(const char *schema, const char *name);*/
