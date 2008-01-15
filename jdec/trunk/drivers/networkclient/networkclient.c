/*
 *  Copyright (C) 2006 Antonio Pineda Cabello
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
 *  Authors : Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es>
              Jose Maria Cañas <jmplaza@gsyc.escet.urjc.es>
              Jose Antonio Santos Cadenas <santoscadenas@gmail.com>
 */

/**
 *  jdec networkclient driver provides sensorial information to platform variables such as color, laser or us, from remote jdec networkservers drivers, or oculo and otos servers.
 *
 *  @file networkclient.c
 *  @author Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es>, Jose Maria Cañas Plaza <jmplaza@gsyc.escet.urjc.es> and Jose Antonio Santos Cadenas <santoscadenas@gmail.com>
 *  @version 4.1
 *  @date 30-05-2007
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "jde.h"
#include "jdemessages.h"

/** networkclient driver oneimage socket read mode.*/
#define ONEIMAGE 0
/** networkclient driver messages socket read mode.*/
#define MESSAGES 1

/** Maximum number of sonar measures (picked from player.h)*/
#define SONAR_MAX 64
/** Maximum number of laser measures (picked from player.h)*/
#define LASER_MAX 1024

/** Image standard number of rows*/
#define SIFNTSC_ROWS 240
/** Image standard number of columns*/
#define SIFNTSC_COLUMNS 320

/** networkclient client name.*/
char CLIENT_NAME[MAX_CLIENT_NAME]="jdec";

/** networkclient max number of devices.*/
#define MAXDEVICE 14

/** Maximum number of images served*/
#define MAXCAM 8

/** networkclient colorA device id.*/
#define COLORA_DEVICE 0
/** networkclient colorB device id.*/
#define COLORB_DEVICE 1
/** networkclient colorC device id.*/
#define COLORC_DEVICE 2
/** networkclient colorD device id.*/
#define COLORD_DEVICE 3
/** networkclient pantilt encoders device id.*/
#define PANTILT_ENCODERS_DEVICE 4
/** networkclient pantilt motors device id.*/
#define PANTILT_MOTORS_DEVICE 5
/** networkclient laser device id.*/
#define LASER_DEVICE 6
/** networkclient sonars device id.*/
#define SONARS_DEVICE 7
/** networkclient encoders device id.*/
#define ENCODERS_DEVICE 8
/** networkclient motors device id.*/
#define MOTORS_DEVICE 9
/** networkclient varcolorA device id.*/
#define VARCOLORA_DEVICE 10
/** networkclient varcolorB device id.*/
#define VARCOLORB_DEVICE 11
/** networkclient varcolorC device id.*/
#define VARCOLORC_DEVICE 12
/** networkclient varcolorD device id.*/
#define VARCOLORD_DEVICE 13

/* 4 images, 4 robot devices and 2 devices for pantilt */
/** networkclient pthreads structure.*/
pthread_t network_thread[MAXDEVICE];
/** networkclient state variable for pthreads.*/
int state[MAXDEVICE];
/** networkclient mutex for pthreads.*/
pthread_mutex_t mymutex[MAXDEVICE];
/** networkclient condition flags for pthreads.*/
pthread_cond_t condition[MAXDEVICE];

/** networkclient driver name.*/
char driver_name[256]="networkclient";

/* devices detected and their hostnames, ports and network id's */
/** networkclient detected hostnames in config file.*/
char hostname[MAXDEVICE][256];
/** networkclient detected ports in config file.*/
int port[MAXDEVICE];
/** networkclient networks id.*/
int device_network_id[MAXDEVICE];
/** networkclient sockets for all devices.*/
int device_socket[MAXDEVICE];

/* to correct relative coordenates */
/** networkclient correcting factor to x variable.*/
float correcting_x=0.;
/** networkclient correcting factor to y variable.*/
float correcting_y=0.;
/** networkclient correcting factor to theta variable.*/
float correcting_theta=0.;

/** networkclient devices detected in config file.*/ 
int serve_device[MAXDEVICE];
/** networkclient devices active at each moment.*/
int device_active[MAXDEVICE];
/** networkclient variable to detect when the pthreads must end its execution.*/
int networkclient_close_command=0;

/** id for colorA schema.*/
int colorA_schema_id;
/** id for colorB schema.*/
int colorB_schema_id;
/** id for colorC schema.*/
int colorC_schema_id;
/** id for colorD schema.*/
int colorD_schema_id;
/** id for varcolorA schema.*/
int varcolorA_schema_id;
/** id for varcolorB schema.*/
int varcolorB_schema_id;
/** id for varcolorC schema.*/
int varcolorC_schema_id;
/** id for varcolorD schema.*/
int varcolorD_schema_id;
/** id for laser schema.*/
int laser_schema_id;
/** id for encoders schema.*/
int encoders_schema_id;
/** id for sonars schema.*/
int sonars_schema_id;
/** id for motors schema.*/
int motors_schema_id;
/** id for pantilt motors schema.*/
int ptmotors_schema_id;
/** id for pantilt encoders schema.*/
int ptencoders_schema_id;

/*API Variables servidas*/
/** 'ptmotors' schema longitude control*/
float longitude; /* degs, pan angle */
/** 'ptmotors' schema latitude control*/
float latitude; /* degs, tilt angle */
/** 'ptmotors' schema longitude speed control*/
float longitude_speed;
/** 'ptmotors' schema latitude speed control*/
float latitude_speed;
/** 'ptmotors' schema cycle control variable*/
int pantiltmotors_cycle;

/** 'ptencoders' schema pan angle information*/
float pan_angle;   /* degs */
/** 'ptencoders' schema tilt angle information*/
float tilt_angle;  /* degs */
/** 'ptencoders' schema clock*/
unsigned long int pantiltencoders_clock;

/** 'colorA' schema image data*/
char *colorA=NULL; /* sifntsc image itself */
/** 'colorA' schema clock*/
unsigned long int imageA_clock;

/** 'colorB' schema image data*/
char *colorB=NULL; /* sifntsc image itself */
/** 'colorB' schema clock*/
unsigned long int imageB_clock;

/** 'colorC' schema image data*/
char *colorC=NULL; /* sifntsc image itself */
/** 'colorC' schema clock*/
unsigned long int imageC_clock;

/** 'colorD' schema image data*/
char *colorD=NULL; /* sifntsc image itself */
/** 'colorD' schema clock*/
unsigned long int imageD_clock;

/** 'varcolorA' schema image data*/
char *varcolorA=NULL; /* sifntsc image itself */
/** 'varcolorA' schema clock*/
unsigned long int varimageA_clock;

/** 'varcolorB' schema image data*/
char *varcolorB=NULL; /* sifntsc image itself */
/** 'varcolorB' schema clock*/
unsigned long int varimageB_clock;

/** 'varcolorC' schema image data*/
char *varcolorC=NULL; /* sifntsc image itself */
/** 'varcolorC' schema clock*/
unsigned long int varimageC_clock;

/** 'varcolorD' schema image data*/
char *varcolorD=NULL; /* sifntsc image itself */
/** 'varcolorD' schema clock*/
unsigned long int varimageD_clock;

/** width of each served video**/
int width[MAXCAM];
/** height of each served video**/
int height[MAXCAM];

/** 'encoders' schema, odometry information.*/
float jde_robot[5];
/** 'encoders' schema, clock variable.*/
unsigned long int encoders_clock;
/** 'encoders' schema variable positions*/
int encoders_number=5;

/** 'laser' schema, laser information.*/
int jde_laser[LASER_MAX];
/** 'laser' schema, clock variable.*/
unsigned long int laser_clock;
/** Number of laser samples*/
int laser_number=0;

/** 'sonars' schema, sonars information.*/
float us[SONAR_MAX];
/** 'sonars' schema, clock variable.*/
unsigned long int us_clock[SONAR_MAX];
/** Number of sonar samples*/
int sonar_number=0;

/** 'motors' schema, speed control.*/
float v; /* mm/s */
/** 'motors' schema, turn speed control.*/
float w; /* deg/s*/
/** 'motors' schema, cycle control.*/
int motors_cycle;


/* Contadores de referencias*/
/** laser ref counter*/
int laser_refs=0;
/** encoders ref counter*/
int encoders_refs=0;
/** sonars ref counter*/
int sonars_refs=0;
/** motors ref counter*/
int motors_refs=0;
/** colorA ref counter*/
int colorA_refs=0;
/** colorB ref counter*/
int colorB_refs=0;
/** colorC ref counter*/
int colorC_refs=0;
/** colorD ref counter*/
int colorD_refs=0;
/** varcolorA ref counter*/
int varcolorA_refs=0;
/** varcolorB ref counter*/
int varcolorB_refs=0;
/** varcolorC ref counter*/
int varcolorC_refs=0;
/** varcolorD ref counter*/
int varcolorD_refs=0;
/** ptmotors ref counter*/
int ptmotors_refs=0;
/** ptencoders ref counter*/
int ptencoders_refs=0;

/** mutex for ref counters*/
pthread_mutex_t refmutex;

/** networkclient function to end execution of the driver, closing file descriptors and stopping devices.*/
void networkclient_close(){

  char last_message[MAX_MESSAGE];

  /* this will stop all the client threads */
  networkclient_close_command=1;

  /* closing all the device sockets */
  if(serve_device[COLORA_DEVICE]){
    sprintf(last_message,"%d\n",NETWORKSERVER_goodbye_images); 
    write(device_socket[COLORA_DEVICE],last_message,strlen(last_message));
    close(device_socket[COLORA_DEVICE]); 

    printf("networkclient: closed connection with networkserver for colorA at %s:%d\n",hostname[COLORA_DEVICE],port[COLORA_DEVICE]);
  }

  if(serve_device[COLORB_DEVICE]){
    sprintf(last_message,"%d\n",NETWORKSERVER_goodbye_images); 
    write(device_socket[COLORB_DEVICE],last_message,strlen(last_message));
    close(device_socket[COLORB_DEVICE]); 

    printf("networkclient: closed connection with networkserver for colorB at %s:%d\n",hostname[COLORB_DEVICE],port[COLORB_DEVICE]);
  }

  if(serve_device[COLORC_DEVICE]){
    sprintf(last_message,"%d\n",NETWORKSERVER_goodbye_images); 
    write(device_socket[COLORC_DEVICE],last_message,strlen(last_message));
    close(device_socket[COLORC_DEVICE]); 

    printf("networkclient: closed connection with networkserver for colorC at %s:%d\n",hostname[COLORC_DEVICE],port[COLORC_DEVICE]);
  }

  if(serve_device[COLORD_DEVICE]){
    sprintf(last_message,"%d\n",NETWORKSERVER_goodbye_images); 
    write(device_socket[COLORD_DEVICE],last_message,strlen(last_message));
    close(device_socket[COLORD_DEVICE]); 

    printf("networkclient: closed connection with networkserver for colorD at %s:%d\n",hostname[COLORD_DEVICE],port[COLORD_DEVICE]);
  }

  if(serve_device[PANTILT_MOTORS_DEVICE]){
    /* pthread_mutex_lock(&mymutex[PANTILT_MOTORS_DEVICE]);*/
    sprintf(last_message,"%ld %1.1f %1.1f\n",(long int)NETWORKSERVER_pantilt_position,0.,0.);
    write(device_socket[PANTILT_MOTORS_DEVICE],last_message,strlen(last_message));
    /* pthread_mutex_unlock(&mymutex[PANTILT_MOTORS_DEVICE]);*/

    printf("networkclient: closed connection with networkserver for pantiltmotors at %s:%d\n",hostname[PANTILT_MOTORS_DEVICE],port[PANTILT_MOTORS_DEVICE]);
  }

  if(serve_device[PANTILT_ENCODERS_DEVICE]){
    sprintf(last_message,"%d\n",NETWORKSERVER_goodbye_images); 
    write(device_socket[PANTILT_ENCODERS_DEVICE],last_message,strlen(last_message));
    close(device_socket[PANTILT_ENCODERS_DEVICE]);

    printf("networkclient: closed connection with networkserver for pantiltencoders at %s:%d\n",hostname[PANTILT_ENCODERS_DEVICE],port[PANTILT_ENCODERS_DEVICE]);
  }

  if(serve_device[MOTORS_DEVICE]){
    /*pthread_mutex_lock(&mymutex[MOTORS_DEVICE]);*/
      sprintf(last_message,"%d %1.1f %1.1f\n",NETWORKSERVER_drive_speed,0.,0.);
      write(device_socket[MOTORS_DEVICE],last_message,strlen(last_message));
      sprintf(last_message,"%d %1.1f %1.1f\n",NETWORKSERVER_steer_speed,0.,0.);
      write(device_socket[MOTORS_DEVICE],last_message,strlen(last_message));
      sprintf(last_message,"%d\n",NETWORKSERVER_goodbye); 
      write(device_socket[MOTORS_DEVICE],last_message,strlen(last_message));
      close(device_socket[MOTORS_DEVICE]); 
      /*pthread_mutex_unlock(&mymutex[MOTORS_DEVICE]);*/
      
      printf("networkclient: closed connection with networkserver for motors at %s:%d\n",hostname[MOTORS_DEVICE],port[MOTORS_DEVICE]);
  }

  if(serve_device[ENCODERS_DEVICE]){
    sprintf(last_message,"%d\n",NETWORKSERVER_goodbye); 
    write(device_socket[ENCODERS_DEVICE],last_message,strlen(last_message));
    close(device_socket[ENCODERS_DEVICE]); 

    printf("networkclient: closed connection with networkserver for encoders at %s:%d\n",hostname[ENCODERS_DEVICE],port[ENCODERS_DEVICE]);
  }

  if(serve_device[SONARS_DEVICE]){
    sprintf(last_message,"%d\n",NETWORKSERVER_goodbye); 
    write(device_socket[SONARS_DEVICE],last_message,strlen(last_message));
    close(device_socket[SONARS_DEVICE]); 

    printf("networkclient: closed connection with networkserver for sonars at %s:%d\n",hostname[SONARS_DEVICE],port[SONARS_DEVICE]);
  }

  if(serve_device[LASER_DEVICE]){
    sprintf(last_message,"%d\n",NETWORKSERVER_goodbye); 
    write(device_socket[LASER_DEVICE],last_message,strlen(last_message));
    close(device_socket[LASER_DEVICE]); 

    printf("networkclient: closed connection with networkserver for laser at %s:%d\n",hostname[LASER_DEVICE],port[LASER_DEVICE]);
  }

  printf("driver networkclient off\n");
}

