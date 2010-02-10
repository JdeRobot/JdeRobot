/*
 *  Copyright (C) 2007 Javier Martin Ramos, Jose Antonio Santos Cadenas
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
 *  Authors :  Javier Martin Ramos <xaverbrennt@yahoo.es>
 *             Jose Antonio Santos Cadenas  <santoscadenas@gmail.com>
 */

/**
 *  jdec mplayer driver provides video images to color variables from static video files with any resolution.
 *
 *  @file mplayer.c
 *  @author Javier Martin Ramos <xaverbrennt@yahoo.es> and Jose Antonio Santos Cadenas  <santoscadenas@gmail.com>
 *  @version 5.0
 *  @date 2007-11-17
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "jde.h"
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


/** Diferent kinds of input*/
enum
{
   /**Input from a video*/
   VIDEO = 0,
   /**Input from a usb camera*/
   V4LCAM,
   /**Input from a tv capturer*/
   V4LTV,
   /**Input from a composite video signal*/
   V4LCOMP,
   /**Input from super-video signal*/
   V4LSVID
};

/** max number of videos used.*/
#define MAXVIDS 8
/** max number of characters in file path and name.*/
#define ROUTE_LEN 255

/** Image standard number of rows*/
#define SIFNTSC_ROWS 240
/** Image standard number of columns*/
#define SIFNTSC_COLUMNS 320

/** mplayer driver instant fps.*/
int mplayer_fps;
/** mplayer driver instant resolution.*/
int mplayer_res;

/** mplayer driver pthread structure for video playing. */
pthread_t mplayer_th[MAXVIDS];
/** pthread state variable.*/
int state;
/** mutex for video playing.*/
pthread_mutex_t mymutex;
/** mutex for pthreads.*/
pthread_mutex_t color_mutex[MAXVIDS];
/** condition flag for video playing.*/
pthread_cond_t condition;

/** mplayer driver variable to detect when threads have been created.*/
int mplayer_thread_created=0;
/** mplayer driver variable to detect when mplayer structures have been cleaned up.*/
int mplayer_cleaned_up=0;
/** mplayer driver variable to detect when the driver has been setup.*/
int mplayer_setup=0;
/** mplayer driver variable to detect when pthreads must end their execution.*/
int mplayer_close_command=0;

/* mplayer driver API options */
/** mplayer driver name.*/
char driver_name[256]="mplayer";
/** mplayer devices detected in config file.*/
int serve_color[MAXVIDS];
/** mutex for ref count*/
pthread_mutex_t refmutex;
/** mplayer video file names.*/
char video_files[MAXVIDS][256];
/** mplayer video devices for captures.*/
char devices[MAXVIDS][256];
/** mplayer number of color activated in gui.*/
int color_active[MAXVIDS];
/** mplayer repeat structure for videos. if repeat[0] = 1 then video 1 will be repeated when finished.*/
int repeat[MAXVIDS];
/** video sizes */
int width[MAXVIDS];
int height[MAXVIDS];
/** Describes de input type*/
int input_type[MAXVIDS];
/** mplayer speed structure for videos.*/
int speed[MAXVIDS];
/** mplayer number of frames per video structure.*/
int n_frames[MAXVIDS];
/** mplayer process id for mplayer threads.*/
int pid_mplayer[MAXVIDS];
/** mplayer process id for mencoder threads.*/
int pid_mencoder[MAXVIDS];
/** mplayer driver structure to swap data between mplayer and mencoder. data from mplayer to mencoder.*/
char fifo1[MAXVIDS][ROUTE_LEN];
/** mplayer driver structure to swap data between mplayer and mencoder. data from mencoder to mplayer.*/
char fifo2[MAXVIDS][ROUTE_LEN];
/** mplayer used diretory name.*/
char directory[255];
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

/*Variables compartidas*/
/** 'colorA' schema image data*/
char *colorA; /* sifntsc image itself */
/** 'colorA' schema clock*/
unsigned long int imageA_clock;

/** 'colorB' schema image data*/
char *colorB; /* sifntsc image itself */
/** 'colorB' schema clock*/
unsigned long int imageB_clock;

/** 'colorC' schema image data*/
char *colorC; /* sifntsc image itself */
/** 'colorC' schema clock*/
unsigned long int imageC_clock;

/** 'colorD' schema image data*/
char *colorD; /* sifntsc image itself */
/** 'colorD' schema clock*/
unsigned long int imageD_clock;

/** 'varcolorA' schema image data*/
char *varcolorA; /* sifntsc image itself */
/** 'varcolorA' schema clock*/
unsigned long int varimageA_clock;

/** 'varcolorB' schema image data*/
char *varcolorB; /* sifntsc image itself */
/** 'varcolorB' schema clock*/
unsigned long int varimageB_clock;

/** 'varcolorC' schema image data*/
char *varcolorC; /* sifntsc image itself */
/** 'varcolorC' schema clock*/
unsigned long int varimageC_clock;

/** 'varcolorD' schema image data*/
char *varcolorD; /* sifntsc image itself */
/** 'varcolorD' schema clock*/
unsigned long int varimageD_clock;


/* MPLAYER DRIVER FUNCTIONS */

/** mplayer driver function to kill every mplayer and mencoder called.*/
void mplayer_close(){
   int i;

   /*Hacer unlink los fifos y matar a los hijos mplayer y  mencoder*/
   for (i=0; i<MAXVIDS; i++){
      if (serve_color[i]==1){
         kill (pid_mplayer[i], 9);
         kill (pid_mencoder[i], 9);

         unlink (fifo1[i]);
         unlink (fifo2[i]);
      }
   }
   /*borrar el directory temporal (si se puede)*/
   if (rmdir (directory)<0){
      perror ("I can't delete temp dir: ");
   }
   mplayer_close_command=1;
   printf("driver mplayer off\n");
}

/** colorA resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorA_resume(int father, int *brothers, arbitration fn){
   if(serve_color[0]==1){
      pthread_mutex_lock(&refmutex);
      color_active[0]++;
      if ((all[colorA_schema_id].father==GUIHUMAN) ||
           (all[colorA_schema_id].father==SHELLHUMAN))
         all[colorA_schema_id].father = father;
      if(color_active[0]==1)
      {
         pthread_mutex_unlock(&refmutex);
         pthread_mutex_unlock(&color_mutex[0]);
         /* printf("colorA schema resume (mplayer driver)\n");*/
         all[colorA_schema_id].father = father;
         all[colorA_schema_id].fps = 0.;
         all[colorA_schema_id].k =0;
         put_state(colorA_schema_id,winner);
      }
      else
         pthread_mutex_unlock(&refmutex);
   }
   return 0;
}

