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
 *  Authors : David Lobato <dav.lobato@gmail.com>
 *            Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es>
 *            José Antonio Santos <santoscadenas@gmail.com>
 *            Jose Maria Cañas <jmplaza@gsyc.escet.urjc.es>
 *
 *
 */


/**
 * jdec firewire driver provides video images to color variables from firewire
 * cameras using libdc1394 library.
 *
 * This 4.3 version includes support for variable images. Only 320x240 and
 * 640x420 sizes are admitted because firewire cameras only support this sizes.
 *
 * @file firewire.c
 * @author David Lobato <dlobato@gsyc.escet.urjc.es>, Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es>, Jose Maria Cañas Plaza <jmplaza@gsyc.escet.urjc.es>and José Antonio Santos <santoscadenas@gmail.com>
 * @version 4.3
 * @date 2007-11-17
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <libdc1394/dc1394_control.h>
#include <jde.h>
#include <interfaces/varcolor.h>

/** Max number of cameras detected by firewire driver.*/
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

/**
 * Color conversion functions from Bart Nabbe.
 * Corrected by Damien: bad coeficients in YUV2RGB.
 */
#define YUV2RGB(y, u, v, r, g, b)		\
  r = y + ((v*1436) >> 10);			\
  g = y - ((u*352 + v*731) >> 10);		\
  b = y + ((u*1814) >> 10);			\
  r = r < 0 ? 0 : r;				\
  g = g < 0 ? 0 : g;				\
  b = b < 0 ? 0 : b;				\
  r = r > 255 ? 255 : r;			\
  g = g > 255 ? 255 : g;			\
  b = b > 255 ? 255 : b
  
/** Automatic features for firewire cameras.*/
typedef struct{
  int AUTO_EXPOSURE_CONFIG;
  int AUTO_WHITE_BALANCE_CONFIG;
  int AUTO_IRIS_CONFIG;
  int AUTO_FOCUS_CONFIG;
  int AUTO_ZOOM_CONFIG;
}Firewire_features;

/* declarations for libdc1394 */
/** numPorts variable for libdc1394.*/
int numPorts = MAX_PORTS;
/** handle for detecting raw1394 devices.*/
raw1394handle_t handles[MAX_PORTS];
/** number of firewire cameras detected.*/
int numCameras=0;
/** camera capture structure from libdc1394 library.*/
dc1394_cameracapture cameras[MAXCAM];
/** camera node structure from libdc1394 library.*/
nodeid_t *camera_nodes;
/** camera feature structure from libdc1394 library.*/
dc1394_feature_set features;
/** int variable to detect when a image was captured.*/
unsigned long int lastimage=0;

/* declarations for video1394 */
/** firewire camera devices.*/
char *camfile[]={"/dev/video1394/0","/dev/video1394/1","/dev/video1394/2","/dev/video1394/3"};
/** firewire instant fps variable for 320X240 size.*/
int firewire_fps320;
/** firewire instant resolution variable for 320X240 size.*/
int firewire_res320;
/** firewire instant fps variable for 640X480 size.*/
int firewire_fps640;
/** firewire instant resolution variable for 640X480 size.*/
int firewire_res640;

/** pthread identifiers for jdec firewire driver threads.*/
pthread_t firewire_th[MAXCAM];
/** Arguments for firewire threads*/
int args[MAXCAM];
/** pthread state variable for jdec firewire driver.*/
int state[MAXCAM];
/** pthread mutex for jdec firewire driver.*/
pthread_mutex_t mymutex[MAXCAM];
/** pthread condition variable for jdec firewire driver.*/
pthread_cond_t condition[MAXCAM];

/** variable to detect when the pthread is created.*/
int firewire_thread_created[MAXCAM];
/** variable to detect when firewire driver was cleaned up.*/
int firewire_cleaned_up=0;
/** variable to detect when firewire driver was setup.*/
int firewire_setup=0;
/** variable to detect when pthread must end execution.*/
int firewire_terminate_command=0;

/* firewire driver API options */
/** firewire driver name.*/
char driver_name[256]="firewire";
/** colors detected in config file.*/
int serve_color[MAXCAM];
/** structure to know what color uses which camera.*/
int number_color[MAXCAM];
/** structure to know what colors are active in the gui.*/
int color_active[MAXCAM];
/** feature structure from libdc1394 to set firewire cameras characteristics.*/
Firewire_features fwCamFeatures[MAXCAM];
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

Varcolor myA,myB,myC,myD;

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


/* FIREWIRE DRIVER FUNCTIONS */
/** function to transform a buffer from uyvy to rgb.
 *  @param src source buffer.
 *  @param dest destination buffer where the transformation will be set.
 *  @param NumPixels how many pixels per buffer.*/
