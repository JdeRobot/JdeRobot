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
 *
 *  Authors : José Antonio Santos Cadenas <santoscadenas@gmail.com>
 *            Javier Martín Ramos <jmartinramos@gmail.com>
 */

#include <jde.h>
#include <forms.h>
#include <graphics_xforms.h>
#include <stdlib.h>
#include <unistd.h>
#include "recordergui.h"
#include "recorder.h"
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

/** Image standard number of rows*/
#define SIFNTSC_ROWS 240
/** Image standard number of columns*/
#define SIFNTSC_COLUMNS 320

/*Gui declarations*/
Display *mydisplay;
int  *myscreen;

/*Gui callbacks*/
registerbuttons myregister_buttonscallback;
registerdisplay myregister_displaycallback;
deletebuttons mydelete_buttonscallback;
deletedisplay mydelete_displaycallback;


#define RECORDERver "recorder 2.0"

#define NULL_FORMAT "none"
#define BGR24 "bgr24"
#define BGR32 "bgr32"

#define ROUTE_LEN 255

int recorder_id=0;
int recorder_brothers[MAX_SCHEMAS];
arbitration recorder_callforarbitration;

char directorio[255];
char fifo[ROUTE_LEN];

/**
 * Elige el codec que satisfaga tus intereses de entre los que liste el comando
 * "mencoder -ovc help" y cambia el valor de CODEC_NAME.
 */
#define CODEC_NAME "copy"

/* exported variables */
int recorder_cycle=40; /* ms */

#define PUSHED 1
#define RELEASED 0

/*gui variables*/
float record_fps=25.0;
int record=0;
char video_name[256];
char input_image[256];
char input_schema[256];
int preview = PUSHED;
FD_recordergui *fd_recordergui=NULL;
unsigned char show_img[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4];//Para mostrar en el display
char image_format[10]; // Formato con el que mencoder interpretará la imagen de entrada
char ncanales;

/*Xlib variables*/
GC recorder_gc;
Window recorder_window;
XImage *image;

/*Imported variables*/
char **mycolorA=NULL;
runFn mycolorArun;
stopFn mycolorAstop;

int recordergui_setupDisplay(void)
      /* Inicializa las ventanas, la paleta de colores y memoria compartida para visualizacion*/
{
   int vmode;
   static XGCValues gc_values;

   gc_values.graphics_exposures = False;
   recorder_gc = XCreateGC(mydisplay,recorder_window, GCGraphicsExposures, &gc_values);
   
   vmode= fl_get_vclass();

   if ((vmode==TrueColor)&&(fl_state[vmode].depth==16))
   {
      printf("16 bits mode\n");
      /* Imagen principal */
      image = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),16,
			       ZPixmap, 0, (char*)show_img, SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);

   }
   else if ((vmode==TrueColor)&&(fl_state[vmode].depth==24))
   {
      printf("24 bits mode\n");
      /* Imagen principal */
      image = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),24,
			       ZPixmap, 0, (char*)show_img, SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);

   }
   else if ((vmode==TrueColor)&&(fl_state[vmode].depth==32))
   {

      printf("32 bits mode\n");
      /* Imagen principal */
      image = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),32,
			       ZPixmap, 0, (char*)show_img, SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);

      
   }
   else if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8))
   {
      printf("8 bits mode\n");
      /* Imagen principal */
      image = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),8,
			       ZPixmap, 0, (char*)show_img, SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);
   }
   else
   {
      perror("Unsupported color mode in X server");exit(1);
   }
   return 1;
}

void recorder_terminate(){
 if (fd_recordergui!=NULL)
    {
      if (all[recorder_id].guistate==on) 
	fl_hide_form(fd_recordergui->recordergui);
      fl_free_form(fd_recordergui->recordergui);
    }
  printf ("recorder close\n");
}

