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
#include <graphics_gtk.h>

#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <interfaces/wiimote.h>

int wiimote_viewer_id=0;
int wiimote_viewer_brothers[MAX_SCHEMAS];
arbitration wiimote_viewer_callforarbitration;

registerdisplay myregister_displaycallback;
deletedisplay mydelete_displaycallback;

/*Global variables*/
unsigned short *buttons=NULL;
unsigned char *acc=NULL;
struct nunchuk_state *nunchuk=NULL;

int *buttons_id=NULL;
int *acc_id=NULL;
int *nunchuk_id=NULL;

runFn buttons_run=NULL;
stopFn buttons_stop=NULL;

runFn acc_run=NULL;
stopFn acc_stop=NULL;

runFn nunchuk_run=NULL;
stopFn nunchuk_stop=NULL;

int buttons_ok=0;
int acc_ok=0;
int nunchuk_ok=0;

int show_buttons=0;
int show_acc=0;
int show_nunchuck=0;


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

void on_active_buttons_toggled (GtkCheckMenuItem *menu_item, gpointer user_data){
   if (buttons_ok){
      if (gtk_check_menu_item_get_active(menu_item)){
         buttons_run(wiimote_viewer_id, NULL, NULL);
         show_buttons=1;
         gtk_widget_show(glade_xml_get_widget(xml, "buttons"));
         gtk_widget_show(glade_xml_get_widget(xml, "buttons2"));
      }
      else{
         buttons_stop();
         show_buttons=0;
         gtk_widget_hide(glade_xml_get_widget(xml, "buttons"));
         gtk_widget_hide(glade_xml_get_widget(xml, "buttons2"));
      }
   }
}

void on_active_accs_toggled (GtkCheckMenuItem *menu_item, gpointer user_data){
   if (acc_ok){
      if (gtk_check_menu_item_get_active(menu_item)){
         acc_run(wiimote_viewer_id, NULL, NULL);
         show_acc=1;
         gtk_widget_show(glade_xml_get_widget(xml, "acc"));
         gtk_widget_show(glade_xml_get_widget(xml, "acc2"));
      }
      else{
         acc_stop();
         show_acc=0;
         gtk_widget_hide(glade_xml_get_widget(xml, "acc"));
         gtk_widget_hide(glade_xml_get_widget(xml, "acc2"));
      }
   }
}

void on_active_nunchuk_toggled (GtkCheckMenuItem *menu_item, gpointer user_data){
   if (nunchuk_ok){
      if (gtk_check_menu_item_get_active(menu_item)){
         nunchuk_run(wiimote_viewer_id, NULL, NULL);
         show_nunchuck=1;
         gtk_widget_show(glade_xml_get_widget(xml, "nunchuk"));
         gtk_widget_show(glade_xml_get_widget(xml, "nunchuk2"));
      }
      else{
         nunchuk_stop();
         show_nunchuck=0;
         gtk_widget_hide(glade_xml_get_widget(xml, "nunchuk"));
         gtk_widget_hide(glade_xml_get_widget(xml, "nunchuk2"));
      }
   }
}

void wiimote_viewer_iteration(){
   if (show_nunchuck){
      /*
      min_x= 23
      max_x= 229
      min_y= 28
      max_y= 225
      */

   }
}

/*Importar símbolos*/
void wiimote_viewer_imports(){
   buttons=(unsigned short *)myimport("wii_buttons", "buttons0");
   acc=(unsigned char *)myimport("wii_accel", "accel0");
   nunchuk=(struct nunchuk_state*)myimport("wii_nunchuk", "nunchuk0");
   
   buttons_id=(int *)myimport("wii_buttons", "id");
   acc_id=(int *)myimport("wii_accel", "id");
   nunchuk_id=(int *)myimport("wii_nunchuk", "id");
   
   buttons_run=(runFn)myimport("wii_buttons", "run");
   buttons_stop=(stopFn)myimport("wii_buttons", "stop");
   
   acc_run=(runFn )myimport("wii_accel", "run");
   acc_stop=(stopFn )myimport("wii_accel", "stop");
   
   nunchuk_run=(runFn )myimport("wii_nunchuk", "run");
   nunchuk_stop=(stopFn )myimport("wii_nunchuk", "stop");
   
   buttons_ok=!(buttons_run==NULL || buttons_stop==NULL || buttons==NULL || buttons_id==NULL);
   if (!buttons_ok){
      printf ("Buttons will not be displayed\n");
   }
   acc_ok=!(acc_run==NULL || acc_stop==NULL || acc==NULL || acc_id==NULL);
   if (!acc_ok){
      printf ("Accelerators will not be displayed\n");
   }
   nunchuk_ok=!(nunchuk_run==NULL || nunchuk_stop==NULL || nunchuk==NULL || nunchuk_id==NULL);
   if (!nunchuk_ok){
      printf ("Accelerators will not be displayed\n");
   }
}