void uyvy2rgb (unsigned char *src, unsigned char *dest, unsigned long long int NumPixels)
{
  register int i = (NumPixels << 1)-1;
  register int j = NumPixels + ( NumPixels << 1 ) -1;
  register int y0, y1, u, v;
  register int r, g, b;

  while (i > 0) {
    y1 = (unsigned char) src[i--];
    v  = (unsigned char) src[i--] - 128;
    y0 = (unsigned char) src[i--];
    u  = (unsigned char) src[i--] - 128;
    YUV2RGB (y1, u, v, r, g, b);
    dest[j--] = r;
    dest[j--] = g;
    dest[j--] = b;
    YUV2RGB (y0, u, v, r, g, b);
    dest[j--] = r;
    dest[j--] = g;
    dest[j--] = b;
  }
}

/** cleans up firewire structures and frees the firewire bus.*/
void firewire_clean_up() {
  int i;
  firewire_cleaned_up=1;
  for (i=0; i < numCameras; i++)
    {
      /* we only use one port */
      dc1394_stop_iso_transmission(handles[cameras[i].port], cameras[numCameras].node);
      dc1394_dma_unlisten( handles[cameras[i].port], &cameras[numCameras] );
      dc1394_dma_release_camera( handles[cameras[i].port], &cameras[numCameras]);
    }
  for (i=0; i < numPorts; i++)
    raw1394_destroy_handle(handles[i]);
}

/** firewire driver closing function invoked when stopping driver.*/
void firewire_terminate(){

  firewire_terminate_command=1;
  firewire_clean_up();
  printf("driver firewire off\n");
}

/** function that will set the default configuration to all firewire cameras detected.*/
void set_default_firewire_camera_config(void){

  int i;
  for(i=0; i< MAXCAM; i++){
    fwCamFeatures[i].AUTO_EXPOSURE_CONFIG=1;
    fwCamFeatures[i].AUTO_WHITE_BALANCE_CONFIG=1;
    fwCamFeatures[i].AUTO_IRIS_CONFIG=1;
    fwCamFeatures[i].AUTO_FOCUS_CONFIG=1;
    fwCamFeatures[i].AUTO_ZOOM_CONFIG=1;
  }
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
         printf("colorA schema run (firewire driver)\n");
         all[colorA_schema_id].father = father;
         all[colorA_schema_id].fps = 0.;
         all[colorA_schema_id].k =0;
         all[colorA_schema_id].k =0;
         put_state(colorA_schema_id,winner);
         /* firewire thread goes winner */
         pthread_mutex_lock(&mymutex[0]);
         state[0]=winner;
         pthread_cond_signal(&condition[0]);
         pthread_mutex_unlock(&mymutex[0]);
      }
   }
   return 0;
}

/** colorA stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
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
         printf("colorA schema stop (firewire driver)\n");
         /* firewire thread goes sleep */
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
         printf("colorB schema run (firewire driver)\n");
         all[colorB_schema_id].father = father;
         all[colorB_schema_id].fps = 0.;
         all[colorB_schema_id].k =0;
   
         put_state(colorB_schema_id,winner);

         /* firewire thread goes winner */
         pthread_mutex_lock(&mymutex[1]);
         state[1]=winner;
         pthread_cond_signal(&condition[1]);
         pthread_mutex_unlock(&mymutex[1]);
      }
   }
   return 0;
}

/** colorB stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
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
         printf("colorB schema stop (firewire driver)\n");
         put_state(colorB_schema_id,slept);
         /* firewire thread goes sleep */
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
         printf("colorC schema run (firewire driver)\n");
         all[colorC_schema_id].father = father;
         all[colorC_schema_id].fps = 0.;
         all[colorC_schema_id].k =0;
         put_state(colorC_schema_id,winner);
         /* firewire thread goes winner */
         pthread_mutex_lock(&mymutex[2]);
         state[2]=winner;
         pthread_cond_signal(&condition[2]);
         pthread_mutex_unlock(&mymutex[2]);
      }
   }
   return 0;
}

/** colorC stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
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
         printf("colorC schema stop (firewire driver)\n");
         put_state(colorC_schema_id,slept);
         /* firewire thread goes sleep */
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
         printf("colorD schema run (firewire driver)\n");
         all[colorD_schema_id].father = father;
         all[colorD_schema_id].fps = 0.;
         all[colorD_schema_id].k =0;
         put_state(colorD_schema_id,winner);
         /* firewire thread goes winner */
         pthread_mutex_lock(&mymutex[3]);
         state[3]=winner;
         pthread_cond_signal(&condition[3]);
         pthread_mutex_unlock(&mymutex[3]);
      }
   }
   return 0;
}

/** colorD stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
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
         printf("colorD schema stop (firewire driver)\n");
         put_state(colorD_schema_id,slept);
         /* firewire thread goes sleep */
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
         printf("varcolorA schema run (firewire driver)\n");
         all[varcolorA_schema_id].father = father;
         all[varcolorA_schema_id].fps = 0.;
         all[varcolorA_schema_id].k =0;
         put_state(varcolorA_schema_id,winner);
         /* firewire thread goes winner */
         pthread_mutex_lock(&mymutex[4]);
         state[4]=winner;
         pthread_cond_signal(&condition[4]);
         pthread_mutex_unlock(&mymutex[4]);
      }
   }
   return 0;
}

