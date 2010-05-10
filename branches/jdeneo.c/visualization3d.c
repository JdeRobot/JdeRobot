/*
 *  jdeneo.c
 *
 *  Copyright (C) 2006-2007 Jesús Ruiz-Ayúcar and José María Cañas
 *  for the Universidad Rey Juan Carlos robotic group.
 *  http://www.robotica-urjc.es
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "visualization3d.h"
#include "trackball.h"
#include "pioneer.h"
#include <gtk/gtkgl.h>
#include <GL/gl.h>
#include <GL/glu.h>

const int RES = 180;
const int HEIGHT = 10;
const int RADIO = 2;
const float DEG2RAD = 3.14159/180; 

static float view_quat_diff[4] = { 0.0, 0.0, 0.0, 1.0 };
static float view_quat[4] = { 0.0, 0.0, 0.0, 1.0 };
static float view_scale = 1.0;

/* storage for one texture  */
int texture[1];

/* Image type - contains height, width, and data */
struct Image {
   unsigned long sizeX;
   unsigned long sizeY;
   char *data;
};
typedef struct Image Image;

gint item_laser         = 0;
gint item_sonar         = 1;
gint item_axis          = 2;
gint item_floor         = 3;
#define item_total 4
gboolean viewed_items[item_total];

/* quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  
  See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info. */
int ImageLoad(char *filename, Image *image) {
   FILE *file;
   unsigned long size;                 /* size of the image in bytes. */
   unsigned long i;                    /* standard counter. */
   unsigned short int planes;          /* number of planes in image (must be 1)  */
   unsigned short int bpp;             /* number of bits per pixel (must be 24) */
   char temp;                          /* temporary color storage for bgr-rgb conversion. */

   /* make sure the file is there. */
   if ((file = fopen(filename, "rb"))==NULL)
   {
      printf("File Not Found : %s\n",filename);
      return 0;
   }

   /* seek through the bmp header, up to the width/height: */
   fseek(file, 18, SEEK_CUR);

   /* read the width */
   if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
      printf("Error reading width from %s.\n", filename);
      return 0;
   }
   printf("Width of %s: %lu\n", filename, image->sizeX);

   /* read the height  */
   if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
      printf("Error reading height from %s.\n", filename);
      return 0;
   }
   printf("Height of %s: %lu\n", filename, image->sizeY);

   /* calculate the size (assuming 24 bits or 3 bytes per pixel). */
   size = image->sizeX * image->sizeY * 3;

   /* read the planes */
   if ((fread(&planes, 2, 1, file)) != 1) {
      printf("Error reading planes from %s.\n", filename);
      return 0;
   }
   if (planes != 1) {
      printf("Planes from %s is not 1: %u\n", filename, planes);
      return 0;
   }

   /* read the bpp */
   if ((i = fread(&bpp, 2, 1, file)) != 1) {
      printf("Error reading bpp from %s.\n", filename);
      return 0;
   }
   if (bpp != 24) {
      printf("Bpp from %s is not 24: %u\n", filename, bpp);
      return 0;
   }

   /* seek past the rest of the bitmap header. */
   fseek(file, 24, SEEK_CUR);

   /* read the data. */
   image->data = (char *) malloc(size);
   if (image->data == NULL) {
      printf("Error allocating memory for color-corrected image data");
      return 0;	
   }

   if ((i = fread(image->data, size, 1, file)) != 1) {
      printf("Error reading image data from %s.\n", filename);
      return 0;
   }

   for (i=0;i<size;i+=3) { /* reverse all of the colors. (bgr -> rgb) */
      temp = image->data[i];
      image->data[i] = image->data[i+2];
      image->data[i+2] = temp;
   }

   /* we're done. */
   return 1;
}

