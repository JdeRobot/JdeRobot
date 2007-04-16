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
#include <pioneer.h>

#define DEGTORAD     (3.14159264 / 180.0)
#define RADTODEG     (180.0 /3.14159264)
#ifndef TRUE
#define TRUE         1
#endif
#ifndef FALSE
#define FALSE        0
#endif

enum states {slept,active,notready,ready,forced,winner};
enum guistates {off,on,pending_off,pending_on};
typedef void (*intcallback)(int i);
typedef void (*arbitration)(void);
typedef void (*resumeFn)(int father, int *brothers, arbitration fn);
typedef void (*suspendFn)(void);
extern void null_arbitration();
extern void put_state(int numschema,int newstate);
extern void speedcounter(int numschema);
extern int myexport(char *schema, char *name, void *p);
extern void *myimport(char *schema, char *name);
extern void jdeshutdown(int sig);
#define GUIHUMAN -1 /* when the human activates some schema from the gui */
#define SHELLHUMAN -2 /* when the human activates some schema from the shell */

#define MAX_SCHEMAS 20
typedef struct {
  void *handle;
  char name[100];
  int *id; /* schema identifier */
  int state;
  int guistate;
  int father; 
  int children[MAX_SCHEMAS];
  float fps;
  long int k;
  
  void (*startup)(void);
  void (*close)(void);
  void (*suspend)(void);
  void (*resume)(int father, int *brothers, arbitration fn);
  void (*guiresume)(void);
  void (*guisuspend)(void);
 
  pthread_mutex_t mymutex;
  pthread_cond_t condition;
  pthread_t mythread;
}JDESchema;

extern JDESchema all[MAX_SCHEMAS];
extern int num_schemas;


typedef struct {
  void *handle;
  char name[100];
  int id;
  void (*startup)(char *configfile);
  /* the resume and suspend functions are sensors&motors oriented for schema programmers. Each drivers provides a resume/suspend pair for each sensor or motor it provides */
  void (*close)(void);
}JDEDriver;
