
/************************************************
 * jdec networkserver driver                    *
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../../core/jde.h"

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

pthread_t network_thread;
int state;
pthread_mutex_t mymutex;
pthread_cond_t condition;

/* networkserver driver API options */
char driver_name[256]="networkserver";

/* port to bind the server */
int networkserver_port=0;

/* devices detected and their network id */
int serve_device[MAXDEVICE];
int device_network_id[MAXDEVICE];

/* other vars */
int display_fps=0;
int networkserver_close_command=0;

/* if we need to show fps */
void networkserver_display_fps(){
  display_fps=1;
}

void networkserver_close(){

  printf("driver networkserver off\n");
}

int networkserver_resume(){

  if(state==slept){
    printf("networkserver: server resume\n");
    pthread_mutex_lock(&mymutex);
    state=winner;

    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mymutex);
  }
  return 0;
}

int networkserver_suspend(){

  if(state==winner){
    printf("networkserver: server suspend\n");
    pthread_mutex_lock(&mymutex);
    state=slept;

    pthread_mutex_unlock(&mymutex);
  }
  return 0;
}

void *networkserver_thread(void *not_used){
  unsigned char *src,*dest;
  static float fps=0, fpsold=0;
  struct timeval t;
  static unsigned long int then;
  unsigned long int now;

  printf("networkserver: networkserver thread started\n");

  do{

    pthread_mutex_lock(&mymutex);

    if (state==slept){
      printf("networkserver: networkserver thread goes sleep mode\n");
      pthread_cond_wait(&condition,&mymutex);
      printf("networkserver: networkserver thread woke up\n");
      pthread_mutex_unlock(&mymutex);
    }else{
      
      pthread_mutex_unlock(&mymutex);

      /* if displaying fps needed */
      if(display_fps==1){
	gettimeofday(&t,NULL);
	now=t.tv_sec*1000000+t.tv_usec;
	/*lastimage = now;*/
	if ((now-then)>2000000) /* periodo de integracion, de cuenta de imagenes: 2 segundos, mas o menos */
	  {fps=fps*1000000/(now-then);
	    if ((fps>fpsold+1)||(fps<fpsold-1)) 
	      {fpsold=fps;}
	    fps=0.;
	    then=now;
	  }
	else fps++;
      }

    }
  }while(networkserver_close_command==0);
  pthread_exit(0);
}

int networkserver_parseconf(char *configfile){

  int end_parse=0; int end_section=0; int driver_config_parsed=0;
  FILE *myfile;
  const int limit = 256;

  if ((myfile=fopen(configfile,"r"))==NULL){
    printf("networkserver: cannot find config file\n");
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
	printf ("networkserver: line too long in config file!\n"); 
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
		    printf("networkserver: error in config file.\n'end_section' keyword required before starting new driver section.\n");
		    end_section=1; end_parse=1;

		  }else if(strcmp(word3,"serves")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s %s",word3,word4,word5)>2){
		      
		      if((strcmp(word4,"colorA")==0)&&(serve_device[COLORA_DEVICE]==0)){
			serve_device[COLORA_DEVICE]=1;
			device_network_id[COLORA_DEVICE]=atoi(word5);
		      }
		      else if((strcmp(word4,"colorB")==0)&&(serve_device[COLORB_DEVICE]==0)){
			serve_device[COLORB_DEVICE]=1;
			device_network_id[COLORB_DEVICE]=atoi(word5);
		      }
		      else if((strcmp(word4,"colorC")==0)&&(serve_device[COLORC_DEVICE]==0)){
			serve_device[COLORC_DEVICE]=1;
			device_network_id[COLORC_DEVICE]=atoi(word5);
		      }
		      else if((strcmp(word4,"colorD")==0)&&(serve_device[COLORD_DEVICE]==0)){
			serve_device[COLORD_DEVICE]=1;
			device_network_id[COLORD_DEVICE]=atoi(word5);
		      }
		      else{
			printf("networkserver: serves line incorrect\n");
		      }
		    }
		    else if(sscanf(buffer_file2,"%s %s",word3,word4)>1){
		      
		      if((strcmp(word4,"laser")==0)&&(serve_device[LASER_DEVICE]==0)){serve_device[LASER_DEVICE]=1;}
		      else if((strcmp(word4,"sonars")==0)&&(serve_device[SONARS_DEVICE]==0)){serve_device[SONARS_DEVICE]=1;}
		      else if((strcmp(word4,"encoders")==0)&&(serve_device[ENCODERS_DEVICE]==0)){serve_device[ENCODERS_DEVICE]=1;}
		      else if((strcmp(word4,"motors")==0)&&(serve_device[MOTORS_DEVICE]==0)){serve_device[MOTORS_DEVICE]=1;}
		      else if((strcmp(word4,"pantiltencoders")==0)&&(serve_device[PANTILT_ENCODERS_DEVICE]==0)){serve_device[PANTILT_ENCODERS_DEVICE]=1;}
		      else if((strcmp(word4,"pantiltmotors")==0)&&(serve_device[PANTILT_MOTORS_DEVICE]==0)){serve_device[PANTILT_MOTORS_DEVICE]=1;}
		      else{printf("networkserver: serves line incorrect\n");}
		    }

		  }else if(strcmp(word3,"socket")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s",word3,word4)>1){
		      networkserver_port=atoi(word4);
		      printf("networkserver: server will be started using port %d\n",networkserver_port);
		    }else{
		      printf("networkserver: socket line incorrect\n");
		    }
		  }else printf("networkserver: i don't know what to do with '%s'\n",buffer_file2);
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
	  printf("networkserver: warning! no device served.\n");
	}
      }
    }
    return 0;
  }else return -1;
}

int networkserver_init(){

  return 0;
}

void networkserver_startup(char *configfile)
{
  int i;

  /* reseting serve color array and setting default options */
  for(i=0;i<MAXDEVICE;i++){serve_device[i]=0; device_network_id[i]=0;}

  /* we call the function to parse the config file */
  if(networkserver_parseconf(configfile)==-1){
    printf("networkserver: cannot initiate driver. configfile parsing error.\n");
    exit(-1);
  }

  /* networkserver driver init */
  if(networkserver_init()!=0){
    printf("networkserver: cannot initiate driver. devices not ready or not supported.\n");
    exit(-1);
  }

  printf("networkserver driver started up\n");
}