/* Load Bitmaps And Convert To Textures */
void LoadGLTextures() {
   /* Load Texture */
   Image *image1;

   /* allocate space for texture */
   image1 = (Image *) malloc(sizeof(Image));
   if (image1 == NULL) {
      printf("Error allocating space for image");
      exit(0);
   }

   if (!ImageLoad("gui/pixmaps/parquet.bmp", image1)) {
      exit(1);
   }

   /* Create Texture */
   glGenTextures(1, &texture[0]);

   /* 2d texture (x and y size) */
   glBindTexture(GL_TEXTURE_2D, texture[0]);

   /* scale linearly when image bigger than texture */
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   /* scale linearly when image smalled than texture */
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

   /* 2d texture, level of detail 0 (normal), 3 components (red, green, blue),
      x size from image, y size from image,  border 0 (normal), rgb color data, 
      unsigned byte data, and finally the data itself. */
   glTexImage2D(  GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,
                  GL_RGB, GL_UNSIGNED_BYTE, image1->data);
}

float robot2xy3d(float i) {
   return i / 100.0;
}

static void init_view (void) {
  view_quat[0] = view_quat_diff[0] = 0.0;
  view_quat[1] = view_quat_diff[1] = 0.0;
  view_quat[2] = view_quat_diff[2] = 0.0;
  view_quat[3] = view_quat_diff[3] = 1.0;
  view_scale = 1.0;
}

static void realize (GtkWidget *widget, gpointer data) {
   GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
   GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);

   GLfloat ambient[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat position[] = {0.0, 3.0, 3.0, 0.0};

   GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
   GLfloat local_view[] = {0.0};

   init_view();
   LoadGLTextures();                            /* Load The Texture(s) */

   /*** OpenGL BEGIN ***/
   if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
      return;

   glEnable(GL_TEXTURE_2D);                     /* Enable Texture Mapping */
   glClearColor(1.0f, 1.0f, 1.0f, 0.0f);        /* This Will Clear The Background Color To White */
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

   gluPerspective(45.0f,(GLfloat)widget->allocation.width/(GLfloat)widget->allocation.height,0.1f,100.0f);	/* Calculate The Aspect Ratio Of The Window */

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glMatrixMode(GL_MODELVIEW);

   gdk_gl_drawable_gl_end (gldrawable);
   /*** OpenGL END ***/
}

static gboolean configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer data) {
   GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
   GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);

   GLfloat w = widget->allocation.width;
   GLfloat h = widget->allocation.height;
   GLfloat aspect;

   /*** OpenGL BEGIN ***/
   if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
      return FALSE;

   glViewport(0, 0, w, h);
   /* Reset The Current Viewport And Perspective Transformation */

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   if (w > h) {
      aspect = w / h;
      glFrustum (-aspect, aspect, -1.0, 1.0, 5.0, 60.0);
   }
   else {
      aspect = h / w;
      glFrustum (-1.0, 1.0, -aspect, aspect, 5.0, 60.0);
   }

   glMatrixMode(GL_MODELVIEW);

   gdk_gl_drawable_gl_end (gldrawable);
   /*** OpenGL END ***/

   return TRUE;
}

