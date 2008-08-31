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
#include "ipp_demo.h"
#include "graphics_gtk.h"

#include "filters.h"

#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

int ipp_demo_id=0;
int ipp_demo_brothers[MAX_SCHEMAS];
arbitration ipp_demo_callforarbitration;

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

enum{
   PASS=0,
   OPFLOW,
   CONV,
   CANNY,
   CORNER,
};

enum{
   CONV_X=3,
   CONV_Y=3,
};

/*Global variables*/
char *image;
int image_selected=-1;
int filter_selected=PASS;
int contexto_pila;
int show_points=0;
int show_arrows=0;
int* conv_matrix=NULL;
int divisor;
float high=13.0f, low=10.0f;

/* exported variables */
int ipp_demo_cycle=80; /* ms */

/*GUI Variables*/
pthread_t hilo_gtk;
GladeXML *xml=NULL; /*Fichero xml*/
GtkWidget *win=NULL; /*Ventana*/

void cambiar_imagen(int i, char* color){

   width[i]=myimport(color,"width");
   height[i]=myimport(color,"height");
   mycolor[i]=myimport(color, color);
   myresume[i]=(resumeFn)myimport(color,"resume");
   mysuspend[i]=(suspendFn)myimport(color,"suspend");
   
   if ((width[i]!=NULL) && (height[i]!=NULL) && (mycolor[i]!=NULL) &&
        (myresume[i]!=NULL) && (mysuspend[i]!=NULL))
   {
      if (image_selected!=-1){
         mysuspend[image_selected]();
         free(image);
      }
      image_selected=i;
      myresume[image_selected](ipp_demo_id,NULL,NULL);
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
   }
}

void hide_all_frames(){
   gtk_widget_hide(glade_xml_get_widget(xml, "opflow_options"));
   gtk_widget_hide(glade_xml_get_widget(xml, "opflow_options1"));

   gtk_widget_hide(glade_xml_get_widget(xml, "conv_options"));
   gtk_widget_hide(glade_xml_get_widget(xml, "conv_options1"));

   gtk_widget_hide(glade_xml_get_widget(xml, "canny_options"));
   gtk_widget_hide(glade_xml_get_widget(xml, "canny_options1"));

   gtk_widget_hide(glade_xml_get_widget(xml, "corner_options"));
   gtk_widget_hide(glade_xml_get_widget(xml, "corner_options1"));

   gtk_window_resize (GTK_WINDOW(win),1,1);
}

/*Callbacks*/
gboolean on_delete_window (GtkWidget *widget, GdkEvent *event, gpointer user_data){
   gdk_threads_leave();
   ipp_demo_guisuspend();
   gdk_threads_enter();
   return TRUE;
}

void on_conv_matrix_changed (GtkEntry *entry, GdkEvent *event, gpointer matrix){
   char* s_value;
   int value;
   char* s_name;
   char s_divisor[20];
   int i;

   s_name=(char *)gtk_widget_get_name ((GtkWidget *)entry);
   
   s_value=(char *)gtk_entry_get_text (entry);
   value=atoi(s_value);

   if (strcmp(s_name, "conv_0_0")==0){
      conv_matrix[0+0*CONV_X]=value;
   }else if (strcmp(s_name, "conv_0_1")==0){
      conv_matrix[0+1*CONV_X]=value;
   }else if (strcmp(s_name, "conv_0_2")==0){
      conv_matrix[0+2*CONV_X]=value;
   }else if (strcmp(s_name, "conv_1_0")==0){
      conv_matrix[1+0*CONV_X]=value;
   }else if (strcmp(s_name, "conv_1_1")==0){
      conv_matrix[1+1*CONV_X]=value;
   }else if (strcmp(s_name, "conv_1_2")==0){
      conv_matrix[1+2*CONV_X]=value;
   }else if (strcmp(s_name, "conv_2_0")==0){
      conv_matrix[2+0*CONV_X]=value;
   }else if (strcmp(s_name, "conv_2_1")==0){
      conv_matrix[2+1*CONV_X]=value;
   }else if (strcmp(s_name, "conv_2_2")==0){
      conv_matrix[2+2*CONV_X]=value;
   }
   
   divisor=0;
   for (i=0; i<CONV_X*CONV_Y; i++){
      divisor+=conv_matrix[i];
   }

   snprintf(s_divisor, 20, "%d", divisor);

   gtk_entry_set_text ((GtkEntry *)glade_xml_get_widget(xml, "divisor"), s_divisor);
}

