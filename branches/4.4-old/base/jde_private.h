/*
 *
 *  Copyright (C) 1997-2009 JDE Developers Team
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
 *            David Lobato Bravo <dav.lobato@gmail.com>
 *
 */
#ifndef __JDE_PRIVATE_H__
#define __JDE_PRIVATE_H__

/**
 * Init jde loading configuration file (given or default one).
 * This function must be used before to use jde.
 * @param cf if not null used as path for config file.
 * @return 1 if jde initialization success
 */
extern int jdeinit(const char* cf);

/**
 * Parse a config file
 * @param cf config file to parse
 * @return 1 if parsing ok
 */
extern int parse_configfile(const char* cf);

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

/* /\** */
/*  * Get schema state */
/*  * @param s schema */
/*  * @return schema state */
/*  *\/ */
/* extern int get_state(const JDESchema* s); */

/* /\** */
/*  * Set schema state */
/*  * @param s schema to be set */
/*  * @param newstate Schema's new states */
/*  * @return void */
/*  *\/ */
/* extern void set_state (JDESchema* s,const int newstate); */

/* /\** */
/*  * This functions must be called at every schema's iteration, it increments the */
/*  * schema's iteration counter to calculate schema's ips. */
/*  * @param s Schema */
/*  * @return void */
/*  *\/ */
/* extern void speedcounter2(JDESchema* s); */

#endif /*__JDE_PRIVATE_H__*/
