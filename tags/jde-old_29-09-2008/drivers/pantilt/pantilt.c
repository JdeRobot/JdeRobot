/*
 *  Copyright (C) 2006 Roberto Calvo Palomino
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
 *  Authors : Jose Maria Ca�as Plaza <jmplaza@gsyc.escet.urjc.es>
 *            Victor G�mez G�mez <vmanuel@gsyc.escet.urjc.es>
 *            Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es>
 *            Roberto Calvo Palomino <rocapal@gsyc.escet.urjc.es>
 */

/**
 *  jdec pantilt driver provides sensorial information from a pantilt neck conected.
 *
 *  @file pantilt.c
 *  @author Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es>, Victor Gómez Gómez <vmanuel@gsyc.escet.urjc.es>, Roberto Calvo Palomino <rocapal@gsyc.escet.urjc.es> and Jose Maria Cañas Plaza <jmplaza@gsyc.escet.urjc.es>
 *  @version 4.1
 *  @date 30-05-2007
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "jde.h"
#include <termios.h>
#include <math.h> 

/** pantilt driver pantiltencoders period polling.*/
#define PANTILTENCODERS_POLLING 300 /* period to ask for new pantilt encoders (ms) */
/** pantilt driver rs232 baud rate.*/
#define RS232_BAUD_RATE B9600
/** pantilt driver from encoder units to deg. factor.*/
#define ENCOD_TO_DEG (3.086/60.) /* from encoder units to deg, also 205.89/4002 */
/** pantilt driver max char size in string message.*/
#define MAX_MESSAGE 2048
/** pantilt driver debug macro.*/
/* Uncomment this for see traces */ 
#define D(x...) /*printf(x); */  

/* directed perception pantilt limits */
/** Max pan angle constant*/
#define MAX_PAN_ANGLE 159.13 /* degrees */
/** Min pan angle constant*/
#define MIN_PAN_ANGLE -159.13 /* degrees */
/** Max tilt angle constant*/
#define MAX_TILT_ANGLE 30. /* degrees */
/** Min tilt angle constant*/
#define MIN_TILT_ANGLE -46. /* degrees */
/** Max pantilt speed*/
#define MAX_SPEED_PANTILT 205.89

/** pantilt driver pthread for reading.*/
pthread_t pantilt_readth;
/** pantilt driver pthread for polling.*/
pthread_t pantilt_pollth;
/** pantilt driver device name. Example /dev/tty01.*/
char pantilt_device[MAX_MESSAGE];
/** pantilt driver serial port number.*/
int pantilt_serialport;

/** pantilt driver main pthread.*/
pthread_t pantilt_th;

/** pantilt driver state variable for motors pthread.*/
int state_motors;
/** pantilt driver state variable for encoders pthread.*/
int state_encoders;
/** pantilt driver mutex for motors pthread.*/
pthread_mutex_t mymutex_motors;
/** pantilt driver mutex for encoders pthread.*/
pthread_mutex_t mymutex_encoders;
/** pantilt driver mutex for serial pantilt device.*/
pthread_mutex_t serialpantilt_mutex;
/** pantilt driver condition flag for motors pthread.*/
pthread_cond_t condition_motors;
/** pantilt driver condition flag for encoders pthread.*/
pthread_cond_t condition_encoders;
/** pantilt driver reading buffer.*/
char pantilt_in[MAX_MESSAGE];
/** pantilt driver writting buffer.*/
char pantilt_out[MAX_MESSAGE];
/** pantilt driver variable to remember the current position while reading a buffer.*/
int pantilt_marca;

/** id for pantilt motors schema.*/
int ptmotors_schema_id;
/** id for pantilt encoders schema.*/
int ptencoders_schema_id;

/** pantilt driver last longitude value read.*/
float longitude_last=0.;
/** pantilt driver last latitude value read.*/
float latitude_last=0.;
/** pantilt driver last longitude speed value read.*/
float longspeed_last=0.;
/** pantilt driver last latitude speed value read.*/
float latspeed_last=0.;