/** pantiltencoders resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int networkclient_pantiltencoders_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (ptencoders_refs>0){
      ptencoders_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      ptencoders_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((device_active[PANTILT_ENCODERS_DEVICE==0])&&(serve_device[PANTILT_ENCODERS_DEVICE])){
         char message_out[MAX_MESSAGE];
      
         printf("ptencoders schema resume (networkclient driver)\n");
         all[ptencoders_schema_id].father = father;
         all[ptencoders_schema_id].fps = 0.;
         all[ptencoders_schema_id].k =0;
         put_state(ptencoders_schema_id,winner);

         device_active[PANTILT_ENCODERS_DEVICE]=1;
         pthread_mutex_lock(&mymutex[PANTILT_ENCODERS_DEVICE]);
         sprintf(message_out,"%d\n",NETWORKSERVER_subscribe_pantilt_encoders);
         write(device_socket[PANTILT_ENCODERS_DEVICE],message_out,strlen(message_out));
         state[PANTILT_ENCODERS_DEVICE]=winner;
         pthread_cond_signal(&condition[PANTILT_ENCODERS_DEVICE]);
         pthread_mutex_unlock(&mymutex[PANTILT_ENCODERS_DEVICE]);
      }
   }
   return 0;
}

/** pantiltencoders suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int networkclient_pantiltencoders_suspend(){
   pthread_mutex_lock(&refmutex);
   if (ptencoders_refs>1){
      ptencoders_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      ptencoders_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((device_active[PANTILT_ENCODERS_DEVICE])&&(serve_device[PANTILT_ENCODERS_DEVICE])){
         char message_out[MAX_MESSAGE];

         printf("ptencoders schema suspend (networkclient driver)\n");
         device_active[PANTILT_ENCODERS_DEVICE]=0;
         pthread_mutex_lock(&mymutex[PANTILT_ENCODERS_DEVICE]);
         sprintf(message_out,"%d\n",NETWORKSERVER_unsubscribe_pantilt_encoders);
         write(device_socket[PANTILT_ENCODERS_DEVICE],message_out,strlen(message_out));
         put_state(ptencoders_schema_id,slept);
         state[PANTILT_ENCODERS_DEVICE]=slept;
         pthread_mutex_unlock(&mymutex[PANTILT_ENCODERS_DEVICE]);
      }
   }
   return 0;
}

/** pantiltmotors resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int networkclient_pantiltmotors_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (ptmotors_refs>0){
      ptmotors_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      ptmotors_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((device_active[PANTILT_MOTORS_DEVICE==0])&&(serve_device[PANTILT_MOTORS_DEVICE])){
         printf("ptmotors schema resume (networkclient driver)\n");
         all[ptmotors_schema_id].father = father;
         all[ptmotors_schema_id].fps = 0.;
         all[ptmotors_schema_id].k =0;
         put_state(ptmotors_schema_id,winner);
         device_active[PANTILT_MOTORS_DEVICE]=1;
         pthread_mutex_lock(&mymutex[PANTILT_MOTORS_DEVICE]);
         state[PANTILT_MOTORS_DEVICE]=winner;
         pthread_cond_signal(&condition[PANTILT_MOTORS_DEVICE]);
         pthread_mutex_unlock(&mymutex[PANTILT_MOTORS_DEVICE]);
      }
   }
   return 0;
}

/** pantiltmotors suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int networkclient_pantiltmotors_suspend(){
   pthread_mutex_lock(&refmutex);
   if (ptmotors_refs>1){
      ptmotors_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      ptmotors_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((device_active[PANTILT_MOTORS_DEVICE])&&(serve_device[PANTILT_MOTORS_DEVICE])){
         printf("ptmotors schema suspend (networkclient driver)\n");
         device_active[PANTILT_MOTORS_DEVICE]=0;
         pthread_mutex_lock(&mymutex[PANTILT_MOTORS_DEVICE]);
         put_state(ptmotors_schema_id,slept);
         state[PANTILT_MOTORS_DEVICE]=slept;
         pthread_mutex_unlock(&mymutex[PANTILT_MOTORS_DEVICE]);
      }
   }
   return 0;
}

/** laser resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int networkclient_laser_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (laser_refs>0){
      laser_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      laser_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((device_active[LASER_DEVICE]==0)&&(serve_device[LASER_DEVICE])){
         char message_out[MAX_MESSAGE];

         printf("laser schema resume (networkclient driver)\n");
         all[laser_schema_id].father = father;
         all[laser_schema_id].fps = 0.;
         all[laser_schema_id].k = 0;
         put_state(laser_schema_id,winner);
         device_active[LASER_DEVICE]=1;
         pthread_mutex_lock(&mymutex[LASER_DEVICE]);
         sprintf(message_out,"%d\n",NETWORKSERVER_subscribe_laser);
         write(device_socket[LASER_DEVICE],message_out,strlen(message_out));
         state[LASER_DEVICE]=winner;
         pthread_cond_signal(&condition[LASER_DEVICE]);
         pthread_mutex_unlock(&mymutex[LASER_DEVICE]);
      }
   }
   return 0;
}

/** laser suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int networkclient_laser_suspend(){
   pthread_mutex_lock(&refmutex);
   if (laser_refs>1){
      laser_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      laser_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((device_active[LASER_DEVICE])&&(serve_device[LASER_DEVICE])){
         char message_out[MAX_MESSAGE];

         printf("laser schema suspend (networking driver)\n");
         device_active[LASER_DEVICE]=0;
         pthread_mutex_lock(&mymutex[LASER_DEVICE]);
         sprintf(message_out,"%d\n",NETWORKSERVER_unsubscribe_laser);
         write(device_socket[LASER_DEVICE],message_out,strlen(message_out));
         state[LASER_DEVICE]=slept;
         put_state(laser_schema_id,slept);
         pthread_mutex_unlock(&mymutex[LASER_DEVICE]);
      }
   }
   return 0;
}

/** encoders resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int networkclient_encoders_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (encoders_refs>0){
      encoders_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      encoders_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((device_active[ENCODERS_DEVICE]==0)&&(serve_device[ENCODERS_DEVICE])){
         char message_out[MAX_MESSAGE];

         printf("encoders schema resume (networking driver)\n");
         all[encoders_schema_id].father = father;
         all[encoders_schema_id].fps = 0.;
         all[encoders_schema_id].k =0;
         put_state(encoders_schema_id,winner);
         device_active[ENCODERS_DEVICE]=1;
         pthread_mutex_lock(&mymutex[ENCODERS_DEVICE]);
         sprintf(message_out,"%d\n", NETWORKSERVER_subscribe_encoders);
         write(device_socket[ENCODERS_DEVICE],message_out,strlen(message_out));
         state[ENCODERS_DEVICE]=winner;
         pthread_cond_signal(&condition[ENCODERS_DEVICE]);
         pthread_mutex_unlock(&mymutex[ENCODERS_DEVICE]);
      }
   }
   return 0;
}

/** encoders suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int networkclient_encoders_suspend(){
   pthread_mutex_lock(&refmutex);
   if (encoders_refs>1){
      encoders_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      encoders_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((device_active[ENCODERS_DEVICE])&&(serve_device[ENCODERS_DEVICE])){
         char message_out[MAX_MESSAGE];

         printf("laser schema suspend (networkclient driver)\n");
         device_active[ENCODERS_DEVICE]=0;
         pthread_mutex_lock(&mymutex[ENCODERS_DEVICE]);
         sprintf(message_out,"%d\n", NETWORKSERVER_unsubscribe_encoders);
         write(device_socket[ENCODERS_DEVICE],message_out,strlen(message_out));
         state[ENCODERS_DEVICE]=slept;
         put_state(encoders_schema_id,slept);
         pthread_mutex_unlock(&mymutex[ENCODERS_DEVICE]);
      }
   }
   return 0;
}

/** sonars resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int networkclient_sonars_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (sonars_refs>0){
      sonars_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      sonars_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((device_active[SONARS_DEVICE]==0)&&(serve_device[SONARS_DEVICE])){
         char message_out[MAX_MESSAGE];

         printf("sonars schema resume (networkclient driver)\n");
         all[sonars_schema_id].father = father;
         all[sonars_schema_id].fps = 0.;
         all[sonars_schema_id].k =0;
         put_state(sonars_schema_id,winner);
         device_active[SONARS_DEVICE]=1;
         pthread_mutex_lock(&mymutex[SONARS_DEVICE]);
         sprintf(message_out,"%d\n",NETWORKSERVER_subscribe_us);
         write(device_socket[SONARS_DEVICE],message_out,strlen(message_out));
         state[SONARS_DEVICE]=winner;
         pthread_cond_signal(&condition[SONARS_DEVICE]);
         pthread_mutex_unlock(&mymutex[SONARS_DEVICE]);
      }
   }
   return 0;
}

/** sonars suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int networkclient_sonars_suspend(){
   pthread_mutex_lock(&refmutex);
   if (sonars_refs>1){
      sonars_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      sonars_refs=0;
      pthread_mutex_unlock(&refmutex);
            if((device_active[SONARS_DEVICE])&&(serve_device[SONARS_DEVICE])){
         char message_out[MAX_MESSAGE];

         printf("sonars schema suspend (networkclient driver)\n");
         device_active[SONARS_DEVICE]=0;
         pthread_mutex_lock(&mymutex[SONARS_DEVICE]);
         sprintf(message_out,"%d\n",NETWORKSERVER_unsubscribe_us);
         write(device_socket[SONARS_DEVICE],message_out,strlen(message_out));
         state[SONARS_DEVICE]=slept;
         put_state(sonars_schema_id,slept);
         pthread_mutex_unlock(&mymutex[SONARS_DEVICE]);
            }
   }
   return 0;
}

/** motors resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int networkclient_motors_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (motors_refs>0){
      motors_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      motors_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((device_active[MOTORS_DEVICE]==0)&&(serve_device[MOTORS_DEVICE])){
         printf("motors schema resume (networkclient driver)\n");
         all[motors_schema_id].father = father;
         all[motors_schema_id].fps = 0.;
         all[motors_schema_id].k =0;
         put_state(motors_schema_id,winner);
         device_active[MOTORS_DEVICE]=1;
         pthread_mutex_lock(&mymutex[MOTORS_DEVICE]);
         state[MOTORS_DEVICE]=winner;
         pthread_cond_signal(&condition[MOTORS_DEVICE]);
         pthread_mutex_unlock(&mymutex[MOTORS_DEVICE]);
      }
   }
   return 0;
}

/** motors suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int networkclient_motors_suspend(){
   pthread_mutex_lock(&refmutex);
   if (motors_refs>1){
      motors_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      motors_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((device_active[MOTORS_DEVICE])&&(serve_device[MOTORS_DEVICE])){
         printf("motors schema suspend (networkclient driver)\n");
         device_active[MOTORS_DEVICE]=0;
         pthread_mutex_lock(&mymutex[MOTORS_DEVICE]);
         state[MOTORS_DEVICE]=slept;
         put_state(motors_schema_id,slept);
         pthread_mutex_unlock(&mymutex[MOTORS_DEVICE]);
      }
   }
   return 0;
}

/** colorA resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int networkclient_colorA_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (colorA_refs>0){
      colorA_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorA_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((device_active[COLORA_DEVICE]==0)&&(serve_device[COLORA_DEVICE])){
         printf("colorA schema resume (networkclient driver)\n");
         all[colorA_schema_id].father = father;
         all[colorA_schema_id].fps = 0.;
         all[colorA_schema_id].k =0;
         put_state(colorA_schema_id,winner);
         device_active[COLORA_DEVICE]=1;
         pthread_mutex_lock(&mymutex[COLORA_DEVICE]);
         state[COLORA_DEVICE]=winner;
         pthread_cond_signal(&condition[COLORA_DEVICE]);
         pthread_mutex_unlock(&mymutex[COLORA_DEVICE]);
      }
   }
   return 0;
}

/** colorA suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int networkclient_colorA_suspend(){
   pthread_mutex_lock(&refmutex);
   if (colorA_refs>1){
      colorA_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorA_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((device_active[COLORA_DEVICE])&&(serve_device[COLORA_DEVICE])){
         printf("colorA schema suspend (networkclient driver)\n");
         device_active[COLORA_DEVICE]=0;
         pthread_mutex_lock(&mymutex[COLORA_DEVICE]);
         state[COLORA_DEVICE]=slept;
         put_state(colorA_schema_id,slept);
         pthread_mutex_unlock(&mymutex[COLORA_DEVICE]);
      }
   }
   return 0;
}

/** colorB resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int networkclient_colorB_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (colorB_refs>0){
      colorB_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorB_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((device_active[COLORB_DEVICE]==0)&&(serve_device[COLORB_DEVICE])){
         printf("colorB schema resume (networkclient driver)\n");
         all[colorB_schema_id].father = father;
         all[colorB_schema_id].fps = 0.;
         all[colorB_schema_id].k =0;
         put_state(colorB_schema_id,winner);
         device_active[COLORB_DEVICE]=1;
         pthread_mutex_lock(&mymutex[COLORB_DEVICE]);
         state[COLORB_DEVICE]=winner;
         pthread_cond_signal(&condition[COLORB_DEVICE]);
         pthread_mutex_unlock(&mymutex[COLORB_DEVICE]);
      }
   }
   return 0;
}

/** colorB suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int networkclient_colorB_suspend(){
   pthread_mutex_lock(&refmutex);
   if (colorB_refs>1){
      colorB_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorB_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((device_active[COLORB_DEVICE])&&(serve_device[COLORB_DEVICE])){
         printf("colorB schema suspend (networkclient driver)\n");
         device_active[COLORB_DEVICE]=0;
         pthread_mutex_lock(&mymutex[COLORB_DEVICE]);
         state[COLORB_DEVICE]=slept;
         put_state(colorB_schema_id,slept);
         pthread_mutex_unlock(&mymutex[COLORB_DEVICE]);
      }
   }
   return 0;
}

/** colorC resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int networkclient_colorC_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (colorC_refs>0){
      colorC_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorC_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((device_active[COLORC_DEVICE]==0)&&(serve_device[COLORC_DEVICE])){
         printf("colorC schema resume (networkclient driver)\n");
         all[colorC_schema_id].father = father;
         all[colorC_schema_id].fps = 0.;
         all[colorC_schema_id].k =0;
         put_state(colorC_schema_id,winner);
         device_active[COLORC_DEVICE]=1;
         pthread_mutex_lock(&mymutex[COLORC_DEVICE]);
         state[COLORC_DEVICE]=winner;
         pthread_cond_signal(&condition[COLORC_DEVICE]);
         pthread_mutex_unlock(&mymutex[COLORC_DEVICE]);
      }
   }
   return 0;
}

/** colorC suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int networkclient_colorC_suspend(){
   pthread_mutex_lock(&refmutex);
   if (colorC_refs>1){
      colorC_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorC_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((device_active[COLORC_DEVICE])&&(serve_device[COLORC_DEVICE])){
         printf("colorC schema suspend (networkclient driver)\n");
         device_active[COLORC_DEVICE]=0;
         pthread_mutex_lock(&mymutex[COLORC_DEVICE]);
         state[COLORC_DEVICE]=slept;
         put_state(colorC_schema_id,slept);
         pthread_mutex_unlock(&mymutex[COLORC_DEVICE]);
      }
   }
   return 0;
}

/** colorD resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int networkclient_colorD_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (colorD_refs>0){
      colorD_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorD_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((device_active[COLORD_DEVICE]==0)&&(serve_device[COLORD_DEVICE])){
         printf("colorD schema resume (networkclient driver)\n");
         all[colorD_schema_id].father = father;
         all[colorD_schema_id].fps = 0.;
         all[colorD_schema_id].k =0;
         put_state(colorD_schema_id,winner);
         device_active[COLORD_DEVICE]=1;
         pthread_mutex_lock(&mymutex[COLORD_DEVICE]);
         state[COLORD_DEVICE]=winner;
         pthread_cond_signal(&condition[COLORD_DEVICE]);
         pthread_mutex_unlock(&mymutex[COLORD_DEVICE]);
      }
   }
   return 0;
}

/** colorD suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int networkclient_colorD_suspend(){
   pthread_mutex_lock(&refmutex);
   if (colorD_refs>1){
      colorD_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorD_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((device_active[COLORD_DEVICE])&&(serve_device[COLORD_DEVICE])){
         printf("colorD schema suspend (networkclient driver)\n");
         device_active[COLORD_DEVICE]=0;
         pthread_mutex_lock(&mymutex[COLORD_DEVICE]);
         state[COLORD_DEVICE]=slept;
         put_state(colorD_schema_id,slept);
         pthread_mutex_unlock(&mymutex[COLORD_DEVICE]);
      }
   }
   return 0;
}

/** varcolorA resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int networkclient_varcolorA_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (varcolorA_refs>0){
      varcolorA_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorA_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((device_active[VARCOLORA_DEVICE]==0)&&(serve_device[VARCOLORA_DEVICE])){
         printf("varcolorA schema resume (networkclient driver)\n");
         all[varcolorA_schema_id].father = father;
         all[varcolorA_schema_id].fps = 0.;
         all[varcolorA_schema_id].k =0;
         put_state(varcolorA_schema_id,winner);
         device_active[VARCOLORA_DEVICE]=1;
         pthread_mutex_lock(&mymutex[VARCOLORA_DEVICE]);
         state[VARCOLORA_DEVICE]=winner;
         pthread_cond_signal(&condition[VARCOLORA_DEVICE]);
         pthread_mutex_unlock(&mymutex[VARCOLORA_DEVICE]);
      }
   }
   return 0;
}

/** varcolorA suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int networkclient_varcolorA_suspend(){
   pthread_mutex_lock(&refmutex);
   if (varcolorA_refs>1){
      varcolorA_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorA_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((device_active[VARCOLORA_DEVICE])&&(serve_device[VARCOLORA_DEVICE])){
         printf("varcolorA schema suspend (networkclient driver)\n");
         device_active[VARCOLORA_DEVICE]=0;
         pthread_mutex_lock(&mymutex[VARCOLORA_DEVICE]);
         state[VARCOLORA_DEVICE]=slept;
         put_state(varcolorA_schema_id,slept);
         pthread_mutex_unlock(&mymutex[VARCOLORA_DEVICE]);
      }
   }
   return 0;
}

/** varcolorB resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int networkclient_varcolorB_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (varcolorB_refs>0){
      varcolorB_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorB_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((device_active[VARCOLORB_DEVICE]==0)&&(serve_device[VARCOLORB_DEVICE])){
         printf("varcolorB schema resume (networkclient driver)\n");
         all[varcolorB_schema_id].father = father;
         all[varcolorB_schema_id].fps = 0.;
         all[varcolorB_schema_id].k =0;
         put_state(varcolorB_schema_id,winner);
         device_active[VARCOLORB_DEVICE]=1;
         pthread_mutex_lock(&mymutex[VARCOLORB_DEVICE]);
         state[VARCOLORB_DEVICE]=winner;
         pthread_cond_signal(&condition[VARCOLORB_DEVICE]);
         pthread_mutex_unlock(&mymutex[VARCOLORB_DEVICE]);
      }
   }
   return 0;
}

/** varcolorB suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int networkclient_varcolorB_suspend(){
   pthread_mutex_lock(&refmutex);
   if (varcolorB_refs>1){
      varcolorB_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorB_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((device_active[VARCOLORB_DEVICE])&&(serve_device[VARCOLORB_DEVICE])){
         printf("varcolorB schema suspend (networkclient driver)\n");
         device_active[VARCOLORB_DEVICE]=0;
         pthread_mutex_lock(&mymutex[VARCOLORB_DEVICE]);
         state[VARCOLORB_DEVICE]=slept;
         put_state(varcolorB_schema_id,slept);
         pthread_mutex_unlock(&mymutex[VARCOLORB_DEVICE]);
      }
   }
   return 0;
}

/** varcolorC resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int networkclient_varcolorC_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (varcolorC_refs>0){
      varcolorC_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorC_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((device_active[VARCOLORC_DEVICE]==0)&&(serve_device[VARCOLORC_DEVICE])){
         printf("varcolorC schema resume (networkclient driver)\n");
         all[varcolorC_schema_id].father = father;
         all[varcolorC_schema_id].fps = 0.;
         all[varcolorC_schema_id].k =0;
         put_state(varcolorC_schema_id,winner);
         device_active[VARCOLORC_DEVICE]=1;
         pthread_mutex_lock(&mymutex[VARCOLORC_DEVICE]);
         state[VARCOLORC_DEVICE]=winner;
         pthread_cond_signal(&condition[VARCOLORC_DEVICE]);
         pthread_mutex_unlock(&mymutex[VARCOLORC_DEVICE]);
      }
   }
   return 0;
}

/** varcolorC suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int networkclient_varcolorC_suspend(){
   pthread_mutex_lock(&refmutex);
   if (varcolorC_refs>1){
      varcolorC_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorC_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((device_active[VARCOLORC_DEVICE])&&(serve_device[VARCOLORC_DEVICE])){
         printf("varcolorC schema suspend (networkclient driver)\n");
         device_active[VARCOLORC_DEVICE]=0;
         pthread_mutex_lock(&mymutex[VARCOLORC_DEVICE]);
         state[VARCOLORC_DEVICE]=slept;
         put_state(varcolorC_schema_id,slept);
         pthread_mutex_unlock(&mymutex[VARCOLORC_DEVICE]);
      }
   }
   return 0;
}

/** varcolorD resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int networkclient_varcolorD_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (varcolorD_refs>0){
      varcolorD_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorD_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((device_active[VARCOLORD_DEVICE]==0)&&(serve_device[VARCOLORD_DEVICE])){
         printf("varcolorD schema resume (networkclient driver)\n");
         all[varcolorD_schema_id].father = father;
         all[varcolorD_schema_id].fps = 0.;
         all[varcolorD_schema_id].k =0;
         put_state(varcolorD_schema_id,winner);
         device_active[VARCOLORD_DEVICE]=1;
         pthread_mutex_lock(&mymutex[VARCOLORD_DEVICE]);
         state[VARCOLORD_DEVICE]=winner;
         pthread_cond_signal(&condition[VARCOLORD_DEVICE]);
         pthread_mutex_unlock(&mymutex[VARCOLORD_DEVICE]);
      }
   }
   return 0;
}

/** varcolorD suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int networkclient_varcolorD_suspend(){
   pthread_mutex_lock(&refmutex);
   if (varcolorD_refs>1){
      varcolorD_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorD_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((device_active[VARCOLORD_DEVICE])&&(serve_device[VARCOLORD_DEVICE])){
         printf("varcolorD schema suspend (networkclient driver)\n");
         device_active[VARCOLORD_DEVICE]=0;
         pthread_mutex_lock(&mymutex[VARCOLORD_DEVICE]);
         state[VARCOLORD_DEVICE]=slept;
         put_state(varcolorD_schema_id,slept);
         pthread_mutex_unlock(&mymutex[VARCOLORD_DEVICE]);
      }
   }
   return 0;
}

/** networkclient driver pthread function for pantiltencoders reception.*/
void *networkclient_pantiltencoders_thread(void *not_used){
  int j=0;
  long int readn=0,bytes_readn=0,start=0;
  int pantiltencoders_trac;
  char message_in[MAX_MESSAGE];
  char lastmessage[MAX_MESSAGE];

  printf("networkclient pantiltencoders thread started\n");

  do{

    pthread_mutex_lock(&mymutex[PANTILT_ENCODERS_DEVICE]);

    if (state[PANTILT_ENCODERS_DEVICE]==slept){
      printf("networkclient pantiltencoders thread goes sleep mode\n");
      pthread_cond_wait(&condition[PANTILT_ENCODERS_DEVICE],&mymutex[PANTILT_ENCODERS_DEVICE]);
      printf("networkclient pantiltencoders thread woke up\n");
      pthread_mutex_unlock(&mymutex[PANTILT_ENCODERS_DEVICE]);
    }else{
      
      pthread_mutex_unlock(&mymutex[PANTILT_ENCODERS_DEVICE]);
      
      /* reading from socket */
      readn=read(device_socket[PANTILT_ENCODERS_DEVICE],&(message_in[pantiltencoders_trac]),MAX_MESSAGE-pantiltencoders_trac);
      bytes_readn=readn;

      switch(readn){
      case 0: exit(1); break;
      case -1: break; /* nothing to read */
      default:

	start=0;
	for(j=pantiltencoders_trac;(j<MAX_MESSAGE)&&(j<pantiltencoders_trac+bytes_readn);j++){
	  if (message_in[j]=='\n'){
	    int type;
	    unsigned long int network_clock;
		
	    strncpy(lastmessage,&message_in[start],j-start);
	    lastmessage[j-start]='\0';
	    start=j+1;

	    if (sscanf(lastmessage,"%d %lu",&type,&network_clock)!=2) {
	      printf("networkclient: i don't understand message from networkserver to pantiltencoders: (%s)\n",lastmessage);
	    }else{

	      int k=0;
	      while((lastmessage[k]!='\n')&&(lastmessage[k]!=' ')&&(lastmessage[k]!='\0')) k++;
	      k++;
	      while((lastmessage[k]!='\n')&&(lastmessage[k]!=' ')&&(lastmessage[k]!='\0')) k++;
	      if (type == NETWORKSERVER_pantilt_encoders) {
		sscanf(&lastmessage[++j],"%f %f\n",&pan_angle, &tilt_angle);
		pantiltencoders_clock=network_clock;
		speedcounter(ptencoders_schema_id);
	      }
	    }
	  }
	}
      }
    }
  }while(networkclient_close_command==0);
  pthread_exit(0);
}

