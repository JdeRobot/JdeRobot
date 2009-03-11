#ifndef LOADER_H
#define LOADER_H
#include <jde.h>
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

void* load_so(const char* sopath);
int init_py(int argc, char** argv);
void* load_py(const char* pypath);

#ifdef __cplusplus
} /*extern "C"*/
#endif /*__cplusplus*/
#endif /*LOADER_H*/
