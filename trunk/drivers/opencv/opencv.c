/*
 *  Copyright (C) 2006 José María Cañas Plaza
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

/**
 * jdec opencv driver provides video images to color variables from opencv
 * cameras using opencv library.
 *
 * This 4.3 version includes support for variable images. 
 *
 * @file opencv.c
 * @author Sara Marugán Alonso <s.marugan@alumnos.urjc.es>
 * @version 4.3.0
 * @date 2009-02-13
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "jde.h"
#include <interfaces/varcolor.h>

/** Max number of cameras detected by opencv driver.*/
#define MAXCAM 8
/* uncomment the following to drop frames to prevent delays */
/** Dropping frames define.*/
#define DROP_FRAMES 1
/** Max ports per node define.*/
#define MAX_PORTS 3
/** Max number of buffers per node.*/
#define NUM_BUFFERS 8

/** Image standard number of rows*/
#define SIFNTSC_ROWS 240
/** Image standard number of columns*/
#define SIFNTSC_COLUMNS 320

/*input types*/
#define VIDEO_FILE 0
#define VIDEO_DEVICE 1
#define IMAGE_FILE 2

/* camera struct */
typedef struct{
  int frame_width;
  int frame_height;
  char name[256];
  int input_type;
}opencv_camera;

/* declarations for opencv cameras */
static opencv_camera cameras[MAXCAM];
/** number of opencv cameras detected.*/
int numCameras=0;
int camCount=0;
/** int variable to detect when a image was captured.*/
unsigned long int lastimage=0;

/** pthread identifiers for jdec opencv driver threads.*/
pthread_t opencv_th[MAXCAM];
/** Arguments for opencv threads*/
int args[MAXCAM];
/** pthread state variable for jdec opencv driver.*/
int state[MAXCAM];
/** pthread mutex for jdec opencv driver.*/
pthread_mutex_t mymutex[MAXCAM];
/** pthread condition variable for jdec opencv driver.*/
pthread_cond_t condition[MAXCAM];

/** variable to detect when the pthread is created.*/
int opencv_thread_created[MAXCAM];
/** variable to detect when opencv driver was cleaned up.*/
int opencv_cleaned_up=0;
/** variable to detect when opencv driver was setup.*/
int opencv_setup=0;
/** variable to detect when pthread must end execution.*/
int opencv_terminate_command=0;

/* opencv driver API options */
/** opencv driver name.*/
char driver_name[256]="opencv";
/** colors detected in config file.*/
int serve_color[MAXCAM];
/** structure to know what colors are active in the gui.*/
int color_active[MAXCAM];

/** width of each served video**/
int width[MAXCAM];
/** height of each served video**/
int height[MAXCAM];
/** id set to colorA schema.*/
int colorA_schema_id;
/** id set to colorB schema.*/
int colorB_schema_id;
/** id set to colorC schema.*/
int colorC_schema_id;
/** id set to colorD schema.*/
int colorD_schema_id;
/** id for varcolorA schema.*/
int varcolorA_schema_id;
/** id for varcolorB schema.*/
int varcolorB_schema_id;
/** id for varcolorC schema.*/
int varcolorC_schema_id;
/** id for varcolorD schema.*/
int varcolorD_schema_id;

/*API variables servidas*/
Varcolor myA,myB,myC,myD; /* for varcolorA,varcolorB... */

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

/*Contadores de referencias*/
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

/** mutex for ref counters*/
pthread_mutex_t refmutex;


/* OPENCV DRIVER FUNCTIONS */

/** opencv driver closing function invoked when stopping driver.*/
void opencv_terminate(){
  opencv_terminate_command=1;
  printf("driver opencv off\n");
}

/** colorA run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorA_run(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (colorA_refs>0){
      colorA_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorA_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[0]==1)&&(color_active[0]==0)){
         color_active[0]=1;
         printf("colorA schema run (opencv driver)\n");
         all[colorA_schema_id].father = father;
         all[colorA_schema_id].fps = 0.;
         all[colorA_schema_id].k =0;
         all[colorA_schema_id].k =0;
         put_state(colorA_schema_id,winner);
         /* opencv thread goes winner */
         pthread_mutex_lock(&mymutex[0]);
         state[0]=winner;
         pthread_cond_signal(&condition[0]);
         pthread_mutex_unlock(&mymutex[0]);
      }
   }
   return 0;
}

/** colorA stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int mycolorA_stop(){

   pthread_mutex_lock(&refmutex);
   if (colorA_refs>1){
      colorA_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorA_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[0]==1)&&(color_active[0]==1)){
         color_active[0]=0;
         put_state(colorA_schema_id,slept);
         printf("colorA schema stop (opencv driver)\n");
         /* opencv thread goes sleep */
         pthread_mutex_lock(&mymutex[0]);
         state[0]=slept;
         pthread_mutex_unlock(&mymutex[0]);
      }
   }
   return 0;
}

