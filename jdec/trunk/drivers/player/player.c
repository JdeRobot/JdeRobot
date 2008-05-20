/*
 *  Copyright (C) 2006 Antonio Pineda Cabello, Raul Isado <rauli@mi.madritel.es>
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
 *  Authors : Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es>, Raul Isado <rauli@mi.madritel.es>
 */

/**
 *  jdec player driver provides sensorial information for robot variables from a player server.
 *  4.2 version includes support for bumpers
 *
 *  @file player.c
 *  @author Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es>, Raul Isado <rauli@mi.madritel.es> and José Antonio Santos Cadenas <santoscadenas@gmail.com>
 *  @version 4.2
 *  @date 30-05-2007
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <libplayerc/playerc.h>
#include <jde.h>

/** player driver command period cycle.*/
#define PLAYER_COMMAND_CYCLE 75 /* ms */

/** Robot max speed*/
#define MAX_VEL 1000 /* mm/sec, hardware limit: 1800 */
/** Robot max rotation speed*/
#define MAX_RVEL 180 /* deg/sec, hardware limit: 360 */

/** Maximum number of laser measures*/
#define MAX_LASER 720
/** Maximum number of sonar measures*/
#define MAX_SONAR 100
/** Maximum number of bumpers*/
#define MAX_BUMPER 100

/* for player support through the player server */
/** player driver client structure.*/
playerc_client_t *player_client;
/** player driver robot position structure.*/
playerc_position2d_t *player_position;
/** player driver sonar structure.*/
playerc_sonar_t *player_sonar;
/** player driver laser structure.*/
playerc_laser_t *player_laser;
/** player driver power structure.*/
playerc_power_t *player_power;
/** player driver bumpers structure*/
playerc_bumper_t *player_bumpers;
/** player driver debug active variable.*/
int DEBUG=0;

/* driver internal variables */
/** player driver time stamp variable.*/
unsigned long tag=0;

/* player thread attributes */
/** player driver pthread.*/
pthread_t player_th;
/** player driver state variable for pthread.*/
int state;
/** player driver mutex variable for pthread.*/
pthread_mutex_t mymutex;
/** player driver condition flag for pthread.*/
pthread_cond_t condition;

/* player driver API options */
/** player driver name.*/
char driver_name[256]="player";
/** player driver variable to detect when pthread must end its execution.*/
int player_close_command=0;

/** player driver variable to check if laser device was detected in config.*/
int serve_laser=0;
/** player driver variable to check if encoders device was detected in config.*/
int serve_encoders=0;
/** player driver variable to check if sonars device was detected in config.*/
int serve_sonars=0;
/** player driver variable to check if motors device was detected in config.*/
int serve_motors=0;
/** player driver variable to check if bumpers device was detected in config.*/
int serve_bumpers=0;
/** player driver variable to check if laser was activated in gui.*/
int laser_active=0;
/** player driver variable to check if encoders was activated in gui.*/
int encoders_active=0;
/** player driver variable to check if sonars was activated in gui.*/
int sonars_active=0;
/** player driver variable to check if motors was activated in gui.*/
int motors_active=0;
/** player driver variable to check if motors was activated in gui.*/
int bumpers_active=0;

/*API de variables servidas*/
/** 'encoders' schema, odometry information.*/
float jde_robot[5];
/** 'encoders' schema, clock variable.*/
unsigned long int encoders_clock;
/** 'encoders' schema variable positions*/
int encoders_number=5;

/** 'laser' schema, laser information.*/
int jde_laser[MAX_LASER];
/** 'laser' schema, clock variable.*/
unsigned long int laser_clock;
/** Number of laser samples*/
int laser_number=0;
/** Angular resolution samples per degree */
int laser_resolution=0;

/** 'sonars' schema, sonars information.*/
float us[MAX_SONAR];
/** 'sonars' schema, clock variable.*/
unsigned long int us_clock[MAX_SONAR];
/** Number of sonar samples*/
int sonar_number=0;

/** 'motors' schema, speed control.*/
float v; /* mm/s */
/** 'motors' schema, turn speed control.*/
float w; /* deg/s*/
/** 'motors' schema, cycle control.*/
int motors_cycle;

/** 'bumpers' schema bumpers information*/
unsigned char jde_bumpers[MAX_BUMPER];
/** 'bumpers' schema clock variable*/
unsigned long int bumpers_clock;
/** numbers of bumpers*/
int bumpers_number=0;

