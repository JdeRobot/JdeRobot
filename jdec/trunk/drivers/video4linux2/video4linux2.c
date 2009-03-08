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
 *  Authors : Jose Maria Cañas <jmplaza@gsyc.escet.urjc.es>
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <jde.h>
#include <interfaces/varcolor.h>

/* SIF image size */
#define SIFNTSC_ROWS 240
#define SIFNTSC_COLUMNS 320

/** Using linux time headers.*/
#define _LINUX_TIME_H 1
/** Using linux device headers.*/
#define _DEVICE_H_ 1
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>

#include <linux/videodev2.h>

/** Constants */
const int RGB24_FORMAT = 861030210;

/** Struct to save the buffers */
struct buffer {
        void *                  start;
        size_t                  length;
};

/** Driver name.*/
char driver_name[256]="video4linux2";
/** Flag to stop the threads when the driver is closed */
int finish_flag=0;
/** Max number of cameras served by this video4linux2 driver.*/
#define MAXCAM 4
/** Buffers used in each camera */
#define MAXBUFFERS 4
/** File descriptors for /dev/video cameras.*/
int fdv4l[MAXCAM];
/** Video capability option.*/
struct v4l2_capability cap[MAXCAM];
/** Video channel option.*/
struct v4l2_input input[MAXCAM];
/** Video window option. Includes its size: .width y .height */
struct v4l2_format format[MAXCAM];
/** Threads really used */
int v4l_requested[MAXCAM];
/** Device names. Example: "/dev/video0".*/
char v4l_filename[MAXCAM][256];
/** Input of devices */
int v4l_input[MAXCAM];
/** Input width & height */
int v4l_width[MAXCAM];
int v4l_height[MAXCAM];
/** Buffers for each device */
/*struct buffer buffers[MAXBUFFERS];*/
struct buffer buffers[MAXCAM][MAXBUFFERS];



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
Varcolor myA,myB,myC,myD;



/** colorA run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorA_run(int father, int *brothers, arbitration fn){
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
	  printf("colorA schema run (video4linux2 driver)\n");
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

/** colorA stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
int mycolorA_stop(){

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
         printf("colorA schema stop (video4linux2 driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[colorA]].mymutex));
         put_state(color_schema_id[colorA],slept);
         pthread_mutex_unlock(&(all[color_schema_id[colorA]].mymutex));
      }
   }
   return 0;
}


/** colorB run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorB_run(int father, int *brothers, arbitration fn){

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
         printf("colorB schema run (video4linux2 driver)\n");
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

/** colorB stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
int mycolorB_stop(){

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
         printf("colorB schema stop (video4linux2 driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[colorB]].mymutex));
         put_state(color_schema_id[colorB],slept);
         pthread_mutex_unlock(&(all[color_schema_id[colorB]].mymutex));
      }
   }
   return 0;
}

/** colorC run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorC_run(int father, int *brothers, arbitration fn){
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
         printf("colorC schema run (video4linux2 driver)\n");
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

/** colorC stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
int mycolorC_stop(){
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
         printf("colorC schema stop (video4linux2 driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[colorC]].mymutex));
         put_state(color_schema_id[colorC],slept);
         pthread_mutex_unlock(&(all[color_schema_id[colorC]].mymutex));
      }
   }
   return 0;
}

/** colorD run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorD_run(int father, int *brothers, arbitration fn){
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
         printf("colorD schema run (video4linux2 driver)\n");
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

/** colorD stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
int mycolorD_stop(){
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
         printf("colorD schema stop (video4linux2 driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[colorD]].mymutex));
         put_state(color_schema_id[colorD],slept);
         pthread_mutex_unlock(&(all[color_schema_id[colorD]].mymutex));
      }
   }
   return 0;
}

/** varcolorA run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorA_run(int father, int *brothers, arbitration fn){
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
	  printf("varcolorA schema run (video4linux2 driver)\n");
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

/** varcolorA stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
int myvarcolorA_stop(){

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
         printf("varcolorA schema stop (video4linux2 driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[varcolorA]].mymutex));
         put_state(color_schema_id[varcolorA],slept);
         pthread_mutex_unlock(&(all[color_schema_id[varcolorA]].mymutex));
      }
   }
   return 0;
}

/** varcolorB run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorB_run(int father, int *brothers, arbitration fn){
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
	  printf("varcolorB schema run (video4linux2 driver)\n");
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

/** varcolorB stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
int myvarcolorB_stop(){

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
         printf("varcolorB schema stop (video4linux2 driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[varcolorB]].mymutex));
         put_state(color_schema_id[varcolorB],slept);
         pthread_mutex_unlock(&(all[color_schema_id[varcolorB]].mymutex));
      }
   }
   return 0;
}


/** varcolorC run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorC_run(int father, int *brothers, arbitration fn){
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
	  printf("varcolorC schema run (video4linux2 driver)\n");
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

/** varcolorC stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
int myvarcolorC_stop(){

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
         printf("varcolorC schema stop (video4linux2 driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[varcolorC]].mymutex));
         put_state(color_schema_id[varcolorC],slept);
         pthread_mutex_unlock(&(all[color_schema_id[varcolorC]].mymutex));
      }
   }
   return 0;
}

/** varcolorD run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorD_run(int father, int *brothers, arbitration fn){
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
	  printf("varcolorD schema run (video4linux2 driver)\n");
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

/** varcolorD stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
int myvarcolorD_stop(){

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
         printf("varcolorD schema stop (video4linux2 driver)\n");
         pthread_mutex_lock(&(all[color_schema_id[varcolorD]].mymutex));
         put_state(color_schema_id[varcolorD],slept);
         pthread_mutex_unlock(&(all[color_schema_id[varcolorD]].mymutex));
      }
   }
   return 0;
}


/* Transforma un pixel YUV 4:4:4 a un pixel RGB de 24 bits 
	Conversion sacada de la wikipedia 
	http://en.wikipedia.org/wiki/YUV
*/
inline void YUV2RGB (unsigned char y, unsigned char u, unsigned char v, unsigned char *r, unsigned char *g, unsigned char *b) {
int r1,g1,b1;

r1 = y + (( (u-128)*1814 ) >> 10);
g1 = y - (( (u-128)*352 + (v-128)*731 ) >> 10);
b1 = y + (( (v-128)*1436 ) >> 10);

if (r1 < 0) r1=0;
if (g1 < 0) g1=0;
if (b1 < 0) b1=0;

if (r1 > 255) r1=255;
if (g1 > 255) g1=255;
if (b1 > 255) b1=255;

*r = (unsigned char) r1;
*g = (unsigned char) g1;
*b = (unsigned char) b1;
}




