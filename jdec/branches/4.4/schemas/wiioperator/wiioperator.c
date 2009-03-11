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
 *  Authors : Eduardo Perdices García <edupergar@gmail.com>
 */

#include <jde.h>
#include <stdlib.h>
#include <wiioperator.h>
#include <graphics_gtk.h>
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <interfaces/wiimote.h>

/* MOTORS CONSTANTS */
#define V_MAX 1600
#define V_MIN 100
#define W_MAX 30
#define W_MIN 8
#define V_STEP 10.0
#define W_STEP 1.0
#define SECURITY_MARGIN 10
#define SECURITY_MARGIN_ACC 3

/* PANTILT CONSTANTS */
#define LAT_STEP 5.0
#define LON_STEP 5.0

/* ACC CONSTANTS */
#define Y_MIN 110
#define Y_MAX 140
#define X_MIN 110
#define X_MAX 140

int wiioperator_id=0;
int wiioperator_brothers[MAX_SCHEMAS];
arbitration wiioperator_callforarbitration;

registerdisplay myregister_displaycallback;
deletedisplay mydelete_displaycallback;

/*Global variables*/

int controles_ok = 0;
int motors_ok = 1;
int buttons_ok = 1;
int acc_ok = 1;
int ptmotors_ok = 1;
int state = 1;
int my_v=300;
int my_w=20;

/* exported variables */
int wiioperator_cycle=30; /* ms */

/* Imported symbols*/
unsigned short *buttons=NULL;
unsigned char *acc=NULL;

runFn buttons_run=NULL;
stopFn buttons_stop=NULL;
runFn acc_run=NULL;
stopFn acc_stop=NULL;

float *v=NULL;
float *w=NULL;

runFn motors_run=NULL;
stopFn motors_stop=NULL;

float *longitude=NULL;
float *latitude=NULL;
float *pan=NULL;
float *tilt=NULL;
float *min_latitude=NULL;
float *max_latitude=NULL;
float *min_longitude=NULL;
float *max_longitude=NULL;

runFn ptmotors_run=NULL;
stopFn ptmotors_stop=NULL;
runFn ptencoders_run=NULL;
stopFn ptencoders_stop=NULL;

/*GUI Variables*/

GladeXML *xml;
GtkWidget *win;

/*Callbacks*/

gboolean on_delete_window (GtkWidget *widget, GdkEvent *event, gpointer user_data){
   gtk_widget_hide(widget);
   gtk_widget_queue_draw(widget);
   all[wiioperator_id].guistate=pending_off;
   return TRUE;
}


void on_active_buttons_toggled (GtkCheckMenuItem *menu_item, gpointer user_data){
	if (!buttons_ok){
		buttons_run(wiioperator_id, NULL, NULL);
		buttons_ok = 1;
	}
	else{
		buttons_stop();
		buttons_ok = 0;
	}
}

void on_active_motors_toggled (GtkCheckMenuItem *menu_item, gpointer user_data){
	if (!motors_ok){
		motors_run(wiioperator_id, NULL, NULL);
		motors_ok = 1;
	}
	else{
		motors_stop();
		motors_ok = 0;
	}
}

void on_active_acc_toggled (GtkCheckMenuItem *menu_item, gpointer user_data){
	if (!acc_ok){
		acc_run(wiioperator_id, NULL, NULL);
		acc_ok = 1;
	}
	else{
		acc_stop();
		acc_ok = 0;
	}
}

void on_active_pantilt_toggled (GtkCheckMenuItem *menu_item, gpointer user_data){
	if (!ptmotors_ok){
		ptmotors_run(wiioperator_id, NULL, NULL);
		ptmotors_ok = 1;
	}
	else{
		ptmotors_stop();
		ptmotors_ok = 0;
	}
}

