/*
 *  Copyright (C) 2007 Javier Martín Ramos, Sara Marugán Alonso
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
 *  Authors : Javier Martín Ramos
 *            Sara Marugán Alonso
 */

/**
 *  jdec evi driver provides sensorial information from a evi neck conected.
 *
 *  @file evi.c
 *  @author Javier Martín Ramos <j.ramos@pantuflo.es>, Sara Marugán Alonso
 *  @version 0.1
 *  @date 2007.12.05
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "camera.h"
#include "jde.h"
#include "evi.h"
#include <math.h>

#define EVILIB_OFF                   1
#define EVILIB_ON                    2
#define EVILIB_min_pspeed                   1  /* En modo de velocidad */
#define EVILIB_min_tspeed                   1  /* En modo de velocidad */
#define EVILIB_max_pspeed                  24  /* En modo de velocidad */
#define EVILIB_max_tspeed                  20  /* En modo de velocidad */
#define EVILIB_max_pspeed_dps             300  /* grados por segundo */
#define EVILIB_max_tspeed_dps             125  /* grados por segundo */
#define EVILIB_min_zspeed                   0  // min zoom speed
#define EVILIB_max_zspeed                   7  // max zoom speed

#define EVILIB_minpan                    -100 // degrees
#define EVILIB_maxpan                     100 // degrees
#define EVILIB_mintilt                    -25 // degrees
#define EVILIB_maxtilt                     25 // degrees
#define EVILIB_minzoom                      0 // lens x0 digital zoom
#define EVILIB_maxzoom                     40 // lens x40 digital zoom

#define EVILIB_NO_WAIT_COMP 0
#define EVILIB_WAIT_COMP    1
#define EVILIB_ABSOLUTE             34
#define EVILIB_DIRECT       9

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/** evi driver device name. Example /dev/tty01.*/
#define MAX_MESSAGE 256
char evi_device[MAX_MESSAGE];

/** PTmotors thread*/
pthread_t PTmotorsTh;
/** PTencoders thread.*/
pthread_t PTencodersTh;
/** Zmotors thread*/
pthread_t ZmotorsTh;
/** Zencoders thread.*/
pthread_t ZencodersTh;

/** evi driver state variable for PTmotors pthread.*/
int state_PTmotors;
/** evi driver state variable for PTencoders pthread.*/
int state_PTencoders;
/** evi driver state variable for Zmotors pthread.*/
int state_Zmotors;
/** evi driver state variable for Zencoders pthread.*/
int state_Zencoders;
/** evi driver mutex for PTmotors pthread.*/
pthread_mutex_t mymutex_PTmotors;
/** evi driver mutex for PTencoders pthread.*/
pthread_mutex_t mymutex_PTencoders;
/** evi driver mutex for Zmotors pthread.*/
pthread_mutex_t mymutex_Zmotors;
/** evi driver mutex for Zencoders pthread.*/
pthread_mutex_t mymutex_Zencoders;
/** evi driver condition flag for PTmotors pthread.*/
pthread_cond_t condition_PTmotors;
/** evi driver condition flag for PTencoders pthread.*/
pthread_cond_t condition_PTencoders;
/** evi driver condition flag for Zmotors pthread.*/
pthread_cond_t condition_Zmotors;
/** evi driver condition flag for Zencoders pthread.*/
pthread_cond_t condition_Zencoders;

/** id for PTmotors schema.*/
int PTmotors_schema_id;
/** id for PTencoders schema.*/
int PTencoders_schema_id;
/** id for Zmotors schema.*/
int Zmotors_schema_id;
/** id for Zencoders schema.*/
int Zencoders_schema_id;

int PTmotors_cycle = 1000/30;
int PTencoders_cycle = 1000/30;
int Zmotors_cycle = 1000/30;
int Zencoders_cycle = 1000/30;
unsigned long PTencoders_clock = 0;
unsigned long Zencoders_clock = 0;

/** Camera */
camera *cam = NULL;

/** PTmotors: Setting camera position and speed */
float longitude=0.;
float latitude=0.;
float longitude_speed=0.;
float latitude_speed=0.;
float max_longitude = EVILIB_maxpan;
float max_latitude = EVILIB_maxtilt;
float min_longitude = EVILIB_minpan;
float min_latitude = EVILIB_mintilt;
float max_longitude_speed = EVILIB_max_pspeed_dps;
float max_latitude_speed = EVILIB_max_tspeed_dps;

