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
 *  Authors : Teodoro Gonzalez sanchez <tgonzale@gsyc.es>
 *            José María Cañas <jmplaza@gsyc.es>
 *
 */

/************************************************
 * jdec gazebo driver                           *
 ************************************************/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <gazebo.h>
#include <jde.h>
#include <interfaces/varcolor.h>

/*
In gazebo.h: 
GZ_LASER_MAX_RANGES
GZ_SONAR_MAX_RANGES
*/
#define NUM_LASER 180
#define NUM_SONARS 16
#define NUM_BUMPERS 10
#define MAX_VEL 1000 /* mm/sec, hardware limit: 1800 */
#define MAX_RVEL 180 /* deg/sec, hardware limit: 360 */
/* SIF image size */
#define SIFNTSC_ROWS 240
#define SIFNTSC_COLUMNS 320


/** The driver's command thread cycle*/
#define GAZEBO_COMMAND_CYCLE 100	/* ms */
/** The driver's main cycle*/
#define GAZEBO_CYCLE 33 /*ms*/
/** Maximum number of cameras*/
#define MAXCAM 8
/** Max drivers name lenth*/
#define MAX_MODEL_ID	100 


gz_client_t *client = NULL;

gz_position_t *position = NULL;
gz_camera_t *camera[MAXCAM];
gz_stereo_t *stereo = NULL;
gz_laser_t *laser = NULL;
gz_sonar_t *sonar = NULL;
gz_ptz_t *ptz = NULL;


typedef struct gazebo_camera_name
{
  /* tipo: 0 unassigned, 1 camera, 2 stereo left, 3 stereo right,
           4 left disparity, 5 right disparity */
  int tipo;
  /* camara or stereo head gazebo id. NULL when not asigned */
  char name[MAX_MODEL_ID];	
} gc_name,* pgc_name;

/* The gazebo device names for jde laser, sonar etc. For cameras, 
   color_name says which gazebo source feeds each jde camera */
char laser_name[MAX_MODEL_ID] = "";
char sonar_name[MAX_MODEL_ID] = "";
char position_name[MAX_MODEL_ID] = "";
char motors_name[MAX_MODEL_ID] = "";
char ptz_name[MAX_MODEL_ID]="";
gc_name color_name[MAXCAM]; 

/* Variables put to 0.0 and no change during the execution */
float correcting_x = 0.; /* mm */
float correcting_y = 0.; /* mm */
float correcting_theta = 0.; /* deg */

/* Gazebo Server and client */
int server_id = 0;
int client_id = 0;


/* Jde and drivers stuff declarations */
pthread_t gazebo_th;
void *gazebo_thread (void *not_used);

int laser_schema_id,
  sonars_schema_id,
  encoders_schema_id,
  motors_schema_id,
  camera_schema_id[MAXCAM],
  ptmotors_schema_id,
  ptencoders_schema_id;

int gazebo_encoders_run (int father, int *brothers, arbitration fn);
int gazebo_encoders_stop ();
int gazebo_motors_run (int father, int *brothers, arbitration fn);
int gazebo_motors_stop ();
int gazebo_camera0_run (int father, int *brothers, arbitration fn);
int gazebo_camera0_stop ();
int gazebo_camera1_run (int father, int *brothers, arbitration fn);
int gazebo_camera1_stop ();
int gazebo_camera2_run (int father, int *brothers, arbitration fn);
int gazebo_camera2_stop ();
int gazebo_camera3_run (int father, int *brothers, arbitration fn);
int gazebo_camera3_stop ();
int gazebo_camera4_run (int father, int *brothers, arbitration fn);
int gazebo_camera4_stop ();
int gazebo_camera5_run (int father, int *brothers, arbitration fn);
int gazebo_camera5_stop ();
int gazebo_camera6_run (int father, int *brothers, arbitration fn);
int gazebo_camera6_stop ();
int gazebo_camera7_run (int father, int *brothers, arbitration fn);
int gazebo_camera7_stop ();
int gazebo_laser_run (int father, int *brothers, arbitration fn);
int gazebo_laser_stop ();
int gazebo_sonars_run (int father, int *brothers, arbitration fn);
int gazebo_sonars_stop ();
int gazebo_ptmotors_run (int father, int *brothers, arbitration fn);
int gazebo_ptmotors_stop ();
int gazebo_ptencoders_run (int father, int *brothers, arbitration fn);
int gazebo_ptencoders_stop ();

/* Functions to do the data transfer job */
void gazebo_laser_callback ();
void gazebo_camera_callback (int camnum);
void gazebo_ptmotors_callback ();
void gazebo_ptencoders_callback ();
void gazebo_sonars_callback ();
void gazebo_encoders_callback ();
void gazebo_motors_callback ();


/** pthread state variable.*/
int state;
/** mutex for the thread of this driver.*/
pthread_mutex_t mymutex;
/** condition for the thread of this driver.*/
pthread_cond_t condition;


char driver_name[256] = "gazebo";
int gazebo_terminate_command = 0;

int serve_laser = 0, 
  serve_encoders = 0, 
  serve_sonars = 0,
  serve_motors = 0,
  serve_ptmotors = 0,
  serve_ptencoders = 0,
  serve_color[MAXCAM];

int laser_active = 0,
  encoders_active = 0,
  sonars_active = 0, 
  motors_active = 0,
  ptmotors_active = 0,
  ptencoders_active = 0,
  color_active[MAXCAM];



/*Variables a exportar*/
char *colorA; /** 'colorA' schema image data*/
char *colorB; /** 'colorB' schema image data*/
char *colorC; /** 'colorC' schema image data*/
char *colorD;/** 'color' schema image data*/
Varcolor myA,myB,myC,myD; /* for varcolorA,varcolorB... */
float v; /* mm/s */
float w; /* deg/s*/
float jde_robot[5];
float us[NUM_SONARS];
int jde_laser[NUM_LASER];
float pan_angle, tilt_angle;  /* degs */
float longitude; /* degs, pan angle */
float latitude; /* degs, tilt angle */
/** 'ptmotors' schema latitude and longitude speed control*/
float longitude_speed;
float latitude_speed;

/** gazebo pantilt max pan, degrees.*/
float max_pan = 54;
/** gazebo pantilt min pan, degrees.*/
float min_pan = -54;
/** gazebo pantilt max tilt, degrees.*/
float max_tilt = 44;
/** gazebo pantilt min tilt, degrees.*/
float min_tilt = -44;
/** Max longitude speed, degrees/sec. It is irrelevant as gazebo implements pantilt speed internally, so this parameters has no effect */
float max_longitude_speed = 20;
/** Max latitude speed, degrees/sec. It is irrelevant as gazebo implements pantilt speed internally, so this parameters has no effect */
float max_latitude_speed = 20;


int should_driver_stop()
{
  int i,shouldstop=1;

  if ((laser_active)||(sonars_active)||
      (encoders_active)||(motors_active)||
      (ptmotors_active)||(ptencoders_active))
    shouldstop=0;
  
  for(i=0;i<MAXCAM;i++)
    if (color_active[i]) shouldstop=0;

  if (shouldstop)
    {
      pthread_mutex_lock(&mymutex);
      state=slept;
      pthread_mutex_unlock(&mymutex);
    }
  return shouldstop;
}

