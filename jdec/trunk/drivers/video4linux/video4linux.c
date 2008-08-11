/*
 *  Copyright (C) 2007 Jose Maria Cañas Plaza
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
 */

/**
 *  jdec video4linux driver provides video images to color variables from video4linux devices, such as USB or BT878 cameras.
 *
 *  @file video4linux.c
 *  @author Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es> and Jose Maria Ca�as Plaza <jmplaza@gsyc.escet.urjc.es>
 *  @version 4.1
 *  @date 30-05-2007
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <jde.h>

/* SIF image size */
#define SIFNTSC_ROWS 240
#define SIFNTSC_COLUMNS 320

/** Using linux time headers.*/
#define _LINUX_TIME_H 1
/** Using linux device headers.*/
#define _DEVICE_H_ 1
/* before videodev.h. It avoids /usr/include/linux/time.h:9: redefinition of `struct timespec' */
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/videodev.h>

#define CLIP         320
# define RED_NULL    128  /* 137 */
# define BLUE_NULL   128  /* 156 */
# define LUN_MUL     256  /* 360 */
# define RED_MUL     512
# define BLUE_MUL    512
#define GREEN1_MUL  (-RED_MUL/2)
#define GREEN2_MUL  (-BLUE_MUL/6)
#define RED_ADD     (-RED_NULL  * RED_MUL)
#define BLUE_ADD    (-BLUE_NULL * BLUE_MUL)
#define GREEN1_ADD  (-RED_ADD/2)
#define GREEN2_ADD  (-BLUE_ADD/6)


/** Color conversion, taken from xawtv.*/
#define CLIP        320
/** Color conversion, taken from xawtv.*/
# define RED_NULL    128  /* 137 */
/** Color conversion, taken from xawtv.*/
# define BLUE_NULL   128  /* 156 */
/** Color conversion, taken from xawtv.*/
# define LUN_MUL     256  /* 360 */
/** Color conversion, taken from xawtv.*/
#define RED_MUL     512
/** Color conversion, taken from xawtv.*/
#define BLUE_MUL    512
/** Color conversion, taken from xawtv.*/
#define GREEN1_MUL  (-RED_MUL/2)
/** Color conversion, taken from xawtv.*/
#define GREEN2_MUL  (-BLUE_MUL/6)
/** Color conversion, taken from xawtv.*/
#define RED_ADD     (-RED_NULL  * RED_MUL)
/** Color conversion, taken from xawtv.*/
#define BLUE_ADD    (-BLUE_NULL * BLUE_MUL)
/** Color conversion, taken from xawtv.*/
#define GREEN1_ADD  (-RED_ADD/2)
/** Color conversion, taken from xawtv.*/
#define GREEN2_ADD  (-BLUE_ADD/6)

/** Lookup tables for color conversion.*/
static unsigned int  ng_yuv_gray[256];
/** Lookup tables for color conversion.*/
static unsigned int  ng_yuv_red[256];
/** Lookup tables for color conversion.*/
static unsigned int  ng_yuv_blue[256];
/** Lookup tables for color conversion.*/
static unsigned int  ng_yuv_g1[256];
/** Lookup tables for color conversion.*/
static unsigned int  ng_yuv_g2[256];
/** Lookup tables for color conversion.*/
static unsigned int  ng_clip[256 + 2 * CLIP];

/** Macro to transform color to gray.*/
#define GRAY(val)		ng_yuv_gray[val]
/** Macro to transform color to red.*/
#define RED(gray,red)		ng_clip[ CLIP + gray + ng_yuv_red[red] ]
/** Macro to transform color to green.*/
#define GREEN(gray,red,blue)	ng_clip[ CLIP + gray + ng_yuv_g1[red] + ng_yuv_g2[blue] ]
/** Macro to transform color to blue.*/
#define BLUE(gray,blue)		ng_clip[ CLIP + gray + ng_yuv_blue[blue] ]


/** Driver name.*/
char driver_name[256]="video4linux";
/** Flag to stop the threads when the driver is closed */
int finish_flag=0;
/** Max number of cameras served by this video4linux driver.*/
#define MAXCAM 4
/** Map local memory for v4l capture. */
static char *map[MAXCAM];
/** File descriptors for /dev/video cameras.*/
int fdv4l[MAXCAM];
/** Video capability option.*/
struct video_capability cap[MAXCAM];
/** Video channel option.*/
struct video_channel chan[MAXCAM];
/** Video picture option.*/
struct video_picture vpic[MAXCAM];
/** Video buffers. */
static struct video_mbuf gb_buffers[MAXCAM]; 
/** Video window option. Includes its size: .width y .height */
struct video_window win[MAXCAM];
/** Video mmap option.*/
struct video_mmap gb[MAXCAM];
/** Threads really used */
int v4l_requested[MAXCAM];
/** Device names. Example: "/dev/video0".*/
char v4l_filename[MAXCAM][256];


enum destinations {colorA,colorB,colorC,colorD,varcolorA,varcolorB,varcolorC,varcolorD,MAXDEST};
/** Colors requested in config file. If color_requested[0] = 1, then colorA was detected in config file */
int color_requested[MAXDEST];
/** Active colors at each moment. If color_active[0] = 1, then colorA is being served right now */
int color_active[MAXDEST];
int color_v4l[MAXDEST]; /** Which device-camera serves which color.*/
int color_schema_id[MAXDEST];
unsigned long int color_clock[MAXDEST]; /* time stamp of the last image captured */
int color_refs[MAXDEST]; /* Contadores de referencias*/
pthread_mutex_t refmutex; /** mutex for ref counters*/
 
/*API variables servidas, image data */
char *colorA_image; 
char *colorB_image; 
char *colorC_image; 
char *colorD_image; 
char *varcolorA_image; 
char *varcolorB_image; 
char *varcolorC_image; 
char *varcolorD_image; 