void *video4linux2_thread0() {
  int cam=0; /* thread0 => cam =0 */
  int color = 0; /* the color (colorA,colorB...) already served by this thread */
  int i,j;
  unsigned char *dest=NULL;
  size_t length = 0;
  unsigned char *buf_temp;
  unsigned char r,g,b,y1,y2,u,v;
  struct v4l2_buffer buf;

  printf("video4linux2: thread %d started\n",cam);

  for(j=0;j<MAXDEST;j++) {
    if ((color_requested[j]) && (color_v4l[j]==cam)) {
	color=j;
    }
  }

  if (color==colorA) dest=(unsigned char*)colorA_image;
  else if (color==colorB) dest=(unsigned char*)colorB_image;
  else if (color==colorC) dest=(unsigned char*)colorC_image;
  else if (color==colorD) dest=(unsigned char*)colorD_image;
  else if (color==varcolorA) dest=(unsigned char*)myA.img;
  else if (color==varcolorB) dest=(unsigned char*)myB.img;
  else if (color==varcolorC) dest=(unsigned char*)myC.img;
  else if (color==varcolorD) dest=(unsigned char*)myD.img;

	/* inicializacion de parametros dependiendo del formato del video */
	if (format[cam].fmt.pix.pixelformat == RGB24_FORMAT) {
		length = v4l_width[cam]*v4l_height[cam]*3;
	
	} else if (format[cam].fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV) {
		length = v4l_width[cam]*v4l_height[cam] * 2;
		/*buf_temp = malloc(length);*/
	
	}



  for(;finish_flag==0;) {
  
      pthread_mutex_lock(&(all[color_schema_id[color]].mymutex));
      
      if (all[color_schema_id[color]].state==slept){
		printf("video4linux2: thread %d goes sleep mode\n",cam);
		pthread_cond_wait(&(all[color_schema_id[color]].condition),&(all[color_schema_id[color]].mymutex));
		printf("video4linux2: thread %d woke up\n",cam);
		pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));

      } else {      
		pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));
	
		speedcounter(color_schema_id[color]);
		color_clock[color]++;
		if (color==varcolorA) myA.clock++;
		else if (color==varcolorB) myB.clock++;
		else if (color==varcolorC) myC.clock++;
		else if (color==varcolorD) myD.clock++;

		/* Desencolar el buffer lleno */
		memset(&buf, 0, sizeof(&buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		if (-1 == ioctl (fdv4l[cam], VIDIOC_DQBUF, &buf)) {
			perror("VIDIOC_DQBUF");
		}

		/* Copiamos el buffer dependiendo del formato */
		if (format[cam].fmt.pix.pixelformat == RGB24_FORMAT) {
			memcpy(dest, buffers[cam][buf.index].start, length);

		} else if (format[cam].fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV) {
			buf_temp=buffers[cam][buf.index].start;

			/* realizamos la conversion de YUYV a RGB24_FORMAT */
			for (i=0; i<v4l_width[cam]*v4l_height[cam]/2; i++) {
				y1 = buf_temp[i*4+0];
				y2 = buf_temp[i*4+2];
				u = buf_temp[i*4+1];
				v = buf_temp[i*4+3];

				YUV2RGB(y1, u, v, &r, &g, &b);
				dest[i*6+0] = r;
				dest[i*6+1] = g;
				dest[i*6+2] = b;

				YUV2RGB(y2, u, v, &r, &g, &b);
				dest[i*6+3] = r;
				dest[i*6+4] = g;
				dest[i*6+5] = b;
			}

		} else {
			printf("formato no soportado\n");

		}

		/* Encolar el buffer ya copiado */
		if (-1 == ioctl (fdv4l[cam], VIDIOC_QBUF, &buf))
			perror("VIDIOC_QBUF");
      }
    }
  /*free (buf_temp);*/
  pthread_exit(0);
}



