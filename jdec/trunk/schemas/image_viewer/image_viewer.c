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
#include "image_viewer.h"
#include "graphics_gtk.h"

#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

int image_viewer_id=0;
int image_viewer_brothers[MAX_SCHEMAS];
arbitration image_viewer_callforarbitration;

#define MAX_COLOR 8
/*Imported variables*/
int* width[MAX_COLOR];
int* height[MAX_COLOR];
char** mycolor[MAX_COLOR];
resumeFn myresume[MAX_COLOR];
suspendFn mysuspend[MAX_COLOR];
char *mensajes[MAX_COLOR]={"Mostrando colorA","Mostrando colorB","Mostrando colorC","Mostrando colorD","Mostrando varcolorA","Mostrando varcolorB","Mostrando varcolorC","Mostrando varcolorD"};

registerdisplay myregister_displaycallback;
deletedisplay mydelete_displaycallback;

/*Global variables*/
char *image;
int image_selected=-1;
pthread_mutex_t main_mutex;
int contexto_pila;

/* exported variables */
int image_viewer_cycle=40; /* ms */

/*GUI Variables*/
pthread_t hilo_gtk;
GladeXML *xml; /*Fichero xml*/
GtkWidget *win; /*Ventana*/

void cambiar_imagen(int i){
   if ((width[i]!=NULL) && (height[i]!=NULL) && (mycolor[i]!=NULL) &&
        (myresume[i]!=NULL) && (mysuspend[i]!=NULL))
   {
      pthread_mutex_lock(&main_mutex);
      if (image_selected!=-1){
         mysuspend[image_selected]();
         free(image);
      }
      image_selected=i;
      myresume[image_selected](image_viewer_id,NULL,NULL);
      image=(char *)malloc(width[image_selected][0]*height[image_selected][0]*3);
      {
         GdkPixbuf *imgBuff;
         GtkImage *img=(GtkImage *)glade_xml_get_widget(xml, "image");
         imgBuff = gdk_pixbuf_new_from_data((unsigned char *)image,
                                             GDK_COLORSPACE_RGB,0,8,
                                             width[image_selected][0],height[image_selected][0],
                                             width[image_selected][0]*3,NULL,NULL);
         gtk_image_clear(img);
         gtk_image_set_from_pixbuf(img, imgBuff);
         gtk_statusbar_push((GtkStatusbar *)glade_xml_get_widget(xml, "barra_estado"),
                             contexto_pila,
                             mensajes[i]);
         gtk_widget_queue_draw(GTK_WIDGET(img));
         gtk_window_resize (GTK_WINDOW(win),1,1);
      }
      pthread_mutex_unlock(&main_mutex);
   }
}

/*Callbacks*/
gboolean on_delete_window (GtkWidget *widget, GdkEvent *event, gpointer user_data){
   gdk_threads_leave();
   image_viewer_guisuspend();
   gdk_threads_enter();
   return TRUE;
}

