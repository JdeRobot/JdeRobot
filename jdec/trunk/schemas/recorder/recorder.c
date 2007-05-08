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
 */

#define NORMAL 0
#define SPEEDY 1

#define MODE SPEEDY

#include "jde.h"
#include "jdegui.h"
#include "recordergui.h"
#include "recorder.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>



#define RECORDERver "recorder 1.0" /*Nombre y versión del programa*/

#define MYFIFO "/tmp/recorder_fifo"

int recorder_id=0;
int recorder_brothers[MAX_SCHEMAS];
arbitration recorder_callforarbitration;

/* exported variables */
int recorder_cycle=40; /* ms */

#define PUSHED 1
#define RELEASED 0

/*gui variables*/
float record_fps=25.0;
int record=0;
char video_name[256];

FD_recordergui *fd_recordergui=NULL;

/*Xlib variables*/
GC recorder_gc;
Window recorder_window;
XImage *image;

/*Imported variables*/
char **mycolorA=NULL;
resumeFn mycolorAresume;
suspendFn mycolorAsuspend;

unsigned char show_img[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4];//Para mostrar en el display

int recordergui_setupDisplay(void)
      /* Inicializa las ventanas, la paleta de colores y memoria compartida para visualizacion*/
{
   int vmode;
   static XGCValues gc_values;

   gc_values.graphics_exposures = False;
   recorder_gc = XCreateGC(display,recorder_window, GCGraphicsExposures, &gc_values);
   
   vmode= fl_get_vclass();

   if ((vmode==TrueColor)&&(fl_state[vmode].depth==16))
   {
      printf("16 bits mode\n");
      /* Imagen principal */
      image = XCreateImage(display,DefaultVisual(display,screen),16,
			       ZPixmap, 0, (char*)show_img, SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);

   }
   else if ((vmode==TrueColor)&&(fl_state[vmode].depth==24))
   {
      printf("24 bits mode\n");
      /* Imagen principal */
      image = XCreateImage(display,DefaultVisual(display,screen),24,
			       ZPixmap, 0, (char*)show_img, SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);

   }
   else if ((vmode==TrueColor)&&(fl_state[vmode].depth==32))
   {

      printf("32 bits mode\n");
      /* Imagen principal */
      image = XCreateImage(display,DefaultVisual(display,screen),32,
			       ZPixmap, 0, (char*)show_img, SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);

      
   }
   else if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8))
   {
      printf("8 bits mode\n");
      /* Imagen principal */
      image = XCreateImage(display,DefaultVisual(display,screen),8,
			       ZPixmap, 0, (char*)show_img, SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);
   }
   else
   {
      perror("Unsupported color mode in X server");exit(1);
   }
   return 1;
}


void recorder_iteration()
{
  static int prev_state=0;
  static int fifo;
  speedcounter(recorder_id);

  if (prev_state==0 && record==1){
     /*Comienza una nueva grabación*/
     /*Se crea un fifo y se lanza mplayer*/
     unlink (MYFIFO);
     if ( (mkfifo (MYFIFO, 0600) != 0) ){
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
// 	mencoder file2 -demuxer rawvideo -rawvideo fps=25.0:w=320:h=240:format=bgr24 -o file3 -ovc lavc
	sprintf(str,"fps=%.1f:w=%d:h=%d:format=bgr24",record_fps,
		SIFNTSC_COLUMNS,SIFNTSC_ROWS);
	execlp("mencoder","mencoder", MYFIFO,"-demuxer","rawvideo", "-rawvideo",
	       str, "-o", video_name, "-ovc", "lavc" ,NULL);
	printf("Error executing mencoder\n");
	exit(1);
     }
     if ((fifo=open(MYFIFO, O_WRONLY))<0){
	fprintf (stderr, "Error al abrir el fifo\n");
	exit(1);
     }
     prev_state=1;
  }
  else if (prev_state==1 && record==1){
     /*Está grabando, se escribe en el fifo la imagen actual colorA*/
     if (write (fifo, (*mycolorA), SIFNTSC_COLUMNS*SIFNTSC_ROWS*3)>SIFNTSC_COLUMNS*SIFNTSC_ROWS*3){
	fprintf (stderr, "Error al Escribir en el fifo\n");
	exit(1);
     }
  }
  else if (prev_state==1 && record==0){
     /*Termina la grabación se cierra el fifo y morirá con ello mencoder*/
     close (fifo);
     wait (NULL); /*Esperar la muerte de mencoder*/
     prev_state=0;
  }
}


