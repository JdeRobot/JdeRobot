#include "loader.h"
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <Python.h>

PyThreadState * mainThreadState = NULL;
PyGILState_STATE gstate;



void* load_so(const char* sopath){
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

void* load_py(const char* pypath){
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
