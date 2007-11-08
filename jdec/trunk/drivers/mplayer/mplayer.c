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
 *  jdec mplayer driver provides video images to color variables from static video files with 320x240 resolution and .ogg extension.
 *
 *  @file mplayer.c
 *  @author Javier Martin Ramos <xaverbrennt@yahoo.es> and Jose Antonio Santos Cadenas  <santoscadenas@gmail.com>
 *  @version 4.1
 *  @date 30-05-2007
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

/** max number of videos used.*/
#define MAXVIDS 4
/** max number of characters in file path and name.*/
#define ROUTE_LEN 255

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
/** mplayer number of color activated in gui.*/
int color_active[MAXVIDS];
/** mplayer repeat structure for videos. if repeat[0] = 1 then video 1 will be repeated when finished.*/
int repeat[MAXVIDS];
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


/* MPLAYER DRIVER FUNCTIONS */

/** mplayer driver function to close devices.*/
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
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorA_resume(int father, int *brothers, arbitration fn){
   if(serve_color[0]==1){
      pthread_mutex_lock(&refmutex);
      color_active[0]++;
      pthread_mutex_unlock(&refmutex);
      if ((all[colorD_schema_id].father==GUIHUMAN) ||
           (all[colorD_schema_id].father==SHELLHUMAN))
         all[colorD_schema_id].father = father;
      if(color_active[0]==1)
      {
         pthread_mutex_unlock(&color_mutex[0]);
         /*printf("colorA schema resume (mplayer driver)\n");*/
         all[colorA_schema_id].father = father;
         all[colorA_schema_id].fps = 0.;
         all[colorA_schema_id].k =0;
         put_state(colorA_schema_id,winner);
	  
         if((color_active[1]==0)&&(color_active[2]==0)&&(color_active[3]==0)){
            /* mplayer thread goes winner */
            pthread_mutex_lock(&mymutex);
            state=winner;
            pthread_cond_signal(&condition);
            pthread_mutex_unlock(&mymutex);
         }
      }
   }
   return 0;
}

/** colorA suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorA_suspend(){
   pthread_mutex_lock(&refmutex);
   color_active[0]--;
   pthread_mutex_unlock(&refmutex);
   if((serve_color[0]==1)&&(color_active[0]==0)){
      pthread_mutex_lock(&color_mutex[0]);
      put_state(colorA_schema_id,slept);
      /*printf("colorA schema suspend (mplayer driver)\n");*/
      if((color_active[1]==0)&&(color_active[2]==0)&&(color_active[3]==0)){
         /* mplayer thread goes sleep */
         pthread_mutex_lock(&mymutex);
         state=slept;
         pthread_mutex_unlock(&mymutex);
      }
   }
   return 0;
}

/** colorB resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorB_resume(int father, int *brothers, arbitration fn){
   if(serve_color[1]==1)
   {
      pthread_mutex_lock(&refmutex);
      color_active[1]++;
      pthread_mutex_unlock(&refmutex);
      if ((all[colorD_schema_id].father==GUIHUMAN) ||
           (all[colorD_schema_id].father==SHELLHUMAN))
         all[colorD_schema_id].father = father;
      if(color_active[1]==1)
      {
         pthread_mutex_unlock(&color_mutex[1]);
         /*printf("colorB schema resume (mplayer driver)\n");*/
         all[colorB_schema_id].father = father;
         all[colorB_schema_id].fps = 0.;
         all[colorB_schema_id].k =0;
         put_state(colorB_schema_id,winner);
	  
         if((color_active[0]==0)&&(color_active[2]==0)&&(color_active[3]==0)){
            /* mplayer thread goes winner */
            pthread_mutex_lock(&mymutex);
            state=winner;
            pthread_cond_signal(&condition);
            pthread_mutex_unlock(&mymutex);
         }
      }
   }
   return 0;
}

/** colorB suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorB_suspend(){
   pthread_mutex_lock(&refmutex);
   color_active[1]--;
   pthread_mutex_unlock(&refmutex);
   if((serve_color[1]==1)&&(color_active[1]==1)){
      pthread_mutex_lock(&color_mutex[1]);
      /*printf("colorB schema suspend (mplayer driver)\n");*/
      put_state(colorB_schema_id,slept);
    
      if((color_active[0]==0)&&(color_active[2]==0)&&(color_active[3]==0)){
         /* mplayer thread goes sleep */
         pthread_mutex_lock(&mymutex);
         state=slept;
         pthread_mutex_unlock(&mymutex);
      }
   }
   return 0;
}