void wiioperator_iteration(){
   int ch_v=0, ch_w=0;

   speedcounter(wiioperator_id);

	if(controles_ok) {

		if(buttons_ok) {
			if(*buttons & CWIID_BTN_A)
				state = 1;
			else if(*buttons & CWIID_BTN_B)
				state = 2;
		}

		/*	State 1: Motors -> Arrows, Pantilt -> Accel
			State 2: Motors -> Accel, Pantilt -> Arrows*/

		if (state == 1) {
			/*Motors*/
			if(buttons_ok) {
				if (*buttons & CWIID_BTN_UP){
					*v=my_v;
					ch_v=1;
				}
				else if (*buttons & CWIID_BTN_DOWN){
					*v=-my_v;
					ch_v=1;
				}

				if (*buttons & CWIID_BTN_LEFT){
					*w=my_w;
					ch_w=1;
				}
				if (*buttons & CWIID_BTN_RIGHT){
					*w=-my_w;
					ch_w=1;
				}
			}

			/*Stop motors*/
			if (!ch_v || !buttons_ok)
				*v=0;
			if (!ch_w || !buttons_ok)
				*w=0;

			/*Pantilt*/
			if(acc_ok) {
				if (acc[CWIID_Y] < Y_MIN) {
					if((*latitude - LAT_STEP) > ((*min_latitude) + SECURITY_MARGIN_ACC))
						*latitude = *latitude - LAT_STEP;
				} else if(acc[CWIID_Y] > Y_MAX) {
					if((*latitude + LAT_STEP) < ((*max_latitude) - SECURITY_MARGIN_ACC))
						*latitude = *latitude + LAT_STEP;
				}
				if (acc[CWIID_X] < X_MIN) {
					if((*longitude - LON_STEP) > ((*min_longitude) + SECURITY_MARGIN*2))
						*longitude = *longitude - LON_STEP;
				}else if(acc[CWIID_X] > X_MAX) {
					if((*longitude + LON_STEP) < ((*max_longitude) - SECURITY_MARGIN*2))
						*longitude = *longitude + LON_STEP;
				}
			}
		} else {
			/*Pantilt*/
			if(buttons_ok) {
					
				if (*buttons & CWIID_BTN_UP) {
					if(((*tilt) + LAT_STEP) < ((*max_latitude) - SECURITY_MARGIN*2))
						(*latitude) = (*latitude) + LAT_STEP;
				} else if (*buttons & CWIID_BTN_DOWN) {
					if(((*tilt) - LAT_STEP) > ((*min_latitude) + SECURITY_MARGIN*2))
						(*latitude) = (*latitude) - LAT_STEP;
				}
				if (*buttons & CWIID_BTN_LEFT) {
					if(((*pan) - LON_STEP) > ((*min_longitude) + SECURITY_MARGIN*2))
						(*longitude) = (*longitude) - LON_STEP;
				} else if (*buttons & CWIID_BTN_RIGHT) {
					if (((*pan) + LON_STEP) < ((*max_longitude) - SECURITY_MARGIN*2));
						(*longitude) = (*longitude) + LON_STEP;
				}
			}

			/*Motors*/
			if(acc_ok) {
				if (acc[CWIID_Y] < Y_MIN) {
					*v=-my_v;
					ch_v=1;
				} else if(acc[CWIID_Y] > Y_MAX) {
					*v=my_v;
					ch_v=1;
				}

				if (acc[CWIID_X] < X_MIN) {
					*w=my_w;
					ch_w=1;
				} else if(acc[CWIID_X] > X_MAX) {
					*w=-my_w;
					ch_w=1;
				}
			}
			/*Stop motors*/
			if (!ch_v || !acc_ok)
				*v=0;
			if (!ch_w || !acc_ok)
				*w=0;
		}
			
		/*Increment or decrement speeds*/
		if(buttons_ok) {
			if (*buttons & CWIID_BTN_PLUS){
				my_v+=V_STEP;
				if (my_v > V_MAX)
				   my_v=V_MAX;
			}
			if (*buttons & CWIID_BTN_MINUS){
				my_v-=V_STEP;
				if (my_v < V_MIN)
				   my_v=V_MIN;
			}
			if (*buttons & CWIID_BTN_1){
				my_w+=W_STEP;
				if (my_w > W_MAX)
				   my_w=W_MAX;
			}
			if (*buttons & CWIID_BTN_2){
				my_w-=W_STEP;
				if (my_w < W_MIN)
				   my_w=W_MIN;
			}
		}
	}
}