/** pantilt driver current max pan variable.*/
float max_pan;
/** pantilt driver current min pan variable.*/
float min_pan;
/** pantilt driver current max tilt variable.*/
float max_tilt;
/** pantilt driver current min tilt variable.*/
float min_tilt;

/*API Variables compartidas*/
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
/** Max longitude speed*/
float max_longitude_speed = MAX_SPEED_PANTILT;
/** Max latitude speed*/
float max_latitude_speed = MAX_SPEED_PANTILT;

/** 'ptencoders' schema pan angle information*/
float pan_angle;   /* degs */
/** 'ptencoders' schema tilt angle information*/
float tilt_angle;  /* degs */
/** 'ptencoders' schema clock*/
unsigned long int pantiltencoders_clock;


/* real limits in encoders units. MAX_PAN_ANGLE, MAX_TILT_ANGLE... are approximate limits to be used by the client of oculo */
/** pantilt driver variable to detect if pthreads were created.*/
int pantilt_thread_created=0;
/** pantilt driver variable to detect if pantilt devices were cleaned up.*/
int pantilt_cleaned_up=0;
/** pantilt driver  variable to detect if pantilt devices were setup.*/
int pantilt_setup=0;
/** pantilt driver to show fps in jdec shell.*/
int display_fps=0;
/** pantilt driver variable to detect if pthreads must end its execution.*/
int pantilt_close_command=0;

/* Ref counter*/
/** ptmotors ref counter*/
int ptmotors_refs=0;
/** ptencoders ref counter*/
int ptencoders_refs=0;

/** mutex for ref counters*/
pthread_mutex_t refmutex;

/* pantilt driver API options */
/** pantilt driver name.*/
char driver_name[256]="pantilt";

/* activate driver */
/** pantilt driver variable to detect if pantilt encoders were activated on gui.*/
int activate_pantiltencoders=0;
/** pantilt driver variable to detect if pantilt motors were activated on gui.*/
int activate_pantiltmotors=0;


/**
 * Function to truncate float number to nearest integer
 * @param numberF The number that will be truncated
 * @return The number truncated
 */
float truncateFloat (float numberF) 
{ 
 char numeroC[20]; 
 int entero, decimal; 

 if ( sprintf (numeroC, "%f", numberF) < 0 ) 
   { 
    printf("truncateFloat: Error in convert to float number in char*\n"); 
    return 0.0; 
   } 
 sscanf  (numeroC,"%d.%d",&entero,&decimal); 

 while (decimal>10) 
    decimal = decimal / 10; 

    if (decimal > 5) 
       { 
         if (entero>0) 
            { /* Para numeros positivos */ 
              entero=entero+1; 
            } 
         else 
            {       /* Para numeros negativos */ 
              entero=entero-1; 
            } 
       } 

 return (float)entero; 
}


/** pantilt driver function to show fps in jdec shell.*/
void pantilt_display_fps(){
  display_fps=1;
}

/** pantilt driver function to clean up pantilt devices.*/
void pantilt_clean_up() {
  pantilt_cleaned_up=1;
  pthread_mutex_lock(&serialpantilt_mutex);
  close(pantilt_serialport);
  pthread_mutex_unlock(&serialpantilt_mutex);
}

/** pantilt driver function to stop and clean pantilt devices.*/
void pantilt_close(){

  pantilt_close_command=1;
  pantilt_clean_up();
  printf("driver pantilt off\n");
}


/** pantilt driver function to open passed device and sets it to raw mode.
 *  @param filename path and device name.
 *  @param io_flags flags to open device.
 *  @return File descriptor to that device.*/
