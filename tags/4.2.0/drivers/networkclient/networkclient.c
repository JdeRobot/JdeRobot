
/************************************************
 * jdec networkclient driver                    *
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../../core/jde.h"
#include "jdemessages.h"

/* read modes for images */
#define ONEIMAGE 0
#define MESSAGES 1

/* client name */
char CLIENT_NAME[MAX_CLIENT_NAME]="basicjde";

/* number of devices */
#define MAXDEVICE 10

/* constants for devices */
#define COLORA_DEVICE 0
#define COLORB_DEVICE 1
#define COLORC_DEVICE 2
#define COLORD_DEVICE 3
#define PANTILT_ENCODERS_DEVICE 4
#define PANTILT_MOTORS_DEVICE 5
#define LASER_DEVICE 6
#define SONARS_DEVICE 7
#define ENCODERS_DEVICE 8
#define MOTORS_DEVICE 9

/* 4 images, 4 robot devices and 2 devices for pantilt */
pthread_t network_thread[MAXDEVICE];
int state[MAXDEVICE];
pthread_mutex_t mymutex[MAXDEVICE];
pthread_cond_t condition[MAXDEVICE];

/* networkclient driver API options */
char driver_name[256]="networkclient";

/* devices detected and their hostnames, ports and network id's */
char hostname[MAXDEVICE][256];
int port[MAXDEVICE];
int device_network_id[MAXDEVICE];
int device_socket[MAXDEVICE];

/* to correct relative coordenates */
float correcting_x=0.;
float correcting_y=0.;
float correcting_theta=0.;

/* 0,1,2,3 used for colorA,colorB,colorC and colorD
   4,5 used for pantilt encoders and pantilt motors
   6,7,8,9 used for laser, sonars, encoders and motors */
int serve_device[MAXDEVICE];
int device_active[MAXDEVICE];
int networkclient_close_command=0;

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

int networkclient_pantiltencoders_resume(){

  if((device_active[PANTILT_ENCODERS_DEVICE==0])&&(serve_device[PANTILT_ENCODERS_DEVICE])){
    char message_out[MAX_MESSAGE];

    printf("networkclient: pantiltencoders resume\n");
    device_active[PANTILT_ENCODERS_DEVICE]=1;
    pthread_mutex_lock(&mymutex[PANTILT_ENCODERS_DEVICE]);
    sprintf(message_out,"%d\n",NETWORKSERVER_subscribe_pantilt_encoders); 
    write(device_socket[PANTILT_ENCODERS_DEVICE],message_out,strlen(message_out));
    state[PANTILT_ENCODERS_DEVICE]=winner;
    pthread_cond_signal(&condition[PANTILT_ENCODERS_DEVICE]);
    pthread_mutex_unlock(&mymutex[PANTILT_ENCODERS_DEVICE]);
  }

  return 0;
}

int networkclient_pantiltencoders_suspend(){

  if((device_active[PANTILT_ENCODERS_DEVICE])&&(serve_device[PANTILT_ENCODERS_DEVICE])){
    char message_out[MAX_MESSAGE];

    printf("networkclient: pantiltencoders suspend\n");
    device_active[PANTILT_ENCODERS_DEVICE]=0;
    pthread_mutex_lock(&mymutex[PANTILT_ENCODERS_DEVICE]);
    sprintf(message_out,"%d\n",NETWORKSERVER_unsubscribe_pantilt_encoders); 
    write(device_socket[PANTILT_ENCODERS_DEVICE],message_out,strlen(message_out));
    state[PANTILT_ENCODERS_DEVICE]=slept;
    pthread_mutex_unlock(&mymutex[PANTILT_ENCODERS_DEVICE]);
  }
  return 0;
}

int networkclient_pantiltmotors_resume(){

  if((device_active[PANTILT_MOTORS_DEVICE==0])&&(serve_device[PANTILT_MOTORS_DEVICE])){
    printf("networkclient: pantiltmotors resume\n");
    device_active[PANTILT_MOTORS_DEVICE]=1;
    pthread_mutex_lock(&mymutex[PANTILT_MOTORS_DEVICE]);
    state[PANTILT_MOTORS_DEVICE]=winner;
    pthread_cond_signal(&condition[PANTILT_MOTORS_DEVICE]);
    pthread_mutex_unlock(&mymutex[PANTILT_MOTORS_DEVICE]);
  }

  return 0;
}

int networkclient_pantiltmotors_suspend(){

  if((device_active[PANTILT_MOTORS_DEVICE])&&(serve_device[PANTILT_MOTORS_DEVICE])){
    printf("networkclient: pantiltmotors suspend\n");
    device_active[PANTILT_MOTORS_DEVICE]=0;
    pthread_mutex_lock(&mymutex[PANTILT_MOTORS_DEVICE]);
    state[PANTILT_MOTORS_DEVICE]=slept;
    pthread_mutex_unlock(&mymutex[PANTILT_MOTORS_DEVICE]);
  }
  return 0;
}

int networkclient_laser_resume(){

  if((device_active[LASER_DEVICE==0])&&(serve_device[LASER_DEVICE])){
    char message_out[MAX_MESSAGE];

    printf("networkclient: laser resume\n");
    device_active[LASER_DEVICE]=1;
    pthread_mutex_lock(&mymutex[LASER_DEVICE]);
    sprintf(message_out,"%d\n",NETWORKSERVER_subscribe_laser);
    write(device_socket[LASER_DEVICE],message_out,strlen(message_out));
    state[LASER_DEVICE]=winner;
    pthread_cond_signal(&condition[LASER_DEVICE]);
    pthread_mutex_unlock(&mymutex[LASER_DEVICE]);
  }
  return 0;
}

int networkclient_laser_suspend(){

  if((device_active[LASER_DEVICE])&&(serve_device[LASER_DEVICE])){
    char message_out[MAX_MESSAGE];

    printf("networkclient: laser suspend\n");
    device_active[LASER_DEVICE]=0;
    pthread_mutex_lock(&mymutex[LASER_DEVICE]);
    sprintf(message_out,"%d\n",NETWORKSERVER_unsubscribe_laser);
    write(device_socket[LASER_DEVICE],message_out,strlen(message_out));
    state[LASER_DEVICE]=slept;
    pthread_mutex_unlock(&mymutex[LASER_DEVICE]);
  }
  return 0;

}

int networkclient_encoders_resume(){

  if((device_active[ENCODERS_DEVICE]==0)&&(serve_device[ENCODERS_DEVICE])){
    char message_out[MAX_MESSAGE];

    printf("networkclient: encoders resume\n");
    device_active[ENCODERS_DEVICE]=1;
    pthread_mutex_lock(&mymutex[ENCODERS_DEVICE]);
    sprintf(message_out,"%d\n", NETWORKSERVER_subscribe_encoders);
    write(device_socket[ENCODERS_DEVICE],message_out,strlen(message_out));
    state[ENCODERS_DEVICE]=winner;
    pthread_cond_signal(&condition[ENCODERS_DEVICE]);
    pthread_mutex_unlock(&mymutex[ENCODERS_DEVICE]);
  }
  return 0;
}

