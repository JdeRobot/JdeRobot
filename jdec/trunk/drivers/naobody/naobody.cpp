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
/** Motion */
motion* m;

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

/*Motion parameters*/
/*pantiltencoers*/
int serve_pantiltencoders;
int pantiltencoders_active;

/*pantiltmotors*/
int serve_pantiltmotors;
int pantiltmotors_active;

int serve_motion;
int motion_active;
int motion_schema_id;
int pantiltencoders_schema_id;
int pantiltmotors_schema_id;

/*API variables servidas*/
float vnao;
float wnao;
float pnao;
float ynao;
float preal;
float yreal;
float vy;
float vp;
unsigned long int my_clock;
float cycle;
float max_longitude;
float max_latitude;
float min_longitude;
float min_latitude;
float max_y_speed;
float max_p_speed;

/*API variables servidas*/
Varcolor myA;
//float headVal;

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

int motion_run(int father, int *brothers, arbitration fn){	
		if((serve_motion==1)&&(motion_active==0)){
			motion_active=1;
			printf("MotionMotors schema run (naobody driver)\n");
			all[motion_schema_id].father = father;
			all[motion_schema_id].fps = 0.0;
			all[motion_schema_id].k =0;
			put_state(motion_schema_id,winner);
			/* naoqi thread goes winner */
			pthread_mutex_lock(&mymutex);
			state=winner;
			pthread_cond_signal(&condition);
			pthread_mutex_unlock(&mymutex);
		}
	return 0;
}


int motion_stop(){
	if((serve_motion==1)&&(motion_active==1)){
		motion_active=0;
		printf("MotionMotors schema stop (naobody driver)\n");
		put_state(motion_schema_id,slept);
		/* naoqi thread goes sleep */
		pthread_mutex_lock(&mymutex);
		state=slept;
		pthread_mutex_unlock(&mymutex);
	}
	return 0;
}


int pantiltencoders_run(int father, int *brothers, arbitration fn){		
		if((serve_pantiltencoders==1)&&(pantiltencoders_active==0)){
			pantiltencoders_active=1;
			printf("pantiltencoders schema run (naobody driver)\n");
			all[pantiltencoders_schema_id].father = father;
			all[pantiltencoders_schema_id].fps = 0.0;
			all[pantiltencoders_schema_id].k =0;
			put_state(pantiltencoders_schema_id,winner);
			/* naoqi thread goes winner */
			pthread_mutex_lock(&mymutex);
			state=winner;
			pthread_cond_signal(&condition);
			pthread_mutex_unlock(&mymutex);
		}
	return 0;
}

int pantiltencoders_stop(){
	if((serve_pantiltencoders==1)&&(pantiltencoders_active==1)){
		pantiltencoders_active=0;
		printf("pantiltencoders schema stop (naobody driver)\n");
		put_state(pantiltencoders_schema_id,slept);
		/* naoqi thread goes sleep */
		pthread_mutex_lock(&mymutex);
		state=slept;
		pthread_mutex_unlock(&mymutex);
	}
	return 0;
}


int pantiltmotors_run(int father, int *brothers, arbitration fn){		
		if((serve_pantiltmotors==1)&&(pantiltmotors_active==0)){
			pantiltmotors_active=1;
			printf("pantiltmotors schema run (naobody driver)\n");
			all[pantiltmotors_schema_id].father = father;
			all[pantiltmotors_schema_id].fps = 0.0;
			all[pantiltmotors_schema_id].k =0;
			put_state(pantiltmotors_schema_id,winner);
			/* naoqi thread goes winner */
			pthread_mutex_lock(&mymutex);
			state=winner;
			pthread_cond_signal(&condition);
			pthread_mutex_unlock(&mymutex);
		}
	return 0;
}