/** networkclient driver pthread function for pantiltmotors reception.*/
void *networkclient_pantiltmotors_thread(void *not_used){

   char pantiltmotors_out[MAX_MESSAGE];
   
   struct timeval a,b;
   long diff, next;

   printf("networkclient pantiltmotors thread started\n");

   do{
      speedcounter(ptmotors_schema_id);
      pthread_mutex_lock(&mymutex[PANTILT_MOTORS_DEVICE]);

      if (state[PANTILT_MOTORS_DEVICE]==slept){
         printf("networkclient pantiltmotors thread goes sleep mode\n");
         pthread_cond_wait(&condition[PANTILT_MOTORS_DEVICE],&mymutex[PANTILT_MOTORS_DEVICE]);
         printf("networkclient pantiltmotors thread woke up\n");
         pthread_mutex_unlock(&mymutex[PANTILT_MOTORS_DEVICE]);
      }else{

         gettimeofday(&a,NULL);
         sprintf(pantiltmotors_out,"%ld %1.1f %1.1f %1.1f %1.1f\n", (long int)NETWORKSERVER_pantilt_position,longitude,latitude,longitude_speed,latitude_speed);
         write(device_socket[PANTILT_MOTORS_DEVICE],pantiltmotors_out,strlen(pantiltmotors_out));
         pthread_mutex_unlock(&mymutex[PANTILT_MOTORS_DEVICE]);
         gettimeofday(&b,NULL);

         diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
         next = motors_cycle*1000-diff-10000;
      
         if (next>0)
            usleep(pantiltmotors_cycle*1000-diff);
         else {
            usleep(pantiltmotors_cycle*1000);
         }
      }
   }while(networkclient_close_command==0);
   pthread_exit(0);
}