/** PTencoders: Polling camera position and speed */
float longitudePoll=0.;
float latitudePoll=0.;

/** Zmotor */
float zoom;
float zoom_speed=0.;
float min_zoom = EVILIB_minzoom;
float max_zoom = EVILIB_maxzoom;
float max_zoom_speed = EVILIB_max_zspeed;

/** Zencoder */
float zoomPoll=0.;

/** evi driver variable to detect if pthreads were created.*/
int evi_thread_created=0;
/** evi driver variable to detect if evi devices were cleaned up.*/
int evi_cleaned_up=0;
/** evi driver  variable to detect if evi devices were setup.*/
int evi_setup=0;
/** evi driver to show fps in jdec shell.*/
int display_fps=0;
/** evi driver variable to detect if pthreads must end its execution.*/
int evi_close_command=0;

/* Ref counter*/
/** PTmotors ref counter*/
int PTmotors_refs=0;
/** PTencoders ref counter*/
int PTencoders_refs=0;
/** Zmotors ref counter*/
int Zmotors_refs=0;
/** Zencoders ref counter*/
int Zencoders_refs=0;

/** mutex for ref counters*/
pthread_mutex_t refmutex;

/* evi driver API options */
/** evi driver name.*/
char driver_name[256]="evi";

/* activate driver */
/** evi driver variable to detect if PTencoders were activated on gui.*/
int activate_PTencoders=0;
/** evi driver variable to detect if PTmotors were activated on gui.*/
int activate_PTmotors=0;
/** evi driver variable to detect if Zencoders were activated on gui.*/
int activate_Zencoders=0;
/** evi driver variable to detect if Zmotors were activated on gui.*/
int activate_Zmotors=0;


/** evi driver function to clean up evi devices.*/
void evi_clean_up() {
  evi_cleaned_up=1;
  PowerCamera(cam,EVILIB_OFF);
}

/** evi driver function to stop and clean evi devices.*/
void evi_close(){
  evi_close_command=1;
  evi_clean_up();
  printf("driver evi off\n");
}


void Zencoders_iteration() {
   speedcounter(Zencoders_schema_id);
   if(ZoomPosInqCamera(cam,&zoomPoll) != 1) {
      fprintf(stderr, "evi driver: Transmission interrupted\n");
      evi_close();
   }
//    printf("estoy en %f\n", zoomPoll);

}

void *ZencodersThread(void *not_used) {
   struct timeval a,b;
   long diff, next;


   do{
      pthread_mutex_lock(&mymutex_Zencoders);

      if (state_Zencoders==slept)
         pthread_cond_wait(&condition_Zencoders,&mymutex_Zencoders);
      else {
         gettimeofday(&a,NULL);
         Zencoders_iteration();
         gettimeofday(&b,NULL);
         diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
         next = Zencoders_cycle*1000-diff-10000;
         /* discounts 10ms taken by calling usleep itself */
         if (next>0)
            usleep(Zencoders_cycle*1000-diff);
         else {
            usleep(Zencoders_cycle*1000);
         }
      }
      pthread_mutex_unlock(&mymutex_Zencoders);
   } while(evi_close_command==0);

   pthread_exit(0);
}