int should_driver_restart()
{
  /* there is a race condition in the reading of the state variable
     here, but we can live with it */
  if (state==slept)
    {
      /* gazebo thread goes winner */
      pthread_mutex_lock(&mymutex);
      state=winner;
      pthread_mutex_unlock(&mymutex);
      pthread_cond_signal(&condition);
      return 1;
    }
  else return 0;
}


int
gazebo_parseconf (char *configfile);

int
gazebo_init (char *configfile)
{
  int i;
  int stereo_not_init_yet=1;
  
  puts ("Starting gazebo driver");
  fflush (stdout);
  
  for(i=0;i<MAXCAM;i++)
    {
      camera[i]=NULL;
      color_name[i].tipo=0;
      serve_color[i]=0;
      color_active[i]=0;
      camera_schema_id[i]=0;
    }

  /* Parse the configuration file */
  if (gazebo_parseconf (configfile) == -1)
    {
      printf ("gazebo: driver not initialized. Configuration file parsing error.\n");
      exit (-1);
    }

  /* Connection with Gazebo simulator */
  printf ("Connecting to Gazebo Simulator\n");
  gz_error_init (1, 9);		/*Trazas de gazebo al m�ximo*/

  client = gz_client_alloc ();
  if (gz_client_connect_wait (client, server_id, client_id) != 0)
    {
      fprintf (stderr, "gazebo: Error connecting to gazebo server\n");
      exit (-1);
    }
  
  if (serve_ptencoders || serve_ptmotors)
    {
      ptz = gz_ptz_alloc();
      if (gz_ptz_open (ptz, client, ptz_name) != 0)
	{
	  fprintf (stderr, "Error opening \"%s\" ptz\n",ptz_name);
	  return (-1);
	}
    }

  if (serve_motors || serve_encoders)
    {
      position = gz_position_alloc ();
    if (gz_position_open (position, client, motors_name) != 0) {
      position=NULL;
      puts ("Error while opening position");
    }
  }

  for(i=0;i<MAXCAM;i++)
    {
      if (serve_color[i]) 
	{
	  if (color_name[i].tipo == 1)
	    {	  
	      camera[i] = gz_camera_alloc ();
	      if (gz_camera_open (camera[i], client, color_name[i].name) != 0)
		{
		  fprintf (stderr, "Error openning the \"%s\" camera\n",color_name[i].name);
		  exit (-1);
		}
	    }
	  else if ((color_name[i].tipo >1)&&(stereo_not_init_yet))
	    /* only one stereo device in this driver. The first jde-camera inits the stereo gazebo device */
	    {
	      stereo = gz_stereo_alloc ();
	      if (gz_stereo_open (stereo, client, color_name[i].name) != 0)
		{
		  fprintf (stderr, "Error opening  \"%s\" stereohead\n",color_name[i].name);
		  return (-1);
		}
	      stereo_not_init_yet=0;
	    }
	}
    }

  if (serve_sonars)
    {
      sonar = gz_sonar_alloc ();
      if (!sonar)
	puts ("error allocating sonar interface");
      else {/* Abrimos y activamos los sensores */
	if (gz_sonar_open (sonar, client, sonar_name) != 0)
	  {
	    fprintf (stderr, "Error opening sonars from %s robot\n",sonar_name);
	    return (-1);
	  }      
	sonar->data->cmd_enable_sonar = 1;
	//printf("El numero de sonars es %d \n",sonar->data->sonar_count);
      }
    }

  if (serve_laser)
    {
      laser = gz_laser_alloc ();
      if (gz_laser_open (laser, client, laser_name) != 0)
	{
	  fprintf (stderr, "Error openning the %s laser\n",laser_name);
	  return (-1);
	}
    }
 
  /* Set up the virtual schemas API */
  if (serve_motors)
    {
      all[num_schemas].id = (int *) &motors_schema_id;
      (*(all[num_schemas].id)) = num_schemas;
      strcpy (all[num_schemas].name, "motors");
      all[num_schemas].run = (runFn) gazebo_motors_run;
      all[num_schemas].stop = (stopFn) gazebo_motors_stop;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,num_schemas);
      num_schemas++;
      myexport("motors", "id", &motors_schema_id);
      myexport("motors", "v", &v);
      myexport("motors", "w", &w);
      myexport("motors", "run", (void *) &gazebo_motors_run);
      myexport("motors", "stop", (void *) &gazebo_motors_stop);
      v=0; w=0;
    }
  
  if (serve_laser)
    {
      all[num_schemas].id = (int *) &laser_schema_id;
      (*(all[num_schemas].id)) = num_schemas;
      strcpy (all[num_schemas].name, "laser");
      all[num_schemas].run = (runFn) gazebo_laser_run;
      all[num_schemas].stop = (stopFn) gazebo_laser_stop;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,num_schemas);
      num_schemas++;
      myexport ("laser", "id", &laser_schema_id);
      myexport ("laser", "laser", &jde_laser);
      myexport ("laser", "run", (void *) &gazebo_laser_run);
      myexport ("laser", "stop", (void *) &gazebo_laser_stop);
    }

  if (serve_ptmotors)
    {
      all[num_schemas].id = (int *) &ptmotors_schema_id;
      (*(all[num_schemas].id)) = num_schemas;
      strcpy (all[num_schemas].name, "ptmotors");
      all[num_schemas].run = (runFn) gazebo_ptmotors_run;
      all[num_schemas].stop = (stopFn) gazebo_ptmotors_stop;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,num_schemas);
      num_schemas++;
      myexport("ptmotors", "id", &ptmotors_schema_id);
      myexport("ptmotors", "longitude",&longitude);
      myexport("ptmotors", "latitude",&latitude);
      myexport("ptmotors", "longitude_speed",&longitude_speed);
      myexport("ptmotors", "latitude_speed",&latitude_speed);
      myexport("ptmotors", "max_longitude", &max_pan);
      myexport("ptmotors", "min_longitude", &min_pan);
      myexport("ptmotors", "max_latitude", &max_tilt);
      myexport("ptmotors", "min_latitude", &min_tilt);
      myexport("ptmotors", "max_longitude_speed", &max_longitude_speed);
      myexport("ptmotors", "max_latitude_speed", &max_latitude_speed);
      myexport("ptmotors","run",(void *)&gazebo_ptmotors_run);
      myexport("ptmotors", "stop",(void *)&gazebo_ptmotors_stop);
    }
  
  if (serve_ptencoders)
    {
      all[num_schemas].id = (int *) &ptencoders_schema_id;
      (*(all[num_schemas].id)) = num_schemas;
      strcpy (all[num_schemas].name, "ptencoders");
      all[num_schemas].run = (runFn) gazebo_ptencoders_run;
      all[num_schemas].stop = (stopFn) gazebo_ptencoders_stop;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,num_schemas);
      num_schemas++;
      myexport ("ptencoders", "id", &ptencoders_schema_id);
      myexport ("ptencoders", "pan_angle", &pan_angle);
      myexport ("ptencoders", "tilt_angle", &tilt_angle);
      myexport("ptencoders","run",(void *)&gazebo_ptencoders_run);
      myexport("ptencoders","stop",(void *)&gazebo_ptencoders_stop);
      }
  
  if (serve_sonars)
    {
      all[num_schemas].id = (int *) &sonars_schema_id;
      (*(all[num_schemas].id)) = num_schemas;
      strcpy (all[num_schemas].name, "sonars");
      all[num_schemas].run = (runFn) gazebo_sonars_run;
      all[num_schemas].stop = (stopFn) gazebo_sonars_stop;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,num_schemas);
      num_schemas++;
      myexport ("sonars", "id", &sonars_schema_id);
      myexport ("sonars", "us", &us);
      myexport ("sonars", "run", (void *) &gazebo_sonars_run);
      myexport ("sonars", "stop", (void *) &gazebo_sonars_stop);
    }

  if (serve_encoders)
    {
      all[num_schemas].id = (int *) &encoders_schema_id;
      (*(all[num_schemas].id)) = num_schemas;
      strcpy (all[num_schemas].name, "encoders");
      all[num_schemas].run = (runFn) gazebo_encoders_run;
      all[num_schemas].stop = (stopFn) gazebo_encoders_stop;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,num_schemas);
      num_schemas++;
      myexport ("encoders", "id", &encoders_schema_id);
      myexport ("encoders", "jde_robot", &jde_robot);
      myexport ("encoders", "run", (void *) &gazebo_encoders_run);
      myexport ("encoders", "stop", (void *) &gazebo_encoders_stop);
    }

  if (serve_color[0])
    {
      colorA = (char*)malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3*sizeof(char));
      all[num_schemas].id = (int *) &camera_schema_id[0];
      (*(all[num_schemas].id)) = num_schemas;
      strcpy (all[num_schemas].name, "colorA");
      all[num_schemas].run = (runFn) gazebo_camera0_run;
      all[num_schemas].stop = (stopFn) gazebo_camera0_stop;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,num_schemas);
      num_schemas++;
      myexport("colorA","id",&camera_schema_id[0]);
      myexport("colorA","colorA",&colorA);
      myexport("colorA","run",(void *)gazebo_camera0_run);
      myexport("colorA","stop",(void *)gazebo_camera0_stop);
    }

  if (serve_color[1])
    {
      colorB = (char*)malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3*sizeof(char));
      all[num_schemas].id = (int *) &camera_schema_id[1];
      (*(all[num_schemas].id)) = num_schemas;
      strcpy (all[num_schemas].name, "colorB");
      all[num_schemas].run = (runFn) gazebo_camera1_run;
      all[num_schemas].stop = (stopFn) gazebo_camera1_stop;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,num_schemas);
      num_schemas++;
      myexport("colorB","id",&camera_schema_id[1]);
      myexport("colorB","colorB",&colorB);
      myexport("colorB","run",(void *)gazebo_camera1_run);
      myexport("colorB","stop",(void *)gazebo_camera1_stop);
    }
  if (serve_color[2])
    {
      colorC = (char*)malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3*sizeof(char));
      all[num_schemas].id = (int *) &camera_schema_id[2];
      (*(all[num_schemas].id)) = num_schemas;
      strcpy (all[num_schemas].name, "colorC");
      all[num_schemas].run = (runFn) gazebo_camera2_run;
      all[num_schemas].stop = (stopFn) gazebo_camera2_stop;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,num_schemas);
      num_schemas++;
      myexport("colorC","id",&camera_schema_id[2]);
      myexport("colorC","colorC",&colorC);
      myexport("colorC","run",(void *)gazebo_camera2_run);
      myexport("colorC","stop",(void *)gazebo_camera2_stop);
    }
  if (serve_color[3])
    {
      colorD = (char*)malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3*sizeof(char));
      all[num_schemas].id = (int *) &camera_schema_id[3];
      (*(all[num_schemas].id)) = num_schemas;
      strcpy (all[num_schemas].name, "colorD");
      all[num_schemas].run = (runFn) gazebo_camera3_run;
      all[num_schemas].stop = (stopFn) gazebo_camera3_stop;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,num_schemas);
      num_schemas++;
      myexport("colorD","id",&camera_schema_id[3]);
      myexport("colorD","colorD",&colorD);
      myexport("colorD","run",(void *)gazebo_camera3_run);
      myexport("colorD","stop",(void *)gazebo_camera3_stop);
    }

  
  if (serve_color[4])
    {
      myA.img = NULL;
      myA.clock = 0;
      /* the gazebo device must be opened before this operation. The image size 
	 of the jde varcolor image is taken from Gazebo device */
      if (color_name[4].tipo == 1)
	    {	  
	      myA.width=camera[4]->data->width;
	      myA.height=camera[4]->data->height;
	    }
      else if (color_name[4].tipo > 1)
	{
	  myA.width=stereo->data->width;
	  myA.height=stereo->data->height;
	}
      myA.img = (char*)malloc(myA.width*myA.height*3*sizeof(char));
      all[num_schemas].id = (int *) &camera_schema_id[4];
      (*(all[num_schemas].id)) = num_schemas;
      strcpy(all[num_schemas].name,"varcolorA");
      all[num_schemas].run = (runFn) gazebo_camera4_run;
      all[num_schemas].stop = (stopFn) gazebo_camera4_stop;     
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;     
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name, *(all[num_schemas].id));
      num_schemas++;
      myexport("varcolorA","id",&camera_schema_id[4]);
      myexport("varcolorA","varcolorA",&myA);
      myexport("varcolorA","run",(void *)gazebo_camera4_run);
      myexport("varcolorA","stop",(void *)gazebo_camera4_stop);
    }

  if (serve_color[5])
    {
      myB.img = NULL;
      myB.clock = 0;
      /* the gazebo device must be opened before this operation. The image size 
	 of the jde varcolor image is taken from Gazebo device */
      if (color_name[5].tipo == 1)
	    {	  
	      myB.width=camera[5]->data->width;
	      myB.height=camera[5]->data->height;
	    }
      else if (color_name[5].tipo > 1)
	{
	  myB.width=stereo->data->width;
	  myB.height=stereo->data->height;
	}
      myB.img = (char*)malloc(myB.width*myB.height*3*sizeof(char));
      all[num_schemas].id = (int *) &camera_schema_id[5];
      (*(all[num_schemas].id)) = num_schemas;
      strcpy(all[num_schemas].name,"varcolorB");
      all[num_schemas].run = (runFn) gazebo_camera5_run;
      all[num_schemas].stop = (stopFn) gazebo_camera5_stop;     
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;     
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name, *(all[num_schemas].id));
      num_schemas++;
      myexport("varcolorB","id",&camera_schema_id[5]);
      myexport("varcolorB","varcolorB",&myB);
      myexport("varcolorB","run",(void *)gazebo_camera5_run);
      myexport("varcolorB","stop",(void *)gazebo_camera5_stop);
    }

  if (serve_color[6])
    {
      myC.img = NULL;
      myC.clock = 0;
      /* the gazebo device must be opened before this operation. The image size 
	 of the jde varcolor image is taken from Gazebo device */
      if (color_name[6].tipo == 1)
	    {	  
	      myC.width=camera[6]->data->width;
	      myC.height=camera[6]->data->height;
	    }
      else if (color_name[6].tipo > 1)
	{
	  myC.width=stereo->data->width;
	  myC.height=stereo->data->height;
	}
      myC.img = (char*)malloc(myC.width*myC.height*3*sizeof(char));
      all[num_schemas].id = (int *) &camera_schema_id[6];
      (*(all[num_schemas].id)) = num_schemas;
      strcpy(all[num_schemas].name,"varcolorC");
      all[num_schemas].run = (runFn) gazebo_camera6_run;
      all[num_schemas].stop = (stopFn) gazebo_camera6_stop;     
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;     
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name, *(all[num_schemas].id));
      num_schemas++;
      myexport("varcolorC","id",&camera_schema_id[6]);
      myexport("varcolorC","varcolorC",&myC);
      myexport("varcolorC","run",(void *)gazebo_camera6_run);
      myexport("varcolorC","stop",(void *)gazebo_camera6_stop);
    }

  if (serve_color[7])
    {
      myD.img = NULL;
      myD.clock = 0;
      /* the gazebo device must be opened before this operation. The image size 
	 of the jde varcolor image is taken from Gazebo device */
      if (color_name[7].tipo == 1)
	    {	  
	      myD.width=camera[7]->data->width;
	      myD.height=camera[7]->data->height;
	    }
      else if (color_name[7].tipo > 1)
	{
	  myD.width=stereo->data->width;
	  myD.height=stereo->data->height;
	}
      myD.img = (char*)malloc(myD.width*myD.height*3*sizeof(char));
      all[num_schemas].id = (int *) &camera_schema_id[7];
      (*(all[num_schemas].id)) = num_schemas;
      strcpy(all[num_schemas].name,"varcolorD");
      all[num_schemas].run = (runFn) gazebo_camera7_run;
      all[num_schemas].stop = (stopFn) gazebo_camera7_stop;     
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;     
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name, *(all[num_schemas].id));
      num_schemas++;
      myexport("varcolorD","id",&camera_schema_id[7]);
      myexport("varcolorD","varcolorD",&myD);
      myexport("varcolorD","run",(void *)gazebo_camera7_run);
      myexport("varcolorD","stop",(void *)gazebo_camera7_stop);
    }
 
  /* gazebo thread creation */
  pthread_mutex_lock (&mymutex);
  state = slept;
  pthread_create (&gazebo_th, NULL, gazebo_thread, NULL);
  pthread_mutex_unlock (&mymutex);

  return 0;
}