/** networkclient driver pthread function for laser reception.*/
void *networkclient_laser_thread(void *not_used){

  int readn, beginning,i;
  unsigned long int network_clock;
  int type,l;
  int j=0,sensor;

  /* insert pointer on the input_buffer, in order to complete messages with more than one receptions */
  int laser_trac;
  char laser_in[MAX_MESSAGE];
  char *message;

  printf("networkclient laser thread started\n");

  do{

    pthread_mutex_lock(&mymutex[LASER_DEVICE]);

    if (state[LASER_DEVICE]==slept){
      printf("networkclient laser thread goes sleep mode\n");
      pthread_cond_wait(&condition[LASER_DEVICE],&mymutex[LASER_DEVICE]);
      printf("networkclient laser thread woke up\n");
      pthread_mutex_unlock(&mymutex[LASER_DEVICE]);
    }else{
      
       pthread_mutex_unlock(&mymutex[LASER_DEVICE]);

       /* reading from socket. no overflow when using MAX_MESSAGE. messages are read in chunks. */
       readn=read(device_socket[LASER_DEVICE],&laser_in[laser_trac],MAX_MESSAGE);
       switch(readn){
          case 0:
             close(device_socket[LASER_DEVICE]);
             break; /* EOF: closing socket */
          case -1:
             break; /* nothing to read; */
          default:
             /* there is something in the socket. if received '\n' then a
             message has been completed. otherwise, we simply add the received
             chunk to the input_buffer and we will completed in further calls.
             if many message's were completed, we serve them all of them. we
             asume that the message in the buffer starts at the beginning of
             the buffer.*/
          {
             beginning=0;
             for(i=laser_trac; (i<MAX_MESSAGE)&&(i<laser_trac+readn);i++){
                if (laser_in[i]=='\n'){
                   message = &laser_in[beginning];

                   if (sscanf(message,"%d %lu",&type,&network_clock)!=2){
                      printf("networkclient: i don't understand message: (%s)\n",message);
		
                   }else{
                      j=0;
                      /* increments the 'j' pointer over the message till the first measure starts, jumping over the clock and type  */
                      while((message[j]!='\n')&&(message[j]!=' ')&&(message[j]!='\0'))
                         j++;
                      j++;
                      while((message[j]!='\n')&&(message[j]!=' ')&&(message[j]!='\0'))
                         j++;

                      if (type==NETWORKSERVER_laser){
                         speedcounter(laser_schema_id);
                         sensor=0;

                         while (message[j]==' '){/* reading lasers. it stops when buffer[j]=='\n' or '\0'*/
                            if (sscanf(&message[++j],"%d ",&l)!=1){
                               printf("networkclient: conversion failed in laser measure conversion\n");
                            }else{ /* adds the measure and increments the pointer 'j' over the message */
                               while((message[j]!='\n')&&(message[j]!=' ')&&(message[j]!='\0')) j++;
                               /*j++;*/
                               jde_laser[sensor++]=l;
                            }
                         }
                         laser_number=sensor;
                         laser_clock=network_clock;
                      }
                   }
                   beginning=i+1;
                }
             }
	  
	  if (beginning==0){ /* message didn't end in this reception */
	    if (laser_trac+readn >= MAX_MESSAGE-1)
               laser_trac=0;
	    /* the reception buffer has been filled with the chunks of the received messages, but no message has been entirely completed. the current message will be ignored. LOOK OUT! the next message will start incorrectly as it will have the rest of the incompleted message at the beginning, and will be ignored too because it can't be understood. that way, when overflow occurs, both messages will be lost.*/
	    else
               laser_trac+=readn;
	    
	  }else{
	    strncpy(laser_in,&(laser_in[beginning]),laser_trac+readn-beginning);
	    laser_trac = laser_trac+readn-beginning;
	    beginning=0; /* for the next chunk received */
	  }
	}
      }
    }
  }while(networkclient_close_command==0);
  pthread_exit(0);
}

/** networkclient driver pthread function for encoders reception.*/
void *networkclient_encoders_thread(void *not_used){

  int readn, beginning,i;
  unsigned long int network_clock;
  int type;
  unsigned long nowtime; /* microsecs */
  float nowx, nowy, nowtheta;
  float a1,a2,a3;
  float tspeed, rspeed; /* mm/s, deg/s */
  unsigned long lasttime; /* microsecs */

  /* insert pointer on the input_buffer, in order to complete messages with more than one receptions */
  int encoders_trac;
  char encoders_in[MAX_MESSAGE];
  char *message;

  printf("networkclient encoders thread started\n");

  do{

    pthread_mutex_lock(&mymutex[ENCODERS_DEVICE]);

    if (state[ENCODERS_DEVICE]==slept){
      printf("networkclient encoders thread goes sleep mode\n");
      pthread_cond_wait(&condition[ENCODERS_DEVICE],&mymutex[ENCODERS_DEVICE]);
      printf("networkclient encoders thread woke up\n");
      pthread_mutex_unlock(&mymutex[ENCODERS_DEVICE]);
    }else{
      
      pthread_mutex_unlock(&mymutex[ENCODERS_DEVICE]);

      /* reading from socket. no overflow when using MAX_MESSAGE. messages are read in chunks. */
      readn=read(device_socket[ENCODERS_DEVICE],&encoders_in[encoders_trac],MAX_MESSAGE);
      switch(readn){
      case 0: close(device_socket[ENCODERS_DEVICE]); break; /* EOF: closing socket */
      case -1: ; break; /* nothing to read; */
      default:
	/* there is something in the socket. if received '\n' then a message has
          been completed. otherwise, we simply add the received chunk to the
          input_buffer and we will completed in further calls. if many message's
          were completed, we serve them all of them. we asume that the message
          in the buffer starts at the beginning of the buffer.*/
	{
	  beginning=0;
	  for(i=encoders_trac; (i<MAX_MESSAGE)&&(i<encoders_trac+readn);i++){
	    if (encoders_in[i]=='\n'){
	      
	      message = &encoders_in[beginning];
              
	      if (sscanf(message,"%d %lu",&type,&network_clock)!=2){
		printf("networkclient: i don't understand message: (%s)\n",message);
		
	      }else{
		if (type==NETWORKSERVER_encoders) 
		  {
                     if (sscanf(message,"%d %lu %f %f %f %lu", &type,
                         &network_clock, &nowx, &nowy, &nowtheta, &nowtime)!=6)
                     {
		      printf("networkclient: conversion failed in encoders measure conversion\n");
		      /* WATCH OUT WITH LOCALE CONFIGURATION!. With bad settings it expects float numbers to be composed of an
			 integer part and a decimal part separated by a single comma (not a single dot). In such a case the sscanf
			 fails when processing real numbers as those involved in encoders data, because networkserver sends them
			 using the dot. */
		      
		      printf("networkclient: this is a float for me: %f\n",3.141592654);
		      
		    }else{
		      speedcounter(encoders_schema_id);
		      encoders_clock=network_clock;
		      if (nowtheta <=0) nowtheta=nowtheta+360; 
		      if (nowtime>lasttime){
			tspeed=(1000000*sqrt((nowx-jde_robot[0])*(nowx-jde_robot[0])+(nowy-jde_robot[1])*(nowy-jde_robot[1])))/(nowtime-lasttime);
			if (((nowx-jde_robot[0])*jde_robot[3]+(nowy-jde_robot[1])*jde_robot[4])<0) tspeed=-tspeed;

			/* square to avoid function calling and its delay. Division by DEGTOREAD introduces some little error */
			a1=(jde_robot[2]/DEGTORAD-nowtheta)*(jde_robot[2]/DEGTORAD-nowtheta);
			a2=(jde_robot[2]/DEGTORAD-nowtheta-360)*(jde_robot[2]/DEGTORAD-nowtheta-360);
			a3=(jde_robot[2]/DEGTORAD-nowtheta+360)*(jde_robot[2]/DEGTORAD-nowtheta+360);
			if ((a1<a2)&&(a1<a3))
			  rspeed=(1000000*(nowtheta-jde_robot[2]/DEGTORAD))/(nowtime-lasttime);
			else if ((a2<a1)&&(a2<a3))
			  rspeed=(1000000*(nowtheta-360-jde_robot[2]/DEGTORAD))/(nowtime-lasttime);
			else
			  rspeed=(1000000*(nowtheta+360-jde_robot[2]/DEGTORAD))/(nowtime-lasttime);
		      }
		    }
		    /* the rest of the line is read as the line is a complete line */
		    
		    lasttime=nowtime;
		    jde_robot[0] = nowx;
		    jde_robot[1] = nowy;
		    jde_robot[2] = nowtheta * DEGTORAD;
		    jde_robot[3] = cos(jde_robot[2]);
		    jde_robot[4] = sin(jde_robot[2]);
		    
		  }
	      }
	      beginning=i+1;
	    }
	  }
	  
	  if (beginning==0){ /* message didn't end in this reception */
	    if (encoders_trac+readn >= MAX_MESSAGE-1) encoders_trac=0;
	    /* the reception buffer has been filled with the chunks of the received messages, but no message has been entirely completed. the current message will be ignored. LOOK OUT! the next message will start incorrectly as it will have the rest of the incompleted message at the beginning, and will be ignored too because it can't be understood. that way, when overflow occurs, both messages will be lost.*/
	    else encoders_trac+=readn;
	    
	  }else{
	    strncpy(encoders_in,&(encoders_in[beginning]),encoders_trac+readn-beginning);
	    encoders_trac = encoders_trac+readn-beginning;
	    beginning=0; /* for the next chunk received */
	  }
	}
      }
    }
  }while(networkclient_close_command==0);
  pthread_exit(0);
}

/** networkclient driver pthread function for sonars reception.*/
void *networkclient_sonars_thread(void *not_used){

  int readn, beginning,i;
  unsigned long int network_clock;
  int type;
  int j=0,sensor=0;
  float measure;

  /* insert pointer on the input_buffer, in order to complete messages with more than one receptions */
  int sonars_trac;
  char sonars_in[MAX_MESSAGE];
  char *message;

  printf("networkclient sonars thread started\n");

  do{

     pthread_mutex_lock(&mymutex[SONARS_DEVICE]);

     if (state[SONARS_DEVICE]==slept){
        printf("networkclient sonars thread goes sleep mode\n");
        pthread_cond_wait(&condition[SONARS_DEVICE],&mymutex[SONARS_DEVICE]);
        printf("networkclient sonars thread woke up\n");
        pthread_mutex_unlock(&mymutex[SONARS_DEVICE]);
     }else{
      
        pthread_mutex_unlock(&mymutex[SONARS_DEVICE]);

        /* reading from socket. no overflow when using MAX_MESSAGE. messages are read in chunks. */
        readn=read(device_socket[SONARS_DEVICE],&sonars_in[sonars_trac],MAX_MESSAGE);
        switch(readn){
           case 0: close(device_socket[SONARS_DEVICE]); break; /* EOF: closing socket */
           case -1: ; break; /* nothing to read; */
           default:
              /* there is something in the socket. if received '\n' then a
              message has been completed. otherwise, we simply add the received
              chunk to the input_buffer and we will completed in further calls.
              if many message's were completed, we serve them all of them. we
              asume that the message in the buffer starts at the beginning of
              the buffer.*/
           {
              beginning=0;
              for(i=sonars_trac; (i<MAX_MESSAGE)&&(i<sonars_trac+readn);i++){
                 if (sonars_in[i]=='\n'){
	      
                    message = &sonars_in[beginning];

                    if (sscanf(message,"%d %lu",&type,&network_clock)!=2){
                       printf("networkclient: i don't understand message: (%s)\n",message);
		
                    }else{
                       j=0;
                       /* increments the 'j' pointer over the message till the first measure starts, jumping over the clock and type  */
                       while((message[j]!='\n')&&(message[j]!=' ')&&(message[j]!='\0')) j++;
                       j++;
                       while((message[j]!='\n')&&(message[j]!=' ')&&(message[j]!='\0')) j++;
		
                       if (type==NETWORKSERVER_sonars)
                       {
                          while (message[j]==' ') { /* reading sonars. it stops when buffer[j]=='\n' or '\0'*/
                             if (sscanf(&message[++j],"%d %f",&sensor, &measure)!=2){
                                printf("networkclient: conversion failed in sonars measure conversion\n");
			
                             }else { /* adds the measure and increments the pointer 'j' over the message */
                                while((message[j]!='\n')&&(message[j]!=' ')&&(message[j]!='\0')) j++;
                                j++;
                                while((message[j]!='\n')&&(message[j]!=' ')&&(message[j]!='\0')) j++;
                                us[sensor]=measure;
                                us_clock[sensor]=network_clock;
                             }
                          }
                          sonar_number=sensor;
                          speedcounter(sonars_schema_id);
                       }
                    }
                    beginning=i+1;
                 }
              }
	  
              if (beginning==0){ /* message didn't end in this reception */
                 if (sonars_trac+readn >= MAX_MESSAGE-1) sonars_trac=0;
                 /* the reception buffer has been filled with the chunks of the received messages, but no message has been entirely completed. the current message will be ignored. LOOK OUT! the next message will start incorrectly as it will have the rest of the incompleted message at the beginning, and will be ignored too because it can't be understood. that way, when overflow occurs, both messages will be lost.*/
                 else sonars_trac+=readn;
	    
              }else{
                 strncpy(sonars_in,&(sonars_in[beginning]),sonars_trac+readn-beginning);
                 sonars_trac = sonars_trac+readn-beginning;
                 beginning=0; /* for the next chunk received */
              }
           }
        }
     }
  }while(networkclient_close_command==0);
  pthread_exit(0);
}