void on_show_points_toggled(GtkCheckButton *show_points_button, gpointer user_data){
   int *show_points=(int *)user_data;
   *show_points=gtk_toggle_button_get_active ((GtkToggleButton *)show_points_button);
}

void on_show_arrows_toggled(GtkCheckButton *show_arrows_button, gpointer user_data){
   int *show_arrows=(int*)user_data;
   *show_arrows=gtk_toggle_button_get_active ((GtkToggleButton *)show_arrows_button);
}

void on_filter_sel_changed(GtkComboBox *filter_sel, gpointer user_data){
   char *valor;
   valor=(char *)gtk_combo_box_get_active_text(filter_sel);

   
   if (strcmp(valor,"pass")==0){
      filter_selected=PASS;
      hide_all_frames();
   }
   else if (strcmp(valor,"optical flow")==0){
      filter_selected=OPFLOW;
      hide_all_frames();
      gtk_widget_show(glade_xml_get_widget(xml, "opflow_options"));
      gtk_widget_show(glade_xml_get_widget(xml, "opflow_options1"));
   }
   else if (strcmp(valor,"convolution")==0){
      filter_selected=CONV;
      hide_all_frames();
      gtk_widget_show(glade_xml_get_widget(xml, "conv_options"));
      gtk_widget_show(glade_xml_get_widget(xml, "conv_options1"));
   }
   else if (strcmp(valor,"canny")==0){
      filter_selected=CANNY;
      hide_all_frames();
      gtk_widget_show(glade_xml_get_widget(xml, "canny_options"));
      gtk_widget_show(glade_xml_get_widget(xml, "canny_options1"));
   }
   else if (strcmp(valor,"corner")==0){
      filter_selected=CORNER;
      hide_all_frames();
      gtk_widget_show(glade_xml_get_widget(xml, "corner_options"));
      gtk_widget_show(glade_xml_get_widget(xml, "corner_options1"));
   }

}

void on_img_sel_changed(GtkComboBox *img_sel, gpointer user_data){
   /*Hay que comprobar el valor que tiene*/
   char *valor;
   valor=(char *)gtk_combo_box_get_active_text(img_sel);
   /*Importar los valores oportunos y modificar la selección*/
   if (strcmp(valor,"colorA")==0){
      if (image_selected!=0){
         cambiar_imagen(0, "colorA");
      }
   }
   else if (strcmp(valor,"colorB")==0){
      if (image_selected!=1){
         cambiar_imagen(1, "colorB");
      }
   }
   else if (strcmp(valor,"colorC")==0){
      if (image_selected!=2){
         cambiar_imagen(2, "colorC");
      }
   }
   else if (strcmp(valor,"colorD")==0){
      if (image_selected!=3){
         cambiar_imagen(3, "colorD");
      }
   }
   else if (strcmp(valor,"varcolorA")==0){
      if (image_selected!=4){
         cambiar_imagen(4, "varcolorA");
      }
   }
   else if (strcmp(valor,"varcolorB")==0){
      if (image_selected!=5){
         cambiar_imagen(5, "varcolorB");
      }
   }
   else if (strcmp(valor,"varcolorC")==0){
      if (image_selected!=6){
         cambiar_imagen(6, "varcolorC");
      }
   }
   else if (strcmp(valor,"varcolorD")==0){
      if (image_selected!=7){
         cambiar_imagen(7, "varcolorD");
      }
   }
}

void bgr2rgb(char *image, int width, int height){
   int i, b, r;
   for (i=0; i < width*height; i++){
      b=image[i*3];
      r=image[i*3+2];
      image[i*3]=r;
      image[i*3+2]=b;
   }
}

void ipp_demo_iteration(){
   speedcounter(ipp_demo_id);

   gdk_threads_enter();
   if (image_selected!=-1){
      char* input=(*mycolor[image_selected]);
      int im_width=width[image_selected][0];
      int im_height=height[image_selected][0];
      switch (filter_selected){
         case OPFLOW:
            filter_opflow(input, image, im_width, im_height, show_points, show_arrows);
            break;
         case CONV:
            filter_conv(input, image, im_width, im_height, conv_matrix, CONV_X,
                        CONV_Y, divisor);
            break;
         case CANNY:
            high=gtk_range_get_value((GtkRange *)glade_xml_get_widget(xml,"high"));
            low=gtk_range_get_value((GtkRange *)glade_xml_get_widget(xml,"low"));
            if (low>=high){
               high=low+0.1f;
            }
            filter_canny(input, image, im_width, im_height, low, high);
            break;
         case CORNER:
            high=gtk_range_get_value((GtkRange *)glade_xml_get_widget(xml,"high2"));
            low=gtk_range_get_value((GtkRange *)glade_xml_get_widget(xml,"low2"));
            if (low>=high){
               high=low+0.1f;
            }
            filter_corner(input, image, im_width, im_height, low, high);
            break;
         default:
            filter_pass(input, image, im_width, im_height);
            break;
      }
      bgr2rgb(image, im_width, im_height);
   }
   gdk_threads_leave();
}