/** varcolorA stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
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
         printf("varcolorA schema stop (firewire driver)\n");
         put_state(varcolorA_schema_id,slept);
         /* firewire thread goes sleep */
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
         printf("varcolorB schema run (firewire driver)\n");
         all[varcolorB_schema_id].father = father;
         all[varcolorB_schema_id].fps = 0.;
         all[varcolorB_schema_id].k =0;
         put_state(varcolorB_schema_id,winner);
         /* firewire thread goes winner */
         pthread_mutex_lock(&mymutex[5]);
         state[5]=winner;
         pthread_cond_signal(&condition[5]);
         pthread_mutex_unlock(&mymutex[5]);
      }
   }
   return 0;
}

/** varcolorB stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
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
         printf("varcolorB schema stop (firewire driver)\n");
         put_state(varcolorB_schema_id,slept);
         /* firewire thread goes sleep */
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
         printf("varcolorC schema run (firewire driver)\n");
         all[varcolorC_schema_id].father = father;
         all[varcolorC_schema_id].fps = 0.;
         all[varcolorC_schema_id].k =0;
         put_state(varcolorC_schema_id,winner);
         /* firewire thread goes winner */
         pthread_mutex_lock(&mymutex[6]);
         state[6]=winner;
         pthread_cond_signal(&condition[6]);
         pthread_mutex_unlock(&mymutex[6]);
      }
   }
   return 0;
}

/** varcolorC stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
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
         printf("varcolorC schema stop (firewire driver)\n");
         put_state(varcolorC_schema_id,slept);
         /* firewire thread goes sleep */
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
         printf("varcolorD schema run (firewire driver)\n");
         all[varcolorD_schema_id].father = father;
         all[varcolorD_schema_id].fps = 0.;
         all[varcolorD_schema_id].k =0;
         put_state(varcolorD_schema_id,winner);
         /* firewire thread goes winner */
         pthread_mutex_lock(&mymutex[7]);
         state[7]=winner;
         pthread_cond_signal(&condition[7]);
         pthread_mutex_unlock(&mymutex[7]);
      }
   }
   return 0;
}

/** varcolorD stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
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
         printf("varcolorD schema stop (firewire driver)\n");
         put_state(varcolorD_schema_id,slept);
         /* firewire thread goes sleep */
         pthread_mutex_lock(&mymutex[7]);
         state[7]=slept;
         pthread_mutex_unlock(&mymutex[7]);
      }
   }
   return 0;
}

