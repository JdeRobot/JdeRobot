#include "loader.h"
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <Python.h>

PyThreadState * mainThreadState = NULL;
PyGILState_STATE gstate;

void* load_so(const char* sopath, const char* cf_path){
  void* h = 0;
  dlerror();
  if ((h=dlopen(sopath,RTLD_LAZY)) == 0)
    fprintf(stderr,"%s\n",dlerror());
  return h;
}

int init_py(int argc, char** argv){
  if (! Py_IsInitialized()) {
    Py_Initialize();
    // initialize thread support
    PyEval_InitThreads();
    PySys_SetArgv(argc, argv);
    // release the lock and save state
    mainThreadState = PyEval_SaveThread();
  }
  return 1;
}


typedef struct{
  char path[MAX_BUFFER];
  char cf_path[MAX_BUFFER];
}python_module_thread_arg_t;

void* python_module_thread(void* args){
  PyGILState_STATE gstate;
  PyObject* main_module;
  PyObject* main_dict;
  PyObject* main_dict_copy;
  PyObject* res;
  python_module_thread_arg_t* mod_args = 
    (python_module_thread_arg_t*)args;
  FILE* py_file;

  if ((py_file = fopen(mod_args->path, "r"))==0){
    fprintf(stderr,"Error opening python module %s: ",mod_args->path);
    perror(0);
    return 0;
  }

  gstate = PyGILState_Ensure();
  // Get a reference to the main module.
  main_module = PyImport_AddModule("__main__");
  
  // Add cf_path as a constant in the main module
  PyModule_AddStringConstant(main_module,"configfile",mod_args->cf_path);

  // Get the main module's dictionary
  // and make a copy of it to execute in a new environment
  main_dict = PyModule_GetDict(main_module);
  main_dict_copy = PyDict_Copy(main_dict);
  
  
  res = PyRun_File(py_file, mod_args->path,
		   Py_file_input,
		   main_dict_copy, main_dict_copy);
  
  Py_DECREF(main_dict_copy);
  Py_DECREF(res);
  PyGILState_Release(gstate);
  fclose(py_file);
  free(mod_args);
  return 0;
}


void* load_py(const char* pypath, const char* cf_path){
  PyObject* py_name, *py_module = 0;
  pthread_t p;
  python_module_thread_arg_t* args;

  args = (python_module_thread_arg_t*)
    calloc(1,sizeof(python_module_thread_arg_t));/*deallocated in thread*/
  strncpy(args->path,pypath,MAX_BUFFER-1);
  args->path[MAX_BUFFER-1] = '\0';
  strncpy(args->cf_path,cf_path,MAX_BUFFER-1);
  args->cf_path[MAX_BUFFER-1] = '\0';
  pthread_create(&p,NULL,python_module_thread,(void*)args);
  pthread_detach(&p);
  return 1;
}

int load_module2(const char* module_path, const char* cf_path){
  char *ppos;

  fprintf(stderr,"Loading %s...\n",module_path);

  ppos=strrchr(module_path,'.');
  if (strcmp(ppos, ".so") == 0) {
    if (load_so(module_path,cf_path) == 0){
      fprintf(stderr,"error loading so module %s\n",module_path);
      return -1;
    }
  } else if (strcmp(ppos, ".py") == 0) {
    if (load_py(module_path,cf_path) == 0){
      fprintf(stderr,"error loading python module %s\n",module_path);
      return 0;
    }
  } else {
    fprintf(stderr,"unknown module type %s\n",module_path);
    return 0;
  }
  return 1;
}