/*Contadores de referencias*/
/** laser ref counter*/
int laser_refs=0;
/** encoders ref counter*/
int encoders_refs=0;
/** sonars ref counter*/
int sonars_refs=0;
/** motors ref counter*/
int motors_refs=0;
/** bumpers ref counter*/
int bumpers_refs=0;

/** mutex for ref counters*/
pthread_mutex_t refmutex;

/** id for laser schema.*/
int laser_schema_id;
/** id for encoders schema.*/
int encoders_schema_id;
/** id for sonars schema.*/
int sonars_schema_id;
/** id for motors schema.*/
int motors_schema_id;
/** if for bumpers schema*/
int bumpers_schema_id;

/** player driver connection port.*/
int playerport;
/** player driver hostname.*/
char playerhost[256];
/** player driver factor correction for x.*/
float correcting_x=0.;
/** player driver factor correction for y.*/
float correcting_y=0.;
/** player driver factor correction for theta.*/
float correcting_theta=0.;

/** player driver last w speed command sent to the motors.*/
static float w_sp=0.;
/** player driver last v speed command sent to the motors.*/
static float v_sp=0.;

/*Callback declaration*/
/** player driver laser function callback.*/
void player_laser_callback();
/** player driver encoders function callback.*/
void player_encoders_callback(void *not_used);
/** player driver sonars function callback.*/
void player_sonar_callback(void *not_used);
/** player driver sonars function callback.*/
void player_bumper_callback(void *not_used);


/* PLAYER DRIVER FUNCTIONS */
/** laser resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int player_laser_resume(int father, int *brothers, arbitration fn)
{
   pthread_mutex_lock(&refmutex);
   if (laser_refs>0){
      laser_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      laser_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_laser)&&(laser_active==0)){
         laser_active=1;
         put_state(laser_schema_id,winner);
         printf("laser schema resume (player driver)\n");
         all[laser_schema_id].father = father;
         all[laser_schema_id].fps = 0.;
         all[laser_schema_id].k =0;
    
         if((encoders_active==0)&&(sonars_active==0)&&(motors_active==0)&&(bumpers_active==0)){
            /* player thread goes winner */
            pthread_mutex_lock(&mymutex);
            state=winner;
            pthread_cond_signal(&condition);
            pthread_mutex_unlock(&mymutex);
         }
      }
   }
   return 0;
}

/** laser suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int player_laser_suspend(){
   pthread_mutex_lock(&refmutex);
   if (laser_refs>1){
      laser_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      laser_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_laser)&&(laser_active)){
         laser_active=0;
         put_state(laser_schema_id,slept);
         printf("laser schema suspend (player driver)\n");
         if((encoders_active==0)&&(sonars_active==0)&&(motors_active==0)&&(bumpers_active==0)){
            /* player thread goes sleep */
            pthread_mutex_lock(&mymutex);
            state=slept;
            pthread_mutex_unlock(&mymutex);
         }
      }
   }
   return 0;
}

/** encoders resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int player_encoders_resume(int father, int *brothers, arbitration fn)
{
   pthread_mutex_lock(&refmutex);
   if (encoders_refs>0){
      encoders_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      encoders_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_encoders)&&(encoders_active==0)){
         encoders_active=1;
         put_state(encoders_schema_id,winner);
         printf("encoders schema resume (player driver)\n");
         all[encoders_schema_id].father = father;
         all[encoders_schema_id].fps = 0.;
         all[encoders_schema_id].k =0;

         if((laser_active==0)&&(sonars_active==0)&&(motors_active==0)&&(bumpers_active==0)){
            /* player thread goes winner */
            pthread_mutex_lock(&mymutex);
            state=winner;
            pthread_cond_signal(&condition);
            pthread_mutex_unlock(&mymutex);
         }
      }
   }
   return 0;
}

/** encoders suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int player_encoders_suspend(){
   pthread_mutex_lock(&refmutex);
   if (encoders_refs>1){
      encoders_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      encoders_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_encoders)&&(encoders_active)){
         encoders_active=0;
         put_state(encoders_schema_id,slept);
         printf("encoders schema suspend (player driver)\n");
         if((laser_active==0)&&(sonars_active==0)&&(motors_active==0)&&(bumpers_active==0)){
            /* player thread goes sleep */
            pthread_mutex_lock(&mymutex);
            state=slept;
            pthread_mutex_unlock(&mymutex);
         }
      }
   }
   return 0;
}