/** Zencoders suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int Zencoders_suspend() {
   pthread_mutex_lock(&refmutex);
   if (Zencoders_refs > 1) {
      Zencoders_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      Zencoders_refs = 0;
      pthread_mutex_unlock(&refmutex);
      if (activate_Zencoders) {
         pthread_mutex_lock(&mymutex_Zencoders);
         state_Zencoders=slept;
         put_state(Zencoders_schema_id,slept);
         printf("Zencoders schema suspend\n");
         pthread_mutex_unlock(&mymutex_Zencoders);
      }
   }
   return 0;
}


/** Zencoders resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int Zencoders_resume(int father, int *brothers, arbitration fn) {
   pthread_mutex_lock(&refmutex);
   if (Zencoders_refs > 0){
      Zencoders_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      Zencoders_refs = 1;
      pthread_mutex_unlock(&refmutex);
      if (activate_Zencoders) {
         pthread_mutex_lock(&mymutex_Zencoders);
         state_Zencoders=active;
         all[Zencoders_schema_id].father = father;
         all[Zencoders_schema_id].fps = 0.;
         all[Zencoders_schema_id].k =0;
         put_state(Zencoders_schema_id,winner);
         printf("Zencoders schema resume\n");
         pthread_cond_signal(&condition_Zencoders);
         pthread_mutex_unlock(&mymutex_Zencoders);
      }
   }
   return 0;
}


/** Zmotors main iteration function.*/
void Zmotors_iteration() {
   static char initiated = 0;
   static float zoomPos, lastZoomPos;
   static int zoomSpeed;

   speedcounter(Zmotors_schema_id);

   if (!initiated) {
      initiated++;
      lastZoomPos = 0;
   }

   zoomPos = MIN( EVILIB_maxzoom, MAX(zoom, EVILIB_minzoom) );
   zoomSpeed = (int) zoom_speed;

   if (zoomSpeed > 0) {
      if (zoomPos != lastZoomPos) {
         int e;
         e = ZoomCamera(cam, EVILIB_DIRECT, zoomSpeed, zoomPos, EVILIB_NO_WAIT_COMP);
         if (e != 1) {
            fprintf(stderr, "evi driver: Transmission interrupted\n");
            evi_close();
         }
         lastZoomPos = zoomPos;
      }
   }
}

/** Zmotors suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int Zmotors_suspend()
{
   pthread_mutex_lock(&refmutex);
   if (Zmotors_refs > 1) {
      Zmotors_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      Zmotors_refs=0;
      pthread_mutex_unlock(&refmutex);
      if (activate_Zmotors) {
         pthread_mutex_lock(&mymutex_Zmotors);
         state_Zmotors=slept;
         put_state(Zmotors_schema_id,slept);
         printf("Zmotors schema suspend\n");
         pthread_mutex_unlock(&mymutex_Zmotors);
      }
   }
   return 0;
}


/** Zmotors resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int Zmotors_resume(int father, int *brothers, arbitration fn)
{
   pthread_mutex_lock(&refmutex);
   if (Zmotors_refs > 0){
      Zmotors_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      Zmotors_refs=1;
      pthread_mutex_unlock(&refmutex);
      if (activate_Zmotors) {
         pthread_mutex_lock(&mymutex_Zmotors);
         state_Zmotors=winner;
         all[Zmotors_schema_id].father = father;
         all[Zmotors_schema_id].fps = 0.;
         all[Zmotors_schema_id].k =0;
         put_state(Zmotors_schema_id,winner);
         printf("Zmotors schema resume\n");
         pthread_cond_signal(&condition_Zmotors);
         pthread_mutex_unlock(&mymutex_Zmotors);
      }
   }
   return 0;
}

/** Zmotors pthread function.*/
void *ZmotorsThread(void *not_used) {
   struct timeval a,b;
   long diff, next;

   do{
      if (state_Zmotors==slept) {
         pthread_mutex_lock(&mymutex_Zmotors);
         /* printf("Zmotors off\n");*/
         pthread_cond_wait(&condition_Zmotors,&mymutex_Zmotors);
         /* printf("Zmotors on\n");*/
         pthread_mutex_unlock(&mymutex_Zmotors);
      }
      else {
         pthread_mutex_unlock(&mymutex_Zmotors);
         gettimeofday(&a,NULL);
         Zmotors_iteration();
         gettimeofday(&b,NULL);  
         diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
         next = Zmotors_cycle*1000-diff-10000;
         /* discounts 10ms taken by calling usleep itself */
         if (next>0)
            usleep(Zmotors_cycle*1000-diff);
         else {
            //printf("time interval violated: Zmotors\n");
            usleep(Zmotors_cycle*1000);
         }
      }
   } while(evi_close_command==0);

   pthread_exit(0);
}



void PTencoders_iteration() {
   speedcounter(PTencoders_schema_id);
   if(Pan_TiltPosInqCamera(cam, &longitudePoll, &latitudePoll) != 1) {
      fprintf(stderr, "evi driver: Transmission interrupted\n");
      evi_close();
   }
//    printf("estoy en %f %f\n", longitudePoll, latitudePoll);
}

