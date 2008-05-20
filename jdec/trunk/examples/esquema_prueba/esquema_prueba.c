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
#include "esquema_prueba.h"
#include "graphics_gtk.h"

#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

/** Image standard number of rows*/
#define SIFNTSC_ROWS 240
/** Image standard number of columns*/
#define SIFNTSC_COLUMNS 320

int esquema_prueba_id=0;
int esquema_prueba_brothers[MAX_SCHEMAS];
arbitration esquema_prueba_callforarbitration;

enum esquema_prueba_states {init,t1,r1,t2,r2,t3,r3,t4,end};
static int esquema_prueba_state;

/*Imported variables*/
char **mycolorA=NULL;
resumeFn mycolorAresume=NULL;
suspendFn mycolorAsuspend=NULL;

registerdisplay myregister_displaycallback;
deletedisplay mydelete_displaycallback;

/*Global variables*/
char imageA[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3];
int display_imageA=0;
int aplicar_filtro=0;
double filtro=1.0;

/* exported variables */
int esquema_prueba_cycle=40; /* ms */

/*GUI Variables*/
pthread_t hilo_gtk;
GladeXML *xml; /*Fichero xml*/
GtkWidget *win;

/*Callbacks*/
void on_togglebutton1_clicked (GtkToggleButton *button, gpointer user_data){
   if (gtk_toggle_button_get_active(button)) {
      GdkPixbuf *imgBuff;
      GtkImage *image=(GtkImage *)glade_xml_get_widget(xml, "image1");
      display_imageA=1;
      mycolorAresume(esquema_prueba_id,NULL,NULL);
      /* We read from our buffer: colorA */
      imgBuff = gdk_pixbuf_new_from_data((unsigned char *)imageA,
                                          GDK_COLORSPACE_RGB,0,8,
                                          SIFNTSC_COLUMNS,SIFNTSC_ROWS,
                                          SIFNTSC_COLUMNS*3,NULL,NULL);

      gtk_image_set_from_pixbuf(image, imgBuff);
   }
   else {
      GtkImage *image = GTK_IMAGE(glade_xml_get_widget(xml, "image1"));
      display_imageA=0;
      gtk_image_clear(image);
      mycolorAsuspend();
   }
}

void on_togglebutton2_clicked (GtkToggleButton *button, gpointer user_data){
   if (gtk_toggle_button_get_active(button))
      aplicar_filtro=1;
   else
      aplicar_filtro=0;
}

void on_vscale1_move_slider (GtkRange *range, GtkScrollType scroll, gpointer user_data){
   filtro=gtk_range_get_value(range);
}

void esquema_prueba_iteration(){
   int i;
   
   speedcounter(esquema_prueba_id);

   for (i=0;i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++){
      if (aplicar_filtro){
         imageA[i*3]=(*mycolorA)[i*3+2]*filtro;
         imageA[i*3+1]=(*mycolorA)[i*3+1]*filtro;
         imageA[i*3+2]=(*mycolorA)[i*3]*filtro;
      }
      else{
         imageA[i*3]=(*mycolorA)[i*3+2];
         imageA[i*3+1]=(*mycolorA)[i*3+1];
         imageA[i*3+2]=(*mycolorA)[i*3];
      }
   }

}


/*Importar símbolos*/
void esquema_prueba_imports(){

   mycolorA=(char **)myimport("colorA","colorA");
   mycolorAresume=(resumeFn) myimport("colorA", "resume");
   mycolorAsuspend=(suspendFn) myimport("colorA", "suspend");
   if (mycolorA == NULL || mycolorAresume == NULL || mycolorAsuspend == NULL){
      fprintf(stderr, "No se pueden importar las variables necesarias desde colorA\n");
      jdeshutdown(1);
   }
}

/*Exportar símbolos*/
void esquema_prueba_exports(){

   myexport("esquema_prueba","cycle",&esquema_prueba_cycle);
   myexport("esquema_prueba","resume",(void *)esquema_prueba_resume);
   myexport("esquema_prueba","suspend",(void *)esquema_prueba_suspend);
}

/*Las inicializaciones van en esta parte*/
void esquema_prueba_init(){
   if (myregister_displaycallback==NULL){
      if ((myregister_displaycallback=(registerdisplay)myimport ("graphics_gtk", "register_displaycallback"))==NULL){
         printf ("I can't fetch register_displaycallback from graphics_gtk\n");
         jdeshutdown(1);
      }
      if ((mydelete_displaycallback=(deletedisplay)myimport ("graphics_gtk", "delete_displaycallback"))==NULL){
         jdeshutdown(1);
         printf ("I can't fetch delete_displaycallback from graphics_gtk\n");
      }
   }
}

/*Al suspender el esquema*/
void esquema_prueba_end(){
}


void esquema_prueba_suspend()
{
  pthread_mutex_lock(&(all[esquema_prueba_id].mymutex));
  put_state(esquema_prueba_id,slept);
  printf("esquema_prueba: off\n");
  pthread_mutex_unlock(&(all[esquema_prueba_id].mymutex));
  esquema_prueba_end();
}


