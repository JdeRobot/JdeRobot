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
#include "opengl_viewer.h"
#include "graphics_gtk.h"

#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gtk/gtkgl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "obj_loader.h"

#define DISTANCE_MAX 25.0

int opengl_viewer_id=0;
int opengl_viewer_brothers[MAX_SCHEMAS];
arbitration opengl_viewer_callforarbitration;

/*Imported variables*/


registerdisplay myregister_displaycallback;
deletedisplay mydelete_displaycallback;

/*Global variables*/
float eye_posx=0, eye_posy=0, eye_posz=0, incx=0, incy=0;
float old_x=0, old_y=0;
float escala=1.;

obj_type obj;/*Object loaded from .obj file*/


/* exported variables */
int opengl_viewer_cycle=40; /* ms */

/*GUI Variables*/
GladeXML *xml; /*Fichero xml*/
GtkWidget *win;
GtkWidget *drawing_area;

pthread_mutex_t gl_mutex;

void Cobre (void)
{
   float ambiente[] = {0.33f, 0.26f, 0.23f, 1.0f};
   float difusa[] = {0.50f, 0.11f, 0.00f, 1.0f};
   float especular[] = {0.95f, 0.73f, 0.00f, 1.0f};
   glMaterialfv (GL_FRONT, GL_AMBIENT, ambiente);
   glMaterialfv (GL_FRONT, GL_DIFFUSE, difusa);
   glMaterialfv (GL_FRONT, GL_SPECULAR, especular);
   glMaterialf (GL_FRONT, GL_SHININESS, 93.0f);
}

void Bone (void)
{
   float ambiente[] = {0.7f, 0.7f, 0.3f, 1.0f};
   float difusa[] = {0.9f, 0.9f, 0.8f, 1.0f};
   float especular[] = {0.05f, 0.05f, 0.05f, 1.0f};
   glMaterialfv (GL_FRONT, GL_AMBIENT, ambiente);
   glMaterialfv (GL_FRONT, GL_DIFFUSE, difusa);
   glMaterialfv (GL_FRONT, GL_SPECULAR, especular);
   glMaterialf (GL_FRONT, GL_SHININESS, 0.1f);
}

void Luz0 (void)
{
   float luz0_posicion[] = {20.f, 15.f, 3.f, 1.f};
   float luz0_ambiente[] = {0.5f, 0.5f, 0.5f, 1.f};
   float luz0_difusa[] = {.8f , .8f, .8f, 1.f};
   float luz0_especular[] = {.5f, .5f, .5f, 1.f};
   int i;

   for (i=0; i<3; i++){
      luz0_ambiente[i]=gtk_adjustment_get_value(gtk_range_get_adjustment((GtkRange *)glade_xml_get_widget(xml,"ambient")));
      luz0_difusa[i]=gtk_adjustment_get_value(gtk_range_get_adjustment((GtkRange *)glade_xml_get_widget(xml,"diffuse")));
      luz0_especular[i]=gtk_adjustment_get_value(gtk_range_get_adjustment((GtkRange *)glade_xml_get_widget(xml,"specular")));
   }
   luz0_posicion[0]=gtk_adjustment_get_value(gtk_range_get_adjustment((GtkRange *)glade_xml_get_widget(xml,"x")));
   luz0_posicion[1]=gtk_adjustment_get_value(gtk_range_get_adjustment((GtkRange *)glade_xml_get_widget(xml,"y")));
   luz0_posicion[2]=gtk_adjustment_get_value(gtk_range_get_adjustment((GtkRange *)glade_xml_get_widget(xml,"z")));
 
   glLightfv (GL_LIGHT0, GL_POSITION, luz0_posicion);
   glLightfv (GL_LIGHT0, GL_AMBIENT, luz0_ambiente);
   glLightfv (GL_LIGHT0, GL_DIFFUSE, luz0_difusa);
   glLightfv (GL_LIGHT0, GL_SPECULAR, luz0_especular);
   glEnable(GL_LIGHT0);
}