/** colorA suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorA_suspend(){
   pthread_mutex_lock(&refmutex);
   color_active[0]--;
   if((serve_color[0]==1)&&(color_active[0]==0)){
      pthread_mutex_unlock(&refmutex);
      pthread_mutex_lock(&color_mutex[0]);
      /*printf("colorA schema suspend (mplayer driver)\n");*/
      put_state(colorA_schema_id,slept);
   }
   else
      pthread_mutex_unlock(&refmutex);
   return 0;
}

/** colorB resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorB_resume(int father, int *brothers, arbitration fn){
   if(serve_color[1]==1)
   {
      pthread_mutex_lock(&refmutex);
      color_active[1]++;
      if ((all[colorB_schema_id].father==GUIHUMAN) ||
           (all[colorB_schema_id].father==SHELLHUMAN))
         all[colorB_schema_id].father = father;
      if(color_active[1]==1)
      {
         pthread_mutex_unlock(&refmutex);
         pthread_mutex_unlock(&color_mutex[1]);
	 /* printf("colorB schema resume (mplayer driver)\n");*/
         all[colorB_schema_id].father = father;
         all[colorB_schema_id].fps = 0.;
         all[colorB_schema_id].k =0;
         put_state(colorB_schema_id,winner);
      }
      else
         pthread_mutex_unlock(&refmutex);
   }
   return 0;
}

/** colorB suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorB_suspend(){
   pthread_mutex_lock(&refmutex);
   color_active[1]--;
    if((serve_color[1]==1)&&(color_active[1]==0)){
      pthread_mutex_unlock(&refmutex);
      pthread_mutex_lock(&color_mutex[1]);
      /* printf("colorB schema suspend (mplayer driver)\n");*/
      put_state(colorB_schema_id,slept);
   }
   else
      pthread_mutex_unlock(&refmutex);
   return 0;
}

/** colorC resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorC_resume(int father, int *brothers, arbitration fn){
   if(serve_color[2]==1)
   {
      pthread_mutex_lock(&refmutex);
      color_active[2]++;
      if ((all[colorC_schema_id].father==GUIHUMAN) ||
           (all[colorC_schema_id].father==SHELLHUMAN))
         all[colorC_schema_id].father = father;
      if(color_active[2]==1)
      {
         pthread_mutex_unlock(&refmutex);
         pthread_mutex_unlock(&color_mutex[2]);
	 /* printf("colorC schema resume (mplayer driver)\n");*/
         all[colorC_schema_id].father = father;
         all[colorC_schema_id].fps = 0.;
         all[colorC_schema_id].k =0;
         put_state(colorC_schema_id,winner);
      }
      else
         pthread_mutex_unlock(&refmutex);
   }
   return 0;
}

/** colorC suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorC_suspend(){
   pthread_mutex_lock(&refmutex);
   color_active[2]--;
   if((serve_color[2]==1)&&(color_active[2]==0)){
      pthread_mutex_unlock(&refmutex);
      pthread_mutex_lock(&color_mutex[2]);
      /* printf("colorC schema suspend (mplayer driver)\n"); */
      put_state(colorC_schema_id,slept);
   }
   else
      pthread_mutex_unlock(&refmutex);
   return 0;
}

/** colorD resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorD_resume(int father, int *brothers, arbitration fn){

   if(serve_color[3]==1)
   {
      pthread_mutex_lock(&refmutex);
      color_active[3]++;
      if ((all[colorD_schema_id].father==GUIHUMAN) ||
           (all[colorD_schema_id].father==SHELLHUMAN))
         all[colorD_schema_id].father = father;
      if(color_active[3]==1)
      {
         pthread_mutex_unlock(&refmutex);
         pthread_mutex_unlock(&color_mutex[3]);
	 /* printf("colorD schema resume (mplayer driver)\n");*/
         all[colorD_schema_id].father = father;
         all[colorD_schema_id].fps = 0.;
         all[colorD_schema_id].k =0;
         put_state(colorD_schema_id,winner);
      }
      else
         pthread_mutex_unlock(&refmutex);
   }
   return 0;
}

/** colorD suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorD_suspend(){
   pthread_mutex_lock(&refmutex);
   color_active[3]--;
   if((serve_color[3]==1)&&(color_active[3]==0)){
      pthread_mutex_unlock(&refmutex);
      pthread_mutex_lock(&color_mutex[3]);
      /* printf("colorD schema suspend (mplayer driver)\n"); */
      put_state(colorD_schema_id,slept);
   }
   else
      pthread_mutex_unlock(&refmutex);
   return 0;
}

/** varcolorA resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorA_resume(int father, int *brothers, arbitration fn){
   if(serve_color[4]==1){
      pthread_mutex_lock(&refmutex);
      color_active[4]++;
      if ((all[varcolorA_schema_id].father==GUIHUMAN) ||
           (all[varcolorA_schema_id].father==SHELLHUMAN))
         all[varcolorA_schema_id].father = father;
      if(color_active[4]==1)
      {
         pthread_mutex_unlock(&refmutex);
         pthread_mutex_unlock(&color_mutex[4]);
         /*printf("varcolorA schema resume (mplayer driver)\n");*/
         all[varcolorA_schema_id].father = father;
         all[varcolorA_schema_id].fps = 0.;
         all[varcolorA_schema_id].k =0;
         put_state(varcolorA_schema_id,winner);
      }
      else
         pthread_mutex_unlock(&refmutex);
   }
   return 0;
}

/** varcolorA suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int myvarcolorA_suspend(){
   pthread_mutex_lock(&refmutex);
   color_active[4]--;
   if((serve_color[4]==1)&&(color_active[4]==0)){
      pthread_mutex_unlock(&refmutex);
      pthread_mutex_lock(&color_mutex[4]);
      put_state(varcolorA_schema_id,slept);
      /*printf("varcolorA schema suspend (mplayer driver)\n");*/
   }
   else
      pthread_mutex_unlock(&refmutex);
   return 0;
}

/** varcolorB resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorB_resume(int father, int *brothers, arbitration fn){
   if(serve_color[5]==1){
      pthread_mutex_lock(&refmutex);
      color_active[5]++;
      if ((all[varcolorB_schema_id].father==GUIHUMAN) ||
           (all[varcolorB_schema_id].father==SHELLHUMAN))
         all[varcolorB_schema_id].father = father;
      if(color_active[5]==1)
      {
         pthread_mutex_unlock(&refmutex);
         pthread_mutex_unlock(&color_mutex[5]);
         /*printf("varcolorB schema resume (mplayer driver)\n");*/
         all[varcolorB_schema_id].father = father;
         all[varcolorB_schema_id].fps = 0.;
         all[varcolorB_schema_id].k =0;
         put_state(varcolorB_schema_id,winner);
      }
      else
         pthread_mutex_unlock(&refmutex);
   }
   return 0;
}

