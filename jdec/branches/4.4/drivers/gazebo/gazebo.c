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
 *  Authors : Teodoro Gonzalez sanchez <tgonzale@gsyc.escet.urjc.es>
 *
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
#include <gazebo.h>
#include <jde.h>

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
#define MAXCAM 4
/** Max drivers name lenth*/
#define MAX_MODEL_ID	100 


gz_client_t *client = NULL;
gz_position_t *position = NULL;
gz_camera_t *camera[4] = { NULL, NULL, NULL, NULL };
gz_laser_t *laser = NULL;
gz_sonar_t *sonar = NULL;
gz_power_t *power = NULL;
gz_stereo_t *stereo = NULL;
gz_ptz_t *ptz = NULL;

typedef struct gazebo_camera_name
{
  /* tipo 0 unasigned, 1 camera, 2 stereo left, 3 stereo right */
  int tipo;
  /* camara or stereo head gazebo id. NULL when not asigned */
  char gazebo_id[MAX_MODEL_ID];	
} gc_name,* pgc_name;

/*These variables below say which gazebo source feed each jde camera*/
gc_name colorA_name = { 0, "" };
gc_name colorB_name = { 0, "" };
gc_name colorC_name = { 0, "" };
gc_name colorD_name = { 0, "" };

/* The gazebo device names for jde laser, sonar etc */
char laser_name[MAX_MODEL_ID] = "";
char sonar_name[MAX_MODEL_ID] = "";
char position_name[MAX_MODEL_ID] = "";
char motors_name[MAX_MODEL_ID] = "";
char ptz_name[MAX_MODEL_ID]="";
char stereo_name[MAX_MODEL_ID]="";

/* Variables put to 0.0 and no change during the execution */
float correcting_x = 0.;
float correcting_y = 0.;
float correcting_theta = 0.;

/* Gazebo Server and client */
int server_id = 0;
int client_id = 0;


/* Jde and drivers stuff declarations */
pthread_t gazebo_th;
void *gazebo_thread (void *not_used);
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
int gazebo_laser_run (int father, int *brothers, arbitration fn);
int gazebo_laser_stop ();
int gazebo_sonars_run (int father, int *brothers, arbitration fn);
int gazebo_sonars_stop ();
int gazebo_ptmotors_run (int father, int *brothers, arbitration fn);
int gazebo_ptmotors_stop ();
int gazebo_ptencoders_run (int father, int *brothers, arbitration fn);
int gazebo_ptencoders_stop ();

/* Operation functions declaration. 
 * They do the data transfer job */
void gazebo_laser_callback ();
void gazebo_camera_callback (int camnum);
void gazebo_stereo_callback (int camnum);
void gazebo_ptmotors_callback ();
void gazebo_ptencoders_callback ();
void gazebo_sonars_callback ();
void gazebo_encoders_callback ();
void gazebo_motors_callback ();


int state;
pthread_mutex_t mymutex;
pthread_cond_t condition;

/** pthread state variable.*/
int state;
/** mutex for video playing.*/
pthread_mutex_t mymutex;
/** mutex for pthreads.*/
pthread_mutex_t color_mutex[MAXCAM];
/** condition flag for video playing.*/
pthread_cond_t condition;


char driver_name[256] = "gazebo";
int gazebo_terminate_command = 0;

int serve_laser = 0, 
  serve_encoders = 0, 
  serve_sonars = 0,
  serve_motors = 0,
  serve_stereo = 0,
  serve_ptmotors = 0,
  serve_ptencoders = 0,
  serve_color[MAXCAM],
  laser_active = 0,
  encoders_active = 0,
  sonars_active = 0, 
  motors_active = 0,
  camera_active = 0,
  stereo_active = 0,
  ptmotors_active = 0,
  ptencoders_active = 0,
  color_active[MAXCAM];

int 
laser_schema_id,
  encoders_schema_id,
  sonars_schema_id,
  motors_schema_id,
  camera_schema_id[4],
  stereor_schema_id,
  stereol_schema_id,
  ptmotors_schema_id,
  ptencoders_schema_id;