void wiioperator_imports(){
   buttons=(unsigned short *)myimport ("wii_buttons","buttons0");
   buttons_run=(runFn)myimport ("wii_buttons","run");
   buttons_stop=(stopFn)myimport ("wii_buttons","stop");

   acc=(unsigned char *)myimport("wii_accel", "accel0");
   acc_run=(runFn )myimport("wii_accel", "run");
   acc_stop=(stopFn )myimport("wii_accel", "stop");

   v=myimport("motors","v");
   w=myimport("motors","w");
   motors_run=(runFn)myimport("motors","run");
   motors_stop=(stopFn)myimport("motors","stop");

   longitude=myimport("ptmotors","longitude");
   latitude=myimport("ptmotors","latitude");
   min_longitude=myimport("ptmotors","min_longitude");
   max_longitude=myimport("ptmotors","max_longitude");
   min_latitude=myimport("ptmotors","min_latitude");
   max_latitude=myimport("ptmotors","max_latitude");
   ptmotors_run=(runFn)myimport("ptmotors","run");
   ptmotors_stop=(stopFn)myimport("ptmotors","stop");

   pan=myimport("ptencoders","pan_angle");
   tilt=myimport("ptencoders","tilt_angle");
   ptencoders_run=(runFn)myimport("ptencoders","run");
   ptencoders_stop=(stopFn)myimport("ptencoders","stop");

	if (!buttons || !buttons_run || !buttons_stop)
	{
		fprintf (stderr, "ERROR: I can't import necessary symbols from wii_buttons\n");
		jdeshutdown(-1);
	}

   if (!acc || !acc_run || !acc_stop) {
		fprintf (stderr, "ERROR: I can't import necessary symbols from wii_accel\n");
		jdeshutdown(-1);
   }

   if ( !v || !w || !motors_run || !motors_stop){
      fprintf (stderr, "ERROR: I can't import necessary symbols from motors\n");
      jdeshutdown(-1);
   }

   if ( 	!longitude || !latitude || !ptmotors_run || !ptmotors_stop ||
			!min_longitude || !max_longitude || !min_latitude || !max_latitude){
      fprintf (stderr, "ERROR: I can't import necessary symbols from ptmotors\n");
      jdeshutdown(-1);
   }

   if ( 	!pan || !tilt || !ptencoders_run || !ptencoders_stop ){
      fprintf (stderr, "ERROR: I can't import necessary symbols from ptencoders\n");
      jdeshutdown(-1);
   }

	controles_ok=1;
}

void wiioperator_exports(){

   myexport("wiioperator", "id", &wiioperator_id);
   myexport("wiioperator","cycle",&wiioperator_cycle);
   myexport("wiioperator","run",(void *)wiioperator_run);
   myexport("wiioperator","stop",(void *)wiioperator_stop);
}

void wiioperator_stop()
{
 
}

void wiioperator_guiinit(){
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


void wiioperator_terminate()
{
  pthread_mutex_lock(&(all[wiioperator_id].mymutex));
  put_state(wiioperator_id,slept);
  printf("wiioperator: off\n");
  pthread_mutex_unlock(&(all[wiioperator_id].mymutex));
  wiioperator_end();
  buttons_stop();
  motors_stop();
  ptmotors_stop();
  ptencoders_stop();

}


void wiioperator_run(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[wiioperator_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[wiioperator_id].mymutex));
  /* this schema runs its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[wiioperator_id].children[i]=FALSE;
  all[wiioperator_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) wiioperator_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {wiioperator_brothers[i]=brothers[i];i++;}
    }
  wiioperator_callforarbitration=fn;
  put_state(wiioperator_id,notready);
  printf("wiioperator: on\n");
  pthread_cond_signal(&(all[wiioperator_id].condition));
  pthread_mutex_unlock(&(all[wiioperator_id].mymutex));
  wiioperator_imports();
  buttons_run(wiioperator_id, NULL, NULL);
  acc_run(wiioperator_id, NULL, NULL);
  motors_run(wiioperator_id, NULL, NULL);
  ptmotors_run(wiioperator_id, NULL, NULL);
  ptencoders_run(wiioperator_id, NULL, NULL);

}

void *wiioperator_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;

   for(;;)
   {
      pthread_mutex_lock(&(all[wiioperator_id].mymutex));

      if (all[wiioperator_id].state==slept)
      {
	 pthread_cond_wait(&(all[wiioperator_id].condition),&(all[wiioperator_id].mymutex));
	 pthread_mutex_unlock(&(all[wiioperator_id].mymutex));
      }
      else
      {
	 /* check preconditions. For now, preconditions are always satisfied*/
	 if (all[wiioperator_id].state==notready)
	    put_state(wiioperator_id,ready);
	 /* check brothers and arbitrate. For now this is the only winner */
	 if (all[wiioperator_id].state==ready)
	 {put_state(wiioperator_id,winner);
	 gettimeofday(&a,NULL);
	 aa=a.tv_sec*1000000+a.tv_usec;
	 n=0;
	 }

	 if (all[wiioperator_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	 {
	    pthread_mutex_unlock(&(all[wiioperator_id].mymutex));
	    /*      gettimeofday(&a,NULL);*/
	    n++;
	    wiioperator_iteration();
	    gettimeofday(&b,NULL);
	    bb=b.tv_sec*1000000+b.tv_usec;
	    next=aa+(n+1)*(long)wiioperator_cycle*1000-bb;

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
	    pthread_mutex_unlock(&(all[wiioperator_id].mymutex));
	    usleep(wiioperator_cycle*1000);
	 }
      }
   }
}