void esquema_prueba_resume(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[esquema_prueba_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[esquema_prueba_id].mymutex));
  /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[esquema_prueba_id].children[i]=FALSE;
  all[esquema_prueba_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) esquema_prueba_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {esquema_prueba_brothers[i]=brothers[i];i++;}
    }
  esquema_prueba_callforarbitration=fn;
  put_state(esquema_prueba_id,notready);
  printf("esquema_prueba: on\n");
  pthread_cond_signal(&(all[esquema_prueba_id].condition));
  pthread_mutex_unlock(&(all[esquema_prueba_id].mymutex));
  esquema_prueba_imports();
}

void *esquema_prueba_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;

   for(;;)
   {
      pthread_mutex_lock(&(all[esquema_prueba_id].mymutex));

      if (all[esquema_prueba_id].state==slept)
      {
	 esquema_prueba_state=init;
	 pthread_cond_wait(&(all[esquema_prueba_id].condition),&(all[esquema_prueba_id].mymutex));
	 pthread_mutex_unlock(&(all[esquema_prueba_id].mymutex));
      }
      else
      {
	 /* check preconditions. For now, preconditions are always satisfied*/
	 if (all[esquema_prueba_id].state==notready)
	    put_state(esquema_prueba_id,ready);
	 /* check brothers and arbitrate. For now this is the only winner */
	 if (all[esquema_prueba_id].state==ready)
	 {put_state(esquema_prueba_id,winner);
	 gettimeofday(&a,NULL);
	 aa=a.tv_sec*1000000+a.tv_usec;
	 n=0;
	 }

	 if (all[esquema_prueba_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	 {
	    pthread_mutex_unlock(&(all[esquema_prueba_id].mymutex));
	    /*      gettimeofday(&a,NULL);*/
	    n++;
	    esquema_prueba_iteration();
	    gettimeofday(&b,NULL);
	    bb=b.tv_sec*1000000+b.tv_usec;
	    next=aa+(n+1)*(long)esquema_prueba_cycle*1000-bb;

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
	    pthread_mutex_unlock(&(all[esquema_prueba_id].mymutex));
	    usleep(esquema_prueba_cycle*1000);
	 }
      }
   }
}

void esquema_prueba_startup()
{
  pthread_mutex_lock(&(all[esquema_prueba_id].mymutex));
  printf("esquema_prueba schema started up\n");
  esquema_prueba_exports();
  put_state(esquema_prueba_id,slept);
  pthread_create(&(all[esquema_prueba_id].mythread),NULL,esquema_prueba_thread,NULL);
  pthread_mutex_unlock(&(all[esquema_prueba_id].mymutex));
  esquema_prueba_init();
}

void esquema_prueba_guidisplay(){
   if (display_imageA){
      GtkImage *image = GTK_IMAGE(glade_xml_get_widget(xml, "image1"));

      gdk_threads_enter();
      gtk_widget_queue_draw(GTK_WIDGET(image));
      gdk_threads_leave();
   }
}


void esquema_prueba_guisuspend(void){
   if (win!=NULL){
      gdk_threads_enter();
      gtk_widget_hide(win);
      gdk_threads_leave();
   }
   mydelete_displaycallback(esquema_prueba_guidisplay);
}

void esquema_prueba_guiresume(void){
   static int cargado=0;
   static pthread_mutex_t esquema_prueba_gui_mutex;

   pthread_mutex_lock(&esquema_prueba_gui_mutex);
   if (!cargado){
      loadglade ld_fn;
      cargado=1;
      pthread_mutex_unlock(&esquema_prueba_gui_mutex);
      /*Cargar la ventana desde el archivo xml .glade*/
      gdk_threads_enter();
      if ((ld_fn=(loadglade)myimport("graphics_gtk","load_glade"))==NULL){
         fprintf (stderr,"I can't fetch 'load_glade' from 'graphics_gtk'.\n");
         jdeshutdown(1);
      }
      xml = ld_fn ("esquema_prueba.glade");
      if (xml==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }
      win = glade_xml_get_widget(xml, "window1");
      /*Conectar los callbacks*/
      /*glade_xml_signal_autoconnect (xml);*/
      {
         GtkToggleButton *button1, *button2;
         GtkVScale *slider1;
         button1=(GtkToggleButton *)glade_xml_get_widget(xml, "togglebutton1");
         g_signal_connect (G_OBJECT (button1), "clicked",
                           G_CALLBACK (on_togglebutton1_clicked), NULL);
         button2=(GtkToggleButton *)glade_xml_get_widget(xml, "togglebutton2");
         g_signal_connect (G_OBJECT (button2), "clicked",
                           G_CALLBACK (on_togglebutton2_clicked), NULL);
         slider1=(GtkVScale *)glade_xml_get_widget(xml, "vscale1");
         g_signal_connect (G_OBJECT (slider1), "value_changed",
                           G_CALLBACK (on_vscale1_move_slider), NULL);
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
      pthread_mutex_unlock(&esquema_prueba_gui_mutex);
      gdk_threads_enter();
      gtk_widget_show(win);
      gtk_widget_queue_draw(GTK_WIDGET(win));
      gdk_threads_leave();
   }

   myregister_displaycallback(esquema_prueba_guidisplay);
}