/** colorB run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorB_run(int father, int *brothers, arbitration fn){

   pthread_mutex_lock(&refmutex);
   if (colorB_refs>0){
      colorB_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorB_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[1]==1)&&(color_active[1]==0)){
         color_active[1]=1;
         printf("colorB schema run (opencv driver)\n");
         all[colorB_schema_id].father = father;
         all[colorB_schema_id].fps = 0.;
         all[colorB_schema_id].k =0;
   
         put_state(colorB_schema_id,winner);

         /* opencv thread goes winner */
         pthread_mutex_lock(&mymutex[1]);
         state[1]=winner;
         pthread_cond_signal(&condition[1]);
         pthread_mutex_unlock(&mymutex[1]);
      }
   }
   return 0;
}

/** colorB stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int mycolorB_stop(){

   pthread_mutex_lock(&refmutex);
   if (colorB_refs>1){
      colorB_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorB_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[1]==1)&&(color_active[1]==1)){
         color_active[1]=0;
         printf("colorB schema stop (opencv driver)\n");
         put_state(colorB_schema_id,slept);
         /* opencv thread goes sleep */
         pthread_mutex_lock(&mymutex[1]);
         state[1]=slept;
         pthread_mutex_unlock(&mymutex[1]);
      }
   }
   return 0;
}

/** colorC run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorC_run(int father, int *brothers, arbitration fn){

   pthread_mutex_lock(&refmutex);
   if (colorC_refs>0){
      colorC_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorC_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[2]==1)&&(color_active[2]==0)){
         color_active[2]=1;
         printf("colorC schema run (opencv driver)\n");
         all[colorC_schema_id].father = father;
         all[colorC_schema_id].fps = 0.;
         all[colorC_schema_id].k =0;
         put_state(colorC_schema_id,winner);
         /* opencv thread goes winner */
         pthread_mutex_lock(&mymutex[2]);
         state[2]=winner;
         pthread_cond_signal(&condition[2]);
         pthread_mutex_unlock(&mymutex[2]);
      }
   }
   return 0;
}

/** colorC stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int mycolorC_stop(){
   
   pthread_mutex_lock(&refmutex);
   if (colorC_refs>1){
      colorC_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorC_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[2]==1)&&(color_active[2]==1)){
         color_active[2]=0;
         printf("colorC schema stop (opencv driver)\n");
         put_state(colorC_schema_id,slept);
         /* opencv thread goes sleep */
         pthread_mutex_lock(&mymutex[2]);
         state[2]=slept;
         pthread_mutex_unlock(&mymutex[2]);
      }
   }
   return 0;
}

/** colorD run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorD_run(int father, int *brothers, arbitration fn){

   pthread_mutex_lock(&refmutex);
   if (colorD_refs>0){
      colorD_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorD_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[3]==1)&&(color_active[3]==0)){
         color_active[3]=1;
         printf("colorD schema run (opencv driver)\n");
         all[colorD_schema_id].father = father;
         all[colorD_schema_id].fps = 0.;
         all[colorD_schema_id].k =0;
         put_state(colorD_schema_id,winner);
         /* opencv thread goes winner */
         pthread_mutex_lock(&mymutex[3]);
         state[3]=winner;
         pthread_cond_signal(&condition[3]);
         pthread_mutex_unlock(&mymutex[3]);
      }
   }
   return 0;
}

/** colorD stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int mycolorD_stop(){
   
   pthread_mutex_lock(&refmutex);
   if (colorD_refs>1){
      colorD_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorD_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[3]==1)&&(color_active[3]==1)){
         color_active[3]=0;
         printf("colorD schema stop (opencv driver)\n");
         put_state(colorD_schema_id,slept);
         /* opencv thread goes sleep */
         pthread_mutex_lock(&mymutex[3]);
         state[3]=slept;
         pthread_mutex_unlock(&mymutex[3]);
      }
   }
   return 0;
}

/** varcolorA run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorA_run(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (varcolorA_refs>0){
      varcolorA_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorA_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[4]==1)&&(color_active[4]==0)){
         color_active[4]=1;
         printf("varcolorA schema run (opencv driver)\n");
         all[varcolorA_schema_id].father = father;
         all[varcolorA_schema_id].fps = 0.;
         all[varcolorA_schema_id].k =0;
         put_state(varcolorA_schema_id,winner);
         /* opencv thread goes winner */
         pthread_mutex_lock(&mymutex[4]);
         state[4]=winner;
         pthread_cond_signal(&condition[4]);
         pthread_mutex_unlock(&mymutex[4]);
      }
   }
   return 0;
}

/** varcolorA stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int myvarcolorA_stop(){
   pthread_mutex_lock(&refmutex);
   if (varcolorA_refs>1){
      varcolorA_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorA_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[4]==1)&&(color_active[4]==1)){
         color_active[4]=0;
         printf("varcolorA schema stop (opencv driver)\n");
         put_state(varcolorA_schema_id,slept);
         /* opencv thread goes sleep */
         pthread_mutex_lock(&mymutex[4]);
         state[4]=slept;
         pthread_mutex_unlock(&mymutex[4]);
      }
   }
   return 0;
}