/** firewire driver pthread function.*/
void *firewire_thread(void *id)
{
   int i;
   int capturedA=FALSE, capturedB=FALSE, capturedC=FALSE,  capturedD=FALSE;
   int capturedvarA=FALSE, capturedvarB=FALSE, capturedvarC=FALSE, capturedvarD=FALSE;
   i=*((int*)id);

  printf("firewire driver started up: number %d\n", i);

  do{
        
    pthread_mutex_lock(&mymutex[i]);

    if (state[i]==slept){
      printf("firewire thread in sleep mode\n");
      pthread_cond_wait(&condition[i],&mymutex[i]);
      printf("firewire thread woke up\n");
      pthread_mutex_unlock(&mymutex[i]);
      
    }else{
      
      pthread_mutex_unlock(&mymutex[i]);

      /* image capture for all cameras - only capturing when needed */
      switch (i){
         case 0:
            if(color_active[0]){
               if(dc1394_dma_single_capture(&cameras[number_color[0]])==DC1394_FAILURE)
                  perror("firewire_thread");
               capturedA=TRUE;
            }
            break;
         case 1:
            if((color_active[1])){
               if(dc1394_dma_single_capture(&cameras[number_color[1]])==DC1394_FAILURE)
                  perror("firewire_thread");
               capturedB=TRUE;
            }
            break;

         case 2:
            if((color_active[2])){
               if(dc1394_dma_single_capture(&cameras[number_color[2]])==DC1394_FAILURE)
                  perror("firewire_thread");
               capturedC=TRUE;
            }
            break;

         case 3:
            if((color_active[3])){
               if(dc1394_dma_single_capture(&cameras[number_color[3]])==DC1394_FAILURE)
                  perror("firewire_thread");
               capturedD=TRUE;
            }
            break;
         case 4:
            if(color_active[4]){
               if(dc1394_dma_single_capture(&cameras[number_color[4]])==DC1394_FAILURE)
                  perror("firewire_thread");
               capturedvarA=TRUE;
            }
            break;

         case 5:
            if((color_active[5])){
               if(dc1394_dma_single_capture(&cameras[number_color[5]])==DC1394_FAILURE)
                  perror("firewire_thread");
               capturedvarB=TRUE;
            }
            break;

         case 6:
            if((color_active[6])){
               if(dc1394_dma_single_capture(&cameras[number_color[6]])==DC1394_FAILURE)
                  perror("firewire_thread");
               capturedvarC=TRUE;
            }
            break;

         case 7:
            if((color_active[7])){
               if(dc1394_dma_single_capture(&cameras[number_color[7]])==DC1394_FAILURE)
                  perror("firewire_thread");
               capturedvarD=TRUE;
            }
            break;
      }


      /* image copy between buffers */
      /* dma_done_with_buffer for all active cameras */
      if(color_active[0]&&(capturedA==TRUE)){
	uyvy2rgb((unsigned char*)cameras[number_color[0]].capture_buffer,(unsigned char*)colorA,SIFNTSC_COLUMNS*SIFNTSC_ROWS);
	if(dc1394_dma_done_with_buffer(&cameras[number_color[0]])==DC1394_FAILURE) perror("firewire_thread");
        else {
           speedcounter(colorA_schema_id);
           imageA_clock=lastimage;
        }
	capturedA=FALSE;
      }

      if((color_active[1])&&(capturedB==TRUE)){
	uyvy2rgb((unsigned char*)cameras[number_color[1]].capture_buffer,(unsigned char*)colorB,SIFNTSC_COLUMNS*SIFNTSC_ROWS);
	if(dc1394_dma_done_with_buffer(&cameras[number_color[1]])==DC1394_FAILURE) perror("firewire_thread");
	else {
           speedcounter(colorB_schema_id);
           imageB_clock=lastimage;
        }
	capturedB=FALSE;
      }

      if((color_active[2])&&(capturedC==TRUE)){
	uyvy2rgb((unsigned char*)cameras[number_color[2]].capture_buffer,(unsigned char*)colorC,SIFNTSC_COLUMNS*SIFNTSC_ROWS);
	if(dc1394_dma_done_with_buffer(&cameras[number_color[2]])==DC1394_FAILURE) perror("firewire_thread");
        else {
           speedcounter(colorC_schema_id);
           imageC_clock=lastimage;
        }
	capturedC=FALSE;
      }

      if((color_active[3])&&(capturedD==TRUE)){
	uyvy2rgb((unsigned char*)cameras[number_color[3]].capture_buffer,(unsigned char*)colorD,SIFNTSC_COLUMNS*SIFNTSC_ROWS);
	if(dc1394_dma_done_with_buffer(&cameras[number_color[3]])==DC1394_FAILURE) perror("firewire_thread");
        else {
           speedcounter(colorD_schema_id);
           imageD_clock=lastimage;
        }
	capturedD=FALSE;
      }

      if((color_active[4])&&(capturedvarA==TRUE)){
         uyvy2rgb((unsigned char*)cameras[number_color[4]].capture_buffer,(unsigned char*)myA.img,width[4]*height[4]);
         if(dc1394_dma_done_with_buffer(&cameras[number_color[4]])==DC1394_FAILURE) perror("firewire_thread");
         else {
            speedcounter(varcolorA_schema_id);
            myA.clock=lastimage;
         }
         capturedvarA=FALSE;
      }

      if((color_active[5])&&(capturedvarB==TRUE)){
         uyvy2rgb((unsigned char*)cameras[number_color[5]].capture_buffer,(unsigned char*)myB.img,width[5]*height[5]);
         if(dc1394_dma_done_with_buffer(&cameras[number_color[5]])==DC1394_FAILURE) perror("firewire_thread");
         else {
            speedcounter(varcolorB_schema_id);
            myB.clock=lastimage;
         }
         capturedvarB=FALSE;
      }

      if((color_active[6])&&(capturedvarC==TRUE)){
         uyvy2rgb((unsigned char*)cameras[number_color[6]].capture_buffer,(unsigned char*)myC.img,width[6]*height[6]);
         if(dc1394_dma_done_with_buffer(&cameras[number_color[6]])==DC1394_FAILURE) perror("firewire_thread");
         else {
            speedcounter(varcolorC_schema_id);
            myC.clock=lastimage;
         }
         capturedvarC=FALSE;
      }

      if((color_active[7])&&(capturedvarD==TRUE)){
         uyvy2rgb((unsigned char*)cameras[number_color[7]].capture_buffer,(unsigned char*)myD.img,width[7]*height[7]);
         if(dc1394_dma_done_with_buffer(&cameras[number_color[7]])==DC1394_FAILURE) perror("firewire_thread");
         else {
            speedcounter(varcolorD_schema_id);
            myD.clock=lastimage;
         }
         capturedvarD=FALSE;
      }
      lastimage++;
    }
  }while(firewire_terminate_command==0);

  pthread_exit(0);
}

/** Determines if an image configuration could be captured or not
 *  @param width width of the possible image
 *  @param height height of the possible image
 *  @return 1 if the size could be captured, otherwise 0
 */
int size_ok(int width, int height){
   if (width==320 && height==240)
      return 1;
   else if (width==640 && height==480)
      return 1;
   else
      return 0;
}


/** firewire driver parse configuration file function.
 *  @param configfile path and name to the config file.
 *  @return 0 if parsing was successful or -1 if something went wrong.*/
