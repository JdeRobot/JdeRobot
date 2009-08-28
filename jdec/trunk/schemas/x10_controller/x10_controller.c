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

#include "jde.h"
#include "x10_controller.h"
#include "graphics_gtk.h"
#include <x10.h>

#include <sys/time.h>
#include <time.h>

#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#define DELETE_STACK_ITERS 60 /*ms*/

#define MSG_LEN 70

int x10_controller_id=0;
int x10_controller_brothers[MAX_SCHEMAS];
arbitration x10_controller_callforarbitration;

/*Global variables*/

/* exported variables */
int x10_controller_cycle=30; /* ms */

/* Imported symbols*/
t_iface *x10=NULL;
runFn x10_run=NULL;
stopFn x10_stop=NULL;

int init=0;

/*GUI Variables*/
GladeXML *xml; /*Fichero xml*/
GtkWidget *win; /*Ventana*/
int context;
int n_msg=0;

registerdisplay myregister_displaycallback;
deletedisplay mydelete_displaycallback;

/*Callbacks*/
gboolean on_delete_window (GtkWidget *widget, GdkEvent *event, gpointer user_data){
   gdk_threads_leave();
   x10_controller_hide();
   gdk_threads_enter();
   return TRUE;
}

void on_property_changed(GtkComboBoxEntry *img_sel, gpointer user_data){
   char *property;
   property=(char *)gtk_combo_box_get_active_text((GtkComboBox *)img_sel);

   if (strcmp(property,"bright")==0 || strcmp(property,"dim")==0){
      gtk_widget_show((GtkWidget *)(glade_xml_get_widget(xml, "level_label")));
      gtk_widget_show((GtkWidget *)(glade_xml_get_widget(xml, "level")));
   }
   else{
      gtk_widget_hide((GtkWidget *)(glade_xml_get_widget(xml, "level_label")));
      gtk_widget_hide((GtkWidget *)(glade_xml_get_widget(xml, "level")));
   }
}

void on_apply_clicked(GtkButton *button, gpointer user_data){
   char *property;
   char *house, *item, code[5];
   char message[MSG_LEN];
   property=(char *)gtk_combo_box_get_active_text(
             (GtkComboBox *)glade_xml_get_widget(xml, "property"));
   house=(char *)gtk_combo_box_get_active_text(
             (GtkComboBox *)glade_xml_get_widget(xml, "house"));
   item=(char *)gtk_combo_box_get_active_text(
             (GtkComboBox *)glade_xml_get_widget(xml, "item_number"));
   snprintf (code, 5, "%s%s", house, item);

   if (strcmp(property,"bright")==0 || strcmp(property,"dim")==0){
      int level;
      char *s_level;
      s_level=(char *)gtk_combo_box_get_active_text(
               (GtkComboBox *)glade_xml_get_widget(xml, "level"));
      level=atoi(s_level);

      if (x10->set_property(code, property, level)!=0){
         snprintf (message, MSG_LEN, "Error launching %s, %s, %d", property, code, level);
      }
      else{
         snprintf (message, MSG_LEN, "Done %s, %s, %d", property, code, level);
      }
   }
   else{
      if (x10->set_property(code, property)!=0){
         snprintf (message, MSG_LEN, "Error launching %s, %s", property, code);
      }
      else{
         snprintf (message, MSG_LEN, "Done %s, %s", property, code);
      }
   }
   gtk_statusbar_push((GtkStatusbar *)glade_xml_get_widget(xml, "status"),
                       context,
                       message);
   n_msg++;
}


void x10_controller_iteration(){
   speedcounter(x10_controller_id);
   /*if (x10->set_property("a3", "on")!=0){
      printf ("Error en on\n");
   }
   else{
      printf ("Encendido\n");
      sleep(1);
   }
   if (x10->set_property("a3", "dim", 15)!=0){
      printf ("Error en dim\n");
   }
   else{
      printf ("Bajada la intensidad\n");
      sleep(1);
   }
   if (x10->set_property("a3", "bright", 10)!=0){
      printf ("Error en bright\n");
   }
   else{
      printf ("Subida la intensidad\n");
      sleep(1);
   }
   if (x10->set_property("a3", "off")!=0){
      printf ("Error en off\n");
   }
   else{
      printf ("Apagado\n");
   }*/
}