/** varcolorB run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorB_run(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (varcolorB_refs>0){
      varcolorB_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorB_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[5]==1)&&(color_active[5]==0)){
         color_active[5]=1;
         printf("varcolorB schema run (opencv driver)\n");
         all[varcolorB_schema_id].father = father;
         all[varcolorB_schema_id].fps = 0.;
         all[varcolorB_schema_id].k =0;
         put_state(varcolorB_schema_id,winner);
         /* opencv thread goes winner */
         pthread_mutex_lock(&mymutex[5]);
         state[5]=winner;
         pthread_cond_signal(&condition[5]);
         pthread_mutex_unlock(&mymutex[5]);
      }
   }
   return 0;
}

/** varcolorB stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int myvarcolorB_stop(){
   pthread_mutex_lock(&refmutex);
   if (varcolorB_refs>1){
      varcolorB_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorB_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[5]==1)&&(color_active[5]==1)){
         color_active[5]=0;
         printf("varcolorB schema stop (opencv driver)\n");
         put_state(varcolorB_schema_id,slept);
         /* opencv thread goes sleep */
         pthread_mutex_lock(&mymutex[5]);
         state[5]=slept;
         pthread_mutex_unlock(&mymutex[5]);
      }
   }
   return 0;
}

/** varcolorC run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorC_run(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (varcolorC_refs>0){
      varcolorC_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorC_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[6]==1)&&(color_active[6]==0)){
         color_active[6]=1;
         printf("varcolorC schema run (opencv driver)\n");
         all[varcolorC_schema_id].father = father;
         all[varcolorC_schema_id].fps = 0.;
         all[varcolorC_schema_id].k =0;
         put_state(varcolorC_schema_id,winner);
         /* opencv thread goes winner */
         pthread_mutex_lock(&mymutex[6]);
         state[6]=winner;
         pthread_cond_signal(&condition[6]);
         pthread_mutex_unlock(&mymutex[6]);
      }
   }
   return 0;
}

/** varcolorC stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int myvarcolorC_stop(){
   pthread_mutex_lock(&refmutex);
   if (varcolorC_refs>1){
      varcolorC_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorC_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[6]==1)&&(color_active[6]==1)){
         color_active[6]=0;
         printf("varcolorC schema stop (opencv driver)\n");
         put_state(varcolorC_schema_id,slept);
         /* opencv thread goes sleep */
         pthread_mutex_lock(&mymutex[6]);
         state[6]=slept;
         pthread_mutex_unlock(&mymutex[6]);
      }
   }
   return 0;
}

/** varcolorD run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorD_run(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (varcolorD_refs>0){
      varcolorD_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorD_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[7]==1)&&(color_active[7]==0)){
         color_active[7]=1;
         printf("varcolorD schema run (opencv driver)\n");
         all[varcolorD_schema_id].father = father;
         all[varcolorD_schema_id].fps = 0.;
         all[varcolorD_schema_id].k =0;
         put_state(varcolorD_schema_id,winner);
         /* opencv thread goes winner */
         pthread_mutex_lock(&mymutex[7]);
         state[7]=winner;
         pthread_cond_signal(&condition[7]);
         pthread_mutex_unlock(&mymutex[7]);
      }
   }
   return 0;
}

/** varcolorD stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int myvarcolorD_stop(){
   pthread_mutex_lock(&refmutex);
   if (varcolorD_refs>1){
      varcolorD_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorD_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[7]==1)&&(color_active[7]==1)){
         color_active[7]=0;
         printf("varcolorD schema stop (opencv driver)\n");
         put_state(varcolorD_schema_id,slept);
         /* opencv thread goes sleep */
         pthread_mutex_lock(&mymutex[7]);
         state[7]=slept;
         pthread_mutex_unlock(&mymutex[7]);
      }
   }
   return 0;
}