/** colorA resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorA_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (color_refs[colorA]>0){
      color_refs[colorA]++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[colorA]=1;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[colorA]==1)&&(color_active[colorA]==0))
	{
	  color_active[colorA]=1;
	  printf("colorA schema resume (video4linux driver)\n");
	  pthread_mutex_lock(&(all[color_schema_id[colorA]].mymutex));
	  all[color_schema_id[colorA]].father = father;
	  all[color_schema_id[colorA]].fps = 0.;
	  all[color_schema_id[colorA]].k =0;
	  put_state(color_schema_id[colorA],winner);
	  pthread_cond_signal(&(all[color_schema_id[colorA]].condition));
	  pthread_mutex_unlock(&(all[color_schema_id[colorA]].mymutex));
	}
   }
   return 0;
}

/** colorA suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorA_suspend(){

   pthread_mutex_lock(&refmutex);
   if (color_refs[colorA]>1){
      color_refs[colorA]--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[colorA]=0;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[colorA]==1)&&(color_active[colorA])){
         color_active[colorA]=0;
         printf("colorA schema suspend (video4linux driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[colorA]].mymutex));
         put_state(color_schema_id[colorA],slept);
         pthread_mutex_unlock(&(all[color_schema_id[colorA]].mymutex));
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

   pthread_mutex_lock(&refmutex);
   if (color_refs[colorB]>0){
      color_refs[colorB]++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[colorB]=1;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[colorB]==1)&&(color_active[colorB]==0)){
         color_active[colorB]=1;
         printf("colorB schema resume (video4linux driver)\n");
	 pthread_mutex_lock(&(all[color_schema_id[colorB]].mymutex)); 
	 all[color_schema_id[colorB]].father = father;
         all[color_schema_id[colorB]].fps = 0.;
         all[color_schema_id[colorB]].k =0;
         put_state(color_schema_id[colorB],winner);
         pthread_cond_signal(&(all[color_schema_id[colorB]].condition));
         pthread_mutex_unlock(&(all[color_schema_id[colorB]].mymutex));
      }
   }
   return 0;
}

/** colorB suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorB_suspend(){

   pthread_mutex_lock(&refmutex);
   if (color_refs[colorB]>1){
      color_refs[colorB]--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[colorB]=0;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[colorB]==1)&&(color_active[colorB])){
         color_active[colorB]=0;
         printf("colorB schema suspend (video4linux driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[colorB]].mymutex));
         put_state(color_schema_id[colorB],slept);
         pthread_mutex_unlock(&(all[color_schema_id[colorB]].mymutex));
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
   pthread_mutex_lock(&refmutex);
   if (color_refs[colorC]>0){
      color_refs[colorC]++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[colorC]=1;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[colorC]==1)&&(color_active[colorC]==0)){
         color_active[colorC]=1;
         printf("colorC schema resume (video4linux driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[colorC]].mymutex));
         all[color_schema_id[colorC]].father = father;
         all[color_schema_id[colorC]].fps = 0.;
         all[color_schema_id[colorC]].k =0;
         put_state(color_schema_id[colorC],winner);
         pthread_cond_signal(&(all[color_schema_id[colorC]].condition));
         pthread_mutex_unlock(&(all[color_schema_id[colorC]].mymutex));
      }
   }
   return 0;
}

/** colorC suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorC_suspend(){
   pthread_mutex_lock(&refmutex);
   if (color_refs[colorC]>1){
      color_refs[colorC]--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[colorC]=0;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[colorC]==1)&&(color_active[colorC])){
         color_active[colorC]=0;
         printf("colorC schema suspend (video4linux driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[colorC]].mymutex));
         put_state(color_schema_id[colorC],slept);
         pthread_mutex_unlock(&(all[color_schema_id[colorC]].mymutex));
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
   pthread_mutex_lock(&refmutex);
   if (color_refs[colorD]>0){
      color_refs[colorD]++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[colorD]=1;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[colorD]==1)&&(color_active[colorD]==0)){
         color_active[colorD]=1;
         printf("colorD schema resume (video4linux driver)\n");
	 pthread_mutex_lock(&(all[color_schema_id[colorD]].mymutex));
	 all[color_schema_id[colorD]].father = father;
         all[color_schema_id[colorD]].fps = 0.;
         all[color_schema_id[colorD]].k =0;
         put_state(color_schema_id[colorD],winner);
         pthread_cond_signal(&(all[color_schema_id[colorD]].condition));
         pthread_mutex_unlock(&(all[color_schema_id[colorD]].mymutex));
      }
   }
   return 0;
}

/** colorD suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorD_suspend(){
   pthread_mutex_lock(&refmutex);
   if (color_refs[colorD]>1){
      color_refs[colorD]--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[colorD]=0;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[colorD]==1)&&(color_active[colorD])){
         color_active[colorD]=0;
         printf("colorD schema suspend (video4linux driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[colorD]].mymutex));
         put_state(color_schema_id[colorD],slept);
         pthread_mutex_unlock(&(all[color_schema_id[colorD]].mymutex));
      }
   }
   return 0;
}

/** varcolorA resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorA_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (color_refs[varcolorA]>0){
      color_refs[varcolorA]++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[varcolorA]=1;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[varcolorA]==1)&&(color_active[varcolorA]==0))
	{
	  color_active[varcolorA]=1;
	  printf("varcolorA schema resume (video4linux driver)\n");
	  pthread_mutex_lock(&(all[color_schema_id[varcolorA]].mymutex));
	  all[color_schema_id[varcolorA]].father = father;
	  all[color_schema_id[varcolorA]].fps = 0.;
	  all[color_schema_id[varcolorA]].k =0;
	  put_state(color_schema_id[varcolorA],winner);
	  pthread_cond_signal(&(all[color_schema_id[varcolorA]].condition));
	  pthread_mutex_unlock(&(all[color_schema_id[varcolorA]].mymutex));
	}
   }
   return 0;
}

/** varcolorA suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int myvarcolorA_suspend(){

   pthread_mutex_lock(&refmutex);
   if (color_refs[varcolorA]>1){
      color_refs[varcolorA]--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[varcolorA]=0;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[varcolorA]==1)&&(color_active[varcolorA])){
         color_active[varcolorA]=0;
         printf("varcolorA schema suspend (video4linux driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[varcolorA]].mymutex));
         put_state(color_schema_id[varcolorA],slept);
         pthread_mutex_unlock(&(all[color_schema_id[varcolorA]].mymutex));
      }
   }
   return 0;
}

/** varcolorB resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorB_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (color_refs[varcolorB]>0){
      color_refs[varcolorB]++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[varcolorB]=1;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[varcolorB]==1)&&(color_active[varcolorB]==0))
	{
	  color_active[varcolorB]=1;
	  printf("varcolorB schema resume (video4linux driver)\n");
	  pthread_mutex_lock(&(all[color_schema_id[varcolorB]].mymutex));
	  all[color_schema_id[varcolorB]].father = father;
	  all[color_schema_id[varcolorB]].fps = 0.;
	  all[color_schema_id[varcolorB]].k =0;
	  put_state(color_schema_id[varcolorB],winner);
	  pthread_cond_signal(&(all[color_schema_id[varcolorB]].condition));
	  pthread_mutex_unlock(&(all[color_schema_id[varcolorB]].mymutex));
	}
   }
   return 0;
}

/** varcolorB suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int myvarcolorB_suspend(){

   pthread_mutex_lock(&refmutex);
   if (color_refs[varcolorB]>1){
      color_refs[varcolorB]--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[varcolorB]=0;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[varcolorB]==1)&&(color_active[varcolorB])){
         color_active[varcolorB]=0;
         printf("varcolorB schema suspend (video4linux driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[varcolorB]].mymutex));
         put_state(color_schema_id[varcolorB],slept);
         pthread_mutex_unlock(&(all[color_schema_id[varcolorB]].mymutex));
      }
   }
   return 0;
}


/** varcolorC resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorC_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (color_refs[varcolorC]>0){
      color_refs[varcolorC]++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[varcolorC]=1;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[varcolorC]==1)&&(color_active[varcolorC]==0))
	{
	  color_active[varcolorC]=1;
	  printf("varcolorC schema resume (video4linux driver)\n");
	  pthread_mutex_lock(&(all[color_schema_id[varcolorC]].mymutex));
	  all[color_schema_id[varcolorC]].father = father;
	  all[color_schema_id[varcolorC]].fps = 0.;
	  all[color_schema_id[varcolorC]].k =0;
	  put_state(color_schema_id[varcolorC],winner);
	  pthread_cond_signal(&(all[color_schema_id[varcolorC]].condition));
	  pthread_mutex_unlock(&(all[color_schema_id[varcolorC]].mymutex));
	}
   }
   return 0;
}

/** varcolorC suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int myvarcolorC_suspend(){

   pthread_mutex_lock(&refmutex);
   if (color_refs[varcolorC]>1){
      color_refs[varcolorC]--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[varcolorC]=0;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[varcolorC]==1)&&(color_active[varcolorC])){
         color_active[varcolorC]=0;
         printf("varcolorC schema suspend (video4linux driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[varcolorC]].mymutex));
         put_state(color_schema_id[varcolorC],slept);
         pthread_mutex_unlock(&(all[color_schema_id[varcolorC]].mymutex));
      }
   }
   return 0;
}

/** varcolorD resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorD_resume(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (color_refs[varcolorD]>0){
      color_refs[varcolorD]++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[varcolorD]=1;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[varcolorD]==1)&&(color_active[varcolorD]==0))
	{
	  color_active[varcolorD]=1;
	  printf("varcolorD schema resume (video4linux driver)\n");
	  pthread_mutex_lock(&(all[color_schema_id[varcolorD]].mymutex));
	  all[color_schema_id[varcolorD]].father = father;
	  all[color_schema_id[varcolorD]].fps = 0.;
	  all[color_schema_id[varcolorD]].k =0;
	  put_state(color_schema_id[varcolorD],winner);
	  pthread_cond_signal(&(all[color_schema_id[varcolorD]].condition));
	  pthread_mutex_unlock(&(all[color_schema_id[varcolorD]].mymutex));
	}
   }
   return 0;
}

/** varcolorD suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int myvarcolorD_suspend(){

   pthread_mutex_lock(&refmutex);
   if (color_refs[varcolorD]>1){
      color_refs[varcolorD]--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      color_refs[varcolorD]=0;
      pthread_mutex_unlock(&refmutex);
      if((color_requested[varcolorD]==1)&&(color_active[varcolorD])){
         color_active[varcolorD]=0;
         printf("varcolorD schema suspend (video4linux driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[varcolorD]].mymutex));
         put_state(color_schema_id[varcolorD],slept);
         pthread_mutex_unlock(&(all[color_schema_id[varcolorD]].mymutex));
      }
   }
   return 0;
}




void *video4linux_thread0(){
  int cam=0; /* thread0 => cam =0 */
  int color; /* the color (colorA,colorB...) already served by this thread */
  int f,c,yy,j;
  unsigned char y,u,v;
  unsigned char *src,*dest;
  /** int variable to detect when a image was captured.*/
  struct timeval a;
  int cols, rows;

  printf("video4linux: thread %d started\n",cam);

  for(j=0;j<MAXDEST;j++)
    if ((color_requested[j]) && (color_v4l[j]==cam))
      {color=j;
      }

  src = map[cam]+gb_buffers[cam].offsets[gb[cam].frame];
  if (color==colorA) dest=(unsigned char*)colorA_image;
  else if (color==colorB) dest=(unsigned char*)colorB_image;
  else if (color==colorC) dest=(unsigned char*)colorC_image;
  else if (color==colorD) dest=(unsigned char*)colorD_image;
  else if (color==varcolorA) dest=(unsigned char*)varcolorA_image;
  else if (color==varcolorB) dest=(unsigned char*)varcolorB_image;
  else if (color==varcolorC) dest=(unsigned char*)varcolorC_image;
  else if (color==varcolorD) dest=(unsigned char*)varcolorD_image;

  cols = win[cam].width;
  rows = win[cam].height;
  
  for(;finish_flag==0;)  
    {
      pthread_mutex_lock(&(all[color_schema_id[color]].mymutex));
      
      if (all[color_schema_id[color]].state==slept){
	printf("video4linux: thread %d goes sleep mode\n",cam);
	pthread_cond_wait(&(all[color_schema_id[color]].condition),&(all[color_schema_id[color]].mymutex));
	printf("video4linux: thread %d woke up\n",cam);
	pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));
      }else{      
	pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));
	
	/* video4linux image capture */
	
	speedcounter(color_schema_id[color]);
	gettimeofday(&a,NULL);
	color_clock[color]= (a.tv_sec)*1000000+a.tv_usec;
	
	if (vpic[cam].palette==VIDEO_PALETTE_YUV420P){
	  for (j = 0; j < cols*rows; j++) {
	    f=j/cols; c=j%cols;
	    y=src[j];
	    u=src[cols*rows+(f/2)*cols/2+c/2];
	    v=src[cols*rows*5/4+(f/2)*cols/2+c/2];
	    yy = GRAY(y);
	    dest[3*j]=BLUE(yy,u);
	    dest[3*j+1]=GREEN(yy,v,u);
	    dest[3*j+2]=RED(yy,v);	
	  }
	  
	}else if (vpic[cam].palette==VIDEO_PALETTE_RGB565){
	  for (j = 0; j < cols*rows; j++) {
	    dest[3*j]=  (0x1F&src[2*j])<<3; /* blue */
	    dest[3*j+1]= ((0x07&src[2*j+1])<<5) | ((0xE0&src[2*j])>>3); /* green */
	    dest[3*j+2]= (0xF8&(src[2*j+1]));  /* red */
	  }
	  
	}else if (vpic[cam].palette==VIDEO_PALETTE_RGB24){
	  memcpy(dest,src,cols*rows*3);
	}
	
	/* asks for next capture */
	gb[cam].frame=(gb[cam].frame+1)%2;
	if (-1 == ioctl(fdv4l[cam],VIDIOCMCAPTURE,&gb[cam])) {perror("VIDIOCMCAPTURE");}
	
	/* "libera" ese frame para que el driver pueda cargar ahi nueva imagen.*/
	if (-1 == ioctl(fdv4l[cam],VIDIOCSYNC,&gb[cam].frame)) {
	  perror("VIDIOCSYNC");
	}
      }
    }
  pthread_exit(0);
}