static gboolean expose_event (GtkWidget *widget, GdkEventExpose *event, gpointer data) {
   float i, j;
   float matColors[4];
   Tvoxel start, end;
   int k;
   GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
   GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);

   float m[4][4];

   /*** OpenGL BEGIN ***/
   if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
      return FALSE;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();

   glTranslatef(0.0f,0.0f,-10.0);
   glScalef (view_scale, view_scale, view_scale);
   add_quats (view_quat_diff, view_quat, view_quat);
   build_rotmatrix (m, view_quat);
   glMultMatrixf (&m[0][0]);

   glTranslatef(robot2xy3d(-robot[0]), robot2xy3d(-robot[1]), 0.0);

   /* Dibujamos al robot */
   glPushMatrix();
      /* Lo coloca en su nueva posicion (robot[0],robot[1]) */
      glTranslatef(robot2xy3d(robot[0]), robot2xy3d(robot[1]), 0.0);

      /* Lo giramos todo para que apunte a su nueva posici� */
      glRotatef((robot[2] * RADTODEG) + 90.0, 0.0f, 0.0f, 1.0f);

      /* Colocamos el cuello donde proceda */
      glPushMatrix();
         glTranslatef(0.0f, 0.0f, 0.0f);
         /*load_5(); * Base cuello */
         /*load_6(); * Panel cuello */
         glPushMatrix();
            glTranslatef(-0.1f, -1.8f, 0.0f);  /* Lo llevamos a su sitio */
            glRotatef(45.0, 0.0f, 0.0f, 1.0f); /* Lo giramos */
            glTranslatef(0.1f, 1.8f, 0.0f);    /* Lo llevamos al centro */
            /*load_4(); * Base giro */
            glPushMatrix();
               glTranslatef(-0.1f, -1.8f, 5.0f);  /* Lo llevamos a su sitio */
               glRotatef(15.0, 1.0f, 0.0f, 0.0f); /* Lo giramos */
               glTranslatef(0.1f, 1.8f, -5.0f);   /* Lo llevamos al centro */
               /*load_2(); * Camaras */
               /*load_0(); * Base de c�aras */
               /*load_3(); * Giro cuello */
            glPopMatrix();
         glPopMatrix();
      glPopMatrix();

      load_0();   /** Plato_ */
      load_1();   /** Rueda_ */
      load_2();   /** Rueda_ */
      load_3();   /** Plato_ */
      load_4();   /** Cámaras_ */
      load_5();   /** Base de cámaras_ */
      load_6();   /** Giro cuello */
      load_7();   /** Base giro_ */
      load_8();   /** Base cuello_ */
      load_9();   /** Panel cuello_ */
      load_10();  /** Laser_ */
      load_12();  /** Paneles_ */
      load_13();
      load_14();  /** Sonares_ */
      load_15();  /** Cuerpo_ */
      load_16();  /** Soporte Sonares_ */
      load_17();  /** Base */

   glPopMatrix();

   if (viewed_items[item_sonar]) {
      /* Dibujamos los sonares */
      matColors[0] = 1.0;
      matColors[1] = 0.0;
      matColors[2] = 0.0;
      matColors[3] = 1.0;
      for (k = 0; k < NUM_SONARS; k++) {
         us2xy(k, 0.0f, 0.0f, &start);
         us2xy(k, us[k], 0.0f, &end);
         glBegin(GL_LINES);
            glVertex3f (robot2xy3d(start.x), robot2xy3d(start.y), 2.0f);
            glVertex3f (robot2xy3d(end.x),   robot2xy3d(end.y),   2.0f);
         glEnd();
      }
   }

   if (viewed_items[item_laser]) {
      /* Dibujamos los laser */
      matColors[0] = 1.0;
      matColors[1] = 0.0;
      matColors[2] = 0.0;
      matColors[3] = 0.5;
      glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matColors);
      for(k = 0; k < NUM_LASER - 1; k++) {
         glBegin(GL_POLYGON);
            glVertex3f (robot2xy3d(start.x), robot2xy3d(start.y), 3.2f);
            laser2xy(k, laser[k], &end);
            glVertex3f (robot2xy3d(end.x), robot2xy3d(end.y), 3.2f);
            laser2xy(k + 1, laser[k + 1], &end);
            glVertex3f (robot2xy3d(end.x), robot2xy3d(end.y), 3.2f);
         glEnd();
      }
   }

   if (viewed_items[item_floor]) {
      /* Dibujamos el suelo */
      matColors[0] = 1.0;
      matColors[1] = 1.0;
      matColors[2] = 1.0;
      matColors[3] = 1.0;
      glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matColors);
      glBindTexture(GL_TEXTURE_2D, texture[0]);
      for(i = -320.0; i <= 320.0; i += 16.0)
         for(j = -320.0; j <= 320.0; j += 16.0) {
            glBegin(GL_QUADS);
               glTexCoord2f   (0.0f, 0.0f);
               glVertex3f     (i, j, 0.0f);
               glTexCoord2f   (1.0f, 0.0f);
               glVertex3f     (i+16.0, j, 0.0f);
               glTexCoord2f   (1.0f, 1.0f);
               glVertex3f     (i+16.0, j+16.0, 0.0f);
               glTexCoord2f   (0.0f, 1.0f);
               glVertex3f     (i, j+16.0, 0.0f);
            glEnd();
         }
   }

   if (viewed_items[item_axis]) {
      /* Nos pintamos los ejes */
      glLineWidth(5.0f);
      matColors[0] = 1.0;
      matColors[1] = 0.0;
      matColors[2] = 0.0;
      matColors[3] = 1.0;
      glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matColors);
      glBegin(GL_LINES);
         glVertex3f     (0.0f, 0.0f, 0.0f);
         glVertex3f     (500.0f, 0.0f, 0.0f);
      glEnd();

      matColors[0] = 0.0;
      matColors[1] = 1.0;
      matColors[2] = 0.0;
      matColors[3] = 1.0;
      glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matColors);
      glBegin(GL_LINES);
         glVertex3f     (0.0f, 0.0f, 0.0f);
         glVertex3f     (0.0f, 500.0f, 0.0f);
      glEnd();

      matColors[0] = 0.0;
      matColors[1] = 0.0;
      matColors[2] = 1.0;
      matColors[3] = 1.0;
      glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matColors);
      glBegin(GL_LINES);
         glVertex3f     (0.0f, 0.0f, 0.0f);
         glVertex3f     (0.0f, 0.0f, 500.0f);
      glEnd();
   }

   if (gdk_gl_drawable_is_double_buffered (gldrawable))
      gdk_gl_drawable_swap_buffers (gldrawable);
   else
      glFlush ();

   gdk_gl_drawable_gl_end (gldrawable);
   /*** OpenGL END ***/

   return TRUE;
}

