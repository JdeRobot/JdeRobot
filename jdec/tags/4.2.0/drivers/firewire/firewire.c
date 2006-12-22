/*
 *  Copyright (C) 2006 Antonio Pineda Cabello
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
 *  Authors : David Lobato <dlobato@gsyc.escet.urjc.es>, Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es>
 */

/************************************************
 * jdec firewire driver                         *
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <libdc1394/dc1394_control.h>
#include "jde.h"
#define MAXCAM 4

/* uncomment the following to drop frames to prevent delays */
#define DROP_FRAMES 1
#define MAX_PORTS 3
#define NUM_BUFFERS 8

/* color conversion functions from Bart Nabbe.*/
/* corrected by Damien: bad coeficients in YUV2RGB*/
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
  
/* AUTOMATIC FEATURES FOR FIREWIRE CAMERAS
   0 means deactivated option */
typedef struct{
  int AUTO_EXPOSURE_CONFIG;
  int AUTO_WHITE_BALANCE_CONFIG;
  int AUTO_IRIS_CONFIG;
  int AUTO_FOCUS_CONFIG;
  int AUTO_ZOOM_CONFIG;
}Firewire_features;

/* declarations for libdc1394 */
int numPorts = MAX_PORTS;
raw1394handle_t handles[MAX_PORTS];
int numCameras=0;
dc1394_cameracapture cameras[MAXCAM];
nodeid_t *camera_nodes;
dc1394_feature_set features;
unsigned long int lastimage;

/* declarations for video1394 */
char *camfile[]={"/dev/video1394/0","/dev/video1394/1","/dev/video1394/2","/dev/video1394/3"};
int firewire_fps;
int firewire_res;

pthread_t firewire_th;
int state;
pthread_mutex_t mymutex;
pthread_cond_t condition;

int firewire_thread_created=0;
int firewire_cleaned_up=0;
int firewire_setup=0;
int display_fps=0;
int firewire_close_command=0;

/* firewire driver API options */
char driver_name[256]="firewire";
int serve_color[MAXCAM];
int number_color[MAXCAM];
int color_active[MAXCAM];
Firewire_features fwCamFeatures[MAXCAM];


/* FIREWIRE DRIVER FUNCTIONS */

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

void firewire_display_fps(){
  display_fps=1;
}

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

void firewire_close(){

  firewire_close_command=1;
  firewire_clean_up();
  printf("driver firewire off\n");
}

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

int colorA_resume(){
  if((serve_color[0]==1)&&(color_active[0]==0)){
    color_active[0]=1;
    printf("firewire: colorA resume\n");

    if((color_active[1]==0)&&(color_active[2]==0)&&(color_active[3]==0)){

      /* firewire thread goes winner */
      pthread_mutex_lock(&mymutex);
      state=winner;
      pthread_cond_signal(&condition);
      pthread_mutex_unlock(&mymutex);
    }
  }
  return 0;
}

int colorA_suspend(){

  if((serve_color[0]==1)&&(color_active[0]==1)){
    color_active[0]=0;
    printf("firewire: colorA suspend\n");
    if((color_active[1]==0)&&(color_active[2]==0)&&(color_active[3]==0)){    

      /* firewire thread goes sleep */
      pthread_mutex_lock(&mymutex);
      state=slept;
      pthread_mutex_unlock(&mymutex);
    }
  }
  return 0;
}

int colorB_resume(){
  if((serve_color[1]==1)&&(color_active[1]==0)){
    color_active[1]=1;
    printf("firewire: colorB resume\n");
    if((color_active[0]==0)&&(color_active[2]==0)&&(color_active[3]==0)){
      
      /* firewire thread goes winner */
      pthread_mutex_lock(&mymutex);
      state=winner;
      pthread_cond_signal(&condition);
      pthread_mutex_unlock(&mymutex);
    }
  }
  return 0;
}

int colorB_suspend(){

  if((serve_color[1]==1)&&(color_active[1]==1)){
    color_active[1]=0;
    printf("firewire: colorB suspend\n");
    if((color_active[0]==0)&&(color_active[2]==0)&&(color_active[3]==0)){    
   
      /* firewire thread goes sleep */
      pthread_mutex_lock(&mymutex);
      state=slept;
      pthread_mutex_unlock(&mymutex);
    }
  }
  return 0;
}