/** sonars resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int player_sonars_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (sonars_refs>0){
      sonars_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      sonars_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_sonars)&&(sonars_active==0)){
         sonars_active=1;
         put_state(sonars_schema_id,winner);
         printf("sonars schema resume (player driver)\n");
         all[sonars_schema_id].father = father;
         all[sonars_schema_id].fps = 0.;
         all[sonars_schema_id].k =0;

         if((laser_active==0)&&(encoders_active==0)&&(motors_active==0)&&(bumpers_active==0)){
            /* player thread goes winner */
            pthread_mutex_lock(&mymutex);
            state=winner;
            pthread_cond_signal(&condition);
            pthread_mutex_unlock(&mymutex);
         }
      }
   }
   return 0;
}

/** sonars suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int player_sonars_suspend(){
   pthread_mutex_lock(&refmutex);
   if (sonars_refs>1){
      sonars_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      sonars_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_sonars)&&(sonars_active)){
         sonars_active=0;
         put_state(sonars_schema_id,slept);
         printf("sonars schema suspend (player driver)\n");
         if((laser_active==0)&&(encoders_active==0)&&(motors_active==0)&&(bumpers_active==0)){
            /* player thread goes to sleep */
            pthread_mutex_lock(&mymutex);
            state=slept;
            pthread_mutex_unlock(&mymutex);
         }
      }
   }
   return 0;
}

/** bumpers resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int player_bumpers_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (bumpers_refs>0){
      bumpers_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      bumpers_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_bumpers)&&(bumpers_active==0)){
         bumpers_active=1;
         put_state(bumpers_schema_id,winner);
         printf("bumpers schema resume (player driver)\n");
         all[bumpers_schema_id].father = father;
         all[bumpers_schema_id].fps = 0.;
         all[bumpers_schema_id].k =0;

         if((laser_active==0)&&(encoders_active==0)&&(motors_active==0)&&(sonars_active==0)){
            /* player thread goes winner */
            pthread_mutex_lock(&mymutex);
            state=winner;
            pthread_cond_signal(&condition);
            pthread_mutex_unlock(&mymutex);
         }
      }
   }
   return 0;
}

/** bumpers suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int player_bumpers_suspend(){
   pthread_mutex_lock(&refmutex);
   if (bumpers_refs>1){
      bumpers_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      bumpers_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_bumpers)&&(bumpers_active)){
         bumpers_active=0;
         put_state(bumpers_schema_id,slept);
         printf("bumpers schema suspend (player driver)\n");
         if((laser_active==0)&&(encoders_active==0)&&(motors_active==0)&&(sonars_active==0)){
            /* player thread goes to sleep */
            pthread_mutex_lock(&mymutex);
            state=slept;
            pthread_mutex_unlock(&mymutex);
         }
      }
   }
   return 0;
}

/** motors resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int player_motors_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (motors_refs>0){
      motors_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      motors_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_motors)&&(motors_active==0)){
         motors_active=1;
         put_state(motors_schema_id,winner);
         printf("motors schema resume (player driver)\n");
         all[motors_schema_id].father = father;
         all[motors_schema_id].fps = 0.;
         all[motors_schema_id].k =0;

         if((laser_active==0)&&(encoders_active==0)&&(sonars_active==0)&&(bumpers_active==0)){
            pthread_mutex_lock(&mymutex);
            state=winner;
            pthread_cond_signal(&condition);
            pthread_mutex_unlock(&mymutex);
         }
      }
   }
   return 0;
}

/** motors suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int player_motors_suspend(){
   pthread_mutex_lock(&refmutex);
   if (motors_refs>1){
      motors_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      motors_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_motors)&&(motors_active)){
         /* security stop */
         v_sp=0; w_sp=0;
         playerc_position2d_set_cmd_vel(player_position,v_sp,0,w_sp,0);

         motors_active=0;
         put_state(motors_schema_id,slept);
         printf("motors schema resume (player driver)\n");
         if((laser_active==0)&&(encoders_active==0)&&(sonars_active==0)&&(bumpers_active==0)){
            pthread_mutex_lock(&mymutex);
            state=slept;
            pthread_mutex_unlock(&mymutex);
         }
      }
   }
   return 0;
}