int networkclient_encoders_suspend(){

  if((device_active[ENCODERS_DEVICE])&&(serve_device[ENCODERS_DEVICE])){
    char message_out[MAX_MESSAGE];

    printf("networkclient: laser suspend\n");
    device_active[ENCODERS_DEVICE]=0;
    pthread_mutex_lock(&mymutex[ENCODERS_DEVICE]);
    sprintf(message_out,"%d\n", NETWORKSERVER_unsubscribe_encoders);
    write(device_socket[ENCODERS_DEVICE],message_out,strlen(message_out));
    state[ENCODERS_DEVICE]=slept;
    pthread_mutex_unlock(&mymutex[ENCODERS_DEVICE]);
  }
  return 0;

}

int networkclient_sonars_resume(){

  if((device_active[SONARS_DEVICE]==0)&&(serve_device[SONARS_DEVICE])){
    char message_out[MAX_MESSAGE];

    printf("networkclient: sonars resume\n");
    device_active[SONARS_DEVICE]=1;
    pthread_mutex_lock(&mymutex[SONARS_DEVICE]);
    sprintf(message_out,"%d\n",NETWORKSERVER_subscribe_us);
    write(device_socket[SONARS_DEVICE],message_out,strlen(message_out));
    state[SONARS_DEVICE]=winner;
    pthread_cond_signal(&condition[SONARS_DEVICE]);
    pthread_mutex_unlock(&mymutex[SONARS_DEVICE]);
  }
  return 0;
}

int networkclient_sonars_suspend(){

  if((device_active[SONARS_DEVICE])&&(serve_device[SONARS_DEVICE])){
    char message_out[MAX_MESSAGE];

    printf("networkclient: sonars suspend\n");
    device_active[SONARS_DEVICE]=0;
    pthread_mutex_lock(&mymutex[SONARS_DEVICE]);
    sprintf(message_out,"%d\n",NETWORKSERVER_unsubscribe_us);
    write(device_socket[SONARS_DEVICE],message_out,strlen(message_out));
    state[SONARS_DEVICE]=slept;
    pthread_mutex_unlock(&mymutex[SONARS_DEVICE]);
  }
  return 0;

}

int networkclient_motors_resume(){

  if((device_active[MOTORS_DEVICE]==0)&&(serve_device[MOTORS_DEVICE])){
    printf("networkclient: motors resume\n");
    device_active[MOTORS_DEVICE]=1;
    pthread_mutex_lock(&mymutex[MOTORS_DEVICE]);
    state[MOTORS_DEVICE]=winner;
    pthread_cond_signal(&condition[MOTORS_DEVICE]);
    pthread_mutex_unlock(&mymutex[MOTORS_DEVICE]);
  }
  return 0;
}

int networkclient_motors_suspend(){

  if((device_active[MOTORS_DEVICE])&&(serve_device[MOTORS_DEVICE])){
    printf("networkclient: motors suspend\n");
    device_active[MOTORS_DEVICE]=0;
    pthread_mutex_lock(&mymutex[MOTORS_DEVICE]);
    state[MOTORS_DEVICE]=slept;
    pthread_mutex_unlock(&mymutex[MOTORS_DEVICE]);
  }
  return 0;

}

int networkclient_colorA_resume(){

  if((device_active[COLORA_DEVICE]==0)&&(serve_device[COLORA_DEVICE])){
    char message_out[MAX_MESSAGE];
    
    printf("networkclient: colorA resume\n");
    device_active[COLORA_DEVICE]=1;
    pthread_mutex_lock(&mymutex[COLORA_DEVICE]);
    sprintf(message_out,"%d %d\n",NETWORKSERVER_rgb24bpp_sifntsc_image_query,device_network_id[COLORA_DEVICE]);
    write(device_socket[COLORA_DEVICE],message_out,strlen(message_out));
    state[COLORA_DEVICE]=winner;
    pthread_cond_signal(&condition[COLORA_DEVICE]);
    pthread_mutex_unlock(&mymutex[COLORA_DEVICE]);
  }
  return 0;
}

int networkclient_colorA_suspend(){

  if((device_active[COLORA_DEVICE])&&(serve_device[COLORA_DEVICE])){
    printf("networkclient: colorA suspend\n");
    device_active[COLORA_DEVICE]=0;
    pthread_mutex_lock(&mymutex[COLORA_DEVICE]);
    state[COLORA_DEVICE]=slept;
    pthread_mutex_unlock(&mymutex[COLORA_DEVICE]);
  }
  return 0;

}

int networkclient_colorB_resume(){

  if((device_active[COLORB_DEVICE]==0)&&(serve_device[COLORB_DEVICE])){
    char message_out[MAX_MESSAGE];
    
    printf("networkclient: colorB resume\n");
    device_active[COLORB_DEVICE]=1;
    pthread_mutex_lock(&mymutex[COLORB_DEVICE]);
    sprintf(message_out,"%d %d\n",NETWORKSERVER_rgb24bpp_sifntsc_image_query,device_network_id[COLORB_DEVICE]);
    write(device_socket[COLORB_DEVICE],message_out,strlen(message_out));
    state[COLORB_DEVICE]=winner;
    pthread_cond_signal(&condition[COLORB_DEVICE]);
    pthread_mutex_unlock(&mymutex[COLORB_DEVICE]);
  }
  return 0;
}

int networkclient_colorB_suspend(){

  if((device_active[COLORB_DEVICE])&&(serve_device[COLORB_DEVICE])){
    printf("networkclient: colorB suspend\n");
    device_active[COLORB_DEVICE]=0;
    pthread_mutex_lock(&mymutex[COLORB_DEVICE]);
    state[COLORB_DEVICE]=slept;
    pthread_mutex_unlock(&mymutex[COLORB_DEVICE]);
  }
  return 0;

}

int networkclient_colorC_resume(){

  if((device_active[COLORC_DEVICE]==0)&&(serve_device[COLORC_DEVICE])){
    char message_out[MAX_MESSAGE];

    printf("networkclient: colorC resume\n");
    device_active[COLORC_DEVICE]=1;
    pthread_mutex_lock(&mymutex[COLORC_DEVICE]);
    sprintf(message_out,"%d %d\n",NETWORKSERVER_rgb24bpp_sifntsc_image_query,device_network_id[COLORC_DEVICE]);
    write(device_socket[COLORC_DEVICE],message_out,strlen(message_out));
    state[COLORC_DEVICE]=winner;
    pthread_cond_signal(&condition[COLORC_DEVICE]);
    pthread_mutex_unlock(&mymutex[COLORC_DEVICE]);
  }
  return 0;
}

int networkclient_colorC_suspend(){

  if((device_active[COLORC_DEVICE])&&(serve_device[COLORC_DEVICE])){
    printf("networkclient: colorC suspend\n");
    device_active[COLORC_DEVICE]=0;
    pthread_mutex_lock(&mymutex[COLORC_DEVICE]);
    state[COLORC_DEVICE]=slept;
    pthread_mutex_unlock(&mymutex[COLORC_DEVICE]);
  }
  return 0;

}

int networkclient_colorD_resume(){

  if((device_active[COLORD_DEVICE]==0)&&(serve_device[COLORD_DEVICE])){
    char message_out[MAX_MESSAGE];

    printf("networkclient: colorD resume\n");
    device_active[COLORD_DEVICE]=1;
    pthread_mutex_lock(&mymutex[COLORD_DEVICE]);
    sprintf(message_out,"%d %d\n",NETWORKSERVER_rgb24bpp_sifntsc_image_query,device_network_id[COLORD_DEVICE]);
    write(device_socket[COLORD_DEVICE],message_out,strlen(message_out));
    state[COLORD_DEVICE]=winner;
    pthread_cond_signal(&condition[COLORD_DEVICE]);
    pthread_mutex_unlock(&mymutex[COLORD_DEVICE]);
  }
  return 0;
}