/** opencv driver pthread function.*/
void *opencv_thread(void *id)
{
   int i,r;
   int captured=FALSE;
   i=*((int*)id);
   char *color;
   CvCapture* myCapture = NULL;
   IplImage *frame = NULL;
   IplImage *frame_aux = NULL;
   double fps_videofile;
   struct timeval a, b;
   long n=0; /* iteration */
   long next,bb,aa;
   int input_type=cameras[i].input_type;
   int diff_size=-1;
   double t1,tiempo;

   /* initilize captures */
   switch (input_type) {
		case VIDEO_DEVICE:
			 myCapture = cvCreateCameraCapture(atoi(&cameras[i].name[strlen(cameras[i].name)-1])); 
			 frame = cvCreateImage(cvSize(cameras[i].frame_width, cameras[i].frame_height), IPL_DEPTH_8U, 3);
			 break;
		case VIDEO_FILE:
			 myCapture = cvCreateFileCapture(cameras[i].name);
			 frame = cvCreateImage(cvSize(cameras[i].frame_width, cameras[i].frame_height), IPL_DEPTH_8U, 3);
			 break;
		case IMAGE_FILE:
			 frame_aux=cvLoadImage(cameras[i].name,1);
		         if(frame_aux!=NULL){
				 frame = cvCreateImage(cvSize(cameras[i].frame_width, cameras[i].frame_height), IPL_DEPTH_8U, 3);
				 if(different_size(cameras[i],frame_aux->width,frame_aux->height)){
					cvResize(frame_aux,frame,CV_INTER_LINEAR);
				 }
				 else
					cvCopy(frame_aux,frame,NULL);
			 }
			 else
				 frame=NULL;
			 break;
  }

  if (myCapture == NULL && frame == NULL){
     printf ("Error in %d thread: can't initialize image\n",i);
     pthread_exit(0);
  }
  
  printf("opencv driver started up: number %d\n", i);


  /* MAIN LOOP */
  while(opencv_terminate_command==0){
        
    pthread_mutex_lock(&mymutex[i]);

    if (state[i]==slept){
      printf("opencv thread in sleep mode\n");
      pthread_cond_wait(&condition[i],&mymutex[i]);
      printf("opencv thread woke up\n");
      pthread_mutex_unlock(&mymutex[i]);
      
    }else{
      
      pthread_mutex_unlock(&mymutex[i]);

      gettimeofday(&a, NULL);
      aa=a.tv_sec*1000000+a.tv_usec;
      n=0;

      if(color_active[i]){
	        if(input_type!=IMAGE_FILE){
               		if( (frame_aux = cvQueryFrame(myCapture))!=NULL){
				if(diff_size==-1)
					diff_size=different_size(cameras[i],frame_aux->width,frame_aux->height);
				if(diff_size){
					cvResize(frame_aux,frame,CV_INTER_LINEAR);
				}
				else{
					cvCopy(frame_aux,frame,NULL);
				}
				captured=TRUE;
			}
			else if(input_type==VIDEO_FILE){
				cvReleaseCapture(&myCapture);
				myCapture = cvCreateFileCapture(cameras[i].name);
			}
	        }
                else captured=TRUE;
      }

      switch (i){
		 case 0:
		    color=colorA;
		    break;
		 case 1:
		    color=colorB;
		    break;
		 case 2:
		    color=colorC;
		    break;
		 case 3:
		    color=colorD;
		    break;
		 case 4:
		    color=myA.img;
		    break;
		 case 5:
		    color=myB.img;
		    break;
		 case 6:
		    color=myC.img;
		    break;
		 case 7:
		    color=myD.img;
		    break;
		 default:
		    fprintf(stderr, "opencv_thread: Unknown option %d.\n", i);
      }

      if(input_type==VIDEO_FILE){
	 fps_videofile=cvGetCaptureProperty(myCapture,CV_CAP_PROP_FPS);
         gettimeofday(&b,NULL);
         bb=b.tv_sec*1000000+b.tv_usec;
         next=aa+(n+1)*(long)(1000/fps_videofile)*1000-bb;
         if (next>1000) {
            usleep(next-1000);
            /* discounts 10ms taken by calling usleep itself, on average */
         }
      }

      if((color_active[i])&&(captured==TRUE)){
		memcpy(color,frame->imageData,frame->width*frame->height*3);
		captured=FALSE;

	        switch (i){
			 case 0:
			    speedcounter(colorA_schema_id);
			    imageA_clock=lastimage;
			    break;
			 case 1:
			    speedcounter(colorB_schema_id);
			    imageB_clock=lastimage;
			    break;
			 case 2:
			    speedcounter(colorC_schema_id);
			    imageC_clock=lastimage;
			    break;
			 case 3:
			    speedcounter(colorD_schema_id);
			    imageD_clock=lastimage;
			    break;
			 case 4:
			    speedcounter(varcolorA_schema_id);
			    myA.clock=lastimage;
			    break;
			 case 5:
			    speedcounter(varcolorB_schema_id);
			    myB.clock=lastimage;
			    break;
			 case 6:
			    speedcounter(varcolorC_schema_id);
			    myC.clock=lastimage;
			    break;
			 case 7:
			    speedcounter(varcolorD_schema_id);
			    myD.clock=lastimage;
			    break;
			 default:
			    fprintf(stderr, "opencv_thread: Unknown option %d.\n", i);
	        }
      }

      lastimage++;

    }// end else
  }// end while

  cvReleaseCapture(&myCapture);
  pthread_exit(0);
}

/** Determines if an image configuration could be captured or not
 *  @param width width of the possible image
 *  @param height height of the possible image
 *  @return 1 if the size could be captured, otherwise 0
 */
int size_ok(int width, int height){
   if (width>0 && height>0)
      return 1;
   else
      return 0;
}

/** Determines if an image size from camera has the same size of an image
 *  @param c: camera
 *  @param i: image
 *  @return 1 if have the same size, otherwise 0
 */
int different_size(opencv_camera c,int w, int h){
	return ((c.frame_width!=w) || (c.frame_height!=h));
}

/** opencv driver parse configuration file function.
 *  @param configfile path and name to the config file.
 *  @return 0 if parsing was successful or -1 if something went wrong.*/