static gboolean idle (GtkWidget *widget) {
   if (widget->window != NULL) {
      /* Invalidate the whole window. */
      gdk_window_invalidate_rect (widget->window, &widget->allocation, FALSE);

      /* Update synchronously. */
      gdk_window_process_updates (widget->window, FALSE);
   }

   return TRUE;
}

static guint idle_id = 0;

static void idle_add (GtkWidget *widget) {
   if (idle_id == 0) {
      idle_id = g_timeout_add_full (G_PRIORITY_LOW,
                                 100,
                                 (GSourceFunc) idle,
                                 widget,
                                 NULL);
   }
}

static float begin_x = 0.0;
static float begin_y = 0.0;
static float dx = 0.0;
static float dy = 0.0;

static gboolean button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer data) {
   begin_x = event->x;
   begin_y = event->y;

   view_quat_diff[0] = 0.0;
   view_quat_diff[1] = 0.0;
   view_quat_diff[2] = 0.0;
   view_quat_diff[3] = 1.0;

   return FALSE;
}

static gboolean button_release_event (GtkWidget *widget, GdkEventButton *event, gpointer data) {
   dx = 0.0;
   dy = 0.0;

   return FALSE;
}

static gboolean motion_notify_event (GtkWidget *widget, GdkEventMotion *event, gpointer data) {
   float w = widget->allocation.width;
   float h = widget->allocation.height;
   float x = event->x;
   float y = event->y;
   gboolean redraw = FALSE;

   /* Rotation. */
   if (event->state & GDK_BUTTON1_MASK)
   {
      trackball   (view_quat_diff,
                  (2.0 * begin_x - w) / w,
                  (h - 2.0 * begin_y) / h,
                  (2.0 * x - w) / w,
                  (h - 2.0 * y) / h);

      dx = x - begin_x;
      dy = y - begin_y;

      redraw = TRUE;
   }

   begin_x = x;
   begin_y = y;

   if (redraw)
      gdk_window_invalidate_rect (widget->window, &widget->allocation, FALSE);

   return TRUE;
}

static gboolean scroll_event (GtkRange *range, GdkEventScroll *event, gpointer data) {
   if (event->direction == GDK_SCROLL_DOWN)
      view_scale-=0.02;
   if (event->direction == GDK_SCROLL_UP)
      view_scale+=0.02;
   gtk_widget_queue_draw(GTK_WIDGET((GtkWidget *)data));

   return TRUE;
}

/* Hebra que redibuja el contenido del drawing_area */
gboolean redrawScene(gpointer data) {
   gtk_widget_queue_draw(GTK_WIDGET((GtkWidget *)data));
   return (gboolean)TRUE;
}