int firewire_parseconf(char *configfile){

   int end_parse=0; int end_section=0; int driver_config_parsed=0;
   FILE *myfile;
   const int limit = 256;

   if ((myfile=fopen(configfile,"r"))==NULL){
      printf("firewire: cannot find config file\n");
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
            printf ("firewire: line too long in config file!\n");
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
	      
                     char buffer_file2[256],word3[256],word4[256];
                     char word5[256],word6[256], word7[256],word8[256];
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
                              printf("firewire: error in config file.\n'end_section' keyword required before starting new driver section.\n");
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
                                    number_color[0]=atoi(word5);
                                    width[0] = SIFNTSC_COLUMNS;
                                    height[0] = SIFNTSC_ROWS;
                                    if (cameras[number_color[0]].frame_width == (-1)){
                                       cameras[number_color[0]].frame_width=width[0];
                                       cameras[number_color[0]].frame_height=height[0];
                                       if(strcmp(word6,"autofocus_on")!=0)
                                          fwCamFeatures[number_color[0]].AUTO_FOCUS_CONFIG=0;
                                       else
                                          fwCamFeatures[number_color[0]].AUTO_FOCUS_CONFIG=1;
                                    }
                                    else{
                                       serve_color[0]=0;
                                       fprintf (stderr,
                                             "Camera %d already in use by an other color capture\n",
                                             number_color[0]);
                                    }

                                 }else if(strcmp(word4,"colorB")==0){
                                    serve_color[1]=1;
                                    number_color[1]=atoi(word5);
                                    width[1] = SIFNTSC_COLUMNS;
                                    height[1] = SIFNTSC_ROWS;
                                    if (cameras[number_color[1]].frame_width == (-1)){
                                       cameras[number_color[1]].frame_width=width[1];
                                       cameras[number_color[1]].frame_height=height[1];
                                       if(strcmp(word6,"autofocus_on")!=0)
                                          fwCamFeatures[number_color[1]].AUTO_FOCUS_CONFIG=0;
                                       else
                                          fwCamFeatures[number_color[1]].AUTO_FOCUS_CONFIG=1;
                                    }
                                    else{
                                       serve_color[1]=0;
                                       fprintf (stderr,
                                             "Camera %d already in use by an other color capture\n",
                                             number_color[1]);
                                    }

                                 }else if(strcmp(word4,"colorC")==0){
                                    serve_color[2]=1;
                                    number_color[2]=atoi(word5);
                                    width[2] = SIFNTSC_COLUMNS;
                                    height[2] = SIFNTSC_ROWS;
                                    if (cameras[number_color[2]].frame_width == (-1)){
                                       cameras[number_color[2]].frame_width=width[2];
                                       cameras[number_color[2]].frame_height=height[2];
                                       if(strcmp(word6,"autofocus_on")!=0)
                                          fwCamFeatures[number_color[2]].AUTO_FOCUS_CONFIG=0;
                                       else
                                          fwCamFeatures[number_color[2]].AUTO_FOCUS_CONFIG=1;
                                    }
                                    else{
                                       serve_color[2]=0;
                                       fprintf (stderr,
                                             "Camera %d already in use by an other color capture\n",
                                             number_color[2]);
                                    }

                                 }else if(strcmp(word4,"colorD")==0){
                                    serve_color[3]=1;
                                    number_color[3]=atoi(word5);
                                    width[3] = SIFNTSC_COLUMNS;
                                    height[3] = SIFNTSC_ROWS;
                                    if (cameras[number_color[3]].frame_width == (-1)){
                                       cameras[number_color[3]].frame_width=width[3];
                                       cameras[number_color[3]].frame_height=height[3];
                                       if(strcmp(word6,"autofocus_on")!=0)
                                          fwCamFeatures[number_color[3]].AUTO_FOCUS_CONFIG=0;
                                       else
                                          fwCamFeatures[number_color[3]].AUTO_FOCUS_CONFIG=1;
                                    }
                                    else{
                                       serve_color[3]=0;
                                       fprintf (stderr,
                                             "Camera %d already in use by an other color capture\n",
                                             number_color[3]);
                                    }
                                 }
                              }
                              else if (words==6){
                                 if(strcmp(word4,"varcolorA")==0){
                                    serve_color[4]=1;
                                    number_color[4]=atoi(word5);
                                    width[4] = atoi(word6);
                                    height[4] = atoi(word7);
                                    if (cameras[number_color[4]].frame_width == (-1)){
                                       if (!size_ok(width[4], height[4])){
                                          fprintf (stderr, "Wrong image size for varcolorA, changed to default size 320X240\n");
                                          width[4] = SIFNTSC_COLUMNS;
                                          height[4] = SIFNTSC_ROWS;
                                       }
                                       cameras[number_color[4]].frame_width=width[4];
                                       cameras[number_color[4]].frame_height=height[4];
                                       if(strcmp(word6,"autofocus_on")!=0)
                                          fwCamFeatures[number_color[4]].AUTO_FOCUS_CONFIG=0;
                                       else
                                          fwCamFeatures[number_color[4]].AUTO_FOCUS_CONFIG=1;
                                    }
                                    else{
                                       serve_color[4]=0;
                                       fprintf (stderr,
                                             "Camera %d already in use by an other color capture\n",
                                             number_color[4]);
                                    }
                                 }else if(strcmp(word4,"varcolorB")==0){
                                    serve_color[5]=1;
                                    number_color[5]=atoi(word5);
                                    width[5] = atoi(word6);
                                    height[5] = atoi(word7);
                                    if (cameras[number_color[5]].frame_width == (-1)){
                                       if (!size_ok(width[5], height[5])){
                                          fprintf (stderr, "Wrong image size for varcolorB, changed to default size 320X240\n");
                                          width[5] = SIFNTSC_COLUMNS;
                                          height[5] = SIFNTSC_ROWS;
                                       }
                                       cameras[number_color[5]].frame_width=width[5];
                                       cameras[number_color[5]].frame_height=height[5];
                                       if(strcmp(word6,"autofocus_on")!=0)
                                          fwCamFeatures[number_color[5]].AUTO_FOCUS_CONFIG=0;
                                       else
                                          fwCamFeatures[number_color[5]].AUTO_FOCUS_CONFIG=1;
                                    }
                                    else{
                                       serve_color[5]=0;
                                       fprintf (stderr,
                                             "Camera %d already in use by an other color capture\n",
                                             number_color[4]);
                                    }
                                 }else if(strcmp(word4,"varcolorC")==0){
                                    serve_color[6]=1;
                                    number_color[6]=atoi(word5);
                                    width[6] = atoi(word6);
                                    height[6] = atoi(word7);
                                    if (cameras[number_color[6]].frame_width == (-1)){
                                       if (!size_ok(width[6], height[6])){
                                          fprintf (stderr, "Wrong image size for varcolorC, changed to default size 320X240\n");
                                          width[6] = SIFNTSC_COLUMNS;
                                          height[6] = SIFNTSC_ROWS;
                                       }
                                       cameras[number_color[6]].frame_width=width[6];
                                       cameras[number_color[6]].frame_height=height[6];
                                       if(strcmp(word6,"autofocus_on")!=0)
                                          fwCamFeatures[number_color[6]].AUTO_FOCUS_CONFIG=0;
                                       else
                                          fwCamFeatures[number_color[6]].AUTO_FOCUS_CONFIG=1;
                                    }
                                    else{
                                       serve_color[6]=0;
                                       fprintf (stderr,
                                             "Camera %d already in use by an other color capture\n",
                                             number_color[6]);
                                    }
                                 }else if(strcmp(word4,"varcolorD")==0){
                                    serve_color[7]=1;
                                    number_color[7]=atoi(word5);
                                    width[7] = atoi(word6);
                                    height[7] = atoi(word7);
                                    if (cameras[number_color[7]].frame_width == (-1)){
                                       if (!size_ok(width[7], height[7])){
                                          fprintf (stderr, "Wrong image size for varcolorD, changed to default size 320X240\n");
                                          width[7] = SIFNTSC_COLUMNS;
                                          height[7] = SIFNTSC_ROWS;
                                       }
                                       cameras[number_color[7]].frame_width=width[7];
                                       cameras[number_color[7]].frame_height=height[7];
                                       if(strcmp(word6,"autofocus_on")!=0)
                                          fwCamFeatures[number_color[7]].AUTO_FOCUS_CONFIG=0;
                                       else
                                          fwCamFeatures[number_color[7]].AUTO_FOCUS_CONFIG=1;
                                    }
                                    else{
                                       serve_color[7]=0;
                                       fprintf (stderr,
                                             "Camera %d already in use by an other color capture\n",
                                             number_color[7]);
                                    }
                                 }

                              }else{
                                 printf("firewire: provides line incorrect\n");
                              }
                           }else printf("firewire: i don't know what to do with '%s'\n",buffer_file2);
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
         printf("firewire: warning! no color provided.\n");
      }
      return 0;
   }else return -1;
}