void *video4linux2_thread1(){
  int cam=1; /* thread0 => cam =0 */
  int color = 0; /* the color (colorA,colorB...) already served by this thread */
  int i,j;
  unsigned char *dest=NULL;
  size_t length = 0;
  unsigned char *buf_temp;
  unsigned char r,g,b,y1,y2,u,v;
  struct v4l2_buffer buf;

  printf("video4linux2: thread %d started\n",cam);

  for(j=0;j<MAXDEST;j++) {
    if ((color_requested[j]) && (color_v4l[j]==cam)) {
	color=j;
    }
  }

  if (color==colorA) dest=(unsigned char*)colorA_image;
  else if (color==colorB) dest=(unsigned char*)colorB_image;
  else if (color==colorC) dest=(unsigned char*)colorC_image;
  else if (color==colorD) dest=(unsigned char*)colorD_image;
  else if (color==varcolorA) dest=(unsigned char*)myA.img;
  else if (color==varcolorB) dest=(unsigned char*)myB.img;
  else if (color==varcolorC) dest=(unsigned char*)myC.img;
  else if (color==varcolorD) dest=(unsigned char*)myD.img;

	/* inicializacion de parametros dependiendo del formato del video */
	if (format[cam].fmt.pix.pixelformat == RGB24_FORMAT) {
		length = v4l_width[cam]*v4l_height[cam]*3;
	
	} else if (format[cam].fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV) {
		length = v4l_width[cam]*v4l_height[cam] * 2;
		/*buf_temp = malloc(length);*/
	
	}



  for(;finish_flag==0;) {
  
      pthread_mutex_lock(&(all[color_schema_id[color]].mymutex));
      
      if (all[color_schema_id[color]].state==slept){
		printf("video4linux2: thread %d goes sleep mode\n",cam);
		pthread_cond_wait(&(all[color_schema_id[color]].condition),&(all[color_schema_id[color]].mymutex));
		printf("video4linux2: thread %d woke up\n",cam);
		pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));

      } else {      
		pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));
	
		speedcounter(color_schema_id[color]);
		color_clock[color]++;
		if (color==varcolorA) myA.clock++;
		else if (color==varcolorB) myB.clock++;
		else if (color==varcolorC) myC.clock++;
		else if (color==varcolorD) myD.clock++;

		/* Desencolar el buffer lleno */
		memset(&buf, 0, sizeof(&buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		if (-1 == ioctl (fdv4l[cam], VIDIOC_DQBUF, &buf)) {
			perror("VIDIOC_DQBUF");
		}

		/* Copiamos el buffer dependiendo del formato */
		if (format[cam].fmt.pix.pixelformat == RGB24_FORMAT) {
			memcpy(dest, buffers[cam][buf.index].start, length);

		} else if (format[cam].fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV) {
			buf_temp=buffers[cam][buf.index].start;

			/* realizamos la conversion de YUYV a RGB24_FORMAT */
			for (i=0; i<v4l_width[cam]*v4l_height[cam]/2; i++) {
				y1 = buf_temp[i*4+0];
				y2 = buf_temp[i*4+2];
				u = buf_temp[i*4+1];
				v = buf_temp[i*4+3];

				YUV2RGB(y1, u, v, &r, &g, &b);
				dest[i*6+0] = r;
				dest[i*6+1] = g;
				dest[i*6+2] = b;

				YUV2RGB(y2, u, v, &r, &g, &b);
				dest[i*6+3] = r;
				dest[i*6+4] = g;
				dest[i*6+5] = b;
			}

		} else {
			printf("formato no soportado\n");

		}

		/* Encolar el buffer ya copiado */
		if (-1 == ioctl (fdv4l[cam], VIDIOC_QBUF, &buf))
			perror("VIDIOC_QBUF");
      }
    }
  /*free (buf_temp);*/
  pthread_exit(0);
}


void *video4linux2_thread2(){
  int cam=2; /* thread0 => cam =0 */
  int color = 0; /* the color (colorA,colorB...) already served by this thread */
  int i,j;
  unsigned char *dest=NULL;
  size_t length = 0;
  unsigned char *buf_temp;
  unsigned char r,g,b,y1,y2,u,v;
  struct v4l2_buffer buf;

  printf("video4linux2: thread %d started\n",cam);

  for(j=0;j<MAXDEST;j++) {
    if ((color_requested[j]) && (color_v4l[j]==cam)) {
	color=j;
    }
  }

  if (color==colorA) dest=(unsigned char*)colorA_image;
  else if (color==colorB) dest=(unsigned char*)colorB_image;
  else if (color==colorC) dest=(unsigned char*)colorC_image;
  else if (color==colorD) dest=(unsigned char*)colorD_image;
  else if (color==varcolorA) dest=(unsigned char*)myA.img;
  else if (color==varcolorB) dest=(unsigned char*)myB.img;
  else if (color==varcolorC) dest=(unsigned char*)myC.img;
  else if (color==varcolorD) dest=(unsigned char*)myD.img;

	/* inicializacion de parametros dependiendo del formato del video */
	if (format[cam].fmt.pix.pixelformat == RGB24_FORMAT) {
		length = v4l_width[cam]*v4l_height[cam]*3;
	
	} else if (format[cam].fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV) {
		length = v4l_width[cam]*v4l_height[cam] * 2;
		/*buf_temp = malloc(length);*/
	
	}



  for(;finish_flag==0;) {
  
      pthread_mutex_lock(&(all[color_schema_id[color]].mymutex));
      
      if (all[color_schema_id[color]].state==slept){
		printf("video4linux2: thread %d goes sleep mode\n",cam);
		pthread_cond_wait(&(all[color_schema_id[color]].condition),&(all[color_schema_id[color]].mymutex));
		printf("video4linux2: thread %d woke up\n",cam);
		pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));

      } else {      
		pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));
	
		speedcounter(color_schema_id[color]);
		color_clock[color]++;
		if (color==varcolorA) myA.clock++;
		else if (color==varcolorB) myB.clock++;
		else if (color==varcolorC) myC.clock++;
		else if (color==varcolorD) myD.clock++;

		/* Desencolar el buffer lleno */
		memset(&buf, 0, sizeof(&buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		if (-1 == ioctl (fdv4l[cam], VIDIOC_DQBUF, &buf)) {
			perror("VIDIOC_DQBUF");
		}

		/* Copiamos el buffer dependiendo del formato */
		if (format[cam].fmt.pix.pixelformat == RGB24_FORMAT) {
			memcpy(dest, buffers[cam][buf.index].start, length);

		} else if (format[cam].fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV) {
			buf_temp=buffers[cam][buf.index].start;

			/* realizamos la conversion de YUYV a RGB24_FORMAT */
			for (i=0; i<v4l_width[cam]*v4l_height[cam]/2; i++) {
				y1 = buf_temp[i*4+0];
				y2 = buf_temp[i*4+2];
				u = buf_temp[i*4+1];
				v = buf_temp[i*4+3];

				YUV2RGB(y1, u, v, &r, &g, &b);
				dest[i*6+0] = r;
				dest[i*6+1] = g;
				dest[i*6+2] = b;

				YUV2RGB(y2, u, v, &r, &g, &b);
				dest[i*6+3] = r;
				dest[i*6+4] = g;
				dest[i*6+5] = b;
			}

		} else {
			printf("formato no soportado\n");

		}

		/* Encolar el buffer ya copiado */
		if (-1 == ioctl (fdv4l[cam], VIDIOC_QBUF, &buf))
			perror("VIDIOC_QBUF");
      }
    }
  /*free (buf_temp);*/
  pthread_exit(0);
}


