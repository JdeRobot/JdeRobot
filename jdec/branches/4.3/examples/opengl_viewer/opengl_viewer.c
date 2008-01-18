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

#include <pioneeropengl.h>

#define DISTANCE_MAX 25.0

int opengl_viewer_id=0;
int opengl_viewer_brothers[MAX_SCHEMAS];
arbitration opengl_viewer_callforarbitration;

/*Imported variables*/


registerdisplay myregister_displaycallback;
deletedisplay mydelete_displaycallback;

/*Global variables*/
float phi=0, theta=0, escala=1;
float old_x=0, old_y=0;


/* exported variables */
int opengl_viewer_cycle=40; /* ms */

/*GUI Variables*/
GladeXML *xml; /*Fichero xml*/
GtkWidget *win;
GtkWidget *drawing_area;

pthread_mutex_t gl_mutex;

/*Callbacks*/
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
/*    glOrtho(-1, 1, -1, 1, -1, 1);*/

   if (w > h) {
      glViewport (0, (h - w) / 2, w, w);
   }
   else {
      glViewport ((w - h) / 2, 0, h, h);
   }

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glScalef(escala, escala, escala);
   glRotatef (phi, 1.0, 0.0, 0.0);
   glRotatef (theta, 0.0, 1.0, 0.0);
   
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

   GLfloat L0pos[] = { 100, 100, 100 };
   GLfloat L1pos[] = { 100, 100, 100 };
   GLfloat L2pos[] = { 100, 100, 100 };
   GLfloat L3pos[] = { -100, -100, -100 };
   GLfloat L4pos[] = { -100, -100, -100 };
   GLfloat L5pos[] = { -100, -100, -100 };


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
//    gluPerspective (45.0, 1.0, 1, DISTANCE_MAX + 2);
//    gluPerspective (45.0, 1.0, 1, DISTANCE_MAX + 2);

   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   
   glLightfv (GL_LIGHT0, GL_POSITION, L0pos);
   glLightfv (GL_LIGHT1, GL_POSITION, L1pos);
   glLightfv (GL_LIGHT2, GL_POSITION, L2pos);
   glLightfv (GL_LIGHT3, GL_POSITION, L3pos);
   glLightfv (GL_LIGHT4, GL_POSITION, L4pos);
   glLightfv (GL_LIGHT5, GL_POSITION, L5pos);
   
   glRotatef (phi, 1.0, 0.0, 0.0);
   glRotatef (theta, 0.0, 1.0, 0.0);

   glScalef (escala, escala, escala);

   glEnable (GL_DEPTH_TEST);
   glColor3f(1,0.5,1);
   {
      GLfloat angulo;
      int i;
      glBegin(GL_LINES);
      for (i=0; i<360; i+=3)
      {
         angulo = (GLfloat)i*3.14159f/180.0f; // grados a radianes
         glVertex3f(0.0f, 0.0f, 0.0f);
         glVertex3f(cos(angulo), sin(angulo), 0.0f);
      }
      glEnd();
   }
   glColor3f(1.0,1.0,0.);
   glBegin(GL_TRIANGLES);
   {
      glVertex3f(-0.8,-1,-1);
      glVertex3f(1,-0.8,-1);
      glVertex3f(0,1,-1);
   }
   glEnd();
   loadModel();

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

      GLfloat ambient[] = {1.0, 1.0, 1.0, 1.0};
      GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
      GLfloat position[] = {0.0, 3.0, 3.0, 0.0};

      GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
      GLfloat local_view[] = {0.0};

      /*** OpenGL BEGIN ***/
      if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext)){
         pthread_mutex_unlock(&gl_mutex);
         return;
      }

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      
      glEnable (GL_DEPTH_TEST);
      glEnable(GL_TEXTURE_2D);                     /* Enable Texture Mapping */
      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);        /* This Will Clear The Background Color To White */
      glClearDepth(1.0);                           /* Enables Clearing Of The Depth Buffer */

      glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
      glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
      glLightfv (GL_LIGHT0, GL_POSITION, position);
      glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
      glLightModelfv (GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

      glDepthFunc(GL_LESS);                        /* The Type Of Depth Test To Do */
      glEnable (GL_LIGHTING);
      glEnable (GL_LIGHT0);
      glEnable (GL_AUTO_NORMAL);
      glEnable (GL_NORMALIZE);

      glEnable(GL_DEPTH_TEST);                     /* Enables Depth Testing */
      glShadeModel(GL_SMOOTH);                     /* Enables Smooth Color Shading */

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();                            /* Reset The Projection Matrix */

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
   if (GDK_BUTTON1_MASK){ /*Si está pulsado el botón 1*/
      theta -= x - old_x;
      phi -= y - old_y;
      gtk_widget_queue_draw (widget);
      old_x=x;
      old_y=y;
//       gdk_window_invalidate_rect (widget->window, &widget->allocation, FALSE);
   }
}

static gboolean scroll_event (GtkRange *range, GdkEventScroll *event, gpointer data){
   if (event->direction == GDK_SCROLL_DOWN)
      escala-=0.02;
   if (event->direction == GDK_SCROLL_UP)
      escala+=0.02;
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

void opengl_viewer_startup()
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