/** firewire driver init function. It will start all firewire required devices
 *  and setting them the default configuration.
 *  @return 0 if initialitation was successful or -1 if something went wrong.*/
void firewire_deviceinit(){

  int p,i;
  raw1394handle_t raw_handle;
  struct raw1394_portinfo ports[MAX_PORTS];
  unsigned int channel;
  unsigned int speed;
  int firewire_res;
  int firewire_fps;
  
  firewire_res320 = MODE_320x240_YUV422;
  firewire_fps320 = FRAMERATE_30;
  firewire_res640 = MODE_640x480_YUV422;
  firewire_fps640 = FRAMERATE_15;
  /* get the number of ports (cards) */
  raw_handle = raw1394_new_handle();
  if (raw_handle==NULL) {
    perror("Unable to aquire a raw1394 handle\n");
    perror("did you load the drivers?\n");
    exit(-1);
  }
  
  numPorts = raw1394_get_port_info(raw_handle, ports, numPorts);
  raw1394_destroy_handle(raw_handle);
  /*printf("number of ports = %d\n", numPorts);*/
  
  /* get dc1394 handle to each port */
  for (p = 0; p < numPorts; p++)
    {
      int camCount;
      
      handles[p] = dc1394_create_handle(p);
      if (handles[p]==NULL) {
	perror("Unable to aquire a raw1394 handle\n");
	perror("did you load the drivers?\n");
	if(firewire_cleaned_up==0) firewire_clean_up();
	exit(-1);
      }
      
      /* get the camera nodes and describe them as we find them */
      camera_nodes = dc1394_get_camera_nodes(handles[p], &camCount, 1);
      /*printf("number of cameras in port %d = %d\n",p,camCount);*/
      
      /* setup cameras for capture */
      for (i = 0; i < camCount; i++)
	{	
	  cameras[numCameras].node = camera_nodes[i];
	  
	  if(dc1394_get_camera_feature_set(handles[p], cameras[numCameras].node, &features) !=DC1394_SUCCESS) 
	    {
	      printf("unable to get feature set\n");
	    } else {
	    /*dc1394_print_feature_set(&features);*/
	  }
	  /*printf("numCameras es %d\n",numCameras);*/
	  if (dc1394_get_iso_channel_and_speed(handles[p], cameras[numCameras].node,
					       &channel, &speed) != DC1394_SUCCESS) 
	    {
	      perror("unable to get the iso channel number\n");
	      if(firewire_cleaned_up==0) firewire_clean_up();
	      exit(-1);
	    }
	  if (cameras[numCameras].frame_width==640){
             firewire_fps=firewire_fps640;
             firewire_res=firewire_res640;
          }
          else{
             firewire_fps=firewire_fps320;
             firewire_res=firewire_res320;
          }
	  if (dc1394_dma_setup_capture(handles[p], cameras[numCameras].node, i+1 /*channel*/,
              FORMAT_VGA_NONCOMPRESSED, firewire_res,
              SPEED_400, firewire_fps, NUM_BUFFERS, DROP_FRAMES,
              camfile[numCameras], &cameras[numCameras]) != DC1394_SUCCESS)
	    {
	      fprintf(stderr, "unable to setup camera- check line %d of %s to make sure\n",
		      __LINE__,__FILE__);
	      perror("that the video mode,framerate and format are supported\n");
	      printf("is one supported by your camera\n");
	      if(firewire_cleaned_up==0) firewire_clean_up();
	      exit(-1);
	    }
	  
	  /* deactivating autofocus and some other automatic features for each camera */
	  if(numCameras<4){
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,
                                 FEATURE_EXPOSURE,
                                 fwCamFeatures[numCameras].AUTO_EXPOSURE_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,
                                 FEATURE_WHITE_BALANCE,
                                 fwCamFeatures[numCameras].AUTO_WHITE_BALANCE_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,
                                 FEATURE_IRIS,
                                 fwCamFeatures[numCameras].AUTO_IRIS_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,
                                 FEATURE_FOCUS,
                                 fwCamFeatures[numCameras].AUTO_FOCUS_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,
                                 FEATURE_ZOOM,
                                 fwCamFeatures[numCameras].AUTO_ZOOM_CONFIG);
	  }
	  
	  /*have the camera start sending us data*/
	  if (dc1394_start_iso_transmission(handles[p], cameras[numCameras].node) !=DC1394_SUCCESS) 
	    {
	      perror("unable to start camera iso transmission. are the cameras connected?\n");
	      if(firewire_cleaned_up==0) firewire_clean_up();
	      exit(-1);
	    }
	  numCameras++;
	}
    }
  
  fflush(stdout);
  if (numCameras < 1) {
    perror("no cameras found\n");
    if(firewire_cleaned_up==0) firewire_clean_up();
    exit(-1);
  }

  firewire_setup=1;
}

