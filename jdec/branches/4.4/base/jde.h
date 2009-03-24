/*
 *
 *  Copyright (C) 1997-2008 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Authors : José María Cañas Plaza <jmplaza@gsyc.escet.urjc.es>
 *
 */

#ifndef __JDE_H__
#define __JDE_H__

#include <sys/time.h>
#include <time.h>
#include <pthread.h>

/** Conversion from deg to rad*/
#define DEGTORAD     (3.14159264 / 180.0)
/** Conversion from rad to deg*/
#define RADTODEG     (180.0 /3.14159264)
#ifndef TRUE
/** True value*/
#define TRUE         1
#endif
#ifndef FALSE
/** False value*/
#define FALSE        0
#endif

/** Maximum buffer size (for strings)*/
#define MAX_BUFFER 512
#define MAX_NAME 128

#ifdef __cplusplus
 extern "C" {
#endif

/** Possible schema's states*/
enum states {slept,active,notready,ready,forced,winner};
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

/**
 * Null arbitrarion function
 * @return void
 */
extern void null_arbitration();
/**
 * Puts the state in newstate to the schema idexed by numschema
 * @param newstate Schema's new states
 * @param numschema Schema's index in the schema's array
 * @return void
 */
extern void put_state(const int numschema, const int newstate);
/**
 * This functions must be called at every schema's iteration, it increments the
 * schema's iteration counter to calculate schema's ips.
 * @param numschema Schema's identifier
 * @return void
 */
extern void speedcounter(const int numschema);
/**
 * It is used to share variables and functions between diferent schemas and
 * drivers (remember that they are in diferent names' spaces)
 * @param schema String containing the exporter schema's name
 * @param name String containing the exported variable's names
 * @param p Pointer to the variable or function casted to (void *)
 * @return 1 if the variables was correctly exported, othewise 0
 */
extern int myexport(const char *schema, const char *name, void *p);
/**
 * Get the pointer to a variable shared with myexport
 * @param schema String containing the exporter schema's name
 * @param name String containing the exported variable's names
 * @return The pointer to the variable casted to (void *)
 */
extern void *myimport(const char *schema, const char *name);
/**
 * This function must be used instead of exit() to terminate de program
 * correctly
 * @param sig The same as the status argument at exit function
 * @return void
 */
extern void jdeshutdown(const int sig);

/**
 * Get the pointer to global configfile
 * @return pointer to global configfile
 */
extern char* get_configfile();


/** when the human activates some schema from the gui */
#define GUIHUMAN -1 

/** when the human activates some schema from the shell */
#define SHELLHUMAN -2 

/** Maximum number of schemas*/
#define MAX_SCHEMAS 20
/** Jde schema type definition*/
typedef struct {
   /** Dynamic library handler for the schema module*/
  void *handle;
  /** Schema's name*/
  char name[MAX_NAME];
  /** Schema's identifier*/
  int *id; /* schema identifier */
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
}JDESchema;

/**
 * Get a schema using the id number
 * @param id schema id
 * @return pointer to schema or null
 */
extern JDESchema* get_schema(const int id);

/** Array with all the loaded schemas*/
extern JDESchema all[MAX_SCHEMAS];
/** Number of loaded schemas*/
extern int num_schemas;


/** Jde type definition for drivers and services */
typedef struct {
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

#ifdef __cplusplus
 }
#endif

#endif