void *video4linux2_thread3(){
  int cam=3; /* thread0 => cam =0 */
  int color = 0; /* the color (colorA,colorB...) already served by this thread */
  int i,j;
  unsigned char *dest=NULL;
  size_t length = 0;
  unsigned char *buf_temp;
  unsigned char r,g,b,y1,y2,u,v;
  struct v4l2_buffer buf;

  printf("video4linux2: thread %d started\n",cam);

  for(j=0;j<MAXDEST;j++) {
    if ((color_requested[j]) && (color_v4l[j]==cam)) {
	color=j;
    }
  }

  if (color==colorA) dest=(unsigned char*)colorA_image;
  else if (color==colorB) dest=(unsigned char*)colorB_image;
  else if (color==colorC) dest=(unsigned char*)colorC_image;
  else if (color==colorD) dest=(unsigned char*)colorD_image;
  else if (color==varcolorA) dest=(unsigned char*)myA.img;
  else if (color==varcolorB) dest=(unsigned char*)myB.img;
  else if (color==varcolorC) dest=(unsigned char*)myC.img;
  else if (color==varcolorD) dest=(unsigned char*)myD.img;

	/* inicializacion de parametros dependiendo del formato del video */
	if (format[cam].fmt.pix.pixelformat == RGB24_FORMAT) {
		length = v4l_width[cam]*v4l_height[cam]*3;
	
	} else if (format[cam].fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV) {
		length = v4l_width[cam]*v4l_height[cam] * 2;
		/*buf_temp = malloc(length);*/
	
	}



  for(;finish_flag==0;) {
  
      pthread_mutex_lock(&(all[color_schema_id[color]].mymutex));
      
      if (all[color_schema_id[color]].state==slept){
		printf("video4linux2: thread %d goes sleep mode\n",cam);
		pthread_cond_wait(&(all[color_schema_id[color]].condition),&(all[color_schema_id[color]].mymutex));
		printf("video4linux2: thread %d woke up\n",cam);
		pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));

      } else {      
		pthread_mutex_unlock(&(all[color_schema_id[color]].mymutex));
	
		speedcounter(color_schema_id[color]);
		color_clock[color]++;
		if (color==varcolorA) myA.clock++;
		else if (color==varcolorB) myB.clock++;
		else if (color==varcolorC) myC.clock++;
		else if (color==varcolorD) myD.clock++;

		/* Desencolar el buffer lleno */
		memset(&buf, 0, sizeof(&buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		if (-1 == ioctl (fdv4l[cam], VIDIOC_DQBUF, &buf)) {
			perror("VIDIOC_DQBUF");
		}

		/* Copiamos el buffer dependiendo del formato */
		if (format[cam].fmt.pix.pixelformat == RGB24_FORMAT) {
			memcpy(dest, buffers[cam][buf.index].start, length);

		} else if (format[cam].fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV) {
			buf_temp=buffers[cam][buf.index].start;

			/* realizamos la conversion de YUYV a RGB24_FORMAT */
			for (i=0; i<v4l_width[cam]*v4l_height[cam]/2; i++) {
				y1 = buf_temp[i*4+0];
				y2 = buf_temp[i*4+2];
				u = buf_temp[i*4+1];
				v = buf_temp[i*4+3];

				YUV2RGB(y1, u, v, &r, &g, &b);
				dest[i*6+0] = r;
				dest[i*6+1] = g;
				dest[i*6+2] = b;

				YUV2RGB(y2, u, v, &r, &g, &b);
				dest[i*6+3] = r;
				dest[i*6+4] = g;
				dest[i*6+5] = b;
			}

		} else {
			printf("formato no soportado\n");

		}

		/* Encolar el buffer ya copiado */
		if (-1 == ioctl (fdv4l[cam], VIDIOC_QBUF, &buf))
			perror("VIDIOC_QBUF");
      }
    }
  /*free (buf_temp);*/
  pthread_exit(0);
}



