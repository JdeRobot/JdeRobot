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
#include <schema.h>

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

#ifdef __cplusplus
extern "C" {
#endif



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
extern void put_state(const int numschema, const int newstate) __attribute__((deprecated));
/**
 * This functions must be called at every schema's iteration, it increments the
 * schema's iteration counter to calculate schema's ips.
 * @param numschema Schema's identifier
 * @return void
 */
extern void speedcounter(const int numschema) __attribute__((deprecated));

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
 * Init jde loading configuration file (given or default one).
 * This function must be used before to use jde.
 * @param argc argument count
 * @param argv argument array
 * @param cf if not null used as path for config file.
 * @return 1 if jde initialization success
 */
extern int jdeinit(int argc, char** argv, const char* cf);


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

/**
 * Load the so named as a schema
 * @param name shared object to load without .so
 * @return loaded schema if ok, null otherwise
 */
extern JDESchema* jde_loadschema(const char *name);

/**
 * Load the so named as a driver/service
 * @param name shared object to load without .so
 * @return loaded driver/service if ok, null otherwise
 */
extern JDEDriver* jde_loaddriver(const char *name);


/**
 * Find schema by name
 * @param name schema to be searched
 * @return schema object or null if can't be founded it
 */
extern JDESchema* find_schema (const char *name);

/**
 * Parse a config file
 * @param cf config file to parse
 * @return 1 if parsing ok
 */
extern int parse_configfile(const char* cf);


#ifdef __cplusplus
 }
#endif

#endif