/** PTencoders poll pthread function.*/
void *PTencodersThread(void *not_used) {
   struct timeval a,b;
   long diff, next;


   do{
      pthread_mutex_lock(&mymutex_PTencoders);

      if (state_PTencoders==slept) 
         pthread_cond_wait(&condition_PTencoders,&mymutex_PTencoders);
      else {
         gettimeofday(&a,NULL);
         PTencoders_iteration();
         gettimeofday(&b,NULL);
         diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
         next = PTencoders_cycle*1000-diff-10000;
         /* discounts 10ms taken by calling usleep itself */
         if (next>0)
            usleep(PTencoders_cycle*1000-diff);
         else {
            usleep(PTencoders_cycle*1000);
         }
      }
      pthread_mutex_unlock(&mymutex_PTencoders);
   } while(evi_close_command==0);

   pthread_exit(0);
}


/** PTencoders suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int PTencoders_suspend() {
   pthread_mutex_lock(&refmutex);
   if (PTencoders_refs > 1) {
      PTencoders_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      PTencoders_refs = 0;
      pthread_mutex_unlock(&refmutex);
      if (activate_PTencoders) {
         pthread_mutex_lock(&mymutex_PTencoders);
         state_PTencoders=slept;
         put_state(PTencoders_schema_id,slept);
         printf("PTencoders schema suspend\n");
         pthread_mutex_unlock(&mymutex_PTencoders);
      }
   }
   return 0;
}


/** PTencoders resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int PTencoders_resume(int father, int *brothers, arbitration fn) {
   pthread_mutex_lock(&refmutex);
   if (PTencoders_refs > 0){
      PTencoders_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      PTencoders_refs = 1;
      pthread_mutex_unlock(&refmutex);
      if (activate_PTencoders) {
         pthread_mutex_lock(&mymutex_PTencoders);
         state_PTencoders=active;
         all[PTencoders_schema_id].father = father;
         all[PTencoders_schema_id].fps = 0.;
         all[PTencoders_schema_id].k =0;
         put_state(PTencoders_schema_id,winner);
         printf("PTencoders schema resume\n");
         pthread_cond_signal(&condition_PTencoders);
         pthread_mutex_unlock(&mymutex_PTencoders);
      }
   }
   return 0;
}

inline int degreesPerSecond2PanSpeed(float dps) {
   const float panSpeedMode2dps[EVILIB_max_pspeed+1] = {0.0, 2.0, 2.4, 3.0, 3.7,
      4.7, 6.1, 7.4, 9.1, 11., 14., 18., 22., 27., 34., 42., 52., 65., 81., 100.,
      125., 155., 190., 240., 300.};
   int i;
   for (i = EVILIB_max_pspeed; i >= (EVILIB_min_pspeed - 1); i--) {
      if (dps >= panSpeedMode2dps[i]) {
         break;
      }
   }
   return i ;
}


inline int degreesPerSecond2TiltSpeed(float dps) {
   const float tiltSpeedMode2dps[EVILIB_max_tspeed+1] = {0.0, 2.0, 2.4, 3.0, 3.7,
      4.7, 6.1, 7.4, 9.1, 11., 14., 18., 22., 27., 34., 42., 52., 65., 81., 100.,
      125.};
   int i;
   for (i = EVILIB_max_tspeed; i >= (EVILIB_min_tspeed - 1); i--) {
      if (dps >= tiltSpeedMode2dps[i]) {
         break;
      }
   }
   return i ;
}

/** PTmotors main iteration function.*/
void PTmotors_iteration() {
   static char initiated = 0;
   static unsigned char panSpeedLUT[(int) EVILIB_max_pspeed_dps * 2];
   static unsigned char tiltSpeedLUT[(int) EVILIB_max_tspeed_dps * 2];
   static float panSpeed, tiltSpeed;
   static float panPos, tiltPos, lastPanPos, lastTiltPos;
   static int panSpeedMode, tiltSpeedMode;
   int e;

   speedcounter(PTmotors_schema_id);

   if (!initiated) {
   /* los comandos para especificar la velocidad de la cámara no admiten grados
      por segundo, sino "modos" de velocidad. Truncamos al más cercano y 
      guardamos en una LUT */
      unsigned short i;
      for (i = 0; i < (EVILIB_max_pspeed_dps * 2); i++) {
         panSpeedLUT[i] = degreesPerSecond2PanSpeed(i/2.);
      }
      for (i = 0; i < (EVILIB_max_tspeed_dps * 2); i++) {
         tiltSpeedLUT[i] = degreesPerSecond2TiltSpeed(i/2.);
      }
      initiated++;
      lastPanPos = 0;
      lastTiltPos = 0;
      longitude_speed = 70;
      latitude_speed = 150;
      longitude = 0.;
      latitude = 0.;
   }

   panPos = MIN(EVILIB_maxpan, MAX(longitude, EVILIB_minpan) );
   tiltPos = MIN(EVILIB_maxtilt, MAX(latitude, EVILIB_mintilt) );

   panSpeed = longitude_speed;
   tiltSpeed = latitude_speed;

   panSpeedMode = panSpeedLUT[(int) (panSpeed * 2.)];
   tiltSpeedMode = tiltSpeedLUT[(int) (tiltSpeed * 2.)];

   if ( (tiltSpeedMode > 0) && (panSpeedMode > 0) ) {
      if ( (tiltPos != lastTiltPos) || (panPos != lastPanPos) ) {
//          printf("ve a %f %f %d %d \n", panPos, tiltPos, panSpeedMode, tiltSpeedMode);
         e = Pan_TiltDriveCamera(cam,
                                 EVILIB_ABSOLUTE,
                                 panSpeedMode,
                                 tiltSpeedMode,
                                 panPos,
                                 tiltPos,
                                 EVILIB_NO_WAIT_COMP);
         if (e != 1) {
            fprintf(stderr, "evi driver: Transmission interrupted\n");
            evi_close();
         }
         lastPanPos = panPos;
         lastTiltPos = tiltPos;
      }
   }
}