void
gazebo_terminate ()
{
  int i;
  
  gazebo_terminate_command = 1;
  if ((serve_motors || serve_encoders)&&(position))
    {
      gz_position_close (position);
      gz_position_free (position);
      position=NULL;
    }
  
  if ((serve_ptencoders || serve_ptmotors)&&(ptz))
    {
      gz_ptz_close(ptz);
      gz_ptz_free(ptz);
      ptz=NULL;
    }
  
  if ((serve_sonars)&&(sonar))
    {
      gz_sonar_close (sonar);
      gz_sonar_free (sonar);
      sonar = NULL;
    }  

  if ((serve_laser)&&(laser))
    {
      gz_laser_close (laser);
      gz_laser_free (laser);
      laser = NULL;
    }
  
  for(i=0;i<MAXCAM;i++)
    if ((serve_color[i])&&(camera[i]))
	{
	  if (color_name[i].tipo == 1)
	    {
	      gz_camera_close (camera[i]);
	      gz_camera_free (camera[i]);
	      camera[i] = NULL;
	    }
	  else if ((color_name[i].tipo > 1)&&(stereo))
	    {
	      gz_stereo_close(stereo);
	      gz_stereo_free(stereo);
	      stereo=NULL;
	      /* setting stereo to NULL prevents new gz_stereo_close from the 
		 second jde-camera that belongs to the same stereo gazebo device */
	    }
	};
  
  gz_client_disconnect (client);
  gz_client_free (client);
  printf ("Switching gazebo off\n");
}


