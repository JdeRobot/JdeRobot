/*
 *  jdeneo.c
 *
 *  Copyright (C) 2006-2007 Jesús Ruiz-Ayúcar and José María Cañas
 *  for the Universidad Rey Juan Carlos robotic group.
 *  http://www.robotica-urjc.es
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef JDE
#define JDE
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
/* CHUSO */
#include <gmodule.h>
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#define DEGTORAD     (3.14159264 / 180.0)
#define RADTODEG     (180.0 /3.14159264)
#define MAX_USERS 3

/* Para la GUI GTK+ */
#define GUIFILE "/jde.glade"
#define IMAGEAFILE "/pixmaps/imageA.png"
#define IMAGEBFILE "/pixmaps/imageB.png"
#define ICONFILE "/pixmaps/peloto.png"
#define SPLASHFILE "/pixmaps/splash.png"

#ifndef TRUE
#define TRUE         1
#endif
#ifndef FALSE
#define FALSE        0
#endif

#ifndef tvoxel
#define tvoxel
typedef struct voxel{
  float x;
  float y;
} Tvoxel;
#endif

typedef void (*intcallback)(int i);
typedef void (*arbitration)(void);
extern  void null_arbitration();
extern  void put_state(int numschema,int newstate);

typedef enum { SCH_SONARS,
               SCH_LASER,
               SCH_ENCODERS,
               SCH_PANTILTENCODERS,
               SCH_IMAGEA,
               SCH_IMAGEB,
               SCH_MOTORS,
               SCH_PANTILTMOTORS,
               NUM_BASICSCHEMAS,
               SCH_GUIXFORMS,
               SCH_MYSCHEMA,
               NUM_SCHEMAS} Schemas;
extern int debug[NUM_SCHEMAS];
extern int state[NUM_SCHEMAS];
enum states {slept,active,notready,ready,forced,winner,checking};
extern pthread_mutex_t mymutex[NUM_SCHEMAS];
extern pthread_cond_t condition[NUM_SCHEMAS];
extern pthread_t schema[NUM_SCHEMAS];

extern void jdeshutdown(int sig);
extern void us2xy(int numsensor, float d,float phi, Tvoxel *point);
extern void laser2xy(int reading, float d, Tvoxel *point);
extern void laser2robot(int reading, float d, Tvoxel *point);
extern void robot2xy(float x, float y, Tvoxel *point);
extern void loadConfig();
int jdeinit(void);

extern float fpsA, fpsB, fpssonars, fpsencoders, fpslaser, fpspantiltencoders, fpsmotors, fpspantiltmotors;
extern int ksonars, klaser, kencoders, kmotors;
extern char *greyA;
extern char *greyB;
extern char *name[NUM_SCHEMAS];

extern GladeXML *xml;

/***************** API of variables ***************/
#define NUM_LASER          180
#define NUM_SONARS         16
#define NUM_BUMPERS        10
#define MAX_VEL            1000     /* mm/sec, hardware limit: 1800 */
#define MAX_RVEL           180      /* deg/sec, hardware limit: 360 */

/* SIF image size */
#define SIFNTSC_ROWS       240
#define SIFNTSC_COLUMNS    320

/* directed perception pantilt limits */
#define MAX_PAN_ANGLE      158.     /* degrees */
#define MIN_PAN_ANGLE      -158.    /* degrees */
#define MAX_TILT_ANGLE     30.      /* degrees */
#define MIN_TILT_ANGLE     -46.     /* degrees */
#define MAX_SPEED_PANTILT  205.89

extern float laser_coord[5];
extern float us_coord[NUM_SONARS][5];  /* sensor positions */
extern float camera_coord[5];          /* camera position */

extern float robot[5]; 
extern unsigned long int encoders_clock;
extern void encoders_suspend(int i);
extern int encoders_resume(arbitration fn);

extern int laser[NUM_LASER];
extern unsigned long int laser_clock;
extern void laser_suspend(int i);
extern int laser_resume(arbitration fn);

extern float us[NUM_SONARS];
extern unsigned long int us_clock[NUM_SONARS];
extern void sonars_suspend(int i);
extern int sonars_resume(intcallback fn);

extern guchar *colorA; /* sifntsc image itself */
extern unsigned long int imageA_clock;
extern int imageA_resume(arbitration fn);
extern void imageA_suspend(int);

extern guchar *colorB; /* sifntsc image itself */
extern unsigned long int imageB_clock;
extern int imageB_resume(arbitration fn);
extern void imageB_suspend(int);

extern float pan_angle, tilt_angle;  /* degs */
extern unsigned long int pantiltencoders_clock;
extern int pantiltencoders_resume(arbitration fn);
extern void pantiltencoders_suspend(int);

extern float v; /* mm/s */
extern float w; /* deg/s*/
extern void motors_suspend();
extern void motors_resume(int *brothers, arbitration fn);
extern int motors_cycle;

extern float longitude; /* degs, pan angle */
extern float latitude;  /* degs, tilt angle */
extern void pantiltmotors_suspend();
extern void pantiltmotors_resume(int *brothers, arbitration fn);
extern float longitude_speed;
extern float latitude_speed;
extern int pantiltmotors_cycle;

extern gchar **directories;

/* Tipo para la estructura que guarda la lista de variables de cada esquema */
typedef struct _schema_vars schema_vars;

struct _schema_vars {
   gchar *id;
   gchar *varname;
   void  *var;
};

extern GSList *schemas;
extern GSList *vars;

enum
{
   COL_NAME = 0,
   COL_SHOW,
   COL_PLAY,
   COL_IPS,
   COL_STATE,
   NUM_COLS
} ;

GtkTreeStore         *treestore;
GtkTreeView          *view;
#include "gui.h"
#include "schemas.h"
#endif
