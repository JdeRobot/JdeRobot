#ifndef HIERARCHY_H
#define HIERARCHY_H


#ifdef __cplusplus
extern "C" {
#endif

/*forward declarations*/
struct JDESchema;
typedef struct JDEHierarchy_p JDEHierarchy_p;

typedef struct JDEHierarchy{
  char *name;
  char *config_file;
  char *path;
  struct JDEHierarchy_p *priv;
}JDEHierarchy;

#define MAX_BUFFER 512
/*defined at jderobot.c. Used to make work load_module*/
extern char path[MAX_BUFFER];

/**
 * Create new hierarchy
 * @param argc argument count
 * @param argv argument array
 * @param cf if not null used as path for config file.
 * @return 1 if jde initialization success
 */
extern JDEHierarchy * new_JDEHierarchy(int argc, char** argv, 
				       const char* cf);

/**
 * Delete the hierarchy stoping all propertly
 * @param self the hierarchy to delete
 * @return void
 */
extern void delete_JDEHierarchy(JDEHierarchy * const self);


/**
 * Add a schema to the hierarchy
 * @param self hierarchy
 * @param s schema to be added
 * @return 1 on success, <0 otherwise
 */
extern int JDEHierarchy_add_schema(JDEHierarchy * const self,
				   struct JDESchema* const s);

/**
 * Find schema in a hierarchy
 * @param self hierarchy
 * @param name schema name to be searched
 * @return schema if found, 0 otherwise
 */
extern struct JDESchema* JDEHierarchy_find_schema (JDEHierarchy * const self, 
						   const char *name);

/**
 * Get an interface proxy
 * @param self hierarchy
 * @param interface_name interface name
 * @param instance_name instance name for the searched interface. If 0
 * interface_name will be used
 * @param user schema making the request. It could be 0.
 * @return interface proxy if found, 0 otherwise
 */
extern struct JDEInterfacePrx *
JDEHierarchy_interfaceprx_get(JDEHierarchy * const self,
			      const char* interface_name,
			      const char* instance_name,
			      JDESchema* const user);

/**
 * Export symbol in this hierarchy
 * @param self hierarchy
 * @param namespace name space to index the symbol
 * @param symbol_name symbol name
 * @param p symbol pointer
 * @return 1 if the variables was correctly exported, 0 othewise
 */
extern int JDEHierarchy_myexport (JDEHierarchy * const self,
				  const char *namespace, const char *symbol_name,
				  void *p);


/**
 * Import symbol from this hierarchy
 * @param self hierarchy
 * @param namespace name space to make the search
 * @param symbol_name name of the searched symbol
 * @return pointer to the symbol if found, 0 otherwise
 */
extern void* JDEHierarchy_myimport (JDEHierarchy * const self,
				    const char *namespace, const char *symbol_name);


/**
 * Get the root schema of this hierarchy.
 * @param self hierarchy
 * @return the root schema
 */
extern struct JDESchema * JDEHierarchy_root_schema_get (JDEHierarchy * const self);


#ifdef __cplusplus
 }
#endif

#endif /*HIERARCHY_H*/