/*Variables a exportar*/
char *colorA; /** 'colorA' schema image data*/
char *colorB; /** 'colorB' schema image data*/
char *colorC; /** 'colorC' schema image data*/
char *colorD;/** 'color' schema image data*/
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
/*Fin variables a exportar*/

int
gazebo_deviceinit ()
{
  
  printf ("connecting to Gazebo Server");
  
  gz_error_init (1, 9);		/*Trazas de gazebo al m�ximo*/

  client = gz_client_alloc ();
  
  position = gz_position_alloc ();
  
  
  if (gz_client_connect_wait (client, server_id, client_id) != 0)
    {
      fprintf (stderr, "gazebo: Error connecting to gazebo server\n");
      exit (-1);
    }
  
  if (serve_stereo)
    {
      stereo = gz_stereo_alloc ();
      
      if (gz_stereo_open (stereo, client, stereo_name) != 0)
	{
	  fprintf (stderr, "Error opening  \"%s\" stereohead\n",
		   stereo_name);
	  return (-1);
	}
    }

  if (serve_ptencoders || serve_ptmotors)
   {
	ptz = gz_ptz_alloc();
      if (gz_ptz_open (ptz, client, ptz_name) != 0)
	{
	  fprintf (stderr, "Error opening \"%s\" ptz\n",
		   ptz_name);
	  return (-1);
	}
   }
  return (0);
}

int
gazebo_parseconf (char *configfile);

