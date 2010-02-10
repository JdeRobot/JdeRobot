#include "loader.h"
#include <schema.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <Python.h>
#include "swigpyrun.h"

//PyThreadState * mainThreadState = NULL;
//PyGILState_STATE gstate;

int init_py(int argc, char** argv){
  if (! Py_IsInitialized()) {
    Py_Initialize();
    // initialize thread support
    PyEval_InitThreads();
    PySys_SetArgv(argc, argv);
    // release the lock and save state
    //mainThreadState = PyEval_SaveThread();
  }
  return 1;
}

/* typedef struct{ */
/*   char path[MAX_BUFFER]; */
/*   char cf_path[MAX_BUFFER]; */
/* }python_module_thread_arg_t; */

/* void* python_module_thread(void* args){ */
/*   PyGILState_STATE gstate; */
/*   PyObject* main_module; */
/*   PyObject* main_dict; */
/*   PyObject* main_dict_copy; */
/*   PyObject* res; */
/*   python_module_thread_arg_t* mod_args =  */
/*     (python_module_thread_arg_t*)args; */
/*   FILE* py_file; */

/*   if ((py_file = fopen(mod_args->path, "r"))==0){ */
/*     fprintf(stderr,"Error opening python module %s: ",mod_args->path); */
/*     perror(0); */
/*     return 0; */
/*   } */

/*   gstate = PyGILState_Ensure(); */
/*   // Get a reference to the main module. */
/*   main_module = PyImport_AddModule("__main__"); */
  
/*   // Add cf_path as a constant in the main module */
/*   PyModule_AddStringConstant(main_module,"configfile",mod_args->cf_path); */

/*   // Get the main module's dictionary */
/*   // and make a copy of it to execute in a new environment */
/*   main_dict = PyModule_GetDict(main_module); */
/*   main_dict_copy = PyDict_Copy(main_dict); */
  
  
/*   res = PyRun_File(py_file, mod_args->path, */
/* 		   Py_file_input, */
/* 		   main_dict_copy, main_dict_copy); */
/*   if (!res){ */
/*     fprintf(stderr,"Error running python module %s:\n",mod_args->path); */
/*     if (PyErr_Occurred()) */
/*       PyErr_Print(); */
/*   }else */
/*     Py_DECREF(res); */
/*   Py_DECREF(main_dict_copy); */
/*   PyGILState_Release(gstate); */
/*   fclose(py_file); */
/*   free(mod_args); */
/*   return 0; */
/* } */


struct JDESchema *load_schema_py(const char *pypath, const char *cf_path){
  PyGILState_STATE gstate;
  PyObject *main_module;
  PyObject *main_dict;
  PyObject *main_dict_copy;
  PyObject *res;
  FILE *py_file;
  PyObject *py_maker,*py_schema;
  JDESchema *schema = 0;
  int rc;

  /* args = (python_module_thread_arg_t*) */
/*     calloc(1,sizeof(python_module_thread_arg_t));/\*deallocated in thread*\/ */
/*   strncpy(args->path,pypath,MAX_BUFFER-1); */
/*   args->path[MAX_BUFFER-1] = '\0'; */
/*   strncpy(args->cf_path,cf_path,MAX_BUFFER-1); */
/*   args->cf_path[MAX_BUFFER-1] = '\0'; */
/*   pthread_create(&p,NULL,python_module_thread,(void*)args); */
/*   pthread_detach(&p); */

  if ((py_file = fopen(pypath, "r"))==0){
    fprintf(stderr,"Error opening python module %s: ",pypath);
    perror(0);
    return 0;
  }

  gstate = PyGILState_Ensure();
  // Get a reference to the main module.
  main_module = PyImport_AddModule("__main__");
  
/*   // Add cf_path as a constant in the main module */
/*   PyModule_AddStringConstant(main_module,"configfile",mod_args->cf_path); */

  // Get the main module's dictionary
  // and make a copy of it to execute in a new environment
  main_dict = PyModule_GetDict(main_module);
  main_dict_copy = PyDict_Copy(main_dict);
  
  
  res = PyRun_File(py_file, pypath,
		   Py_file_input,
		   main_dict_copy, main_dict_copy);
  if (res == 0){
    fprintf(stderr,"Error running python module %s:\n",pypath);
    if (PyErr_Occurred())
      PyErr_Print();
  }else{/*module loaded*/
    Py_DECREF(res);
    /*get createSchema*/
    py_maker = PyDict_GetItemString(main_dict_copy,"createSchema");
    py_schema = PyObject_CallObject(py_maker,NULL);
    rc = SWIG_ConvertPtr(py_schema,
			 (void**)&schema,
			 SWIG_TypeQuery("JDESchema *"),
			 SWIG_POINTER_DISOWN);
    if (!SWIG_IsOK(rc))
      fprintf(stderr,"Error converting python schema to c schema\n");
    Py_DECREF(py_schema);/*a reference must be kept in python code*/
  }

  Py_DECREF(main_dict_copy);
  PyGILState_Release(gstate);
  fclose(py_file);
  return schema;
}


struct JDESchema *load_schema_so(const char* sopath, const char* cf_path){
  JDESchema* (*maker)();
  JDESchema *s;
  void* h = 0;

  dlerror();
  if ((h=dlopen(sopath,RTLD_LAZY)) == 0)
    fprintf(stderr,"%s\n",dlerror());
  else{
    maker = (JDESchema* (*)())dlsym(h,"createSchema");
    if (maker==0){
      dlclose(h);
      fprintf(stderr,"%s\n",dlerror());
    }else{
      s = maker();
      s->handle = h;
      return s;
    }
  }
  return 0;
}

struct JDESchema *load_schema(const char* schema_path, const char* cf_path){
  char *ppos;

  fprintf(stderr,"Loading %s...\n",schema_path);

  ppos=strrchr(schema_path,'.');
  if (strcmp(ppos, ".so") == 0)
    return load_schema_so(schema_path,cf_path);
  else if (strcmp(ppos, ".py") == 0)
    return load_schema_py(schema_path,cf_path);
  else {
    fprintf(stderr,"unknown module type %s\n",schema_path);
    return 0;
  }
}