/** networkclient driver pthread function for motors reception.*/
void *networkclient_motors_thread(void *not_used){
   float ac=0.;
   char message[MAX_MESSAGE];

   struct timeval a,b;
   long diff, next;

   printf("networkclient motors thread started\n");

   do{
      speedcounter(motors_schema_id);
      pthread_mutex_lock(&mymutex[MOTORS_DEVICE]);

      if (state[MOTORS_DEVICE]==slept){
         printf("networkclient motors thread goes sleep mode\n");
         pthread_cond_wait(&condition[MOTORS_DEVICE],&mymutex[MOTORS_DEVICE]);
         printf("networkclient motors thread woke up\n");
         pthread_mutex_unlock(&mymutex[MOTORS_DEVICE]);
      }else{
      
         /* sends to networkserver the speed control in mm/s, mms/s2 through a the device socket */
         gettimeofday(&a,NULL);
         sprintf(message,"%d %1.1f %1.1f\n",NETWORKSERVER_drive_speed,v,ac);
         write(device_socket[MOTORS_DEVICE],message,strlen(message));
         sprintf(message,"%d %1.1f %1.1f\n",NETWORKSERVER_steer_speed,w,ac);
         write(device_socket[MOTORS_DEVICE],message,strlen(message));
         gettimeofday(&b,NULL);
      
         pthread_mutex_unlock(&mymutex[MOTORS_DEVICE]);

         diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
         next = motors_cycle*1000-diff-10000;
      
         if (next>0)
            usleep(motors_cycle*1000-diff);
         else {
            usleep(motors_cycle*1000);
         }
      }
   }while(networkclient_close_command==0);
   pthread_exit(0);
}

int leer_cadena (int s, char * buff, int maximo){
   int leidos=0;
   int retorno=0;
   do{
      if (read (s, buff, 1)<0){
         fprintf(stderr, "error en read %s\n", strerror(errno));
         return -1;
      }
      leidos++;
      if (retorno){
         if (*buff=='\n'){
            buff[1]='\0';
            return leidos;
         }
         else{
            retorno=0;
         }
      }
      if (*buff=='\r'){
         retorno=1;
      }
      buff++;
   }while (leidos<maximo-1);
   return leidos;
}

/** networkclient driver pthread function for colorA reception.*/
void *networkclient_colorA_thread(void *not_used){
   char *mmbuf;
   unsigned long int network_clock;
   /*  fd_set dummy_set,read_set;*/
   FILE *buffer_colorA;
   buffer_colorA=fdopen(device_socket[COLORA_DEVICE], "r+");

   printf("networkclient colorA thread started\n");

   do{

      pthread_mutex_lock(&mymutex[COLORA_DEVICE]);

      if (state[COLORA_DEVICE]==slept){
         printf("networkclient colorA thread goes sleep mode\n");
         pthread_cond_wait(&condition[COLORA_DEVICE],&mymutex[COLORA_DEVICE]);
         printf("networkclient colorA thread woke up\n");
         pthread_mutex_unlock(&mymutex[COLORA_DEVICE]);
      }else{
      
         pthread_mutex_unlock(&mymutex[COLORA_DEVICE]);
         /*Write petition*/
         fprintf(buffer_colorA,"%d %d\n",
                 NETWORKSERVER_rgb24bpp_sifntsc_image_query,
                 device_network_id[COLORA_DEVICE]);
         fflush(buffer_colorA);
         /*Read message*/
         {
            int i2,i3,i4,i5,type;
            char buffer_lectura[MAX_MESSAGE];
            if (fgets(buffer_lectura, MAX_MESSAGE, buffer_colorA)!=NULL){
               if (sscanf(buffer_lectura,"%d %lu %d %d %d %d\n",&type,
                   &network_clock,&i2,&i3,&i4,&i5)!=6)
               {
                  printf("networkclient: i don't understand message from networkserver to colorA. (%s)\n",
                         buffer_lectura);
               }
               else{
                  if (type == NETWORKSERVER_rgb24bpp_sifntsc_image) {
                     int total=0;
                     int actual=0;
                     /*Read image*/
                     mmbuf=(char *)malloc(sizeof(char)*i3*i4*i5);
                     do{
                        actual=fread( &(mmbuf[total]), sizeof(char),
                                        (i3*i4*i5)-total, buffer_colorA);
                        if (actual>0)
                           total+=actual;
                        else{
                           if (ferror(buffer_colorA)){
                              perror("Error at fread");
                              clearerr(buffer_colorA);
                           }
                           if (feof(buffer_colorA)){
                              perror("EOF at fread");
                              clearerr(buffer_colorA);
                           }
                        }
                     }while(total<i3*i4*i5);

                     if (width[0]!=i3 || height[0]!=i4){
                        fprintf (stderr, "Error: network colorA image size and local are different\n");
                        jdeshutdown(1);
                     }
                     else{
                        memcpy (colorA, mmbuf, width[0]*height[0]*i5);
                        free(mmbuf);
                        speedcounter(colorA_schema_id);
                        imageA_clock=network_clock;
                     }
                  }
               }
            }
            else{
               fprintf(stderr, "Error en la lectura del socket de colorA, deja de ser servido\n");
               pthread_exit(0);
            }
         }
      }
   }while(networkclient_close_command==0);
   pthread_exit(0);
}

/** networkclient driver pthread function for colorB reception.*/
void *networkclient_colorB_thread(void *not_used){
   char * mmbuf;
   unsigned long int network_clock;
   /*  fd_set dummy_set,read_set;*/
   FILE *buffer_colorB;
   buffer_colorB=fdopen(device_socket[COLORB_DEVICE], "r+");

   printf("networkclient colorB thread started\n");

   do{

      pthread_mutex_lock(&mymutex[COLORB_DEVICE]);

      if (state[COLORB_DEVICE]==slept){
         printf("networkclient colorB thread goes sleep mode\n");
         pthread_cond_wait(&condition[COLORB_DEVICE],&mymutex[COLORB_DEVICE]);
         printf("networkclient colorB thread woke up\n");
         pthread_mutex_unlock(&mymutex[COLORB_DEVICE]);
      }else{
      
         pthread_mutex_unlock(&mymutex[COLORB_DEVICE]);
         /*Write petition*/
         fprintf(buffer_colorB,"%d %d\n",
                 NETWORKSERVER_rgb24bpp_sifntsc_image_query,
                 device_network_id[COLORB_DEVICE]);
         fflush(buffer_colorB);
         /*Read message*/
         {
            int i2,i3,i4,i5,type;
            char buffer_lectura[MAX_MESSAGE];
            if (fgets(buffer_lectura, MAX_MESSAGE, buffer_colorB)!=NULL){
               if (sscanf(buffer_lectura,"%d %lu %d %d %d %d\n",&type,
                   &network_clock,&i2,&i3,&i4,&i5)!=6)
               {
                  printf("networkclient: i don't understand message from networkserver to colorB. (%s)\n",
                         buffer_lectura);
               }
               else{
                  if (type == NETWORKSERVER_rgb24bpp_sifntsc_image) {
                     int total=0;
                     int actual=0;
                     /*Read image*/
                     mmbuf=(char *)malloc(sizeof(char)*i3*i4*i5);
                     do{
                        actual=fread( &(mmbuf[total]), sizeof(char),
                                        (i3*i4*i5)-total, buffer_colorB);
                        if (actual>0)
                           total+=actual;
                        else{
                           if (ferror(buffer_colorB)){
                              perror("Error at fread");
                              clearerr(buffer_colorB);
                           }
                           if (feof(buffer_colorB)){
                              perror("EOF at fread");
                              clearerr(buffer_colorB);
                           }
                        }
                     }while(total<i3*i4*i5);

                     if (width[1]!=i3 || height[1]!=i4){
                        fprintf (stderr, "Error: network colorB image size and local are different\n");
                        jdeshutdown(1);
                     }
                     else{
                        memcpy (colorB, mmbuf, width[1]*height[1]*i5);
                        free(mmbuf);
                        speedcounter(colorB_schema_id);
                        imageB_clock=network_clock;
                     }
                  }
               }
            }
            else{
               fprintf(stderr, "Error en la lectura del socket de colorB, deja de ser servido\n");
               pthread_exit(0);
            }
         }
      }
   }while(networkclient_close_command==0);
   pthread_exit(0);
}

/** networkclient driver pthread function for colorC reception.*/
void *networkclient_colorC_thread(void *not_used){
   char * mmbuf;
   unsigned long int network_clock;
   /*  fd_set dummy_set,read_set;*/
   FILE *buffer_colorC;
   buffer_colorC=fdopen(device_socket[COLORC_DEVICE], "r+");

   printf("networkclient colorC thread started\n");

   do{

      pthread_mutex_lock(&mymutex[COLORC_DEVICE]);

      if (state[COLORC_DEVICE]==slept){
         printf("networkclient colorC thread goes sleep mode\n");
         pthread_cond_wait(&condition[COLORC_DEVICE],&mymutex[COLORC_DEVICE]);
         printf("networkclient colorC thread woke up\n");
         pthread_mutex_unlock(&mymutex[COLORC_DEVICE]);
      }else{
      
         pthread_mutex_unlock(&mymutex[COLORC_DEVICE]);
         /*Write petition*/
         fprintf(buffer_colorC,"%d %d\n",
                 NETWORKSERVER_rgb24bpp_sifntsc_image_query,
                 device_network_id[COLORC_DEVICE]);
         fflush(buffer_colorC);
         /*Read message*/
         {
            int i2,i3,i4,i5,type;
            char buffer_lectura[MAX_MESSAGE];
            if (fgets(buffer_lectura, MAX_MESSAGE, buffer_colorC)!=NULL){
               if (sscanf(buffer_lectura,"%d %lu %d %d %d %d\n",&type,
                   &network_clock,&i2,&i3,&i4,&i5)!=6)
               {
                  printf("networkclient: i don't understand message from networkserver to colorC. (%s)\n",
                         buffer_lectura);
               }
               else{
                  if (type == NETWORKSERVER_rgb24bpp_sifntsc_image) {
                     int total=0;
                     int actual=0;
                     /*Read image*/
                     mmbuf=(char *)malloc(sizeof(char)*i3*i4*i5);
                     do{
                        actual=fread( &(mmbuf[total]), sizeof(char),
                                        (i3*i4*i5)-total, buffer_colorC);
                        if (actual>0)
                           total+=actual;
                        else{
                           if (ferror(buffer_colorC)){
                              perror("Error at fread");
                              clearerr(buffer_colorC);
                           }
                           if (feof(buffer_colorC)){
                              perror("EOF at fread");
                              clearerr(buffer_colorC);
                           }
                        }
                     }while(total<i3*i4*i5);

                     if (width[2]!=i3 || height[2]!=i4){
                        fprintf (stderr, "Error: network colorC image size and local are different\n");
                        jdeshutdown(1);
                     }
                     else{
                        memcpy (colorC, mmbuf, width[2]*height[2]*i5);
                        free(mmbuf);
                        speedcounter(colorC_schema_id);
                        imageB_clock=network_clock;
                     }
                  }
               }
            }
            else{
               fprintf(stderr, "Error en la lectura del socket de colorC, deja de ser servido\n");
               pthread_exit(0);
            }
         }
      }
   }while(networkclient_close_command==0);
   pthread_exit(0);
}

/** networkclient driver pthread function for colorD reception.*/
void *networkclient_colorD_thread(void *not_used){
   char *mmbuf;
   unsigned long int network_clock;
   /*  fd_set dummy_set,read_set;*/
   FILE *buffer_colorD;
   buffer_colorD=fdopen(device_socket[COLORD_DEVICE], "r+");

   printf("networkclient colorD thread started\n");

   do{

      pthread_mutex_lock(&mymutex[COLORD_DEVICE]);

      if (state[COLORD_DEVICE]==slept){
         printf("networkclient colorD thread goes sleep mode\n");
         pthread_cond_wait(&condition[COLORD_DEVICE],&mymutex[COLORD_DEVICE]);
         printf("networkclient colorD thread woke up\n");
         pthread_mutex_unlock(&mymutex[COLORD_DEVICE]);
      }else{
      
         pthread_mutex_unlock(&mymutex[COLORD_DEVICE]);
         /*Write petition*/
         fprintf(buffer_colorD,"%d %d\n",
                 NETWORKSERVER_rgb24bpp_sifntsc_image_query,
                 device_network_id[COLORD_DEVICE]);
         fflush(buffer_colorD);
         /*Read message*/
         {
            int i2,i3,i4,i5,type;
            char buffer_lectura[MAX_MESSAGE];
            if (fgets(buffer_lectura, MAX_MESSAGE, buffer_colorD)!=NULL){
               if (sscanf(buffer_lectura,"%d %lu %d %d %d %d\n",&type,
                   &network_clock,&i2,&i3,&i4,&i5)!=6)
               {
                  printf("networkclient: i don't understand message from networkserver to colorD. (%s)\n",
                         buffer_lectura);
               }
               else{
                  if (type == NETWORKSERVER_rgb24bpp_sifntsc_image) {
                     int total=0;
                     int actual=0;
                     /*Read image*/
                     mmbuf=(char *)malloc(sizeof(char)*i3*i4*i5);
                     do{
                        actual=fread( &(mmbuf[total]), sizeof(char),
                                        (i3*i4*i5)-total, buffer_colorD);
                        if (actual>0)
                           total+=actual;
                        else{
                           if (ferror(buffer_colorD)){
                              perror("Error at fread");
                              clearerr(buffer_colorD);
                           }
                           if (feof(buffer_colorD)){
                              perror("EOF at fread");
                              clearerr(buffer_colorD);
                           }
                        }
                     }while(total<i3*i4*i5);

                     if (width[3]!=i3 || height[3]!=i4){
                        fprintf (stderr, "Error: network colorD image size and local are different\n");
                        jdeshutdown(1);
                     }
                     else{
                        memcpy (colorD, mmbuf, width[3]*height[3]*i5);
                        free(mmbuf);
                        speedcounter(colorD_schema_id);
                        imageB_clock=network_clock;
                     }
                  }
               }
            }
            else{
               fprintf(stderr, "Error en la lectura del socket de colorD, deja de ser servido\n");
               pthread_exit(0);
            }
         }
      }
   }while(networkclient_close_command==0);
   pthread_exit(0);
}