int
gazebo_ptmotors_run (int father, int *brothers, arbitration fn)
{
  
  if ((serve_ptmotors) && (ptmotors_active == 0))
    {
      ptmotors_active = 1;
      /*
      gz_ptz_lock(ptz,1);
      ptz->data->cmd_pan=0.0;
      ptz->data->cmd_tilt=0.0;
      gz_ptz_unlock(ptz);
      pan_angle=0.0;
      tilt_angle=0.0;
      longitude=0.0;
      latitude=0.0;
      */
      put_state (ptmotors_schema_id, winner);
      printf ("gazebo: ptz command  run\n");
      all[ptmotors_schema_id].father = father;
      all[ptmotors_schema_id].fps = 0.;
      all[ptmotors_schema_id].k = 0;
      should_driver_restart();
    }
  return 0;
}

int
gazebo_ptmotors_stop ()
{
  if ((serve_ptmotors) && (ptmotors_active))
    {
      ptmotors_active = 0;
      printf ("gazebo: ptz command stop\n");
      put_state (ptmotors_schema_id, slept);
      should_driver_stop(); 
    }
  return 0;
}

int
gazebo_ptencoders_run (int father, int *brothers, arbitration fn)
{
  
  if ((serve_ptencoders) && (ptencoders_active == 0))
    {
      ptencoders_active = 1;
      put_state (ptencoders_schema_id, winner);
      all[ptencoders_schema_id].father = father;
      all[ptencoders_schema_id].fps = 0.;
      all[ptencoders_schema_id].k = 0;
      should_driver_restart();
    }
  return 0;
}

int
gazebo_ptencoders_stop ()
{
  if ((serve_ptencoders) && (ptencoders_active))
    {
      ptencoders_active = 0;
      printf ("gazebo: ptz encoders stop\n");
      put_state (ptencoders_schema_id, slept);
      should_driver_stop();
    }
  return 0;
}

int
gazebo_camera0_run (int father, int *brothers, arbitration fn)
{
 
  if ((serve_color[0]==1)&&(color_active[0]==0)) 
     {       
       color_active[0]++;
       if ((all[camera_schema_id[0]].father==GUIHUMAN) ||
	   (all[camera_schema_id[0]].father==SHELLHUMAN))
	 all[camera_schema_id[0]].father = father;
       if(color_active[0]==1)	 
	 {
	   all[camera_schema_id[0]].father = father;
	   all[camera_schema_id[0]].fps = 0.;
	   all[camera_schema_id[0]].k = 0;
	   put_state (camera_schema_id[0], winner);
	   should_driver_restart();
	 }
     }
   return 0;
}

int
gazebo_camera0_stop ()
{
  if ((serve_color[0]) && (color_active[0]==0))
    {
      color_active[0]--;
      if (color_active[0]<=0)
	{
	  put_state (camera_schema_id[0], slept);
	  should_driver_stop();     
	}
    }
  return 0;
}


int
gazebo_camera1_run (int father, int *brothers, arbitration fn)
{
  
  if ((serve_color[1]==1)&&(color_active[1]==0)) 
     {
      color_active[1]++;
      if ((all[camera_schema_id[1]].father==GUIHUMAN) ||
	  (all[camera_schema_id[1]].father==SHELLHUMAN))
	all[camera_schema_id[1]].father = father;
      if(color_active[1]==1)
	{
	  all[camera_schema_id[1]].father = father;
	  all[camera_schema_id[1]].fps = 0.;
	  all[camera_schema_id[1]].k = 0;
	  put_state (camera_schema_id[1], winner);
	  should_driver_restart();
	}
     }
  return 0;
}


