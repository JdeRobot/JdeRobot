#ifndef LOADER_H
#define LOADER_H
#include <jde.h>
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

void* load_so(const char* sopath, const char* cf_path);
int init_py(int argc, char** argv);
void* load_py(const char* pypath, const char* cf_path);
int load_module2(const char* module_path, const char* cf_path);

#ifdef __cplusplus
} /*extern "C"*/
#endif /*__cplusplus*/
#endif /*LOADER_H*/
