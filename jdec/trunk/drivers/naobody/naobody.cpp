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
 *  Authors : 	Eduardo Perdices <edupergar@gmail.com>
 *				Francisco Rivas <fm.rivas@alumnos.urjc.es>
 *
 */

#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "adapter.h"
#include <jde.h>
#include "naobody.h"
#include <interfaces/varcolor.h>

/*Image size used by default*/
#define DEFAULT_ROWS 240
#define DEFAULT_COLUMNS 320
#define DEFAULT_FPS 5

/** Camera */
Camera * cam = NULL;
/** Head */
Head * head = NULL;

/** pthread identifiers for jdec naobody driver threads.*/
pthread_t naobody_th;
int args;
int state;
/** pthread mutex for jdec firewire driver.*/
pthread_mutex_t mymutex;
/** pthread condition variable for jdec firewire driver.*/
pthread_cond_t condition;

int naobody_terminate_command=0;

//Datos de la maquina a la que conectamos
char IP[20];
int PORT;

/** int variable to detect when a image was captured.*/
unsigned long int lastimage=0;

char driver_name[256]="naobody";

/*Camera parameters*/
int width;
int height;
int fps;
int serve_color;
int color_active;
int number_color;
int varcolorA_schema_id;

/*Head parameters*/
int serve_head;
int head_active;
int head_schema_id;

/*API variables servidas*/
Varcolor myA;
float headVal;

/** varcolorA run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorA_run(int father, int *brothers, arbitration fn){		
	if((serve_color==1)&&(color_active==0)){
		color_active=1;
		printf("varcolorA schema run (naobody driver)\n");
		all[varcolorA_schema_id].father = father;
		all[varcolorA_schema_id].fps = 0.0;
		all[varcolorA_schema_id].k =0;
		put_state(varcolorA_schema_id,winner);
		/* naobody thread goes winner */
		pthread_mutex_lock(&mymutex);
		state=winner;
		pthread_cond_signal(&condition);
		pthread_mutex_unlock(&mymutex);
	}
	return 0;
}

/** varcolorA stop function following jdec platform API schemas.		
 *  @return integer stopping result.*/
int myvarcolorA_stop(){
	if((serve_color==1)&&(color_active==1)){
		color_active=0;
		printf("varcolorA schema stop (naobody driver)\n");
		put_state(varcolorA_schema_id,slept);
		/* naobody thread goes sleep */
		pthread_mutex_lock(&mymutex);
		state=slept;
		pthread_mutex_unlock(&mymutex);
	}
	return 0;
}

/** head run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int head_run(int father, int *brothers, arbitration fn){		
	if((serve_head==1)&&(head_active==0)){
		head_active=1;
		printf("HeadMotors schema run (naobody driver)\n");
		all[head_schema_id].father = father;
		all[head_schema_id].fps = 0.0;
		all[head_schema_id].k =0;
		put_state(head_schema_id,winner);
		/* naobody thread goes winner */
		pthread_mutex_lock(&mymutex);
		state=winner;
		pthread_cond_signal(&condition);
		pthread_mutex_unlock(&mymutex);
	}
	return 0;
}

/** head stop function following jdec platform API schemas.		
 *  @return integer stopping result.*/
int head_stop(){
	if((serve_head==1)&&(head_active==1)){
		head_active=0;
		printf("HeadMotors schema stop (naobody driver)\n");
		put_state(head_schema_id,slept);
		/* naobody thread goes sleep */
		pthread_mutex_lock(&mymutex);
		state=slept;
		pthread_mutex_unlock(&mymutex);
	}
	return 0;
}

