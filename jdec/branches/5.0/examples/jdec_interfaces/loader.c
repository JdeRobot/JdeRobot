#include <loader.h>
#include <stdio.h>
#include <glib.h>
#include <dlfcn.h>

/*SO handles list*/
static GList* handles = 0;

int load_so(const char* sopath) {
  void* h;
  if ((h = dlopen(sopath,RTLD_NOW)) == 0) {
    fprintf(stderr,"%s\n",dlerror());
    return -1;
  }
  handles = g_list_append(handles,h);
  return 0;
}

/*
PyObject* load_py(const char* pypath, int argc, char** argv) {
  PyObject* py_name, *py_module = 0;

  if (! Py_IsInitialized()) {
    Py_Initialize();
    PySys_SetArgv(argc, argv);
  }

  py_name = PyString_FromString(pypath);
  if (py_name == 0) {
    PyErr_Print();
    return 0;
  }

  py_module = PyImport_Import(py_name);
  Py_DECREF(py_name);

  if (py_module == 0) {
    PyErr_Print();
    fprintf(stderr, "%s: Failed to load %s\n",__PRETTY_FUNCTION__,pypath);
    return 0;
  }
  return py_module;
}
*/