void recorder_iteration(){
   static int prev_state=0;
   static int fifo_fd;
   speedcounter(recorder_id);

   if (prev_state==0 && record==1 && strcmp(image_format, NULL_FORMAT)){
      /*Comienza una nueva grabación*/
      /*Se crea un fifo y se lanza mplayer*/
      unlink (fifo);
      if ( (mkfifo (fifo, 0600) != 0) ){
         fprintf (stderr, "imposible crear el fifo\n");
         exit (1);
      }
      if (fork() == 0) {/* We create a new process...
         // ... close its stdin, stdout & stderr ...*/
         char str[50];
         int file;
         file = open("/dev/null",O_RDWR);
         close(0); dup(file);
         close(1); dup(file);
         close(2); dup(file);

         sprintf(str,"fps=%.1f:w=%d:h=%d:format=%s",record_fps,
                 SIFNTSC_COLUMNS,SIFNTSC_ROWS, image_format);
         execlp("mencoder","mencoder", fifo,"-demuxer","rawvideo", "-rawvideo",
                str, "-o", video_name, "-ovc", CODEC_NAME ,NULL);
         printf("Error executing mencoder\n");
         exit(1);
      }
      if ((fifo_fd=open(fifo, O_WRONLY))<0){
         fprintf (stderr, "Error al abrir el fifo\n");
         exit(1);
      }
      prev_state=1;
   }
   else if (prev_state==1 && record==1){
      /*Está grabando, se escribe en el fifo la imagen actual colorA*/
      if (write (fifo_fd, (*mycolorA), SIFNTSC_COLUMNS*SIFNTSC_ROWS*ncanales)>SIFNTSC_COLUMNS*SIFNTSC_ROWS*ncanales){
         fprintf (stderr, "Error al Escribir en el fifo\n");
         exit(1);
      }
   }
   else if (prev_state==1 && record==0){
      /*Termina la grabación se cierra el fifo y morirá con ello mencoder*/
      close (fifo_fd);
      unlink(fifo);
      wait (NULL); /*Esperar la muerte de mencoder*/
      prev_state=0;
   }
}


void recorder_stop(){
  pthread_mutex_lock(&(all[recorder_id].mymutex));
  put_state(recorder_id,slept);
  /*  printf("recorder: off\n");*/
  pthread_mutex_unlock(&(all[recorder_id].mymutex));
}


void recorder_run(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN) 
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[recorder_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[recorder_id].mymutex));
  /* this schema runs its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[recorder_id].children[i]=FALSE;
  all[recorder_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) recorder_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {recorder_brothers[i]=brothers[i];i++;}
    }
  recorder_callforarbitration=fn;
  put_state(recorder_id,notready);

  /* printf("recorder: on\n");*/
  pthread_cond_signal(&(all[recorder_id].condition));
  pthread_mutex_unlock(&(all[recorder_id].mymutex));
}

void *recorder_thread(void *not_used)
{
  struct timeval a,b;
  long n=0; /* iteration */
  long next,bb,aa;

  for(;;)
    {
      pthread_mutex_lock(&(all[recorder_id].mymutex));

      if (all[recorder_id].state==slept)
	{
	  pthread_cond_wait(&(all[recorder_id].condition),&(all[recorder_id].mymutex));
	  pthread_mutex_unlock(&(all[recorder_id].mymutex));
	}
      else
	{
	  /* check preconditions. For now, preconditions are always satisfied*/
	  if (all[recorder_id].state==notready)
	    put_state(recorder_id,ready);
	  /* check brothers and arbitrate. For now this is the only winner */
	  if (all[recorder_id].state==ready)
	    {put_state(recorder_id,winner);
	    gettimeofday(&a,NULL);
	    aa=a.tv_sec*1000000+a.tv_usec;
	    n=0;
	    }

	  if (all[recorder_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[recorder_id].mymutex));
	      /*      gettimeofday(&a,NULL);*/
	      n++;
	      recorder_iteration();
	      gettimeofday(&b,NULL);
	      bb=b.tv_sec*1000000+b.tv_usec;
	      next=aa+(n+1)*(long)recorder_cycle*1000-bb;

	      /* diff = (b.tv_sec*1000000+b.tv_usec)-(a.tv_sec*1000000+a.tv_usec);*/
	      /* next = (long)recorder_cycle*1000-diff-3; */

	      if (next>5000)
		{
		  usleep(next-5000);
		  /* discounts 5ms taken by calling usleep itself, on average */
		}
	      else  ;
	    }
	  else
	    /* just let this iteration go away. overhead time negligible */
	    {
	      pthread_mutex_unlock(&(all[recorder_id].mymutex));
	      usleep(recorder_cycle*1000);
	    }
	}
    }
}

