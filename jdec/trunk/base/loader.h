#ifndef LOADER_H
#define LOADER_H
#include <jde.h>
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Initialize python support
 *
 * @param argc number of arguments
 * @param argv array with arguments
 * @return 1 on successful initialization,0 otherwise
 */
int init_py(int argc, char** argv);

/**
 * Load a python module
 *
 * @param pypath path to the module
 * @param cf_path path of the config file for this module
 * @return 1 on successful loading,0 otherwise.
 */
void* load_py(const char* pypath, const char* cf_path);

/**
 * Load a shared object module
 *
 * @param sopath path to the module
 * @param cf_path path of the config file for this module
 * @return 1 on successful loading,0 otherwise.
 */
void* load_so(const char* sopath, const char* cf_path);

/**
 * Load a module, python or shared object.
 * 
 * @param module_path path to the module. The path has to finish with
 * .py or .so extensions to be recognized as modules.
 * @param cf_path path of the config file for this module
 * @return 1 on successful loading,0 otherwise.
 */
int load_module2(const char* module_path, const char* cf_path);

#ifdef __cplusplus
} /*extern "C"*/
#endif /*__cplusplus*/
#endif /*LOADER_H*/