void *video4linux_thread1(){
  int cam=1; /* thread0 => cam =0 */
  int color; /* the color (colorA,colorB...) already served by this thread */
  int f,c,yy,j;
  unsigned char y,u,v;
  unsigned char *src,*dest;
  /** int variable to detect when a image was captured.*/
  struct timeval a;
  int cols, rows;

  printf("video4linux: thread %d started\n",cam);

  for(j=0;j<MAXDEST;j++)
    if ((color_requested[j]) && (color_v4l[j]==cam))
      {color=j;
      }

  src = map[cam]+gb_buffers[cam].offsets[gb[cam].frame];
  if (color==colorA) dest=(unsigned char*)colorA_image;
  else if (color==colorB) dest=(unsigned char*)colorB_image;
  else if (color==colorC) dest=(unsigned char*)colorC_image;
  else if (color==colorD) dest=(unsigned char*)colorD_image;
  else if (color==varcolorA) dest=(unsigned char*)varcolorA_image;
  else if (color==varcolorB) dest=(unsigned char*)varcolorB_image;
  else if (color==varcolorC) dest=(unsigned char*)varcolorC_image;
  else if (color==varcolorD) dest=(unsigned char*)varcolorD_image;
  
  cols = win[cam].width;
  rows = win[cam].height;
  
  for(;finish_flag==0;)  
    {
      pthread_mutex_lock(&(all[color_schema_id[color]].mymutex));
      
      if (all[color_schema_id[color]].state==slept){
	printf("video4linux: thread %d goes sleep mode\n",cam);
	pthread_cond_wait(&(all[color_schema_id[color]].condition),&(all[color_schema_id[color]].mymutex));
	printf("video4linux: thread %d woke up\n",cam);
	pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));
      }else{      
	pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));
	
	/* video4linux image capture */
	
	speedcounter(color_schema_id[color]);
	gettimeofday(&a,NULL);
	color_clock[color]= (a.tv_sec)*1000000+a.tv_usec;
	
	if (vpic[cam].palette==VIDEO_PALETTE_YUV420P){
	  for (j = 0; j < cols*rows; j++) {
	    f=j/cols; c=j%cols;
	    y=src[j];
	    u=src[cols*rows+(f/2)*cols/2+c/2];
	    v=src[cols*rows*5/4+(f/2)*cols/2+c/2];
	    yy = GRAY(y);
	    dest[3*j]=BLUE(yy,u);
	    dest[3*j+1]=GREEN(yy,v,u);
	    dest[3*j+2]=RED(yy,v);	
	  }
	  
	}else if (vpic[cam].palette==VIDEO_PALETTE_RGB565){
	  for (j = 0; j < cols*rows; j++) {
	    dest[3*j]=  (0x1F&src[2*j])<<3; /* blue */
	    dest[3*j+1]= ((0x07&src[2*j+1])<<5) | ((0xE0&src[2*j])>>3); /* green */
	    dest[3*j+2]= (0xF8&(src[2*j+1]));  /* red */
	  }
	  
	}else if (vpic[cam].palette==VIDEO_PALETTE_RGB24){
	  memcpy(dest,src,cols*rows*3);
	}
	
	/* asks for next capture */
	gb[cam].frame=(gb[cam].frame+1)%2;
	if (-1 == ioctl(fdv4l[cam],VIDIOCMCAPTURE,&gb[cam])) {perror("VIDIOCMCAPTURE");}
	
	/* "libera" ese frame para que el driver pueda cargar ahi nueva imagen.*/
	if (-1 == ioctl(fdv4l[cam],VIDIOCSYNC,&gb[cam].frame)) {
	  perror("VIDIOCSYNC");
	}
      }
    }
  pthread_exit(0);
}