int opencv_parseconf(char *configfile){

   int end_parse=0; int end_section=0; int driver_config_parsed=0;
   FILE *myfile;
   const int limit = 256;

   if ((myfile=fopen(configfile,"r"))==NULL){
      printf("opencv: cannot find config file\n");
      return -1;
   }

   do{
    
      char word[256],word2[256],buffer_file[1024];
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
            printf ("opencv: line too long in config file!\n");
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
	      
                     char buffer_file2[1024],word3[256],word4[256];
                     char word5[256],word6[1024], word7[256],word8[256];
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
                              printf("opencv: error in config file.\n'end_section' keyword required before starting new driver section.\n");
                              end_section=1; end_parse=1;

                           }else if(strcmp(word3,"provides")==0){
                              int words;
                              while((buffer_file2[z]!='\n')&&
                                     (buffer_file2[z]!=' ')&&
                                     (buffer_file2[z]!='\0')&&
                                     (buffer_file2[z]!='\t')) z++;
                              words=sscanf(buffer_file2,"%s %s %s %s %s %s",
                                           word3,word4,word5,word6,word7,word8);
                              if(words==4){
                                 if(strcmp(word4,"colorA")==0){
                           	    serve_color[0]=1;
				    strncpy(cameras[0].name,word6,strlen(word6));
                                    width[0] = SIFNTSC_COLUMNS;
                                    height[0] = SIFNTSC_ROWS;
				    camCount++;
				    if (strncmp(word5, "videofile", strlen("videofile"))==0){
                                          cameras[0].input_type=VIDEO_FILE;
				    }
                                    else if (strncmp(word5, "video", strlen("video"))==0){
                                          cameras[0].input_type=VIDEO_DEVICE;
				    }
				    else if (strncmp(word5, "image",strlen("image"))==0)
					  cameras[0].input_type=IMAGE_FILE;	

				    if (cameras[0].frame_width == -1){	
					  cameras[0].frame_width=width[0];
                                          cameras[0].frame_height=height[0];
                                    }
                                    else{
                                          serve_color[0]=0;
                                          fprintf (stderr,"Camera %d already in use by an other color capture\n",camCount);
                                    }	

                                 }else if(strcmp(word4,"colorB")==0){
                           	    serve_color[1]=1;
				    strncpy(cameras[1].name,word6,strlen(word6));
                                    width[1] = SIFNTSC_COLUMNS;
                                    height[1] = SIFNTSC_ROWS;
				    camCount++;
				    if (strncmp(word5, "videofile", strlen("videofile"))==0){
                                          cameras[1].input_type=VIDEO_FILE;
				    }
                                    else if (strncmp(word5, "video", strlen("video"))==0){
                                          cameras[1].input_type=VIDEO_DEVICE;
				    }
				    else if (strncmp(word5, "image",strlen("image"))==0)
					  cameras[1].input_type=IMAGE_FILE;	

				    if (cameras[1].frame_width == -1){	
					  cameras[1].frame_width=width[1];
                                          cameras[1].frame_height=height[1];
                                    }
                                    else{
                                          serve_color[1]=0;
                                          fprintf (stderr,"Camera %d already in use by an other color capture\n",camCount);
                                    }		

                                 }else if(strcmp(word4,"colorC")==0){
                           	    serve_color[2]=1;
				    strncpy(cameras[2].name,word6,strlen(word6));
                                    width[2] = SIFNTSC_COLUMNS;
                                    height[2] = SIFNTSC_ROWS;
				    camCount++;
				    if (strncmp(word5, "videofile", strlen("videofile"))==0){
                                          cameras[2].input_type=VIDEO_FILE;
				    }
                                    else if (strncmp(word5, "video", strlen("video"))==0){
                                          cameras[2].input_type=VIDEO_DEVICE;
				    }
				    else if (strncmp(word5, "image",strlen("image"))==0)
					  cameras[2].input_type=IMAGE_FILE;

				    if (cameras[2].frame_width == -1){	
					  cameras[2].frame_width=width[2];
                                          cameras[2].frame_height=height[2];
                                    }
                                    else{
                                          serve_color[2]=0;
                                          fprintf (stderr,"Camera %d already in use by an other color capture\n",camCount);
                                    }			

                               }else if(strcmp(word4,"colorD")==0){
                                    serve_color[3]=1;
				    strncpy(cameras[3].name,word6,strlen(word6));
                                    width[3] = SIFNTSC_COLUMNS;
                                    height[3] = SIFNTSC_ROWS;
				    camCount++;
				    if (strncmp(word5, "videofile", strlen("videofile"))==0){
                                          cameras[3].input_type=VIDEO_FILE;
				    }
                                    else if (strncmp(word5, "video", strlen("video"))==0){
                                          cameras[3].input_type=VIDEO_DEVICE;
				    }
				    else if (strncmp(word5, "image",strlen("image"))==0)
					  cameras[3].input_type=IMAGE_FILE;	

				    if (cameras[3].frame_width == -1){	
					  cameras[3].frame_width=width[3];
                                          cameras[3].frame_height=height[3];
                                    }
                                    else{
                                          serve_color[3]=0;
                                          fprintf (stderr,"Camera %d already in use by an other color capture\n",camCount);
                                    }		
                                }
                              }
                              else if (words==6){
                                 if(strcmp(word4,"varcolorA")==0){
                                    serve_color[4]=1;
				    strncpy(cameras[4].name,word6,strlen(word6));
                                    width[4] = atoi(word7);
                                    height[4] = atoi(word8);
				    camCount++;
				    if (strncmp(word5, "videofile", strlen("videofile"))==0){
                                          cameras[4].input_type=VIDEO_FILE;
				    }
                                    else if (strncmp(word5, "video", strlen("video"))==0){
                                          cameras[4].input_type=VIDEO_DEVICE;
				    }
				    else if (strncmp(word5, "image",strlen("image"))==0)
					  cameras[4].input_type=IMAGE_FILE;			

                                    if (cameras[4].frame_width == -1){
                                       if (!size_ok(width[4], height[4])){
                                          fprintf (stderr, "Wrong image size for varcolorA, changed to default size 320X240\n");
                                          width[4] = SIFNTSC_COLUMNS;
                                          height[4] = SIFNTSC_ROWS;
                                       }
                                       cameras[4].frame_width=width[4];
                                       cameras[4].frame_height=height[4];
                                    }
                                    else{
                                       serve_color[4]=0;
                                       fprintf (stderr,"Camera %d already in use by an other color capture\n",camCount);
                                    }
                                 }else if(strcmp(word4,"varcolorB")==0){
                                    serve_color[5]=1;
				    strncpy(cameras[5].name,word6,strlen(word6));
                                    width[5] = atoi(word7);
                                    height[5] = atoi(word8);
				    camCount++;
				    if (strncmp(word5, "videofile", strlen("videofile"))==0){
                                          cameras[5].input_type=VIDEO_FILE;
				    }
                                    else if (strncmp(word5, "video", strlen("video"))==0){
                                          cameras[5].input_type=VIDEO_DEVICE;
				    }
				    else if (strncmp(word5, "image",strlen("image"))==0)
					  cameras[5].input_type=IMAGE_FILE;			

                                    if (cameras[5].frame_width == -1){
                                       if (!size_ok(width[5], height[5])){
                                          fprintf (stderr, "Wrong image size for varcolorA, changed to default size 320X240\n");
                                          width[5] = SIFNTSC_COLUMNS;
                                          height[5] = SIFNTSC_ROWS;
                                       }
                                       cameras[5].frame_width=width[5];
                                       cameras[5].frame_height=height[5];
                                    }
                                    else{
                                       serve_color[5]=0;
                                       fprintf (stderr,"Camera %d already in use by an other color capture\n",camCount);
                                    }
                                 }else if(strcmp(word4,"varcolorC")==0){
                                    serve_color[6]=1;
				    strncpy(cameras[6].name,word6,strlen(word6));
                                    width[6] = atoi(word7);
                                    height[6] = atoi(word8);
				    camCount++;
				    if (strncmp(word5, "videofile", strlen("videofile"))==0){
                                          cameras[6].input_type=VIDEO_FILE;
				    }
                                    else if (strncmp(word5, "video", strlen("video"))==0){
                                          cameras[6].input_type=VIDEO_DEVICE;
				    }
				    else if (strncmp(word5, "image",strlen("image"))==0)
					  cameras[6].input_type=IMAGE_FILE;			

                                    if (cameras[6].frame_width == -1){
                                       if (!size_ok(width[6], height[6])){
                                          fprintf (stderr, "Wrong image size for varcolorA, changed to default size 320X240\n");
                                          width[6] = SIFNTSC_COLUMNS;
                                          height[6] = SIFNTSC_ROWS;
                                       }
                                       cameras[6].frame_width=width[6];
                                       cameras[6].frame_height=height[6];
                                    }
                                    else{
                                       serve_color[6]=0;
                                       fprintf (stderr,"Camera %d already in use by an other color capture\n",camCount);
                                    }
                                 }else if(strcmp(word4,"varcolorD")==0){
                                    serve_color[7]=1;
				    strncpy(cameras[7].name,word6,strlen(word6));
                                    width[7] = atoi(word7);
                                    height[7] = atoi(word8);
				    camCount++;
				    if (strncmp(word5, "videofile", strlen("videofile"))==0){
                                          cameras[7].input_type=VIDEO_FILE;
				    }
                                    else if (strncmp(word5, "video", strlen("video"))==0){
                                          cameras[7].input_type=VIDEO_DEVICE;
				    }
				    else if (strncmp(word5, "image",strlen("image"))==0)
					  cameras[7].input_type=IMAGE_FILE;			

                                    if (cameras[7].frame_width == -1){
                                       if (!size_ok(width[7], height[7])){
                                          fprintf (stderr, "Wrong image size for varcolorA, changed to default size 320X240\n");
                                          width[7] = SIFNTSC_COLUMNS;
                                          height[7] = SIFNTSC_ROWS;
                                       }
                                       cameras[7].frame_width=width[7];
                                       cameras[7].frame_height=height[7];
                                    }
                                    else{
                                       serve_color[7]=0;
                                       fprintf (stderr,"Camera %d already in use by an other color capture\n",camCount);
                                    }
                                 }


                                }else{
                                   printf("opencv: provides line incorrect\n");
                                }
                           }else printf("opencv: i don't know what to do with '%s'\n",buffer_file2);
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
      if((serve_color[0]==0)&&(serve_color[1]==0)&&(serve_color[2]==0)&&(serve_color[3]==0)
	  &&(serve_color[4]==0)&&(serve_color[5]==0)&&(serve_color[6]==0)&&(serve_color[7]==0)){
         printf("opencv: warning! no color provided.\n");
      }
      return 0;
   }else return -1;
}