/** varcolorB suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int myvarcolorB_suspend(){
   pthread_mutex_lock(&refmutex);
   color_active[5]--;
   if((serve_color[5]==1)&&(color_active[5]==0)){
      pthread_mutex_unlock(&refmutex);
      pthread_mutex_lock(&color_mutex[5]);
      put_state(varcolorB_schema_id,slept);
      /*printf("varcolorB schema suspend (mplayer driver)\n");*/
   }
   else
      pthread_mutex_unlock(&refmutex);
   return 0;
}

/** varcolorC resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorC_resume(int father, int *brothers, arbitration fn){
   if(serve_color[6]==1){
      pthread_mutex_lock(&refmutex);
      color_active[6]++;
      if ((all[varcolorC_schema_id].father==GUIHUMAN) ||
           (all[varcolorC_schema_id].father==SHELLHUMAN))
         all[varcolorC_schema_id].father = father;
      if(color_active[6]==1)
      {
         pthread_mutex_unlock(&refmutex);
         pthread_mutex_unlock(&color_mutex[6]);
         /*printf("varcolorC schema resume (mplayer driver)\n");*/
         all[varcolorC_schema_id].father = father;
         all[varcolorC_schema_id].fps = 0.;
         all[varcolorC_schema_id].k =0;
         put_state(varcolorC_schema_id,winner);
      }
      else
         pthread_mutex_unlock(&refmutex);
   }
   return 0;
}

/** varcolorC suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int myvarcolorC_suspend(){
   pthread_mutex_lock(&refmutex);
   color_active[6]--;
   if((serve_color[6]==1)&&(color_active[6]==0)){
      pthread_mutex_unlock(&refmutex);
      pthread_mutex_lock(&color_mutex[6]);
      put_state(varcolorC_schema_id,slept);
      /*printf("varcolorC schema suspend (mplayer driver)\n");*/
   }
   else
      pthread_mutex_unlock(&refmutex);
   return 0;
}

/** varcolorD resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorD_resume(int father, int *brothers, arbitration fn){
   if(serve_color[7]==1){
      pthread_mutex_lock(&refmutex);
      color_active[7]++;
      if ((all[varcolorD_schema_id].father==GUIHUMAN) ||
           (all[varcolorD_schema_id].father==SHELLHUMAN))
         all[varcolorD_schema_id].father = father;
      if(color_active[7]==1)
      {
         pthread_mutex_unlock(&refmutex);
         pthread_mutex_unlock(&color_mutex[7]);
         /*printf("varcolorD schema resume (mplayer driver)\n");*/
         all[varcolorD_schema_id].father = father;
         all[varcolorD_schema_id].fps = 0.;
         all[varcolorD_schema_id].k =0;
         put_state(varcolorD_schema_id,winner);
      }
      else
         pthread_mutex_unlock(&refmutex);
   }
   return 0;
}

/** varcolorD suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int myvarcolorD_suspend(){
   pthread_mutex_lock(&refmutex);
   color_active[7]--;
   if((serve_color[7]==1)&&(color_active[7]==0)){
      pthread_mutex_unlock(&refmutex);
      pthread_mutex_lock(&color_mutex[7]);
      put_state(varcolorD_schema_id,slept);
      /*printf("varcolorD schema suspend (mplayer driver)\n");*/
   }
   else
      pthread_mutex_unlock(&refmutex);
   return 0;
}

/** mplayer driver function to create 2 fifos required to capture images using mplayer+mencoder
 *  @param i selected color to launch a mplayer thread.*/
void mplayer_fifocreate(int i){
   umask (0000);
   unlink (fifo1[i]);
   unlink (fifo2[i]);
   if ( (mkfifo (fifo1[i], 0600) != 0) ||
	(mkfifo (fifo2[i], 0600) != 0) )
     jdeshutdown (1);
}

/** mplayer driver function to start a mplayer process for a selected color.
 *  @param i selected color to launch a mplayer thread.*/
void mplayer_start(int i){
   int file;
   char str[100];
   char str2[100];
   char str3[100];

   if (pid_mplayer[i]!=-1){
      kill (pid_mplayer[i], 9);
      wait (NULL);
   }
   if (pid_mencoder[i]!=-1){
      kill (pid_mencoder[i], 9);
      wait (NULL);
   }
  
   if ((pid_mencoder[i]=fork()) == 0) {/* We create a new process...
      // ... close its stdin, stdout & stderr ...*/
      char cache_size[20];
      snprintf(cache_size, 20, "%d", (int)((width[i]*height[i]*4)/1024));

      file = open("/dev/null",O_RDWR);
      close(0); dup(file);
      close(1); dup(file);
      close(2); dup(file);

      execlp("mencoder","mencoder",fifo1[i],"-nosound", "-cache", cache_size, "-o",
             fifo2[i], "-ovc","raw","-of","rawvideo","-vf","format=bgr24",NULL);
      printf("Error executing mencoder\n");
      exit(1);
   }

   if ((pid_mplayer[i]=fork()) == 0) { /* We create a new process...
      // ... close its stdin, stdout & stderr ...*/
      file = open("/dev/null",O_RDWR);
      close(0); dup(file);
      close(1); dup(file);
      close(2); dup(file);
      /* ... and exec the mplayer command.*/
      sprintf(str,"scale=%d:%d", width[i],height[i]);
      sprintf (str2, "yuv4mpeg:file=%s", fifo1[i]);
      switch (input_type[i]){
         case VIDEO:
            execlp("mplayer","mplayer",video_files[i],"-vo", str2,
                   "-vf", str, "-ao","null","-slave",NULL);
            printf("Error executing mplayer\n");
            exit(1);
            break;
         case V4LCAM:
            sprintf(str3, "driver=v4l:width=%d:height=%d:device=%s",
                    width[i], height[i], devices[i]);
            execlp("mplayer","mplayer","tv://", "-tv", str3 ,"-vo", str2,
                   "-vf", str, "-ao","null","-slave",NULL);
            printf("Error executing mplayer\n");
            exit(1);
            break;
         case V4LTV:
            sprintf(str3, "driver=v4l:width=%d:height=%d:device=%s:input=0",
                    width[i], height[i], devices[i]);
            execlp("mplayer","mplayer","tv://", "-tv", str3 ,"-vo", str2,
                   "-vf", str, "-ao","null","-slave",NULL);
            printf("Error executing mplayer\n");
            exit(1);
            break;
         case V4LCOMP:
            sprintf(str3, "driver=v4l:width=%d:height=%d:device=%s:input=1",
                    width[i], height[i], devices[i]);
            execlp("mplayer","mplayer","tv://", "-tv", str3 ,"-vo", str2,
                   "-vf", str, "-ao","null","-slave",NULL);
            printf("Error executing mplayer\n");
            exit(1);
            break;
         case V4LSVID:
            sprintf(str3, "driver=v4l:width=%d:height=%d:device=%s:input=2",
                    width[i], height[i], devices[i]);
            execlp("mplayer","mplayer","tv://", "-tv", str3 ,"-vo", str2,
                   "-vf", str, "-ao","null","-slave",NULL);
            printf("Error executing mplayer\n");
            exit(1);
            break;
      }
   }
}