/** video4linux2 driver parse configuration file function.
 *  @param configfile path and name to the config file.
 *  @return 0 if parsing was successful or -1 if something went wrong.*/
int video4linux2_parseconf(char *configfile){

  int end_parse=0; int end_section=0; int driver_config_parsed=0;
  FILE *myfile;
  const int limit = 256;
  int numcams = 0;

  if ((myfile=fopen(configfile,"r"))==NULL){
    printf("video4linux2: cannot find config file\n");
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
	printf ("video4linux2: line too long in config file!\n"); 
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
	      
	      char buffer_file2[256],word3[256],word4[256],word5[256],word6[256],word7[256],word8[256];
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
		    printf("video4linux2: error in config file.\n'end_section' keyword required before starting new driver section.\n");
		    end_section=1; end_parse=1;

		  }else if(strcmp(word3,"provides")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s %s %s %s %s",word3,word4,word5,word6,word7,word8)>2){
		      v4l_requested[numcams]=1;
		      strcpy(v4l_filename[numcams],word5);
		      v4l_input[numcams]=atoi(word6);
		      if (strcmp(word4,"colorA")==0) {
			  v4l_width[numcams]=SIFNTSC_COLUMNS;
			  v4l_height[numcams]=SIFNTSC_ROWS;
			  color_requested[colorA]=1;
			  color_v4l[colorA]=numcams;
			}
		      else if (strcmp(word4,"colorB")==0)
			{
			  v4l_width[numcams]=SIFNTSC_COLUMNS;
			  v4l_height[numcams]=SIFNTSC_ROWS;
			  color_requested[colorB]=1; 
			  color_v4l[colorB]=numcams;
			}
		      else if (strcmp(word4,"colorC")==0)
			{
			  v4l_width[numcams]=SIFNTSC_COLUMNS;
			  v4l_height[numcams]=SIFNTSC_ROWS;
			  color_requested[colorC]=1; 
			  color_v4l[colorC]=numcams;
			}
		      else if (strcmp(word4,"colorD")==0)
			{
			  v4l_width[numcams]=SIFNTSC_COLUMNS;
			  v4l_height[numcams]=SIFNTSC_ROWS;
			  color_requested[colorD]=1; 
			  color_v4l[colorD]=numcams;
			}
		      else if (strcmp(word4,"varcolorA")==0)
			{
			  v4l_width[numcams]=atoi(word7);
			  v4l_height[numcams]=atoi(word8);
			  color_requested[varcolorA]=1; 
			  color_v4l[varcolorA]=numcams;
			}
		      else if (strcmp(word4,"varcolorB")==0)
			{
			  v4l_width[numcams]=atoi(word7);
			  v4l_height[numcams]=atoi(word8);
			  color_requested[varcolorB]=1; 
			  color_v4l[varcolorB]=numcams;
			}
		      else if (strcmp(word4,"varcolorC")==0)
			{
			  v4l_width[numcams]=atoi(word7);
			  v4l_height[numcams]=atoi(word8);
			  color_requested[varcolorC]=1; 
			  color_v4l[varcolorC]=numcams;
			}
		      else if (strcmp(word4,"varcolorD")==0)
			{
			  v4l_width[numcams]=atoi(word7);
			  v4l_height[numcams]=atoi(word8);
			  color_requested[varcolorD]=1; 
			  color_v4l[varcolorD]=numcams;
			}
		      /*printf(" Requested size: %d x %d \n", v4l_width[numcams], v4l_height[numcams]);*/
		      numcams++;
		    }else{
		      printf("video4linux2: provides line incorrect\n");
		    }
		  }else printf("video4linux2: i don't know what to do with '%s'\n",buffer_file2);
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
	printf("video4linux2: warning! no color provided.\n");
      return 0;
    }
  else return -1;
}

/** video4linux2 driver init function. It will start all video4linux2 required devices and setting them the default configuration.
 *  @return 0 if initialitation was successful or -1 if something went wrong.*/
