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
 *  Authors : Javier Martin Ramos <xaverbrennt@yahoo.es>
 *            Jose Antonio Santos Cadenas  <santoscadenas@gmail.com>
 *
 */

/**
 *  jdec x10 driver provides support for x10 tecnology using heyu program
 *
 *  @file x10.c
 *  @author Jose Antonio Santos Cadenas  <santoscadenas@gmail.com>
 *  @version 0.1
 *  @date 2008-03-31
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <jde.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdarg.h>
#include <sys/wait.h>

#include "x10.h"


/* x10 driver API options */
/** x10 driver name.*/
char *driver_name="x10";

/** x10 schema identifier*/
int x10_id;

/** Mutex to control concurrent access to ref counters*/
pthread_mutex_t refmutex;
/** Indicates if x10 schema is active*/
int x10_active=0;

/** Mutex to control concurrent access to the hardware*/
pthread_mutex_t hardware_mutex;

/** x10 schema clock*/
unsigned long int x10_clock=0;

t_iface interface;

int set_property(char *unit, char *property, ...){
   va_list args;
   int pid;
   int num;
   char c_num[5];
   int status;

   pthread_mutex_lock(&refmutex);
   if (x10_active<=0){
      return -10;
   }
   pthread_mutex_unlock(&refmutex);

   pthread_mutex_lock(&hardware_mutex);
   va_start(args, property);

   if (!(pid=fork())){
      /*Close child stdout and stdin*/
      int file = open("/dev/null",O_RDWR);
      close(0); dup(file);
      close(1); dup(file);
      close(2); dup(file);
      /*Child executes heyu program*/
      if (strcmp(property, "bright")==0 || strcmp(property, "dim")==0){
         num=va_arg(args, int);
         snprintf(c_num, 5, "%d", num);
         va_end(args);
         execlp("heyu","heyu", property, unit, c_num ,NULL);
         exit(10);
      }
      else{
         va_end(args);
         execlp("heyu","heyu", property, unit, NULL);
         exit(10);
      }
   }
   else{
     waitpid(pid, &status, 0);
   }
   va_end(args);
   pthread_mutex_unlock(&hardware_mutex);

   return status;
}

/*DRIVER FUNCTIONS*/
/** x10 run function following jdec platform schemas API.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int x10_run(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   x10_active++;
   if ((all[x10_id].father==GUIHUMAN) ||
        (all[x10_id].father==SHELLHUMAN))
      all[x10_id].father = father;
   if(x10_active==1)
   {
      pthread_mutex_unlock(&refmutex);
      /*printf("colorA schema run (mplayer driver)\n");*/
      all[x10_id].father = father;
      all[x10_id].fps = 0.;
      all[x10_id].k =0;
      put_state(x10_id,winner);
   }
   else
      pthread_mutex_unlock(&refmutex);
   return 0;
}

/** x10 stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
int x10_stop(){
   pthread_mutex_lock(&refmutex);
   x10_active--;
   if(x10_active==0){
      pthread_mutex_unlock(&refmutex);
      put_state(x10_id,slept);
   }
   else
      pthread_mutex_unlock(&refmutex);
   return 0;
}

/** x10 driver function to finish the driver*/
void x10_terminate(){
   printf("driver %s off\n", driver_name);
}

/** x10 driver init function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void x10_init(char *configfile)
{
   interface.set_property=set_property;

   /*Activate the schema*/
   all[num_schemas].id = (int *) &x10_id;
   strcpy(all[num_schemas].name,"x10");
   all[num_schemas].run = (runFn) x10_run;
   all[num_schemas].stop = (stopFn) x10_stop;
   printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
   (*(all[num_schemas].id)) = num_schemas;
   all[num_schemas].fps = 0.;
   all[num_schemas].k =0;
   all[num_schemas].state=slept;
   all[num_schemas].terminate = NULL;
   all[num_schemas].handle = NULL;
   num_schemas++;
   myexport(all[x10_id].name,"id",&x10_id);
   myexport(all[x10_id].name,"run",(void *)x10_run);
   myexport(all[x10_id].name,"stop",(void *)x10_stop);
   myexport(all[x10_id].name,"x10", (void*)&interface);
}