/*Exportar símbolos*/
void wiimote_viewer_exports(){

   myexport("wiimote_viewer","cycle",&wiimote_viewer_cycle);
   myexport("wiimote_viewer","run",(void *)wiimote_viewer_run);
   myexport("wiimote_viewer","stop",(void *)wiimote_viewer_stop);
}

/*Las inicializaciones van en esta parte*/
void wiimote_viewer_guiinit(){
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


void wiimote_viewer_terminate(){
}

void wiimote_viewer_stop()
{
  pthread_mutex_lock(&(all[wiimote_viewer_id].mymutex));
  put_state(wiimote_viewer_id,slept);
  printf("wiimote_viewer: off\n");
  pthread_mutex_unlock(&(all[wiimote_viewer_id].mymutex));
}


void wiimote_viewer_run(int father, int *brothers, arbitration fn)
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
  /* this schema runs its execution with no children at all */
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

void wiimote_viewer_init(char *configfile)
{
  pthread_mutex_lock(&(all[wiimote_viewer_id].mymutex));
  printf("wiimote_viewer schema started up\n");
  wiimote_viewer_exports();
  put_state(wiimote_viewer_id,slept);
  pthread_create(&(all[wiimote_viewer_id].mythread),NULL,wiimote_viewer_thread,NULL);
  pthread_mutex_unlock(&(all[wiimote_viewer_id].mymutex));
  wiimote_viewer_guiinit();
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
   if(show_nunchuck){
      gtk_progress_bar_set_fraction ((GtkProgressBar *)glade_xml_get_widget(xml, "nunchuk_accX"),
                                      (float)((*nunchuk).acc[CWIID_X])/(float)CWIID_ACC_MAX);
      gtk_progress_bar_set_fraction ((GtkProgressBar *)glade_xml_get_widget(xml, "nunchuk_accY"),
                                      (float)((*nunchuk).acc[CWIID_Y])/(float)CWIID_ACC_MAX);
      gtk_progress_bar_set_fraction ((GtkProgressBar *)glade_xml_get_widget(xml, "nunchuk_accZ"),
                                      (float)((*nunchuk).acc[CWIID_Z])/(float)CWIID_ACC_MAX);

      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btC")),
                                   ((*nunchuk).buttons & CWIID_NUNCHUK_BTN_C));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btZ")),
                                   ((*nunchuk).buttons & CWIID_NUNCHUK_BTN_Z));
      {
         char label[5];
         snprintf (label, 5, "%d", (*nunchuk).stick[CWIID_X]);
         gtk_label_set_text((GtkLabel *)glade_xml_get_widget(xml, "nunchuk_x"), label);
         snprintf (label, 5, "%d", (*nunchuk).stick[CWIID_Y]);
         gtk_label_set_text((GtkLabel *)glade_xml_get_widget(xml, "nunchuk_y"), label);
      }
   }
   else{
      gtk_progress_bar_set_fraction ((GtkProgressBar *)glade_xml_get_widget(xml, "nunchuk_accX"),
                                      0.0);
      gtk_progress_bar_set_fraction ((GtkProgressBar *)glade_xml_get_widget(xml, "nunchuk_accY"),
                                      0.0);
      gtk_progress_bar_set_fraction ((GtkProgressBar *)glade_xml_get_widget(xml, "nunchuk_accZ"),
                                      0.0);
      
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btC")),
                                   FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "btZ")),
                                   FALSE);
   }
   gtk_window_resize (GTK_WINDOW(win),1,1);
   gtk_widget_queue_draw(win);
   gdk_threads_leave();
}


void wiimote_viewer_hide(void){
   if (win!=NULL){
      gdk_threads_enter();
      gtk_widget_hide(win);
      gdk_threads_leave();
   }
   mydelete_displaycallback(wiimote_viewer_guidisplay);
}

void wiimote_viewer_show(void){
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
                          "toggled", G_CALLBACK(on_active_buttons_toggled), NULL);
         g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "active_accs")),
                          "toggled", G_CALLBACK(on_active_accs_toggled), NULL);
         g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "active_nunchuk")),
                          "toggled", G_CALLBACK(on_active_nunchuk_toggled), NULL);
      }

      /*Hide frames until they are displayed*/
      gtk_widget_hide(glade_xml_get_widget(xml, "nunchuk"));
      gtk_widget_hide(glade_xml_get_widget(xml, "nunchuk2"));
      gtk_widget_hide(glade_xml_get_widget(xml, "acc"));
      gtk_widget_hide(glade_xml_get_widget(xml, "acc2"));
      gtk_widget_hide(glade_xml_get_widget(xml, "buttons"));
      gtk_widget_hide(glade_xml_get_widget(xml, "buttons2"));

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