int colorC_resume(){
  if((serve_color[2]==1)&&(color_active[2]==0)){
    color_active[2]=1;
    printf("firewire: colorC resume\n");
    if((color_active[1]==0)&&(color_active[0]==0)&&(color_active[3]==0)){
      
      /* firewire thread goes winner */
      pthread_mutex_lock(&mymutex);
      state=winner;
      pthread_cond_signal(&condition);
      pthread_mutex_unlock(&mymutex);
    }
  }
  return 0;
}

int colorC_suspend(){

  if((serve_color[2]==1)&&(color_active[2]==1)){
    color_active[2]=0;
    printf("firewire: colorC suspend\n");
    if((color_active[0]==0)&&(color_active[1]==0)&&(color_active[3]==0)){    
   
      /* firewire thread goes sleep */
      pthread_mutex_lock(&mymutex);
      state=slept;
      pthread_mutex_unlock(&mymutex);
    }
  }
  return 0;
}

int colorD_resume(){
  if((serve_color[3]==1)&&(color_active[3]==0)){
    color_active[3]=1;
    printf("firewire: colorD resume\n");
    if((color_active[1]==0)&&(color_active[2]==0)&&(color_active[0]==0)){
      
      /* firewire thread goes winner */
      pthread_mutex_lock(&mymutex);
      state=winner;
      pthread_cond_signal(&condition);
      pthread_mutex_unlock(&mymutex);
    }
  }
  return 0;
}

int colorD_suspend(){

  if((serve_color[3]==1)&&(color_active[3]==1)){
    color_active[3]=0;
    printf("firewire: colorD suspend\n");
    if((color_active[0]==0)&&(color_active[1]==0)&&(color_active[2]==0)){    
   
      /* firewire thread goes sleep */
      pthread_mutex_lock(&mymutex);
      state=slept;
      pthread_mutex_unlock(&mymutex);
    }
  }
  return 0;
}