/** player driver motors iteration function to send commands to player server.*/
void player_motors_iteration(){

  float  w_new, v_new;

  int v_integer=0; int w_integer=0;

  speedcounter(motors_schema_id);

  /* getting speed in integer value */
  v_integer=(int)v; w_integer=(int)w;

  if(v_integer>MAX_VEL) v_new=MAX_VEL/1000;
  else if (v_integer<-MAX_VEL) v_new=-MAX_VEL/1000;
  else v_new=v/1000;

  if(w_integer>MAX_RVEL) w_new=MAX_RVEL*DEGTORAD;
  else if(w_integer<-MAX_RVEL) w_new=-MAX_RVEL*DEGTORAD;
  else w_new=w*DEGTORAD;

  /* sending speed to player server */
  /*  We send speed commands in all iterations, to prevent the watchdog at the 
      pioneer robot stopping the robot when the w_new or the v_new are constant 
      for a long period. Now we get a continuous movement */
  /*  if ((v_sp!=v_new)||(w_sp!=w_new))
      {
  */
  v_sp=v_new;
  w_sp=w_new;
  playerc_position2d_set_cmd_vel(player_position,v_sp,0,w_sp,0);
  /* }*/
}


/** player driver main thread.*/
void *player_thread(){
  struct timeval t;
  unsigned long now;
  static unsigned long lastmotor=0;
  /*
  static unsigned long last=0;
  static int howmany=0;
  */

  printf("player: player thread started up\n");
  
  do{

    if (player_close_command==0){

       pthread_mutex_lock(&mymutex);
       if (state==slept){
          printf("player: player thread in sleep mode\n");
          pthread_cond_wait(&condition,&mymutex);
          printf("player: player thread woke up\n");
          pthread_mutex_unlock(&mymutex);

       }else{

          pthread_mutex_unlock(&mymutex);

          gettimeofday(&t,NULL);
          now=t.tv_sec*1000000+t.tv_usec;
      /*printf("%ld, %ld\n",now-before,now-lastmotor);
          before=now;
      */

          /* sending motors command */
          if((serve_motors)&&(motors_active))
          {
             if ((now-lastmotor) > PLAYER_COMMAND_CYCLE*1000)
             { lastmotor=now;
             player_motors_iteration();
             }
          }
          /* player_motors_iteration();*/

          /* reading from player server */
          playerc_client_read(player_client);

      /*
          gettimeofday(&t,NULL);
          now=t.tv_sec*1000000+t.tv_usec;
          howmany++;
          if ((now-last)>10000000)
          {fprintf(stderr,"player: client -> %d iterations in 10 secs\n",howmany);
          last = now;
          howmany=0;
       }
      */
       }
    }
  }while(player_close_command==0);
  playerc_client_disconnect(player_client);
//   playerc_client_destroy(player_client);
}

/** player driver closing function invoked when stopping driver.*/
void player_close(){
   player_close_command=1;
   player_laser_suspend();
   player_encoders_suspend();
   player_sonars_suspend();
   player_bumpers_suspend();
   player_motors_suspend();

   if (serve_encoders==1){
      playerc_client_delcallback(player_client,&(player_position->info),
                                 &player_encoders_callback,&(player_position));
   }
   if (serve_sonars==1){
      playerc_client_addcallback(player_client,&(player_sonar->info),
                                 &player_sonar_callback,&(player_sonar->scan));
   }
   if (serve_laser==1){
      playerc_client_addcallback(player_client,&(player_laser->info),
                                 &player_laser_callback,&(player_laser->scan));
   }
   if (serve_bumpers==1){
      playerc_client_addcallback(player_client,&(player_bumpers->info),
                                 &player_bumper_callback,
                                 &(player_bumpers->bumpers));
   }

   pthread_cond_signal(&condition);
   printf("disconnected from Player\n");
}

