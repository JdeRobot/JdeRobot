#ifndef LOADER_H
#define LOADER_H

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

struct JDESchema;

/**
 * Load a schema, python or shared object.
 * 
 * @param schema_path path to the schema. The path has to finish with
 * .so extension to be recognized as schema.
 * @param cf_path path of the config file for this schema
 * @return 1 on successful loading,0 otherwise.
 */
struct JDESchema *load_schema(const char* schema_path, const char* cf_path);

#ifdef __cplusplus
} /*extern "C"*/
#endif /*__cplusplus*/
#endif /*LOADER_H*/
