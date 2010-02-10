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
#include "wiimote_viewer.h"
#include "graphics_gtk.h"

#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "wiimote.h"

int wiimote_viewer_id=0;
int wiimote_viewer_brothers[MAX_SCHEMAS];
arbitration wiimote_viewer_callforarbitration;

registerdisplay myregister_displaycallback;
deletedisplay mydelete_displaycallback;

/*Global variables*/
unsigned short *buttons=NULL;
unsigned char *acc=NULL;
int *buttons_id=NULL;
int *acc_id=NULL;
resumeFn buttons_resume=NULL;
suspendFn buttons_suspend=NULL;
resumeFn acc_resume=NULL;
suspendFn acc_suspend=NULL;

int buttons_ok=0;
int acc_ok=0;

int show_buttons=0;
int show_acc=0;


/* exported variables */
int wiimote_viewer_cycle=40; /* ms */

/*GUI Variables*/
GladeXML *xml; /*Fichero xml*/
GtkWidget *win; /*Ventana*/

/*Callbacks*/
gboolean on_delete_window (GtkWidget *widget, GdkEvent *event, gpointer user_data){
   gtk_widget_hide(widget);
   gtk_widget_queue_draw(widget);
   all[wiimote_viewer_id].guistate=pending_off;
   return TRUE;
}

void on_active_buttons_clicked (GtkToggleButton *button, gpointer user_data){
   if (buttons_ok){
      if (gtk_toggle_button_get_active(button)){
         buttons_resume(wiimote_viewer_id, NULL, NULL);
         show_buttons=1;
      }
      else{
         buttons_suspend();
         show_buttons=0;
      }
   }
}

void on_active_accs_clicked (GtkToggleButton *button, gpointer user_data){
   if (acc_ok){
      if (gtk_toggle_button_get_active(button)){
         acc_resume(wiimote_viewer_id, NULL, NULL);
         show_acc=1;
      }
      else{
         acc_suspend();
         show_acc=0;
      }
   }
}

void wiimote_viewer_iteration(){

}

/*Importar símbolos*/
void wiimote_viewer_imports(){
   buttons=(unsigned short *)myimport("wii_buttons", "buttons0");
   acc=(unsigned char *)myimport("wii_accel", "accel0");
   buttons_id=(int *)myimport("wii_buttons", "id");
   acc_id=(int *)myimport("wii_accel", "id");;
   buttons_resume=(resumeFn)myimport("wii_buttons", "resume");
   buttons_suspend=(suspendFn)myimport("wii_buttons", "suspend");
   acc_resume=(resumeFn )myimport("wii_accel", "resume");
   acc_suspend=(suspendFn )myimport("wii_accel", "suspend");
   buttons_ok=!(buttons_resume==NULL || buttons_suspend==NULL || buttons==NULL || buttons_id==NULL);
   if (!buttons_ok){
      printf ("Buttons will not be displayed\n");
   }
   acc_ok=!(acc_resume==NULL || acc_suspend==NULL || acc==NULL || acc_id==NULL);
   if (!acc_ok){
      printf ("Accelerators will not be displayed\n");
   }
}

/*Exportar símbolos*/
void wiimote_viewer_exports(){

   myexport("wiimote_viewer","cycle",&wiimote_viewer_cycle);
   myexport("wiimote_viewer","resume",(void *)wiimote_viewer_resume);
   myexport("wiimote_viewer","suspend",(void *)wiimote_viewer_suspend);
}

/*Las inicializaciones van en esta parte*/
void wiimote_viewer_init(){
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

/*Al suspender el esquema*/
void wiimote_viewer_end(){
}

void wiimote_viewer_stop(){
}

void wiimote_viewer_suspend()
{
  pthread_mutex_lock(&(all[wiimote_viewer_id].mymutex));
  put_state(wiimote_viewer_id,slept);
  printf("wiimote_viewer: off\n");
  pthread_mutex_unlock(&(all[wiimote_viewer_id].mymutex));
  wiimote_viewer_end();
}


void wiimote_viewer_resume(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[wiimote_viewer_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[wiimote_viewer_id].mymutex));
  /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[wiimote_viewer_id].children[i]=FALSE;
  all[wiimote_viewer_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) wiimote_viewer_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {wiimote_viewer_brothers[i]=brothers[i];i++;}
    }
  wiimote_viewer_callforarbitration=fn;
  put_state(wiimote_viewer_id,notready);
  printf("wiimote_viewer: on\n");
  pthread_cond_signal(&(all[wiimote_viewer_id].condition));
  pthread_mutex_unlock(&(all[wiimote_viewer_id].mymutex));
  wiimote_viewer_imports();
}