/** firewire driver init function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void firewire_init(char *configfile)
{
  int i;

  /* reseting serve color array and setting default options */
  for(i=0;i<MAXCAM;i++){
     serve_color[i]=0;
     number_color[i]=-1;
     color_active[i]=0;
     cameras[i].frame_width=-1;
     cameras[i].frame_height=-1;
  }
  set_default_firewire_camera_config();


  /* we call the function to parse the config file */
  if(firewire_parseconf(configfile)==-1){
    printf("firewire: cannot initiate driver. configfile parsing error.\n");
    exit(-1);
  }

  /* firewire initialitation */
  if(firewire_setup==0) firewire_deviceinit();

  for (i=0; i<MAXCAM; i++){
     if(firewire_thread_created[i]!=1){
        pthread_mutex_lock(&mymutex[i]);
        state[i]=slept;
//     pthread_create(&firewire_th,NULL,firewire_thread,NULL);
        if (serve_color[i]){
           args[i]=i;
           pthread_create(&firewire_th[i],NULL,firewire_thread,(void*)&args[i]);
        }
        firewire_thread_created[i]=1;
        pthread_mutex_unlock(&mymutex[i]);
     }
  }

  /* displays autofocus message for each camera */
  if(serve_color[0]==1){
    if(number_color[0]<=numCameras){
      if(fwCamFeatures[number_color[0]].AUTO_FOCUS_CONFIG==1)
         printf("colorA autofocus feature: on\n");
      else
         printf("colorA autofocus feature: off\n");

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
    }else{
      serve_color[0]=0;
      printf("cannot find firewire camera for colorA\n");
    }
  }

  if(serve_color[1]==1){
    if(number_color[1]<=numCameras){
      if(fwCamFeatures[number_color[1]].AUTO_FOCUS_CONFIG==1)
         printf("colorB autofocus feature: on\n");
      else
         printf("colorB autofocus feature: off\n");
      
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
    }else{
      serve_color[1]=0;
      printf("cannot find firewire camera for colorB\n");
    }
  }

  if(serve_color[2]==1){
    if(number_color[2]<=numCameras){
      if(fwCamFeatures[number_color[2]].AUTO_FOCUS_CONFIG==1)
         printf("colorC autofocus feature: on\n");
      else
         printf("colorC autofocus feature: off\n");

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
    }else{
      serve_color[2]=0;
      printf("cannot find firewire camera for colorC\n");
    }
  }

  if(serve_color[3]==1){
    if(number_color[3]<=numCameras){
      if(fwCamFeatures[number_color[3]].AUTO_FOCUS_CONFIG==1)
         printf("colorD autofocus feature: on\n");
      else
         printf("colorD autofocus feature: off\n");

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
    }else{
      serve_color[3]=0;
      printf("cannot find firewire camera for colorD\n");
    }
  }
  /*creates new schema for varcolorA*/
  if(serve_color[4]==1){
     if(number_color[4]<=numCameras){
        if(fwCamFeatures[number_color[4]].AUTO_FOCUS_CONFIG==1)
           printf("varcolorA autofocus feature: on\n");
        else
           printf("varcolorA autofocus feature: off\n");
        
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
	
	myA.img=(char *)malloc (width[4]*height[4]*3);
	myA.width=width[4];
	myA.height=height[4];
	myA.clock=0;
        myexport("varcolorA","varcolorA",&myA);
        myexport("varcolorA","id",&varcolorA_schema_id);
        myexport("varcolorA","run",(void *)myvarcolorA_run);
        myexport("varcolorA","stop",(void *)myvarcolorA_stop);
     }else{
        serve_color[4]=0;
        printf("cannot find firewire camera for varcolorA\n");
     }
  }
  /*creates new schema for varcolorB*/
  if(serve_color[5]==1){
     if(number_color[5]<=numCameras){
        if(fwCamFeatures[number_color[5]].AUTO_FOCUS_CONFIG==1)
           printf("varcolorB autofocus feature: on\n");
        else
           printf("varcolorB autofocus feature: off\n");

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

        myB.img=(char *)malloc (width[5]*height[5]*3);
	myB.width=width[5];
	myB.height=height[5];
	myB.clock=0;
        myexport("varcolorB","varcolorB",&myB);
        myexport("varcolorB","id",&varcolorB_schema_id);
        myexport("varcolorB","run",(void *)myvarcolorB_run);
        myexport("varcolorB","stop",(void *)myvarcolorB_stop);
     }else{
        serve_color[5]=0;
        printf("cannot find firewire camera for varcolorB\n");
     }
  }

  /*creates new schema for varcolorC*/
  if(serve_color[6]==1){
     if(number_color[6]<=numCameras){
        if(fwCamFeatures[number_color[6]].AUTO_FOCUS_CONFIG==1)
           printf("varcolorC autofocus feature: on\n");
        else
           printf("varcolorC autofocus feature: off\n");

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

	myC.img=(char *)malloc (width[6]*height[6]*3);
	myC.width=width[6];
	myC.height=height[6];
	myC.clock=0;
        myexport("varcolorC","varcolorC",&myC);
        myexport("varcolorC","id",&varcolorC_schema_id);
        myexport("varcolorC","run",(void *)myvarcolorC_run);
        myexport("varcolorC","stop",(void *)myvarcolorC_stop);
     }else{
        serve_color[6]=0;
        printf("cannot find firewire camera for varcolorC\n");
     }
  }

  /*creates new schema for varcolorD*/
  if(serve_color[7]==1){
     if(number_color[7]<=numCameras){
        if(fwCamFeatures[number_color[7]].AUTO_FOCUS_CONFIG==1)
           printf("varcolorD autofocus feature: on\n");
        else
           printf("varcolorD autofocus feature: off\n");

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

	myD.img=(char *)malloc (width[7]*height[7]*3);
	myD.width=width[7];
	myD.height=height[7];
	myD.clock=0;
        myexport("varcolorD","varcolorD",&myD);
        myexport("varcolorD","id",&varcolorD_schema_id);
        myexport("varcolorD","run",(void *)myvarcolorD_run);
        myexport("varcolorD","stop",(void *)myvarcolorD_stop);
     }else{
        serve_color[7]=0;
        printf("cannot find firewire camera for varcolorD\n");
     }
  }
  
  printf("firewire driver started up\n");
}
