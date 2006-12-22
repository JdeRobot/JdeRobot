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
 *  Authors : Jose Maria Cañas Plaza <jmplaza@gsyc.escet.urjc.es>
 *            Victor Gómez Gómez <vmanuel@gsyc.escet.urjc.es
 *            Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es>
 *            Roberto Calvo Palomino <rocapal@gsyc.escet.urjc.es>
 */

/************************************************
 * jdec pantilt driver                         *
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "jde.h"
#include <termios.h>

/* directed perception pantilt limits */
#define MAX_PAN_ANGLE 158. /* degrees */
#define MIN_PAN_ANGLE -158. /* degrees */
#define MAX_TILT_ANGLE 30. /* degrees */
#define MIN_TILT_ANGLE -46. /* degrees */
#define MAX_SPEED_PANTILT 205.89
#define PANTILTENCODERS_POLLING 100 /* period to ask for new pantilt encoders (ms) */
#define RS232_BAUD_RATE B9600
#define ENCOD_TO_DEG (3.086/60.) /* from encoder units to deg, also 205.89/4002 */
#define MAX_MESSAGE 2048

#define D(x...) //printf(x); /* Uncomment this for see traces */


pthread_t pantilt_readth,pantilt_pollth;
char pantilt_device[MAX_MESSAGE]; 
int pantilt_serialport;

pthread_t pantilt_th;
int state_motors, state_encoders;
pthread_mutex_t mymutex_motors, mymutex_encoders, serialpantilt_mutex;
pthread_cond_t condition_motors, condition_encoders;
char pantilt_in[MAX_MESSAGE];
char pantilt_out[MAX_MESSAGE];
int pantilt_marca;

float longitude_last=0., latitude_last=0.;
float longspeed_last=0., latspeed_last=0.;

float max_pan, min_pan, max_tilt, min_tilt; 
/* real limits in encoders units. MAX_PAN_ANGLE, MAX_TILT_ANGLE... are approximate limits to be used by the client of oculo */

int pantilt_thread_created=0;
int pantilt_cleaned_up=0;
int pantilt_setup=0;
int display_fps=0;
int pantilt_close_command=0;

/* pantilt driver API options */
char driver_name[256]="pantilt";

/* activate driver */
int activate_pantiltencoders=0;
int activate_pantiltmotors=0;

void pantilt_display_fps(){
  display_fps=1;
}

void pantilt_clean_up() {
  pantilt_cleaned_up=1;
  pthread_mutex_lock(&serialpantilt_mutex);
  close(pantilt_serialport);
  pthread_mutex_unlock(&serialpantilt_mutex);
}

void pantilt_close(){

  pantilt_close_command=1;
  pantilt_clean_up();
  printf("driver pantilt off\n");
}




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

void SendCmd(char *cmd){

  pthread_mutex_lock(&serialpantilt_mutex);
  write(pantilt_serialport,cmd,strlen(cmd)); 
  D("Pantilt.c :: SendCmd :: serialpantilt send: %s\n",cmd);
  pthread_mutex_unlock(&serialpantilt_mutex);

}