int openRaw(char* filename, mode_t io_flags)
/*                                                        */
/*  Opens passed filename and sets its device to raw mode.*/
/*  Returns file descriptor.                              */
/*                                                        */
/*  raw mode is defined as:                               */
/*  Off: echo, canonical input, extended processing,      */
/*       signals, break key, parity, 8th bit strip,       */
/*       flow control, output post processing             */
/*   On: 8 bit size                                       */
{
  struct termios term_info;
  int fd;

  fd = open(filename,io_flags);
  if (fd == -1)
    {
      /* maybe complain if TRACE is on */
      return -1;
    }
  
  if(tcgetattr(fd,&term_info) <0)
    {
      /* complain - fd is not a terminal */
      return -1;
    }

  /* turn off echo, canonical mode, extended processing, signals */
  term_info.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); 
  
  /* turn off break sig, cr->nl, parity off, 8 bit strip, flow control */
  term_info.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

  /* clear size, turn off parity bit */
  term_info.c_cflag &= ~(CSIZE | PARENB);

  /* set size to 8 bits */
  term_info.c_cflag |= CS8;

  /* turn output processing off */
  term_info.c_oflag &= ~(OPOST);

  /* Set time and bytes to read at once */
  term_info.c_cc[VTIME] = 0;
  term_info.c_cc[VMIN] = 0;

  if(tcsetattr(fd,TCSAFLUSH,&term_info) <0)
    return -1;

  return fd;
}

/** pantilt driver function to set baud rate to and opened device.
 *  @param fd file descriptor to the selected device.
 *  @param baudRate desired baud rate. Baud rate constants are found in <termios.h>.
 *  @return 0 if successful or -1 if something was wrong.*/
int setBaudRate (int fd, speed_t baudRate)
/*  Set the baud rate of an open device.                  */
/*  Baud rate constants are found in <termios.h>.         */
/*  B9600 == 9600 baud                                    */
{
   struct termios terminfo;
   int error;
     
   error = tcgetattr(fd, &terminfo);
   if (error)
      {
         perror("tcgetattr()");
         return(-1);
      }
   error = cfsetospeed(&terminfo, baudRate);
   if (error)
      {
         fprintf(stderr, "cfsetospeed(%ld): ",
                 (long)baudRate);
         perror(NULL);
         return(-1);
      }
   error = cfsetispeed(&terminfo, baudRate);
   if (error)
      {
         fprintf(stderr, "cfsetispeed(%ld): ",
                 (long)baudRate);
         perror(NULL);
         return(-1);
      }
   error = tcsetattr(fd, TCSANOW, &terminfo);
   if (error)
      {
         perror("tcsetattr()");
         return(-1);
      }
   return(1);
}

/** pantilt driver function to send a command to a pantilt neck.
 *  @param cmd desired command to send.*/
void SendCmd(char *cmd){

  pthread_mutex_lock(&serialpantilt_mutex);
  write(pantilt_serialport,cmd,strlen(cmd)); 
  D("Pantilt.c :: SendCmd :: serialpantilt send: %s\n",cmd);
  pthread_mutex_unlock(&serialpantilt_mutex);

}

/** pantilt driver function to decode messages from pantilt neck.
 *  @param mensaje received message from pantilt neck.*/
void serve_serialpantilt_message(char *mensaje)
{
  int pan_encoders, tilt_encoders,kk; /* encoders units */
  
  if (sscanf(mensaje,"* Current Pan position is %d",&pan_encoders)==1) 
    {
      pan_angle=(float)pan_encoders*ENCOD_TO_DEG; 
      /* if (source[SCH_PANTILTENCODERS]==serialpantilt) */ 
      speedcounter(ptencoders_schema_id);
      /*printf("PAN=%.2f\n",pan_angle);*/
    }
  else if (sscanf(mensaje,"* Current Tilt position is %d",&tilt_encoders)==1)
   {tilt_angle=(float)tilt_encoders*ENCOD_TO_DEG;
   /*printf("TILT=%.2f\n",tilt_angle);*/}
  else if (sscanf(mensaje,"* Minimum Pan position is %d",&kk)==1)
    { min_pan=(float)kk*ENCOD_TO_DEG; 
    printf(" Minimum Pan = %.2f\n",min_pan);
    }
  else if (sscanf(mensaje,"* Maximum Pan position is %d",&kk)==1) 
    { max_pan=(float)kk*ENCOD_TO_DEG; 
    printf(" Maximum Pan = %.2f\n",max_pan);
    }
  else if (sscanf(mensaje,"* Minimum Tilt position is %d",&kk)==1) 
    { min_tilt=(float)kk*ENCOD_TO_DEG; 
    printf(" Minimun Tilt = %.2f\n",min_tilt);
    }
  else if (sscanf(mensaje,"* Maximum Tilt position is %d",&kk)==1) 
    { max_tilt=(float)kk*ENCOD_TO_DEG; 
    printf(" Maximun Tilt = %.2f\n",max_tilt);
    }
}