void recorder_suspend()
{
  pthread_mutex_lock(&(all[recorder_id].mymutex));
  put_state(recorder_id,slept);
  /*  printf("recorder: off\n");*/
  pthread_mutex_unlock(&(all[recorder_id].mymutex));
}


void recorder_resume(int father, int *brothers, arbitration fn)
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
  /* this schema resumes its execution with no children at all */
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

  mycolorAresume(recorder_id,NULL,NULL); /*Arrancar colorA*/

  /* printf("recorder: on\n");*/
  pthread_cond_signal(&(all[recorder_id].condition));
  pthread_mutex_unlock(&(all[recorder_id].mymutex));
}

#if MODE == NORMAL
void *recorder_thread(void *not_used)
{
   struct timeval a,b;
   long diff, next;

   for(;;)
   {
      /* printf("recorder: iteration-cojo\n");*/
      pthread_mutex_lock(&(all[recorder_id].mymutex));

      if (all[recorder_id].state==slept)
      {
	 /*printf("recorder: off\n");*/
	 v=0; w=0;
	 /*  printf("recorder: suelto para dormirme en condicional\n");*/
	 pthread_cond_wait(&(all[recorder_id].condition),&(all[recorder_id].mymutex));
	 /*  printf("recorder: cojo tras dormirme en condicional\n");*/
	 /*printf("recorder: on\n");*/
// 	 recorder_state=init;
	 /* esto es para la aplicación, no tiene que ver con infraestrucura */
	 pthread_mutex_unlock(&(all[recorder_id].mymutex));
	 /* printf("recorder: iteration-suelto1\n");*/
      }
      else
      {
	 /* check preconditions. For now, preconditions are always satisfied*/
	 if (all[recorder_id].state==notready) put_state(recorder_id,ready);
	 else all[recorder_id].state=ready;
	 /* check brothers and arbitrate. For now this is the only winner */
	 if (all[recorder_id].state==ready) put_state(recorder_id,winner);


	 if (all[recorder_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	 {
	    pthread_mutex_unlock(&(all[recorder_id].mymutex));
	    /*printf("recorder: iteration-suelto2\n");*/

	    gettimeofday(&a,NULL);
	    recorder_iteration();
	    gettimeofday(&b,NULL);

	    diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	    next = recorder_cycle*1000-diff-10000;
	    /* discounts 10ms taken by calling usleep itself */
	    if (next>0) usleep(recorder_cycle*1000-diff);
	    else
	    {printf("time interval violated: recorder\n");
// 	    usleep(recorder_cycle*1000);
	    }
	 }
	 else
	    /* just let this iteration go away. overhead time negligible */
	 {
	    pthread_mutex_unlock(&(all[recorder_id].mymutex));
	    /*printf("recorder: iteration-suelto3\n");*/
	    usleep(recorder_cycle*1000);
	 }
      }
   }
}
#elif MODE == SPEEDY
void *recorder_thread(void *not_used)
{
  struct timeval a,b,c;
  long n=0; /* iteration */
  struct timespec d;
  long diff,next,bb,aa;

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
#else
Debe asignar un modo
#endif

void recorder_startup()
{
  int i;
  pthread_mutex_lock(&(all[recorder_id].mymutex));
  printf("recorder schema started up\n");
  myexport("recorder","id",&recorder_id);
  myexport("recorder","cycle",&recorder_cycle);
  myexport("recorder","resume",(void *)recorder_resume);
  myexport("recorder","suspend",(void *)recorder_suspend);
  put_state(recorder_id,slept);
  for (i=0; i<SIFNTSC_COLUMNS*SIFNTSC_ROWS; i++){
     show_img[i*4+3]=UCHAR_MAX;
  }

  /*Imports*/
  mycolorA=(char **) myimport ("colorA", "colorA");
  mycolorAresume=(resumeFn)myimport ("colorA", "resume");
  mycolorAsuspend=(suspendFn *)myimport("colorA","suspend");
  
  pthread_create(&(all[recorder_id].mythread),NULL,recorder_thread,NULL);
  pthread_mutex_unlock(&(all[recorder_id].mymutex));
}

void recorder_guibuttons(FL_OBJECT *obj){
   if (obj == fd_recordergui->record){
      if (fl_get_button (obj)==RELEASED){
         record=0;
      }
      else {
	 /*Comprobar si las opciones son correctas (nombre adecuado)*/
// 	 printf("nombre=>%s<\n",(char *)fl_get_input(fd_recordergui->video_name));
	 char *str;
	 str=(char *)fl_get_input(fd_recordergui->video_name);
	 if (str[0]=='\0'){
// 	    fprintf (stderr, "Indica el nombre del fichero\n");
	    record=0;
	    fl_set_button(fd_recordergui->record,record);
	    fl_set_object_label(fd_recordergui->status,"Indica el nombre del fichero");
	 }
	 else{
	    char str_aux[256];
	    strcpy (video_name,str);
// 	    printf ("%s\n",video_name);
	    snprintf (str_aux, 255,"Recording \"%s\"\n",video_name);
	    str_aux[255]='\0';
	    fl_set_object_label(fd_recordergui->status,str_aux);
	    record=1;
	 }
      }
   }
   else if (obj == fd_recordergui->fps){
      if (record==0){
         record_fps = (float)fl_get_slider_value(obj);
         recorder_cycle=(int)(1000/record_fps);
      }
      else{
	 fl_set_slider_value(fd_recordergui->fps,record_fps);
      }
   }
   else if (obj == fd_recordergui->stop){
      if (fl_get_button (obj)==PUSHED){
	 record=0;
	 fl_set_button(fd_recordergui->stop, RELEASED);
	 fl_set_button(fd_recordergui->record,record);
	 fl_set_object_label(fd_recordergui->status,"Stoped");
      }
   }
}

void recorder_guidisplay(){
   if (*mycolorA!=NULL){
      int i;
      for (i=0; i<SIFNTSC_COLUMNS*SIFNTSC_ROWS; i++){
	 show_img[i*4]=(*mycolorA)[i*3];
	 show_img[i*4+1]=(*mycolorA)[i*3+1];
	 show_img[i*4+2]=(*mycolorA)[i*3+2];
      }
      
      XPutImage(display,recorder_window,recorder_gc,image,0,0,
		fd_recordergui->image->x,fd_recordergui->image->y,
		SIFNTSC_COLUMNS, SIFNTSC_ROWS);
   }
}

void recorder_guisuspend(void)
{
   delete_buttonscallback(recorder_guibuttons);
   delete_displaycallback(recorder_guidisplay);
   fl_hide_form(fd_recordergui->recordergui);
}

void recorder_guiresume(void)
{
   static int k=0;

   if (k==0){ /* not initialized */

      k++;
      fd_recordergui = create_form_recordergui();
      fl_set_form_position(fd_recordergui->recordergui,100,200);
      fl_show_form(fd_recordergui->recordergui,FL_PLACE_POSITION,FL_FULLBORDER,RECORDERver);
      recorder_window = FL_ObjWin(fd_recordergui->image);
      recordergui_setupDisplay();
      mycolorA=(char **) myimport ("colorA", "colorA");
      mycolorAresume=(resumeFn)myimport ("colorA", "resume");
      mycolorAsuspend=(suspendFn *)myimport("colorA","suspend");
   }
   else{
      fl_show_form(fd_recordergui->recordergui,FL_PLACE_POSITION,FL_FULLBORDER,RECORDERver);
      recorder_window = FL_ObjWin(fd_recordergui->image);
   }

   mycolorAresume(recorder_id,NULL,NULL); /*Arrancar colorA*/
   
   /*Asignar los valores al gui*/
   fl_set_button (fd_recordergui->record, record);
   fl_set_slider_value (fd_recordergui->fps, record_fps);
   recorder_cycle=(int)(1000/record_fps);
   fl_set_object_label(fd_recordergui->status,"Stoped");

   register_buttonscallback(recorder_guibuttons);
   register_displaycallback(recorder_guidisplay);
}