void wiioperator_init(char *configfile)
{
  pthread_mutex_lock(&(all[wiioperator_id].mymutex));
  printf("wiioperator schema started up\n");
  wiioperator_exports();
  put_state(wiioperator_id,slept);
  pthread_create(&(all[wiioperator_id].mythread),NULL,wiioperator_thread,NULL);
  pthread_mutex_unlock(&(all[wiioperator_id].mymutex));
  wiioperator_guiinit();
}

void wiioperator_guidisplay(){

	char cadvel[10];
	char cadang[10];
	char cadstate[10];

   gdk_threads_enter();

	snprintf(cadvel,10, "%d", my_v);
	snprintf(cadang,10, "%d", my_w);
	snprintf(cadstate,10, "%d", state);
	gtk_label_set_text((GtkLabel *)glade_xml_get_widget(xml, "label_vel"), cadvel);
	gtk_label_set_text((GtkLabel *)glade_xml_get_widget(xml, "label_angle"), cadang);
	gtk_label_set_text((GtkLabel *)glade_xml_get_widget(xml, "label_state"), cadstate);

   gtk_window_resize (GTK_WINDOW(win),1,1);
   gtk_widget_queue_draw(win);
   gdk_threads_leave();

}

void wiioperator_hide(){
   if (win!=NULL){
      gdk_threads_enter();
      gtk_widget_hide(win);
      gdk_threads_leave();
   }
   mydelete_displaycallback(wiioperator_guidisplay);
}

void wiioperator_show(){
   static int cargado=0;
   static pthread_mutex_t wiioperator_gui_mutex;

   pthread_mutex_lock(&wiioperator_gui_mutex);
   if (!cargado){
      loadglade ld_fn;
      cargado=1;
      pthread_mutex_unlock(&wiioperator_gui_mutex);

      gdk_threads_enter();
      if ((ld_fn=(loadglade)myimport("graphics_gtk","load_glade"))==NULL){
         fprintf (stderr,"I can't fetch 'load_glade' from 'graphics_gtk'.\n");
         jdeshutdown(1);
      }
      xml = ld_fn ("wiioperator.glade");
      if (xml==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }

      win = glade_xml_get_widget(xml, "window");
      {
         g_signal_connect(G_OBJECT(win), "delete-event",
                          G_CALLBACK(on_delete_window), NULL);
         g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "active_buttons")),
                          "toggled", G_CALLBACK(on_active_buttons_toggled), NULL);
         g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "active_motors")),
                          "toggled", G_CALLBACK(on_active_motors_toggled), NULL);
         g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "active_accel")),
                          "toggled", G_CALLBACK(on_active_acc_toggled), NULL);
         g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "active_pantilt")),
                          "toggled", G_CALLBACK(on_active_pantilt_toggled), NULL);
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
      pthread_mutex_unlock(&wiioperator_gui_mutex);
      gdk_threads_enter();
      gtk_widget_show(win);
      gtk_widget_queue_draw(GTK_WIDGET(win));
      gdk_threads_leave();
   }

   myregister_displaycallback(wiioperator_guidisplay);
}