/** pantilt driver poll pthread function.*/
void *serialpantilt_pollthread() 
{
  struct timeval a,b;
  long diff, next;

  
  do{
      pthread_mutex_lock(&mymutex_encoders);

      if (state_encoders==slept) 
	  pthread_cond_wait(&condition_encoders,&mymutex_encoders);
      else 
	{
	  gettimeofday(&a,NULL);      
	  SendCmd("PP\nTP\n");	  
	  gettimeofday(&b,NULL); 
	  diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	  next = PANTILTENCODERS_POLLING*1000-diff-10000; 
	  /* discounts 10ms taken by calling usleep itself */
	  if (next>0) usleep(PANTILTENCODERS_POLLING*1000-diff);
	  else 
	    {
	      printf("time interval violated: pantiltencoders\n"); 
	      usleep(PANTILTENCODERS_POLLING*1000);
	    }
	}
      pthread_mutex_unlock(&mymutex_encoders);
    }while(pantilt_close_command==0);

  pthread_exit(0);
}

/** pantilt driver read pthread function.*/
void *serialpantilt_readthread(void *not_used) 
{ 
  int leidos=0, comienzo=0, j=0;
  
  do{
     usleep(PANTILTENCODERS_POLLING*0.75*1000);
     /* this read should be blocking, but awfully it seems it is not */
     leidos=read(pantilt_serialport,&(pantilt_in[pantilt_marca]),MAX_MESSAGE-pantilt_marca-1);
     /* printf("desbloqueo\n");*/

     switch(leidos)
       {
       case 0: /* printf(" Se cerro el puerto serie\n"); exit(1);  El puerto serie nunca se puede cerrar */ break;
       case -1: break; /* no hay nada que leer */
       default: 
	 comienzo=0;
	 for(j=pantilt_marca; (j<MAX_MESSAGE)&&(j<pantilt_marca+leidos);j++)
	   if (pantilt_in[j]=='\n') 
	     {
	       serve_serialpantilt_message(&pantilt_in[comienzo]);
	       comienzo=j+1;
	     }
	 
	 if (comienzo==0) 
	   {
	     if ((pantilt_marca+leidos)> MAX_MESSAGE-1) pantilt_marca=0; /* Se desbordo el buffer */
	     else pantilt_marca+=leidos;
	   }
	 else 
	   {
	     strncpy(pantilt_in,&(pantilt_in[comienzo]),pantilt_marca+leidos-comienzo);
	     pantilt_marca=pantilt_marca+leidos-comienzo;
	     comienzo=0;
	   }
       }

  }while(pantilt_close_command==0); 

  pthread_exit(0);
}