int
gazebo_init (char *configfile)
{
  puts ("Starting gazebo");
  fflush (stdout);
  
  if (gazebo_parseconf (configfile) == -1)
    {
      printf ("gazebo: driver not initialized. Configuration file parsing error.\n");
      exit (-1);
    }
  
  if (laser_name[0])
    serve_laser = 1;
  if (position_name[0])
    serve_encoders = 1;
  if (sonar_name[0])
    serve_sonars = 1;
  if (motors_name[0])
    {
      puts ("motors_name");
      puts (motors_name);
      serve_motors = 1;
    }
  if (ptz_name[0]){
    serve_ptencoders=1; //Si hay nombre de ptz con gazebo sirve las dos cosas
    serve_ptmotors=1;
  }
  
  
  if (colorA_name.gazebo_id[0])
    {
      puts (colorA_name.gazebo_id);
      serve_color[0] = 1;
    }
  if (colorB_name.gazebo_id[0]){
    puts (colorB_name.gazebo_id);
    serve_color[1] = 1;
  }
  if (colorC_name.gazebo_id[0]){
    puts (colorC_name.gazebo_id);
    serve_color[2] = 1;
  }
  if (colorD_name.gazebo_id[0]){
    puts (colorD_name.gazebo_id);
    serve_color[3] = 1;
  }

  if (colorA_name.tipo > 1) {
    serve_stereo = 1;
    strcpy (stereo_name, colorA_name.gazebo_id);
  }
  
  else if (colorB_name.tipo > 1) {
    serve_stereo = 1;
    strcpy (stereo_name, colorB_name.gazebo_id);
  }
  
  else if (colorC_name.tipo > 1) {
      serve_stereo = 1;
      strcpy (stereo_name, colorC_name.gazebo_id);
  }
  
  
  else if (colorD_name.tipo > 1) {
    serve_stereo = 1;
    strcpy (stereo_name, colorD_name.gazebo_id);
  }
  
  gazebo_deviceinit ();
  
  /* gazebo thread creation */
  pthread_mutex_lock (&mymutex);
  //state = slept;
  state = winner;
  pthread_create (&gazebo_th, NULL, gazebo_thread, NULL);
  pthread_mutex_unlock (&mymutex);
  
  if(serve_motors || serve_encoders){
    if (gz_position_open (position, client, motors_name) != 0) {
      position=NULL;
      puts ("error while opening position");
    }
  	}
  
  if (serve_motors)
    {
      all[num_schemas].id = (int *) &motors_schema_id;
      strcpy (all[num_schemas].name, "motors");
      all[num_schemas].run = (runFn) gazebo_motors_run;
      all[num_schemas].stop = (stopFn) gazebo_motors_stop;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,
	      num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
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
      strcpy (all[num_schemas].name, "laser");
      all[num_schemas].run = (runFn) gazebo_laser_run;
      all[num_schemas].stop = (stopFn) gazebo_laser_stop;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,
	      num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport ("laser", "id", &laser_schema_id);
      myexport ("laser", "laser", &jde_laser);
      myexport ("laser", "run", (void *) &gazebo_laser_run);
      myexport ("laser", "stop", (void *) &gazebo_laser_stop);
    }

  if (serve_ptmotors)
    {
      all[num_schemas].id = (int *) &ptmotors_schema_id;
      strcpy (all[num_schemas].name, "ptmotors");
      all[num_schemas].run = (runFn) gazebo_ptmotors_run;
      all[num_schemas].stop = (stopFn) gazebo_ptmotors_stop;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,
	      num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
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
      strcpy (all[num_schemas].name, "ptencoders");
      all[num_schemas].run = (runFn) gazebo_ptencoders_run;
      all[num_schemas].stop = (stopFn) gazebo_ptencoders_stop;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,
	      num_schemas);
      
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
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
      strcpy (all[num_schemas].name, "sonars");
      all[num_schemas].run = (runFn) gazebo_sonars_run;
      all[num_schemas].stop = (stopFn) gazebo_sonars_stop;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,
	      num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport ("sonars", "id", &sonars_schema_id);
      myexport ("sonars", "us", &us);
      myexport ("sonars", "run", (void *) &gazebo_sonars_run);
      myexport ("sonars", "stop", (void *) &gazebo_sonars_stop);
    }

  if (serve_encoders)
    {
      all[num_schemas].id = (int *) &encoders_schema_id;
      strcpy (all[num_schemas].name, "encoders");
      all[num_schemas].run = (runFn) gazebo_encoders_run;
      all[num_schemas].stop = (stopFn) gazebo_encoders_stop;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,
	      num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport ("encoders", "id", &encoders_schema_id);
      myexport ("encoders", "jde_robot", &jde_robot);
      myexport ("encoders", "run", (void *) &gazebo_encoders_run);
      myexport ("encoders", "stop", (void *) &gazebo_encoders_stop);
    }

  if (serve_color[0])
    {
      colorA = (char*)malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3*sizeof(char));
      pthread_mutex_lock(&color_mutex[0]);
      all[num_schemas].id = (int *) &camera_schema_id[0];
      strcpy (all[num_schemas].name, "colorA");
      all[num_schemas].run = (runFn) gazebo_camera0_run;
      all[num_schemas].stop = (stopFn) gazebo_camera0_stop;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,
	      num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("colorA","id",&camera_schema_id[0]);
      myexport("colorA","colorA",&colorA);
      myexport("colorA","run",(void *)gazebo_camera0_run);
      myexport("colorA","stop",(void *)gazebo_camera0_stop);
    }

  if (serve_color[1])
    {
      colorB = (char*)malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3*sizeof(char));
      pthread_mutex_lock(&color_mutex[1]);
      all[num_schemas].id = (int *) &camera_schema_id[1];
      strcpy (all[num_schemas].name, "colorB");
      all[num_schemas].run = (runFn) gazebo_camera1_run;
      all[num_schemas].stop = (stopFn) gazebo_camera1_stop;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,
	      num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("colorB","id",&camera_schema_id[1]);
      myexport("colorB","colorB",&colorB);
      myexport("colorB","run",(void *)gazebo_camera1_run);
      myexport("colorB","stop",(void *)gazebo_camera1_stop);
    }
  if (serve_color[2])
    {
      colorC = (char*)malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3*sizeof(char));
      pthread_mutex_lock(&color_mutex[2]);
      all[num_schemas].id = (int *) &camera_schema_id[2];
      strcpy (all[num_schemas].name, "colorC");
      all[num_schemas].run = (runFn) gazebo_camera2_run;
      all[num_schemas].stop = (stopFn) gazebo_camera2_stop;

      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,
	      num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("colorC","id",&camera_schema_id[2]);
      myexport("colorC","colorC",&colorC);
      myexport("colorC","run",(void *)gazebo_camera2_run);
      myexport("colorC","stop",(void *)gazebo_camera2_stop);

    }

  if (serve_color[3])
    {
      colorD = (char*)malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3*sizeof(char));
      pthread_mutex_lock(&color_mutex[3]);
      all[num_schemas].id = (int *) &camera_schema_id[3];
      strcpy (all[num_schemas].name, "colorD");
      all[num_schemas].run = (runFn) gazebo_camera3_run;
      all[num_schemas].stop = (stopFn) gazebo_camera3_stop;
      printf ("%s schema loaded (id %d)\n", all[num_schemas].name,
	      num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k = 0;
      all[num_schemas].state = slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("colorD","id",&camera_schema_id[3]);
      myexport("colorD","colorD",&colorD);
      myexport("colorD","run",(void *)gazebo_camera3_run);
      myexport("colorD","stop",(void *)gazebo_camera3_stop);
    }

  return 0;
}