/** networkclient driver pthread function for varcolorA reception.*/
void *networkclient_varcolorA_thread(void *not_used){
   char *mmbuf;
   unsigned long int network_clock;
   /*  fd_set dummy_set,read_set;*/
   FILE *buffer_varcolorA;
   buffer_varcolorA=fdopen(device_socket[VARCOLORA_DEVICE], "r+");

   printf("networkclient varcolorA thread started\n");

   do{

      pthread_mutex_lock(&mymutex[VARCOLORA_DEVICE]);

      if (state[VARCOLORA_DEVICE]==slept){
         printf("networkclient varcolorA thread goes sleep mode\n");
         pthread_cond_wait(&condition[VARCOLORA_DEVICE],&mymutex[VARCOLORA_DEVICE]);
         printf("networkclient varcolorA thread woke up\n");
         pthread_mutex_unlock(&mymutex[VARCOLORA_DEVICE]);
      }else{
      
         pthread_mutex_unlock(&mymutex[VARCOLORA_DEVICE]);
         /*Write petition*/
         fprintf(buffer_varcolorA,"%d %d\n",
                 NETWORKSERVER_rgb24bpp_sifntsc_image_query,
                 device_network_id[VARCOLORA_DEVICE]);
         fflush(buffer_varcolorA);
         /*Read message*/
         {
            int i2,i3,i4,i5,type;
            char buffer_lectura[MAX_MESSAGE];
            if (fgets(buffer_lectura, MAX_MESSAGE, buffer_varcolorA)!=NULL){
               if (sscanf(buffer_lectura,"%d %lu %d %d %d %d\n",&type,
                   &network_clock,&i2,&i3,&i4,&i5)!=6)
               {
                  printf("networkclient: i don't understand message from networkserver to varcolorA. (%s)\n",
                         buffer_lectura);
               }
               else{
                  if (type == NETWORKSERVER_rgb24bpp_sifntsc_image) {
                     int total=0;
                     int actual=0;
                     /*Read image*/
                     mmbuf=(char *)malloc(sizeof(char)*i3*i4*i5);
                     do{
                        actual=fread( &(mmbuf[total]), sizeof(char),
                                        (i3*i4*i5)-total, buffer_varcolorA);
                        if (actual>0)
                           total+=actual;
                        else{
                           if (ferror(buffer_varcolorA)){
                              perror("Error at fread");
                              clearerr(buffer_varcolorA);
                           }
                           if (feof(buffer_varcolorA)){
                              perror("EOF at fread");
                              clearerr(buffer_varcolorA);
                           }
                        }
                     }while(total<i3*i4*i5);

                     if (width[4]!=i3 || height[4]!=i4){
                        fprintf (stderr, "Error: network varcolorA image size and local are different\n");
                        jdeshutdown(1);
                     }
                     else{
                        memcpy (varcolorA, mmbuf, width[4]*height[4]*i5);
                        free(mmbuf);
                        speedcounter(varcolorA_schema_id);
                        imageB_clock=network_clock;
                     }
                  }
               }
            }
            else{
               fprintf(stderr, "Error en la lectura del socket de varcolorA, deja de ser servido\n");
               pthread_exit(0);
            }
         }
      }
   }while(networkclient_close_command==0);
   pthread_exit(0);
}

/** networkclient driver pthread function for varcolorB reception.*/
void *networkclient_varcolorB_thread(void *not_used){
   char *mmbuf;
   unsigned long int network_clock;
   /*  fd_set dummy_set,read_set;*/
   FILE *buffer_varcolorB;
   buffer_varcolorB=fdopen(device_socket[VARCOLORB_DEVICE], "r+");

   printf("networkclient varcolorB thread started\n");

   do{

      pthread_mutex_lock(&mymutex[VARCOLORB_DEVICE]);

      if (state[VARCOLORB_DEVICE]==slept){
         printf("networkclient varcolorB thread goes sleep mode\n");
         pthread_cond_wait(&condition[VARCOLORB_DEVICE],&mymutex[VARCOLORB_DEVICE]);
         printf("networkclient varcolorB thread woke up\n");
         pthread_mutex_unlock(&mymutex[VARCOLORB_DEVICE]);
      }else{
      
         pthread_mutex_unlock(&mymutex[VARCOLORB_DEVICE]);
         /*Write petition*/
         fprintf(buffer_varcolorB,"%d %d\n",
                 NETWORKSERVER_rgb24bpp_sifntsc_image_query,
                 device_network_id[VARCOLORB_DEVICE]);
         fflush(buffer_varcolorB);
         /*Read message*/
         {
            int i2,i3,i4,i5,type;
            char buffer_lectura[MAX_MESSAGE];
            if (fgets(buffer_lectura, MAX_MESSAGE, buffer_varcolorB)!=NULL){
               if (sscanf(buffer_lectura,"%d %lu %d %d %d %d\n",&type,
                   &network_clock,&i2,&i3,&i4,&i5)!=6)
               {
                  printf("networkclient: i don't understand message from networkserver to varcolorB. (%s)\n",
                         buffer_lectura);
               }
               else{
                  if (type == NETWORKSERVER_rgb24bpp_sifntsc_image) {
                     int total=0;
                     int actual=0;
                     /*Read image*/
                     mmbuf=(char *)malloc(sizeof(char)*i3*i4*i5);
                     do{
                        actual=fread( &(mmbuf[total]), sizeof(char),
                                        (i3*i4*i5)-total, buffer_varcolorB);
                        if (actual>0)
                           total+=actual;
                        else{
                           if (ferror(buffer_varcolorB)){
                              perror("Error at fread");
                              clearerr(buffer_varcolorB);
                           }
                           if (feof(buffer_varcolorB)){
                              perror("EOF at fread");
                              clearerr(buffer_varcolorB);
                           }
                        }
                     }while(total<i3*i4*i5);

                     if (width[5]!=i3 || height[5]!=i4){
                        fprintf (stderr, "Error: network varcolorB image size and local are different\n");
                        jdeshutdown(1);
                     }
                     else{
                        memcpy (varcolorB, mmbuf, width[5]*height[5]*i5);
                        free(mmbuf);
                        speedcounter(varcolorB_schema_id);
                        imageB_clock=network_clock;
                     }
                  }
               }
            }
            else{
               fprintf(stderr, "Error en la lectura del socket de varcolorB, deja de ser servido\n");
               pthread_exit(0);
            }
         }
      }
   }while(networkclient_close_command==0);
   pthread_exit(0);
}

/** networkclient driver pthread function for varcolorC reception.*/
void *networkclient_varcolorC_thread(void *not_used){
   char *mmbuf;
   unsigned long int network_clock;
   /*  fd_set dummy_set,read_set;*/
   FILE *buffer_varcolorC;
   buffer_varcolorC=fdopen(device_socket[VARCOLORC_DEVICE], "r+");

   printf("networkclient varcolorC thread started\n");

   do{

      pthread_mutex_lock(&mymutex[VARCOLORC_DEVICE]);

      if (state[VARCOLORC_DEVICE]==slept){
         printf("networkclient varcolorC thread goes sleep mode\n");
         pthread_cond_wait(&condition[VARCOLORC_DEVICE],&mymutex[VARCOLORC_DEVICE]);
         printf("networkclient varcolorC thread woke up\n");
         pthread_mutex_unlock(&mymutex[VARCOLORC_DEVICE]);
      }else{
      
         pthread_mutex_unlock(&mymutex[VARCOLORC_DEVICE]);
         /*Write petition*/
         fprintf(buffer_varcolorC,"%d %d\n",
                 NETWORKSERVER_rgb24bpp_sifntsc_image_query,
                 device_network_id[VARCOLORC_DEVICE]);
         fflush(buffer_varcolorC);
         /*Read message*/
         {
            int i2,i3,i4,i5,type;
            char buffer_lectura[MAX_MESSAGE];
            if (fgets(buffer_lectura, MAX_MESSAGE, buffer_varcolorC)!=NULL){
               if (sscanf(buffer_lectura,"%d %lu %d %d %d %d\n",&type,
                   &network_clock,&i2,&i3,&i4,&i5)!=6)
               {
                  printf("networkclient: i don't understand message from networkserver to varcolorC. (%s)\n",
                         buffer_lectura);
               }
               else{
                  if (type == NETWORKSERVER_rgb24bpp_sifntsc_image) {
                     int total=0;
                     int actual=0;
                     /*Read image*/
                     mmbuf=(char *)malloc(sizeof(char)*i3*i4*i5);
                     do{
                        actual=fread( &(mmbuf[total]), sizeof(char),
                                        (i3*i4*i5)-total, buffer_varcolorC);
                        if (actual>0)
                           total+=actual;
                        else{
                           if (ferror(buffer_varcolorC)){
                              perror("Error at fread");
                              clearerr(buffer_varcolorC);
                           }
                           if (feof(buffer_varcolorC)){
                              perror("EOF at fread");
                              clearerr(buffer_varcolorC);
                           }
                        }
                     }while(total<i3*i4*i5);

                     if (width[6]!=i3 || height[6]!=i4){
                        fprintf (stderr, "Error: network varcolorC image size and local are different\n");
                        jdeshutdown(1);
                     }
                     else{
                        memcpy (varcolorC, mmbuf, width[6]*height[6]*i5);
                        free(mmbuf);
                        speedcounter(varcolorC_schema_id);
                        imageB_clock=network_clock;
                     }
                  }
               }
            }
            else{
               fprintf(stderr, "Error en la lectura del socket de varcolorC, deja de ser servido\n");
               pthread_exit(0);
            }
         }
      }
   }while(networkclient_close_command==0);
   pthread_exit(0);
}

/** networkclient driver pthread function for varcolorD reception.*/
void *networkclient_varcolorD_thread(void *not_used){
   char *mmbuf;
   unsigned long int network_clock;
   /*  fd_set dummy_set,read_set;*/
   FILE *buffer_varcolorD;
   buffer_varcolorD=fdopen(device_socket[VARCOLORD_DEVICE], "r+");

   printf("networkclient varcolorD thread started\n");

   do{

      pthread_mutex_lock(&mymutex[VARCOLORD_DEVICE]);

      if (state[VARCOLORD_DEVICE]==slept){
         printf("networkclient varcolorD thread goes sleep mode\n");
         pthread_cond_wait(&condition[VARCOLORD_DEVICE],&mymutex[VARCOLORD_DEVICE]);
         printf("networkclient varcolorD thread woke up\n");
         pthread_mutex_unlock(&mymutex[VARCOLORD_DEVICE]);
      }else{
      
         pthread_mutex_unlock(&mymutex[VARCOLORD_DEVICE]);
         /*Write petition*/
         fprintf(buffer_varcolorD,"%d %d\n",
                 NETWORKSERVER_rgb24bpp_sifntsc_image_query,
                 device_network_id[VARCOLORD_DEVICE]);
         fflush(buffer_varcolorD);
         /*Read message*/
         {
            int i2,i3,i4,i5,type;
            char buffer_lectura[MAX_MESSAGE];
            if (fgets(buffer_lectura, MAX_MESSAGE, buffer_varcolorD)!=NULL){
               if (sscanf(buffer_lectura,"%d %lu %d %d %d %d\n",&type,
                   &network_clock,&i2,&i3,&i4,&i5)!=6)
               {
                  printf("networkclient: i don't understand message from networkserver to varcolorD. (%s)\n",
                         buffer_lectura);
               }
               else{
                  if (type == NETWORKSERVER_rgb24bpp_sifntsc_image) {
                     int total=0;
                     int actual=0;
                     /*Read image*/
                     mmbuf=(char *)malloc(sizeof(char)*i3*i4*i5);
                     do{
                        actual=fread( &(mmbuf[total]), sizeof(char),
                                        (i3*i4*i5)-total, buffer_varcolorD);
                        if (actual>0)
                           total+=actual;
                        else{
                           if (ferror(buffer_varcolorD)){
                              perror("Error at fread");
                              clearerr(buffer_varcolorD);
                           }
                           if (feof(buffer_varcolorD)){
                              perror("EOF at fread");
                              clearerr(buffer_varcolorD);
                           }
                        }
                     }while(total<i3*i4*i5);

                     if (width[7]!=i3 || height[7]!=i4){
                        fprintf (stderr, "Error: network varcolorD image size and local are different\n");
                        jdeshutdown(1);
                     }
                     else{
                        memcpy (varcolorD, mmbuf, width[7]*height[7]*i5);
                        free(mmbuf);
                        speedcounter(varcolorD_schema_id);
                        imageB_clock=network_clock;
                     }
                  }
               }
            }
            else{
               fprintf(stderr, "Error en la lectura del socket de varcolorD, deja de ser servido\n");
               pthread_exit(0);
            }
         }
      }
   }while(networkclient_close_command==0);
   pthread_exit(0);
}

/** networkclient driver parse configuration file function.
 *  @param configfile path and name to the config file.
 *  @return 0 if parsing was successful or -1 if something went wrong.*/