/** mplayer driver internal thread.
 *  @param id selected color id.*/
void *mplayer_thread(void *id){
   int i;
   i=*((int*)id);
   int fifo;
   static unsigned long int lastimage=0;
   char *buff = NULL;

   printf("mplayer driver started up\n");
   if ((fifo=open (fifo2[i],O_RDONLY))==-1){
      perror("Error al abrir el fifo");
      exit(1);
   }
   buff = (char *) malloc(width[i]*height[i]*3);
   do{

      int leidos=0;
      int leidos_tmp=0;

      pthread_mutex_lock(&color_mutex[i]); /*Si está bloqueado se queda ahí*/
      pthread_mutex_unlock (&color_mutex[i]);
      /*Simplemente lee del fifo y lo coloca en colorX*/
      while (leidos<width[i]*height[i]*3){
         leidos_tmp=read (fifo, buff+leidos,
                          width[i]*height[i]*3 - leidos);
         if (leidos_tmp==0){
            if (repeat[i]==1){
	       /*Lanzar otro mplayer si hay repeat
               Se matan mplayer y mencoder, se ignoran errores porque
               Seguramente ya hayan muerto y eso devuelve error*/
               close(fifo);

	       mplayer_fifocreate(i);
               mplayer_start(i);
               if ((fifo=open (fifo2[i],O_RDONLY))==-1){
                  perror("");
                  exit(1);
               }
               leidos=0;
               leidos_tmp=0;
            }
            else{
               break;
            }

         }
         leidos+=leidos_tmp;
      }
      /*Se ha leido todo un frame*/
      switch (i){
         case 0:
            memcpy(colorA, buff, width[i]*height[i]*3);
            speedcounter(colorA_schema_id);
            imageA_clock=lastimage;
            break;
         case 1:
            memcpy(colorB, buff, SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
            speedcounter(colorB_schema_id);
            imageB_clock=lastimage;
            break;
         case 2:
            memcpy(colorC, buff, SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
            speedcounter(colorC_schema_id);
            imageC_clock=lastimage;
            break;
         case 3:
            memcpy(colorD, buff, SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
            speedcounter(colorD_schema_id);
            imageD_clock=lastimage;
            break;
         case 4:
            memcpy(varcolorA, buff, width[i]*height[i]*3);
            speedcounter(varcolorA_schema_id);
            varimageA_clock=lastimage;
            break;
         case 5:
            memcpy(varcolorB, buff, width[i]*height[i]*3);
            speedcounter(varcolorB_schema_id);
            varimageB_clock=lastimage;
            break;
         case 6:
            memcpy(varcolorC, buff, width[i]*height[i]*3);
            speedcounter(varcolorC_schema_id);
            varimageC_clock=lastimage;
            break;
         case 7:
            memcpy(varcolorD, buff, width[i]*height[i]*3);
            speedcounter(varcolorD_schema_id);
            varimageD_clock=lastimage;
            break;
         default:
            fprintf(stderr, "mplayer_thread: Unknown option %d.\n", i);
      }

      lastimage++;

   }while(mplayer_close_command==0);
   close(fifo);
   pthread_exit(0);
}

/** mplayer driver parse configuration file function.
 *  @param configfile path and name to the config file.
 *  @return 0 if parsing was successful or -1 if something went wrong.*/
int mplayer_parseconf(char *configfile){

   int end_parse=0; int end_section=0; int driver_config_parsed=0;
   FILE *myfile;
   const int limit = 256;

   if ((myfile=fopen(configfile,"r"))==NULL){
      printf("mplayer: cannot find config file\n");
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
            printf ("mplayer: line too long in config file!\n");
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
	      
                     char buffer_file2[256],word3[256],word4[256],word5[256];
                     char word6[256],word7[256],word8[256],word9[256];
                     int k=0; int z=0;
                     int words=0;

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
                              printf("mplayer: error in config file.\n'end_section' keyword required before starting new driver section.\n");
                              end_section=1; end_parse=1;

                           }else if(strcmp(word3,"provides")==0){
                              while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
                              /*printf ("%s",buffer_file2);*/
                              words=sscanf(buffer_file2,"%s %s %s %s %s %s %s",
                                           word3,word4,word5,word6,word7,word8,
                                           word9);
			      if (words==3) {
				printf("mplayer: %s from %s\n",word4,word5);
                                 if(strcmp(word4,"colorA")==0){
                                    serve_color[0]=1;
                                    strcpy (video_files[0],word5);
                                    width[0] = SIFNTSC_COLUMNS;
                                    height[0] = SIFNTSC_ROWS;
                                    input_type[0]=VIDEO;
				 }
				 else if(strcmp(word4,"colorB")==0){
                                    serve_color[1]=1;
                                    strcpy (video_files[1],word5);
                                    width[1] = SIFNTSC_COLUMNS;
                                    height[1] = SIFNTSC_ROWS;
                                    input_type[1]=VIDEO;
				 }
				 else if(strcmp(word4,"colorC")==0){
				   serve_color[2]=1;
				   strcpy (video_files[2],word5);
				   width[2] = SIFNTSC_COLUMNS;
				   height[2] = SIFNTSC_ROWS;
				   input_type[2]=VIDEO;
				 }
				 else if(strcmp(word4,"colorD")==0){
				   serve_color[3]=1;
				   strcpy (video_files[3],word5);
				   width[3] = SIFNTSC_COLUMNS;
				   height[3] = SIFNTSC_ROWS;
				   input_type[3]=VIDEO;
				 }
			      }
                              else if(words==4){
                                 printf("mplayer: %s from %s\n",word4,word5);
                                 if(strcmp(word4,"colorA")==0){
                                    serve_color[0]=1;
                                    strcpy (video_files[0],word5);
                                    width[0] = SIFNTSC_COLUMNS;
                                    height[0] = SIFNTSC_ROWS;
                                    input_type[0]=VIDEO;
                                    if(strcmp(word6,"repeat_on")==0)
                                       repeat[0] = 1;
                                    else
                                       repeat[0] = 0;

                                 }else if(strcmp(word4,"colorB")==0){
                                    serve_color[1]=1;
                                    strcpy (video_files[1],word5);
                                    width[1] = SIFNTSC_COLUMNS;
                                    height[1] = SIFNTSC_ROWS;
                                    input_type[1]=VIDEO;
                                    if(strcmp(word6,"repeat_on")==0)
                                       repeat[1] = 1;
                                    else
                                       repeat[1] = 0;

                                 }else if(strcmp(word4,"colorC")==0){
                                    serve_color[2]=1;
                                    strcpy (video_files[2],word5);
                                    width[2] = SIFNTSC_COLUMNS;
                                    height[2] = SIFNTSC_ROWS;
                                    input_type[2]=VIDEO;
                                    if(strcmp(word6,"repeat_on")==0)
                                       repeat[2] = 1;
                                    else
                                       repeat[2] = 0;

                                 }else if(strcmp(word4,"colorD")==0){
                                    serve_color[3]=1;
                                    strcpy (video_files[3],word5);
                                    width[3] = SIFNTSC_COLUMNS;
                                    height[3] = SIFNTSC_ROWS;
                                    input_type[3]=VIDEO;
                                    if(strcmp(word6,"repeat_on")==0)
                                       repeat[3] = 1;
                                    else
                                       repeat[3] = 0;
                                 }
                              }
                              else if((words==5) && (strcmp(word5,"v4l")==0)){
                                 printf("mplayer: %s from device %s\n",word4,word6);
                                 if(strcmp(word4,"colorA")==0){
                                    serve_color[0]=1;
                                    strcpy (devices[0],word6);
                                    width[0] = SIFNTSC_COLUMNS;
                                    height[0] = SIFNTSC_ROWS;
                                    if (strcmp(word7,"cam")==0){
                                       input_type[0]=V4LCAM;
                                    }
                                    else if (strcmp(word7,"tv")==0){
                                       input_type[0]=V4LTV;
                                    }
                                    else if (strcmp(word7,"composite")==0){
                                       input_type[0]=V4LCOMP;
                                    }
                                    else if (strcmp(word7,"s-video")==0){
                                       input_type[0]=V4LSVID;
                                    }
                                 }else if(strcmp(word4,"colorB")==0){
                                    serve_color[1]=1;
                                    strcpy (devices[1],word6);
                                    width[1] = SIFNTSC_COLUMNS;
                                    height[1] = SIFNTSC_ROWS;
                                    if (strcmp(word7,"cam")==0){
                                       input_type[1]=V4LCAM;
                                    }
                                    else if (strcmp(word7,"tv")==0){
                                       input_type[1]=V4LTV;
                                    }
                                    else if (strcmp(word7,"composite")==0){
                                       input_type[1]=V4LCOMP;
                                    }
                                    else if (strcmp(word7,"s-video")==0){
                                       input_type[1]=V4LSVID;
                                    }
                                 }else if(strcmp(word4,"colorC")==0){
                                    serve_color[2]=1;
                                    strcpy (devices[2],word6);
                                    width[2] = SIFNTSC_COLUMNS;
                                    height[2] = SIFNTSC_ROWS;
                                    if (strcmp(word7,"cam")==0){
                                       input_type[2]=V4LCAM;
                                    }
                                    else if (strcmp(word7,"tv")==0){
                                       input_type[2]=V4LTV;
                                    }
                                    else if (strcmp(word7,"composite")==0){
                                       input_type[2]=V4LCOMP;
                                    }
                                    else if (strcmp(word7,"s-video")==0){
                                       input_type[2]=V4LSVID;
                                    }
                                 }else if(strcmp(word4,"colorD")==0){
                                    serve_color[3]=1;
                                    strcpy (devices[3],word6);
                                    width[3] = SIFNTSC_COLUMNS;
                                    height[3] = SIFNTSC_ROWS;
                                    if (strcmp(word7,"cam")==0){
                                       input_type[3]=V4LCAM;
                                    }
                                    else if (strcmp(word7,"tv")==0){
                                       input_type[3]=V4LTV;
                                    }
                                    else if (strcmp(word7,"composite")==0){
                                       input_type[3]=V4LCOMP;
                                    }
                                    else if (strcmp(word7,"s-video")==0){
                                       input_type[3]=V4LSVID;
                                    }
                                 }
                              }

                              else if (words==5){
                                 printf("mplayer: %s from %s\n",word4,word5);
                                 if(strcmp(word4,"varcolorA")==0){
                                    serve_color[4]=1;
                                    strcpy (video_files[4],word5);
                                    width[4] = atoi(word6);
                                    height[4] = atoi(word7);
                                    input_type[4]=VIDEO;
                                 }else if(strcmp(word4,"varcolorB")==0){
                                    serve_color[5]=1;
                                    strcpy (video_files[5],word5);
                                    width[5] = atoi(word6);
                                    height[5] = atoi(word7);
                                    input_type[5]=VIDEO;
				 }else if(strcmp(word4,"varcolorC")==0){
                                    serve_color[6]=1;
                                    strcpy (video_files[6],word5);
                                    width[6] = atoi(word6);
                                    height[6] = atoi(word7);
                                    input_type[6]=VIDEO;
				 }else if(strcmp(word4,"varcolorD")==0){
                                    serve_color[7]=1;
                                    strcpy (video_files[7],word5);
                                    width[7] = atoi(word6);
                                    height[7] = atoi(word7);
                                    input_type[7]=VIDEO;
				 }
			      }
                              else if (words==6){
                                 printf("mplayer: %s from %s\n",word4,word5);
                                 if(strcmp(word4,"varcolorA")==0){
                                    serve_color[4]=1;
                                    strcpy (video_files[4],word5);
                                    width[4] = atoi(word6);
                                    height[4] = atoi(word7);
                                    input_type[4]=VIDEO;
                                    if(strcmp(word8,"repeat_on")==0)
                                       repeat[4] = 1;
                                    else
                                       repeat[4] = 0;

                                 }else if(strcmp(word4,"varcolorB")==0){
                                    serve_color[5]=1;
                                    strcpy (video_files[5],word5);
                                    width[5] = atoi(word6);
                                    height[5] = atoi(word7);
                                    input_type[5]=VIDEO;
                                    if(strcmp(word8,"repeat_on")==0)
                                       repeat[5] = 1;
                                    else
                                       repeat[5] = 0;

                                 }else if(strcmp(word4,"varcolorC")==0){
                                    serve_color[6]=1;
                                    strcpy (video_files[6],word5);
                                    width[6] = atoi(word6);
                                    height[6] = atoi(word7);
                                    input_type[6]=VIDEO;
                                    if(strcmp(word8,"repeat_on")==0)
                                       repeat[6] = 1;
                                    else
                                       repeat[6] = 0;

                                 }else if(strcmp(word4,"varcolorD")==0){
                                    serve_color[7]=1;
                                    strcpy (video_files[7],word5);
                                    width[7] = atoi(word6);
                                    height[7] = atoi(word7);
                                    input_type[7]=VIDEO;
                                    if(strcmp(word8,"repeat_on")==0)
                                       repeat[7] = 1;
                                    else
                                       repeat[7] = 0;
                                 }

                              }
                              else if ((words==7) && (strcmp(word5,"v4l")==0)){
                                 printf("mplayer: %s from device %s\n",word4,word6);
                                 if(strcmp(word4,"varcolorA")==0){
                                    serve_color[4]=1;
                                    strcpy (devices[4],word6);
                                    width[4] = atoi(word7);
                                    height[4] = atoi(word8);
                                    if (strcmp(word9,"cam")==0){
                                       input_type[4]=V4LCAM;
                                    }
                                    else if (strcmp(word9,"tv")==0){
                                       input_type[4]=V4LTV;
                                    }
                                    else if (strcmp(word9,"composite")==0){
                                       input_type[4]=V4LCOMP;
                                    }
                                    else if (strcmp(word9,"s-video")==0){
                                       input_type[4]=V4LSVID;
                                    }
                                 }
                                 else if(strcmp(word4,"varcolorB")==0){
                                    serve_color[5]=1;
                                    strcpy (devices[5],word6);
                                    width[5] = atoi(word7);
                                    height[5] = atoi(word8);
                                    if (strcmp(word9,"cam")==0){
                                       input_type[5]=V4LCAM;
                                    }
                                    else if (strcmp(word9,"tv")==0){
                                       input_type[5]=V4LTV;
                                    }
                                    else if (strcmp(word9,"composite")==0){
                                       input_type[5]=V4LCOMP;
                                    }
                                    else if (strcmp(word9,"s-video")==0){
                                       input_type[5]=V4LSVID;
                                    }
                                 }
                                 else if(strcmp(word4,"varcolorC")==0){
                                    serve_color[6]=1;
                                    strcpy (devices[6],word6);
                                    width[6] = atoi(word7);
                                    height[6] = atoi(word8);
                                    if (strcmp(word9,"cam")==0){
                                       input_type[6]=V4LCAM;
                                    }
                                    else if (strcmp(word9,"tv")==0){
                                       input_type[6]=V4LTV;
                                    }
                                    else if (strcmp(word9,"composite")==0){
                                       input_type[6]=V4LCOMP;
                                    }
                                    else if (strcmp(word9,"s-video")==0){
                                       input_type[6]=V4LSVID;
                                    }
                                 }
                                 else if(strcmp(word4,"varcolorD")==0){
                                    serve_color[7]=1;
                                    strcpy (devices[7],word6);
                                    width[7] = atoi(word7);
                                    height[7] = atoi(word8);
                                    if (strcmp(word9,"cam")==0){
                                       input_type[7]=V4LCAM;
                                    }
                                    else if (strcmp(word9,"tv")==0){
                                       input_type[7]=V4LTV;
                                    }
                                    else if (strcmp(word9,"composite")==0){
                                       input_type[7]=V4LCOMP;
                                    }
                                    else if (strcmp(word9,"s-video")==0){
                                       input_type[7]=V4LSVID;
                                    }
                                 }
                              }
                              else{
                                 printf("mplayer: 'provides' line incorrect\n");
                              }
                           }else printf("mplayer: i don't know what to do with '%s'\n",buffer_file2);
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
      if((serve_color[0]==0)&&(serve_color[1]==0)&&(serve_color[2]==0)&&(serve_color[3]==0)&&(serve_color[4]==0)&&(serve_color[5]==0)&&(serve_color[6]==0)&&(serve_color[7]==0)){
         printf("mplayer: warning! no color provided.\n");
      }
      return 0;
   }else return -1;
}

/** mplayer driver startup function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void mplayer_startup(char *configfile)
{
   int i;

   /* reseting serve color array and setting default options */
   for(i=0;i<MAXVIDS;i++){
      serve_color[i]=0;
      color_active[i]=0;
   }

  /* we call the function to parse the config file */
   if(mplayer_parseconf(configfile)==-1){
      printf("mplayer: cannot initiate driver. configfile parsing error.\n");
      exit(-1);
   }


   strcpy (directory, "/tmp/jde-mplayer-XXXXXX");
   if (mkdtemp(directory)==NULL){
      perror ("I can't create a temp directory: ");
      exit (-1);
   }

   /*inicializar los nombres de los fifos*/
   if (snprintf(fifo1[0], ROUTE_LEN, "%s/colorA-1", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }
   if (snprintf(fifo2[0], ROUTE_LEN, "%s/colorA-2", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }
   if (snprintf(fifo1[1], ROUTE_LEN, "%s/colorB-1", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }
   if (snprintf(fifo2[1], ROUTE_LEN, "%s/colorB-2", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }
   if (snprintf(fifo1[2], ROUTE_LEN, "%s/colorC-1", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }
   if (snprintf(fifo2[2], ROUTE_LEN, "%s/colorC-2", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }
   if (snprintf(fifo1[3], ROUTE_LEN, "%s/colorD-1", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }
   if (snprintf(fifo2[3], ROUTE_LEN, "%s/colorD-2", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }

   if (snprintf(fifo1[4], ROUTE_LEN, "%s/varcolorA-1", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }
   if (snprintf(fifo2[4], ROUTE_LEN, "%s/varcolorA-2", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }
   if (snprintf(fifo1[5], ROUTE_LEN, "%s/varcolorB-1", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }
   if (snprintf(fifo2[5], ROUTE_LEN, "%s/varcolorB-2", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }
   if (snprintf(fifo1[6], ROUTE_LEN, "%s/varcolorC-1", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }
   if (snprintf(fifo2[6], ROUTE_LEN, "%s/varcolorC-2", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }
   if (snprintf(fifo1[7], ROUTE_LEN, "%s/varcolorD-1", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }
   if (snprintf(fifo2[7], ROUTE_LEN, "%s/varcolorD-2", directory)<0){
      fprintf (stderr, "Can't create temp files\n");
      exit (-1);
   }

  for (i=0; i<MAXVIDS; i++){
      /*inicializar todos los fifos */
      if (serve_color[i]==1){
         mplayer_fifocreate(i);
      }
   }

    
   if(mplayer_thread_created==0){
      static int args[MAXVIDS];
      /*Crear hilos para cada imagen*/
      pthread_mutex_lock(&mymutex);
      state=slept;
      if (serve_color[0]){
         args[0]=0;
         colorA = (char *) malloc(width[0]*height[0]*3);
         pthread_create(&mplayer_th[0],NULL,mplayer_thread,(void*)&args[0]);
      }
      if (serve_color[1]){
         args[1]=1;
         colorB = (char *) malloc(width[1]*height[1]*3);
         pthread_create(&mplayer_th[1],NULL,mplayer_thread,(void*)&args[1]);
      }
      if (serve_color[2]){
         args[2]=2;
         colorC = (char *) malloc(width[2]*height[2]*3);
         pthread_create(&mplayer_th[2],NULL,mplayer_thread,(void*)&args[2]);
      }
      if (serve_color[3]){
         args[3]=3;
         colorD = (char *) malloc(width[3]*height[3]*3);
         pthread_create(&mplayer_th[3],NULL,mplayer_thread,(void*)&args[3]);
      }
     if (serve_color[4]){
        args[4]=4;
        varcolorA = (char *) malloc(width[4]*height[4]*3);
        pthread_create(&mplayer_th[4],NULL,mplayer_thread,(void*)&args[4]);
      }
      if (serve_color[5]){
         args[5]=5;
         varcolorB = (char *) malloc(width[5]*height[5]*3);
         pthread_create(&mplayer_th[5],NULL,mplayer_thread,(void*)&args[5]);
      }
      if (serve_color[6]){
         args[6]=6;
         varcolorC = (char *) malloc(width[6]*height[6]*3);
         pthread_create(&mplayer_th[6],NULL,mplayer_thread,(void*)&args[6]);
      }
      if (serve_color[7]){
         args[7]=7;
         varcolorD = (char *) malloc(width[7]*height[7]*3);
         pthread_create(&mplayer_th[7],NULL,mplayer_thread,(void*)&args[7]);
      }

      mplayer_thread_created=1;
      pthread_mutex_unlock(&mymutex);
   }
   /*Se crean los esquemas que a su vez bloquean a su correspondiente thread
   hasta que se active el esquema*/

   /*creates new schema for colorA*/
   if(serve_color[0]==1){
         pthread_mutex_lock(&color_mutex[0]);
         all[num_schemas].id = (int *) &colorA_schema_id;
         strcpy(all[num_schemas].name,"colorA");
         all[num_schemas].resume = (resumeFn) mycolorA_resume;
         all[num_schemas].suspend = (suspendFn) mycolorA_suspend;
         printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
         (*(all[num_schemas].id)) = num_schemas;
         all[num_schemas].fps = 0.;
         all[num_schemas].k =0;
         all[num_schemas].state=slept;
         all[num_schemas].close = NULL;
         all[num_schemas].handle = NULL;
         num_schemas++;
         myexport("colorA","id",&colorA_schema_id);
         myexport("colorA","colorA",&colorA);
         myexport("colorA","clock", &imageA_clock);
         myexport("colorA","resume",(void *)mycolorA_resume);
         myexport("colorA","suspend",(void *)mycolorA_suspend);
         myexport("colorA","width",&width[0]);
         myexport("colorA","height",&height[0]);
	 mycolorA_resume(SHELLHUMAN,NULL,NULL);
   }

   /*creates new schema for colorB*/
   if(serve_color[1]==1){
         pthread_mutex_lock(&color_mutex[1]);
         all[num_schemas].id = (int *) &colorB_schema_id;
         strcpy(all[num_schemas].name,"colorB");
         all[num_schemas].resume = (resumeFn) mycolorB_resume;
         all[num_schemas].suspend = (suspendFn) mycolorB_suspend;
         printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
         (*(all[num_schemas].id)) = num_schemas;
         all[num_schemas].fps = 0.;
         all[num_schemas].k =0;
         all[num_schemas].state=slept;
         all[num_schemas].close = NULL;
         all[num_schemas].handle = NULL;
         num_schemas++;
         myexport("colorB","id",&colorB_schema_id);
         myexport("colorB","colorB",&colorB);
         myexport("colorB","clock", &imageB_clock);
         myexport("colorB","resume",(void *)mycolorB_resume);
         myexport("colorB","suspend",(void *)mycolorB_suspend);
         myexport("colorB","width",&width[1]);
         myexport("colorB","height",&height[1]);
	 mycolorB_resume(SHELLHUMAN,NULL,NULL);
   }
  
   /*creates new schema for colorC*/
   if(serve_color[2]==1){
         pthread_mutex_lock(&color_mutex[2]);
         all[num_schemas].id = (int *) &colorC_schema_id;
         strcpy(all[num_schemas].name,"colorC");
         all[num_schemas].resume = (resumeFn) mycolorC_resume;
         all[num_schemas].suspend = (suspendFn) mycolorC_suspend;
         printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
         (*(all[num_schemas].id)) = num_schemas;
         all[num_schemas].fps = 0.;
         all[num_schemas].k =0;
         all[num_schemas].state=slept;
         all[num_schemas].close = NULL;
         all[num_schemas].handle = NULL;
         num_schemas++;
         myexport("colorC","id",&colorC_schema_id);
         myexport("colorC","colorC",&colorC);
         myexport("colorC","clock", &imageC_clock);
         myexport("colorC","resume",(void *)mycolorC_resume);
         myexport("colorC","suspend",(void *)mycolorC_suspend);
         myexport("colorC","width",&width[2]);
         myexport("colorC","height",&height[2]);
	 mycolorC_resume(SHELLHUMAN,NULL,NULL);
   }
  
   /*creates new schema for colorD*/
   if(serve_color[3]==1){
         pthread_mutex_lock(&color_mutex[3]);
         all[num_schemas].id = (int *) &colorD_schema_id;
         strcpy(all[num_schemas].name,"colorD");
         all[num_schemas].resume = (resumeFn) mycolorD_resume;
         all[num_schemas].suspend = (suspendFn) mycolorD_suspend;
         printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
         (*(all[num_schemas].id)) = num_schemas;
         all[num_schemas].fps = 0.;
         all[num_schemas].k =0;
         all[num_schemas].state=slept;
         all[num_schemas].close = NULL;
         all[num_schemas].handle = NULL;
         num_schemas++;
         myexport("colorD","id",&colorD_schema_id);
         myexport("colorD","colorD",&colorD);
         myexport("colorD","clock", &imageD_clock);
         myexport("colorD","resume",(void *)mycolorD_resume);
         myexport("colorD","suspend",(void *)mycolorD_suspend);
         myexport("colorD","width",&width[3]);
         myexport("colorD","height",&height[3]);
	 mycolorD_resume(SHELLHUMAN,NULL,NULL);
   }

   /*creates new schema for varcolorA*/
   if(serve_color[4]==1){
         pthread_mutex_lock(&color_mutex[4]);
         all[num_schemas].id = (int *) &varcolorA_schema_id;
         strcpy(all[num_schemas].name,"varcolorA");
         all[num_schemas].resume = (resumeFn) myvarcolorA_resume;
         all[num_schemas].suspend = (suspendFn) myvarcolorA_suspend;
         printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
         (*(all[num_schemas].id)) = num_schemas;
         all[num_schemas].fps = 0.;
         all[num_schemas].k =0;
         all[num_schemas].state=slept;
         all[num_schemas].close = NULL;
         all[num_schemas].handle = NULL;
         num_schemas++;
         myexport("varcolorA","id",&varcolorA_schema_id);
         myexport("varcolorA","varcolorA",&varcolorA);
         myexport("varcolorA","clock", &varimageA_clock);
         myexport("varcolorA","resume",(void *)myvarcolorA_resume);
         myexport("varcolorA","suspend",(void *)myvarcolorA_suspend);
         myexport("varcolorA","width",&width[4]);
         myexport("varcolorA","height",&height[4]);
	 myvarcolorA_resume(SHELLHUMAN,NULL,NULL);
   }

   /*creates new schema for varcolorB*/
   if(serve_color[5]==1){
         pthread_mutex_lock(&color_mutex[5]);
         all[num_schemas].id = (int *) &varcolorB_schema_id;
         strcpy(all[num_schemas].name,"varcolorB");
         all[num_schemas].resume = (resumeFn) myvarcolorB_resume;
         all[num_schemas].suspend = (suspendFn) myvarcolorB_suspend;
         printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
         (*(all[num_schemas].id)) = num_schemas;
         all[num_schemas].fps = 0.;
         all[num_schemas].k =0;
         all[num_schemas].state=slept;
         all[num_schemas].close = NULL;
         all[num_schemas].handle = NULL;
         num_schemas++;
         myexport("varcolorB","id",&varcolorB_schema_id);
         myexport("varcolorB","varcolorB",&varcolorB);
         myexport("varcolorB","clock", &varimageB_clock);
         myexport("varcolorB","resume",(void *)myvarcolorB_resume);
         myexport("varcolorB","suspend",(void *)myvarcolorB_suspend);
         myexport("varcolorB","width",&width[5]);
         myexport("varcolorB","height",&height[5]);
	 myvarcolorB_resume(SHELLHUMAN,NULL,NULL);
   }

   /*creates new schema for varcolorC*/
   if(serve_color[6]==1){
         pthread_mutex_lock(&color_mutex[6]);
         all[num_schemas].id = (int *) &varcolorC_schema_id;
         strcpy(all[num_schemas].name,"varcolorC");
         all[num_schemas].resume = (resumeFn) myvarcolorC_resume;
         all[num_schemas].suspend = (suspendFn) myvarcolorC_suspend;
         printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
         (*(all[num_schemas].id)) = num_schemas;
         all[num_schemas].fps = 0.;
         all[num_schemas].k =0;
         all[num_schemas].state=slept;
         all[num_schemas].close = NULL;
         all[num_schemas].handle = NULL;
         num_schemas++;
         myexport("varcolorC","id",&varcolorC_schema_id);
         myexport("varcolorC","varcolorC",&varcolorC);
         myexport("varcolorC","clock", &varimageC_clock);
         myexport("varcolorC","resume",(void *)myvarcolorC_resume);
         myexport("varcolorC","suspend",(void *)myvarcolorC_suspend);
         myexport("varcolorC","width",&width[6]);
         myexport("varcolorC","height",&height[6]);
	 myvarcolorC_resume(SHELLHUMAN,NULL,NULL);
   }

   /*creates new schema for varcolorD*/
   if(serve_color[7]==1){
         pthread_mutex_lock(&color_mutex[7]);
         all[num_schemas].id = (int *) &varcolorD_schema_id;
         strcpy(all[num_schemas].name,"varcolorD");
         all[num_schemas].resume = (resumeFn) myvarcolorD_resume;
         all[num_schemas].suspend = (suspendFn) myvarcolorD_suspend;
         printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
         (*(all[num_schemas].id)) = num_schemas;
         all[num_schemas].fps = 0.;
         all[num_schemas].k =0;
         all[num_schemas].state=slept;
         all[num_schemas].close = NULL;
         all[num_schemas].handle = NULL;
         num_schemas++;
         myexport("varcolorD","id",&varcolorD_schema_id);
         myexport("varcolorD","varcolorD",&varcolorD);
         myexport("varcolorD","clock", &varimageD_clock);
         myexport("varcolorD","resume",(void *)myvarcolorD_resume);
         myexport("varcolorD","suspend",(void *)myvarcolorD_suspend);
         myexport("varcolorD","width",&width[7]);
         myexport("varcolorD","height",&height[7]);
	 myvarcolorD_resume(SHELLHUMAN,NULL,NULL);
   }

   /* Whenever the mplayer and mencoder processes are launched they start to provide images.
      They require someone reading them from the last fifo. Otherwise, when connecting to 
      a streaming image server, the internal buffers (of mplayer, mencoder and the intervening 
      fifos) are completely filled, which causes a huge delay (several seconds) between the 
      images delivered by the streaming server now and those delivered to the jdec program (which
      is reading from the last fifo). Eventually the system reduces such delay and recovers the 
      liveliness.

      That is why the corresponding schema (colorA,varcolorA,etc) and its thread are started 
      BEFORE launching the mplayer+encoder processes. When they start running there is already
      a thread looking for the image data from the last fifo. It avoids the saturation of the 
      buffers.
   */
   for (i=0; i<MAXVIDS; i++){
     /*inicializar todos los mplayer y mencoder*/
     pid_mplayer[i]=-1;
     pid_mencoder[i]=-1;
     if (serve_color[i]==1){
       mplayer_start(i);
       if ((pid_mplayer[i]<=0) || (pid_mencoder[i]<=0))
	 {
	   serve_color[i]=0;
	   printf("cannot find file for ");
	   if (i==0) printf("colorA\n");
	   else if (i==1) printf("colorB\n");
	   else if (i==2) printf("colorC\n");
	   else if (i==3) printf("colorD\n");
	   else if (i==4) printf("varcolorA\n");
	   else if (i==5) printf("varcolorB\n");
	   else if (i==6) printf("varcolorC\n");
	   else if (i==7) printf("varcolorD\n");
	 }
     }
   }
   
}
