#ifndef LOADER_H
#define LOADER_H
#include <defs.h>
/*#include <Python.h>*/

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/


int load_so(const char* sopath);

  /*
    PyObject* load_py(const char* pypath, int argc = 0, char** argv =
    0);
  */

#ifdef __cplusplus
} /*extern "C"*/
#endif /*__cplusplus*/
#endif /*LOADER_H*/