/** naobody driver pthread function.*/
void *naobody_thread(void *id)
{
	struct timeval t;
	unsigned long now,next;
	static unsigned long lastiteration = 0;
	long stop;
	int i;
	i=*((int*)id);

	printf("naobody driver started up: number %d\n", i);

	do{
        
		pthread_mutex_lock(&mymutex);

		if (state==slept){
			printf("naobody thread in sleep mode\n");
			pthread_cond_wait(&condition,&mymutex);
			printf("naobody thread woke up\n");
			pthread_mutex_unlock(&mymutex);    
		}else{
			pthread_mutex_unlock(&mymutex);
			/*Controlamos el tiempo*/
			gettimeofday (&t, NULL);
			now = t.tv_sec * 1000000 + t.tv_usec;
			lastiteration = now;

			/*Get image from camera*/
			if(color_active) {
				speedcounter(varcolorA_schema_id);
				/*Refrescamos la imagen*/
				updateImageCamera(cam);
				/*Actualizamos los valores de VarColorA*/
				getImageCamera(cam, (unsigned char *)myA.img);
				myA.width = getWidthCamera(cam);
				myA.height = getHeightCamera(cam);
				myA.clock=lastimage;
				lastimage++;
			}

			/* Move head*/
			if(head_active) {
				speedcounter(head_schema_id);
				/*Actualizamos la posicion de la cabeza del robot*/
				moveToHead(head, headVal);
			}

			/* to control the iteration time of this driver */
			gettimeofday (&t, NULL);
			now = t.tv_sec * 1000000 + t.tv_usec;
			stop = (long)(1000.0/(float)fps);
			next=lastiteration+(long)stop*1000;
			if (next>(5000+now)) {
				usleep(next-now-5000);
			}
		}
	}while(naobody_terminate_command==0);

	pthread_exit(0);
}

/** Determines if an image configuration could be captured or not
 *  @param width width of the possible image
 *  @param height height of the possible image
 *  @return 1 if the size could be captured, otherwise 0
 */
int size_ok(int width, int height){
	if (width==640 && height==480)
		return 1;
	else if (width==320 && height==240)
		return 1;
	else if (width==160 && height==120)
		return 1;
	else
		return 0;
}

/** naobody driver parse configuration file function.
 *  @param configfile path and name to the config file.
 *  @return 0 if parsing was successful or -1 if something went wrong.*/
int naobody_parseconf(char *configfile){

   int end_parse=0; int end_section=0; int driver_config_parsed=0;
   FILE *myfile;
   const int limit = 256;

   if ((myfile=fopen(configfile,"r"))==NULL){
      printf("naobody: cannot find config file\n");
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
            printf ("naobody: line too long in config file!\n");
            return -1;
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
	      
                     char buffer_file2[256],word3[256],word4[256];
                     char word5[256],word6[256], word7[256];
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
								while((buffer_file2[z]!='\n')&&
                                     (buffer_file2[z]!=' ')&&
                                     (buffer_file2[z]!='\0')&&
                                     (buffer_file2[z]!='\t')) z++;
								printf("naobody: error in config file.\n'end_section' keyword required before starting new driver section.\n");
								end_section=1; end_parse=1;

							}else if(strcmp(word3,"provides")==0){
								int words;
								while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&
									  (buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) 
									z++;
								words=sscanf(buffer_file2,"%s %s %s %s %s",word3,word4,word5,word6,word7);
								if (words==5){
									if(strcmp(word4,"varcolorA")==0){
										serve_color=1;
										number_color=atoi(word5);
										width = atoi(word6);
										height = atoi(word7);
										/*Puede que haya que comprobar si ya esta en uso cuando haya mas de  ####################*/
										if (!size_ok(width, height)){
											fprintf (stderr, "Wrong image size for varcolorA, changed to default size 320X240\n");
											width = DEFAULT_COLUMNS;
											height = DEFAULT_ROWS;
										}
									} else
										printf("naobody: provides line incorrect\n");
								} else if (words==2) {
									if(strcmp(word4,"head")==0){
										serve_head=1;
									} else
										printf("naobody: provides line incorrect\n");
								}
							} else if(strcmp(word3,"fps")==0){
								int words;
								while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&
									  (buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) 
									z++;
								words=sscanf(buffer_file2,"%s %s",word3,word4);
								if(words==2) {
										fps = atoi(word4);
										if(fps < 1 || fps > 20) {
											fprintf (stderr, "Wrong fps value, changed to default rate 5 fps\n");
											fps = DEFAULT_FPS;
										}
								} else
										printf("naobody: fps line incorrect\n");
							}else printf("naobody: i don't know what to do with '%s'\n",buffer_file2);
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
		if(serve_color==0 && serve_head==0)
			printf("naobody: warning! neither color nor head motor provided.\n");
		return 0;
	} else 
		return -1;
}