int
gazebo_camera1_stop ()
{
  if ((serve_color[1]) && (color_active[1]))
    {
      color_active[1]--;
      if (color_active[1]<=0)
	{
	  put_state (camera_schema_id[1], slept);
	  should_driver_stop();     
	}
    }
  return 0;
}


int
gazebo_camera2_run (int father, int *brothers, arbitration fn)
{

  if ((serve_color[2]==1)&&(color_active[2]==0)) 
     { 
       color_active[2]++;
       if ((all[camera_schema_id[2]].father==GUIHUMAN) ||
	   (all[camera_schema_id[2]].father==SHELLHUMAN))
	 all[camera_schema_id[2]].father = father;
       if(color_active[2]==1)
	 {
	   all[camera_schema_id[2]].father = father;
	   all[camera_schema_id[2]].fps = 0.;
	   all[camera_schema_id[2]].k = 0;
	   put_state (camera_schema_id[2], winner);
	   should_driver_restart();
	 }
     }
  return 0;
}

int
gazebo_camera2_stop ()
{
  if ((serve_color[2]) && (color_active[2]))
    {
      color_active[2]--;
      if (color_active[2]<=0)
	{
	  put_state (camera_schema_id[2], slept);
	  should_driver_stop();     
	}
    }
  return 0;
}


int
gazebo_camera3_run (int father, int *brothers, arbitration fn)
{
 if ((serve_color[3]==1)&&(color_active[3]==0)) 
     {
       color_active[3]++;
      if ((all[camera_schema_id[3]].father==GUIHUMAN) ||
	  (all[camera_schema_id[3]].father==SHELLHUMAN))
	all[camera_schema_id[3]].father = father;
      if(color_active[3]==1)	
	{
	  all[camera_schema_id[3]].father = father;
	  all[camera_schema_id[3]].fps = 0.;
	  all[camera_schema_id[3]].k = 0;
	  put_state (camera_schema_id[3], winner);
	  should_driver_restart();
	}
     }
 return 0;
}



int
gazebo_camera3_stop ()
{
  if ((serve_color[3]) && (color_active[3]))
    {
      color_active[3]--;
      printf ("gazebo: camera 3 stop\n");
      if (color_active[3]<=0)
	{
	  put_state (camera_schema_id[2], slept);
	  should_driver_stop();     
	}
    }
  return 0;
}

int
gazebo_camera4_run (int father, int *brothers, arbitration fn)
{
  if ((serve_color[4]==1)&&(color_active[4]==0)) 
     {
       color_active[4]++;
      if ((all[camera_schema_id[4]].father==GUIHUMAN) ||
	  (all[camera_schema_id[4]].father==SHELLHUMAN))
	all[camera_schema_id[4]].father = father;
      if(color_active[4]==1)	
	{
	  all[camera_schema_id[4]].father = father;
	  all[camera_schema_id[4]].fps = 0.;
	  all[camera_schema_id[4]].k = 0;
	  put_state (camera_schema_id[4], winner);
	  should_driver_restart();	 
	}
     }
  return 0;
}


int
gazebo_camera4_stop ()
{
  if ((serve_color[4]) && (color_active[4]))
    {
      color_active[4]--;
      /* printf ("gazebo: camera 4 stop\n");*/
      if (color_active[4]<=0)
	{
	  put_state (camera_schema_id[4], slept);
	  should_driver_stop();     
	}
    }
  return 0;
}

int
gazebo_camera5_run (int father, int *brothers, arbitration fn)
{
  if ((serve_color[5]==1)&&(color_active[5]==0)) 
     {
       color_active[5]++;
      if ((all[camera_schema_id[5]].father==GUIHUMAN) ||
	  (all[camera_schema_id[5]].father==SHELLHUMAN))
	all[camera_schema_id[5]].father = father;
      if(color_active[5]==1)	
	{
	  all[camera_schema_id[5]].father = father;
	  all[camera_schema_id[5]].fps = 0.;
	  all[camera_schema_id[5]].k = 0;
	  put_state (camera_schema_id[5], winner);
	  should_driver_restart();	 
	}
     }
  return 0;
}


int
gazebo_camera5_stop ()
{
  if ((serve_color[5]) && (color_active[5]))
    {
      color_active[5]--;
      /* printf ("gazebo: camera 5 stop\n");*/
      if (color_active[5]<=0)
	{
	  put_state (camera_schema_id[5], slept);
	  should_driver_stop();     
	}
    }
  return 0;
}

int
gazebo_camera6_run (int father, int *brothers, arbitration fn)
{
  if ((serve_color[6]==1)&&(color_active[6]==0)) 
     {
       color_active[6]++;
      if ((all[camera_schema_id[6]].father==GUIHUMAN) ||
	  (all[camera_schema_id[6]].father==SHELLHUMAN))
	all[camera_schema_id[6]].father = father;
      if(color_active[6]==1)	
	{
	  all[camera_schema_id[6]].father = father;
	  all[camera_schema_id[6]].fps = 0.;
	  all[camera_schema_id[6]].k = 0;
	  put_state (camera_schema_id[6], winner);
	  should_driver_restart();	 
	}
     }
  return 0;
}


int
gazebo_camera6_stop ()
{
  if ((serve_color[6]) && (color_active[6]))
    {
      color_active[6]--;
      /* printf ("gazebo: camera 6 stop\n");*/
      if (color_active[6]<=0)
	{
	  put_state (camera_schema_id[6], slept);
	  should_driver_stop();     
	}
    }
  return 0;
}

int
gazebo_camera7_run (int father, int *brothers, arbitration fn)
{
  if ((serve_color[7]==1)&&(color_active[7]==0)) 
     {
       color_active[7]++;
      if ((all[camera_schema_id[7]].father==GUIHUMAN) ||
	  (all[camera_schema_id[7]].father==SHELLHUMAN))
	all[camera_schema_id[7]].father = father;
      if(color_active[7]==1)	
	{
	  all[camera_schema_id[7]].father = father;
	  all[camera_schema_id[7]].fps = 0.;
	  all[camera_schema_id[7]].k = 0;
	  put_state (camera_schema_id[7], winner);
	  should_driver_restart();	 
	}
     }
  return 0;
}


int
gazebo_camera7_stop ()
{
  if ((serve_color[7]) && (color_active[7]))
    {
      color_active[7]--;
      /* printf ("gazebo: camera 7 stop\n");*/
      if (color_active[7]<=0)
	{
	  put_state (camera_schema_id[7], slept);
	  should_driver_stop();     
	}
    }
  return 0;
}



int
gazebo_encoders_run (int father, int *brothers, arbitration fn)
{
  if ((serve_encoders) && (encoders_active == 0))
    {
      encoders_active = 1;
      put_state (encoders_schema_id, winner);
      printf ("gazebo: encoders run\n");
      all[encoders_schema_id].father = father;
      all[encoders_schema_id].fps = 0.;
      all[encoders_schema_id].k = 0;
      should_driver_restart();	  
    }
  return 0;
}