/** pantilt encoders suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int ptencoders_suspend()
{
   pthread_mutex_lock(&refmutex);
   if (ptencoders_refs>1){
      ptencoders_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      ptencoders_refs=0;
      pthread_mutex_unlock(&refmutex);
      if (activate_pantiltencoders) {
         pthread_mutex_unlock(&mymutex_encoders);
         state_encoders=slept;
         put_state(ptencoders_schema_id,slept);
         printf("ptencoders schema suspend\n");
         pthread_mutex_unlock(&mymutex_encoders);
      }
   }
   return 0;
}


/** pantilt encoders resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int ptencoders_resume(int father, int *brothers, arbitration fn)
{
   pthread_mutex_lock(&refmutex);
   if (ptencoders_refs>0){
      ptencoders_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      ptencoders_refs=1;
      pthread_mutex_unlock(&refmutex);
      if (activate_pantiltencoders) {
         pthread_mutex_unlock(&mymutex_encoders);
         state_encoders=active;
         all[ptencoders_schema_id].father = father;
         all[ptencoders_schema_id].fps = 0.;
         all[ptencoders_schema_id].k =0;
         put_state(ptencoders_schema_id,winner);
         printf("ptencoders schema resume\n");
         pthread_cond_signal(&condition_encoders);
         pthread_mutex_unlock(&mymutex_encoders);
      }
   }
   return 0;
}

/** pantilt driver motors main iteration function.*/
void pantiltmotors_iteration()
{  
  float longspeed, latspeed, longcommand, latcommand;

  speedcounter(ptmotors_schema_id);

  /* truncate both speed and position inside permited values*/
  if (longitude_speed < 0.0)     longspeed=0.0;
  else if (longitude_speed > MAX_SPEED_PANTILT)  longspeed=MAX_SPEED_PANTILT;
  else longspeed=longitude_speed;
  if (latitude_speed < 0.0)    latspeed=0.0;
  else if (latitude_speed > MAX_SPEED_PANTILT) latspeed=MAX_SPEED_PANTILT;
  else latspeed=latitude_speed;
 
  if (longspeed==0.0) {
    sprintf(pantilt_out,"PS0\n");
    SendCmd(pantilt_out);
  }
  else {

    if (longitude > max_pan) longcommand = max_pan;
    else if (longitude < min_pan) longcommand = min_pan;
    else longcommand=longitude;
    
    if ( (longcommand!=longitude_last) || (longspeed != longspeed_last) ) {
      sprintf(pantilt_out,"PP%d PS%d\n",(int)truncateFloat(longcommand/ENCOD_TO_DEG),(int)truncateFloat(longspeed/ENCOD_TO_DEG));       
      SendCmd(pantilt_out);
      longitude_last=longcommand;
      longspeed_last=longspeed;
    }
  }
  
  if (latspeed==0.0) {
    sprintf(pantilt_out,"TS0\n");
    SendCmd(pantilt_out);
  } 
  else {

    if (latitude > max_tilt)  latcommand = max_tilt;
    else if (latitude < min_tilt) latcommand = min_tilt;
    else latcommand=latitude;
    
    if( (latcommand!=latitude_last) || (latspeed!=latspeed_last) )  {
      sprintf(pantilt_out,"TP%d TS%d\n",(int)truncateFloat(latcommand/ENCOD_TO_DEG),(int)truncateFloat(latspeed/ENCOD_TO_DEG)); 
      SendCmd(pantilt_out);
      latitude_last=latcommand;
      latspeed_last=latspeed;
    }
  }
  /* if (debug[SCH_PANTILTMOTORS]) printf("pantiltmotors:  %1.1f %1.1f %1.1f %1.1f \n",latitude,longitude,latitude_speed,longitude_speed); */

}

/** pantilt motors suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int ptmotors_suspend()
{
   pthread_mutex_lock(&refmutex);
   if (ptmotors_refs>1){
      ptmotors_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      ptmotors_refs=0;
      pthread_mutex_unlock(&refmutex);
      if (activate_pantiltmotors) {
         pthread_mutex_lock(&mymutex_motors);
         state_motors=slept;
         put_state(ptmotors_schema_id,slept);
         printf("ptmotors schema suspend\n");
         printf("ptmotors_suspend: pan_angle=%f - tilt_angle=%f\n",pan_angle,tilt_angle);
         pthread_mutex_unlock(&mymutex_motors);
      }
   }
   return 0;
}


/** pantilt motors resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int ptmotors_resume(int father, int *brothers, arbitration fn)
{
   pthread_mutex_lock(&refmutex);
   if (ptmotors_refs>0){
      ptmotors_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      ptmotors_refs=1;
      pthread_mutex_unlock(&refmutex);
      if (activate_pantiltmotors) {

         printf("ptmotors_resume: pan_angle=%f - tilt_angle=%f\n",pan_angle,tilt_angle);

         longitude=pan_angle;
         latitude=tilt_angle;

    /* unfortunately it reads pan_angle an tilt_angle before sensationsoculo1
         has updated them for the first time, so this schema always starts
         positioning the pantilt unit at (0,0) angles */

         pthread_mutex_lock(&mymutex_motors);
         state_motors=winner;
         all[ptmotors_schema_id].father = father;
         all[ptmotors_schema_id].fps = 0.;
         all[ptmotors_schema_id].k =0;
         put_state(ptmotors_schema_id,winner);
         printf("ptmotors schema resume\n");
         pthread_cond_signal(&condition_motors);
         pthread_mutex_unlock(&mymutex_motors);
      }
   }
   return 0;
}