void on_img_sel_changed(GtkComboBoxEntry *img_sel, gpointer user_data){
   /*Hay que comprobar el valor que tiene*/
   char *valor;
   valor=(char *)gtk_combo_box_get_active_text((GtkComboBox *)img_sel);
   /*Importar los valores oportunos y modificar la selección*/
   printf (valor);
   printf("\n");
   if (strcmp(valor,"colorA")==0){
      if (image_selected!=0){
         width[0]=myimport("colorA","width");
         height[0]=myimport("colorA","height");
         mycolor[0]=myimport("colorA","colorA");
         myresume[0]=(resumeFn)myimport("colorA","resume");
         mysuspend[0]=(suspendFn)myimport("colorA","suspend");
         cambiar_imagen(0);
      }
   }
   else if (strcmp(valor,"colorB")==0){
      if (image_selected!=1){
         width[1]=myimport("colorB","width");
         height[1]=myimport("colorB","height");
         mycolor[1]=myimport("colorB","colorB");
         myresume[1]=(resumeFn)myimport("colorB","resume");
         mysuspend[1]=(suspendFn)myimport("colorB","suspend");
         cambiar_imagen(1);
      }
   }
   else if (strcmp(valor,"colorC")==0){
      if (image_selected!=2){
         width[2]=myimport("colorC","width");
         height[2]=myimport("colorC","height");
         mycolor[2]=myimport("colorC","colorC");
         myresume[2]=(resumeFn)myimport("colorC","resume");
         mysuspend[2]=(suspendFn)myimport("colorC","suspend");
         cambiar_imagen(2);
      }
   }
   else if (strcmp(valor,"colorD")==0){
      if (image_selected!=3){
         width[3]=myimport("colorD","width");
         height[3]=myimport("colorD","height");
         mycolor[3]=myimport("colorD","colorD");
         myresume[3]=(resumeFn)myimport("colorD","resume");
         mysuspend[3]=(suspendFn)myimport("colorD","suspend");
         cambiar_imagen(3);
      }
   }
   else if (strcmp(valor,"varcolorA")==0){
      if (image_selected!=4){
         width[4]=myimport("varcolorA","width");
         height[4]=myimport("varcolorA","height");
         mycolor[4]=myimport("varcolorA","varcolorA");
         myresume[4]=(resumeFn)myimport("varcolorA","resume");
         mysuspend[4]=(suspendFn)myimport("varcolorA","suspend");
         cambiar_imagen(4);
      }
   }
   else if (strcmp(valor,"varcolorB")==0){
      if (image_selected!=5){
         width[5]=myimport("varcolorB","width");
         height[5]=myimport("varcolorB","height");
         mycolor[5]=myimport("varcolorB","varcolorB");
         myresume[5]=(resumeFn)myimport("varcolorB","resume");
         mysuspend[5]=(suspendFn)myimport("varcolorB","suspend");
         cambiar_imagen(5);
      }
   }
   else if (strcmp(valor,"varcolorC")==0){
      if (image_selected!=6){
         width[6]=myimport("varcolorC","width");
         height[6]=myimport("varcolorC","height");
         mycolor[6]=myimport("varcolorC","varcolorC");
         myresume[6]=(resumeFn)myimport("varcolorC","resume");
         mysuspend[6]=(suspendFn)myimport("varcolorC","suspend");
         cambiar_imagen(6);
      }
   }
   else if (strcmp(valor,"varcolorD")==0){
      if (image_selected!=7){
         width[7]=myimport("varcolorD","width");
         height[7]=myimport("varcolorD","height");
         mycolor[7]=myimport("varcolorD","varcolorD");
         myresume[7]=(resumeFn)myimport("varcolorD","resume");
         mysuspend[7]=(suspendFn)myimport("varcolorD","suspend");
         cambiar_imagen(7);
      }
   }
}

void image_viewer_iteration(){
   int i;
   
   speedcounter(image_viewer_id);

   pthread_mutex_lock(&main_mutex);
   if (image_selected!=-1){
      for (i=0;i<width[image_selected][0]*height[image_selected][0]; i++){
         image[i*3]=(*mycolor[image_selected])[i*3+2];
         image[i*3+1]=(*mycolor[image_selected])[i*3+1];
         image[i*3+2]=(*mycolor[image_selected])[i*3];
      }
   }
   pthread_mutex_unlock(&main_mutex);
}


/*Importar símbolos*/
void image_viewer_imports(){

}

/*Exportar símbolos*/
void image_viewer_exports(){

   myexport("image_viewer", "id", &image_viewer_id);
   myexport("image_viewer","cycle",&image_viewer_cycle);
   myexport("image_viewer","resume",(void *)image_viewer_resume);
   myexport("image_viewer","suspend",(void *)image_viewer_suspend);
}

/*Las inicializaciones van en esta parte*/
void image_viewer_init(){
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
void image_viewer_end(){
}

void image_viewer_stop(){
}

void image_viewer_suspend()
{
  pthread_mutex_lock(&(all[image_viewer_id].mymutex));
  put_state(image_viewer_id,slept);
  printf("image_viewer: off\n");
  pthread_mutex_unlock(&(all[image_viewer_id].mymutex));
  image_viewer_end();
}


void image_viewer_resume(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[image_viewer_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[image_viewer_id].mymutex));
  /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[image_viewer_id].children[i]=FALSE;
  all[image_viewer_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) image_viewer_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {image_viewer_brothers[i]=brothers[i];i++;}
    }
  image_viewer_callforarbitration=fn;
  put_state(image_viewer_id,notready);
  printf("image_viewer: on\n");
  pthread_cond_signal(&(all[image_viewer_id].condition));
  pthread_mutex_unlock(&(all[image_viewer_id].mymutex));
  image_viewer_imports();
}