int
gazebo_encoders_stop ()
{

  if ((serve_encoders) && (encoders_active))
    {
      encoders_active = 0;
      put_state (encoders_schema_id, slept);
      printf ("gazebo: encoders stop\n");
      should_driver_stop();  
    }
  return 0;
}

int
gazebo_sonars_run (int father, int *brothers, arbitration fn)
{
  if ((serve_sonars) && (sonars_active == 0))
    {
      sonars_active = 1;
      put_state (sonars_schema_id, winner);
      printf ("gazebo: sonars run\n");

      all[sonars_schema_id].father = father;
      all[sonars_schema_id].fps = 0.;
      all[sonars_schema_id].k = 0;
      should_driver_restart();	  
    }
  return 0;
}

int
gazebo_sonars_stop ()
{
  if ((serve_sonars) && (sonars_active))
    {
      sonars_active = 0;
      put_state (sonars_schema_id, slept);
      printf ("gazebo: sonars stop\n");
      should_driver_stop();  
    }
  return 0;
}

int
gazebo_motors_run (int father, int *brothers, arbitration fn)
{
  //position->data->cmd_enable_motors = 1;
  if ((serve_motors) && (motors_active == 0))
    {
      put_state (motors_schema_id, winner);
      motors_active = 1;
      printf ("gazebo: motors run\n");
      all[motors_schema_id].father = father;
      all[motors_schema_id].fps = 0.;
      all[motors_schema_id].k = 0;
      should_driver_restart();	  
    }
  return 0;
}

int
gazebo_motors_stop ()
{

  if ((serve_motors) && (motors_active))
    {
      int v_sp = 0;
      int w_sp = 0;
      motors_active = 0;
      /*Velocidad a cero */
      gz_position_lock (position, 1);
      position->data->cmd_vel_pos[0] = v_sp;	//Velocidad en X 
      position->data->cmd_vel_rot[2] = w_sp;	//Velocidad en Z
      gz_position_unlock (position);
      printf ("gazebo: motors stop\n");
      put_state (motors_schema_id, slept);
      should_driver_stop();  
    }
  return 0;
}

int
gazebo_laser_run (int father, int *brothers, arbitration fn)
{
  if ((serve_laser) && (laser_active == 0))
    {
      laser_active = 1;
      put_state (laser_schema_id, winner);
      printf ("gazebo: laser run\n");
      all[laser_schema_id].father = father;
      all[laser_schema_id].fps = 0.;
      all[laser_schema_id].k = 0;
      should_driver_restart();	  
    }
  return 0;
}

int
gazebo_laser_stop ()
{

  if ((serve_laser) && (laser_active))
    {
      laser_active = 0;
      put_state (laser_schema_id, slept);
      printf ("gazebo: laser stop\n");
      should_driver_stop();  
    }
  return 0;
}

void
gazebo_motors_iteration ()
{

  double v_double, w_double;
  speedcounter (motors_schema_id);

  if(!position){
	fprintf(stderr,"Position model not opened\n");
	return ;
	}

  //tomamos la velocidad actual
  gz_position_lock (position, 1);
  v_double = position->data->vel_pos[0];
  w_double = position->data->vel_rot[2];
  gz_position_unlock (position);

  gz_position_lock (position, 1);
  //jde trabaja en  mm/s, gazebo en m/s
  position->data->cmd_vel_pos[0] = (v/1000);	 
  // w esta en grados/s , gazebo trata radianes/s
  position->data->cmd_vel_rot[2] = w * DEGTORAD;	
  gz_position_unlock (position);
}

void *
gazebo_thread (void *not_used)
{
  struct timeval t;
  unsigned long now,next;
  static unsigned long lastmotor = 0;
  static unsigned long lastiteration = 0;
  int i;

  printf ("gazebo: gazebo thread started up\n");

  do
    {
      pthread_mutex_lock (&mymutex);
      if (state == slept)
	{
	  printf ("gazebo: gazebo thread in sleep mode\n");
	  pthread_cond_wait (&condition, &mymutex);
	  printf ("gazebo: gazebo thread woke up\n");
	  pthread_mutex_unlock (&mymutex);
	}
      else
	{
	  pthread_mutex_unlock (&mymutex);
	  gettimeofday (&t, NULL);
	  now = t.tv_sec * 1000000 + t.tv_usec;
	  lastiteration = now;

	  /* sending motors command */
	  if ((serve_motors) && (motors_active))
	    {
	      if ((now - lastmotor) > GAZEBO_COMMAND_CYCLE * 1000)
		{
		  lastmotor = now;
		  gazebo_motors_iteration ();
		}
	    }

	  gz_client_wait (client);
	  /*Actualizamos */
	  if (laser_active)
	    gazebo_laser_callback ();

	  if (ptmotors_active)
		gazebo_ptmotors_callback();

	  if (ptencoders_active)
		gazebo_ptencoders_callback();

	  for(i=0;i<MAXCAM;i++)
	    {
	      if ((serve_color[i]) && (color_active[i]))
		gazebo_camera_callback (i);
	    }

	  if (sonars_active)
	    gazebo_sonars_callback ();

	  if (encoders_active)
	    gazebo_encoders_callback ();

	 /* to control the iteration time of this driver */
	  gettimeofday (&t, NULL);
	  now = t.tv_sec * 1000000 + t.tv_usec;
	  next=lastiteration+(long)GAZEBO_CYCLE*1000;
	  if (next>(5000+now))
	    {
	      usleep(next-now-5000);
	    }
	}
    }
  while (gazebo_terminate_command == 0);
  pthread_exit (0);
}