void *video4linux_thread2(){
  int cam=2; /* thread0 => cam =0 */
  int color; /* the color (colorA,colorB...) already served by this thread */
  int f,c,yy,j;
  unsigned char y,u,v;
  unsigned char *src,*dest;
  /** int variable to detect when a image was captured.*/
  struct timeval a;
  int cols, rows;

  printf("video4linux: thread %d started\n",cam);

  for(j=0;j<MAXDEST;j++)
    if ((color_requested[j]) && (color_v4l[j]==cam))
      {color=j;
      }

  src = map[cam]+gb_buffers[cam].offsets[gb[cam].frame];
  if (color==colorA) dest=(unsigned char*)colorA_image;
  else if (color==colorB) dest=(unsigned char*)colorB_image;
  else if (color==colorC) dest=(unsigned char*)colorC_image;
  else if (color==colorD) dest=(unsigned char*)colorD_image;
  else if (color==varcolorA) dest=(unsigned char*)varcolorA_image;
  else if (color==varcolorB) dest=(unsigned char*)varcolorB_image;
  else if (color==varcolorC) dest=(unsigned char*)varcolorC_image;
  else if (color==varcolorD) dest=(unsigned char*)varcolorD_image;

  cols = win[cam].width;
  rows = win[cam].height;
    
  for(;finish_flag==0;)  
    {
      pthread_mutex_lock(&(all[color_schema_id[color]].mymutex));
      
      if (all[color_schema_id[color]].state==slept){
	printf("video4linux: thread %d goes sleep mode\n",cam);
	pthread_cond_wait(&(all[color_schema_id[color]].condition),&(all[color_schema_id[color]].mymutex));
	printf("video4linux: thread %d woke up\n",cam);
	pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));
      }else{      
	pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));
	
	/* video4linux image capture */
	
	speedcounter(color_schema_id[color]);
	gettimeofday(&a,NULL);
	color_clock[color]= (a.tv_sec)*1000000+a.tv_usec;
	
	if (vpic[cam].palette==VIDEO_PALETTE_YUV420P){
	  for (j = 0; j < cols*rows; j++) {
	    f=j/cols; c=j%cols;
	    y=src[j];
	    u=src[cols*rows+(f/2)*cols/2+c/2];
	    v=src[cols*rows*5/4+(f/2)*cols/2+c/2];
	    yy = GRAY(y);
	    dest[3*j]=BLUE(yy,u);
	    dest[3*j+1]=GREEN(yy,v,u);
	    dest[3*j+2]=RED(yy,v);	
	  }
	  
	}else if (vpic[cam].palette==VIDEO_PALETTE_RGB565){
	  for (j = 0; j < cols*rows; j++) {
	    dest[3*j]=  (0x1F&src[2*j])<<3; /* blue */
	    dest[3*j+1]= ((0x07&src[2*j+1])<<5) | ((0xE0&src[2*j])>>3); /* green */
	    dest[3*j+2]= (0xF8&(src[2*j+1]));  /* red */
	  }
	  
	}else if (vpic[cam].palette==VIDEO_PALETTE_RGB24){
	  memcpy(dest,src,cols*rows*3);
	}
	
	/* asks for next capture */
	gb[cam].frame=(gb[cam].frame+1)%2;
	if (-1 == ioctl(fdv4l[cam],VIDIOCMCAPTURE,&gb[cam])) {perror("VIDIOCMCAPTURE");}
	
	/* "libera" ese frame para que el driver pueda cargar ahi nueva imagen.*/
	if (-1 == ioctl(fdv4l[cam],VIDIOCSYNC,&gb[cam].frame)) {
	  perror("VIDIOCSYNC");
	}
      }
    }
  pthread_exit(0);
}