int video4linux2_deviceinit(){

  int i,k,n,n_buffers;
  struct v4l2_input in;
  struct v4l2_fmtdesc fmtdesc;
  struct v4l2_standard std;
  struct v4l2_requestbuffers req;
  struct v4l2_buffer buf;
  struct v4l2_cropcap cropcap;
  struct v4l2_crop crop;

  v4l2_std_id std_id;
  enum v4l2_buf_type type;

   
  /* recorremos todos los posibles dispositivos */
  for(n=0;n<MAXCAM;n++){

    /* inicializamos la apertura de todos los dispositivos */
	if(v4l_requested[n]){

		/* Apertura del dispositivo de videoforlinux */
		fdv4l[n] = open(v4l_filename[n], O_RDWR);
		if (fdv4l[n] < 0) {
			perror(v4l_filename[n]); 
			return -1;
		}
	
		/* Informacion general del dispositivo */
		if (-1 == ioctl (fdv4l[n], VIDIOC_QUERYCAP, &cap[n])) {
			perror("VIDIOC_QUERYCAP");
			return -1;
		}
		printf("CAPABILITY: \n Device = %s \n Driver = %s \n", cap[n].card, cap[n].driver);

		/* Comprobamos que el dispositivo soporta video_capture */
		if (!(cap[n].capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
			perror("v4l_filename[n]");
			return -1;
		}

		/* Comprobamos que el dispositivo soporta streaming */
		if (!(cap[n].capabilities & V4L2_CAP_STREAMING)) {
			perror("v4l_filename[n]");
			return -1;
		}


		/* CROPCAP information */
		memset(&cropcap, 0, sizeof(&cropcap));
		cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == ioctl (fdv4l[n], VIDIOC_CROPCAP, &cropcap)) {
			perror("VIDIOC_CROPCAP");
			return -1;
		}

		memset(&crop, 0, sizeof(&crop));
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		crop.c = cropcap.defrect; /* reset to default */
		if (-1 == ioctl (fdv4l[n], VIDIOC_S_CROP, &crop)) {
			if (errno == EINVAL) 
				printf("Cropping is not supported\n");
			else {
				perror("VIDIOC_S_CROP");
				return -1;
			}
		}



		/* Información de los canales (inputs) del dispositivo */
		printf("INPUTS: \n");
		for (i=0; i<32; i++) {
			memset(&in, 0, sizeof(&in));
			in.index=i;
			if (-1 == ioctl (fdv4l[n], VIDIOC_ENUMINPUT, &in)) {
				if (EINVAL == errno) {
					break;
				} else {
					perror("VIDIOC_ENUMINPUT:");
				}
			}
			if (i>0) printf(" |");
			printf(" %d = %s", in.index, in.name);
		}
		printf("\n");
		/* Seleccionamos un input */
		if (-1 == ioctl (fdv4l[n], VIDIOC_S_INPUT, &v4l_input[n])) {
			perror("VIDIOC_S_INPUT");
			return -1;
		}
		printf("Selected input %d\n", v4l_input[n]);



		/* Intentamos seleccionar el formato, intentamos que nos salga directamente en RGB de 24bits */
		format[n].type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == ioctl (fdv4l[n], VIDIOC_G_FMT, &format[n])) {
			perror("VIDIOC_G_FMT");
			return -1;
		}
		format[n].fmt.pix.width = v4l_width[n];
		format[n].fmt.pix.height = v4l_height[n];
		format[n].fmt.pix.pixelformat = RGB24_FORMAT;
		if (-1 == ioctl (fdv4l[n], VIDIOC_TRY_FMT, &format[n])) {
			/* Si NO sólo cambiamos el tamaño de la imagen */
			format[n].type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			if (-1 == ioctl (fdv4l[n], VIDIOC_G_FMT, &format[n])) { 
				perror("VIDIOC_G_FMT");
				return -1;
			}
			format[n].fmt.pix.width = v4l_width[n];
			format[n].fmt.pix.height = v4l_height[n];
			format[n].fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
			if (-1 == ioctl (fdv4l[n], VIDIOC_S_FMT, &format[n])) {
				perror("VIDIOC_S_FMT");
				return -1;
			}
		} else {
			/* Si funciona intentamos que nos salga directamente en RGB de 24bits */
			format[n].type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			if (-1 == ioctl (fdv4l[n], VIDIOC_G_FMT, &format[n])) {
				perror("VIDIOC_G_FMT");
				return -1;
			}
			format[n].fmt.pix.width = v4l_width[n];
			format[n].fmt.pix.height = v4l_height[n];
			format[n].fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
			if (-1 == ioctl (fdv4l[n], VIDIOC_S_FMT, &format[n])) {
				perror("VIDIOC_S_FMT");
				return -1;
			}
		}
		
		/* Escribimos en pantalla el formato actual */
		printf("FORMAT:\n");
		for (k=0; k<32; k++) {
			memset(&fmtdesc, 0, sizeof(&fmtdesc));
			fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			fmtdesc.index=k;
			if (-1 == ioctl (fdv4l[n], VIDIOC_ENUM_FMT, &fmtdesc)) {
				perror(" unknown format\n");
				break;
			}
			if (fmtdesc.pixelformat == format[n].fmt.pix.pixelformat) {
				printf(" %dx%d - %s - %X\n", format[n].fmt.pix.width, format[n].fmt.pix.height, fmtdesc.description, fmtdesc.pixelformat);
				printf(" colorspace %d\n", format[n].fmt.pix.colorspace);
				break;
			}
		}


		/* Comprobamos el estandar usado */
		if (-1 == ioctl (fdv4l[n], VIDIOC_G_STD, &std_id)) {
			perror("VIDIOC_G_STD");
		}
	
		/* Escribimos en pantalla el estandar actual */
		printf("STANDARD:\n");
		for (i=0; i<32; i++) {
			memset(&std, 0, sizeof(&std));
			std.index=i;
	
			if (-1 == ioctl (fdv4l[n], VIDIOC_ENUMSTD, &std)) {
				printf(" unknown standard\n");
				break;
			}
			if (std.id == std_id) {
				printf(" %s\n", std.name);
				break;
			}
		}


		/* Iniciamos mmap */
		memset(&req, 0, sizeof(&req));
		req.count = MAXBUFFERS;
		req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		req.memory = V4L2_MEMORY_MMAP;
		if (-1 == ioctl (fdv4l[n], VIDIOC_REQBUFS, &req)) {
			perror("VIDIOC_REQBUFS");
			return -1;
		}
		/* Hacemos una comprobacion de los bufferes disponibles y apuntamos sus parametros */
		for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
			memset(&buf, 0, sizeof(&buf));
	
			buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory      = V4L2_MEMORY_MMAP;
			buf.index       = n_buffers;
	
			if (-1 == ioctl (fdv4l[n], VIDIOC_QUERYBUF, &buf)) {
				perror("VIDIOC_QUERYBUF");
				return -1;
			}

			buffers[n][n_buffers].length = buf.length;
			buffers[n][n_buffers].start =
				mmap (NULL /* start anywhere */,
				buf.length,
				PROT_READ | PROT_WRITE /* required */,
				MAP_SHARED /* recommended */,
				fdv4l[n], buf.m.offset);
	
			if (MAP_FAILED == buffers[n][n_buffers].start) {
				perror("mmap");
				return -1;
			}
		}
		/* Inicializamos los bufferes */
		for (i = 0; i < req.count; ++i) {
			memset(&buf, 0, sizeof(&buf));

			buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory      = V4L2_MEMORY_MMAP;
			buf.index       = i;

			if (-1 == ioctl (fdv4l[n], VIDIOC_QBUF, &buf)) {
				perror("VIDIOC_QBUF");
				return -1;
			}
		}
		/* Ponemos en marcha el streaming */
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == ioctl (fdv4l[n], VIDIOC_STREAMON, &type)) {
			perror("VIDIOC_STREAMON");
			return -1;
		}
	}
}

  /* inicializamos todos los esquemas de imagenes utilizados. */
  if(color_requested[colorA]){
    all[num_schemas].id = (int *) &(color_schema_id[colorA]);
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

    colorA_image=(char *)malloc(v4l_width[color_v4l[colorA]]*v4l_height[color_v4l[colorA]]*3);    
    myexport("colorA","id",&(color_schema_id[colorA]));
    myexport("colorA","colorA",&colorA_image);
    myexport("colorA","clock", &(color_clock[colorA]));
    myexport("colorA","width",&v4l_width[color_v4l[colorA]]/*&prueba1*/);
    myexport("colorA","height",&v4l_height[color_v4l[colorA]]/*&prueba2*/);
    myexport("colorA","run",(void *)mycolorA_run);
    myexport("colorA","stop",(void *)mycolorA_stop);

    if (color_v4l[colorA]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread0,NULL); 
    else if (color_v4l[colorA]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread1,NULL); 
    else if (color_v4l[colorA]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread2,NULL); 
    else if (color_v4l[colorA]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread3,NULL); 

    num_schemas++;
  }

  if(color_requested[colorB]){
    all[num_schemas].id = (int *) &(color_schema_id[colorB]);
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

    colorB_image=(char *)malloc(v4l_width[color_v4l[colorB]]*v4l_height[color_v4l[colorB]]*3);    
    myexport("colorB","id",&(color_schema_id[colorB]));
    myexport("colorB","colorB",&colorB_image);
    myexport("colorB","clock", &(color_clock[colorB]));
    myexport("colorB","width",&v4l_width[color_v4l[colorB]]);
    myexport("colorB","height",&v4l_height[color_v4l[colorB]]);
    myexport("colorB","run",(void *)mycolorB_run);
    myexport("colorB","stop",(void *)mycolorB_stop);

    if (color_v4l[colorB]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread0,NULL);
    else if (color_v4l[colorB]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread1,NULL);
    else if (color_v4l[colorB]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread2,NULL);
    else if (color_v4l[colorB]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread3,NULL);

    num_schemas++;
  }

  if(color_requested[colorC]){
    all[num_schemas].id = (int *) &(color_schema_id[colorC]);
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

    colorC_image=(char *)malloc(v4l_width[color_v4l[colorC]]*v4l_height[color_v4l[colorC]]*3);
    myexport("colorC","id",&(color_schema_id[colorC]));
    myexport("colorC","colorC",&colorC_image);
    myexport("colorC","clock", &(color_clock[colorC]));
    myexport("colorC","width",&v4l_width[color_v4l[colorC]]);
    myexport("colorC","height",&v4l_height[color_v4l[colorC]]);
    myexport("colorC","run",(void *)mycolorC_run);
    myexport("colorC","stop",(void *)mycolorC_stop);
    
    if (color_v4l[colorC]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread0,NULL); 
    else if (color_v4l[colorC]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread1,NULL); 
    else if (color_v4l[colorC]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread2,NULL); 
    else if (color_v4l[colorC]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread3,NULL); 

    num_schemas++;
  }

  if(color_requested[colorD]){
    all[num_schemas].id = (int *) &(color_schema_id[colorD]);
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

    colorD_image=(char *)malloc(v4l_width[color_v4l[colorD]]*v4l_height[color_v4l[colorD]]*3);
    myexport("colorD","id",&(color_schema_id[colorD]));
    myexport("colorD","colorD",&colorD_image);
    myexport("colorD","clock", &(color_clock[colorD]));
    myexport("colorD","width",&v4l_width[color_v4l[colorD]]);
    myexport("colorD","height",&v4l_height[color_v4l[colorD]]);
    myexport("colorD","run",(void *)mycolorD_run);
    myexport("colorD","stop",(void *)mycolorD_stop);

    if (color_v4l[colorD]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread0,NULL); 
    else if (color_v4l[colorD]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread1,NULL); 
    else if (color_v4l[colorD]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread2,NULL); 
    else if (color_v4l[colorD]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread3,NULL); 

    num_schemas++;
  }

  if(color_requested[varcolorA]){
    all[num_schemas].id = (int *) &(color_schema_id[varcolorA]);
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

    myA.img=(char *)malloc(v4l_width[color_v4l[varcolorA]]*v4l_height[color_v4l[varcolorA]]*3);    
    myA.width=v4l_width[color_v4l[varcolorA]];
    myA.height=v4l_height[color_v4l[varcolorA]];
    myA.clock=color_clock[varcolorA];
    myexport("varcolorA","varcolorA",&myA);
    myexport("varcolorA","id",&(color_schema_id[varcolorA]));
    myexport("varcolorA","run",(void *)myvarcolorA_run);
    myexport("varcolorA","stop",(void *)myvarcolorA_stop);

    if (color_v4l[varcolorA]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread0,NULL); 
    else if (color_v4l[varcolorA]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread1,NULL); 
    else if (color_v4l[varcolorA]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread2,NULL); 
    else if (color_v4l[varcolorA]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread3,NULL); 
    
    num_schemas++;
  }

  if(color_requested[varcolorB]){
    all[num_schemas].id = (int *) &(color_schema_id[varcolorB]);
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
    
    myB.img=(char *)malloc(v4l_width[color_v4l[varcolorB]]*v4l_height[color_v4l[varcolorB]]*3);    
    myB.width=v4l_width[color_v4l[varcolorB]];
    myB.height=v4l_height[color_v4l[varcolorB]];
    myB.clock=color_clock[varcolorB];
    myexport("varcolorB","varcolorB",&myB);
    myexport("varcolorB","id",&(color_schema_id[varcolorB]));
    myexport("varcolorB","run",(void *)myvarcolorB_run);
    myexport("varcolorB","stop",(void *)myvarcolorB_stop);
    
    if (color_v4l[varcolorB]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread0,NULL); 
    else if (color_v4l[varcolorB]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread1,NULL); 
    else if (color_v4l[varcolorB]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread2,NULL); 
    else if (color_v4l[varcolorB]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread3,NULL); 

    num_schemas++;
  }

  if(color_requested[varcolorC]){
    all[num_schemas].id = (int *) &(color_schema_id[varcolorC]);
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

    myC.img=(char *)malloc(v4l_width[color_v4l[varcolorC]]*v4l_height[color_v4l[varcolorC]]*3);    
    myC.width=v4l_width[color_v4l[varcolorC]];
    myC.height=v4l_height[color_v4l[varcolorC]];
    myC.clock=color_clock[varcolorC];
    myexport("varcolorC","varcolorC",&myC);
    myexport("varcolorC","id",&(color_schema_id[varcolorC]));
    myexport("varcolorC","run",(void *)myvarcolorC_run);
    myexport("varcolorC","stop",(void *)myvarcolorC_stop);

    if (color_v4l[varcolorC]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread0,NULL); 
    else if (color_v4l[varcolorC]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread1,NULL); 
    else if (color_v4l[varcolorC]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread2,NULL); 
    else if (color_v4l[varcolorC]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread3,NULL); 

    num_schemas++;
  }

  if(color_requested[varcolorD]){
    all[num_schemas].id = (int *) &(color_schema_id[varcolorD]);
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

    myD.img=(char *)malloc(v4l_width[color_v4l[varcolorD]]*v4l_height[color_v4l[varcolorD]]*3);    
    myD.width=v4l_width[color_v4l[varcolorD]];
    myD.height=v4l_height[color_v4l[varcolorD]];
    myD.clock=color_clock[varcolorD];
    myexport("varcolorD","varcolorD",&myD);
    myexport("varcolorD","id",&(color_schema_id[varcolorD]));
    myexport("varcolorD","run",(void *)myvarcolorD_run);
    myexport("varcolorD","stop",(void *)myvarcolorD_stop);

    if (color_v4l[varcolorD]==0)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread0,NULL); 
    else if (color_v4l[varcolorD]==1)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread1,NULL); 
    else if (color_v4l[varcolorD]==2)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread2,NULL); 
    else if (color_v4l[varcolorD]==3)
      pthread_create(&(all[num_schemas].mythread),NULL,video4linux2_thread3,NULL); 

    num_schemas++;
  }

  return 0;
}