void serve_serialpantilt_message(char *mensaje)
{
  int pan_encoders, tilt_encoders,kk; /* encoders units */
  
  if (sscanf(mensaje,"* Current Pan position is %d",&pan_encoders)==1) 
    {
      pan_angle=(float)pan_encoders*ENCOD_TO_DEG; 
      //if (source[SCH_PANTILTENCODERS]==serialpantilt)
      kpantiltencoders++;
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


void *serialpantilt_pollthread(void *not_used) 
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


int ptencoders_suspend()
{

  if (activate_pantiltencoders) {
    pthread_mutex_unlock(&mymutex_encoders);
    state_encoders=slept;
    printf("pantiltencoders driver off\n");
    pthread_mutex_unlock(&mymutex_encoders);
  }

  return 0;
}

int ptencoders_resume()
{

  if (activate_pantiltencoders) {
    pthread_mutex_unlock(&mymutex_encoders);
    state_encoders=active;
    printf("pantiltencoders driver on\n");
    pthread_cond_signal(&condition_encoders);
    pthread_mutex_unlock(&mymutex_encoders);
  }

  return 0;
}

void pantiltmotors_iteration()
{  
  float longspeed, latspeed, longcommand, latcommand;

  kpantiltmotors++;

  /* truncate both speed and position inside permited values*/
  if (longitude_speed < 0.0)     longspeed=0.0;
  else if (longitude_speed > MAX_SPEED_PANTILT)  longspeed=MAX_SPEED_PANTILT;
  else longspeed=longitude_speed;
  if (latitude_speed < 0.0)    latspeed=0.0;
  else if (latitude_speed > MAX_SPEED_PANTILT) latspeed=MAX_SPEED_PANTILT;
  else latspeed=latitude_speed;
 

  if (longitude > max_pan) longcommand = max_pan;
  else if (longitude < min_pan) longcommand = min_pan;
  else longcommand=longitude;
  if (latitude > max_tilt)  latcommand = max_tilt;
  else if (latitude < min_tilt) latcommand = min_tilt;
  else latcommand=latitude;

  if ( (longcommand!=longitude_last) || (longspeed != longspeed_last) ) {
    sprintf(pantilt_out,"PS%d PP%d\n",(int)(longspeed/ENCOD_TO_DEG),(int)(longcommand/ENCOD_TO_DEG));    
    SendCmd(pantilt_out);
    longitude_last=longcommand;
    longspeed_last=longspeed;
  }
  if( (latcommand!=latitude_last) || (latspeed!=latspeed_last) )  {
    sprintf(pantilt_out,"TS%d TP%d\n",(int)(latspeed/ENCOD_TO_DEG),(int)(latcommand/ENCOD_TO_DEG));
    SendCmd(pantilt_out);
    latitude_last=latcommand;
    latspeed_last=latspeed;
  }
 
  //if (debug[SCH_PANTILTMOTORS]) printf("pantiltmotors:  %1.1f %1.1f %1.1f %1.1f \n",latitude,longitude,latitude_speed,longitude_speed); 
}

int ptmotors_suspend()
{

  if (activate_pantiltmotors) {
    pthread_mutex_lock(&mymutex_motors);
    state_motors=slept;
    printf("pantiltmotors driver off\n");
    printf("ptmotors_suspend: pan_angle=%f - tilt_angle=%f\n",pan_angle,tilt_angle); 
    pthread_mutex_unlock(&mymutex_motors);
  }

  return 0;
}

int ptmotors_resume()
{

  if (activate_pantiltmotors) {

    printf("ptmotors_resume: pan_angle=%f - tilt_angle=%f\n",pan_angle,tilt_angle); 

    longitude=pan_angle;
    latitude=tilt_angle;         

    /* unfortunately it reads pan_angle an tilt_angle before sensationsoculo1
       has updated them for the first time, so this schema always starts 
       positioning the pantilt unit at (0,0) angles */ 

    pthread_mutex_lock(&mymutex_motors);
    state_motors=winner;
    printf("pantiltmotors driver on\n");
    pthread_cond_signal(&condition_motors);
    pthread_mutex_unlock(&mymutex_motors);
  }

  return 0;
}


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

void pantilt_init(){

  //printf("pantilt device on %s serial port\n",pantilt_device); 
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

    pantiltencoders_resume=ptencoders_resume;
    pantiltencoders_suspend=ptencoders_suspend;

  }

  if (activate_pantiltmotors) {

    printf("pantiltmotors driver started up\n");
    pthread_mutex_lock(&mymutex_motors);
    state_motors=slept;
    pthread_create(&pantilt_th,NULL,pantiltmotors_thread,NULL);
    pthread_mutex_unlock(&mymutex_motors);    

    pantiltmotors_resume=ptmotors_resume;
    pantiltmotors_suspend=ptmotors_suspend;
  }

  printf("pantilt driver started up\n");
}