/*Importar símbolos*/
void x10_controller_imports(){
   x10=(t_iface *)myimport ("x10","x10");
   x10_run=(runFn)myimport ("x10","run");
   x10_stop=(stopFn)myimport ("x10","stop");

   if (!x10 || !x10_run || !x10_stop){
      fprintf(stderr, "x10_controller ERROR: I can't import symbols from x10 driver\n");
      jdeshutdown(-1);
   }
}

/*Exportar símbolos*/
void x10_controller_exports(){

   myexport("x10_controller", "id", &x10_controller_id);
   myexport("x10_controller","cycle",&x10_controller_cycle);
   myexport("x10_controller","run",(void *)x10_controller_run);
   myexport("x10_controller","stop",(void *)x10_controller_stop);
}

/*Las inicializaciones van en esta parte*/
void x10_controller_guiinit(){
   if (myregister_displaycallback==NULL){
      if ((myregister_displaycallback=
           (registerdisplay)myimport ("graphics_gtk",
            "register_displaycallback"))==NULL)
      {
         printf ("I can't fetch register_displaycallback from graphics_gtk\n");
         jdeshutdown(1);
      }
      if ((mydelete_displaycallback=
           (deletedisplay)myimport ("graphics_gtk", "delete_displaycallback"))
           ==NULL)
      {
         printf ("I can't fetch delete_displaycallback from graphics_gtk\n");
         jdeshutdown(1);
      }
   }
}

void x10_controller_terminate(){
}

void x10_controller_stop()
{
  pthread_mutex_lock(&(all[x10_controller_id].mymutex));
  put_state(x10_controller_id,slept);
  printf("x10_controller: off\n");
  pthread_mutex_unlock(&(all[x10_controller_id].mymutex));
  x10_stop();
  init=0;
}


void x10_controller_run(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[x10_controller_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[x10_controller_id].mymutex));
  /* this schema runs its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[x10_controller_id].children[i]=FALSE;
  all[x10_controller_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) x10_controller_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {x10_controller_brothers[i]=brothers[i];i++;}
    }
  x10_controller_callforarbitration=fn;
  put_state(x10_controller_id,notready);
  printf("x10_controller: on\n");
  pthread_cond_signal(&(all[x10_controller_id].condition));
  pthread_mutex_unlock(&(all[x10_controller_id].mymutex));
  x10_controller_imports();
  x10_run(x10_controller_id, NULL, NULL);
}

void *x10_controller_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;

   for(;;)
   {
      pthread_mutex_lock(&(all[x10_controller_id].mymutex));

      if (all[x10_controller_id].state==slept)
      {
	 pthread_cond_wait(&(all[x10_controller_id].condition),&(all[x10_controller_id].mymutex));
	 pthread_mutex_unlock(&(all[x10_controller_id].mymutex));
      }
      else
      {
	 /* check preconditions. For now, preconditions are always satisfied*/
	 if (all[x10_controller_id].state==notready)
	    put_state(x10_controller_id,ready);
	 /* check brothers and arbitrate. For now this is the only winner */
	 if (all[x10_controller_id].state==ready)
	 {put_state(x10_controller_id,winner);
	 gettimeofday(&a,NULL);
	 aa=a.tv_sec*1000000+a.tv_usec;
	 n=0;
	 }

	 if (all[x10_controller_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	 {
	    pthread_mutex_unlock(&(all[x10_controller_id].mymutex));
	    /*      gettimeofday(&a,NULL);*/
	    n++;
	    x10_controller_iteration();
	    gettimeofday(&b,NULL);
	    bb=b.tv_sec*1000000+b.tv_usec;
	    next=aa+(n+1)*(long)x10_controller_cycle*1000-bb;

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
	    pthread_mutex_unlock(&(all[x10_controller_id].mymutex));
	    usleep(x10_controller_cycle*1000);
	 }
      }
   }
}