/** player driver laser function callback.*/
void player_laser_callback(){
   int i=0;
   double j=0.0;
   int player_resolution;
   double jump;
   int offset;
  
   speedcounter(laser_schema_id);
   laser_clock=tag++;

   /*Now we must transform player resolution into user defined resolution*/
   player_resolution=1/(RADTODEG*player_laser->scan_res);
   jump=(double)player_resolution / (double)laser_resolution;
 
   /*Check if is possible serve this resolution and number of measures*/
   if (jump < 1.0){
      fprintf(stderr,"player: I can't serve laser at %d measures per degree\n",
              laser_resolution);
      jdeshutdown(-1);
   }
   else if((player_laser->scan_count/jump) < laser_number){
      fprintf(stderr,"player: I can't serve laser at %d measures\n",
              laser_number);
      jdeshutdown(-1);
   }

   offset=((player_laser->scan_count/jump)-laser_number)/2;
   i=0;
   j=offset;
   while(j<player_laser->scan_count && i<laser_number){
      jde_laser[i]=(int)(player_laser->scan[(int)j][0]*1000);
      j=j+jump;
      i++;
   }
}

/** player driver encoders function callback.*/
void player_encoders_callback(void *not_used)
{
  float robotx,roboty,robottheta;
 
  speedcounter(encoders_schema_id);
  encoders_clock=tag;
  tag++;

  robotx=(player_position->px)*1000*(float)cos(DEGTORAD*correcting_theta) - (player_position->py)*1000*(float)sin(DEGTORAD*correcting_theta) + correcting_x;
  roboty=(player_position->py)*1000*(float)cos(DEGTORAD*correcting_theta) + (player_position->px)*1000*(float)sin(DEGTORAD*correcting_theta) + correcting_y;
  robottheta=(player_position->pa*RADTODEG) + correcting_theta;
  if(robottheta<=0) robottheta=robottheta+360;

  jde_robot[0]=robotx;
  jde_robot[1]=roboty;
  jde_robot[2]=robottheta*DEGTORAD;
  jde_robot[3]=cos(jde_robot[2]);
  jde_robot[4]=sin(jde_robot[2]);
}

/** player driver sonars function callback.*/
void player_sonar_callback(void *not_used)
{
  int j;
  
  speedcounter(sonars_schema_id);  

  if (player_sonar->pose_count > MAX_SONAR){
     fprintf (stderr, "player: I can't serve %d sonar measures, maximum %d.\n",
              player_sonar->pose_count, MAX_SONAR);
     jdeshutdown(-1);
  }
  for (j=0;j<player_sonar->pose_count; j++){
    /* for pioneer 16 data per reading */
    us[j]=(float)player_sonar->scan[j]*1000;
    us_clock[j]=tag;
    tag++;
  }
  sonar_number=player_sonar->pose_count;
}

/** player driver sonars function callback.*/
void player_bumper_callback(void *not_used)
{
   int j;
  
   speedcounter(bumpers_schema_id);
   
   if (player_bumpers->bumper_count > MAX_BUMPER){
      fprintf (stderr, "player: I can't serve %d bumper measures, maximum %d.\n",
               player_bumpers->bumper_count, MAX_BUMPER);
      jdeshutdown(-1);
   }
   
   for (j=0;j<player_bumpers->bumper_count; j++){
      /* for pioneer 16 data per reading */
      jde_bumpers[j]=(float)player_bumpers->bumpers[j];
      bumpers_clock=tag;
      tag++;
   }
   bumpers_number=player_bumpers->bumper_count;
}

/** player driver battery function callback.*/
void player_battery_callback(void *not_used)
{
  /* this is unused in JDE since we don't use the charge battery sensor.
     if you want to use this option you only have to use the 'player_power->charge' */
  tag++;
}


/** player driver init function. It will start all player required devices and setting them the default configuration.
 *  @return 0 if initialitation was successful or -1 if something went wrong.*/
