/*
 *  Copyright (C) 2007 Jose Antonio Santos Cadenas
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
 *              Jose Maria Cañas Plaza <jmplaza@gsyc.es>
 */

/**
 *  jdec graphics_xforms driver provides support for Xforms window manager.
 *
 *  @file graphics_xforms.c
 *  @author Jose Antonio Santos Cadenas <santoscadenas@gmail.com> and Jose Maria Cañas Plaza <jmplaza@gsyc.es>
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

/** pthread identifier for jdec graphics_xform driver thread.*/
pthread_t graphics_xforms_id;

/* Necesarias para las Xlib, para todos los guis */
/** Necessary to connect the schemas gui with Xlib */
Display* display;
/** Screen numbre to denermine connection to Xlib */
int screen;

/** graphics_xforms driver name.*/
char driver_name[256]="graphics_xforms";

/** Graphics_xforms thread will iterate each \ref graphics_xforms_cycle  ms*/
int graphics_xforms_cycle=70; /* ms */

/************************FIFO*********************************/
/** Fifo structure node type definition*/
typedef struct node{
   /** Pointer to de data casted to (void *)*/
   void *data;
   /** Pointer to the next node*/
   struct node * next;
}t_node;

/** Pointer to a fifo node*/
typedef t_node *p_node;
/** Fifo type definition*/
typedef struct fifo{
   /** Pointer to the first node of the fifo*/
   p_node init;
   /** Pointer to the last node of the fifo*/
   p_node end;
   /** Mutex to control concurrent access to the fifo*/
   pthread_mutex_t fifo_mutex;
}t_fifo;

/** Queue containing all the display callbacks. @see gui_callback*/
t_fifo gui_callbacks;

/**
 * This function inserts new data in a queue
 * @param fifo The fifo where the data will be inserted
 * @param data Pointer to tha data casted to (void *)
 * @returns 0 if the insetion was correct, 1 if ther ocurred an error
 */
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

/**
 * This function extracts new data from a queue
 * @param fifo The fifo (queue) containing the desired data at its firts position
 * @returns A pointer to the requested data, NULL in case of error
 */
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

/**
 * Initiates a fifo, it deletes all the possible information set the mutex correctly
 * @param fifo The fifo that will be initiated
 * @returns 0 if no errors ocurred
 */
int new_fifo (t_fifo *fifo){
   if (fifo->init==NULL)
      while (extract(fifo)!=NULL); /*Extract all the data*/
   /*The initializate the straucture.*/
   fifo->init=NULL;
   fifo->end=NULL;
   pthread_mutex_init(&(fifo->fifo_mutex),PTHREAD_MUTEX_TIMED_NP);
   return 0;
}

/*********************END_FIFO*********************************/

/**
 * @brief This functions will be called by schemas implementing its gui with xforms when they are requested to show its gui.
 * 
 * As xforms functions can't be called from different threads. When a schema
 * (or human) requires to activate one of the xforms guis, this function must
 * be called instead of the real gui_suspend or gui_resume function. Then the
 * graphics_xforms thread will call the real function to activate/deactivate
 * schema's gui.
 *
 * @param f Pointer to the function that must be called to change the schema's gui state.
 * 
 */
void gui_callback(gui_function f){
   if (((void *)f)!=NULL)
      insert(&gui_callbacks,(void *)f);
}

/* GRAPHICS_XFORMS DRIVER FUNCTIONS */

/**
 * Array with all the subscribed buttons callbacks.
 *
 * This callbacks are used to implement the schema's buttons actions
 */
guibuttons buttonscallbacks[MAX_SCHEMAS];
/** The number of subscribed buttons callbacks*/
int num_buttonscallbacks=0;
/**
 * @brief This fuction is used to subscribe a buttons callback.
 *
 * It will add the function pointed by f to the @ref buttonscallbacks array.and
 * increment the \ref num_buttonscallbacks counter.
 *
 * @param f A pointer to the function
 * @returns 1 if the function was correctly subscribed, othewise 0
 */
int register_buttonscallback(guibuttons f)
{
   int i;
   int found=0;
   if (f!=NULL)
   {
      for(i=0;i<num_buttonscallbacks;i++){
         if (buttonscallbacks[i]==NULL)
         {
            buttonscallbacks[i]=f;
            found=1;
            break;
         }
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

/**
 * @brief This fuction is used to unsubscribe a buttons callback.
 *
 * It will delete the function pointed by f from the @ref buttonscallbacks array.and
 * decrement the \ref num_buttonscallbacks counter.
 *
 * @param f A pointer to the function
 * @returns 1 if the function was correctly unsubscribed, othewise 0
 */
int delete_buttonscallback(guibuttons f)
{
   int i;
   if (f!=NULL)
   {
      for(i=0;i<num_buttonscallbacks;i++){
         if (buttonscallbacks[i]==f){
            buttonscallbacks[i]=NULL;
            break;
         }
      }
   }
   return 1;
}

/**
 * Array with all the subscribed display callbacks.
 *
 * This callbacks are used to refresh the schema's display
 */
guidisplay displaycallbacks[MAX_SCHEMAS];
/** The number of subscribed display callbacks*/
int num_displaycallbacks=0;
/**
 * @brief This fuction is used to subscribe a display callback.
 *
 * It will add the function pointed by f to the @ref displaycallbacks array.and
 * increment the \ref num_displaycallbacks counter.
 *
 * @param f A pointer to the function
 * @returns 1 if the function was correctly subscribed, othewise 0
 */
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

/**
 * @brief This fuction is used to unsubscribe a display callback.
 *
 * It will delete the function pointed by f from the @ref displaycallbacks array.and
 * decrement the \ref num_displaycallbacks counter.
 *
 * @param f A pointer to the function
 * @returns 1 if the function was correctly unsubscribed, othewise 0
 */
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

/**
 * \brief Driver thread iteration
 *
 * Checks all the buttons and call every registered buttons callback. Then
 * calls every registered display callbacks to refresh it. And finally
 * extract every gui functions from the queue and calls it.
 */
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

/** graphics_xforms driver function finalize.*/
void graphics_xforms_close(){
  /*  pthread_kill (graphics_xforms_id, 15);*/
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