int networkclient_colorD_suspend(){

  if((device_active[COLORD_DEVICE])&&(serve_device[COLORD_DEVICE])){
    printf("networkclient: colorD suspend\n");
    device_active[COLORD_DEVICE]=0;
    pthread_mutex_lock(&mymutex[COLORD_DEVICE]);
    state[COLORD_DEVICE]=slept;
    pthread_mutex_unlock(&mymutex[COLORD_DEVICE]);
  }
  return 0;
}

void *networkclient_pantiltencoders_thread(void *not_used){
  int j=0;
  long int readn=0,bytes_readn=0,start=0;
  int pantiltencoders_trac;
  char message_in[MAX_MESSAGE];
  char lastmessage[MAX_MESSAGE];

  printf("networkclient: networkclient pantiltencoders thread started\n");

  do{

    pthread_mutex_lock(&mymutex[PANTILT_ENCODERS_DEVICE]);

    if (state[PANTILT_ENCODERS_DEVICE]==slept){
      printf("networkclient: networkclient pantiltencoders thread goes sleep mode\n");
      pthread_cond_wait(&condition[PANTILT_ENCODERS_DEVICE],&mymutex[PANTILT_ENCODERS_DEVICE]);
      printf("networkclient: networkclient pantiltencoders thread woke up\n");
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
		kpantiltencoders++;
	      }
	    }
	  }
	}
      }
    }
  }while(networkclient_close_command==0);
  pthread_exit(0);
}

void *networkclient_pantiltmotors_thread(void *not_used){

  char pantiltmotors_out[MAX_MESSAGE];

  printf("networkclient: networkclient pantiltmotors thread started\n");

  do{

    kpantiltmotors++;
    pthread_mutex_lock(&mymutex[PANTILT_MOTORS_DEVICE]);

    if (state[PANTILT_MOTORS_DEVICE]==slept){
      printf("networkclient: networkclient pantiltmotors thread goes sleep mode\n");
      pthread_cond_wait(&condition[PANTILT_MOTORS_DEVICE],&mymutex[PANTILT_MOTORS_DEVICE]);
      printf("networkclient: networkclient pantiltmotors thread woke up\n");
      pthread_mutex_unlock(&mymutex[PANTILT_MOTORS_DEVICE]);
    }else{
            
      sprintf(pantiltmotors_out,"%ld %1.1f %1.1f %1.1f %1.1f\n", (long int)NETWORKSERVER_pantilt_position,longitude,latitude,longitude_speed,latitude_speed);
      write(device_socket[PANTILT_MOTORS_DEVICE],pantiltmotors_out,strlen(pantiltmotors_out));
      pthread_mutex_unlock(&mymutex[PANTILT_MOTORS_DEVICE]);
    }
  }while(networkclient_close_command==0);
  pthread_exit(0);
}

