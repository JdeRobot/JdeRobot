/*
 *  Copyright (C) 2006 José María Cañas Plaza 
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Authors : José María Cañas Plaza <jmplaza@gsyc.escet.urjc.es>
 */

#ifndef __JDE_H__
#define __JDE_H__

#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <math.h>

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
/** Schema's resume funcion's type definition*/
typedef void (*resumeFn)(int father, int *brothers, arbitration fn);
/** Schema's suspend funcion's type definition*/
typedef void (*suspendFn)(void);

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
extern void put_state(int numschema,int newstate);
/**
 * This functions must be called at every schema's iteration, it increments the
 * schema's iteration counter to calculate schema's ips.
 * @param numschema Schema's identifier
 * @return void
 */
extern void speedcounter(int numschema);
/**
 * It is used to share variables and functions between diferent schemas and
 * drivers (remember that they are in diferent names' spaces)
 * @param schema String containing the exporter schema's name
 * @param name String containing the exported variable's names
 * @param p Pointer to the variable or function casted to (void *)
 * @return 1 if the variables was correctly exported, othewise 0
 */
extern int myexport(char *schema, char *name, void *p);
/**
 * Get the pointer to a variable shared with myexport
 * @param schema String containing the exporter schema's name
 * @param name String containing the exported variable's names
 * @return The pointer to the variable casted to (void *)
 */
extern void *myimport(char *schema, char *name);
/**
 * This function must be used instead of exit() to terminate de program
 * correctly
 * @param sig The same as the status argument at exit function
 * @return void
 */
extern void jdeshutdown(int sig);


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
  char name[100];
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
   * Pointer to the schema's startup function
   * @return void
   */
  void (*startup)(void);
  /**
   * Pointer to the schema's close function
   * @return void
   */
  void (*close)(void);
  /**
   * Pointer to the schema's suspend function
   * @return void
   */
  void (*suspend)(void);
  /**
   * Pointer to the schema's resume function
   * @param father The father's schema identifier (the one who resumes it)
   * @param brothers An array whith the schema's brothers
   * @param fn The arbitration function to decide wich of the brothers must run
   * @return void
   */
  void (*resume)(int father, int *brothers, arbitration fn);
  /**
   * Pointer to the schema's guiresume function, used to show it's gui
   * @return void
   */
  void (*guiresume)(void);
  /**
   * Pointer to the schema's guisuspend function, used to hide it's gui
   * @return void
   */
  void (*guisuspend)(void);

  /** A mutex to protect critical regions that affect de schema*/
  pthread_mutex_t mymutex;
  /** A condition to wait until de conditions are the correct to run*/
  pthread_cond_t condition;
  /** Schema's thread identifier*/
  pthread_t mythread;
}JDESchema;

/** Array with all the loaded schemas*/
extern JDESchema all[MAX_SCHEMAS];
/** Number of loaded schemas*/
extern int num_schemas;


/** Jde driver type definition*/
typedef struct {
   /** Dynamic library handler for the driver module*/
   void *handle;
   /** Driver's name*/
   char name[100];
   /** Driver's identifier*/
   int id;
   
   /**
    * Pointer to the driver's startup function
    * @param configfile Path and name to the config file of this driver.
    * @return void
    */
   void (*startup)(char *configfile);
   /**
    * Pointer to the schema's close function
    * @return void
    */
   void (*close)(void);
}JDEDriver;

#ifdef __cplusplus
 }
#endif

#endif