void *video4linux_thread3(){
  int cam=3; /* thread0 => cam =0 */
  int color; /* the color (colorA,colorB...) already served by this thread */
  int f,c,yy,j;
  unsigned char y,u,v;
  unsigned char *src,*dest;
  /** int variable to detect when a image was captured.*/
  struct timeval a;
  int cols, rows;

  printf("video4linux: thread %d started\n",cam);

  for(j=0;j<MAXDEST;j++)
    if ((color_requested[j]) && (color_v4l[j]==cam))
      {color=j;
      }

  src = map[cam]+gb_buffers[cam].offsets[gb[cam].frame];
  if (color==colorA) dest=(unsigned char*)colorA_image;
  else if (color==colorB) dest=(unsigned char*)colorB_image;
  else if (color==colorC) dest=(unsigned char*)colorC_image;
  else if (color==colorD) dest=(unsigned char*)colorD_image;
  else if (color==varcolorA) dest=(unsigned char*)varcolorA_image;
  else if (color==varcolorB) dest=(unsigned char*)varcolorB_image;
  else if (color==varcolorC) dest=(unsigned char*)varcolorC_image;
  else if (color==varcolorD) dest=(unsigned char*)varcolorD_image;

  cols = win[cam].width;
  rows = win[cam].height;
    
  for(;finish_flag==0;)  
    {
      pthread_mutex_lock(&(all[color_schema_id[color]].mymutex));
      
      if (all[color_schema_id[color]].state==slept){
	printf("video4linux: thread %d goes sleep mode\n",cam);
	pthread_cond_wait(&(all[color_schema_id[color]].condition),&(all[color_schema_id[color]].mymutex));
	printf("video4linux: thread %d woke up\n",cam);
	pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));
      }else{      
	pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));
	
	/* video4linux image capture */
	
	speedcounter(color_schema_id[color]);
	gettimeofday(&a,NULL);
	color_clock[color]= (a.tv_sec)*1000000+a.tv_usec;
	
	if (vpic[cam].palette==VIDEO_PALETTE_YUV420P){
	  for (j = 0; j < cols*rows; j++) {
	    f=j/cols; c=j%cols;
	    y=src[j];
	    u=src[cols*rows+(f/2)*cols/2+c/2];
	    v=src[cols*rows*5/4+(f/2)*cols/2+c/2];
	    yy = GRAY(y);
	    dest[3*j]=BLUE(yy,u);
	    dest[3*j+1]=GREEN(yy,v,u);
	    dest[3*j+2]=RED(yy,v);	
	  }
	  
	}else if (vpic[cam].palette==VIDEO_PALETTE_RGB565){
	  for (j = 0; j < cols*rows; j++) {
	    dest[3*j]=  (0x1F&src[2*j])<<3; /* blue */
	    dest[3*j+1]= ((0x07&src[2*j+1])<<5) | ((0xE0&src[2*j])>>3); /* green */
	    dest[3*j+2]= (0xF8&(src[2*j+1]));  /* red */
	  }
	  
	}else if (vpic[cam].palette==VIDEO_PALETTE_RGB24){
	  memcpy(dest,src,cols*rows*3);
	}
	
	/* asks for next capture */
	gb[cam].frame=(gb[cam].frame+1)%2;
	if (-1 == ioctl(fdv4l[cam],VIDIOCMCAPTURE,&gb[cam])) {perror("VIDIOCMCAPTURE");}
	
	/* "libera" ese frame para que el driver pueda cargar ahi nueva imagen.*/
	if (-1 == ioctl(fdv4l[cam],VIDIOCSYNC,&gb[cam].frame)) {
	  perror("VIDIOCSYNC");
	}
      }
    }
  pthread_exit(0);
}

/** video4linux driver parse configuration file function.
 *  @param configfile path and name to the config file.
 *  @return 0 if parsing was successful or -1 if something went wrong.*/