void naobody_terminate(){
	naobody_terminate_command=1;
	if(serve_head) {
		//Liberamos la cabeza motora
		deleteHead(head);
	}
	if(serve_color) {
		//Liberamos la camara de naobody
		deleteCamera(cam);
	}
	printf("driver naobody off\n");
}

/** naobody driver init function. It will start all naobody required devices
 *  and setting them the default configuration.
 *  @return 0 if initialitation was successful or -1 if something went wrong.*/
int naobody_deviceinit(){

	if(serve_color) {
		/*Intentamos obtener la camara*/
		cam = newCamera(width, height, fps);
		if (cam == NULL) {
			fprintf(stderr, "Cannot create camera\n");
			return -1;
		}
	
		/*Update width and height*/
		width = getWidthCamera(cam);
		height = getHeightCamera(cam);
	}
	if(serve_head) {
		printf("inicializamos el head\n");
		head = newHead();
		if(head == NULL) {
			fprintf(stderr, "Cannot create head motors\n");
			return -1;
		}
	}
	
	return 0;
}

/** naobody driver init function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void naobody_init(char *configfile)
{
	width = DEFAULT_COLUMNS;
	height = DEFAULT_ROWS;
	fps = DEFAULT_FPS;
	serve_color=0;
	number_color=-1;
	color_active=0;
	serve_head=0;
	head_active=0;
	
	/* we call the function to parse the config file */
	if(naobody_parseconf(configfile)==-1){
		printf("naobody: cannot initiate driver. configfile parsing error.\n");
		exit(-1);
	}

	/* naobody initialitation */
	if(naobody_deviceinit()) {
		printf("naobody: cannot initiate driver.\n");
		exit(-1);
	}		

	pthread_mutex_lock(&mymutex);
	state=slept;
    if (serve_color || serve_head){
		args=0;
		/*Call to function thread*/
		pthread_create(&naobody_th,NULL,naobody_thread,(void*)&args);
	} 
	pthread_mutex_unlock(&mymutex);

	/*creates new schema for varcolorA*/
	if(serve_color==1){
		all[num_schemas].id = (int *) &varcolorA_schema_id;
		strcpy(all[num_schemas].name,"varcolorA");
		all[num_schemas].run = (runFn) myvarcolorA_run;
		all[num_schemas].stop = (stopFn) myvarcolorA_stop;
		printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
		(*(all[num_schemas].id)) = num_schemas;
		all[num_schemas].fps = 0.;
		all[num_schemas].k =0;
		all[num_schemas].state=slept;
		all[num_schemas].terminate = NULL;
		all[num_schemas].handle = NULL;
		num_schemas++;

		myA.width=width;
		myA.height=height;
		myA.img=(char*)malloc(myA.width*myA.height*3*sizeof(char));
		myA.clock=0;
		myexport("varcolorA","varcolorA",&myA);
		myexport("varcolorA","id",&varcolorA_schema_id);
		myexport("varcolorA","run",(void *)myvarcolorA_run);
		myexport("varcolorA","stop",(void *)myvarcolorA_stop);
	}

	/*Creates a new schema for head motors*/
	if(serve_head==1) {
		all[num_schemas].id = (int *) &head_schema_id;
		strcpy(all[num_schemas].name,"HeadMotors");
		all[num_schemas].run = (runFn) head_run;
		all[num_schemas].stop = (stopFn) head_stop;
		printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
		(*(all[num_schemas].id)) = num_schemas;
		all[num_schemas].fps = 0.;
		all[num_schemas].k =0;
		all[num_schemas].state=slept;
		all[num_schemas].terminate = NULL;
		all[num_schemas].handle = NULL;
		num_schemas++;

		headVal = 0.0;
		myexport("HeadMotors","value",&headVal);
		myexport("HeadMotors","id",&head_schema_id);
		myexport("HeadMotors","run",(void *)head_run);
		myexport("HeadMotors","stop",(void *)head_stop);
	}
  
	printf("naobody driver started up\n");
}