/** colorC resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorC_resume(int father, int *brothers, arbitration fn){
   if(serve_color[2]==1)
   {
      pthread_mutex_lock(&refmutex);
      color_active[2]++;
      pthread_mutex_unlock(&refmutex);
      if ((all[colorD_schema_id].father==GUIHUMAN) ||
           (all[colorD_schema_id].father==SHELLHUMAN))
         all[colorD_schema_id].father = father;
      if(color_active[2]==1)
      {
         pthread_mutex_unlock(&color_mutex[2]);
         /*printf("colorC schema resume (mplayer driver)\n");*/
         all[colorC_schema_id].father = father;
         all[colorC_schema_id].fps = 0.;
         all[colorC_schema_id].k =0;
         put_state(colorC_schema_id,winner);
	  
         if((color_active[1]==0)&&(color_active[0]==0)&&(color_active[3]==0)){
            /* mplayer thread goes winner */
            pthread_mutex_lock(&mymutex);
            state=winner;
            pthread_cond_signal(&condition);
            pthread_mutex_unlock(&mymutex);
         }
      }
   }
   return 0;
}

/** colorC suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorC_suspend(){
   pthread_mutex_lock(&refmutex);
   color_active[2]--;
   pthread_mutex_unlock(&refmutex);
   if((serve_color[2]==1)&&(color_active[2]==1)){
      pthread_mutex_lock(&color_mutex[2]);
      /*printf("colorC schema suspend (mplayer driver)\n");*/
      put_state(colorC_schema_id,slept);
    
      if((color_active[0]==0)&&(color_active[1]==0)&&(color_active[3]==0)){
         /* mplayer thread goes sleep */
         pthread_mutex_lock(&mymutex);
         state=slept;
         pthread_mutex_unlock(&mymutex);
      }
   }
   return 0;
}

/** colorD resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorD_resume(int father, int *brothers, arbitration fn){

   if(serve_color[3]==1)
   {
      pthread_mutex_lock(&refmutex);
      color_active[3]++;
      pthread_mutex_unlock(&refmutex);
      if ((all[colorD_schema_id].father==GUIHUMAN) ||
           (all[colorD_schema_id].father==SHELLHUMAN))
         all[colorD_schema_id].father = father;
      if(color_active[3]==1)
      {
         pthread_mutex_unlock(&color_mutex[3]);
         /*printf("colorD schema resume (mplayer driver)\n");*/
         all[colorD_schema_id].father = father;
         all[colorD_schema_id].fps = 0.;
         all[colorD_schema_id].k =0;
         put_state(colorD_schema_id,winner);
	  
         if((color_active[1]==0)&&(color_active[2]==0)&&(color_active[0]==0)){
            /* mplayer thread goes winner */
            pthread_mutex_lock(&mymutex);
            state=winner;
            pthread_cond_signal(&condition);
            pthread_mutex_unlock(&mymutex);
         }
      }
   }
   return 0;
}

/** colorD suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorD_suspend(){
   pthread_mutex_lock(&refmutex);
   color_active[3]--;
   pthread_mutex_unlock(&refmutex);
   if((serve_color[3]==1)&&(color_active[3]==1)){
      pthread_mutex_lock(&color_mutex[3]);
      /*printf("colorD schema suspend (mplayer driver)\n");*/
      put_state(colorD_schema_id,slept);
    
      if((color_active[0]==0)&&(color_active[1]==0)&&(color_active[2]==0)){
         /* mplayer thread goes sleep */
         pthread_mutex_lock(&mymutex);
         state=slept;
         pthread_mutex_unlock(&mymutex);
      }
   }
   return 0;
}

/** mplayer driver function to start an mplayer process for a selected color.
 *  @param i selected color to launch an mplayer thread.*/
void mplayer_start(int i){
   int file;
   char str[100];
   char str2[100];

   umask (0000);
	 
   unlink (fifo1[i]);
   unlink (fifo2[i]);
   if ( (mkfifo (fifo1[i], 0600) != 0) ||
	 (mkfifo (fifo2[i], 0600) != 0) )
      exit (1);

   if ((pid_mencoder[i]=fork()) == 0) {/* We create a new process...
      // ... close its stdin, stdout & stderr ...*/
      file = open("/dev/null",O_RDWR);
      close(0); dup(file);
      close(1); dup(file);
      close(2); dup(file);
	    
      execlp("mencoder","mencoder",fifo1[i],"-nosound","-o",fifo2[i],
             "-ovc","raw","-of","rawvideo","-vf","format=bgr24",NULL);
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
      sprintf(str,"scale=%d:%d",SIFNTSC_COLUMNS,SIFNTSC_ROWS);
      sprintf (str2, "yuv4mpeg:file=%s", fifo1[i]);
      execlp("mplayer","mplayer",video_files[i],"-vo", str2,
             "-vf", str, "-ao","null","-slave",NULL);
      printf("Error executing mplayer\n");
      exit(1);
   }
}

/** mplayer driver internal thread.
 *  @param id selected color id.*/