void *networkclient_laser_thread(void *not_used){

  int readn, beginning,i;
  unsigned long int network_clock;
  int type,l;
  int j=0,sensor;

  /* insert pointer on the input_buffer, in order to complete messages with more than one receptions */
  int laser_trac;
  char laser_in[MAX_MESSAGE];
  char *message;

  printf("networkclient: networkclient laser thread started\n");

  do{

    pthread_mutex_lock(&mymutex[LASER_DEVICE]);

    if (state[LASER_DEVICE]==slept){
      printf("networkclient: networkclient laser thread goes sleep mode\n");
      pthread_cond_wait(&condition[LASER_DEVICE],&mymutex[LASER_DEVICE]);
      printf("networkclient: networkclient laser thread woke up\n");
      pthread_mutex_unlock(&mymutex[LASER_DEVICE]);
    }else{
      
      pthread_mutex_unlock(&mymutex[LASER_DEVICE]);

      /* reading from socket. no overflow when using MAX_MESSAGE. messages are read in chunks. */
      readn=read(device_socket[LASER_DEVICE],&laser_in[laser_trac],MAX_MESSAGE);
      switch(readn){
      case 0: close(device_socket[LASER_DEVICE]); break; /* EOF: closing socket */
      case -1: ; break; /* nothing to read; */
      default:
	/* there is something in the socket. if received '\n' then a message has been completed. otherwise, we simply add the
	   received chunk to the input_buffer and we will completed in further calls. if many message's were completed, we serve
	   them all of them. we asume that the message in the buffer starts at the beginning of the buffer.*/
	{
	  beginning=0;
	  for(i=laser_trac; (i<MAX_MESSAGE)&&(i<laser_trac+readn);i++){
	    if (laser_in[i]=='\n'){
	      
	      message = &laser_in[beginning];
	      
	      if (sscanf(message,"%d %lu",&type,&network_clock)!=2){
		printf("networkclient: i don't understand message: (%s)\n",message);
		
	      }else{
		/* increments the 'j' pointer over the message till the first measure starts, jumping over the clock and type  */
		while((message[j]!='\n')&&(message[j]!=' ')&&(message[j]!='\0')) j++;
		j++;
		while((message[j]!='\n')&&(message[j]!=' ')&&(message[j]!='\0')) j++;

		if (type==NETWORKSERVER_laser)  
		  { 
		    klaser++;
		    sensor=0;
		    
		    while (message[j]==' '){/* reading sonars. it stops when buffer[j]=='\n' or '\0'*/
		      if (sscanf(&message[++j],"%d ",&l)!=1){
			printf("networkclient: conversion failed in laser measure conversion\n");
			
		      }else{ /* adds the measure and increments the pointer 'j' over the message */
			while((message[j]!='\n')&&(message[j]!=' ')&&(message[j]!='\0')) j++;
			/*j++;*/
			jde_laser[sensor++]=l;
			laser_clock=network_clock;
		      }
		    }
		  }
	      }	      
	      beginning=i+1;
	    }
	  }
	  
	  if (beginning==0){ /* message didn't end in this reception */
	    if (laser_trac+readn >= MAX_MESSAGE-1) laser_trac=0;
	    /* the reception buffer has been filled with the chunks of the received messages, but no message has been entirely completed. the current message will be ignored. LOOK OUT! the next message will start incorrectly as it will have the rest of the incompleted message at the beginning, and will be ignored too because it can't be understood. that way, when overflow occurs, both messages will be lost.*/
	    else laser_trac+=readn;
	    
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

void *networkclient_encoders_thread(void *not_used){

  int readn, beginning,i;
  unsigned long int network_clock;
  int type;
  int j=0;
  unsigned long nowtime; /* microsecs */
  float nowx, nowy, nowtheta;
  float a1,a2,a3;
  float tspeed, rspeed; /* mm/s, deg/s */
  unsigned long lasttime; /* microsecs */

  /* insert pointer on the input_buffer, in order to complete messages with more than one receptions */
  int encoders_trac;
  char encoders_in[MAX_MESSAGE];
  char *message;

  printf("networkclient: networkclient encoders thread started\n");

  do{

    pthread_mutex_lock(&mymutex[ENCODERS_DEVICE]);

    if (state[ENCODERS_DEVICE]==slept){
      printf("networkclient: networkclient encoders thread goes sleep mode\n");
      pthread_cond_wait(&condition[ENCODERS_DEVICE],&mymutex[ENCODERS_DEVICE]);
      printf("networkclient: networkclient encoders thread woke up\n");
      pthread_mutex_unlock(&mymutex[ENCODERS_DEVICE]);
    }else{
      
      pthread_mutex_unlock(&mymutex[ENCODERS_DEVICE]);

      /* reading from socket. no overflow when using MAX_MESSAGE. messages are read in chunks. */
      readn=read(device_socket[ENCODERS_DEVICE],&encoders_in[encoders_trac],MAX_MESSAGE);
      switch(readn){
      case 0: close(device_socket[ENCODERS_DEVICE]); break; /* EOF: closing socket */
      case -1: ; break; /* nothing to read; */
      default:
	/* there is something in the socket. if received '\n' then a message has been completed. otherwise, we simply add the
	   received chunk to the input_buffer and we will completed in further calls. if many message's were completed, we serve
	   them all of them. we asume that the message in the buffer starts at the beginning of the buffer.*/
	{
	  beginning=0;
	  for(i=encoders_trac; (i<MAX_MESSAGE)&&(i<encoders_trac+readn);i++){
	    if (encoders_in[i]=='\n'){
	      
	      message = &encoders_in[beginning];
	      
	      if (sscanf(message,"%d %lu",&type,&network_clock)!=2){
		printf("networkclient: i don't understand message: (%s)\n",message);
		
	      }else{
		/* increments the 'j' pointer over the message till the first measure starts, jumping over the clock and type  */
		while((message[j]!='\n')&&(message[j]!=' ')&&(message[j]!='\0')) j++;
		j++;
		while((message[j]!='\n')&&(message[j]!=' ')&&(message[j]!='\0')) j++;
	      
		if (type==NETWORKSERVER_encoders) 
		  { 
		    if (sscanf(&message[++j],"%f %f %f %lu",&nowx,&nowy,&nowtheta,&nowtime)!=4){
		      printf("networkclient: conversion failed in encoders measure conversion\n");
		      /* WATCH OUT WITH LOCALE CONFIGURATION!. With bad settings it expects float numbers to be composed of an
			 integer part and a decimal part separated by a single comma (not a single dot). In such a case the sscanf
			 fails when processing real numbers as those involved in encoders data, because networkserver sends them
			 using the dot. */
		      
		      printf("networkclient: this is a float for me: %f\n",3.141592654);
		      
		    }else{
		      kencoders++;
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

void *networkclient_sonars_thread(void *not_used){

  int readn, beginning,i;
  unsigned long int network_clock;
  int type;
  int j=0,sensor;
  float measure;

  /* insert pointer on the input_buffer, in order to complete messages with more than one receptions */
  int sonars_trac;
  char sonars_in[MAX_MESSAGE];
  char *message;

  printf("networkclient: networkclient sonars thread started\n");

  do{

    pthread_mutex_lock(&mymutex[SONARS_DEVICE]);

    if (state[SONARS_DEVICE]==slept){
      printf("networkclient: networkclient sonars thread goes sleep mode\n");
      pthread_cond_wait(&condition[SONARS_DEVICE],&mymutex[SONARS_DEVICE]);
      printf("networkclient: networkclient sonars thread woke up\n");
      pthread_mutex_unlock(&mymutex[SONARS_DEVICE]);
    }else{
      
      pthread_mutex_unlock(&mymutex[SONARS_DEVICE]);

      /* reading from socket. no overflow when using MAX_MESSAGE. messages are read in chunks. */
      readn=read(device_socket[SONARS_DEVICE],&sonars_in[sonars_trac],MAX_MESSAGE);
      switch(readn){
      case 0: close(device_socket[SONARS_DEVICE]); break; /* EOF: closing socket */
      case -1: ; break; /* nothing to read; */
      default:
	/* there is something in the socket. if received '\n' then a message has been completed. otherwise, we simply add the
	   received chunk to the input_buffer and we will completed in further calls. if many message's were completed, we serve
	   them all of them. we asume that the message in the buffer starts at the beginning of the buffer.*/
	{
	  beginning=0;
	  for(i=sonars_trac; (i<MAX_MESSAGE)&&(i<sonars_trac+readn);i++){
	    if (sonars_in[i]=='\n'){
	      
	      message = &sonars_in[beginning];
	      
	      if (sscanf(message,"%d %lu",&type,&network_clock)!=2){
		printf("networkclient: i don't understand message: (%s)\n",message);
		
	      }else{
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
			ksonars++;
			us[sensor]=measure;
			us_clock[sensor]=network_clock;
		      }
		    }
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

void *networkclient_motors_thread(void *not_used){
  float ac=0.;
  char message[MAX_MESSAGE];

  printf("networkclient: networkclient motors thread started\n");

  do{

    kmotors++;
    pthread_mutex_lock(&mymutex[MOTORS_DEVICE]);

    if (state[MOTORS_DEVICE]==slept){
      printf("networkclient: networkclient motors thread goes sleep mode\n");
      pthread_cond_wait(&condition[MOTORS_DEVICE],&mymutex[MOTORS_DEVICE]);
      printf("networkclient: networkclient motors thread woke up\n");
      pthread_mutex_unlock(&mymutex[MOTORS_DEVICE]);
    }else{
      
      /* sends to networkserver the speed control in mm/s, mms/s2 through a the device socket */

      sprintf(message,"%d %1.1f %1.1f\n",NETWORKSERVER_drive_speed,v,ac);
      write(device_socket[PANTILT_MOTORS_DEVICE],message,strlen(message));
      sprintf(message,"%d %1.1f %1.1f\n",NETWORKSERVER_steer_speed,w,ac);
      write(device_socket[PANTILT_MOTORS_DEVICE],message,strlen(message));

      pthread_mutex_unlock(&mymutex[MOTORS_DEVICE]);
    }
  }while(networkclient_close_command==0);
  pthread_exit(0);
}

void *networkclient_colorA_thread(void *not_used){
  int j=0;
  long int readn=0,bytes_readn=0,start=0;
  int read_mode=MESSAGES; int colorA_trac;
  /* the next buffer is used to receive the image that enters through the socket. the buffer is a bit greater because of the
     asynchronous read in chunks that is made in the socket in order to receive the incoming data. */
  char mmbuf[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3+MAX_MESSAGE];
  char message_in[MAX_MESSAGE];
  char message_out[MAX_MESSAGE];
  char lastmessage[MAX_MESSAGE];
  /*  fd_set dummy_set,read_set;*/

  printf("networkclient: networkclient colorA thread started\n");

  do{

    pthread_mutex_lock(&mymutex[COLORA_DEVICE]);

    if (state[COLORA_DEVICE]==slept){
      printf("networkclient: networkclient colorA thread goes sleep mode\n");
      pthread_cond_wait(&condition[COLORA_DEVICE],&mymutex[COLORA_DEVICE]);
      printf("networkclient: networkclient colorA thread woke up\n");
      pthread_mutex_unlock(&mymutex[COLORA_DEVICE]);
    }else{
      
      pthread_mutex_unlock(&mymutex[COLORA_DEVICE]);

      /* FD_ZERO(&dummy_set);
	 FD_ZERO(&read_set);
	 FD_SET(device_socket[COLORA_DEVICE],&read_set);
	 select(FD_SETSIZE,&read_set,&dummy_set,&dummy_set,NULL);*/

      if (read_mode==MESSAGES){
	readn=read(device_socket[COLORA_DEVICE],&(message_in[colorA_trac]),MAX_MESSAGE-colorA_trac);
      }else if (read_mode==ONEIMAGE){
	readn=read(device_socket[COLORA_DEVICE],&(mmbuf[colorA_trac]),SIFNTSC_COLUMNS*SIFNTSC_ROWS*3+MAX_MESSAGE-colorA_trac);
      }

      bytes_readn=readn;      
      switch(readn){
      case 0: exit(1); break;
      case -1: break; /* nothing to read */
      default: 
	if (read_mode==ONEIMAGE) { /* incomplete image */
	  if ((colorA_trac+bytes_readn)<(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3)){ 
	    colorA_trac+=bytes_readn;
	  }else{
	    read_mode=MESSAGES;
	    /*complete image. now we put in message mode */
	    strncpy(message_in,&(mmbuf[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3]),colorA_trac+bytes_readn-SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
	    bytes_readn=colorA_trac+bytes_readn-SIFNTSC_COLUMNS*SIFNTSC_ROWS*3;
	    /* preparing to read next image */
	    colorA_trac=0; start=0;
	    /* completing image received */
	    kA+=1.;
	    memcpy(colorA,mmbuf,SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
	    /* asking for next image */
	    pthread_mutex_lock(&mymutex[COLORA_DEVICE]);
	    sprintf(message_out,"%d %d\n",NETWORKSERVER_rgb24bpp_sifntsc_image_query,device_network_id[COLORA_DEVICE]);
	    write(device_socket[COLORA_DEVICE],message_out,strlen(message_out));
	    pthread_mutex_unlock(&mymutex[COLORA_DEVICE]);
	  }
	}
	  
	if (read_mode==MESSAGES){
	  start=0;
	  for(j=colorA_trac;(j<MAX_MESSAGE)&&(j<colorA_trac+bytes_readn)&&(read_mode==MESSAGES);j++){
	    if (message_in[j]=='\n'){
	      int type;
	      unsigned long int network_clock;
		
	      strncpy(lastmessage,&message_in[start],j-start);
	      lastmessage[j-start]='\0';
	      start=j+1;

	      if (sscanf(lastmessage,"%d %lu",&type,&network_clock)!=2) {
		printf("networkclient: i don't understand message from networkserver to colorA: (%s)\n",lastmessage);
	      }else{

		int i2,i3,i4,i5;
		int k=0;

		while((lastmessage[k]!='\n')&&(lastmessage[k]!=' ')&&(lastmessage[k]!='\0')) k++;
		k++;
		while((lastmessage[k]!='\n')&&(lastmessage[k]!=' ')&&(lastmessage[k]!='\0')) k++;
		if (type == NETWORKSERVER_rgb24bpp_sifntsc_image) {
		  sscanf(&lastmessage[++k],"%d %d %d %d\n",&i2,&i3,&i4,&i5);

		  if ((colorA_trac+bytes_readn-start)<SIFNTSC_COLUMNS*SIFNTSC_ROWS*3){
		    strncpy(mmbuf,&(message_in[start]),colorA_trac+bytes_readn-start);
		    read_mode=ONEIMAGE; 
		    colorA_trac=colorA_trac+bytes_readn-start;
		    /* we break to get out of the for and switch */
		    break; 
		  }else{ /* (colorA_trac+bytes_readn-start)>=SIFNTSC_COLUMNS*SIFNTSC_ROWS*1 */
		    strncpy(mmbuf,&(message_in[start]),SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
		    start=start+SIFNTSC_COLUMNS*SIFNTSC_ROWS*3;
		    j=j+SIFNTSC_COLUMNS*SIFNTSC_ROWS*3;
		    
		    /* image completed */
		    kA+=1.;
		    memcpy(colorA,mmbuf,SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
		    /* asking for next image */
		    pthread_mutex_lock(&mymutex[COLORA_DEVICE]);
		  sprintf(message_out,"%d %d\n",NETWORKSERVER_rgb24bpp_sifntsc_image_query,device_network_id[COLORA_DEVICE]);
		  write(device_socket[COLORA_DEVICE],message_out,strlen(message_out));
		  pthread_mutex_unlock(&mymutex[COLORA_DEVICE]);
		  }
		}
	      }
	    }
	  }
	}
	  
	if (read_mode==MESSAGES){
	  if ((colorA_trac+bytes_readn-start)>= MAX_MESSAGE){
	    colorA_trac=0; printf("networkclient: reception buffer overflow for colorA\n");
	  }else if ((colorA_trac+bytes_readn-start)>0){
	    strncpy(message_in,&(message_in[start]),colorA_trac+bytes_readn-start);
	    /* shift the remaining bytes of the chunk (the bytes don't complete a message) to the start position of the buffer. */
	    colorA_trac=colorA_trac+bytes_readn-start;
	  }
	}
      }
    }
  }while(networkclient_close_command==0);
  pthread_exit(0);
}

void *networkclient_colorB_thread(void *not_used){
  int j=0;
  long int readn=0,bytes_readn=0,start=0;
  int read_mode=MESSAGES; int colorB_trac;
  /* the next buffer is used to receive the image that enters through the socket. the buffer is a bit greater because of the
     asynchronous read in chunks that is made in the socket in order to receive the incoming data. */
  char mmbuf[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3+MAX_MESSAGE];
  char message_in[MAX_MESSAGE];
  char message_out[MAX_MESSAGE];
  char lastmessage[MAX_MESSAGE];
  /*  fd_set dummy_set,read_set;*/

  printf("networkclient: networkclient colorB thread started\n");

  do{

    pthread_mutex_lock(&mymutex[COLORB_DEVICE]);

    if (state[COLORB_DEVICE]==slept){
      printf("networkclient: networkclient colorB thread goes sleep mode\n");
      pthread_cond_wait(&condition[COLORB_DEVICE],&mymutex[COLORB_DEVICE]);
      printf("networkclient: networkclient colorB thread woke up\n");
      pthread_mutex_unlock(&mymutex[COLORB_DEVICE]);
    }else{
      
      pthread_mutex_unlock(&mymutex[COLORB_DEVICE]);

      /* FD_ZERO(&dummy_set);
	 FD_ZERO(&read_set);
	 FD_SET(device_socket[COLORB_DEVICE],&read_set);
	 select(FD_SETSIZE,&read_set,&dummy_set,&dummy_set,NULL);*/

      if (read_mode==MESSAGES){
	readn=read(device_socket[COLORB_DEVICE],&(message_in[colorB_trac]),MAX_MESSAGE-colorB_trac);
      }else if (read_mode==ONEIMAGE){
	readn=read(device_socket[COLORB_DEVICE],&(mmbuf[colorB_trac]),SIFNTSC_COLUMNS*SIFNTSC_ROWS*3+MAX_MESSAGE-colorB_trac);
      }

      bytes_readn=readn;      
      switch(readn){
      case 0: exit(1); break;
      case -1: break; /* nothing to read */
      default: 
	if (read_mode==ONEIMAGE) { /* incomplete image */
	  if ((colorB_trac+bytes_readn)<(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3)){ 
	    colorB_trac+=bytes_readn;
	  }else{
	    read_mode=MESSAGES;
	    /*complete image. now we put in message mode */
	    strncpy(message_in,&(mmbuf[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3]),colorB_trac+bytes_readn-SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
	    bytes_readn=colorB_trac+bytes_readn-SIFNTSC_COLUMNS*SIFNTSC_ROWS*3;
	    /* preparing to read next image */
	    colorB_trac=0; start=0;
	    /* completing image received */
	    kB+=1.;
	    memcpy(colorB,mmbuf,SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
	    /* asking for next image */
	    pthread_mutex_lock(&mymutex[COLORB_DEVICE]);
	    sprintf(message_out,"%d %d\n",NETWORKSERVER_rgb24bpp_sifntsc_image_query,device_network_id[COLORB_DEVICE]);
	    write(device_socket[COLORB_DEVICE],message_out,strlen(message_out));
	    pthread_mutex_unlock(&mymutex[COLORB_DEVICE]);
	  }
	}
	  
	if (read_mode==MESSAGES){
	  start=0;
	  for(j=colorB_trac;(j<MAX_MESSAGE)&&(j<colorB_trac+bytes_readn)&&(read_mode==MESSAGES);j++){
	    if (message_in[j]=='\n'){
	      int type;
	      unsigned long int network_clock;
		
	      strncpy(lastmessage,&message_in[start],j-start);
	      lastmessage[j-start]='\0';
	      start=j+1;

	      if (sscanf(lastmessage,"%d %lu",&type,&network_clock)!=2) {
		printf("networkclient: i don't understand message from networkserver to colorB: (%s)\n",lastmessage);
	      }else{

		int i2,i3,i4,i5;
		int k=0;

		while((lastmessage[k]!='\n')&&(lastmessage[k]!=' ')&&(lastmessage[k]!='\0')) k++;
		k++;
		while((lastmessage[k]!='\n')&&(lastmessage[k]!=' ')&&(lastmessage[k]!='\0')) k++;
		if (type == NETWORKSERVER_rgb24bpp_sifntsc_image) {
		  sscanf(&lastmessage[++k],"%d %d %d %d\n",&i2,&i3,&i4,&i5);

		  if ((colorB_trac+bytes_readn-start)<SIFNTSC_COLUMNS*SIFNTSC_ROWS*3){
		    strncpy(mmbuf,&(message_in[start]),colorB_trac+bytes_readn-start);
		    read_mode=ONEIMAGE; 
		    colorB_trac=colorB_trac+bytes_readn-start;
		    /* we break to get out of the for and switch */
		    break; 
		  }else{ /* (colorB_trac+bytes_readn-start)>=SIFNTSC_COLUMNS*SIFNTSC_ROWS*1 */
		    strncpy(mmbuf,&(message_in[start]),SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
		    start=start+SIFNTSC_COLUMNS*SIFNTSC_ROWS*3;
		    j=j+SIFNTSC_COLUMNS*SIFNTSC_ROWS*3;
		    
		    /* image completed */
		    kB+=1.;
		    memcpy(colorB,mmbuf,SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
		    /* asking for next image */
		    pthread_mutex_lock(&mymutex[COLORB_DEVICE]);
		  sprintf(message_out,"%d %d\n",NETWORKSERVER_rgb24bpp_sifntsc_image_query,device_network_id[COLORB_DEVICE]);
		  write(device_socket[COLORB_DEVICE],message_out,strlen(message_out));
		  pthread_mutex_unlock(&mymutex[COLORB_DEVICE]);
		  }
		}
	      }
	    }
	  }
	}
	  
	if (read_mode==MESSAGES){
	  if ((colorB_trac+bytes_readn-start)>= MAX_MESSAGE){
	    colorB_trac=0; printf("networkclient: reception buffer overflow for colorB\n");
	  }else if ((colorB_trac+bytes_readn-start)>0){
	    strncpy(message_in,&(message_in[start]),colorB_trac+bytes_readn-start);
	    /* shift the remaining bytes of the chunk (the bytes don't complete a message) to the start position of the buffer. */
	    colorB_trac=colorB_trac+bytes_readn-start;
	  }
	}
      }
    }
  }while(networkclient_close_command==0);
  pthread_exit(0);
}

void *networkclient_colorC_thread(void *not_used){
  int j=0;
  long int readn=0,bytes_readn=0,start=0;
  int read_mode=MESSAGES; int colorC_trac;
  /* the next buffer is used to receive the image that enters through the socket. the buffer is a bit greater because of the
     asynchronous read in chunks that is made in the socket in order to receive the incoming data. */
  char mmbuf[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3+MAX_MESSAGE];
  char message_in[MAX_MESSAGE];
  char message_out[MAX_MESSAGE];
  char lastmessage[MAX_MESSAGE];
  /*  fd_set dummy_set,read_set;*/

  printf("networkclient: networkclient colorC thread started\n");

  do{

    pthread_mutex_lock(&mymutex[COLORC_DEVICE]);

    if (state[COLORC_DEVICE]==slept){
      printf("networkclient: networkclient colorC thread goes sleep mode\n");
      pthread_cond_wait(&condition[COLORC_DEVICE],&mymutex[COLORC_DEVICE]);
      printf("networkclient: networkclient colorC thread woke up\n");
      pthread_mutex_unlock(&mymutex[COLORC_DEVICE]);
    }else{
      
      pthread_mutex_unlock(&mymutex[COLORC_DEVICE]);

      /* FD_ZERO(&dummy_set);
	 FD_ZERO(&read_set);
	 FD_SET(device_socket[COLORC_DEVICE],&read_set);
	 select(FD_SETSIZE,&read_set,&dummy_set,&dummy_set,NULL);*/

      if (read_mode==MESSAGES){
	readn=read(device_socket[COLORC_DEVICE],&(message_in[colorC_trac]),MAX_MESSAGE-colorC_trac);
      }else if (read_mode==ONEIMAGE){
	readn=read(device_socket[COLORC_DEVICE],&(mmbuf[colorC_trac]),SIFNTSC_COLUMNS*SIFNTSC_ROWS*3+MAX_MESSAGE-colorC_trac);
      }

      bytes_readn=readn;      
      switch(readn){
      case 0: exit(1); break;
      case -1: break; /* nothing to read */
      default: 
	if (read_mode==ONEIMAGE) { /* incomplete image */
	  if ((colorC_trac+bytes_readn)<(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3)){ 
	    colorC_trac+=bytes_readn;
	  }else{
	    read_mode=MESSAGES;
	    /*complete image. now we put in message mode */
	    strncpy(message_in,&(mmbuf[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3]),colorC_trac+bytes_readn-SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
	    bytes_readn=colorC_trac+bytes_readn-SIFNTSC_COLUMNS*SIFNTSC_ROWS*3;
	    /* preparing to read next image */
	    colorC_trac=0; start=0;
	    /* completing image received */
	    kC+=1.;
	    memcpy(colorC,mmbuf,SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
	    /* asking for next image */
	    pthread_mutex_lock(&mymutex[COLORC_DEVICE]);
	    sprintf(message_out,"%d %d\n",NETWORKSERVER_rgb24bpp_sifntsc_image_query,device_network_id[COLORC_DEVICE]);
	    write(device_socket[COLORC_DEVICE],message_out,strlen(message_out));
	    pthread_mutex_unlock(&mymutex[COLORC_DEVICE]);
	  }
	}
	  
	if (read_mode==MESSAGES){
	  start=0;
	  for(j=colorC_trac;(j<MAX_MESSAGE)&&(j<colorC_trac+bytes_readn)&&(read_mode==MESSAGES);j++){
	    if (message_in[j]=='\n'){
	      int type;
	      unsigned long int network_clock;
		
	      strncpy(lastmessage,&message_in[start],j-start);
	      lastmessage[j-start]='\0';
	      start=j+1;

	      if (sscanf(lastmessage,"%d %lu",&type,&network_clock)!=2) {
		printf("networkclient: i don't understand message from networkserver to colorC: (%s)\n",lastmessage);
	      }else{

		int i2,i3,i4,i5;
		int k=0;

		while((lastmessage[k]!='\n')&&(lastmessage[k]!=' ')&&(lastmessage[k]!='\0')) k++;
		k++;
		while((lastmessage[k]!='\n')&&(lastmessage[k]!=' ')&&(lastmessage[k]!='\0')) k++;
		if (type == NETWORKSERVER_rgb24bpp_sifntsc_image) {
		  sscanf(&lastmessage[++k],"%d %d %d %d\n",&i2,&i3,&i4,&i5);

		  if ((colorC_trac+bytes_readn-start)<SIFNTSC_COLUMNS*SIFNTSC_ROWS*3){
		    strncpy(mmbuf,&(message_in[start]),colorC_trac+bytes_readn-start);
		    read_mode=ONEIMAGE; 
		    colorC_trac=colorC_trac+bytes_readn-start;
		    /* we break to get out of the for and switch */
		    break; 
		  }else{ /* (colorC_trac+bytes_readn-start)>=SIFNTSC_COLUMNS*SIFNTSC_ROWS*1 */
		    strncpy(mmbuf,&(message_in[start]),SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
		    start=start+SIFNTSC_COLUMNS*SIFNTSC_ROWS*3;
		    j=j+SIFNTSC_COLUMNS*SIFNTSC_ROWS*3;
		    
		    /* image completed */
		    kC+=1.;
		    memcpy(colorC,mmbuf,SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
		    /* asking for next image */
		    pthread_mutex_lock(&mymutex[COLORC_DEVICE]);
		  sprintf(message_out,"%d %d\n",NETWORKSERVER_rgb24bpp_sifntsc_image_query,device_network_id[COLORC_DEVICE]);
		  write(device_socket[COLORC_DEVICE],message_out,strlen(message_out));
		  pthread_mutex_unlock(&mymutex[COLORC_DEVICE]);
		  }
		}
	      }
	    }
	  }
	}
	  
	if (read_mode==MESSAGES){
	  if ((colorC_trac+bytes_readn-start)>= MAX_MESSAGE){
	    colorC_trac=0; printf("networkclient: reception buffer overflow for colorC\n");
	  }else if ((colorC_trac+bytes_readn-start)>0){
	    strncpy(message_in,&(message_in[start]),colorC_trac+bytes_readn-start);
	    /* shift the remaining bytes of the chunk (the bytes don't complete a message) to the start position of the buffer. */
	    colorC_trac=colorC_trac+bytes_readn-start;
	  }
	}
      }
    }
  }while(networkclient_close_command==0);
  pthread_exit(0);
}

void *networkclient_colorD_thread(void *not_used){
  int j=0;
  long int readn=0,bytes_readn=0,start=0;
  int read_mode=MESSAGES; int colorD_trac;
  /* the next buffer is used to receive the image that enters through the socket. the buffer is a bit greater because of the
     asynchronous read in chunks that is made in the socket in order to receive the incoming data. */
  char mmbuf[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3+MAX_MESSAGE];
  char message_in[MAX_MESSAGE];
  char message_out[MAX_MESSAGE];
  char lastmessage[MAX_MESSAGE];
  /*  fd_set dummy_set,read_set;*/

  printf("networkclient: networkclient colorD thread started\n");

  do{

    pthread_mutex_lock(&mymutex[COLORD_DEVICE]);

    if (state[COLORD_DEVICE]==slept){
      printf("networkclient: networkclient colorD thread goes sleep mode\n");
      pthread_cond_wait(&condition[COLORD_DEVICE],&mymutex[COLORD_DEVICE]);
      printf("networkclient: networkclient colorD thread woke up\n");
      pthread_mutex_unlock(&mymutex[COLORD_DEVICE]);
    }else{
      
      pthread_mutex_unlock(&mymutex[COLORD_DEVICE]);

      /* FD_ZERO(&dummy_set);
	 FD_ZERO(&read_set);
	 FD_SET(device_socket[COLORD_DEVICE],&read_set);
	 select(FD_SETSIZE,&read_set,&dummy_set,&dummy_set,NULL);*/

      if (read_mode==MESSAGES){
	readn=read(device_socket[COLORD_DEVICE],&(message_in[colorD_trac]),MAX_MESSAGE-colorD_trac);
      }else if (read_mode==ONEIMAGE){
	readn=read(device_socket[COLORD_DEVICE],&(mmbuf[colorD_trac]),SIFNTSC_COLUMNS*SIFNTSC_ROWS*3+MAX_MESSAGE-colorD_trac);
      }

      bytes_readn=readn;      
      switch(readn){
      case 0: exit(1); break;
      case -1: break; /* nothing to read */
      default: 
	if (read_mode==ONEIMAGE) { /* incomplete image */
	  if ((colorD_trac+bytes_readn)<(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3)){ 
	    colorD_trac+=bytes_readn;
	  }else{
	    read_mode=MESSAGES;
	    /*complete image. now we put in message mode */
	    strncpy(message_in,&(mmbuf[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3]),colorD_trac+bytes_readn-SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
	    bytes_readn=colorD_trac+bytes_readn-SIFNTSC_COLUMNS*SIFNTSC_ROWS*3;
	    /* preparing to read next image */
	    colorD_trac=0; start=0;
	    /* completing image received */
	    kD+=1.;
	    memcpy(colorD,mmbuf,SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
	    /* asking for next image */
	    pthread_mutex_lock(&mymutex[COLORD_DEVICE]);
	    sprintf(message_out,"%d %d\n",NETWORKSERVER_rgb24bpp_sifntsc_image_query,device_network_id[COLORD_DEVICE]);
	    write(device_socket[COLORD_DEVICE],message_out,strlen(message_out));
	    pthread_mutex_unlock(&mymutex[COLORD_DEVICE]);
	  }
	}
	  
	if (read_mode==MESSAGES){
	  start=0;
	  for(j=colorD_trac;(j<MAX_MESSAGE)&&(j<colorD_trac+bytes_readn)&&(read_mode==MESSAGES);j++){
	    if (message_in[j]=='\n'){
	      int type;
	      unsigned long int network_clock;
		
	      strncpy(lastmessage,&message_in[start],j-start);
	      lastmessage[j-start]='\0';
	      start=j+1;

	      if (sscanf(lastmessage,"%d %lu",&type,&network_clock)!=2) {
		printf("networkclient: i don't understand message from networkserver to colorD: (%s)\n",lastmessage);
	      }else{

		int i2,i3,i4,i5;
		int k=0;

		while((lastmessage[k]!='\n')&&(lastmessage[k]!=' ')&&(lastmessage[k]!='\0')) k++;
		k++;
		while((lastmessage[k]!='\n')&&(lastmessage[k]!=' ')&&(lastmessage[k]!='\0')) k++;
		if (type == NETWORKSERVER_rgb24bpp_sifntsc_image) {
		  sscanf(&lastmessage[++k],"%d %d %d %d\n",&i2,&i3,&i4,&i5);

		  if ((colorD_trac+bytes_readn-start)<SIFNTSC_COLUMNS*SIFNTSC_ROWS*3){
		    strncpy(mmbuf,&(message_in[start]),colorD_trac+bytes_readn-start);
		    read_mode=ONEIMAGE; 
		    colorD_trac=colorD_trac+bytes_readn-start;
		    /* we break to get out of the for and switch */
		    break; 
		  }else{ /* (colorD_trac+bytes_readn-start)>=SIFNTSC_COLUMNS*SIFNTSC_ROWS*1 */
		    strncpy(mmbuf,&(message_in[start]),SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
		    start=start+SIFNTSC_COLUMNS*SIFNTSC_ROWS*3;
		    j=j+SIFNTSC_COLUMNS*SIFNTSC_ROWS*3;
		    
		    /* image completed */
		    kD+=1.;
		    memcpy(colorD,mmbuf,SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
		    /* asking for next image */
		    pthread_mutex_lock(&mymutex[COLORD_DEVICE]);
		  sprintf(message_out,"%d %d\n",NETWORKSERVER_rgb24bpp_sifntsc_image_query,device_network_id[COLORD_DEVICE]);
		  write(device_socket[COLORD_DEVICE],message_out,strlen(message_out));
		  pthread_mutex_unlock(&mymutex[COLORD_DEVICE]);
		  }
		}
	      }
	    }
	  }
	}
	  
	if (read_mode==MESSAGES){
	  if ((colorD_trac+bytes_readn-start)>= MAX_MESSAGE){
	    colorD_trac=0; printf("networkclient: reception buffer overflow for colorD\n");
	  }else if ((colorD_trac+bytes_readn-start)>0){
	    strncpy(message_in,&(message_in[start]),colorD_trac+bytes_readn-start);
	    /* shift the remaining bytes of the chunk (the bytes don't complete a message) to the start position of the buffer. */
	    colorD_trac=colorD_trac+bytes_readn-start;
	  }
	}
      }
    }
  }while(networkclient_close_command==0);
  pthread_exit(0);
}

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
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s %s %s %s",word3,word4,word5,word6,word7)>4){
		      
		      if((strcmp(word4,"colorA")==0)&&(serve_device[COLORA_DEVICE]==0)){
			serve_device[COLORA_DEVICE]=1;
			device_active[COLORA_DEVICE]=0;
			strcpy(hostname[COLORA_DEVICE],word5);
			port[COLORA_DEVICE]=atoi(word6);
			device_network_id[COLORA_DEVICE]=atoi(word7);
		      }
		      else if((strcmp(word4,"colorB")==0)&&(serve_device[COLORB_DEVICE]==0)){
			serve_device[COLORB_DEVICE]=1;
			device_active[COLORB_DEVICE]=0;
			strcpy(hostname[COLORB_DEVICE],word5);
			port[COLORB_DEVICE]=atoi(word6);
			device_network_id[COLORB_DEVICE]=atoi(word7);
		      }
		      else if((strcmp(word4,"colorC")==0)&&(serve_device[COLORC_DEVICE]==0)){
			serve_device[COLORC_DEVICE]=1;
			device_active[COLORC_DEVICE]=0;
			strcpy(hostname[COLORC_DEVICE],word5);
			port[COLORC_DEVICE]=atoi(word6);
			device_network_id[COLORC_DEVICE]=atoi(word7);
		      }
		      else if((strcmp(word4,"colorD")==0)&&(serve_device[COLORD_DEVICE]==0)){
			serve_device[COLORD_DEVICE]=1;
			device_active[COLORD_DEVICE]=0;
			strcpy(hostname[COLORD_DEVICE],word5);
			port[COLORD_DEVICE]=atoi(word6);
			device_network_id[COLORD_DEVICE]=atoi(word7);
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
      case COLORA_DEVICE: pthread_create(&network_thread[COLORA_DEVICE],NULL,networkclient_colorA_thread,NULL); break;
      case COLORB_DEVICE: pthread_create(&network_thread[COLORB_DEVICE],NULL,networkclient_colorB_thread,NULL); break;
      case COLORC_DEVICE: pthread_create(&network_thread[COLORC_DEVICE],NULL,networkclient_colorC_thread,NULL); break;
      case COLORD_DEVICE: pthread_create(&network_thread[COLORD_DEVICE],NULL,networkclient_colorD_thread,NULL); break;
      case PANTILT_ENCODERS_DEVICE: pthread_create(&network_thread[PANTILT_ENCODERS_DEVICE],NULL,networkclient_pantiltencoders_thread,NULL); break;
      case PANTILT_MOTORS_DEVICE: pthread_create(&network_thread[PANTILT_MOTORS_DEVICE],NULL,networkclient_pantiltmotors_thread,NULL); break;
      case LASER_DEVICE: pthread_create(&network_thread[LASER_DEVICE],NULL,networkclient_laser_thread,NULL); break;
      case SONARS_DEVICE: pthread_create(&network_thread[SONARS_DEVICE],NULL,networkclient_sonars_thread,NULL); break;
      case ENCODERS_DEVICE: pthread_create(&network_thread[ENCODERS_DEVICE],NULL,networkclient_encoders_thread,NULL); break;
      case MOTORS_DEVICE: pthread_create(&network_thread[MOTORS_DEVICE],NULL,networkclient_motors_thread,NULL); break;
      }
      pthread_mutex_unlock(&mymutex[i]);
    }
  }

  return 0;
}

void networkclient_startup(char *configfile)
{
  int i;

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
  if(serve_device[COLORA_DEVICE]){imageA_resume=networkclient_colorA_resume; imageA_suspend=networkclient_colorA_suspend;}
  if(serve_device[COLORB_DEVICE]){imageB_resume=networkclient_colorB_resume; imageB_suspend=networkclient_colorB_suspend;}
  if(serve_device[COLORC_DEVICE]){imageC_resume=networkclient_colorC_resume; imageC_suspend=networkclient_colorC_suspend;}
  if(serve_device[COLORD_DEVICE]){imageD_resume=networkclient_colorD_resume; imageD_suspend=networkclient_colorD_suspend;}
  if(serve_device[PANTILT_ENCODERS_DEVICE]){pantiltencoders_resume=networkclient_pantiltencoders_resume; pantiltencoders_suspend=networkclient_pantiltencoders_suspend;}
  if(serve_device[PANTILT_MOTORS_DEVICE]){pantiltmotors_resume=networkclient_pantiltmotors_resume; pantiltmotors_suspend=networkclient_pantiltmotors_suspend;}
  if(serve_device[LASER_DEVICE]){laser_resume=networkclient_laser_resume; laser_suspend=networkclient_laser_suspend;}
  if(serve_device[ENCODERS_DEVICE]){encoders_resume=networkclient_encoders_resume; encoders_suspend=networkclient_encoders_suspend;}
  if(serve_device[SONARS_DEVICE]){sonars_resume=networkclient_sonars_resume; sonars_suspend=networkclient_sonars_suspend;}
  if(serve_device[MOTORS_DEVICE]){motors_resume=networkclient_motors_resume; motors_suspend=networkclient_motors_suspend;}

  printf("networkclient driver started up\n");
}
