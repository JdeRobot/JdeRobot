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

#define DEGTORAD     (3.14159264 / 180.0)
#define RADTODEG     (180.0 /3.14159264)
#ifndef TRUE
#define TRUE         1
#endif
#ifndef FALSE
#define FALSE        0
#endif
 

enum states {slept,active,notready,ready,forced,winner};
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
#define GUIHUMAN -1 /* when the human activate some schema from the gui */

#define MAX_SCHEMAS 100
typedef struct {
  void *handle;
  char name[100];
  int *id; /* schema identifier */
  int state;
  int father; 
  int children[MAX_SCHEMAS];
  float fps;
  int k;
  
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

typedef int(*fn)(); /* for resume and suspend funcions of sensors and motors */


#ifndef tvoxel
#define tvoxel
typedef struct voxel{
  float x;
  float y;}Tvoxel;
#endif

extern void us2xy(int numsensor, float d,float phi, Tvoxel *point);
extern void laser2xy(int reading, float d, Tvoxel *point);


/***************** Robot Configuration ***************/
#define NUM_LASER 180
#define NUM_SONARS 16
#define NUM_BUMPERS 10
#define MAX_VEL 1000 /* mm/sec, hardware limit: 1800 */
#define MAX_RVEL 180 /* deg/sec, hardware limit: 360 */
/* SIF image size */
#define SIFNTSC_ROWS 240
#define SIFNTSC_COLUMNS 320
/* directed perception pantilt limits */
#define MAX_PAN_ANGLE 158. /* degrees */
#define MIN_PAN_ANGLE -158. /* degrees */
#define MAX_TILT_ANGLE 30. /* degrees */
#define MIN_TILT_ANGLE -46. /* degrees */
#define MAX_SPEED_PANTILT 205.89

extern float laser_coord[5]; /* laser sensor position */
extern float us_coord[NUM_SONARS][5];/* us sensor positions */
extern float camera_coord[5]; /* camera position */


/***************** API of variables ***************/
extern float jde_robot[5]; /* odometry information */
extern fn encoders_suspend;
extern fn encoders_resume;
extern unsigned long int encoders_clock;
extern float fpsencoders;
extern int kencoders;

extern int jde_laser[NUM_LASER];
extern fn laser_suspend;
extern fn laser_resume;
extern unsigned long int laser_clock;
extern float fpslaser;
extern int klaser;

extern float us[NUM_SONARS];
extern fn sonars_suspend;
extern fn sonars_resume;
extern unsigned long int us_clock[NUM_SONARS];
extern float fpssonars;
extern int ksonars;

extern char *colorA; /* sifntsc image itself */
extern fn imageA_resume;
extern fn imageA_suspend;
extern unsigned long int imageA_clock;
extern float fpsA;
extern int kA;

extern char *colorB; /* sifntsc image itself */
extern fn imageB_resume;
extern fn imageB_suspend;
extern unsigned long int imageB_clock;
extern float fpsB;
extern int kB;

extern char *colorC; /* sifntsc image itself */
extern fn imageC_resume;
extern fn imageC_suspend;
extern unsigned long int imageC_clock;
extern float fpsC;
extern int kC;

extern char *colorD; /* sifntsc image itself */
extern fn imageD_resume;
extern fn imageD_suspend;
extern unsigned long int imageD_clock;
extern float fpsD;
extern int kD;

extern float pan_angle, tilt_angle;  /* degs */
extern fn pantiltencoders_resume;
extern fn pantiltencoders_suspend;
extern unsigned long int pantiltencoders_clock;
extern float fpspantiltencoders;
extern int kpantiltencoders;

extern float v; /* mm/s */
extern float w; /* deg/s*/
extern fn motors_suspend;
extern fn motors_resume;
extern int motors_cycle;
extern float fpsmotors;
extern int kmotors;

extern float longitude; /* degs, pan angle */
extern float latitude; /* degs, tilt angle */
extern fn pantiltmotors_suspend;
extern fn pantiltmotors_resume;
extern float longitude_speed;
extern float latitude_speed;
extern int pantiltmotors_cycle;
extern float fpspantiltmotors;
extern int kpantiltmotors;