void Luz1 (void)
{
   float luz0_posicion[] = {20.f, 15.f, 3.f, 0.f};
   float luz0_ambiente[] = {0.5f, 0.5f, 0.5f, 1.f};
   float luz0_difusa[] = {.8f , .8f, .8f, 1.f};
   float luz0_especular[] = {.5f, .5f, .5f, 1.f};
   int i;

   for (i=0; i<3; i++){
      luz0_ambiente[i]=gtk_adjustment_get_value(gtk_range_get_adjustment((GtkRange *)glade_xml_get_widget(xml,"ambient")));
      luz0_difusa[i]=gtk_adjustment_get_value(gtk_range_get_adjustment((GtkRange *)glade_xml_get_widget(xml,"diffuse")));
      luz0_especular[i]=gtk_adjustment_get_value(gtk_range_get_adjustment((GtkRange *)glade_xml_get_widget(xml,"specular")));
   }
   luz0_posicion[0]=gtk_adjustment_get_value(gtk_range_get_adjustment((GtkRange *)glade_xml_get_widget(xml,"x")));
   luz0_posicion[1]=gtk_adjustment_get_value(gtk_range_get_adjustment((GtkRange *)glade_xml_get_widget(xml,"y")));
   luz0_posicion[2]=gtk_adjustment_get_value(gtk_range_get_adjustment((GtkRange *)glade_xml_get_widget(xml,"z")));
 
   glLightfv (GL_LIGHT0, GL_POSITION, luz0_posicion);
   glLightfv (GL_LIGHT0, GL_AMBIENT, luz0_ambiente);
   glLightfv (GL_LIGHT0, GL_DIFFUSE, luz0_difusa);
   glLightfv (GL_LIGHT0, GL_SPECULAR, luz0_especular);
   glEnable(GL_LIGHT0);
}

/*Callbacks*/
void on_hscale_move_slider (GtkRange *range, GtkScrollType scroll, gpointer user_data){
   gtk_widget_queue_draw (drawing_area);
}

static gboolean configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer data) {
   GLfloat w;
   GLfloat h;

   GdkGLContext *glcontext;
   GdkGLDrawable *gldrawable;

   pthread_mutex_lock(&gl_mutex);

   w = widget->allocation.width;
   h = widget->allocation.height;
   
   glcontext = gtk_widget_get_gl_context (widget);
   gldrawable = gtk_widget_get_gl_drawable (widget);

   if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext)){
      pthread_mutex_unlock(&gl_mutex);
      return FALSE;
   }
   
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0,1.0,1.0,100.0);
   glTranslatef(0.0,0.0,-5.0);

   if (w > h) {
      glViewport (0, (h - w) / 2, w, w);
   }
   else {
      glViewport ((w - h) / 2, 0, h, h);
   }

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   if (gdk_gl_drawable_is_double_buffered (gldrawable)){
      gdk_gl_drawable_swap_buffers (gldrawable);
   }
   else{
      glFlush ();
   }

   gdk_gl_drawable_gl_end (gldrawable);
   pthread_mutex_unlock(&gl_mutex);
   return TRUE;
}

static gboolean expose_event (GtkWidget *widget, GdkEventExpose *event, gpointer data) {
   GdkGLContext *glcontext;
   GdkGLDrawable *gldrawable;
   double VUPx, VUPy, VUPz;
 
   VUPx=-cos(3.14/2-(incy/20*2*3.14))*cos(incx/20*2*3.14); // Coordenada X del vector hacia arriba de la cámara
   VUPy=sin(3.14/2-(incy/20*2*3.14)); // Coordenada X del vector hacia arriba de la cámara
   VUPz=-cos(3.14/2-(incy/20*2*3.14))*sin(incx/20*2*3.14); // Coordenada X del vector hacia arriba de la cámara

   pthread_mutex_lock(&gl_mutex);
   
   glcontext = gtk_widget_get_gl_context (widget);
   gldrawable = gtk_widget_get_gl_drawable (widget);
   
   if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext)){
      pthread_mutex_unlock(&gl_mutex);
      return FALSE;
   }

   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity();
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable (GL_DEPTH_TEST);
   glEnable(GL_LIGHTING); /*Habilita la iluminación.*/
   glEnable(GL_NORMALIZE); /*Normaliza las normales.*/

   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

   gluLookAt (eye_posx, eye_posy, eye_posz,0.0,0.0,0.0,VUPx,VUPy,VUPz); // Posicionamos la Cámara
   
   Luz1();
   
   glScalef (escala, escala, escala);