/*Importar símbolos*/
void ipp_demo_imports(){

}

/*Exportar símbolos*/
void ipp_demo_exports(){

   myexport("ipp_demo", "id", &ipp_demo_id);
   myexport("ipp_demo","cycle",&ipp_demo_cycle);
   myexport("ipp_demo","resume",(void *)ipp_demo_resume);
   myexport("ipp_demo","suspend",(void *)ipp_demo_suspend);
}

/*Las inicializaciones van en esta parte*/
void ipp_demo_init(){
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
   if (conv_matrix==NULL){
      int i;
      conv_matrix=(int *)malloc( sizeof(int)*CONV_X*CONV_Y);
      for (i=0; i<CONV_X*CONV_Y; i++){
            conv_matrix[i]=0;
      }
   }
}

/*Al suspender el esquema*/
void ipp_demo_end(){
   if (image_selected!=-1)
      mysuspend[image_selected]();
}

void ipp_demo_stop(){
}

void ipp_demo_suspend()
{
  pthread_mutex_lock(&(all[ipp_demo_id].mymutex));
  put_state(ipp_demo_id,slept);
  printf("ipp_demo: off\n");
  pthread_mutex_unlock(&(all[ipp_demo_id].mymutex));
  ipp_demo_end();
}


void ipp_demo_resume(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[ipp_demo_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[ipp_demo_id].mymutex));
  /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[ipp_demo_id].children[i]=FALSE;
  all[ipp_demo_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) ipp_demo_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {ipp_demo_brothers[i]=brothers[i];i++;}
    }
  ipp_demo_callforarbitration=fn;
  put_state(ipp_demo_id,notready);
  printf("ipp_demo: on\n");
  pthread_cond_signal(&(all[ipp_demo_id].condition));
  pthread_mutex_unlock(&(all[ipp_demo_id].mymutex));
  ipp_demo_imports();
  if (image_selected!=-1)
     myresume[image_selected](ipp_demo_id,NULL,NULL);
}