void *firewire_thread(void *not_used)
{
  static float fps=0, fpsold=0;
  struct timeval t;
  static unsigned long int then;
  unsigned long int now;

  printf("firewire: firewire thread started up\n");

  do{

    pthread_mutex_lock(&mymutex);

    if (state==slept){
      printf("firewire: firewire thread in sleep mode\n");
      pthread_cond_wait(&condition,&mymutex);
      printf("firewire: firewire thread woke up\n");
      pthread_mutex_unlock(&mymutex);
      
    }else{
      
      pthread_mutex_unlock(&mymutex);
      
      /* image capture for all cameras - only capturing when needed */
      if(color_active[0]){
	if(dc1394_dma_single_capture(&cameras[number_color[0]])==DC1394_FAILURE) perror("firewire_thread");}
      if((color_active[1])&&(number_color[1]!=number_color[0])){
	if(dc1394_dma_single_capture(&cameras[number_color[1]])==DC1394_FAILURE) perror("firewire_thread");}
      if((color_active[2])&&(number_color[2]!=number_color[0])&&(number_color[2]!=number_color[1])){
	if(dc1394_dma_single_capture(&cameras[number_color[2]])==DC1394_FAILURE) perror("firewire_thread");}
      if((color_active[3])&&(number_color[3]!=number_color[0])&&(number_color[3]!=number_color[1])&&(number_color[3]!=number_color[0])){
	if(dc1394_dma_single_capture(&cameras[number_color[3]])==DC1394_FAILURE) perror("firewire_thread");}

      /* image copy between buffers */
      if(color_active[0]) uyvy2rgb((unsigned char*)cameras[number_color[0]].capture_buffer,(unsigned char*)colorA,SIFNTSC_COLUMNS*SIFNTSC_ROWS);
      if(color_active[1]) uyvy2rgb((unsigned char*)cameras[number_color[1]].capture_buffer,(unsigned char*)colorB,SIFNTSC_COLUMNS*SIFNTSC_ROWS);
      if(color_active[2]) uyvy2rgb((unsigned char*)cameras[number_color[2]].capture_buffer,(unsigned char*)colorC,SIFNTSC_COLUMNS*SIFNTSC_ROWS);
      if(color_active[3]) uyvy2rgb((unsigned char*)cameras[number_color[3]].capture_buffer,(unsigned char*)colorD,SIFNTSC_COLUMNS*SIFNTSC_ROWS);

      /* dma_done_with_buffer for all active cameras */
      if(color_active[0]){
	if(dc1394_dma_done_with_buffer(&cameras[number_color[0]])==DC1394_FAILURE) perror("firewire_thread");}
      if((color_active[1])&&(number_color[1]!=number_color[0])){
	if(dc1394_dma_done_with_buffer(&cameras[number_color[1]])==DC1394_FAILURE) perror("firewire_thread");}
      if((color_active[2])&&(number_color[2]!=number_color[0])&&(number_color[2]!=number_color[1])){
	if(dc1394_dma_done_with_buffer(&cameras[number_color[2]])==DC1394_FAILURE) perror("firewire_thread");}
      if((color_active[3])&&(number_color[3]!=number_color[0])&&(number_color[3]!=number_color[1])&&(number_color[3]!=number_color[0])){
	if(dc1394_dma_done_with_buffer(&cameras[number_color[3]])==DC1394_FAILURE) perror("firewire_thread");}

      /* to get firewire fps */
      gettimeofday(&t,NULL);
      now=t.tv_sec*1000000+t.tv_usec;
      lastimage = now;
      if ((now-then)>2000000) /* periodo de integracion, de cuenta de imagenes: 2 segundos, mas o menos */
	{fps=fps*1000000/(now-then); 
	  if ((fps>fpsold+1)||(fps<fpsold-1)) 
	    {if(display_fps==1) printf("firewire: %.1f fps\n",fps);
	      fpsold=fps;}
	  fps=0.;
	  then=now;
	}
      else fps++;
    }
  }while(firewire_close_command==0);

  pthread_exit(0);
}

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
	      
	      char buffer_file2[256],word3[256],word4[256],word5[256],word6[256];
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
		    printf("firewire: error in config file.\n'end_section' keyword required before starting new driver section.\n");
		    end_section=1; end_parse=1;

		  }else if(strcmp(word3,"provides")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s %s %s",word3,word4,word5,word6)>2){

		      if(strcmp(word4,"colorA")==0){
			serve_color[0]=1;
			number_color[0]=atoi(word5);
			if(strcmp(word6,"autofocus_on")!=0) fwCamFeatures[0].AUTO_FOCUS_CONFIG=0;
			
		      }else if(strcmp(word4,"colorB")==0){
			serve_color[1]=1;
			number_color[1]=atoi(word5);
			if(strcmp(word6,"autofocus_on")!=0) fwCamFeatures[1].AUTO_FOCUS_CONFIG=0;

		      }else if(strcmp(word4,"colorC")==0){
			serve_color[2]=1;
			number_color[2]=atoi(word5);
			if(strcmp(word6,"autofocus_on")!=0) fwCamFeatures[2].AUTO_FOCUS_CONFIG=0;

		      }else if(strcmp(word4,"colorD")==0){
			serve_color[3]=1;
			number_color[3]=atoi(word5);
			if(strcmp(word6,"autofocus_on")!=0) fwCamFeatures[3].AUTO_FOCUS_CONFIG=0;
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

void firewire_init(){

  int p,i;
  raw1394handle_t raw_handle;
  struct raw1394_portinfo ports[MAX_PORTS];
  unsigned int channel;
  unsigned int speed;
  
  firewire_res = MODE_320x240_YUV422;
  firewire_fps = FRAMERATE_30;
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
	  if(numCameras<3){
	    if(number_color[0]==numCameras){
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_EXPOSURE,fwCamFeatures[0].AUTO_EXPOSURE_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_WHITE_BALANCE,fwCamFeatures[0].AUTO_WHITE_BALANCE_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_IRIS,fwCamFeatures[0].AUTO_IRIS_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_FOCUS,fwCamFeatures[0].AUTO_FOCUS_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_ZOOM,fwCamFeatures[0].AUTO_ZOOM_CONFIG);

	    }else if(number_color[1]==numCameras){
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_EXPOSURE,fwCamFeatures[1].AUTO_EXPOSURE_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_WHITE_BALANCE,fwCamFeatures[1].AUTO_WHITE_BALANCE_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_IRIS,fwCamFeatures[1].AUTO_IRIS_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_FOCUS,fwCamFeatures[1].AUTO_FOCUS_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_ZOOM,fwCamFeatures[1].AUTO_ZOOM_CONFIG);

	    }else if(number_color[2]==numCameras){
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_EXPOSURE,fwCamFeatures[2].AUTO_EXPOSURE_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_WHITE_BALANCE,fwCamFeatures[2].AUTO_WHITE_BALANCE_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_IRIS,fwCamFeatures[2].AUTO_IRIS_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_FOCUS,fwCamFeatures[2].AUTO_FOCUS_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_ZOOM,fwCamFeatures[2].AUTO_ZOOM_CONFIG);

	    }else if(number_color[3]==numCameras){
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_EXPOSURE,fwCamFeatures[3].AUTO_EXPOSURE_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_WHITE_BALANCE,fwCamFeatures[3].AUTO_WHITE_BALANCE_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_IRIS,fwCamFeatures[3].AUTO_IRIS_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_FOCUS,fwCamFeatures[3].AUTO_FOCUS_CONFIG);
	      dc1394_auto_on_off(handles[p],cameras[numCameras].node,FEATURE_ZOOM,fwCamFeatures[3].AUTO_ZOOM_CONFIG);
	    }
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