void
gazebo_terminate ()
{
  gazebo_terminate_command = 1;
  if (position)
    {
      gz_position_close (position);
      gz_position_free (position);
      position=NULL;
    }
  
  if (ptz){
	gz_ptz_close(ptz);
	gz_ptz_free(ptz);
	ptz=NULL;
  }
  
  if (stereo){
    gz_stereo_close(stereo);
    gz_stereo_free(stereo);
    stereo=NULL;
  }
  
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
    }
  return 0;
}

int
gazebo_camera0_run (int father, int *brothers, arbitration fn)
{
  if (colorA_name.tipo == 1)
    {
      camera[0] = gz_camera_alloc ();
      if (gz_camera_open (camera[0], client, colorA_name.gazebo_id) != 0)
	{
	  fprintf (stderr, "Error openning the \"%s\" camera\n",
		   colorA_name.gazebo_id);
	  exit (-1);
	}
      
    }
  
   if (serve_color[0]==1) 
     {
       color_active[0]++;
       if ((all[camera_schema_id[3]].father==GUIHUMAN) ||
	   (all[camera_schema_id[3]].father==SHELLHUMAN))
	 all[camera_schema_id[3]].father = father;
       if(color_active[0]==1)
	 
	 {
	   pthread_mutex_unlock(&color_mutex[0]);
	   all[camera_schema_id[0]].father = father;
	   all[camera_schema_id[0]].fps = 0.;
	   all[camera_schema_id[0]].k = 0;
	   put_state (camera_schema_id[0], winner);

	   if((color_active[1]==0)&&(color_active[2]==0)&&(color_active[3]==0)){
	     /* gazebo thread goes winner */
	     pthread_mutex_lock(&mymutex);
	     state=winner;
	     pthread_cond_signal(&condition);
	     pthread_mutex_unlock(&mymutex);
	   }
	 }
     }

   return 0;
}

int
gazebo_camera1_run (int father, int *brothers, arbitration fn)
{
  if (colorB_name.tipo == 1)
    {
      camera[1] = gz_camera_alloc ();
      if (gz_camera_open (camera[1], client, colorB_name.gazebo_id) != 0)
	{
	  fprintf (stderr, "Error opening the \"%s\" camera\n",colorB_name.gazebo_id);
	  exit (-1);
	}
    }
  
  if (serve_color[1]==1) 
    {
      color_active[1]++;
      if ((all[camera_schema_id[3]].father==GUIHUMAN) ||
	  (all[camera_schema_id[3]].father==SHELLHUMAN))
	all[camera_schema_id[3]].father = father;
      if(color_active[1]==1)
	
	{
	  pthread_mutex_unlock(&color_mutex[1]);
	  
	  all[camera_schema_id[1]].father = father;
	  all[camera_schema_id[1]].fps = 0.;
	  all[camera_schema_id[1]].k = 0;
	  put_state (camera_schema_id[1], winner);
	  
	  if((color_active[0]==0)&&(color_active[2]==0)&&(color_active[3]==0)){
	    /* gazebo thread goes winner */
	    pthread_mutex_lock(&mymutex);
	    state=winner;
	    pthread_cond_signal(&condition);
	    pthread_mutex_unlock(&mymutex);
	  }
	}
    }
  
  return 0;
}