int player_init(){

  printf("connecting to Player Server at '%s:%d'\n",playerhost,playerport);
  player_client = playerc_client_create(NULL,playerhost,playerport);
  if (playerc_client_connect(player_client) != 0)
    {
      fprintf(stderr, "player: %s\n", playerc_error_str());
      jdeshutdown(1);
    }
 
  /* Create a position proxy (device id "position:0") and susbscribe in read/write mode */
  if(serve_encoders==1){
    player_position = playerc_position2d_create(player_client, 0);  
    if (playerc_position2d_subscribe(player_position, PLAYER_OPEN_MODE) != 0)
      {
	fprintf(stderr, "player: %s\n", playerc_error_str());
	printf("player: did you set 'position2d:0' in the robot provides line at Player config file?\n");
	printf("player: cannot initiate Player connection\n");
	jdeshutdown(1);
      }
    playerc_position2d_enable(player_position,1);

    /* registering the encoders callback to call when new encoders data arrived  */
    playerc_client_addcallback(player_client,&(player_position->info),&player_encoders_callback,&(player_position));
  } 

  /*Create a sonar proxy and subscribe in read/write mode*/
  if(serve_sonars){
    player_sonar=playerc_sonar_create(player_client, 0);
    if (playerc_sonar_subscribe(player_sonar, PLAYER_OPEN_MODE) != 0)
      {
	fprintf(stderr, "player: %s\n", playerc_error_str());
	printf("player: did you set 'sonar:0' in the robot provides line at Player config file?\n");
	printf("player: cannot initiate Player connection\n");
	jdeshutdown(1);
    }
    playerc_sonar_get_geom(player_sonar);

    /* registering the sonars callback to call when new sonars data arrived */
    playerc_client_addcallback(player_client,&(player_sonar->info),&player_sonar_callback,&(player_sonar->scan));
  }

  /*Create a laser proxy and subscribe in read/write mode*/

  if(serve_laser){
    player_laser=playerc_laser_create(player_client, 0);
    if (playerc_laser_subscribe(player_laser, PLAYER_OPEN_MODE) != 0)
      {
	fprintf(stderr, "player: %s\n", playerc_error_str());
	printf("player: did you set 'laser:0' in the robot provides line at Player config file?\n");
	printf("player: cannot initiate Player connection\n");
	jdeshutdown(1);
      }

    /* registering the laser callback to call when new laser data arrived */
    playerc_client_addcallback(player_client,&(player_laser->info),&player_laser_callback,&(player_laser->scan));
  }    

  if (serve_bumpers){
     player_bumpers=playerc_bumper_create(player_client, 0);
     if (playerc_bumper_subscribe(player_bumpers, PLAYER_OPEN_MODE) != 0){
        fprintf(stderr, "player: %s\n", playerc_error_str());
        printf("player: did you set 'bumper:0' in the robot provides line at Player config file?\n");
        printf("player: cannot initiate Player connection\n");
        jdeshutdown(1);
     }

     /* Register a callback to refresh bumpers data*/
     playerc_client_addcallback(player_client,&(player_bumpers->info),&player_bumper_callback,&(player_bumpers->bumpers));
  }
  /*if(serve_battery){
    player_power=playerc_power_create(player_client, 0);
    if (playerc_power_subscribe(player_power, PLAYER_OPEN_MODE) != 0)
    {
    fprintf(stderr, "Player: %s\n", playerc_error_str());
    return -1;
    }
    playerc_client_addcallback(player_client,&(player_power->info),&player_battery_callback,&(player_power->charge));
    }*/
  
  return 0;
}

/** player driver parse configuration file function.
 *  @param configfile path and name to the config file.
 *  @return 0 if parsing was successful or -1 if something went wrong.*/