/** opencv driver camera count function. 
 *  @return 0 if there is some camera or -1 if not.*/
void opencv_count(){

      int i;
      CvCapture* myCapture = NULL;
      IplImage *frame = NULL;
      int is1394=0;
  
      /* setup cameras for capture */
      for (i = 0; i < MAXCAM; i++)
	{	
		if(serve_color[i]){
			switch (cameras[i].input_type) {
			      case VIDEO_DEVICE:
				 if (strncmp(cameras[i].name, "/dev/video1394", strlen("/dev/video1394"))!=0)
				 	myCapture = cvCreateCameraCapture(atoi(&cameras[i].name[strlen(cameras[i].name)-1]));
				 else 
					/* firewire camera doesn't support this exploration */ 
					/* if there isn't any firewire camera, the specific camera thread will return an error */
					is1394=1;
				 break;
			      case VIDEO_FILE:
				 myCapture = cvCreateFileCapture(cameras[i].name);
				 break;
			      case IMAGE_FILE:
				 frame=cvLoadImage(cameras[i].name,1);
				 fflush(NULL);
				 break;
			}

			if (myCapture != NULL || frame!=NULL || is1394==1){
			    numCameras++;
			    if (myCapture != NULL) cvReleaseCapture(&myCapture);
			}
		}
	}
  
  fflush(stdout);
  if (numCameras < 1) {
    perror("no cameras found\n");
    if(opencv_cleaned_up==0) opencv_clean_up();
    exit(-1);
  }

  opencv_setup=1;
}