void
gazebo_camera_callback (int camnum)
{
  unsigned char *destination=NULL;
  unsigned char *origen=NULL;
  float *origendisparity=NULL;
  float scalefactor = 4;
  /* 4 is a ad hoc scale factor to avoid dark disparity images */
  int myrows=0,mycolumns=0;
  int i=0;

  speedcounter (camera_schema_id[camnum]);
  switch (camnum)
    {
    case 0:
      destination = colorA;
      myrows=SIFNTSC_ROWS;
      mycolumns=SIFNTSC_COLUMNS;
      break;
    case 1:
      destination = colorB;
      myrows=SIFNTSC_ROWS;
      mycolumns=SIFNTSC_COLUMNS;
      break;
    case 2:
      destination = colorC;
      myrows=SIFNTSC_ROWS;
      mycolumns=SIFNTSC_COLUMNS;
      break;
    case 3:
      destination = colorD;
      myrows=SIFNTSC_ROWS;
      mycolumns=SIFNTSC_COLUMNS;
      break;
    case 4:
      destination = myA.img;
      myrows=myA.height;
      mycolumns=myA.width;
      break;
    case 5:
      destination = myB.img;
      myrows=myB.height;
      mycolumns=myB.width;
      break;
    case 6:
      destination = myC.img;
      myrows=myC.height;
      mycolumns=myC.width;
      break;
    case 7:
      destination = myD.img;
      myrows=myD.height;
      mycolumns=myD.width;
      break;
    }


  switch (color_name[camnum].tipo)
    {
    case 1:
      origen = camera[camnum]->data->image;
      break;
    case 2:
      origen = stereo->data->left_image;
      break;
    case 3:
      origen = stereo->data->right_image;
      break;
    case 4:
      origendisparity = stereo->data->left_disparity;
      /* left_disparity contains float values */
      break;
    case 5:
      origendisparity = stereo->data->right_disparity;
      /* left_disparity contains float values */
      break;
    }


  if ((color_name[camnum].tipo)==1)
    {
      gz_camera_lock (camera[camnum], 1);
      /* parece que un dispositivo es RGB y el otro BGR 
	 por tanto damos el cambiazo aqui */
      for (i = 0; i < myrows*mycolumns*3; i += 3)
	{
	  destination[i] = origen[i + 2];
	  destination[i + 1] = origen[i + 1];
	  destination[i + 2] = origen[i];
	}
      gz_camera_unlock (camera[camnum]);
    }
  else if (((color_name[camnum].tipo)==2) ||
	  ((color_name[camnum].tipo)==3))
    {
      gz_stereo_lock (stereo, 1);
      for (i = 0; i < myrows*mycolumns*3; i += 3)
	{
	  destination[i] = origen[i + 2];
	  destination[i + 1] = origen[i + 1];
	  destination[i + 2] = origen[i];
	}
      gz_stereo_unlock (stereo);
    }
  else if  (((color_name[camnum].tipo)==4) ||
	    ((color_name[camnum].tipo)==5))
    {
      gz_stereo_lock (stereo,1);
      for (i = 0; i < myrows*mycolumns; i++)
	{
	  destination[3*i] = (unsigned char)(int)(scalefactor*origendisparity[i]);
	  destination[3*i + 1] = (unsigned char)(int)(scalefactor*origendisparity[i]);
	  destination[3*i + 2] = (unsigned char)(int)(scalefactor*origendisparity[i]);
	}
      gz_stereo_unlock (stereo);
    }
}


void
gazebo_ptmotors_callback(){
  	speedcounter (ptmotors_schema_id);

	if(ptz){
		gz_ptz_lock(ptz,1);
		/* enforce the position limits */
		/* ignores the pan speed and tilt speed values */
		if (longitude > max_pan)
		  longitude = max_pan;
		else if (longitude < min_pan)
		  longitude = min_pan;

		if (latitude > max_tilt)
		  latitude = max_tilt;
		else if (latitude < min_tilt)
		  latitude = min_tilt;
		
		/* pan in gazebo's pantilt is the opposite to pan in directed perception pantilt unit.  */
		/* tilt in gazebo's pantilt is the opposite to pan in directed perception pantilt unit.  */
		ptz->data->cmd_pan=-longitude * DEGTORAD;
		ptz->data->cmd_tilt=-latitude * DEGTORAD;
		gz_ptz_unlock(ptz);
		/*	printf( "Pan Tilt motors TO gazebo: \n longitude: %f latitude:%f\n",longitude,latitude);*/
		}
	}

void
gazebo_ptencoders_callback(){
  	speedcounter (ptencoders_schema_id);

	if(ptz){
		gz_ptz_lock(ptz,1);
		/* pan in gazebo's pantilt is the opposite to pan in directed perception pantilt unit.  */
		/* tilt in gazebo's pantilt is the opposite to pan in directed perception pantilt unit.  */
		pan_angle= -1 * ptz->data->pan * RADTODEG;
		tilt_angle= -1 * ptz->data->tilt * RADTODEG;
		/*		printf( "Pan Tilt encoders from gazebo: \n pan_angle: %f:tilt_angle:%f\n",pan_angle,tilt_angle);*/
		gz_ptz_unlock(ptz);
		}
	}

void
gazebo_laser_callback ()
{
/* 
 * Gazebo ofrece un numero de medidas configurable
 * Mapeamos las medidas de gazebo en las de jde
 * una a una cuando el numero de medidas coincide
 * una a varias o viceversa cuando el n�mero de medidas difiere 
 */

  int cont = 0;
  int cont2;

  double relation; //(gazebo ray number / jde rays number)

  speedcounter(laser_schema_id);
  gz_laser_lock (laser, 1);
  relation= (float)laser->data->range_count /(float)NUM_LASER ;
  
  for (cont = 0; cont < NUM_LASER &&
       (relation * cont) < laser->data->range_count; cont++)
    {
      cont2 = rint(relation * cont);
      jde_laser[cont] = (int) (laser->data->ranges[cont2] * 1000);
	
    }
  gz_laser_unlock (laser);
}


void
gazebo_encoders_callback ()
{

  float robotx, roboty, robottheta;
  speedcounter(encoders_schema_id);

  gz_position_lock (position, 1);
  robotx =
    (position->data->pos[0]) * 1000 * (float) cos (DEGTORAD * correcting_theta) -
    (position->data->pos[1]) * 1000 * (float) sin (DEGTORAD * correcting_theta) +
    correcting_x;
  roboty =
    (position->data->pos[1]) * 1000 * (float) cos (DEGTORAD * correcting_theta) +
    (position->data->pos[0]) * 1000 * (float) sin (DEGTORAD * correcting_theta) +
    correcting_y;
  robottheta = (position->data->rot[2] * RADTODEG) + correcting_theta;

  if (robottheta <= 0) robottheta = robottheta + 360;
  else if (robottheta > 360) robottheta = robottheta - 360;
  gz_position_unlock (position);

  jde_robot[0] = robotx;
  jde_robot[1] = roboty;
  jde_robot[2] = robottheta * DEGTORAD;
  jde_robot[3] = cos (jde_robot[2]);
  jde_robot[4] = sin (jde_robot[2]);
}

void
gazebo_sonars_callback ()
{
/* Gazebo da para cada sensor la posicion x,y,z
 * y su orientaci�n roll,pitch,yaw: 6 par�metros
 * jde contempla x,y mas la orientaci�n : 5 parametros
 * Por ahora suponemos que las posiciones por defecto para
 * el robot pioneer son las adecuadas en ambos sistemas 
 * dado que los modelos de robots coinciden. Pasamos NUM_SONARS rangos
 * Gazebo permite un m�ximo de 48 sensores
 * Gazebo nos da las distancias sonar en metros. Paso a mm para jde
 */
  int j;
  speedcounter(sonars_schema_id);
  gz_sonar_lock (sonar, 1);
  for (j = 0; j < NUM_SONARS; j++)
    {
      us[j] = (float) sonar->data->sonar_ranges[j]*1000.0;
    }
  gz_sonar_unlock (sonar);

}