void *image_viewer_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;

   for(;;)
   {
      pthread_mutex_lock(&(all[image_viewer_id].mymutex));

      if (all[image_viewer_id].state==slept)
      {
	 pthread_cond_wait(&(all[image_viewer_id].condition),&(all[image_viewer_id].mymutex));
	 pthread_mutex_unlock(&(all[image_viewer_id].mymutex));
      }
      else
      {
	 /* check preconditions. For now, preconditions are always satisfied*/
	 if (all[image_viewer_id].state==notready)
	    put_state(image_viewer_id,ready);
	 /* check brothers and arbitrate. For now this is the only winner */
	 if (all[image_viewer_id].state==ready)
	 {put_state(image_viewer_id,winner);
	 gettimeofday(&a,NULL);
	 aa=a.tv_sec*1000000+a.tv_usec;
	 n=0;
	 }

	 if (all[image_viewer_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	 {
	    pthread_mutex_unlock(&(all[image_viewer_id].mymutex));
	    /*      gettimeofday(&a,NULL);*/
	    n++;
	    image_viewer_iteration();
	    gettimeofday(&b,NULL);
	    bb=b.tv_sec*1000000+b.tv_usec;
	    next=aa+(n+1)*(long)image_viewer_cycle*1000-bb;

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
	    pthread_mutex_unlock(&(all[image_viewer_id].mymutex));
	    usleep(image_viewer_cycle*1000);
	 }
      }
   }
}

void image_viewer_startup(char *configfile)
{
  pthread_mutex_lock(&(all[image_viewer_id].mymutex));
  printf("image_viewer schema started up\n");
  image_viewer_exports();
  put_state(image_viewer_id,slept);
  pthread_create(&(all[image_viewer_id].mythread),NULL,image_viewer_thread,NULL);
  pthread_mutex_unlock(&(all[image_viewer_id].mymutex));
  image_viewer_init();
}

void image_viewer_guidisplay(){
   pthread_mutex_lock(&main_mutex);
   if (image_selected!=-1){
      GtkImage *img = GTK_IMAGE(glade_xml_get_widget(xml, "image"));

      gdk_threads_enter();
      gtk_widget_queue_draw(GTK_WIDGET(img));
      gdk_threads_leave();
   }
   pthread_mutex_unlock(&main_mutex);
}


void image_viewer_guisuspend(void){
   mydelete_displaycallback(image_viewer_guidisplay);
   if (win!=NULL){
      gdk_threads_enter();
      gtk_widget_hide(win);
      gdk_threads_leave();
   }
   all[image_viewer_id].guistate=pending_off;
}

void image_viewer_guiresume(void){
   static int cargado=0;
   static pthread_mutex_t image_viewer_gui_mutex;

   pthread_mutex_lock(&image_viewer_gui_mutex);
   if (!cargado){
      loadglade ld_fn;
      cargado=1;
      pthread_mutex_unlock(&image_viewer_gui_mutex);
      /*Cargar la ventana desde el archivo xml .glade*/
      gdk_threads_enter();
      if ((ld_fn=(loadglade)myimport("graphics_gtk","load_glade"))==NULL){
         fprintf (stderr,"I can't fetch 'load_glade' from 'graphics_gtk'.\n");
         jdeshutdown(1);
      }
      xml = ld_fn ("image_viewer.glade");
      if (xml==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }
      win = glade_xml_get_widget(xml, "window");
      /*Conectar los callbacks*/
      {
         GtkComboBoxEntry *sel_ent;
         sel_ent=(GtkComboBoxEntry *)glade_xml_get_widget(xml, "img_sel");
         g_signal_connect(G_OBJECT(sel_ent), "changed",
                          G_CALLBACK(on_img_sel_changed), NULL);
         g_signal_connect(G_OBJECT(win), "delete-event",
                          G_CALLBACK(on_delete_window), NULL);
      }
      if (win==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }
      else{
         gtk_widget_show(win);
         gtk_widget_queue_draw(GTK_WIDGET(win));
      }
      contexto_pila=gtk_statusbar_get_context_id ((GtkStatusbar *)glade_xml_get_widget(xml, "barra_estado"),
            "contexto general");
      gtk_statusbar_push((GtkStatusbar *)glade_xml_get_widget(xml, "barra_estado"),
                         contexto_pila,
                         "No hay fuente de imagen seleccionada");
      gdk_threads_leave();
   }
   else{
      pthread_mutex_unlock(&image_viewer_gui_mutex);
      gdk_threads_enter();
      gtk_widget_show(win);
      gtk_widget_queue_draw(GTK_WIDGET(win));
      gdk_threads_leave();
   }

   myregister_displaycallback(image_viewer_guidisplay);
   all[image_viewer_id].guistate=pending_on;
}