int networkclient_parseconf(char *configfile){

  int end_parse=0; int end_section=0; int driver_config_parsed=0;
  FILE *myfile;
  const int limit = 256;

  if ((myfile=fopen(configfile,"r"))==NULL){
    printf("networkclient: cannot find config file\n");
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
	printf ("networkclient: line too long in config file!\n"); 
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
	      
	      char buffer_file2[256],word3[256],word4[256],word5[256],word6[256],word7[256];
              char word8[256],word9[256];
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
		    printf("networkclient: error in config file.\n'end_section' keyword required before starting new driver section.\n");
		    end_section=1; end_parse=1;

		  }else if(strcmp(word3,"provides")==0){
                     int words=0;
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if((words=sscanf(buffer_file2,"%s %s %s %s %s %s %s",word3,word4,
                        word5,word6,word7,word8,word9))>=5)
                    {
                       if (words==5){
                          if((strcmp(word4,"colorA")==0)&&(serve_device[COLORA_DEVICE]==0)){
                             serve_device[COLORA_DEVICE]=1;
                             device_active[COLORA_DEVICE]=0;
                             strcpy(hostname[COLORA_DEVICE],word5);
                             port[COLORA_DEVICE]=atoi(word6);
                             device_network_id[COLORA_DEVICE]=atoi(word7);
                             width[0]=SIFNTSC_COLUMNS;
                             height[0]=SIFNTSC_ROWS;
                          }
                          else if((strcmp(word4,"colorB")==0)&&(serve_device[COLORB_DEVICE]==0)){
                             serve_device[COLORB_DEVICE]=1;
                             device_active[COLORB_DEVICE]=0;
                             strcpy(hostname[COLORB_DEVICE],word5);
                             port[COLORB_DEVICE]=atoi(word6);
                             device_network_id[COLORB_DEVICE]=atoi(word7);
                             width[1]=SIFNTSC_COLUMNS;
                             height[1]=SIFNTSC_ROWS;
                          }
                          else if((strcmp(word4,"colorC")==0)&&(serve_device[COLORC_DEVICE]==0)){
                             serve_device[COLORC_DEVICE]=1;
                             device_active[COLORC_DEVICE]=0;
                             strcpy(hostname[COLORC_DEVICE],word5);
                             port[COLORC_DEVICE]=atoi(word6);
                             device_network_id[COLORC_DEVICE]=atoi(word7);
                             width[2]=SIFNTSC_COLUMNS;
                             height[2]=SIFNTSC_ROWS;
                          }
                          else if((strcmp(word4,"colorD")==0)&&(serve_device[COLORD_DEVICE]==0)){
                             serve_device[COLORD_DEVICE]=1;
                             device_active[COLORD_DEVICE]=0;
                             strcpy(hostname[COLORD_DEVICE],word5);
                             port[COLORD_DEVICE]=atoi(word6);
                             device_network_id[COLORD_DEVICE]=atoi(word7);
                             width[3]=SIFNTSC_COLUMNS;
                             height[3]=SIFNTSC_ROWS;
                          }
                       }
                       else if (words==7){
                          if((strcmp(word4,"varcolorA")==0)&&(serve_device[VARCOLORA_DEVICE]==0)){
                             serve_device[VARCOLORA_DEVICE]=1;
                             device_active[VARCOLORA_DEVICE]=0;
                             strcpy(hostname[VARCOLORA_DEVICE],word5);
                             port[VARCOLORA_DEVICE]=atoi(word6);
                             device_network_id[VARCOLORA_DEVICE]=atoi(word7);
                             width[4]=atoi(word8);
                             height[4]=atoi(word9);
                          }
                          else if((strcmp(word4,"varcolorB")==0)&&(serve_device[VARCOLORB_DEVICE]==0)){
                             serve_device[VARCOLORB_DEVICE]=1;
                             device_active[VARCOLORB_DEVICE]=0;
                             strcpy(hostname[VARCOLORB_DEVICE],word5);
                             port[VARCOLORB_DEVICE]=atoi(word6);
                             device_network_id[VARCOLORB_DEVICE]=atoi(word7);
                             width[5]=atoi(word8);
                             height[5]=atoi(word9);
                          }
                          else if((strcmp(word4,"varcolorC")==0)&&(serve_device[VARCOLORC_DEVICE]==0)){
                             serve_device[VARCOLORC_DEVICE]=1;
                             device_active[VARCOLORC_DEVICE]=0;
                             strcpy(hostname[VARCOLORC_DEVICE],word5);
                             port[VARCOLORC_DEVICE]=atoi(word6);
                             device_network_id[VARCOLORC_DEVICE]=atoi(word7);
                             width[6]=atoi(word8);
                             height[6]=atoi(word9);
                          }
                          else if((strcmp(word4,"varcolorD")==0)&&(serve_device[VARCOLORD_DEVICE]==0)){
                             serve_device[VARCOLORD_DEVICE]=1;
                             device_active[VARCOLORD_DEVICE]=0;
                             strcpy(hostname[VARCOLORD_DEVICE],word5);
                             port[VARCOLORD_DEVICE]=atoi(word6);
                             device_network_id[VARCOLORD_DEVICE]=atoi(word7);
                             width[7]=atoi(word8);
                             height[7]=atoi(word9);
                          }
                       }
		      else{
			printf("networkclient: provides line incorrect\n");
		      }
		    }
		    else if(sscanf(buffer_file2,"%s %s %s %s",word3,word4,word5,word6)>3){
		      
		      if((strcmp(word4,"laser")==0)&&(serve_device[LASER_DEVICE]==0)){
			serve_device[LASER_DEVICE]=1;
			device_active[LASER_DEVICE]=0;
			strcpy(hostname[LASER_DEVICE],word5);
			port[LASER_DEVICE]=atoi(word6);
		      }
		      else if((strcmp(word4,"sonars")==0)&&(serve_device[SONARS_DEVICE]==0)){
			serve_device[SONARS_DEVICE]=1;
			device_active[SONARS_DEVICE]=0;
			strcpy(hostname[SONARS_DEVICE],word5);
			port[SONARS_DEVICE]=atoi(word6);
		      }
		      else if((strcmp(word4,"encoders")==0)&&(serve_device[ENCODERS_DEVICE]==0)){
			serve_device[ENCODERS_DEVICE]=1;
			device_active[ENCODERS_DEVICE]=0;
			strcpy(hostname[ENCODERS_DEVICE],word5);
			port[ENCODERS_DEVICE]=atoi(word6);
		      }
		      else if((strcmp(word4,"motors")==0)&&(serve_device[MOTORS_DEVICE]==0)){
			serve_device[MOTORS_DEVICE]=1;
			device_active[MOTORS_DEVICE]=0;
			strcpy(hostname[MOTORS_DEVICE],word5);
			port[MOTORS_DEVICE]=atoi(word6);
		      }
		      else if((strcmp(word4,"pantiltencoders")==0)&&(serve_device[PANTILT_ENCODERS_DEVICE]==0)){
			serve_device[PANTILT_ENCODERS_DEVICE]=1;
			device_active[PANTILT_ENCODERS_DEVICE]=0;
			strcpy(hostname[PANTILT_ENCODERS_DEVICE],word5);
			port[PANTILT_ENCODERS_DEVICE]=atoi(word6);
		      }
		      else if((strcmp(word4,"pantiltmotors")==0)&&(serve_device[PANTILT_MOTORS_DEVICE]==0)){
			serve_device[PANTILT_MOTORS_DEVICE]=1;
			device_active[PANTILT_MOTORS_DEVICE]=0;
			strcpy(hostname[PANTILT_MOTORS_DEVICE],word5);
			port[PANTILT_MOTORS_DEVICE]=atoi(word6);
		      }
		      else{
			printf("networkclient: provides line incorrect\n");
		      }
		    }

		  }else if(strcmp(word3,"initial_position")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s %s %s",word3,word4,word5,word6)>3){
		      correcting_x=(float)atof(word4);
		      correcting_y=(float)atof(word5);
		      correcting_theta=(float)atof(word6);
		      printf("networkclient: correcting x=%.1f(mm)  y=%.1f(mm)  theta=%.1f(deg).\n",correcting_x,correcting_y,correcting_theta);
		      printf("networkclient: make sure this is the initial position in world file.\n");
		      
		    }else{
		      printf("networkclient: initial_position line incorrect\n");
		    }
		  }else printf("networkclient: i don't know what to do with '%s'\n",buffer_file2);
		}
	      }
	    }while(end_section==0);
	    end_section=0; 
	  }
	}
      }
    }
  }while(end_parse==0);
  
  /* checking if a driver section was read */
  if(driver_config_parsed==1){
    if((serve_device[COLORA_DEVICE]==0)&&(serve_device[COLORB_DEVICE]==0)&&(serve_device[COLORC_DEVICE]==0)&&(serve_device[COLORD_DEVICE]==0)){
      if((serve_device[PANTILT_MOTORS_DEVICE]==0)&&(serve_device[PANTILT_ENCODERS_DEVICE]==0)){
	if((serve_device[MOTORS_DEVICE]==0)&&(serve_device[LASER_DEVICE]==0)&&(serve_device[ENCODERS_DEVICE]==0)&&(serve_device[SONARS_DEVICE]==0)){
	  printf("networkclient: warning! no device provided.\n");
	}
      }
    }
    return 0;
  }else return -1;
}

/** networkclient driver init function.
 *  @return 0 if initialitation was successful or -1 if something went wrong.*/
int networkclient_init(){

  int i;

  /* init of socket connections */
  for(i=0;i<MAXDEVICE;i++){

    struct sockaddr_in server;
    struct hostent *hp;

    /* if that device is used */
    if(serve_device[i]){
      
      device_socket[i]=socket(AF_INET, SOCK_STREAM, 0);
      
      if (device_socket[i] < 0) {
	perror("opening stream socket");
	return -1;
      }

      server.sin_family = AF_INET;
      hp = gethostbyname(hostname[i]);
      if (hp == 0){
	perror("unknown host");
	return -1;
      }

      bcopy((char *)hp ->h_addr, (char *)&server.sin_addr, hp -> h_length); 
      server.sin_port = htons(port[i]);
	  
      if (connect(device_socket[i], (struct sockaddr *)&server,sizeof server ) >= 0)
	write(device_socket[i],CLIENT_NAME,MAX_CLIENT_NAME);

      /* a client name must be sent although the command line didn't specify it. the server ALWAYS will wait a 10 characters name
	 at the beginning of the connection. if the name isn't sent, then the first client message is taken as name.*/ 
      /* this socket is a blocking one, by default. uncomment the following if you prefer a non-blocking one. */
      /*if(fcntl(device_socket[i], F_SETFL, O_NONBLOCK) < 0) { printf("fcntl FSETFL, O_NONBLOCK\n");  exit(1);}*/

      switch(i){
      case COLORA_DEVICE: printf("networkclient: connecting to networkserver for colorA at %s:%d ",hostname[i],port[i]); break;
      case COLORB_DEVICE: printf("networkclient: connecting to networkserver for colorB at %s:%d ",hostname[i],port[i]); break;
      case COLORC_DEVICE: printf("networkclient: connecting to networkserver for colorC at %s:%d ",hostname[i],port[i]); break;
      case COLORD_DEVICE: printf("networkclient: connecting to networkserver for colorD at %s:%d ",hostname[i],port[i]); break;
      case PANTILT_ENCODERS_DEVICE: printf("networkclient: connecting to networkserver for pantiltencoders at %s:%d ",hostname[i],port[i]); break;
      case PANTILT_MOTORS_DEVICE: printf("networkclient: connecting to networkserver for pantiltmotors at %s:%d ",hostname[i],port[i]); break;
      case LASER_DEVICE: printf("networkclient: connecting to networkserver for laser at %s:%d ",hostname[i],port[i]); break;
      case SONARS_DEVICE: printf("networkclient: connecting to networkserver for sonars at %s:%d ",hostname[i],port[i]); break;
      case ENCODERS_DEVICE: printf("networkclient: connecting to networkserver for encoders at %s:%d ",hostname[i],port[i]); break;
      case MOTORS_DEVICE: printf("networkclient: connecting to networkserver for motors at %s:%d ",hostname[i],port[i]); break;
      }
      
      if (device_socket[i]<0){
	printf("...failed\n");
	/*exit(-1);*/

      }else{
	printf("...ok\n");
      }

      /* we create this device thread */
      pthread_mutex_init(&mymutex[i],NULL);
      pthread_mutex_lock(&mymutex[i]);
      state[i]=slept;
      switch(i){
      case COLORA_DEVICE:
         pthread_create(&network_thread[COLORA_DEVICE],NULL,networkclient_colorA_thread,NULL);
         break;
      case COLORB_DEVICE:
         pthread_create(&network_thread[COLORB_DEVICE],NULL,networkclient_colorB_thread,NULL);
         break;
      case COLORC_DEVICE:
         pthread_create(&network_thread[COLORC_DEVICE],NULL,networkclient_colorC_thread,NULL);
         break;
      case COLORD_DEVICE:
         pthread_create(&network_thread[COLORD_DEVICE],NULL,networkclient_colorD_thread,NULL);
         break;
      case VARCOLORA_DEVICE:
         pthread_create(&network_thread[VARCOLORA_DEVICE],NULL,networkclient_varcolorA_thread,NULL);
         break;
      case VARCOLORB_DEVICE:
         pthread_create(&network_thread[VARCOLORB_DEVICE],NULL,networkclient_varcolorB_thread,NULL);
         break;
      case VARCOLORC_DEVICE:
         pthread_create(&network_thread[VARCOLORC_DEVICE],NULL,networkclient_varcolorC_thread,NULL);
         break;
      case VARCOLORD_DEVICE:
         pthread_create(&network_thread[VARCOLORD_DEVICE],NULL,networkclient_varcolorD_thread,NULL);
         break;
      case PANTILT_ENCODERS_DEVICE:
         pthread_create(&network_thread[PANTILT_ENCODERS_DEVICE],NULL,networkclient_pantiltencoders_thread,NULL);
         break;
      case PANTILT_MOTORS_DEVICE:
         pthread_create(&network_thread[PANTILT_MOTORS_DEVICE],NULL,networkclient_pantiltmotors_thread,NULL);
         break;
      case LASER_DEVICE:
         pthread_create(&network_thread[LASER_DEVICE],NULL,networkclient_laser_thread,NULL);
         break;
      case SONARS_DEVICE:
         pthread_create(&network_thread[SONARS_DEVICE],NULL,networkclient_sonars_thread,NULL);
         break;
      case ENCODERS_DEVICE:
         pthread_create(&network_thread[ENCODERS_DEVICE],NULL,networkclient_encoders_thread,NULL);
         break;
      case MOTORS_DEVICE:
         pthread_create(&network_thread[MOTORS_DEVICE],NULL,networkclient_motors_thread,NULL);
         break;
      }
      pthread_mutex_unlock(&mymutex[i]);
    }
  }
  return 0;
}

