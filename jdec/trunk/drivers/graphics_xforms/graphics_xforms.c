/*
 *  Copyright (C) 2007 Javier Martin Ramos, Jose Antonio Santos Cadenas
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
 *  Authors :   Jose Antonio Santos Cadenas  <santoscadenas@gmail.com>
 */

/**
 *  jdec graphics_xforms driver provides support for GTK window manager.
 *
 *  @file graphics_xforms.c
 *  @author Jose Antonio Santos Cadenas <santoscadenas@gmail.com>
 *  @version 1.0
 *  @date 2007-11-21
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "jde.h"
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <forms.h>

#include "graphics_xforms.h"


pthread_t graphics_xforms_id;

/* Necesarias para las Xlib, para todos los guis */
Display* display;
int screen;

/** graphics_xforms driver name.*/
char driver_name[256]="graphics_xforms";

int graphics_xforms_cycle=70; /* ms */

/*FIFO*/
typedef struct node{
   void *data;
   struct node * next;
}t_node;

typedef t_node *p_node;
typedef struct fifo{
   p_node init;
   p_node end;
   pthread_mutex_t fifo_mutex;
}t_fifo;

t_fifo gui_callbacks;

int insert(t_fifo *fifo, void* data){
   p_node aux;
   pthread_mutex_lock(&(fifo->fifo_mutex));
   if ((aux=(p_node)malloc (sizeof(t_node)))==NULL){
      fprintf (stderr, "Error inserting in fifo\n");
      pthread_mutex_unlock(&(fifo->fifo_mutex));
      return 1;
   }
   aux->data=data;
   aux->next=fifo->init;
   if (fifo->end==NULL){
      fifo->init=aux;
      fifo->end=fifo->init;
   }
   else{
      fifo->end->next=aux;
      fifo->end=aux;
   }
   pthread_mutex_unlock(&(fifo->fifo_mutex));
   return 0;
}

void * extract(t_fifo *fifo){
   p_node aux;
   void * value;
   pthread_mutex_lock(&(fifo->fifo_mutex));
   if (fifo->init==NULL){
      pthread_mutex_unlock(&(fifo->fifo_mutex));
      return NULL;
   }
   else{
      aux=fifo->init;
      fifo->init=aux->next;
      value=aux->data;
      free(aux);
      if (fifo->init==NULL)
         fifo->end=NULL;
      pthread_mutex_unlock(&(fifo->fifo_mutex));
      return value;
   }
}

int new_fifo (t_fifo *fifo){
   if (fifo->init==NULL)
      while (extract(fifo)!=NULL); /*Extract all the data*/
   /*The initializate the straucture.*/
   fifo->init=NULL;
   fifo->end=NULL;
   pthread_mutex_init(&(fifo->fifo_mutex),PTHREAD_MUTEX_TIMED_NP);
   return 0;
}

void gui_callback(gui_function f){
   if (((void *)f)!=NULL)
      insert(&gui_callbacks,(void *)f);
}

/* GRAPHICS_XFORMS DRIVER FUNCTIONS */

guibuttons buttonscallbacks[MAX_SCHEMAS];
int num_buttonscallbacks=0;
int register_buttonscallback(guibuttons f)
{
   int i;
   int found=0;
   if (f!=NULL)
   {
      for(i=0;i<num_buttonscallbacks;i++)
         if (buttonscallbacks[i]==NULL)
      {
         buttonscallbacks[i]=f;
         found=1;
         break;
      }
      if ((found==0)&&(num_buttonscallbacks<MAX_SCHEMAS))
      {
         buttonscallbacks[num_buttonscallbacks]=f;
         num_buttonscallbacks++;
      }
      else if ((found==0)&&(num_buttonscallbacks>=MAX_SCHEMAS))
         printf("Warning no space for registering a buttonscallback\n");
   }
   return 1;
}

int delete_buttonscallback(guibuttons f)
{
   int i;
   if (f!=NULL)
   {
      for(i=0;i<num_buttonscallbacks;i++)
         if (buttonscallbacks[i]==f)
      {
         buttonscallbacks[i]=NULL;
         break;
      }
   }
   return 1;
}


