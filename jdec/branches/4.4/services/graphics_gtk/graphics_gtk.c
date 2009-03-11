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
 *  Authors : Jose Antonio Santos Cadenas <santoscadenas@gmail.com>
 *            Jose Maria Cañas <jmplaza@gsyc.escet.urjc.es>
 *
 *
 */


#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <jde.h>
#include <graphics_gtk.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gtk/gtkgl.h>

/** pthread identifier for jdec graphics_gtk driver thread.*/
pthread_t graphics_gtk_id;
/** pthread identifier for jdec graphics_gtk driver thread.*/
pthread_t graphics_gtk_id2;

/** graphics_gtk driver name.*/
static char driver_name[256]="graphics_gtk";

/** Graphics_gtk thread will iterate each \ref graphics_gtk_cycle  ms */
int graphics_gtk_cycle=70; /* ms */

/** The maximum path size*/
#define PATH_SIZE 512

/** The loaded path from config file*/
static char path[PATH_SIZE];

/* GRAPHICS_GTK DRIVER FUNCTIONS */

/**Array with all the subscribed display callbacks.
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
      else if ((found==0)&&(num_displaycallbacks>=MAX_SCHEMAS)){
         printf("Warning no space for registering a displaycallback\n");
         return 0;
      }
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
   if (f!=NULL){
      for(i=0;i<num_displaycallbacks;i++){
         if (displaycallbacks[i]==f){
            displaycallbacks[i]=NULL;
            break;
         }
      }
   }
   return 1;
}

/** \brief graphics_gtk driver function to finish driver execution.
 * This function kills driver threads
 */
void graphics_gtk_terminate(){
   printf("service graphics_gtk off\n");
}

/**
 * graphics_gtk driver internal thread to run gtk_main, necessary when using gtk.
 */
void *graphics_gtk_thread(void *arg){
   gdk_threads_enter();
   gtk_main();
   gdk_threads_leave();
   pthread_exit(0);
}

/**
 * The thread iteration. It refresh the displays of every subscribed schemas
 * @see register_displaycallback, delete_displaycallback
 */
void graphics_gtk_iteration(){
   int i;

   /* display iteration */
   for(i=0;i<num_displaycallbacks;i++)
   {
      if (displaycallbacks[i]!=NULL)
         (displaycallbacks[i])();
   }
}

/** graphics_gtk driver internal thread.*/
void *graphics_gtk_thread2(void *arg){
   struct timeval a,b;
   long diff, next;

   for(;;)
   {
      gettimeofday(&a,NULL);
      graphics_gtk_iteration();
      gettimeofday(&b,NULL);
      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
      next = graphics_gtk_cycle*1000-diff-10000;
      /* discounts 10ms taken by calling usleep itself */
      if (next>0) usleep(graphics_gtk_cycle*1000-diff);
      else;
      /* Time interval violated but don't bother with 
      any warning message, just display as fast as it can */
   }
}

/**
 * Search in the path the .glade file and loads it
 * @param file_name The name of the .glade file
 * @returns the newly created GladeXML object, or NULL on failure.
 */
GladeXML* load_glade (char * file_name){
   char *directorio, *path2, path_cp[PATH_SIZE];
   
   strncpy(path_cp, path, PATH_SIZE);
   path2=path_cp;

   while ((directorio=strsep(&path2,":"))!=NULL){
      char fichero[1024];
      strncpy(fichero, directorio, 1024);
      strncat(fichero,"/", 1024-strlen(fichero));
      strncat(fichero, file_name, 1024-strlen(fichero));
      if (access(fichero, R_OK)==0){
         /*Se puede cargar el fichero*/
         return glade_xml_new (fichero, NULL,NULL);
      }
   }
   fprintf (stderr, "graphics_gtk: I can't load the specified glade file %s\n",
            file_name);
   return NULL;
}

/**
 * @brief graphics_gtk driver init function following jdec platform API for drivers.
 *
 * It initializates the driver and exports the requiered functions.
 * 
 *  @param configfile path and name to the config file of this driver.
 */
void graphics_gtk_init(char *configfile)
{
   FILE *c_file=NULL;
   int find=0;
   char buff[512];
   
   /* Find the path in the confifile*/
   c_file=fopen(configfile, "r");
   if (c_file==NULL){
      perror("graphics_gtk");
   }

   while (fgets(buff, 512, c_file)!=NULL && !find){
      char word1[512], word2[512];
      if (sscanf (buff, "%s %s",word1, word2)==2){
         if (strcmp(word1, "path")==0){
            strcpy (path, word2);
            find=1;
         }
      }
   }

   /* Setup gtk*/
   printf ("Loading GTK support...\n");
   /* Iniciamos hilos gdk */
   g_thread_init(NULL);
   /* Iniciamos threads */
   gdk_threads_init();
   /*Inicializar gtk*/
   gtk_init(NULL, NULL);
   /* Iniciamos GtkGlExt */
   gtk_gl_init (NULL, NULL);
   /*Incializar gl*/
   gdk_gl_init (NULL, NULL);

   myexport ("graphics_gtk", "register_displaycallback", (void *)register_displaycallback);
   myexport ("graphics_gtk", "delete_displaycallback", (void *)delete_displaycallback);
   myexport ("graphics_gtk", "load_glade", (void *)load_glade);

   pthread_create(&graphics_gtk_id,NULL,graphics_gtk_thread,NULL);
   pthread_create(&graphics_gtk_id2,NULL,graphics_gtk_thread2,NULL);
   printf ("GTK support loaded.\n");
}