int player_parseconf(char *configfile){

  int end_parse=0; int end_section=0; int driver_config_parsed=0;
  int hostname_hostport_ok=0;
  FILE *myfile;
  const int limit = 256;

  if ((myfile=fopen(configfile,"r"))==NULL){
    printf("player: cannot find config file\n");
    return -1;
  }

  do{
    
    char word[256],word2[256],buffer_file[256];
    int i=0; int j=0;

    buffer_file[0]=fgetc(myfile);
    
    /* end of file */
    if (feof(myfile)){
      end_section=1;
      end_parse=1;
      
      /* line comment */
    }else if (buffer_file[0]=='#') {
      while(fgetc(myfile)!='\n');
      
      /* white spaces */
    }else if (buffer_file[0]==' ') {
      while(buffer_file[0]==' ') buffer_file[0]=fgetc(myfile);

      /* tabs */
    }else if(buffer_file[0]=='\t') {
      while(buffer_file[0]=='\t') buffer_file[0]=fgetc(myfile);
      /* storing line in buffer */
    }else{
      
      while(buffer_file[i]!='\n') buffer_file[++i]=fgetc(myfile);
      buffer_file[++i]='\0';

      if (i >= limit-1) { 
	printf("%s...\n", buffer_file); 
	printf ("Line too long in config file!\n"); 
	exit(-1);
      }
      
      /* first word of the line */     
      if (sscanf(buffer_file,"%s",word)==1){
	if (strcmp(word,"driver")==0) {
	  while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
	  sscanf(&buffer_file[j],"%s",word2);
	  
	  /* checking if this section matchs our driver name */
	  if (strcmp(word2,driver_name)==0){
	    /* the sections match */
	    do{
	      
	      char buffer_file2[256],word3[256],word4[256],word5[256],word6[256];
	      int k=0; int z=0;

	      buffer_file2[0]=fgetc(myfile);
	      
	      /* end of file */
	      if (feof(myfile)){
		end_section=1;
		end_parse=1;
		
		/* line comment */
	      }else if (buffer_file2[0]=='#') {
		while(fgetc(myfile)!='\n');
	      
		/* white spaces */
	      }else if (buffer_file2[0]==' ') {
		while(buffer_file2[0]==' ') buffer_file2[0]=fgetc(myfile);

		/* tabs */
	      }else if(buffer_file2[0]=='\t') {
		while(buffer_file2[0]=='\t') buffer_file2[0]=fgetc(myfile);

		/* storing line in buffer */
	      }else{
		
		while(buffer_file2[k]!='\n') buffer_file2[++k]=fgetc(myfile);
		buffer_file2[++k]='\0';
		
		/* first word of the line */
		if (sscanf(buffer_file2,"%s",word3)==1){
		  if (strcmp(word3,"end_driver")==0) {
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    driver_config_parsed=1;
		    end_section=1;
		    end_parse=1;
		    
		  }else if (strcmp(word3,"driver")==0) {
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    printf("player: error in config file.\n'end_section' keyword required before starting new driver section.\n");
		    end_section=1; end_parse=1;

		  }else if(strcmp(word3,"provides")==0){
                    int words;
		    while((buffer_file2[z]!='\n') && (buffer_file2[z]!=' ') &&
                                         (buffer_file2[z]!='\0') &&
                                         (buffer_file2[z]!='\t'))
                    {
                       z++;
                    }
		    if((words=sscanf(buffer_file2,"%s %s %s %s",word3,word4,word5,word6))>1){
		      if(strcmp(word4,"laser")==0 && words==4){
                         serve_laser=1;
                         laser_number=atoi(word5);
                         laser_resolution=atoi(word6);
                         if (laser_number > MAX_LASER){
                            fprintf (stderr, "player: %d laser measures cannot be served, maximum %d.\n",
                                     laser_number, MAX_LASER);
                         }
                      }
		      else if(strcmp(word4,"sonars")==0) serve_sonars=1;
		      else if(strcmp(word4,"encoders")==0) serve_encoders=1;
		      else if(strcmp(word4,"motors")==0) serve_motors=1;
                      else if(strcmp(word4,"bumpers")==0) serve_bumpers=1;

		    }else{
		      printf("player: provides line incorrect\n");
		    }

		  }else if(strcmp(word3,"hostname")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s %s %s",word3,word4,word5,word6)>3){
		      strcpy(playerhost,word4);
		      if(strcmp(word5,"port")!=0) printf("player: unrecognized word '%s'. assuming you mean 'port'.\n",word5);
		      playerport=atoi(word6);
		      hostname_hostport_ok=1;

		    }else{
		      printf("player: hostname line incorrect\n");
		    }

		  }else if(strcmp(word3,"initial_position")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s %s %s",word3,word4,word5,word6)>3){
		      correcting_x=(float)atof(word4);
		      correcting_y=(float)atof(word5);
		      correcting_theta=(float)atof(word6);
		      printf("player: correcting x=%.1f(mm)  y=%.1f(mm)  theta=%.1f(deg).\n",correcting_x,correcting_y,correcting_theta);
		      printf("player: make sure this is the initial position in world file.\n");

		    }else{
		      printf("player: initial_position line incorrect\n");
		    }

		  }else printf("player: I don't know what to do with '%s'\n",buffer_file2);
		}
	      }
	    }while(end_section==0);
	    end_section=0;
	  }
	}
      }
    }
  }while(end_parse==0);
  
  /* checking if a driver section was read correctly */
  if((driver_config_parsed==1)&&(hostname_hostport_ok==1)){
    if((serve_laser==0)&&(serve_motors==0)&&(serve_encoders==0)&&(serve_sonars==0)&&(serve_bumpers==0)){
      printf("player: warning! no device provided.\n");
    }
    return 0;

  }else return -1;
}