static void change_viewed_item (GtkMenuItem *menuitem, gint *item) {
   viewed_items[*item] = gtk_check_menu_item_get_active((GtkCheckMenuItem *)menuitem);
}

/* For popup menu. */
static gboolean button_press_event_popup_menu (GtkWidget *widget, GdkEventButton *event, gpointer data) {
   if (event->button == 3)
   {
      /* Popup menu. */
      gtk_menu_popup (GTK_MENU (widget), NULL, NULL, NULL, NULL,
                     event->button, event->time);
      return TRUE;
   }
   return FALSE;
}

GtkWidget *create_popup_menu(GtkWidget *drawing_area) {
   GtkWidget *menu;
   GtkWidget *menu_item;

   menu = gtk_menu_new();

   /* Laser */
   menu_item = gtk_check_menu_item_new_with_label ("Laser");
   gtk_check_menu_item_set_active((GtkCheckMenuItem *)menu_item, TRUE);
   gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
   g_signal_connect (G_OBJECT (menu_item), "activate",
                     G_CALLBACK (change_viewed_item), (gpointer) &item_laser);
   gtk_widget_show (menu_item);


   /* Sonar */
   menu_item = gtk_check_menu_item_new_with_label ("Sonar");
   gtk_check_menu_item_set_active((GtkCheckMenuItem *)menu_item, TRUE);
   gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
   g_signal_connect (G_OBJECT (menu_item), "activate",
                     G_CALLBACK (change_viewed_item), (gpointer) &item_sonar);
   gtk_widget_show (menu_item);


   /* Axis */
   menu_item = gtk_check_menu_item_new_with_label ("Axis");
   gtk_check_menu_item_set_active((GtkCheckMenuItem *)menu_item, TRUE);
   gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
   g_signal_connect (G_OBJECT (menu_item), "activate",
                     G_CALLBACK (change_viewed_item), (gpointer) &item_axis);
   gtk_widget_show (menu_item);


   /* Floor */
   menu_item = gtk_check_menu_item_new_with_label ("Floor");
   gtk_check_menu_item_set_active((GtkCheckMenuItem *)menu_item, TRUE);
   gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
   g_signal_connect (G_OBJECT (menu_item), "activate",
                     G_CALLBACK (change_viewed_item), (gpointer) &item_floor);
   gtk_widget_show (menu_item);

   return menu;
}

void start3d () {
   GdkGLConfig *glconfig;
   gint major, minor;
   GtkWidget *menu;
   GtkWidget *drawing_area;
   int i;

   /*
   * Default viewed items
   */
   for (i = 0; i < item_total ; i++)
      viewed_items[i] = TRUE;

   /*
   * Query OpenGL extension version.
   */

   gdk_gl_query_version (&major, &minor);
   g_print ("\nOpenGL extension version - %d.%d\n", major, minor);

   /*
   * Configure OpenGL-capable visual.
   */

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
         exit (1);
      }
   }

   /*
   * Drawing area for drawing OpenGL scene.
   */

   drawing_area = glade_xml_get_widget(xml, "drawingarea3d"); /* gtk_drawing_area_new (); */
   gtk_widget_set_size_request (drawing_area, 400, 400);

   /* Set OpenGL-capability to the widget. */
   gtk_widget_set_gl_capability (drawing_area,
                                 glconfig,
                                 NULL,
                                 TRUE,
                                 GDK_GL_RGBA_TYPE);

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
   g_signal_connect (G_OBJECT (drawing_area), "button_release_event",
                           G_CALLBACK (button_release_event), NULL);
   g_signal_connect (G_OBJECT (drawing_area), "motion_notify_event",
                           G_CALLBACK (motion_notify_event), NULL);
   g_signal_connect (G_OBJECT (drawing_area), "scroll-event",
                           G_CALLBACK (scroll_event), drawing_area);

   /* Popup menu */
   menu = create_popup_menu (drawing_area);
   g_signal_connect_swapped (G_OBJECT (drawing_area), "button_press_event",
                            G_CALLBACK (button_press_event_popup_menu), menu);


   gtk_widget_show (drawing_area);

   idle_add(drawing_area);
}