int
gazebo_camera2_run (int father, int *brothers, arbitration fn)
{
  if (colorC_name.tipo == 1)
    {
      camera[2] = gz_camera_alloc ();
      if (gz_camera_open (camera[2], client, colorC_name.gazebo_id) != 0)
	{
	  fprintf (stderr, "Error openning the %s camera\n",
		   colorC_name.gazebo_id);
	  exit (-1);
	}
    }
  
  if (serve_color[2]==1) 
    {
      color_active[2]++;
      if ((all[camera_schema_id[3]].father==GUIHUMAN) ||
	  (all[camera_schema_id[3]].father==SHELLHUMAN))
	all[camera_schema_id[3]].father = father;
      if(color_active[2]==1)
	
	{
	  pthread_mutex_unlock(&color_mutex[2]);
	  
	  all[camera_schema_id[2]].father = father;
	  all[camera_schema_id[2]].fps = 0.;
	  all[camera_schema_id[2]].k = 0;
	  put_state (camera_schema_id[2], winner);
	  
	  if((color_active[0]==0)&&(color_active[1]==0)&&(color_active[3]==0)){
	    /* gazebo thread goes winner */
	    pthread_mutex_lock(&mymutex);
	    state=winner;
	    pthread_cond_signal(&condition);
	    pthread_mutex_unlock(&mymutex);
	  }
	}
    }
  return 0;
}

int
gazebo_camera3_run (int father, int *brothers, arbitration fn)
{
  if (colorD_name.tipo == 1)
    {
      camera[3] = gz_camera_alloc ();
      if (gz_camera_open (camera[3], client, colorD_name.gazebo_id) != 0)
	{
	  fprintf (stderr, "Error openning the %s camera\n",
		colorD_name.gazebo_id);
	  exit (-1);
	}
    }

  if (serve_color[3]==1) 
    {
      color_active[3]++;
      if ((all[camera_schema_id[3]].father==GUIHUMAN) ||
	  (all[camera_schema_id[3]].father==SHELLHUMAN))
	all[camera_schema_id[3]].father = father;
      if(color_active[3]==1)
	
	{
	  pthread_mutex_unlock(&color_mutex[3]);
	  
	  all[camera_schema_id[3]].father = father;
	  all[camera_schema_id[3]].fps = 0.;
	  all[camera_schema_id[3]].k = 0;
	  put_state (camera_schema_id[3], winner);
	  
	  if((color_active[0]==0)&&(color_active[1]==0)&&(color_active[2]==0)){
	    /* gazebo tshread goes winner */
	    pthread_mutex_lock(&mymutex);
	    state=winner;
	    pthread_cond_signal(&condition);
	    pthread_mutex_unlock(&mymutex);
	  }
	}
    }
  return 0;
}




int
gazebo_camera0_stop ()
{
  color_active[0]--;
  if ((serve_color[0]) && (color_active[0]==0))
    {
      pthread_mutex_lock(&color_mutex[0]);
      put_state (camera_schema_id[0], slept);
      if (camera[0])
	{
	  gz_camera_close (camera[0]);
	  gz_camera_free (camera[0]);
	  camera[0] = NULL;
	}
      if((color_active[1]==0)&&(color_active[2]==0)&&(color_active[3]==0)){
	// gazebo thread goes sleep 
	pthread_mutex_lock(&mymutex);
	state=slept;
	pthread_mutex_unlock(&mymutex);
      }
      
    }
  return 0;
}

int
gazebo_camera1_stop ()
{
  color_active[1]--;
  if ((serve_color[1]) && (color_active[1]))
    {
      pthread_mutex_lock(&color_mutex[1]);
      put_state (camera_schema_id[1], slept);
      if (camera[1])
	{
	  gz_camera_close (camera[1]);
	  gz_camera_free (camera[1]);
	  camera[1] = NULL;
	}
      if((color_active[0]==0)&&(color_active[2]==0)&&(color_active[3]==0)){
	// gazebo thread goes sleep 
	pthread_mutex_lock(&mymutex);
	state=slept;
	pthread_mutex_unlock(&mymutex);
      }
    }
  return 0;
}