int video4linux_parseconf(char *configfile){

  int end_parse=0; int end_section=0; int driver_config_parsed=0;
  FILE *myfile;
  const int limit = 256;
  int numcams = 0;

  if ((myfile=fopen(configfile,"r"))==NULL){
    printf("video4linux: cannot find config file\n");
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
	printf ("video4linux: line too long in config file!\n"); 
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
		    printf("video4linux: error in config file.\n'end_section' keyword required before starting new driver section.\n");
		    end_section=1; end_parse=1;

		  }else if(strcmp(word3,"provides")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s %s %s %s",word3,word4,word5,word6,word7)>2){
		      v4l_requested[numcams]=1;
		      strcpy(v4l_filename[numcams],word5);
		      if (strcmp(word4,"colorA")==0)
			{
			  win[numcams].width=SIFNTSC_COLUMNS;
			  win[numcams].height=SIFNTSC_ROWS;
			  color_requested[colorA]=1;
			  color_v4l[colorA]=numcams;
			}
		      else if (strcmp(word4,"colorB")==0)
			{
			  win[numcams].width=SIFNTSC_COLUMNS;
			  win[numcams].height=SIFNTSC_ROWS;
			  color_requested[colorB]=1; 
			  color_v4l[colorB]=numcams;
			}
		      else if (strcmp(word4,"colorC")==0)
			{
			  win[numcams].width=SIFNTSC_COLUMNS;
			  win[numcams].height=SIFNTSC_ROWS;
			  color_requested[colorC]=1; 
			  color_v4l[colorC]=numcams;
			}
		      else if (strcmp(word4,"colorD")==0)
			{
			  win[numcams].width=SIFNTSC_COLUMNS;
			  win[numcams].height=SIFNTSC_ROWS;
			  color_requested[colorD]=1; 
			  color_v4l[colorD]=numcams;
			}
		      else if (strcmp(word4,"varcolorA")==0)
			{
			  win[numcams].width=atoi(word6);
			  win[numcams].height=atoi(word7);
			  color_requested[varcolorA]=1; 
			  color_v4l[varcolorA]=numcams;
			}
		      else if (strcmp(word4,"varcolorB")==0)
			{
			  win[numcams].width=atoi(word6);
			  win[numcams].height=atoi(word7);
			  color_requested[varcolorB]=1; 
			  color_v4l[varcolorB]=numcams;
			}
		      else if (strcmp(word4,"varcolorC")==0)
			{
			  win[numcams].width=atoi(word6);
			  win[numcams].height=atoi(word7);
			  color_requested[varcolorC]=1; 
			  color_v4l[varcolorC]=numcams;
			}
		      else if (strcmp(word4,"varcolorD")==0)
			{
			  win[numcams].width=atoi(word6);
			  win[numcams].height=atoi(word7);
			  color_requested[varcolorD]=1; 
			  color_v4l[varcolorD]=numcams;
			}
		      printf(" Requested size: %d x %d \n", win[numcams].width, win[numcams].height);
		      numcams++;
		    }else{
		      printf("video4linux: provides line incorrect\n");
		    }
		  }else printf("video4linux: i don't know what to do with '%s'\n",buffer_file2);
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
  if (driver_config_parsed==1)
    {
      if (numcams==0) 
	printf("video4linux: warning! no color provided.\n");
      return 0;
    }
  else return -1;
}

/** video4linux driver init function. It will start all video4linux required devices and setting them the default configuration.
 *  @return 0 if initialitation was successful or -1 if something went wrong.*/