int pantiltmotors_stop(){
	if((serve_pantiltmotors==1)&&(pantiltmotors_active==1)){
		pantiltmotors_active=0;
		printf("pantiltmotors schema stop (naobody driver)\n");
		put_state(pantiltmotors_schema_id,slept);
		/* naoqi thread goes sleep */
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

			/* Move body*/
			if(motion_active) {
				speedcounter(motion_schema_id);
				walkmotion(m,vnao,wnao);
			}
			if ((pantiltencoders_active)||(pantiltmotors_active)){
				if (pantiltencoders_active)
					speedcounter(pantiltencoders_schema_id);
				if (pantiltmotors_active)
					speedcounter(pantiltmotors_schema_id);
				headmotion(m,ynao,pnao,&yreal,&preal,vy,vp,&my_clock);
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
								} else 	if (words==2){
									if(strcmp(word4,"body")==0){
										serve_motion=1;
									}
									else if (strcmp(word4,"ptencoders")==0){
										serve_pantiltencoders=1;
									}
									else if (strcmp(word4,"ptmotors")==0){
										serve_pantiltmotors=1;
									}
									else
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
		if(serve_color==0 /*&& serve_head==0*/)
			printf("naobody: warning! neither color nor head motor provided.\n");
		return 0;
	} else 
		return -1;
}

void naobody_terminate(){
	naobody_terminate_command=1;
	if(serve_color) {
		deleteCamera(cam);
	}
	if((serve_motion)||(serve_pantiltencoders)||serve_pantiltmotors) {
		deletemotion(m);	
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
	if((serve_motion)||(serve_pantiltencoders)||(serve_pantiltmotors)) {
		m = newmotion();
		if(m == NULL) {
			fprintf(stderr, "Cannot create motion motors\n");
			return -1;
		}
	}
	
	return 0;
}

/** naobody driver init function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void naobody_init(char *configfile)
{
	serve_color=0;
	number_color=-1;
	color_active=0;
	serve_motion=0;
	motion_active=0;
	serve_pantiltencoders=0;
	pantiltencoders_active=0;
	serve_pantiltmotors=0;
	pantiltmotors_active=0;

	width = DEFAULT_COLUMNS;
	height = DEFAULT_ROWS;
	fps = DEFAULT_FPS;
	pnao=0;
	ynao=0;
	preal=0;
	yreal=0;
	vp=0;
	vy=0;
	my_clock=0;
	cycle=0;
	max_longitude=MAXY;
	max_latitude=MAXP;
	min_longitude=-MAXY;
	min_latitude=-MAXP;
	max_y_speed=MAXVY;
	max_p_speed=MAXVY;
	
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
    if (serve_color || (serve_motion)||(serve_pantiltencoders)||(serve_pantiltmotors)){
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

	/*Creates a new schema for motion*/
	if(serve_motion==1) {
		all[num_schemas].id = (int *) &motion_schema_id;
		strcpy(all[num_schemas].name,"MotionMotors");
		all[num_schemas].run = (runFn) motion_run;
		all[num_schemas].stop = (stopFn) motion_stop;
		printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
		(*(all[num_schemas].id)) = num_schemas;
		all[num_schemas].fps = 0.;
		all[num_schemas].k =0;
		all[num_schemas].state=slept;
		all[num_schemas].terminate = NULL;
		all[num_schemas].handle = NULL;
		num_schemas++;
		vnao = 0.0;
		wnao= 0.0;
		myexport("MotionMotors","v",&vnao);
		myexport("MotionMotors","w",&wnao);
		myexport("MotionMotors","id",&motion_schema_id);
		myexport("MotionMotors","run",(void *)motion_run);
		myexport("MotionMotors","stop",(void *)motion_stop);
	}
	if (serve_pantiltencoders==1){
		all[num_schemas].id = (int *) &pantiltencoders_schema_id;
		strcpy(all[num_schemas].name,"ptencoders");
		all[num_schemas].run = (runFn) pantiltencoders_run;
		all[num_schemas].stop = (stopFn) pantiltencoders_stop;
		printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
		(*(all[num_schemas].id)) = num_schemas;
		all[num_schemas].fps = 0.;
		all[num_schemas].k =0;
		all[num_schemas].state=slept;
		all[num_schemas].terminate = NULL;
		all[num_schemas].handle = NULL;
		num_schemas++;
		myexport("ptencoders","pan_angle",&yreal);
		myexport("ptencoders","tilt_angle",&preal);
		myexport("ptencoders","clock",&my_clock);
		myexport("ptencoders","id",&pantiltencoders_schema_id);
		myexport("ptencoders","run",(void *)pantiltencoders_run);
		myexport("ptencoders","stop",(void *)pantiltencoders_stop);
	}
	if (serve_pantiltmotors==1){
		all[num_schemas].id = (int *) &pantiltmotors_schema_id;
		strcpy(all[num_schemas].name,"ptmotors");
		all[num_schemas].run = (runFn) pantiltmotors_run;
		all[num_schemas].stop = (stopFn) pantiltmotors_stop;
		printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
		(*(all[num_schemas].id)) = num_schemas;
		all[num_schemas].fps = 0.;
		all[num_schemas].k =0;
		all[num_schemas].state=slept;
		all[num_schemas].terminate = NULL;
		all[num_schemas].handle = NULL;
		num_schemas++;
		myexport("ptmotors","cycle",&cycle);
		myexport("ptmotors","longitude",&ynao);
		myexport("ptmotors","latitude",&pnao);
		myexport("ptmotors","max_longitude",&max_longitude);
		myexport("ptmotors","max_latitude",&max_latitude);
		myexport("ptmotors","min_longitude",&min_longitude);
		myexport("ptmotors","min_latitude",&min_latitude);
		myexport("ptmotors","longitude_speed",&vy);
		myexport("ptmotors","latitude_speed",&vp);
		myexport("ptmotors","max_longitud_speed",&max_y_speed);
		myexport("ptmotors","max_latitud_speed",&max_p_speed);
		myexport("ptmotors","id",&pantiltmotors_schema_id);
		myexport("ptmotors","run",(void *)pantiltmotors_run);
		myexport("ptmotors","stop",(void *)pantiltmotors_stop);
	}
  
	printf("naobody driver started up\n");
}