void recorder_guiinit(){
   if (myregister_buttonscallback==NULL){
      if ((myregister_buttonscallback=(registerbuttons)myimport ("graphics_xforms", "register_buttonscallback"))==NULL){
         printf ("I can't fetch register_buttonscallback from graphics_xforms\n");
         jdeshutdown(1);
      }
      if ((mydelete_buttonscallback=(deletebuttons)myimport ("graphics_xforms", "delete_buttonscallback"))==NULL){
         printf ("I can't fetch delete_buttonscallback from graphics_xforms\n");
         jdeshutdown(1);
      }
      if ((myregister_displaycallback=(registerdisplay)myimport ("graphics_xforms", "register_displaycallback"))==NULL){
         printf ("I can't fetch register_displaycallback from graphics_xforms\n");
         jdeshutdown(1);
      }
      if ((mydelete_displaycallback=(deletedisplay)myimport ("graphics_xforms", "delete_displaycallback"))==NULL){
         jdeshutdown(1);
         printf ("I can't fetch delete_displaycallback from graphics_xforms\n");
      }
   }

   if ((myscreen=(int *)myimport("graphics_xforms", "screen"))==NULL){
      fprintf (stderr, "teleoperator: I can't fetch screen from graphics_xforms\n");
      jdeshutdown(1);
   }
   if ((mydisplay=(Display *)myimport("graphics_xforms", "display"))==NULL){
      fprintf (stderr, "teleoperator: I can't fetch display from graphics_xforms\n");
      jdeshutdown(1);
   }
}

void recorder_init(char *configfile)
{
  int i;
  pthread_mutex_lock(&(all[recorder_id].mymutex));
  printf("recorder schema started up\n");
  myexport("recorder","id",&recorder_id);
  myexport("recorder","cycle",&recorder_cycle);
  myexport("recorder","run",(void *)recorder_run);
  myexport("recorder","stop",(void *)recorder_stop);
  put_state(recorder_id,slept);
  for (i=0; i<SIFNTSC_COLUMNS*SIFNTSC_ROWS; i++){
     show_img[i*4+3]=UCHAR_MAX;
  }

  strcpy(image_format, BGR24);
  ncanales = 3;
  strcpy (directorio, "/tmp/jde-recorder-XXXXXX");
  if (mkdtemp(directorio)==NULL){
     perror ("I can't create a temp directory: ");
     exit (-1);
  }

  if (snprintf(fifo, ROUTE_LEN, "%s/recorder", directorio)<0){
     fprintf (stderr, "Can't create temp files\n");
     exit (-1);
  }

  pthread_create(&(all[recorder_id].mythread),NULL,recorder_thread,NULL);
  recorder_guiinit();
  pthread_mutex_unlock(&(all[recorder_id].mymutex));
}

void recorder_guibuttons(void *obj1){
   int value;
   FL_OBJECT *obj=(FL_OBJECT *)obj1;

   if (obj == fd_recordergui->record){
      if (fl_get_button (obj)==RELEASED){
         fl_trigger_object(fd_recordergui->stop);
         record=0;
      }
      else {
         strcpy(video_name, fl_get_input(fd_recordergui->video_name));
         if (video_name[0]=='\0'){
            record=0;
            fl_set_button(fd_recordergui->record,record);
            fl_set_object_label(fd_recordergui->status,"Specify output file name");
         }
         else if (input_image[0]=='\0'){
            record=0;
            fl_set_button(fd_recordergui->record,record);
            fl_set_object_label(fd_recordergui->status,"Specify input image");
         }
         else if (input_schema[0]=='\0'){
            record=0;
            fl_set_button(fd_recordergui->record,record);
            fl_set_object_label(fd_recordergui->status,"Specify input schema");
         }
         else if (!strcmp(image_format, NULL_FORMAT)) {
            record=0;
            fl_set_button(fd_recordergui->record, record);
            fl_set_object_label(fd_recordergui->status,"Specify image format");
         }
         else {
            char str_aux[256];
            snprintf (str_aux, 255,"Recording %s from %s at %s\n",video_name, input_image, input_schema);
            str_aux[255]='\0';
            fl_set_object_label(fd_recordergui->status,str_aux);
            fl_deactivate_object(fd_recordergui->video_name);
            fl_deactivate_object(fd_recordergui->input_image);
            fl_deactivate_object(fd_recordergui->input_schema);
            fl_deactivate_object(fd_recordergui->fetch_input);
            fl_deactivate_object(fd_recordergui->fps);
            fl_deactivate_object(fd_recordergui->bgr24);
            fl_deactivate_object(fd_recordergui->bgr32);
            record=1;
         }
      }
   }
   else if (obj == fd_recordergui->fetch_input) {
      if (record == 0) {
         strcpy(input_schema, fl_get_input(fd_recordergui->input_schema));
         strcpy(input_image, fl_get_input(fd_recordergui->input_image));
         mycolorA=(char **) myimport (input_schema, input_image);
         mycolorArun=(runFn)myimport(input_schema, "run");
         mycolorAstop=(stopFn)myimport(input_schema,"stop");
         if (mycolorA == NULL) {
            fl_set_object_label(fd_recordergui->status,"Can't fetch the input");
         }
         else {
            fl_set_object_label(fd_recordergui->status,"Fetched");
            mycolorArun(recorder_id,NULL,NULL);
         }
      }
      fl_set_button(fd_recordergui->fetch_input, RELEASED);
   }
   else if (obj == fd_recordergui->fps){
      record_fps = (float)fl_get_slider_value(obj);
      recorder_cycle=(int)(1000/record_fps);
   }
   else if (obj == fd_recordergui->stop){
      fl_activate_object(fd_recordergui->video_name);
      fl_activate_object(fd_recordergui->input_image);
      fl_activate_object(fd_recordergui->input_schema);
      fl_activate_object(fd_recordergui->fetch_input);
      fl_activate_object(fd_recordergui->fps);
      fl_activate_object(fd_recordergui->bgr24);
      fl_activate_object(fd_recordergui->bgr32);
      record=0;
      fl_set_button(fd_recordergui->stop, RELEASED);
      fl_set_button(fd_recordergui->record,record);
      fl_set_object_label(fd_recordergui->status,"Stopped");
   }
   else if (obj == fd_recordergui->bgr24) {
      value = fl_get_button(obj);
      fl_set_button(fd_recordergui->bgr32, RELEASED);
      strcpy(image_format, value == PUSHED ? BGR24 : NULL_FORMAT);
      ncanales = 3;
   }
   else if (obj == fd_recordergui->bgr32) {
      value = fl_get_button(obj);
      fl_set_button(fd_recordergui->bgr24, RELEASED);
      strcpy(image_format, value == PUSHED ? BGR32 : NULL_FORMAT);
      ncanales = 4;
   }
   else if (obj == fd_recordergui->preview) {
      preview = fl_get_button(obj);
      if (preview == 0) {
         fl_hide_object(fd_recordergui->image);
      }
      else {
         fl_show_object(fd_recordergui->image);
      }
   }
}