/** networkclient driver startup function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void networkclient_startup(char *configfile)
{
  int i;

  for (i=0; i<MAXCAM; i++){
     width[i]=0;
     height[i]=0;
  }
  
  /* reseting serve color array and setting default options */
  for(i=0;i<MAXDEVICE;i++){serve_device[i]=0; device_active[i]=0; device_network_id[i]=0; state[i]=slept; device_socket[i]=0;}

  /* we call the function to parse the config file */
  if(networkclient_parseconf(configfile)==-1){
    printf("networkclient: cannot initiate driver. configfile parsing error.\n");
    exit(-1);
  }

  /* networkclient driver init */
  if(networkclient_init()!=0){
    printf("networkclient: cannot initiate driver. devices or net sockets not ready.\n");
    exit(-1);
  }

  /* resume and suspend asignments */
  if(serve_device[COLORA_DEVICE]){
    all[num_schemas].id = (int *) &colorA_schema_id;
    strcpy(all[num_schemas].name,"colorA");
    all[num_schemas].resume = (resumeFn) networkclient_colorA_resume;
    all[num_schemas].suspend = (suspendFn) networkclient_colorA_suspend;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].close = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    colorA=malloc(sizeof(char)*width[0]*height[0]*3);
    myexport("colorA", "id", &colorA_schema_id);
    myexport("colorA", "colorA", &colorA);
    myexport("colorA", "width", &(width[0]));
    myexport("colorA", "height", &(height[0]));
    myexport("colorA","clock", &imageA_clock);
    myexport("colorA","resume",(void *) &networkclient_colorA_resume);
    myexport("colorA","suspend",(void *) &networkclient_colorA_suspend);
  }
  if(serve_device[COLORB_DEVICE]){
    all[num_schemas].id = (int *) &colorB_schema_id;
    strcpy(all[num_schemas].name,"colorB");
    all[num_schemas].resume = (resumeFn) networkclient_colorB_resume;
    all[num_schemas].suspend = (suspendFn) networkclient_colorB_suspend;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].close = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    colorB=malloc(sizeof(char)*width[1]*height[1]*3);
    myexport("colorB", "id", &colorB_schema_id);
    myexport("colorB", "colorB", &colorB);
    myexport("colorB", "width", &(width[1]));
    myexport("colorB", "height", &(height[1]));
    myexport("colorB","clock", &imageB_clock);
    myexport("colorB","resume",(void *) &networkclient_colorB_resume);
    myexport("colorB","suspend",(void *) &networkclient_colorB_suspend);
  }
  if(serve_device[COLORC_DEVICE]){
    all[num_schemas].id = (int *) &colorC_schema_id;
    strcpy(all[num_schemas].name,"colorC");
    all[num_schemas].resume = (resumeFn) networkclient_colorC_resume;
    all[num_schemas].suspend = (suspendFn) networkclient_colorC_suspend;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].close = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    colorC=malloc(sizeof(char)*width[2]*height[2]*3);
    myexport("colorC", "id", &colorC_schema_id);
    myexport("colorC", "colorC", &colorC);
    myexport("colorC", "width", &(width[2]));
    myexport("colorC", "height", &(height[2]));
    myexport("colorC","clock", &imageC_clock);
    myexport("colorC","resume",(void *) &networkclient_colorC_resume);
    myexport("colorC","suspend",(void *) &networkclient_colorC_suspend);
  }
  if(serve_device[COLORD_DEVICE]){
    all[num_schemas].id = (int *) &colorD_schema_id;
    strcpy(all[num_schemas].name,"colorD");
    all[num_schemas].resume = (resumeFn) networkclient_colorD_resume;
    all[num_schemas].suspend = (suspendFn) networkclient_colorD_suspend;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].close = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    colorD=malloc(sizeof(char)*width[3]*height[3]*3);
    myexport("colorD", "id", &colorD_schema_id);
    myexport("colorD", "colorD", &colorD);
    myexport("colorD", "width", &(width[3]));
    myexport("colorD", "height",&(height[3]));
    myexport("colorD","clock", &imageD_clock);
    myexport("colorD","resume",(void *) &networkclient_colorD_resume);
    myexport("colorD","suspend",(void *) &networkclient_colorD_suspend);
  }
  if(serve_device[VARCOLORA_DEVICE]){
     all[num_schemas].id = (int *) &varcolorA_schema_id;
     strcpy(all[num_schemas].name,"varcolorA");
     all[num_schemas].resume = (resumeFn) networkclient_varcolorA_resume;
     all[num_schemas].suspend = (suspendFn) networkclient_varcolorA_suspend;
     printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
     (*(all[num_schemas].id)) = num_schemas;
     all[num_schemas].fps = 0.;
     all[num_schemas].k =0;
     all[num_schemas].state=slept;
     all[num_schemas].close = NULL;
     all[num_schemas].handle = NULL;
     num_schemas++;
     varcolorA=malloc(sizeof(char)*width[4]*height[4]*3);
     myexport("varcolorA", "id", &varcolorA_schema_id);
     myexport("varcolorA", "varcolorA", &varcolorA);
     myexport("varcolorA", "width", &(width[4]));
     myexport("varcolorA", "height", &(height[4]));
     myexport("varcolorA","clock", &varimageA_clock);
     myexport("varcolorA","resume",(void *) &networkclient_varcolorA_resume);
     myexport("varcolorA","suspend",(void *) &networkclient_varcolorA_suspend);
  }
  if(serve_device[VARCOLORB_DEVICE]){
     all[num_schemas].id = (int *) &varcolorB_schema_id;
     strcpy(all[num_schemas].name,"varcolorB");
     all[num_schemas].resume = (resumeFn) networkclient_varcolorB_resume;
     all[num_schemas].suspend = (suspendFn) networkclient_varcolorB_suspend;
     printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
     (*(all[num_schemas].id)) = num_schemas;
     all[num_schemas].fps = 0.;
     all[num_schemas].k =0;
     all[num_schemas].state=slept;
     all[num_schemas].close = NULL;
     all[num_schemas].handle = NULL;
     num_schemas++;
     varcolorB=malloc(sizeof(char)*width[5]*height[5]*3);
     myexport("varcolorB", "id", &varcolorB_schema_id);
     myexport("varcolorB", "varcolorB", &varcolorB);
     myexport("varcolorB", "width", &(width[5]));
     myexport("varcolorB", "height", &(height[5]));
     myexport("varcolorB","clock", &varimageB_clock);
     myexport("varcolorB","resume",(void *) &networkclient_varcolorB_resume);
     myexport("varcolorB","suspend",(void *) &networkclient_varcolorB_suspend);
  }
  if(serve_device[VARCOLORC_DEVICE]){
     all[num_schemas].id = (int *) &varcolorC_schema_id;
     strcpy(all[num_schemas].name,"varcolorC");
     all[num_schemas].resume = (resumeFn) networkclient_varcolorC_resume;
     all[num_schemas].suspend = (suspendFn) networkclient_varcolorC_suspend;
     printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
     (*(all[num_schemas].id)) = num_schemas;
     all[num_schemas].fps = 0.;
     all[num_schemas].k =0;
     all[num_schemas].state=slept;
     all[num_schemas].close = NULL;
     all[num_schemas].handle = NULL;
     num_schemas++;
     varcolorC=malloc(sizeof(char)*width[6]*height[6]*3);
     myexport("varcolorC", "id", &varcolorC_schema_id);
     myexport("varcolorC", "varcolorC", &varcolorC);
     myexport("varcolorC", "width", &(width[6]));
     myexport("varcolorC", "height", &(height[6]));
     myexport("varcolorC","clock", &varimageC_clock);
     myexport("varcolorC","resume",(void *) &networkclient_varcolorC_resume);
     myexport("varcolorC","suspend",(void *) &networkclient_varcolorC_suspend);
  }
  if(serve_device[VARCOLORD_DEVICE]){
     all[num_schemas].id = (int *) &varcolorD_schema_id;
     strcpy(all[num_schemas].name,"varcolorD");
     all[num_schemas].resume = (resumeFn) networkclient_varcolorD_resume;
     all[num_schemas].suspend = (suspendFn) networkclient_varcolorD_suspend;
     printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
     (*(all[num_schemas].id)) = num_schemas;
     all[num_schemas].fps = 0.;
     all[num_schemas].k =0;
     all[num_schemas].state=slept;
     all[num_schemas].close = NULL;
     all[num_schemas].handle = NULL;
     num_schemas++;
     varcolorD=malloc(sizeof(char)*width[7]*height[7]*3);
     myexport("varcolorD", "id", &varcolorD_schema_id);
     myexport("varcolorD", "varcolorD", &varcolorD);
     myexport("varcolorD", "width", &(width[7]));
     myexport("varcolorD", "height", &(height[7]));
     myexport("varcolorD","clock", &varimageD_clock);
     myexport("varcolorD","resume",(void *) &networkclient_varcolorD_resume);
     myexport("varcolorD","suspend",(void *) &networkclient_varcolorD_suspend);
  }
  if(serve_device[PANTILT_ENCODERS_DEVICE]){
    all[num_schemas].id = (int *) &ptencoders_schema_id;
    strcpy(all[num_schemas].name,"ptencoders");
    all[num_schemas].resume = (resumeFn) networkclient_pantiltencoders_resume;
    all[num_schemas].suspend = (suspendFn) networkclient_pantiltencoders_suspend;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].close = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    myexport("ptencoders","id",&ptencoders_schema_id);
    myexport("ptencoders","pan_angle",&pan_angle);
    myexport("ptencoders","tilt_angle",&tilt_angle);
    myexport("ptencoders", "clock", &pantiltencoders_clock);
    myexport("ptencoders","resume",(void *)&networkclient_pantiltencoders_resume);
    myexport("ptencoders","suspend",(void *)&networkclient_pantiltencoders_suspend);
  }
  if(serve_device[PANTILT_MOTORS_DEVICE]){
    all[num_schemas].id = (int *) &ptmotors_schema_id;
    strcpy(all[num_schemas].name,"ptmotors");
    all[num_schemas].resume = (resumeFn) networkclient_pantiltmotors_resume;
    all[num_schemas].suspend = (suspendFn) networkclient_pantiltmotors_suspend;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].close = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    pantiltmotors_cycle=150; /*Ajusta el cilo de envío de órdenes a los motores*/
    myexport("ptmotors","id",&ptmotors_schema_id);
    myexport("ptmotors","longitude",&longitude);
    myexport("ptmotors","latitude",&latitude);
    myexport("ptmotors","longitude_speed",&longitude_speed);
    myexport("ptmotors","latitude_speed",&latitude_speed);
    myexport("ptmotors","cycle", &pantiltmotors_cycle);
    myexport("ptmotors","resume",(void *)&networkclient_pantiltmotors_resume);
    myexport("ptmotors","suspend",(void *)&networkclient_pantiltmotors_suspend);
  }
  if(serve_device[LASER_DEVICE]){
    all[num_schemas].id = (int *) &laser_schema_id;
    strcpy(all[num_schemas].name,"laser");
    all[num_schemas].resume = (resumeFn) networkclient_laser_resume;
    all[num_schemas].suspend = (suspendFn) networkclient_laser_suspend;
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
    myexport("laser","resume",(void *) &networkclient_laser_resume);
    myexport("laser","suspend",(void *) &networkclient_laser_suspend);
  }
  if(serve_device[ENCODERS_DEVICE]){
    all[num_schemas].id = (int *) &encoders_schema_id;
    strcpy(all[num_schemas].name,"encoders");
    all[num_schemas].resume = (resumeFn) networkclient_encoders_resume;
    all[num_schemas].suspend = (suspendFn) networkclient_encoders_suspend;
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
    myexport("encoders","resume",(void *) &networkclient_encoders_resume);
    myexport("encoders","suspend",(void *) &networkclient_encoders_suspend);
  }
  if(serve_device[SONARS_DEVICE]){
    all[num_schemas].id = (int *) &sonars_schema_id;
    strcpy(all[num_schemas].name,"sonars");
    all[num_schemas].resume = (resumeFn) networkclient_sonars_resume;
    all[num_schemas].suspend = (suspendFn) networkclient_sonars_suspend;
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
    myexport("sonars", "clock", &us_clock);
    myexport("sonars","number", &sonar_number);
    myexport("sonars","resume",(void *)&networkclient_sonars_resume);
    myexport("sonars","suspend",(void *)&networkclient_sonars_suspend);
  }
  if(serve_device[MOTORS_DEVICE]){
    all[num_schemas].id = (int *) &motors_schema_id;
    strcpy(all[num_schemas].name,"motors");
    all[num_schemas].resume = (resumeFn) networkclient_motors_resume;
    all[num_schemas].suspend = (suspendFn) networkclient_motors_suspend;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].close = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    motors_cycle=150; /*Ajusta el ciclo de envío a los motores, cada 150 ms*/
    myexport("motors","id",&motors_schema_id);
    myexport("motors","v",&v);
    myexport("motors","w",&w);
    myexport("motors","cycle",&motors_cycle);
    myexport("motors","resume",(void *)&networkclient_motors_resume);
    myexport("motors","suspend",(void *)&networkclient_motors_suspend);
  }

  printf("networkclient driver started up\n");
}