void x10_controller_init(char *configfile)
{
  pthread_mutex_lock(&(all[x10_controller_id].mymutex));
  printf("x10_controller schema started up\n");
  x10_controller_exports();
  put_state(x10_controller_id,slept);
  pthread_create(&(all[x10_controller_id].mythread),NULL,x10_controller_thread,NULL);
  pthread_mutex_unlock(&(all[x10_controller_id].mymutex));
  x10_controller_guiinit();
}

void x10_controller_guidisplay(){
   static int old_n_msg=0;
   static int iters=0;
   int i;
   gdk_threads_enter();
   if (n_msg!=0){
      if (old_n_msg==n_msg){
         iters++;
         if (iters>DELETE_STACK_ITERS){
            for (i=0; i<n_msg; i++){
               gtk_statusbar_pop((GtkStatusbar *)glade_xml_get_widget(xml, "status"),
                                  context);
            }
            iters=0;
            n_msg=0;
         }
      }
      else{
         iters=0;
      }
   }
   else{
      iters=0;
   }
   old_n_msg=n_msg;
   gdk_threads_leave();
}

void x10_controller_hide(void){
   mydelete_displaycallback(x10_controller_guidisplay);
   if (win!=NULL){
      gdk_threads_enter();
      gtk_widget_hide(win);
      gdk_threads_leave();
   }
   all[x10_controller_id].guistate=pending_off;
}

void x10_controller_show(void){
   static int cargado=0;
   static pthread_mutex_t x10_controller_gui_mutex;

   pthread_mutex_lock(&x10_controller_gui_mutex);
   if (!cargado){
      loadglade ld_fn;
      cargado=1;
      pthread_mutex_unlock(&x10_controller_gui_mutex);
      /*Cargar la ventana desde el archivo xml .glade*/
      gdk_threads_enter();
      if ((ld_fn=(loadglade)myimport("graphics_gtk","load_glade"))==NULL){
         fprintf (stderr,"I can't fetch 'load_glade' from 'graphics_gtk'.\n");
         jdeshutdown(1);
      }
      xml = ld_fn ("x10_controller.glade");
      if (xml==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }
      win = glade_xml_get_widget(xml, "window1");
      /*Conectar los callbacks*/
      {
         g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "property")), "changed",
                          G_CALLBACK(on_property_changed), NULL);
         g_signal_connect(G_OBJECT(win), "delete-event",
                          G_CALLBACK(on_delete_window), NULL);
         g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "apply")), "clicked",
                          G_CALLBACK(on_apply_clicked), NULL);
      }
      
      gtk_widget_hide((GtkWidget *)(glade_xml_get_widget(xml, "level_label")));
      gtk_widget_hide((GtkWidget *)(glade_xml_get_widget(xml, "level")));

      context=gtk_statusbar_get_context_id ((GtkStatusbar *)
            glade_xml_get_widget(xml, "status"),
                                 "main context");
      gtk_statusbar_push((GtkStatusbar *)glade_xml_get_widget(xml, "status"),
                          context,
                          "Please select an action");
      
      if (win==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }
      else{
         gtk_widget_show(win);
         gtk_widget_queue_draw(GTK_WIDGET(win));
      }
      gdk_threads_leave();
   }
   else{
      pthread_mutex_unlock(&x10_controller_gui_mutex);
      gdk_threads_enter();
      gtk_widget_show(win);
      gtk_widget_queue_draw(GTK_WIDGET(win));
      gdk_threads_leave();
   }

   myregister_displaycallback(x10_controller_guidisplay);
   all[x10_controller_id].guistate=pending_on;
}