/** cleans up **/
void opencv_clean_up(){

}

/** opencv driver init function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void opencv_init(char *configfile)
{
  int i;

  /* reseting serve color array and setting default options */
  for(i=0;i<MAXCAM;i++){
     serve_color[i]=0;
     color_active[i]=0;
     cameras[i].frame_width=-1;
  }

  /* we call the function to parse the config file */
  if(opencv_parseconf(configfile)==-1){
    printf("opencv: cannot initiate driver. configfile parsing error.\n");
    exit(-1);
  }

  /* opencv cameras count */
  opencv_count();

  for (i=0; i<MAXCAM; i++){
     if(opencv_thread_created[i]!=1){
        pthread_mutex_lock(&mymutex[i]);
        state[i]=slept;
        if (serve_color[i]){
           args[i]=i;
           pthread_create(&opencv_th[i],NULL,opencv_thread,(void*)&args[i]);
        }
        opencv_thread_created[i]=1;
        pthread_mutex_unlock(&mymutex[i]);
     }
  }

  if(serve_color[0]==1){
      all[num_schemas].id = (int *) &colorA_schema_id;
      strcpy(all[num_schemas].name,"colorA");
      all[num_schemas].run = (runFn) mycolorA_run;
      all[num_schemas].stop = (stopFn) mycolorA_stop;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("colorA","id",&colorA_schema_id);
      myexport("colorA","colorA",&colorA);
      myexport("colorA","clock", &imageA_clock);
      myexport("colorA","width",&width[0]);
      myexport("colorA","height",&height[0]);
      myexport("colorA","run",(void *)mycolorA_run);
      myexport("colorA","stop",(void *)mycolorA_stop);

      colorA=(char *)malloc (width[0]*height[0]*3);
  }

  if(serve_color[1]==1){ 
      all[num_schemas].id = (int *) &colorB_schema_id;
      strcpy(all[num_schemas].name,"colorB");
      all[num_schemas].run = (runFn) mycolorB_run;
      all[num_schemas].stop = (stopFn) mycolorB_stop;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("colorB","id",&colorB_schema_id);
      myexport("colorB","colorB",&colorB);
      myexport("colorB","clock", &imageB_clock);
      myexport("colorB","width",&width[1]);
      myexport("colorB","height",&height[1]);
      myexport("colorB","run",(void *)mycolorB_run);
      myexport("colorB","stop",(void *)mycolorB_stop);

      colorB=(char *)malloc (width[1]*height[1]*3);
  }

  if(serve_color[2]==1){
      all[num_schemas].id = (int *) &colorC_schema_id;
      strcpy(all[num_schemas].name,"colorC");
      all[num_schemas].run = (runFn) mycolorC_run;
      all[num_schemas].stop = (stopFn) mycolorC_stop;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("colorC","id",&colorC_schema_id);
      myexport("colorC","colorC",&colorC);
      myexport("colorC","clock", &imageC_clock);
      myexport("colorC","width",&width[2]);
      myexport("colorC","height",&height[2]);
      myexport("colorC","run",(void *)mycolorC_run);
      myexport("colorC","stop",(void *)mycolorC_stop);

      colorC=(char *)malloc (width[2]*height[2]*3);
  }

  if(serve_color[3]==1){
      all[num_schemas].id = (int *) &colorD_schema_id;
      strcpy(all[num_schemas].name,"colorD");
      all[num_schemas].run = (runFn) mycolorD_run;
      all[num_schemas].stop = (stopFn) mycolorD_stop;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport("colorD","id",&colorD_schema_id);
      myexport("colorD","colorD",&colorD);
      myexport("colorD","clock", &imageD_clock);
      myexport("colorD","width",&width[3]);
      myexport("colorD","height",&height[3]);
      myexport("colorD","run",(void *)mycolorD_run);
      myexport("colorD","stop",(void *)mycolorD_stop);

      colorD=(char *)malloc (width[3]*height[3]*3);
  }
  /*creates new schema for varcolorA*/
  if(serve_color[4]==1){  
        myA.clock = 0;
	myA.width=width[4];
	myA.height=height[4];
        myA.img = (char*)malloc(myA.width*myA.height*3*sizeof(char));

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
        myexport("varcolorA","id",&varcolorA_schema_id);
        myexport("varcolorA","run",(void *)myvarcolorA_run);
        myexport("varcolorA","stop",(void *)myvarcolorA_stop);
	myexport("varcolorA","varcolorA",&myA);
  }
  /*creates new schema for varcolorB*/
  if(serve_color[5]==1){
        myB.clock = 0;
	myB.width=width[5];
	myB.height=height[5];
        myB.img = (char*)malloc(myB.width*myB.height*3*sizeof(char));

        all[num_schemas].id = (int *) &varcolorB_schema_id;
        strcpy(all[num_schemas].name,"varcolorB");
        all[num_schemas].run = (runFn) myvarcolorB_run;
        all[num_schemas].stop = (stopFn) myvarcolorB_stop;
        printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
        (*(all[num_schemas].id)) = num_schemas;
        all[num_schemas].fps = 0.;
        all[num_schemas].k =0;
        all[num_schemas].state=slept;
        all[num_schemas].terminate = NULL;
        all[num_schemas].handle = NULL;
        num_schemas++;
        myexport("varcolorB","id",&varcolorB_schema_id);
        myexport("varcolorB","run",(void *)myvarcolorB_run);
        myexport("varcolorB","stop",(void *)myvarcolorB_stop);
	myexport("varcolorB","varcolorB",&myB);
  }

  /*creates new schema for varcolorC*/
  if(serve_color[6]==1){
        myC.clock = 0;
	myC.width=width[6];
	myC.height=height[6];
        myC.img = (char*)malloc(myC.width*myC.height*3*sizeof(char));

        all[num_schemas].id = (int *) &varcolorC_schema_id;
        strcpy(all[num_schemas].name,"varcolorC");
        all[num_schemas].run = (runFn) myvarcolorC_run;
        all[num_schemas].stop = (stopFn) myvarcolorC_stop;
        printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
        (*(all[num_schemas].id)) = num_schemas;
        all[num_schemas].fps = 0.;
        all[num_schemas].k =0;
        all[num_schemas].state=slept;
        all[num_schemas].terminate = NULL;
        all[num_schemas].handle = NULL;
        num_schemas++;
        myexport("varcolorC","id",&varcolorC_schema_id);
        myexport("varcolorC","run",(void *)myvarcolorC_run);
        myexport("varcolorC","stop",(void *)myvarcolorC_stop);
	myexport("varcolorC","varcolorC",&myC);
  }

  /*creates new schema for varcolorD*/
  if(serve_color[7]==1){
        myD.clock = 0;
	myD.width=width[7];
	myD.height=height[7];
        myD.img = (char*)malloc(myD.width*myD.height*3*sizeof(char));

        all[num_schemas].id = (int *) &varcolorD_schema_id;
        strcpy(all[num_schemas].name,"varcolorD");
        all[num_schemas].run = (runFn) myvarcolorD_run;
        all[num_schemas].stop = (stopFn) myvarcolorD_stop;
        printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
        (*(all[num_schemas].id)) = num_schemas;
        all[num_schemas].fps = 0.;
        all[num_schemas].k =0;
        all[num_schemas].state=slept;
        all[num_schemas].terminate = NULL;
        all[num_schemas].handle = NULL;
        num_schemas++;
        myexport("varcolorD","id",&varcolorD_schema_id);
        myexport("varcolorD","run",(void *)myvarcolorD_run);
        myexport("varcolorD","stop",(void *)myvarcolorD_stop);
	myexport("varcolorD","varcolorD",&myD);
  }
  
  printf("opencv driver started up\n");
}