/** PTmotors suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int PTmotors_suspend()
{
   pthread_mutex_lock(&refmutex);
   if (PTmotors_refs > 1) {
      PTmotors_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      PTmotors_refs=0;
      pthread_mutex_unlock(&refmutex);
      if (activate_PTmotors) {
         pthread_mutex_lock(&mymutex_PTmotors);
         state_PTmotors=slept;
         put_state(PTmotors_schema_id,slept);
         printf("PTmotors schema suspend\n");
         pthread_mutex_unlock(&mymutex_PTmotors);
      }
   }
   return 0;
}


/** PTmotors resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int PTmotors_resume(int father, int *brothers, arbitration fn)
{
   pthread_mutex_lock(&refmutex);
   if (PTmotors_refs > 0){
      PTmotors_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      PTmotors_refs=1;
      pthread_mutex_unlock(&refmutex);
      if (activate_PTmotors) {
//          longitude=pan_angle;
//          latitude=tilt_angle;
         pthread_mutex_lock(&mymutex_PTmotors);
         state_PTmotors=winner;
         all[PTmotors_schema_id].father = father;
         all[PTmotors_schema_id].fps = 0.;
         all[PTmotors_schema_id].k =0;
         put_state(PTmotors_schema_id,winner);
         printf("PTmotors schema resume\n");
         pthread_cond_signal(&condition_PTmotors);
         pthread_mutex_unlock(&mymutex_PTmotors);
      }
   }
   return 0;
}

/** PTmotors pthread function.*/
void *PTmotorsThread(void *not_used) {
   struct timeval a,b;
   long diff, next;

   do{
      if (state_PTmotors==slept) {
         pthread_mutex_lock(&mymutex_PTmotors);
         /* printf("PTmotors off\n");*/
         pthread_cond_wait(&condition_PTmotors,&mymutex_PTmotors);
         /* printf("PTmotors on\n");*/
         pthread_mutex_unlock(&mymutex_PTmotors);
      }
      else {
         pthread_mutex_unlock(&mymutex_PTmotors);
         gettimeofday(&a,NULL);
         PTmotors_iteration();
         gettimeofday(&b,NULL);  
         diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
         next = PTmotors_cycle*1000-diff-10000; 
         /* discounts 10ms taken by calling usleep itself */
         if (next>0)
            usleep(PTmotors_cycle*1000-diff);
         else {
            //printf("time interval violated: PTmotors\n");
            usleep(PTmotors_cycle*1000);
         }
      }
   } while(evi_close_command==0);

   pthread_exit(0);
}