/** Function will end execution of the driver, closing file descriptors and stopping devices.*/
void video4linux2_terminate(){
  int i;
  enum v4l2_buf_type type;

  finish_flag=1;
  sleep(1);
  for (i=0;i<MAXCAM;i++) {
	if(v4l_requested[i]) close(fdv4l[i]);

	/* Paramos el streaming */
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == ioctl (fdv4l[i], VIDIOC_STREAMOFF, &type)) {
		perror("VIDIOC_STREAMOFF");
		return -1;
	}
  }
  printf("driver video4linux2 off\n");
}


/** video4linux2 driver startup function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void video4linux2_init(char *configfile)
{
  int i;

  for(i=0;i<MAXCAM;i++)
    v4l_requested[i]=0; 

  for(i=0;i<MAXDEST;i++)
    {
      color_requested[i]=0; 
      color_active[i]=0;
      color_v4l[i]=0;
      color_clock[i]=0;
    }
  
  /* we call the function to parse the config file */
  if(video4linux2_parseconf(configfile)==-1){
    printf("video4linux2: cannot initiate driver. configfile parsing error.\n");
    exit(-1);
  }

  /* video4linux2 driver init */
  if(video4linux2_deviceinit()!=0){
    printf("video4linux2: cannot initiate driver. cameras not ready or not supported.\n");
    exit(-1);
  }

  printf("video4linux2 driver started up\n");
}