void *ipp_demo_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;

   for(;;)
   {
      pthread_mutex_lock(&(all[ipp_demo_id].mymutex));

      if (all[ipp_demo_id].state==slept)
      {
	 pthread_cond_wait(&(all[ipp_demo_id].condition),&(all[ipp_demo_id].mymutex));
	 pthread_mutex_unlock(&(all[ipp_demo_id].mymutex));
      }
      else
      {
	 /* check preconditions. For now, preconditions are always satisfied*/
	 if (all[ipp_demo_id].state==notready)
	    put_state(ipp_demo_id,ready);
	 /* check brothers and arbitrate. For now this is the only winner */
	 if (all[ipp_demo_id].state==ready)
	 {put_state(ipp_demo_id,winner);
	 gettimeofday(&a,NULL);
	 aa=a.tv_sec*1000000+a.tv_usec;
	 n=0;
	 }

	 if (all[ipp_demo_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	 {
	    pthread_mutex_unlock(&(all[ipp_demo_id].mymutex));
	    /*      gettimeofday(&a,NULL);*/
	    n++;
	    ipp_demo_iteration();
	    gettimeofday(&b,NULL);
	    bb=b.tv_sec*1000000+b.tv_usec;
	    next=aa+(n+1)*(long)ipp_demo_cycle*1000-bb;

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
	    pthread_mutex_unlock(&(all[ipp_demo_id].mymutex));
	    usleep(ipp_demo_cycle*1000);
	 }
      }
   }
}

void ipp_demo_startup(char *configfile)
{
  pthread_mutex_lock(&(all[ipp_demo_id].mymutex));
  printf("ipp_demo schema started up\n");
  ipp_demo_exports();
  put_state(ipp_demo_id,slept);
  pthread_create(&(all[ipp_demo_id].mythread),NULL,ipp_demo_thread,NULL);
  pthread_mutex_unlock(&(all[ipp_demo_id].mymutex));
  ipp_demo_init();
}

void ipp_demo_guidisplay(){
   if (image_selected!=-1){
      GtkImage *img = GTK_IMAGE(glade_xml_get_widget(xml, "image"));

      gdk_threads_enter();
      gtk_widget_queue_draw(GTK_WIDGET(img));
      gdk_threads_leave();
   }
}


void ipp_demo_guisuspend(void){
   mydelete_displaycallback(ipp_demo_guidisplay);
   if (win!=NULL){
      gdk_threads_enter();
      gtk_widget_hide(win);
      gdk_threads_leave();
   }
   all[ipp_demo_id].guistate=pending_off;
}

void init_gui(){
   loadglade ld_fn;

   /*Cargar la ventana desde el archivo xml .glade*/
   if ((ld_fn=(loadglade)myimport("graphics_gtk","load_glade"))==NULL){
      fprintf (stderr,"I can't fetch 'load_glade' from 'graphics_gtk'.\n");
      jdeshutdown(1);
   }
   xml = ld_fn ("ipp_demo.glade");
   if (xml==NULL){
      fprintf(stderr, "Error al cargar la interfaz gráfica\n");
      jdeshutdown(1);
   }
   win = glade_xml_get_widget(xml, "window");
   /*Conectar los callbacks*/
   {
      GtkComboBox *sel_ent;
      sel_ent=(GtkComboBox *)glade_xml_get_widget(xml, "img_sel");
      g_signal_connect(G_OBJECT(sel_ent), "changed",
                       G_CALLBACK(on_img_sel_changed), NULL);
      g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "filter_sel")), "changed",
                       G_CALLBACK(on_filter_sel_changed), NULL);
      g_signal_connect(G_OBJECT(win), "delete-event",
                       G_CALLBACK(on_delete_window), NULL);

      g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "show_points")), "toggled",
                      G_CALLBACK(on_show_points_toggled), &show_points);
      g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "show_arrows")), "toggled",
                       G_CALLBACK(on_show_arrows_toggled), &show_arrows);
      g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "conv_0_0")), "changed",
                       G_CALLBACK(on_conv_matrix_changed), &conv_matrix);
      g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "conv_0_1")), "changed",
                       G_CALLBACK(on_conv_matrix_changed), &conv_matrix);
      g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "conv_0_2")), "changed",
                       G_CALLBACK(on_conv_matrix_changed), &conv_matrix);
      g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "conv_1_0")), "changed",
                       G_CALLBACK(on_conv_matrix_changed), &conv_matrix);
      g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "conv_1_1")), "changed",
                       G_CALLBACK(on_conv_matrix_changed), &conv_matrix);
      g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "conv_1_2")), "changed",
                       G_CALLBACK(on_conv_matrix_changed), &conv_matrix);
      g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "conv_2_0")), "changed",
                       G_CALLBACK(on_conv_matrix_changed), &conv_matrix);
      g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "conv_2_1")), "changed",
                       G_CALLBACK(on_conv_matrix_changed), &conv_matrix);
      g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "conv_2_2")), "changed",
                       G_CALLBACK(on_conv_matrix_changed), &conv_matrix);

   }
   if (win==NULL){
      fprintf(stderr, "Error al cargar la interfaz gráfica\n");
      jdeshutdown(1);
   }

   hide_all_frames();
   contexto_pila=gtk_statusbar_get_context_id ((GtkStatusbar *)glade_xml_get_widget(xml, "barra_estado"),
         "contexto general");
   gtk_statusbar_push((GtkStatusbar *)glade_xml_get_widget(xml, "barra_estado"),
                       contexto_pila,
                       "No hay fuente de imagen seleccionada");
}

void ipp_demo_guiresume(void){
   static int cargado=0;
   static pthread_mutex_t ipp_demo_gui_mutex;

   pthread_mutex_lock(&ipp_demo_gui_mutex);
   if (!cargado){
      cargado=1;
      pthread_mutex_unlock(&ipp_demo_gui_mutex);
      /*Cargar la ventana desde el archivo xml .glade*/
      gdk_threads_enter();
      init_gui();
      gtk_widget_show(win);
      gtk_widget_queue_draw(GTK_WIDGET(win));
      gdk_threads_leave();
   }
   else{
      pthread_mutex_unlock(&ipp_demo_gui_mutex);
      gdk_threads_enter();
      gtk_widget_show(win);
      gtk_widget_queue_draw(GTK_WIDGET(win));
      gdk_threads_leave();
   }

   myregister_displaycallback(ipp_demo_guidisplay);
   all[ipp_demo_id].guistate=pending_on;
}