/** pantilt driver motors pthread function.*/
void *pantiltmotors_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  do{
      

      if (state_motors==slept) 
	{
	  pthread_mutex_lock(&mymutex_motors);
	  /* printf("pantiltmotors off\n");*/
	  pthread_cond_wait(&condition_motors,&mymutex_motors);
	  /* printf("pantiltmotors on\n");*/
	  pthread_mutex_unlock(&mymutex_motors);
	}
      else 
	{
	  pthread_mutex_unlock(&mymutex_motors);
	  gettimeofday(&a,NULL);
	  pantiltmotors_iteration();
	  gettimeofday(&b,NULL);  
	  diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	  
	  next = pantiltmotors_cycle*1000-diff-10000; 
	  /* discounts 10ms taken by calling usleep itself */
	  if (next>0) usleep(pantiltmotors_cycle*1000-diff);
	  else {printf("time interval violated: pantiltmotors\n"); usleep(pantiltmotors_cycle*1000);}
	}
      
  }while(pantilt_close_command==0);

  pthread_exit(0);
}


/** pantilt driver parse configuration file function.
 *  @param configfile path and name to the config file.
 *  @return 0 if parsing was successful or -1 if something went wrong.*/
int pantilt_parseconf(char *configfile){

  int end_parse=0; int end_section=0; int driver_config_parsed=0;
  FILE *myfile;
  const int limit = 256;

  if ((myfile=fopen(configfile,"r"))==NULL){
    printf("pantilt: cannot find config file\n");
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
	printf ("pantilt: line too long in config file!\n"); 
	exit(-1);
      }
      
      /* first word of the line */     
      if (sscanf(buffer_file,"%s",word)==1){
	if (strcmp(word,"driver")==0) {
	  while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
	  sscanf(&buffer_file[j],"%s",word2);
	  
	  /* checking if this section matchs our driver name */
	  if (strcmp(word2,"pantilt")==0){
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
		    printf("pantilt: error in config file.\n'end_section' keyword required before starting new driver section.\n");
		    end_section=1; end_parse=1;

		  }else if(strcmp(word3,"serial")==0){
		    
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s",word3,word4)>1)
		      strcpy(pantilt_device,word4);		     		 
		    else
		      printf("pantilt: serial line incorrect\n");
		    
		  }else if(strcmp(word3,"provides")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s",word3,word4)>1){
		      if(strcmp(word4,"pantiltencoders")==0){
			activate_pantiltencoders=1;
		      }else if(strcmp(word4,"pantiltmotors")==0){
			activate_pantiltmotors=1;
		      }
		    }else{
		      printf("pantilt: provides line incorrect\n");
		    }
		  }else printf("pantilt: i don't know what to do with '%s'\n",buffer_file2);
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

/** pantilt driver init function. It will start all pantilt required devices and setting them the default configuration.
 *  @return 0 if initialitation was successful or -1 if something went wrong.*/
void pantilt_init(){

  /* printf("pantilt device on %s serial port\n",pantilt_device); */  
  if ((pantilt_serialport = openRaw(pantilt_device, O_RDWR)) <= 0) 
    {perror("Cannot open serial port");}
  if (!setBaudRate(pantilt_serialport,(speed_t) RS232_BAUD_RATE))
    {perror("Unable to set baud rate of the serial port");}
  
  /* lo pone no bloqueante 
     if(fcntl(pantilt_serialport, F_SETFL, O_NONBLOCK) < 0) 
     { printf("fcntl FSETFL, O_NONBLOCK\n");  exit(1);}
  */


  /*  Remove whatever commands might be buffered.  */
  SendCmd("\xFF\xFF\n");
  /*  Make sure we start in immediate mode.  */
  SendCmd("I\n");
  SendCmd("CI\n");
  /*  Set base speeds.  */
  SendCmd("PB1000\n");
  SendCmd("TB1000\n");
  /*  Set upper and lower speed limits  */
  SendCmd("PU6000\n");
  SendCmd("TU6000\n");
  SendCmd("PL31\n");
  SendCmd("TL31\n");
  /*  Set target speeds and accelarations */
  SendCmd("PS2900\n");
  SendCmd("TS2900\n");
  latitude_speed=2900*ENCOD_TO_DEG;
  longitude_speed=2900*ENCOD_TO_DEG;
  SendCmd("PA9000\n"); /* 2000 */
  SendCmd("TA9000\n"); /* 2000 */
  /*  Set move and hold powers to low setting.  */
  SendCmd("PHL\n");
  SendCmd("PML\n");
  SendCmd("THL\n");
  SendCmd("TML\n");
  /*  Move to center.  */
  SendCmd("PP0\n");
  SendCmd("TP0\n");
  pan_angle=0.;
  tilt_angle=0.;
  /*  Enforce soft limits.  */
  SendCmd("LE\n");
  /* para leer los limites de posicion de la unidad*/
  SendCmd("PN\nPX\nTN\nTX\n"); 
 
  pantilt_setup=1;
}

/** pantilt driver startup function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void pantilt_startup(char *configfile)
{
 
  /* we call the function to parse the config file */
  if(pantilt_parseconf(configfile)==-1){
    printf("pantilt: cannot initiate driver. configfile parsing error.\n");
    exit(-1);
  }
  
  /* pantilt initialitation */
  if(pantilt_setup==0) pantilt_init();
  
  if(activate_pantiltencoders) {     
 
    printf("pantiltencoders driver started up\n");
    pthread_create(&pantilt_readth,NULL,serialpantilt_readthread,NULL);
    /*usleep(100000);*/
    pthread_mutex_lock(&mymutex_encoders);
    state_encoders=slept;
    pthread_create(&pantilt_pollth,NULL,serialpantilt_pollthread,NULL);
    pthread_mutex_unlock(&mymutex_encoders);

    all[num_schemas].id = (int *) &ptencoders_schema_id;
    strcpy(all[num_schemas].name,"ptencoders");
    all[num_schemas].resume = (resumeFn) ptencoders_resume;
    all[num_schemas].suspend = (suspendFn) ptencoders_suspend;
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
    myexport("ptencoders","resume",(void *)&ptencoders_resume);
    myexport("ptencoders","suspend",(void *)&ptencoders_suspend);
  }

  if (activate_pantiltmotors) {

    printf("pantiltmotors driver started up\n");
    pthread_mutex_lock(&mymutex_motors);
    state_motors=slept;
    pthread_create(&pantilt_th,NULL,pantiltmotors_thread,NULL);
    pthread_mutex_unlock(&mymutex_motors);    

    all[num_schemas].id = (int *) &ptmotors_schema_id;
    strcpy(all[num_schemas].name,"ptmotors");
    all[num_schemas].resume = (resumeFn) ptmotors_resume;
    all[num_schemas].suspend = (suspendFn) ptmotors_suspend;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].close = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    myexport("ptmotors","id",&ptmotors_schema_id);
    myexport("ptmotors","longitude",&longitude);
    myexport("ptmotors","latitude",&latitude);
    myexport("ptmotors","longitude_speed",&longitude_speed);
    myexport("ptmotors","latitude_speed",&latitude_speed);
    myexport("ptmotors","cycle", &pantiltmotors_cycle);
    myexport("ptmotors","resume",(void *)&ptmotors_resume);
    myexport("ptmotors","suspend",(void *)&ptmotors_suspend);
    myexport("ptmotors", "max_longitude", &max_pan);
    myexport("ptmotors", "max_latitude", &max_tilt);
    myexport("ptmotors", "min_longitude", &min_pan);
    myexport("ptmotors", "min_latitude", &min_tilt);
    myexport("ptmotors", "max_longitude_speed", &max_longitude_speed);
    myexport("ptmotors", "max_latitude_speed", &max_latitude_speed);
  }

  printf("pantilt driver started up\n");
}