int video4linux_init(){

  int i,k,n;

  /* recorremos todos los posibles dispositivos */
  for(n=0;n<MAXCAM;n++){

    /* inicializamos la apertura de todos los dispositivos */
    if(v4l_requested[n]){

      /* Apertura e inicializacion del dispositivo de videoforlinux */
      fdv4l[n] = open(v4l_filename[n], O_RDWR);

      if (fdv4l[n] < 0) {perror(v4l_filename[n]); return -1;}
      if (ioctl(fdv4l[n], VIDIOCGCAP, &cap[n]) < 0) {perror("VIDIOGCAP"); return -1;}
      else {
	printf("CAPABILITY: \n name=%s \n type=%d \n channels=%d \n audios=%d \n maxwidth=%d \n maxheight=%d \n minwidth=%d \n minheight=%d \n\n", cap[n].name, cap[n].type, cap[n].channels, cap[n].audios, cap[n].maxwidth, cap[n].maxheight, cap[n].minwidth, cap[n].minheight);
	printf("%s detected\n",cap[n].name);
      }
	
      
      /*  fcntl(fdv4l[n],F_SETFD,FD_CLOEXEC); */
      for(i=0; i<cap[n].channels; i++){
	chan[n].channel=(int) i;
	if (ioctl(fdv4l[n], VIDIOCGCHAN, &chan[n]) == -1) {
	  perror("VIDIOCGCHAN");
	  close(fdv4l[n]);
	  return -1;
	}else printf(" CHANNEL %d: \n name=%s \n tuners=%d \n type=%d \n\n",chan[n].channel,chan[n].name,chan[n].tuners,chan[n].type);
      }

      /*image size is set at video4linux_parseconf */
      win[n].x=0;
      win[n].y=0; 
      win[n].chromakey=0;
      win[n].clipcount=0;
  
      if ((strcmp(cap[n].name,"BT878(Hauppauge new)")==0) && (cap[n].channels == 3)){
	/* Selecciona el canal composite-video (=1) como canal activo. Si pongo canal Television (=0) tambien funciona. Si pongo s-video (=2) se ve, pero en niveles de gris nada mas */
	chan[n].channel=1;
	if (ioctl(fdv4l[n], VIDIOCSCHAN, &chan[n]) == -1) { perror("VIDIOCSCHAN"); return(-1);}

      }else if((strcmp(cap[n].name,"Philips 740 webcam")==0) || (strcmp(cap[n].name,"Philips 730 webcam")==0)){
#ifdef PWCX
	/* the driver with compression (pwcx) allows higher frame rates */
	win[n].flags = 30 << 16; 
#else
	/* Philips 740 webcam need to configure framerate at 5 to deliver 320x240 images. For higher speeds (including default ones) the maximum image size is 160x120. This doesn't affect to other v4l devices, as long as they don't use bits 16..22 of flags field */
	/* without pwcx, requiring 30 fps causes small image sizes */
	win[n].flags = 5 << 16;
#endif 
	
      }else win[n].flags =0;
	  
      if (ioctl(fdv4l[n], VIDIOCSWIN, &win[n]) < 0) perror("VIDIOCSWIN");
      if (ioctl(fdv4l[n], VIDIOCGWIN, &win[n]) < 0) {perror("VIDIOCGWIN");return -1;}
      else{

	printf("WINDOW \n x=%d y=%d \n width=%d, height=%d \n chromakey=%d \n flags=%d \n clipcount=%d \n\n", win[n].x, win[n].y, win[n].width, win[n].height, win[n].chromakey, win[n].flags, win[n].clipcount);

      }

      printf("video4linux: capture window = %d x %d \n",win[n].width,win[n].height);
	  
	  
      /* Caracteristicas de la imagen, incluyendo formato de pixels!!. Se solicitan al driver y este nos dice que si puede darnoslo o no */
      if (ioctl(fdv4l[n], VIDIOCGPICT, &vpic[n]) < 0) {perror("VIDIOCGPICT"); return -1;}
	  
      /*
	vpic[n].depth=8;
	vpic[n].palette=VIDEO_PALETTE_YUV420P;
	if(ioctl(fdv4l[n], VIDIOCSPICT, &vpic[n]) < 0) 
	{perror("v4l: unable to find a supported capture format"); return -1;}
      */
	  
      vpic[n].depth=8;
      vpic[n].palette=VIDEO_PALETTE_YUV420P;
      if(ioctl(fdv4l[n], VIDIOCSPICT, &vpic[n])<0) {
	vpic[n].depth=16;
	vpic[n].palette=VIDEO_PALETTE_RGB565;
	if(ioctl(fdv4l[n], VIDIOCSPICT, &vpic[n]) < 0) {
	  vpic[n].depth=24;
	  vpic[n].palette=VIDEO_PALETTE_RGB24;
	  if(ioctl(fdv4l[n], VIDIOCSPICT, &vpic[n]) < 0) {perror("v4l: unable to find a supported capture format"); return -1;}
	}
      }
	  
      printf("VIDEO_PICTURE \n brightness=%d \n hue=%d \n colour=%d \n contrast=%d \n whiteness=%d \n depth=%d \n palette=%d \n\n",vpic[n].brightness,vpic[n].hue,vpic[n].colour,vpic[n].contrast,vpic[n].whiteness,vpic[n].depth,vpic[n].palette);
      printf("video4linux: capture palette %d\n",vpic[n].palette);
	  
      gb_buffers[n].frames=2;
      if (cap[n].type & VID_TYPE_CAPTURE) {
	/* map grab buffer */
	if (-1 == ioctl(fdv4l[n],VIDIOCGMBUF,&gb_buffers[n])) {perror("ioctl VIDIOCGMBUF"); return -1;}
	
	map[n] = mmap(0,gb_buffers[n].size,PROT_READ|PROT_WRITE,MAP_SHARED,fdv4l[n],0);
	if ((char*)-1 != map[n]) 
	  printf("Memory mapped: %d Bytes for %d frames\n",gb_buffers[n].size,gb_buffers[n].frames);
	else { perror("mmap"); return -1;}
      }

      gb[n].frame=0;
      gb[n].width=win[n].width;
      gb[n].height=win[n].height;
      gb[n].format=vpic[n].palette;
	  
      /* init Lookup tables for color conversion */
      for (k = 0; k < 256; k++) {
	ng_yuv_gray[k] = k * LUN_MUL >> 8;
	ng_yuv_red[k]  = (RED_ADD    + k * RED_MUL)    >> 8;
	ng_yuv_blue[k] = (BLUE_ADD   + k * BLUE_MUL)   >> 8;
	ng_yuv_g1[k]   = (GREEN1_ADD + k * GREEN1_MUL) >> 8;
	ng_yuv_g2[k]   = (GREEN2_ADD + k * GREEN2_MUL) >> 8;
      }
      
      for (k = 0; k < CLIP; k++)
	ng_clip[k] = 0;
      for (; k < CLIP + 256; k++)
	ng_clip[k] = k - CLIP;
      for (; k < 2 * CLIP + 256; k++)
	ng_clip[k] = 255;
    }
  }

  /* inicializamos todos los esquemas de imagenes utilizados. */
  if(color_requested[colorA]){
    all[num_schemas].id = (int *) &(color_schema_id[colorA]);
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

    colorA_image=(char *)malloc(win[color_v4l[colorA]].width*win[color_v4l[colorA]].height*3);    
    myexport("colorA","id",&(color_schema_id[colorA]));
    myexport("colorA","colorA",&colorA_image);
    myexport("colorA","clock", &(color_clock[colorA]));
    myexport("colorA","width",&win[color_v4l[colorA]].width);
    myexport("colorA","height",&win[color_v4l[colorA]].height);
    myexport("colorA","resume",(void *)mycolorA_resume);
    myexport("colorA","suspend",(void *)mycolorA_suspend);

    if (color_v4l[colorA]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread0,NULL); 
    else if (color_v4l[colorA]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread1,NULL); 
    else if (color_v4l[colorA]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread2,NULL); 
    else if (color_v4l[colorA]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread3,NULL); 

    num_schemas++;
  }

  if(color_requested[colorB]){
    all[num_schemas].id = (int *) &(color_schema_id[colorB]);
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

    colorB_image=(char *)malloc(win[color_v4l[colorB]].width*win[color_v4l[colorB]].height*3);    
    myexport("colorB","id",&(color_schema_id[colorB]));
    myexport("colorB","colorB",&colorB_image);
    myexport("colorB","clock", &(color_clock[colorB]));
    myexport("colorB","width",&win[color_v4l[colorB]].width);
    myexport("colorB","height",&win[color_v4l[colorB]].height);
    myexport("colorB","resume",(void *)mycolorB_resume);
    myexport("colorB","suspend",(void *)mycolorB_suspend);

    if (color_v4l[colorB]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread0,NULL); 
    else if (color_v4l[colorB]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread1,NULL); 
    else if (color_v4l[colorB]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread2,NULL); 
    else if (color_v4l[colorB]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread3,NULL); 

    num_schemas++;
  }

  if(color_requested[colorC]){
    all[num_schemas].id = (int *) &(color_schema_id[colorC]);
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

    colorC_image=(char *)malloc(win[color_v4l[colorC]].width*win[color_v4l[colorC]].height*3);
    myexport("colorC","id",&(color_schema_id[colorC]));
    myexport("colorC","colorC",&colorC_image);
    myexport("colorC","clock", &(color_clock[colorC]));
    myexport("colorC","width",&win[color_v4l[colorC]].width);
    myexport("colorC","height",&win[color_v4l[colorC]].height);
    myexport("colorC","resume",(void *)mycolorC_resume);
    myexport("colorC","suspend",(void *)mycolorC_suspend);
    
    if (color_v4l[colorC]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread0,NULL); 
    else if (color_v4l[colorC]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread1,NULL); 
    else if (color_v4l[colorC]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread2,NULL); 
    else if (color_v4l[colorC]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread3,NULL); 

    num_schemas++;
  }

  if(color_requested[colorD]){
    all[num_schemas].id = (int *) &(color_schema_id[colorD]);
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

    colorD_image=(char *)malloc(win[color_v4l[colorD]].width*win[color_v4l[colorD]].height*3);
    myexport("colorD","id",&(color_schema_id[colorD]));
    myexport("colorD","colorD",&colorD_image);
    myexport("colorD","clock", &(color_clock[colorD]));
    myexport("colorD","width",&win[color_v4l[colorD]].width);
    myexport("colorD","height",&win[color_v4l[colorD]].height);
    myexport("colorD","resume",(void *)mycolorD_resume);
    myexport("colorD","suspend",(void *)mycolorD_suspend);

    if (color_v4l[colorD]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread0,NULL); 
    else if (color_v4l[colorD]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread1,NULL); 
    else if (color_v4l[colorD]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread2,NULL); 
    else if (color_v4l[colorD]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread3,NULL); 

    num_schemas++;
  }

  if(color_requested[varcolorA]){
    all[num_schemas].id = (int *) &(color_schema_id[varcolorA]);
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

    varcolorA_image=(char *)malloc(win[color_v4l[varcolorA]].width*win[color_v4l[varcolorA]].height*3);    
    myexport("varcolorA","id",&(color_schema_id[varcolorA]));
    myexport("varcolorA","varcolorA",&varcolorA_image);
    myexport("varcolorA","clock", &(color_clock[varcolorA]));
    myexport("varcolorA","width",&win[color_v4l[varcolorA]].width);
    myexport("varcolorA","height",&win[color_v4l[varcolorA]].height);
    myexport("varcolorA","resume",(void *)myvarcolorA_resume);
    myexport("varcolorA","suspend",(void *)myvarcolorA_suspend);

    if (color_v4l[varcolorA]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread0,NULL); 
    else if (color_v4l[varcolorA]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread1,NULL); 
    else if (color_v4l[varcolorA]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread2,NULL); 
    else if (color_v4l[varcolorA]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread3,NULL); 
    
    num_schemas++;
  }

  if(color_requested[varcolorB]){
    all[num_schemas].id = (int *) &(color_schema_id[varcolorB]);
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
    
    varcolorB_image=(char *)malloc(win[color_v4l[varcolorB]].width*win[color_v4l[varcolorB]].height*3);    
    myexport("varcolorB","id",&(color_schema_id[varcolorB]));
    myexport("varcolorB","varcolorB",&varcolorB_image);
    myexport("varcolorB","clock", &(color_clock[varcolorB]));
    myexport("varcolorB","width",&win[color_v4l[varcolorB]].width);
    myexport("varcolorB","height",&win[color_v4l[varcolorB]].height);
    myexport("varcolorB","resume",(void *)myvarcolorB_resume);
    myexport("varcolorB","suspend",(void *)myvarcolorB_suspend);
    
    if (color_v4l[varcolorB]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread0,NULL); 
    else if (color_v4l[varcolorB]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread1,NULL); 
    else if (color_v4l[varcolorB]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread2,NULL); 
    else if (color_v4l[varcolorB]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread3,NULL); 

    num_schemas++;
  }

  if(color_requested[varcolorC]){
    all[num_schemas].id = (int *) &(color_schema_id[varcolorC]);
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

    varcolorC_image=(char *)malloc(win[color_v4l[varcolorC]].width*win[color_v4l[varcolorC]].height*3);    
    myexport("varcolorC","id",&(color_schema_id[varcolorC]));
    myexport("varcolorC","varcolorC",&varcolorC_image);
    myexport("varcolorC","clock", &(color_clock[varcolorC]));
    myexport("varcolorC","width",&win[color_v4l[varcolorC]].width);
    myexport("varcolorC","height",&win[color_v4l[varcolorC]].height);
    myexport("varcolorC","resume",(void *)myvarcolorC_resume);
    myexport("varcolorC","suspend",(void *)myvarcolorC_suspend);

    if (color_v4l[varcolorC]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread0,NULL); 
    else if (color_v4l[varcolorC]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread1,NULL); 
    else if (color_v4l[varcolorC]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread2,NULL); 
    else if (color_v4l[varcolorC]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread3,NULL); 

    num_schemas++;
  }

  if(color_requested[varcolorD]){
    all[num_schemas].id = (int *) &(color_schema_id[varcolorD]);
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

    varcolorD_image=(char *)malloc(win[color_v4l[varcolorD]].width*win[color_v4l[varcolorD]].height*3);    
    myexport("varcolorD","id",&(color_schema_id[varcolorD]));
    myexport("varcolorD","varcolorD",&varcolorD_image);
    myexport("varcolorD","clock", &(color_clock[varcolorD]));
    myexport("varcolorD","width",&win[color_v4l[varcolorD]].width);
    myexport("varcolorD","height",&win[color_v4l[varcolorD]].height);
    myexport("varcolorD","resume",(void *)myvarcolorD_resume);
    myexport("varcolorD","suspend",(void *)myvarcolorD_suspend);

    if (color_v4l[varcolorD]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread0,NULL); 
    else if (color_v4l[varcolorD]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread1,NULL); 
    else if (color_v4l[varcolorD]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread2,NULL); 
    else if (color_v4l[varcolorD]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux_thread3,NULL); 

    num_schemas++;
  }

  return 0;
}


/** Function will end execution of the driver, closing file descriptors and stopping devices.*/
void video4linux_close(){
  int i;

  finish_flag=1;
  sleep(1);
  for(i=0;i<MAXCAM;i++){if(v4l_requested[i]) close(fdv4l[i]);}
  printf("driver video4linux off\n");
}


/** video4linux driver startup function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void video4linux_startup(char *configfile)
{
  int i;

  for(i=0;i<MAXCAM;i++)
    v4l_requested[i]=0; 

  for(i=0;i<MAXDEST;i++)
    {
      color_requested[i]=0; 
      color_active[i]=0;
      color_v4l[i]=0;
    }
  
  /* we call the function to parse the config file */
  if(video4linux_parseconf(configfile)==-1){
    printf("video4linux: cannot initiate driver. configfile parsing error.\n");
    exit(-1);
  }

  /* video4linux driver init */
  if(video4linux_init()!=0){
    printf("video4linux: cannot initiate driver. cameras not ready or not supported.\n");
    exit(-1);
  }

  printf("video4linux driver started up\n");
}
