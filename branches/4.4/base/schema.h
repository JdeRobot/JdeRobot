#ifndef SCHEMA_H
#define SCHEMA_H
#include <pthread.h>

#ifdef __cplusplus
 extern "C" {
#endif

/** Maximum size for schema name*/
#define MAX_NAME 128
/** Maximum number of schemas*/
#define MAX_SCHEMAS 20

/** when the human activates some schema from the gui */
#define GUIHUMAN -1 

/** when the human activates some schema from the shell */
#define SHELLHUMAN -2 

/** Possible schema's states*/
enum states {slept,active,notready,ready,forced,winner};
/** Schema state strings. */
extern const char  *const states_str[];/*Update list in schema.c if modified*/

/** Possible schema's gui states*/
enum guistates {off,on,pending_off,pending_on};
/** Callback function's type definition*/
typedef void (*intcallback)(int i);
/** Arbitration function's type definition*/
typedef void (*arbitration)(void);
/** Schema's run funcion's type definition*/
typedef void (*runFn)(int father, int *brothers, arbitration fn);
/** Schema's stop funcion's type definition*/
typedef void (*stopFn)(void);

/*forward declarations*/
struct JDEHierarchy;
struct JDEInterface;
struct JDEInterfacePrx;
typedef struct JDESchema_p JDESchema_p;

/** Jde schema type definition*/
typedef struct JDESchema{
   /** Dynamic library handler for the schema module*/
  void *handle;
  /** Schema's name*/
  char name[MAX_NAME];
  /** Schema's identifier*/
  int *id __attribute__((deprecated)); /* schema identifier */
  /** Schema's state
   * @see states
   */
  int state;
  /** Schema's gui state
   * @see guistates
   */
  int guistate;
  /** Indicates the schema's identifier of the father*/
  int father;
  /** The children of this schema must be set at 1 in this array*/
  int children[MAX_SCHEMAS];
  /** Contains the ips of the schema*/
  float fps;
  /** It is used to calculate the fps value, it must be incremented at each
   * schema iteration
   * @see speedcounter*/
  long int k;

  /** Hierarchy that owns this schema. It's NULL until a hierarchy add
      itself the schema.*/
  struct JDEHierarchy* hierarchy;

  /** Schema version control
   *  0.0 = old schemas
   */
  int version;
  int revision;

  /**
   * Pointer to the schema's init function
   * @param configfile Path and name to the config file of this schema.
   * @return void
   */
  void (*init)(char *configfile);
  /**
   * Pointer to the schema's terminate function
   * @return void
   */
  void (*terminate)(void);
  /**
   * Pointer to the schema's suspend function
   * @return void
   */
  void (*stop)(void);
  /**
   * Pointer to the schema's run function
   * @param father The father's schema identifier (the one who runs it)
   * @param brothers An array whith the schema's brothers
   * @param fn The arbitration function to decide wich of the brothers must run
   * @return void
   */
  void (*run)(int father, int *brothers, arbitration fn);
  /**
   * Pointer to the schema's show function, used to show its gui
   * @return void
   */
  void (*show)(void);
  /**
   * Pointer to the schema's hide function, used to hide its gui
   * @return void
   */
  void (*hide)(void);

  /** A mutex to protect critical regions that affect de schema*/
  pthread_mutex_t mymutex;
  /** A condition to wait until de conditions are the correct to run*/
  pthread_cond_t condition;
  /** Schema's thread identifier*/
  pthread_t mythread;
  struct JDESchema_p *priv;
}JDESchema;

/** Jde type definition for drivers and services */
typedef struct JDEDriver{
   /** Dynamic library handler for the driver module*/
   void *handle;
   /** Driver's name*/
   char name[MAX_NAME];
   /** Driver's identifier*/
   int id;
   
   /**
    * Pointer to the driver's init function
    * @param configfile Path and name to the config file of this driver.
    * @return void
    */
   void (*init)(char *configfile);
   /**
    * Pointer to the driver's terminate function
    * @return void
    */
   void (*terminate)(void);
}JDEDriver;

/********************************************************
  FIXME: all should be remove with another mechanism*/
/**
 * Get a schema using the id number
 * @param id schema id
 * @return pointer to schema or null
 */
extern JDESchema* get_schema(const int id) __attribute__((deprecated));

/** Array with all the loaded schemas*/
extern JDESchema all[MAX_SCHEMAS] __attribute__((deprecated));
/** Number of loaded schemas*/
extern int num_schemas __attribute__((deprecated));
/****************************************************/


/**
 * JDESchema constructor
 * @param name schema's name
 * @param init function pointer to initialize the schema
 * @param terminate function pointer to terminate the schema
 * @param stop function pointer to stop a schema
 * @param start function pointer to start a schema
 * @param show function pointer to show schema's gui
 * @param hide function pointer to hide schema's gui
 * @return the created schema on succes, 0 otherwise
 */
extern JDESchema* new_JDESchema(char* name,
				void (*init)(char *configfile),
				void (*terminate)(void),
				void (*stop)(void),
				void (*run)(int father, int *brothers, arbitration fn),
				void (*show)(void),
				void (*hide)(void));
/**
 * JDESchema destructor
 * not implemented yet.We simply terminate the schema
 * @param self schema to be destroyed.
 */
extern void delete_JDESchema(JDESchema *const self);

/**
 * Initialize schema
 * @param self schema to be initialized
 * @param configfile configuration file path
 * @return 1 on succesful, <0 otherwise
 */
extern int JDESchema_init(JDESchema* const self, char *configfile);

/**
 * Terminate schema
 * @param self schema to be terminated
 * @return 1 on succesful, <0 otherwise
 */
extern int JDESchema_terminate(JDESchema* const self);

/**
 * Stop schema
 * @param self schema to be stopped
 * @return 1 on succesful, <0 otherwise
 */
extern int JDESchema_stop(JDESchema* const self);

/**
 * Run schema
 * @param self schema to be terminated
 * @param father schema starting this schema. Could be 0 if the schema
 * is started stand alone.
 * @return 1 on succesful, <0 otherwise
 */
extern int JDESchema_run(JDESchema* const self, JDESchema* const father);

/**
 * Show schema gui
 * @param self schema
 * @return 1 on succesful, <0 otherwise
 */
extern int JDESchema_show(JDESchema* const self);

/**
 * Hide schema gui
 * @param self schema
 * @return 1 on succesful, <0 otherwise
 */
extern int JDESchema_hide(JDESchema* const self);

/**
 * Get schema state
 * @param self schema
 * @return schema state
 */
extern int JDESchema_state_get(JDESchema* const self);

/**
 * Set schema state
 * @param self schema
 * @param newstate state that will be set
 * @return 1 on succesful, <0 otherwise
 */
extern int JDESchema_state_set(JDESchema* const self, int newstate);
//void JDESchema_wait_statechange(JDESchema* const self);

/**
 * Update schema speed counter
 * @param self schema
 * @return speed counter new value
 */
extern int JDESchema_speedcounter(JDESchema* const self);

/**
 * Add an interface this supplied by this schema
 * @param self schema
 * @param interface interface that will be added
 * @return 1 on succesful, <0 otherwise
 */
extern int JDESchema_interface_add(JDESchema* const self, struct JDEInterface *const interface);

/**
 * Delete an interface supplied by this schema
 * @param self schema
 * @param interface interface that will be deleted
 * @return 1 on succesful, <0 otherwise
 */
extern int JDESchema_interface_del(JDESchema* const self, struct JDEInterface *const interface);

/**
 * Returns a list with all the interfaces that this schema supplies.
 * @param self schema
 * @return allocated array with all the interfaces. Last element is 0.
 * List must be deallocated with free.
 */
extern struct JDEInterface **JDESchema_interface_list(JDESchema* const self);


/**
 * Add an interfaceprx used by this schema
 * @param self schema
 * @param iprx interface proxy
 * @return 1 on succesful, <0 otherwise
 */
extern int JDESchema_interfaceprx_add(JDESchema* const self, struct JDEInterfacePrx *const iprx);

/**
 * Delete an interfaceprx used by this schema
 * @param self schema
 * @param iprx interface proxy
 * @return 1 on succesful, <0 otherwise
 */
extern int JDESchema_interfaceprx_del(JDESchema* const self, struct JDEInterfacePrx *const iprx);

/**
 * Returns a list with all the interface proxies that this schema is using.
 * @param self schema
 * @return allocated array with all the interfaceprx. Last element is
 * 0.List must be deallocated with free.
 */
extern struct JDEInterfacePrx **JDESchema_interfaceprx_list(JDESchema* const self);


/*JDEDriver are supplied for backwards compatibility,
  they will be remove when every entity is a schema*/
extern int JDEDriver_init(JDEDriver* const self, char *configfile);
extern int JDEDriver_terminate(JDEDriver* const self);


#ifdef __cplusplus
 }
#endif
#endif /*SCHEMA_H*/