void recorder_guidisplay(){
	if (preview != RELEASED && mycolorA != NULL && strcmp(image_format, NULL_FORMAT)) {
		if (*mycolorA!=NULL){
			int i;
			for (i=0; i<SIFNTSC_COLUMNS*SIFNTSC_ROWS; i++){
				show_img[i*4]=(*mycolorA)[i*ncanales];
				show_img[i*4+1]=(*mycolorA)[i*ncanales+1];
				show_img[i*4+2]=(*mycolorA)[i*ncanales+2];
			}
			
			XPutImage(mydisplay,recorder_window,recorder_gc,image,0,0,
			fd_recordergui->image->x,fd_recordergui->image->y,
			SIFNTSC_COLUMNS, SIFNTSC_ROWS);
		}
	}
}

void recorder_hide_aux(void)
{
   mydelete_buttonscallback(recorder_guibuttons);
   mydelete_displaycallback(recorder_guidisplay);
   fl_hide_form(fd_recordergui->recordergui);
}

void recorder_hide(void){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "suspend_callback"))!=NULL){
         fn ((gui_function)recorder_hide_aux);
      }
   }
   else{
      fn ((gui_function)recorder_hide_aux);
   }
}

void recorder_show_aux(void)
{
   static int k=0;

   if (k==0){ /* not initialized */

      k++;
      fd_recordergui = create_form_recordergui();
      fl_set_form_position(fd_recordergui->recordergui,100,200);
      fl_show_form(fd_recordergui->recordergui,FL_PLACE_POSITION,FL_FULLBORDER,RECORDERver);
      recorder_window = FL_ObjWin(fd_recordergui->image);
      recordergui_setupDisplay();
   }
   else{
      fl_show_form(fd_recordergui->recordergui,FL_PLACE_POSITION,FL_FULLBORDER,RECORDERver);
      recorder_window = FL_ObjWin(fd_recordergui->image);
   }

   /*Asignar los valores al gui*/
   fl_set_button (fd_recordergui->record, record);
   fl_set_slider_value (fd_recordergui->fps, record_fps);
   recorder_cycle=(int)(1000/record_fps);
   fl_set_object_label(fd_recordergui->status,"Stopped");

   myregister_buttonscallback(recorder_guibuttons);
   myregister_displaycallback(recorder_guidisplay);
}

void recorder_show(void){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "resume_callback"))!=NULL){
         fn ((gui_function)recorder_show_aux);
      }
   }
   else{
      fn ((gui_function)recorder_show_aux);
   }
}