//    glTranslatef(theta*0.1,phi*0.1,-5.0);
//    glRotatef (phi, 1.0, 0.0, 0.0);
//    glRotatef (theta, 0.0, 1.0, 0.0);
   
   Bone();
   
   show_object(&obj);

   if (gdk_gl_drawable_is_double_buffered (gldrawable)){
      gdk_gl_drawable_swap_buffers (gldrawable);
   }
   else{
      glFlush ();
   }

   gdk_gl_drawable_gl_end (gldrawable);

   pthread_mutex_unlock(&gl_mutex);
   return TRUE;
}

static void realize (GtkWidget *widget, gpointer data) {
   pthread_mutex_lock(&gl_mutex);
   {
      GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
      GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);

      /*** OpenGL BEGIN ***/
      if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext)){
         pthread_mutex_unlock(&gl_mutex);
         return;
      }

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      
      glEnable (GL_DEPTH_TEST);
      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);        /* This Will Clear The Background Color To White */
      glClearDepth(1.0);                           /* Enables Clearing Of The Depth Buffer */

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();                            /* Reset The Projection Matrix */

      gdk_gl_drawable_gl_end (gldrawable);
      /*** OpenGL END ***/
   }
   pthread_mutex_unlock(&gl_mutex);
}

static gboolean button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer data) {
   old_x=event->x;
   old_y=event->y;
   return TRUE;
}

static gboolean motion_notify_event (GtkWidget *widget, GdkEventButton *event, gpointer data) {
   float x=event->x;
   float y=event->y;
   incx+=(x-old_x)*(1./escala);
   incy+=(y-old_y)*(1./escala);
   
   if (GDK_BUTTON1_MASK){ /*Si está pulsado el botón 1*/
      eye_posx =80*cos(incy/20*2*3.14)*cos(incx/20*2*3.14); // Posición de la cámara en coord. Polares en X
      eye_posy =80*sin(incy/20*2*3.14); // Posición de la cámara en coord. Polares en Y
      eye_posz =80*cos(incy/20*2*3.14)*sin(incx/20*2*3.14); // Posición de la cámara en coord. Polares en Z
      
      gtk_widget_queue_draw (widget);
      old_x=x;
      old_y=y;
      return TRUE;
   }
   return FALSE;
}

static gboolean scroll_event (GtkRange *range, GdkEventScroll *event, gpointer data){
   if (event->direction == GDK_SCROLL_DOWN){
      escala*=0.95;
   }
   if (event->direction == GDK_SCROLL_UP)
      escala*=1.05;
   gtk_widget_queue_draw(GTK_WIDGET((GtkWidget *)data));
   
   return TRUE;
}

void opengl_viewer_iteration(){
}


/*Importar símbolos*/
void opengl_viewer_imports(){

}

/*Exportar símbolos*/
void opengl_viewer_exports(){
   myexport("opengl_viewer","cycle",&opengl_viewer_cycle);
   myexport("opengl_viewer","resume",(void *)opengl_viewer_resume);
   myexport("opengl_viewer","suspend",(void *)opengl_viewer_suspend);
}

/*Las inicializaciones van en esta parte*/
void opengl_viewer_init(){
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
   obj.vertices=NULL;
   obj.surfaces=NULL;
   /*Load object from file*/
   if (load_object("./esqueleto2.obj", &obj)!=0){
      jdeshutdown(1);
   }
   pthread_mutex_init(&gl_mutex, PTHREAD_MUTEX_TIMED_NP);
}

/*Al suspender el esquema*/
void opengl_viewer_end(){
}

void opengl_viewer_stop(){
}

void opengl_viewer_suspend()
{
  pthread_mutex_lock(&(all[opengl_viewer_id].mymutex));
  put_state(opengl_viewer_id,slept);
  printf("opengl_viewer: off\n");
  pthread_mutex_unlock(&(all[opengl_viewer_id].mymutex));
  opengl_viewer_end();
}