/** evi driver parse configuration file function.
 *  @param configfile path and name to the config file.
 *  @return 0 if parsing was successful or -1 if something went wrong.*/
int evi_parseconf(char *configfile){

  int end_parse=0; int end_section=0; int driver_config_parsed=0;
  FILE *myfile;
  const int limit = 256;

  if ((myfile=fopen(configfile,"r"))==NULL){
    printf("evi: cannot find config file\n");
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
	printf ("evi: line too long in config file!\n"); 
	exit(-1);
      }
      
      /* first word of the line */     
      if (sscanf(buffer_file,"%s",word)==1){
	if (strcmp(word,"driver")==0) {
	  while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
	  sscanf(&buffer_file[j],"%s",word2);
	  
	  /* checking if this section matchs our driver name */
	  if (strcmp(word2,"evi")==0){
	    /* the sections match */
	    do{
	      
	      char buffer_file2[256],word3[256],word4[256];
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
		    printf("evi: error in config file.\n'end_section' keyword required before starting new driver section.\n");
		    end_section=1; end_parse=1;

		  }else if(strcmp(word3,"serial")==0){
		    
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s",word3,word4)>1)
		      strcpy(evi_device,word4);		     		 
		    else
		      printf("evi: serial line incorrect\n");
		    
		  }else if(strcmp(word3,"provides")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s",word3,word4)>1){
		      if(strcmp(word4,"pantiltencoders")==0){
			activate_PTencoders=1;
            }else if(strcmp(word4,"pantiltmotors")==0){
         activate_PTmotors=1;
            }else if(strcmp(word4,"zoomencoders")==0){
         activate_Zencoders=1;
            }else if(strcmp(word4,"zoommotors")==0){
         activate_Zmotors=1;
		      }
		    }else{
		      printf("evi: provides line incorrect\n");
		    }
		  }else printf("evi: i don't know what to do with '%s'\n",buffer_file2);
		}
	      }
	    }while(end_section==0);
	    end_section=0; 
	  }
	}
      }
    }
  }while(end_parse==0);
  
  return 0;   
}

void evi_init(){
   int n;

   cam = NewCamera();
   if (cam == NULL) {
      fprintf(stderr, "Cannot create cam\n");
      exit(1);
   }

   if(InitCamera(cam) != 1) {
      fprintf(stderr, "Cannot initiate cam\n");
      exit(1);
   }

   if(OpenCamera(cam,1,evi_device) != 1) {
      fprintf(stderr, "Cannot open serial port\n");
      exit(1);
   }

   n = PowerInqCamera(cam);
   if(n == -1) {
      fprintf(stderr, "Cannot turn on cam\n");
      exit(1);
   }
   else if (n == EVILIB_OFF) {
      if(PowerCamera(cam,EVILIB_ON) != 1) {
         fprintf(stderr, "Cannot turn on cam\n");
         exit(1);
      }
   }

   if(HomeCamera(cam) != 1)
      exit(1);
}