void *wiimote_viewer_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;

   for(;;)
   {
      pthread_mutex_lock(&(all[wiimote_viewer_id].mymutex));

      if (all[wiimote_viewer_id].state==slept)
      {
	 pthread_cond_wait(&(all[wiimote_viewer_id].condition),&(all[wiimote_viewer_id].mymutex));
	 pthread_mutex_unlock(&(all[wiimote_viewer_id].mymutex));
      }
      else
      {
	 /* check preconditions. For now, preconditions are always satisfied*/
	 if (all[wiimote_viewer_id].state==notready)
	    put_state(wiimote_viewer_id,ready);
	 /* check brothers and arbitrate. For now this is the only winner */
	 if (all[wiimote_viewer_id].state==ready)
	 {put_state(wiimote_viewer_id,winner);
	 gettimeofday(&a,NULL);
	 aa=a.tv_sec*1000000+a.tv_usec;
	 n=0;
	 }

	 if (all[wiimote_viewer_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	 {
	    pthread_mutex_unlock(&(all[wiimote_viewer_id].mymutex));
	    /*      gettimeofday(&a,NULL);*/
	    n++;
	    wiimote_viewer_iteration();
	    gettimeofday(&b,NULL);
	    bb=b.tv_sec*1000000+b.tv_usec;
	    next=aa+(n+1)*(long)wiimote_viewer_cycle*1000-bb;

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
	    pthread_mutex_unlock(&(all[wiimote_viewer_id].mymutex));
	    usleep(wiimote_viewer_cycle*1000);
	 }
      }
   }
}

void wiimote_viewer_startup()
{
  pthread_mutex_lock(&(all[wiimote_viewer_id].mymutex));
  printf("wiimote_viewer schema started up\n");
  wiimote_viewer_exports();
  put_state(wiimote_viewer_id,slept);
  pthread_create(&(all[wiimote_viewer_id].mythread),NULL,wiimote_viewer_thread,NULL);
  pthread_mutex_unlock(&(all[wiimote_viewer_id].mymutex));
  wiimote_viewer_init();
}

void wiimote_viewer_guidisplay(){
   /*Check wiimote status and display it*/
   gdk_threads_enter();
   if (show_buttons){
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btB")),
                           (*buttons & CWIID_BTN_B));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btUp")),
                            (*buttons & CWIID_BTN_UP));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btDown")),
                           (*buttons & CWIID_BTN_DOWN));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btLeft")),
                           (*buttons & CWIID_BTN_LEFT));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btRight")),
                           (*buttons & CWIID_BTN_RIGHT));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btA")),
                           (*buttons & CWIID_BTN_A));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btminus")),
                           (*buttons & CWIID_BTN_MINUS));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btplus")),
                           (*buttons & CWIID_BTN_PLUS));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btHome")),
                           (*buttons & CWIID_BTN_HOME));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "bt1")),
                           (*buttons & CWIID_BTN_1));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "bt2")),
                           (*buttons & CWIID_BTN_2));
   }
   else{
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btB")),
                                   FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btUp")),
                                   FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btDown")),
                                   FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btLeft")),
                                   FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btRight")),
                                   FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btA")),
                                   FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btminus")),
                                   FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btplus")),
                                   FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btHome")),
                                   FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "bt1")),
                                   FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "bt2")),
                                   FALSE);
   }
   if (show_acc){
      gtk_progress_bar_set_fraction ((GtkProgressBar *)glade_xml_get_widget(xml, "accX"),
                                      (float)acc[CWIID_X]/(float)CWIID_ACC_MAX);
      gtk_progress_bar_set_fraction ((GtkProgressBar *)glade_xml_get_widget(xml, "accY"),
                                      (float)acc[CWIID_Y]/(float)CWIID_ACC_MAX);
      gtk_progress_bar_set_fraction ((GtkProgressBar *)glade_xml_get_widget(xml, "accZ"),
                                      (float)acc[CWIID_Z]/(float)CWIID_ACC_MAX);
   }
   else{
      gtk_progress_bar_set_fraction ((GtkProgressBar *)glade_xml_get_widget(xml, "accX"),
                                      0.0);
      gtk_progress_bar_set_fraction ((GtkProgressBar *)glade_xml_get_widget(xml, "accY"),
                                      0.0);
      gtk_progress_bar_set_fraction ((GtkProgressBar *)glade_xml_get_widget(xml, "accZ"),
                                      0.0);
   }
   gtk_widget_queue_draw(win);
   gdk_threads_leave();
}


void wiimote_viewer_guisuspend(void){
   if (win!=NULL){
      gdk_threads_enter();
      gtk_widget_hide(win);
      gdk_threads_leave();
   }
   mydelete_displaycallback(wiimote_viewer_guidisplay);
}

void wiimote_viewer_guiresume(void){
   static int cargado=0;
   static pthread_mutex_t wiimote_viewer_gui_mutex;

   pthread_mutex_lock(&wiimote_viewer_gui_mutex);
   if (!cargado){
      loadglade ld_fn;
      cargado=1;
      pthread_mutex_unlock(&wiimote_viewer_gui_mutex);
      /*Cargar la ventana desde el archivo xml .glade*/
      gdk_threads_enter();
      if ((ld_fn=(loadglade)myimport("graphics_gtk","load_glade"))==NULL){
         fprintf (stderr,"I can't fetch 'load_glade' from 'graphics_gtk'.\n");
         jdeshutdown(1);
      }
      xml = ld_fn ("wiimote_viewer.glade");
      if (xml==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }
      win = glade_xml_get_widget(xml, "window");
      /*Conectar los callbacks*/
      {
         g_signal_connect(G_OBJECT(win), "delete-event",
                          G_CALLBACK(on_delete_window), NULL);
         g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "active_buttons")),
                          "clicked", G_CALLBACK(on_active_buttons_clicked), NULL);
         g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "active_accs")),
                          "clicked", G_CALLBACK(on_active_accs_clicked), NULL);
      }
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
      pthread_mutex_unlock(&wiimote_viewer_gui_mutex);
      gdk_threads_enter();
      gtk_widget_show(win);
      gtk_widget_queue_draw(GTK_WIDGET(win));
      gdk_threads_leave();
   }

   myregister_displaycallback(wiimote_viewer_guidisplay);
}