void opengl_viewer_resume(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[opengl_viewer_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[opengl_viewer_id].mymutex));
  /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[opengl_viewer_id].children[i]=FALSE;
  all[opengl_viewer_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) opengl_viewer_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {opengl_viewer_brothers[i]=brothers[i];i++;}
    }
  opengl_viewer_callforarbitration=fn;
  put_state(opengl_viewer_id,notready);
  printf("opengl_viewer: on\n");
  pthread_cond_signal(&(all[opengl_viewer_id].condition));
  pthread_mutex_unlock(&(all[opengl_viewer_id].mymutex));
  opengl_viewer_imports();
}

void *opengl_viewer_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;

   for(;;)
   {
      pthread_mutex_lock(&(all[opengl_viewer_id].mymutex));

      if (all[opengl_viewer_id].state==slept)
      {
	 pthread_cond_wait(&(all[opengl_viewer_id].condition),&(all[opengl_viewer_id].mymutex));
	 pthread_mutex_unlock(&(all[opengl_viewer_id].mymutex));
      }
      else
      {
	 /* check preconditions. For now, preconditions are always satisfied*/
	 if (all[opengl_viewer_id].state==notready)
	    put_state(opengl_viewer_id,ready);
	 /* check brothers and arbitrate. For now this is the only winner */
	 if (all[opengl_viewer_id].state==ready)
	 {put_state(opengl_viewer_id,winner);
	 gettimeofday(&a,NULL);
	 aa=a.tv_sec*1000000+a.tv_usec;
	 n=0;
	 }

	 if (all[opengl_viewer_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	 {
	    pthread_mutex_unlock(&(all[opengl_viewer_id].mymutex));
	    /*      gettimeofday(&a,NULL);*/
	    n++;
	    opengl_viewer_iteration();
	    gettimeofday(&b,NULL);
	    bb=b.tv_sec*1000000+b.tv_usec;
	    next=aa+(n+1)*(long)opengl_viewer_cycle*1000-bb;

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
	    pthread_mutex_unlock(&(all[opengl_viewer_id].mymutex));
	    usleep(opengl_viewer_cycle*1000);
	 }
      }
   }
}

void opengl_viewer_startup(char *configfile)
{
  pthread_mutex_lock(&(all[opengl_viewer_id].mymutex));
  printf("opengl_viewer schema started up\n");
  opengl_viewer_exports();
  put_state(opengl_viewer_id,slept);
  pthread_create(&(all[opengl_viewer_id].mythread),NULL,opengl_viewer_thread,NULL);
  pthread_mutex_unlock(&(all[opengl_viewer_id].mymutex));
  opengl_viewer_init();
}

void opengl_viewer_guidisplay(){
}


void opengl_viewer_guisuspend(void){
   if (win!=NULL){
      gdk_threads_enter();
      gtk_widget_hide(win);
      gdk_threads_leave();
   }
   mydelete_displaycallback(opengl_viewer_guidisplay);
}

void opengl_viewer_guiresume(void){
   static int cargado=0;
   static pthread_mutex_t opengl_viewer_gui_mutex;

   pthread_mutex_lock(&opengl_viewer_gui_mutex);
   if (!cargado){
      loadglade ld_fn;
      GdkGLConfig *glconfig;
      
      cargado=1;
      pthread_mutex_unlock(&opengl_viewer_gui_mutex);
      /*Cargar la ventana desde el archivo xml .glade*/
      gdk_threads_enter();
      if ((ld_fn=(loadglade)myimport("graphics_gtk","load_glade"))==NULL){
         fprintf (stderr,"I can't fetch 'load_glade' from 'graphics_gtk'.\n");
         jdeshutdown(1);
      }
      xml = ld_fn ("opengl_viewer.glade");
      if (xml==NULL){
         fprintf(stderr, "Error loading graphical interface\n");
         jdeshutdown(1);
      }

      /* Try double-buffered visual */
      glconfig = gdk_gl_config_new_by_mode ( GDK_GL_MODE_RGB   |
            GDK_GL_MODE_DEPTH |
            GDK_GL_MODE_DOUBLE);
      if (glconfig == NULL)  {
         g_print ("*** Cannot find the double-buffered visual.\n");
         g_print ("*** Trying single-buffered visual.\n");

         /* Try single-buffered visual */
         glconfig = gdk_gl_config_new_by_mode ( GDK_GL_MODE_RGB   |
               GDK_GL_MODE_DEPTH);
         if (glconfig == NULL) {
            g_print ("*** No appropriate OpenGL-capable visual found.\n");
            jdeshutdown(1);
         }
      }

      drawing_area = glade_xml_get_widget(xml, "3d-area"); /* gtk_drawing_area_new (); */
      gtk_widget_set_size_request (drawing_area, 400, 400);

      gtk_widget_unrealize(drawing_area);
      /* Set OpenGL-capability to the widget. */
      if (gtk_widget_set_gl_capability (drawing_area,
          glconfig,
          NULL,
          TRUE,
          GDK_GL_RGBA_TYPE)==FALSE)
      {
         printf ("No Gl capability\n");
         jdeshutdown(1);
      }
      gtk_widget_realize(drawing_area);

      gtk_widget_set_child_visible (GTK_WIDGET(drawing_area), TRUE);
      gtk_widget_set_child_visible (GTK_WIDGET(glade_xml_get_widget(xml, "vbox1")),
                                    TRUE);
      
      win = glade_xml_get_widget(xml, "window1");
      /*Conectar los callbacks*/
      /*glade_xml_signal_autoconnect (xml);*/
      if (win==NULL){
         fprintf(stderr, "Error loading graphical interface\n");
         jdeshutdown(1);
      }
      else{
         gtk_widget_show(win);
         gtk_widget_queue_draw(GTK_WIDGET(win));
      }

      gtk_widget_add_events ( drawing_area,
                              GDK_BUTTON1_MOTION_MASK    |
                                    GDK_BUTTON2_MOTION_MASK    |
                                    GDK_BUTTON_PRESS_MASK      |
                                    GDK_BUTTON_RELEASE_MASK    |
                                    GDK_VISIBILITY_NOTIFY_MASK);

      g_signal_connect_after (G_OBJECT (drawing_area), "realize", G_CALLBACK (realize), NULL);
      g_signal_connect (G_OBJECT (drawing_area), "configure_event", G_CALLBACK (configure_event), NULL);
      g_signal_connect (G_OBJECT (drawing_area), "expose_event", G_CALLBACK (expose_event), NULL);
      g_signal_connect (G_OBJECT (drawing_area), "button_press_event",
                        G_CALLBACK (button_press_event), NULL);
//       g_signal_connect (G_OBJECT (drawing_area), "button_release_event",
//                         G_CALLBACK (button_release_event), NULL);
      g_signal_connect (G_OBJECT (drawing_area), "motion_notify_event",
                        G_CALLBACK (motion_notify_event), NULL);
      g_signal_connect (G_OBJECT (drawing_area), "scroll-event",
                        G_CALLBACK (scroll_event), drawing_area);

      g_signal_connect (G_OBJECT (glade_xml_get_widget(xml, "diffuse")), "value_changed",
                        G_CALLBACK (on_hscale_move_slider), NULL);
      g_signal_connect (G_OBJECT (glade_xml_get_widget(xml, "ambient")), "value_changed",
                        G_CALLBACK (on_hscale_move_slider), NULL);
      g_signal_connect (G_OBJECT (glade_xml_get_widget(xml, "specular")), "value_changed",
                        G_CALLBACK (on_hscale_move_slider), NULL);
      g_signal_connect (G_OBJECT (glade_xml_get_widget(xml, "x")), "value_changed",
                        G_CALLBACK (on_hscale_move_slider), NULL);
      g_signal_connect (G_OBJECT (glade_xml_get_widget(xml, "y")), "value_changed",
                        G_CALLBACK (on_hscale_move_slider), NULL);
      g_signal_connect (G_OBJECT (glade_xml_get_widget(xml, "z")), "value_changed",
                        G_CALLBACK (on_hscale_move_slider), NULL);
      
      gdk_threads_leave();
   }
   else{
      pthread_mutex_unlock(&opengl_viewer_gui_mutex);
      gdk_threads_enter();
      gtk_widget_show(win);
      gtk_widget_queue_draw(GTK_WIDGET(win));
      gdk_threads_leave();
   }

   myregister_displaycallback(opengl_viewer_guidisplay);
}