/** evi driver startup function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void evi_startup(char *configfile) {

   /* we call the function to parse the config file */
   if(evi_parseconf(configfile) == -1){
      printf("evi: cannot initiate driver. configfile parsing error.\n");
      exit(-1);
   }
   /* evi initialitation */
   if(evi_setup==0)
      evi_init();

   if(activate_PTencoders) {     
      printf("PTencoders driver started up\n");
      pthread_mutex_lock(&mymutex_PTencoders);
      state_PTencoders=slept;
      pthread_create(&PTencodersTh,NULL,PTencodersThread,NULL);
      pthread_mutex_unlock(&mymutex_PTencoders);

      all[num_schemas].id = (int *) &PTencoders_schema_id;
      strcpy(all[num_schemas].name,"PTencoders");
      all[num_schemas].resume = (resumeFn) PTencoders_resume;
      all[num_schemas].suspend = (suspendFn) PTencoders_suspend;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].close = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("ptencoders", "id", &PTencoders_schema_id);
      myexport("ptencoders", "pan_angle", &longitudePoll);
      myexport("ptencoders", "tilt_angle", &latitudePoll);
      myexport("ptencoders", "clock", &PTencoders_clock);
      myexport("ptencoders", "resume", (void *)&PTencoders_resume);
      myexport("ptencoders", "suspend", (void *)&PTencoders_suspend);
   }

   if (activate_PTmotors) {
      printf("PTmotors driver started up\n");
      pthread_mutex_lock(&mymutex_PTmotors);
      state_PTmotors=slept;
      pthread_create(&PTmotorsTh,NULL,PTmotorsThread,NULL);
      pthread_mutex_unlock(&mymutex_PTmotors);

      all[num_schemas].id = (int *) &PTmotors_schema_id;
      strcpy(all[num_schemas].name,"PTmotors");
      all[num_schemas].resume = (resumeFn) PTmotors_resume;
      all[num_schemas].suspend = (suspendFn) PTmotors_suspend;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].close = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("ptmotors", "id", &PTmotors_schema_id);
      myexport("ptmotors", "longitude", &longitude);
      myexport("ptmotors", "latitude", &latitude);
      myexport("ptmotors", "longitude_speed", &longitude_speed);
      myexport("ptmotors", "latitude_speed", &latitude_speed);
      myexport("ptmotors", "cycle", &PTmotors_cycle);
      myexport("ptmotors", "resume", (void *)&PTmotors_resume);
      myexport("ptmotors", "suspend", (void *)&PTmotors_suspend);
      myexport("ptmotors", "max_longitude", &max_longitude);
      myexport("ptmotors", "max_latitude", &max_latitude);
      myexport("ptmotors", "min_longitude", &min_longitude);
      myexport("ptmotors", "min_latitude", &min_latitude);
      myexport("ptmotors", "max_longitude_speed", &max_longitude_speed);
      myexport("ptmotors", "max_latitude_speed", &max_latitude_speed);
   }

   if(activate_Zencoders) {
      printf("Zencoders driver started up\n");
      pthread_mutex_lock(&mymutex_Zencoders);
      state_Zencoders=slept;
      pthread_create(&ZencodersTh,NULL,ZencodersThread,NULL);
      pthread_mutex_unlock(&mymutex_Zencoders);

      all[num_schemas].id = (int *) &Zencoders_schema_id;
      strcpy(all[num_schemas].name,"Zencoders");
      all[num_schemas].resume = (resumeFn) Zencoders_resume;
      all[num_schemas].suspend = (suspendFn) Zencoders_suspend;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].close = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("zencoders", "id", &Zencoders_schema_id);
      myexport("zencoders", "zoom_position", &zoomPoll);
      myexport("zencoders", "clock", &Zencoders_clock);
      myexport("zencoders", "resume", (void *)&Zencoders_resume);
      myexport("zencoders", "suspend", (void *)&Zencoders_suspend);
   }

   if (activate_Zmotors) {
      printf("Zmotors driver started up\n");
      pthread_mutex_lock(&mymutex_Zmotors);
      state_Zmotors=slept;
      pthread_create(&ZmotorsTh,NULL,ZmotorsThread,NULL);
      pthread_mutex_unlock(&mymutex_Zmotors);

      all[num_schemas].id = (int *) &Zmotors_schema_id;
      strcpy(all[num_schemas].name,"Zmotors");
      all[num_schemas].resume = (resumeFn) Zmotors_resume;
      all[num_schemas].suspend = (suspendFn) Zmotors_suspend;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].close = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("zmotors", "id", &Zmotors_schema_id);
      myexport("zmotors", "zoom", &zoom);
      myexport("zmotors", "zoom_speed", &zoom_speed);
      myexport("zmotors", "cycle", &Zmotors_cycle);
      myexport("zmotors", "resume", (void *)&Zmotors_resume);
      myexport("zmotors", "suspend", (void *)&Zmotors_suspend);
      myexport("zmotors", "max_zoom", &max_zoom);
      myexport("zmotors", "min_zoom", &min_zoom);
      myexport("zmotors", "max_zoom_speed", &max_zoom_speed);
   }

   printf("evi driver started up\n");
}