guidisplay displaycallbacks[MAX_SCHEMAS];
int num_displaycallbacks=0;
int register_displaycallback(guidisplay f)
{
   int i;
   int found=0;
   if (f!=NULL)
   {
      for(i=0;i<num_displaycallbacks;i++)
         if (displaycallbacks[i]==NULL)
      {
         displaycallbacks[i]=f;
         found=1;
         break;
      }
      if ((found==0)&&(num_displaycallbacks<MAX_SCHEMAS))
      {
         displaycallbacks[num_displaycallbacks]=f;
         num_displaycallbacks++;
      }
      else if ((found==0)&&(num_displaycallbacks>=MAX_SCHEMAS))
         printf("Warning no space for registering a displaycallback\n");
   }
   return 1;
}

int delete_displaycallback(guidisplay f)
{
   int i;
   if (f!=NULL)
   {
      for(i=0;i<num_displaycallbacks;i++)
         if (displaycallbacks[i]==f)
      {
         displaycallbacks[i]=NULL;
         break;
      }
   }
   return 1;
}

static void graphics_xforms_iteration()
{
   FL_OBJECT *obj;
   int i;
   gui_function fn;

   /* buttons check (polling) */
   /* Puesto que el control no se cede al form, sino que se hace polling de sus botones pulsados, debemos proveer el enlace para los botones que no tengan callback asociada, en esta rutina de polling. OJO aquellos que tengan callback asociada jamas se veran con fl_check_forms, la libreria llamara automaticamente a su callback sin que fl_check_forms o fl_do_forms se enteren en absoluto.*/
   obj = fl_check_forms();
   if (obj!=NULL){
      for(i=0;i<num_buttonscallbacks;i++){
         if (buttonscallbacks[i]!=NULL)
            (buttonscallbacks[i])(obj);
      }
   }

   /* display iteration */
   for(i=0;i<num_displaycallbacks;i++){
      if (displaycallbacks[i]!=NULL)
         (displaycallbacks[i])();
   }

   while ((fn=(gui_function)extract(&gui_callbacks))!=NULL){
      fn();
   }
}

/** graphics_xforms driver function to close devices.*/
void graphics_xforms_close(){
   pthread_kill (graphics_xforms_id, 15);
   printf("driver graphics_xforms off\n");
}

/** graphics_xforms driver internal thread.
 *  @param id selected color id.*/
void *graphics_xforms_thread(void *id){
   struct timeval a,b;
   long diff, next;

   for(;;)
   {
      gettimeofday(&a,NULL);
      graphics_xforms_iteration();
      gettimeofday(&b,NULL);
      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
      next = graphics_xforms_cycle*1000-diff-10000;
      /* discounts 10ms taken by calling usleep itself */
      if (next>0) usleep(graphics_xforms_cycle*1000-diff);
      else;
      /* Time interval violated but don't bother with 
      any warning message, just display as fast as it can */
   }
}

/** graphics_xforms driver startup function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void graphics_xforms_startup(char *configfile)
{
   int myargc=1;
   char **myargv;
   char *aa;
   char a[]="myjde";
   
   printf ("Loading Xform support...\n");
   
   aa=a;
   myargv=&aa;
   display= fl_initialize(&myargc,myargv,"jdec",0,0);
   screen = DefaultScreen(display);
   myexport ("graphics_xforms", "screen", (void *)&screen);
   myexport ("graphics_xforms", "display", (void *)display);
   myexport ("graphics_xforms", "register_buttonscallback", (void *)register_buttonscallback);
   myexport ("graphics_xforms", "register_displaycallback", (void *)register_displaycallback);
   myexport ("graphics_xforms", "delete_buttonscallback", (void *)delete_buttonscallback);
   myexport ("graphics_xforms", "delete_displaycallback", (void *)delete_displaycallback);
   new_fifo(&gui_callbacks);
   myexport ("graphics_xforms", "resume_callback", (void *)gui_callback);
   myexport ("graphics_xforms", "suspend_callback", (void *)gui_callback);

   
   pthread_create(&graphics_xforms_id,NULL,graphics_xforms_thread,NULL);
   printf ("Xforms support loaded.\n");
}
