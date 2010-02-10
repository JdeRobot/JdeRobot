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

void* load_py(const char* pypath, const char* cf_path){
  PyObject* py_name, *py_module = 0;
  PyGILState_STATE gstate;

  py_name = PyString_FromString(pypath);
  if (py_name == 0) {
    PyErr_Print();
    return 0;
  }

  gstate = PyGILState_Ensure();
  py_module = PyImport_Import(py_name);
  if (py_module == 0) {
    PyErr_Print();
    fprintf(stderr, "%s: Failed to load %s\n",__PRETTY_FUNCTION__,pypath);
    return 0;
  }
  Py_DECREF(py_name);
  /* Release the thread. No Python API allowed beyond this point. */
  PyGILState_Release(gstate);
  return (void*)py_module;
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
    char pymodule[32];
    int lpymodule = ppos-module_path;

    strncpy(pymodule,module_path,lpymodule);
    pymodule[lpymodule] = '\0';
    if (load_py(pymodule,cf_path) == 0){
      fprintf(stderr,"error loading python module %s\n",module_path);
      return -1;
    }
  } else {
    fprintf(stderr,"unknown module type %s\n",module_path);
    return -1;
  }
  return 1;
}