int
gazebo_parseconf (char *configfile)
{
  FILE *conf = fopen (configfile, "r");
  char cpLinea[MAX_BUFFER],word3[MAX_BUFFER],word4[MAX_BUFFER],word5[MAX_BUFFER];
  char *pLine1, *pLine2;
  int ItIsAGazeboLine = 0, i, colors;
  char *camera_token[] = { "colorA", "colorB", "colorC", "colorD", "varcolorA", "varcolorB", "varcolorC", "varcolorD" };


  if (!conf)
    return (-1);
  while (!feof (conf))
    {
      fgets (cpLinea, MAX_BUFFER, conf);
      i = 0;
      while (isspace (cpLinea[i++]));
      if (cpLinea[i - 1] == '#')
	continue;
      if (strstr (cpLinea, "gazebo"))
	{
	  ItIsAGazeboLine = 1;
	  continue;
	}
      if (!ItIsAGazeboLine)
	continue;
      /* puts (cpLinea);*/
      /*tenemos lineas de nuestro driver */
      if (strstr (cpLinea, "end_driver"))
	break;

      for (colors = MAXCAM-1; colors >=0; colors--)
	{
	  pLine1 = strstr (cpLinea, camera_token[colors]);
	  if (pLine1 )
	    {
	      pLine2 = strstr (pLine1, " ");
	      for (i = 0; isspace (pLine2[0]); pLine2++);	//salto blancos
	      
	      if (strstr (pLine2, "leftd"))
		{		/*Stereo left disparity */
		  pLine1 = strstr (pLine2, " ");
		  for (i = 0; isspace (pLine1[0]); pLine1++);	//salto blancos 
		  pLine2 = strstr(pLine1,"\n");
		  if (pLine2) pLine2[0] = (char) 0; /* Changes \n for \0 */
		  /* printf ("Nombre del estero leftd:%s:\n", pLine2); */
		  serve_color[colors]=1;
		  strcpy (color_name[colors].name, pLine1);
		  color_name[colors].tipo = 4;	/*left disparity */
		  puts(color_name[colors].name);
		  break;
		}
	      
	      else if (strstr (pLine2, "rightd"))
		{		/*Stereo right disparity */
		  pLine1 = strstr (pLine2, " ");
		  for (i = 0; isspace (pLine1[0]); pLine1++);	//salto blancos 
		  pLine2 = strstr(pLine1,"\n");
		  if (pLine2) pLine2[0] = (char) 0; /* Changes \n for \0 */
		  serve_color[colors]=1;
		  strcpy (color_name[colors].name, pLine1);	/*stereo id */
		  /* printf ("Nombre del estero rightd:%s:\n", pLine2); */
		  color_name[colors].tipo = 5;	/*right disparity */
		  puts(color_name[colors].name);
		  break;
		}
	      
	      else if (strstr (pLine2, "left"))
		{		/*Stereo left */
		  pLine1 = strstr (pLine2, " ");
		  for (i = 0; isspace (pLine1[0]); pLine1++);	//salto blancos 
		  pLine2 = strstr(pLine1,"\n");
		  if (pLine2) pLine2[0] = (char) 0; /* Changes \n for \0 */
		  serve_color[colors]=1;
		  /* printf ("Nombre de stereo encontrado en left:%s:\n", pLine2); */
		  strcpy(color_name[colors].name, pLine1);	/*stereo id */
		  color_name[colors].tipo = 2;	/* left */
		  puts(color_name[colors].name);
		  break;
		}
	      
	      else if (strstr (pLine2, "right"))
		{		/*Stereo right */
		  pLine1 = strstr (pLine2, " ");
		  for (i = 0; isspace (pLine1[0]); pLine1++);	//salto blancos 
		  pLine2 = strstr(pLine1,"\n");
		  if (pLine2) pLine2[0] = (char) 0; /* Changes \n for \0 */
		  serve_color[colors]=1;
		  /* printf ("Nombre de stereo encontrado en right:%s:\n", pLine2);*/
		  strcpy(color_name[colors].name, pLine2);	/*stereo id */
		  color_name[colors].tipo = 3;	/*right */
		  puts(color_name[colors].name);
		  break;
		}
	      
	      else
		{		/*Se trata de una camara mono */
		  pLine1 = strstr (pLine2, "\n");
		  if (pLine1) pLine1[0] = (char) 0; /* Changes \n for \0 */
		  serve_color[colors]=1;
		  strcpy(color_name[colors].name, pLine2);	/*camera id */
		  color_name[colors].tipo = 1;	/*camara mono */
		  puts(color_name[colors].name);
		  break;
		}
	    }
	}			/*endfor */
      
      pLine2=strstr(cpLinea," laser ");
      if (pLine2)
	{			//Tomo el surtidor del laser
	  pLine2 += 7; //Me salto " laser ";
	  for (i = 0; isspace (pLine2[0]); pLine2++);	//salto blancos
	  pLine1 = strstr (pLine2, "\n");
	  pLine1[0] = (char) 0;
	  serve_laser=1;
	  strncpy (laser_name, pLine2, MAX_MODEL_ID);
	}
      
      pLine2 = strstr (cpLinea, " motors ");
      if (pLine2)
	{			//Tomo el surtidor de motores
	  pLine2 += 8;
	  for (; isspace (pLine2[0]); pLine2++);	//salto blancos
	  pLine1 = strstr (pLine2, "\n");
	  pLine1[0] = (char) 0;
	  serve_motors=1;
	  strncpy (motors_name, pLine2, MAX_MODEL_ID);
	}
      
      pLine2 = strstr(cpLinea," encoders ");
      if (pLine2)
	{			//Tomo el surtidor de encoders
	  pLine2 += 10;
	  for (i = 0; isspace (pLine2[0]); pLine2++);	//salto blancos
	  pLine1 = strstr (pLine2, "\n");
	  pLine1[0] = (char) 0;
	  serve_encoders=1;
	  strncpy (position_name, pLine2, MAX_MODEL_ID);
	}

      pLine2=strstr(cpLinea," sonars ");
      if (pLine2)
	{			//Tomo el surtidor de sonars
	  pLine2 += 8;
	  for (i = 0; isspace (pLine2[0]); pLine2++);	//salto blancos
	  pLine1 = strstr (pLine2, "\n");
	  pLine1[0] = (char) 0;
	  serve_sonars=1;
	  strncpy (sonar_name, pLine2, MAX_MODEL_ID);
	}
      
      pLine2=strstr(cpLinea," pantilt ");
      if (pLine2)
	{			//Tomo el surtidor de ptz
	  pLine2 += 9;
	  for (i = 0; isspace (pLine2[0]); pLine2++);	//salto blancos
	  pLine1 = strstr (pLine2, "\n");
	  pLine1[0] = (char) 0;
	  serve_ptencoders=1;
	  serve_ptmotors=1;
	  strncpy (ptz_name, pLine2, MAX_MODEL_ID);
	}

      pLine2 = strstr(cpLinea, "initial_position ");
      if(pLine2) { //Tomo el surtidor de initial_position
	pLine2 += 17;
	for (i = 0; isspace(pLine2[0]); pLine2++); //salto blancos
	printf("XXX El nombre para initial_position :%s:\n",pLine2);
	if(sscanf(pLine2,"%s %s %s",word3,word4,word5)>2){
	  correcting_x=(float)atof(word3);
	  correcting_y=(float)atof(word4);
	  correcting_theta=(float)atof(word5);
	  printf("gazebo: correcting x=%.1f(mm)  y=%.1f(mm)  theta=%.1f(deg).\n",
		 correcting_x,correcting_y,correcting_theta);
	  printf("gazebo: make sure this is the initial position in the world file provided to Gazebo simulator.\n");
	}else
	  printf("gazebo: wrong initial_position line in the configuration file\n");
	pLine1 = strstr(pLine2, "\n");
	pLine1[0] = (char) 0;
      }
    }
  
  fclose (conf);
  return 0;
}