void firewire_startup(char *configfile)
{
  int i;

  /* reseting serve color array and setting default options */
  for(i=0;i<MAXCAM;i++){serve_color[i]=0; number_color[i]=-1; color_active[i]=0;}
  set_default_firewire_camera_config();

  /* we call the function to parse the config file */
  if(firewire_parseconf(configfile)==-1){
    printf("firewire: cannot initiate driver. configfile parsing error.\n");
    exit(-1);
  }

  /* firewire initialitation */
  if(firewire_setup==0) firewire_init();

  if(firewire_thread_created==0){

    pthread_mutex_lock(&mymutex);
    state=slept;
    pthread_create(&firewire_th,NULL,firewire_thread,NULL);
    firewire_thread_created=1;
    pthread_mutex_unlock(&mymutex);
  }

  /* displays autofocus message for each camera */
  if(serve_color[0]==1){
    if(number_color[0]<=numCameras){
      if(fwCamFeatures[0].AUTO_FOCUS_CONFIG==1) printf("colorA autofocus feature: on\n");
      else printf("colorA autofocus feature: off\n");
    }else{
      serve_color[0]=0;
      printf("cannot find firewire camera for colorA\n");
    }
  }

  if(serve_color[1]==1){
    if(number_color[1]<=numCameras){
      if(fwCamFeatures[1].AUTO_FOCUS_CONFIG==1) printf("colorB autofocus feature: on\n");
      else printf("colorB autofocus feature: off\n");
    }else{
      serve_color[1]=0;
      printf("cannot find firewire camera for colorB\n");
    }
  }

  if(serve_color[2]==1){
    if(number_color[2]<=numCameras){
      if(fwCamFeatures[2].AUTO_FOCUS_CONFIG==1) printf("colorC autofocus feature: on\n");
      else printf("colorC autofocus feature: off\n");
    }else{
      serve_color[2]=0;
      printf("cannot find firewire camera for colorC\n");
    }
  }

  if(serve_color[3]==1){
    if(number_color[3]<=numCameras){
      if(fwCamFeatures[3].AUTO_FOCUS_CONFIG==1) printf("colorD autofocus feature: on\n");
      else printf("colorD autofocus feature: off\n");
    }else{
      serve_color[3]=0;
      printf("cannot find firewire camera for colorD\n");
    }
  }

  /* resume and suspend asignments */
  imageA_resume=colorA_resume;
  imageA_suspend=colorA_suspend;
  imageB_resume=colorB_resume;
  imageB_suspend=colorB_suspend;
  imageC_resume=colorC_resume;
  imageC_suspend=colorC_suspend;
  imageD_resume=colorD_resume;
  imageD_suspend=colorD_suspend;

  printf("firewire driver started up\n");
}