int
gazebo_camera2_stop ()
{
  color_active[2]--;
  if ((serve_color[2]) && (color_active[2]))
    {
      pthread_mutex_lock(&color_mutex[2]);
      put_state (camera_schema_id[2], slept);
      if (camera[2])
	{
	  gz_camera_close (camera[2]);
	  gz_camera_free (camera[2]);
	  camera[2] = NULL;
	}
      if((color_active[0]==0)&&(color_active[1]==0)&&(color_active[3]==0)){
	// gazebo thread goes sleep 
	pthread_mutex_lock(&mymutex);
	state=slept;
	pthread_mutex_unlock(&mymutex);
      }
    }
  return 0;
}

int
gazebo_camera3_stop ()
{
   color_active[3]--;
  if ((serve_color[3]) && (color_active[3]))
    {
      pthread_mutex_lock(&color_mutex[3]);
      printf ("gazebo: camera 3 stop\n");
      put_state (camera_schema_id[2], slept);
      if (camera[3])
	{
	  gz_camera_close (camera[3]);
	  gz_camera_free (camera[3]);
	  camera[3] = NULL;
	}
      if((color_active[0]==0)&&(color_active[1]==0)&&(color_active[2]==0)){
	// gazebo thread goes sleep 
	pthread_mutex_lock(&mymutex);
	state=slept;
	pthread_mutex_unlock(&mymutex);
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
    }
  return 0;
}

int
gazebo_sonars_run (int father, int *brothers, arbitration fn)
{
  if ((serve_sonars) && (sonars_active == 0))
    {
      sonar = gz_sonar_alloc ();
      if (!sonar)
	puts ("error allocating sonar interface");
      /*Abrim os y mActivamos los sensores */
      if (gz_sonar_open (sonar, client, sonar_name) != 0)
	{
	  fprintf (stderr, "Error opening sonars from %s robot\n",sonar_name);
	  return (-1);
	}

      sonar->data->cmd_enable_sonar = 1;

      //printf("El numero de sonars es %d \n",sonar->data->sonar_count);
      
      sonars_active = 1;

      put_state (sonars_schema_id, winner);

      printf ("gazebo: sonars run\n");

      all[sonars_schema_id].father = father;
      all[sonars_schema_id].fps = 0.;
      all[sonars_schema_id].k = 0;
	}
  return 0;
}

int
gazebo_sonars_stop ()
{
  if (sonar)
    {
      gz_sonar_close (sonar);
      gz_sonar_free (sonar);
      sonar = NULL;
    }

  if ((serve_sonars) && (sonars_active))
    {
      sonars_active = 0;
      put_state (sonars_schema_id, slept);
      printf ("gazebo: sonars stop\n");
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
    }
  return 0;
}

int
gazebo_laser_run (int father, int *brothers, arbitration fn)
{

  laser = gz_laser_alloc ();
  if (gz_laser_open (laser, client, laser_name) != 0)
    {
      fprintf (stderr, "Error openning the %s laser\n",laser_name);
      return (-1);
    }

  if ((serve_laser) && (laser_active == 0))
    {
      laser_active = 1;
      put_state (laser_schema_id, winner);
      printf ("gazebo: laser run\n");
      all[laser_schema_id].father = father;
      all[laser_schema_id].fps = 0.;
      all[laser_schema_id].k = 0;
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

      if (laser)
	{
	  gz_laser_close (laser);
	  gz_laser_free (laser);
	  laser = NULL;
	}

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
  struct timeval t,t2;
  unsigned long now, diff,next;
  static unsigned long lastmotor = 0;
  static unsigned long actualmotor = 0;
  static unsigned long lastiteration = 0;

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

	  if (colorA_name.tipo == 1 && color_active[0])
	    gazebo_camera_callback (0);

	  if (colorB_name.tipo == 1 && color_active[1])
	    gazebo_camera_callback (1);

	  if (colorC_name.tipo == 1 && color_active[2])
	    gazebo_camera_callback (2);

	  if (colorD_name.tipo == 1 && color_active[3])
	    gazebo_camera_callback (3);


	  if (colorA_name.tipo > 1 && color_active[0])
	    gazebo_stereo_callback (0);

	  if (colorB_name.tipo > 1 && color_active[0])
	    gazebo_stereo_callback (1);

	  if (colorC_name.tipo > 1 && color_active[0])
	    gazebo_stereo_callback (2);

	  if (colorD_name.tipo > 1 && color_active[0])
	    gazebo_stereo_callback (3);

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
  int i;

/* parece que un dispositivo es RGB y el otro BGR 
 * por tanto damos el cambiazo aqui 
 */
  char *destination;
  speedcounter (camera_schema_id[camnum]);
  gz_camera_lock (camera[camnum], 1);
  switch (camnum)
    {
    case 0:
      destination = colorA;
      break;
    case 1:
      destination = colorB;
      break;
    case 2:
      destination = colorC;
      break;
    case 3:
      destination = colorD;
      break;

    }
  for (i = 0; i < SIFNTSC_COLUMNS * SIFNTSC_ROWS * 3; i += 3)
    {

      destination[i] = camera[camnum]->data->image[i + 2];
      destination[i + 1] = camera[camnum]->data->image[i + 1];
      destination[i + 2] = camera[camnum]->data->image[i];
    }

  gz_camera_unlock (camera[camnum]);
}

void
gazebo_stereo_callback (int camnum)
{
  int i, imagen;
  char *destination;
  unsigned char *origen;

  speedcounter(camera_schema_id[camnum]);
  switch (camnum)
    {
    case 0:
      destination = colorA;
      imagen = colorA_name.tipo;
      break;
    case 1:
      destination = colorB;
      imagen = colorB_name.tipo;
      break;
    case 2:
      destination = colorC;
      imagen = colorC_name.tipo;
      break;
    case 3:
      destination = colorD;
      imagen = colorD_name.tipo;
      break;
    }

  switch (imagen)
    {
    case 2:
      origen = stereo->data->left_image;
      break;
    case 3:
      origen = stereo->data->right_image;
      break;
    case 4:
      /*origen = stereo->data->left_disparity;*/
      break;
    case 5:
      /*origen = stereo->data->right_disparity;*/
      break;
    }


  gz_stereo_lock (stereo, 1);

  for (i = 0; i < SIFNTSC_COLUMNS * SIFNTSC_ROWS * 3; i += 3)
    {
      destination[i] = origen[i + 2];
      destination[i + 1] = origen[i + 1];
      destination[i + 2] = origen[i];
    }

  gz_stereo_unlock (stereo);
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
    (position->data->pos[0]) * 1000 * (float) cos (DEGTORAD *
						   correcting_theta) -
    (position->data->pos[1]) * 1000 * (float) sin (DEGTORAD *
						   correcting_theta) +
    							correcting_x;
  roboty =
    (position->data->pos[1]) * 1000 * (float) cos (DEGTORAD *
						   correcting_theta) +
    (position->data->pos[0]) * 1000 * (float) sin (DEGTORAD *
						   correcting_theta) +
    							correcting_y;

  robottheta = (position->data->rot[2] * RADTODEG) + correcting_theta;

  if (robottheta <= 0)
    robottheta = robottheta + 360;

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
  char cpLinea[160];
  char *pLine1, *pLine2;
  int ItIsAGaceboLine = 0, i, colors;
  char *cameras[] = { "colorA", "colorB", "colorC", "colorD" };
  gc_name *colorX_name[] = { &colorA_name,
			     &colorB_name,
			     &colorC_name,
			     &colorD_name
  };


  if (!conf)
    return (-1);
  while (!feof (conf))
    {
      fgets (cpLinea, 160, conf);
      i = 0;
      while (isspace (cpLinea[i++]));
      if (cpLinea[i - 1] == '#')
	continue;
      if (strstr (cpLinea, "gazebo"))
	{
	  ItIsAGaceboLine = 1;
	  continue;
	}
      if (!ItIsAGaceboLine)
	continue;
      /* puts (cpLinea);*/
      /*tenemos lineas de nuestro driver */
      if (strstr (cpLinea, "end_driver"))
	break;
      for (colors = 0; colors < 4; colors++)
	{
	  
	  pLine1 = strstr (cpLinea, cameras[colors]);
	  if (pLine1 )
	    {
	      pLine2 = strstr (pLine1, " ");
	      for (i = 0; isspace (pLine2[0]); pLine2++);	//salto blancos
	      
	      if (strstr (pLine2, "leftd"))
		{		/*Stereo left dispsrity */
		  pLine1 = strstr (pLine2, " ");
		  pLine1[0] = (char) 0;
		  /* printf ("Nombre del estero leftd:%s:\n", pLine2); */
		  strcpy (colorX_name[colors]->gazebo_id, pLine2);
		  colorX_name[colors]->tipo = 4;	/*left disparity */
		}
	      
	      else if (strstr (pLine2, "rightd"))
		{		/*Stereo right dispsrity */
		  pLine1 = strstr (pLine2, " ");
		  pLine1[0] = (char) 0;
		  strcpy (colorX_name[colors]->gazebo_id, pLine2);	/*stereo id */
		  /* printf ("Nombre del estero rightd:%s:\n", pLine2); */
		  colorX_name[colors]->tipo = 5;	/*right disparity */
		}
	      
	      else if (strstr (pLine2, "left"))
		{		/*Stereo left */
		  pLine1 = strstr (pLine2, " ");
		  if (pLine1)
		    pLine1[0] = (char) 0;
		  /* printf ("Nombre de stereo encontrado en left:%s:\n", pLine2); */
		  strcpy (colorX_name[colors]->gazebo_id, pLine2);	/*stereo id */
		  colorX_name[colors]->tipo = 2;	/* left */
		}
	      
	      else if (strstr (pLine2, "right"))
		{		/*Stereo right */
		  pLine1 = strstr (pLine2, " ");
		  if (pLine1)
		    pLine1[0] = (char) 0;
		  /* printf ("Nombre de stereo encontrado en right:%s:\n", pLine2);*/
		  strcpy (colorX_name[colors]->gazebo_id, pLine2);	/*stereo id */
		  colorX_name[colors]->tipo = 3;	/*right */
		}
	      
	      else
		{		/*Se trata de una camara mono */
		  pLine1 = strstr (pLine2, "\n");
		  pLine1[0] = (char) 0;
		  strcpy (colorX_name[colors]->gazebo_id, pLine2);	/*camera id */
		  colorX_name[colors]->tipo = 1;	/*camara mono */
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
          /* printf("El nombre del laser es :%s:\n",pLine2); */
	  strncpy (laser_name, pLine2, MAX_MODEL_ID);
	}
      
      pLine2 = strstr (cpLinea, " motors ");
      if (pLine2)
	{			//Tomo el surtidor de motores
	  pLine2 += 8;
	  for (; isspace (pLine2[0]); pLine2++);	//salto blancos
	  pLine1 = strstr (pLine2, "\n");
	  pLine1[0] = (char) 0;
          /* printf("El nombre para motores es  :%s:\n",pLine2); */
	  strncpy (motors_name, pLine2, MAX_MODEL_ID);
	}
      
      pLine2 = strstr(cpLinea," encoders ");
      if (pLine2)
	{			//Tomo el surtidor de encoders
	  pLine2 += 10;
	  for (i = 0; isspace (pLine2[0]); pLine2++);	//salto blancos
	  pLine1 = strstr (pLine2, "\n");
	  pLine1[0] = (char) 0;
          /* printf("El nombre para encoders :%s:\n",pLine2);*/
	  strncpy (position_name, pLine2, MAX_MODEL_ID);
	}

      pLine2=strstr(cpLinea," sonars ");
      if (pLine2)
	{			//Tomo el surtidor de sonars
	  pLine2 += 8;
	  for (i = 0; isspace (pLine2[0]); pLine2++);	//salto blancos
	  pLine1 = strstr (pLine2, "\n");
	  pLine1[0] = (char) 0;
	  /*  printf("El nombre para sonar es  :%s:\n",pLine2);*/
	  strncpy (sonar_name, pLine2, MAX_MODEL_ID);
	}
      
      pLine2=strstr(cpLinea," pantilt ");
      if (pLine2)
	{			//Tomo el surtidor de ptz
	  pLine2 += 9;
	  for (i = 0; isspace (pLine2[0]); pLine2++);	//salto blancos
	  pLine1 = strstr (pLine2, "\n");
	  pLine1[0] = (char) 0;
	  /*   printf("El nombre para pantilt es  :%s:\n",pLine2);*/
	  strncpy (ptz_name, pLine2, MAX_MODEL_ID);
	}
      
    }
  
  fclose (conf);
  return 0;
}