void *mplayer_thread(void *id){
   int i;
   i=*((int*)id);
   int fifo;
   static unsigned long int lastimage=0;

   printf("mplayer driver started up\n");

   if ((fifo=open (fifo2[i],O_RDONLY))==-1){
      perror("Error al abrir el fifo");
      exit(1);
   }
   do{

      int leidos=0;
      int leidos_tmp=0;
      char buff[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3];

      pthread_mutex_lock(&color_mutex[i]); /*Si está bloqueado se queda ahí*/
      pthread_mutex_unlock (&color_mutex[i]);
      /*Simplemente lee del fifo y lo coloca en colorX*/
      while (leidos<SIFNTSC_COLUMNS*SIFNTSC_ROWS*3){
         leidos_tmp=read (fifo, buff+leidos,
                          SIFNTSC_COLUMNS*SIFNTSC_ROWS*3 - leidos);
         if (leidos_tmp==0){


            if (repeat[i]==1){
	       /*Lanzar otro mplayer si hay repeat
               Se matan mplayer y mencoder, se ignoran errores porque
               Seguramente ya hayan muerto y eso devuelve error*/
               close(fifo);

               kill (pid_mplayer[i], 9);
               wait (NULL);
               kill (pid_mencoder[i], 9);
               wait (NULL);
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
            memcpy(colorA, buff, SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
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
                              printf("mplayer: error in config file.\n'end_section' keyword required before starting new driver section.\n");
                              end_section=1; end_parse=1;

                           }else if(strcmp(word3,"provides")==0){
                              while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
                              /*printf ("%s",buffer_file2);*/
                              if(sscanf(buffer_file2,"%s %s %s %s",word3,word4,word5,word6)==4){
                                 printf("mplayer: %s from %s\n",word4,word5);
                                 if(strcmp(word4,"colorA")==0){
                                    serve_color[0]=1;
                                    strcpy (video_files[0],word5);
                                    if(strcmp(word6,"repeat_on")==0) repeat[0] = 1; else repeat[0] = 0;

                                 }else if(strcmp(word4,"colorB")==0){
                                    serve_color[1]=1;
                                    strcpy (video_files[1],word5);
                                    if(strcmp(word6,"repeat_on")==0) repeat[1] = 1; else repeat[1] = 0;

                                 }else if(strcmp(word4,"colorC")==0){
                                    serve_color[2]=1;
                                    strcpy (video_files[2],word5);
                                    if(strcmp(word6,"repeat_on")==0) repeat[2] = 1; else repeat[2] = 0;

                                 }else if(strcmp(word4,"colorD")==0){
                                    serve_color[3]=1;
                                    strcpy (video_files[3],word5);
                                    if(strcmp(word6,"repeat_on")==0) repeat[3] = 1; else repeat[3] = 0;
                                 }

                              }else{
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
      if((serve_color[0]==0)&&(serve_color[1]==0)&&(serve_color[2]==0)&&(serve_color[3]==0)){
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

   for (i=0; i<MAXVIDS; i++){
      /*inicializar todos los mplayer y mencoder*/
      pid_mplayer[i]=0;
      pid_mencoder[i]=0;
      if (serve_color[i]==1){
         mplayer_start(i);
      }
   }

   if(mplayer_thread_created==0){
      static int args[MAXVIDS];
      /*Crear hilos para cada imagen*/
      pthread_mutex_lock(&mymutex);
      state=slept;
      if (serve_color[0]){
	 args[0]=0;
    pthread_create(&mplayer_th[0],NULL,mplayer_thread,(void*)&args[0]);
      }
      if (serve_color[1]){
	 args[1]=1;
	 pthread_create(&mplayer_th[1],NULL,mplayer_thread,(void*)&args[1]);
      }
      if (serve_color[2]){
	 args[2]=2;
	 pthread_create(&mplayer_th[2],NULL,mplayer_thread,(void*)&args[2]);
      }
      if (serve_color[3]){
	 args[3]=3;
	 pthread_create(&mplayer_th[3],NULL,mplayer_thread,(void*)&args[3]);
      }
      mplayer_thread_created=1;
      pthread_mutex_unlock(&mymutex);
   }
   /*Se crean los esquemas que a su vez bloquean al su correspondiente threat
   hasta que se active el esquema*/
   
   /*creates new schema for colorA*/
   if(serve_color[0]==1){

      if(pid_mplayer[0]!=0 && pid_mencoder[0]!=0){
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
      }else{
	 serve_color[0]=0;
	 printf("cannot find file for colorA\n");
      }
   }

   /*creates new schema for colorB*/
   if(serve_color[1]==1){
      if(pid_mplayer[1]!=0 && pid_mencoder[1]!=0){
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
      }else{
	 serve_color[1]=0;
	 printf("cannot find file for colorB\n");
      }
   }
  
   /*creates new schema for colorC*/
   if(serve_color[2]==1){
      if(pid_mplayer[2]!=0 && pid_mencoder[2]!=0){
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
      }else{
	 serve_color[2]=0;
	 printf("cannot find file for colorC\n");
      }
   }
  
   /*creates new schema for colorD*/
   if(serve_color[3]==1){
      if(pid_mplayer[3]!=0 && pid_mencoder[3]!=0){
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
      }else{
	 serve_color[3]=0;
	 printf("cannot find file for colorD\n");
      }
   }
}