/** player driver startup function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
int player_startup(char *configfile){

  pthread_mutex_init(&mymutex,PTHREAD_MUTEX_TIMED_NP);
  /* we call the function to parse the config file */
  if(player_parseconf(configfile)==-1){
    printf("player: cannot initiate driver. configfile parsing error.\n");
    exit(-1);
  }

  /* player initialitation */
  player_init();

  /* player thread creation */
  pthread_mutex_lock(&mymutex);
  state=slept;
  pthread_create(&player_th,NULL,player_thread,NULL);
  pthread_mutex_unlock(&mymutex);

  /* resume and suspend asignments */
  if(serve_laser)
    {
      all[num_schemas].id = (int *) &laser_schema_id;
      strcpy(all[num_schemas].name,"laser");
      all[num_schemas].resume = (resumeFn) player_laser_resume;
      all[num_schemas].suspend = (suspendFn) player_laser_suspend;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].close = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("laser","id",&laser_schema_id);
      myexport("laser","laser",&jde_laser);
      myexport("laser","clock", &laser_clock);
      myexport("laser","number", &laser_number);
      myexport("laser","resolution", &laser_resolution);
      myexport("laser","resume",(void *) &player_laser_resume);
      myexport("laser","suspend",(void *) &player_laser_suspend);
    }

  if(serve_encoders)
    {
      all[num_schemas].id = (int *) &encoders_schema_id;
      strcpy(all[num_schemas].name,"encoders");
      all[num_schemas].resume = (resumeFn) player_encoders_resume;
      all[num_schemas].suspend = (suspendFn) player_encoders_suspend;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].close = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("encoders","id",&encoders_schema_id);
      myexport("encoders","jde_robot",&jde_robot);
      myexport("encoders", "clock", &encoders_clock);
      myexport("encoders", "number", &encoders_number);
      myexport("encoders","resume",(void *)&player_encoders_resume);
      myexport("encoders","suspend",(void *)&player_encoders_suspend);
    }

  if(serve_sonars)
    {
      all[num_schemas].id = (int *) &sonars_schema_id;
      strcpy(all[num_schemas].name,"sonars");
      all[num_schemas].resume = (resumeFn) player_sonars_resume;
      all[num_schemas].suspend = (suspendFn) player_sonars_suspend;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].close = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("sonars","id",&sonars_schema_id);
      myexport("sonars","us",&us);
      myexport("sonars","clock", &us_clock);
      myexport("sonars","number", &sonar_number);
      myexport("sonars","resume",(void *)&player_sonars_resume);
      myexport("sonars","suspend",(void *)&player_sonars_suspend);
    }

  if(serve_motors)
    {
      all[num_schemas].id = (int *) &motors_schema_id;
      strcpy(all[num_schemas].name,"motors");
      all[num_schemas].resume = (resumeFn) player_motors_resume;
      all[num_schemas].suspend = (suspendFn) player_motors_suspend;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].close = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("motors","id",&motors_schema_id);
      myexport("motors","v",&v);
      myexport("motors","w",&w);
      myexport("motors","cycle",&motors_cycle);
      myexport("motors","resume",(void *)&player_motors_resume);
      myexport("motors","suspend",(void *)&player_motors_suspend);
    }

  if(serve_bumpers)
    {
       all[num_schemas].id = (int *) &bumpers_schema_id;
       strcpy(all[num_schemas].name,"bumpers");
       all[num_schemas].resume = (resumeFn) player_bumpers_resume;
       all[num_schemas].suspend = (suspendFn) player_bumpers_suspend;
       printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
       (*(all[num_schemas].id)) = num_schemas;
       all[num_schemas].fps = 0.;
       all[num_schemas].k =0;
       all[num_schemas].state=slept;
       all[num_schemas].close = NULL;
       all[num_schemas].handle = NULL;
       num_schemas++;
       myexport("bumpers","id",&bumpers_schema_id);
       myexport("bumpers","bumpers",&jde_bumpers);
       myexport("bumpers","number",&bumpers_number);
       myexport("bumpers","clock", &bumpers_clock);
       myexport("bumpers","resume",(void *) &player_bumpers_resume);
       myexport("bumpers","suspend",(void *) &player_bumpers_suspend);
    }
  return 0;
}
