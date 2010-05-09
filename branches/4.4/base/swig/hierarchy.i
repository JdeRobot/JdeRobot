%module hierarchy

%{
#include <hierarchy.h>
%}


%typemap(in) (int argc, char **argv) {
  /* Check if is a list */
  if (PyList_Check($input)) {
    int i;
    $1 = PyList_Size($input);
    $2 = (char **) malloc(($1+1)*sizeof(char *));
    for (i = 0; i < $1; i++) {
      PyObject *o = PyList_GetItem($input,i);
      if (PyString_Check(o))
	$2[i] = PyString_AsString(PyList_GetItem($input,i));
      else {
	PyErr_SetString(PyExc_TypeError,"list must contain strings");
	free($2);
	return NULL;
      }
    }
    $2[i] = 0;
  } else {
    PyErr_SetString(PyExc_TypeError,"not a list");
    return NULL;
  }
}

%typemap(freearg) (int argc, char **argv) {
  free((char *) $2);
}


typedef struct JDEHierarchy{
  char *name;
  char *config_file;
  char *path;
  %extend{
    JDEHierarchy(int argc, char **argv, const char* cf);
    int add_schema(struct JDESchema* const s);
    struct JDESchema *find_schema (const char *name);
    //int myexport (const char *namespace, const char *symbol_name,void *p);
    //void *myimport (const char *namespace, const char *symbol_name);
    struct JDESchema *root_schema_get();
  }
} JDEHierarchy;

