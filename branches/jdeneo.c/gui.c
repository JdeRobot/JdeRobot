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

#include "gui.h"
#include "jde.h"
#include "jdeservers.h"
#include "visualization3d.h"
#include "cellrendererspin.h"
#include <libgnomecanvas/libgnomecanvas.h>

/*************************************
 *      Variables para el canvas     *
 *************************************/
GnomeCanvas *canvas;
GnomeCanvasPoints *center;
float odometrico[5];
float escala=0.1, width = 270, height = 270;

/* Para el laser */
GnomeCanvasItem *recta = NULL;
GnomeCanvasPoints * coordenadas;

/* Para el sonar */
GnomeCanvasItem *sonars_lines[NUM_SONARS];
GnomeCanvasPoints *sonars_points[NUM_SONARS];

/* Para el robot */
#define EGOMAX 5
GnomeCanvasItem *ego_pol = NULL;
GnomeCanvasItem *ego_bord_pol = NULL;
GnomeCanvasPoints *ego;
GnomeCanvasPoints *ego_bord;
int numego=0;

/*************************************
 *   Variables para los positioner   *
 *************************************/
GnomeCanvas *pos1, *pos2;
float widthPos = 150, heightPos = 90;
double basex, basey;

/* Para el positioner1 (base) */
#define joystick_maxRotVel 30 /* deg/sec */
#define joystick_maxTranVel 200 /* mm/sec */
GnomeCanvasItem *pos1h, *pos1v;
GnomeCanvasPoints *pos1ph, *pos1pv;
float joystick_x, joystick_y;

/* Para el positioner2 (pantilt) */
GnomeCanvasItem *pos2h, *pos2v;
GnomeCanvasPoints *pos2ph, *pos2pv;

/*************************************
 *   Variables de los callbacks      *
 *************************************/
int imageA_cb=0;
int imageB_cb=0;
int encoders_cb=0;
int laser_cb=0;
int sonars_cb=0;
int pantiltencoders_cb=0;

/*************************************
 *   Headers necesarios              *
 *************************************/
void loadSchema(GtkWidget *widget __attribute__ ((unused)), gpointer user_data);
void readVideoA();
void readVideoB();
void drawLaser();
void drawSonars();
void drawRobot();
void reasignVW();


/*************************************
 *   Callbacks de la GUI             *
 *************************************/

gboolean on_base_positioner_button_press (  GtkWidget      *widget,
                                       GdkEventButton *event,
                                       gpointer        user_data) {
   double wx, wy;
   int cx, cy;

   if (event->type == GDK_BUTTON_RELEASE) {
      gnome_canvas_window_to_world (pos1, event->x, event->y, &wx, &wy);
      gnome_canvas_w2c(pos1, wx, wy, &cx, &cy);

      if (cx >= 0 && cx <= widthPos) {
         pos1ph->coords[0] = 0;              /* x1 */
         pos1ph->coords[1] = cy;             /* y1 */
         pos1ph->coords[2] = widthPos;       /* x2 */
         pos1ph->coords[3] = cy;             /* y2 */

         pos1pv->coords[0] = cx;             /* x1 */
         pos1pv->coords[1] = 0;              /* y1 */
         pos1pv->coords[2] = cx;             /* x2 */
         pos1pv->coords[3] = heightPos;      /* y2 */

         /* Dibujamos la linea horizontal */
         gnome_canvas_item_set(pos1h,
                              "points", pos1ph,
                              "fill_color", "red",
                              "width_pixels", 2,
                              NULL);
         /* Dibujamos la linea vertical */
         gnome_canvas_item_set(pos1v,
                              "points", pos1pv,
                              "fill_color", "red",
                              "width_pixels", 2,
                              NULL);
         /* Pasamos a nuevas coordenadas:
            esquina inferior izquierda (0,0)
            esquina superior izquierda (0,1)
            esquina inferior derecha   (1,0)
            esquena superior derecha   (1,1) */
         if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "base_play")))) {
            basex = (cx / widthPos <= 1)?(cx / widthPos):1;
            basey = (1 - (cy / heightPos)<=1)?(1 - (cy / heightPos)):1;

            if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "base_back"))))
               joystick_y=0.5+0.5*basey;
            else 
               joystick_y=0.5-0.5*basey;
            joystick_x=basex;

            reasignVW();
         }
      }
   }
   return TRUE;
}

gboolean on_pantilt_positioner_button_press (  GtkWidget      *widget,
                                       GdkEventButton *event,
                                       gpointer        user_data) {
   g_print("Dentro de pantilt! %.3f, %.3f\n", event->x, event->y);
   return TRUE;
}

void on_canvas_scale_value_change ( GtkRange *range, gpointer user_data) {
   /* Usamos variable de escala en lugar de la herramienta de gnomeCanvas
      (set_pixels_per_unit) por compatibilidad con la version anterior de jde.c */
   escala = width / (gtk_range_get_value(range) * 1000);
   /*gnome_canvas_set_pixels_per_unit(canvas, 20 - gtk_range_get_value(range));*/
}

void on_open_activate() {
   GtkWidget *pFileSelection;

    /* Creación de la ventana de selección */
   pFileSelection = gtk_file_selection_new("Abrir Esquema");

    /* Limitar las acciones a esta ventana */
   gtk_window_set_modal(GTK_WINDOW(pFileSelection), TRUE);

   g_signal_connect (GTK_FILE_SELECTION (pFileSelection)->ok_button,
                     "clicked",
                     G_CALLBACK (loadSchema),
                     pFileSelection);

   /* Ensure that the dialog box is destroyed when the user clicks a button. */

   g_signal_connect_swapped (GTK_FILE_SELECTION (pFileSelection)->ok_button,
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy), 
                             pFileSelection);

   g_signal_connect_swapped (GTK_FILE_SELECTION (pFileSelection)->cancel_button,
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy),
                             pFileSelection);

   gtk_widget_show(pFileSelection);
}

void on_open_clicked (  GtkToolButton *unused_button, gpointer unused_user_data) {
   on_open_activate ();
}

void on_quit_activate() {
   jdeshutdown(0);
}

void on_quit_clicked (  GtkToolButton *tool, gpointer user_data) {
   on_quit_activate ();
}

void on_imageA_yes_toggled(GtkToggleButton *togg, gpointer user_data) {
   GtkImage *image = GTK_IMAGE(glade_xml_get_widget(xml, "imageA"));
   char imageAfile[MAX_MESSAGE];

   if (gtk_toggle_button_get_active(togg)) {
      imageA_cb = imageA_resume(readVideoA);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "imageA_play"), TRUE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "imageA_fps"), TRUE);
   }
   else {
      imageA_suspend(imageA_cb);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "imageA_play"), FALSE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "imageA_fps"), FALSE);
      sprintf(imageAfile, "%s%s", RESOURCEDIR, IMAGEAFILE);
      gtk_image_set_from_file(image, imageAfile);
   }
}

void on_imageB_yes_toggled(GtkToggleButton *togg, gpointer user_data) {
   GtkImage *image = GTK_IMAGE(glade_xml_get_widget(xml, "imageB"));
   char imageBfile[MAX_MESSAGE];

   if (gtk_toggle_button_get_active(togg)) {
      imageB_cb = imageB_resume(readVideoB);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "imageB_play"), TRUE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "imageB_fps"), TRUE);
   }
   else {
      imageB_suspend(imageB_cb);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "imageB_play"), FALSE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "imageB_fps"), FALSE);
      sprintf(imageBfile, "%s%s", RESOURCEDIR, IMAGEBFILE);
      gtk_image_set_from_file(image, imageBfile);
   }
}

void on_imageA_play_toggled(GtkToggleButton *togg, gpointer user_data) {
   GtkImage *image = GTK_IMAGE(glade_xml_get_widget(xml, "imageA"));
   GdkPixbuf *imgBuff;
   char imageAfile[MAX_MESSAGE];

   if (gtk_toggle_button_get_active(togg)){
      /* La primera vez escribimos desde colorA sin esperar al callback 
         por si el dispositivo fuera de tipo file */
      imgBuff = gdk_pixbuf_new_from_data(colorA,GDK_COLORSPACE_RGB,0,8,320,240,960,NULL,NULL);

      gtk_image_set_from_pixbuf(image, imgBuff);
      gtk_widget_queue_draw(GTK_WIDGET(image));
   }
   else {
      sprintf(imageAfile, "%s%s", RESOURCEDIR, IMAGEAFILE);
      gtk_image_set_from_file(image, imageAfile);
   }
}

void on_imageB_play_toggled(GtkToggleButton *togg, gpointer user_data) {
   GtkImage *image = GTK_IMAGE(glade_xml_get_widget(xml, "imageB"));
   GdkPixbuf *imgBuff;
   char imageBfile[MAX_MESSAGE];

   if (gtk_toggle_button_get_active(togg)) {
      /* La primera vez escribimos desde colorB sin esperar al callback 
         por si el dispositivo fuera de tipo file */
      imgBuff = gdk_pixbuf_new_from_data(colorB,GDK_COLORSPACE_RGB,0,8,320,240,960,NULL,NULL);

      gtk_image_set_from_pixbuf(image, imgBuff);
      gtk_widget_queue_draw(GTK_WIDGET(image));
   }
   else {
      sprintf(imageBfile, "%s%s", RESOURCEDIR, IMAGEBFILE);
      gtk_image_set_from_file(image, imageBfile);
   }
}

void on_robot_yes_toggled(GtkToggleButton *togg, gpointer user_data) {
   if (gtk_toggle_button_get_active(togg)) {
      encoders_cb = encoders_resume(drawRobot);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "robot_play"), TRUE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "robot_fps"), TRUE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "laser_yes"), TRUE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "sonars_yes"), TRUE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "pantilt_yes"), TRUE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "motor_yes"), TRUE);
   }
   else {
      encoders_suspend(encoders_cb);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "robot_play"), FALSE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "robot_fps"), FALSE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "laser_yes"), FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(
         glade_xml_get_widget(xml, "laser_yes")), FALSE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "sonars_yes"), FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(
         glade_xml_get_widget(xml, "sonars_yes")), FALSE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "pantilt_yes"), FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(
         glade_xml_get_widget(xml, "pantilt_yes")), FALSE);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(
         glade_xml_get_widget(xml, "motor_yes")), FALSE);

      /* Borramos la imagen del robot */
      gnome_canvas_item_set(ego_pol,
                        "fill_color-rgba", 0x00000000,
                        "outline_color-rgba", 0x00000000,
                        NULL);
   }
}

void on_laser_yes_toggled(GtkToggleButton *togg, gpointer user_data) {
   if (gtk_toggle_button_get_active(togg)) {
      laser_cb=laser_resume(drawLaser);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "laser_play"), TRUE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "laser_fps"), TRUE);
   }
   else {
      laser_suspend(laser_cb);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "laser_play"), FALSE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "laser_fps"), FALSE);

      /* Quitamos la imagen del laser */
      gnome_canvas_item_set(recta,
                           "fill_color-rgba", 0x00000000,
                           "outline_color-rgba", 0x00000000,
                           NULL);
   }
}

void on_sonars_yes_toggled(GtkToggleButton *togg, gpointer user_data) {
   int i;

   if (gtk_toggle_button_get_active(togg)) {
      sonars_cb = sonars_resume(drawSonars);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "sonars_play"), TRUE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "sonars_fps"), TRUE);
   }
   else {
      sonars_suspend(sonars_cb);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "sonars_play"), FALSE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "sonars_fps"), FALSE);

      /* Quitamos las lineas de los sonars */
      for (i = 0; i < NUM_SONARS; i++) {
         gnome_canvas_item_set(sonars_lines[i],
                              "fill_color-rgba", 0x00000000,
                              NULL);
      }
   }
}

void on_pantilt_yes_toggled(GtkToggleButton *togg, gpointer user_data) {
   if (gtk_toggle_button_get_active(togg)) {
      pantiltencoders_cb = pantiltencoders_resume(NULL);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "pantilt_play"), TRUE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "pantilt_fps"), TRUE);
   }
   else {
      pantiltencoders_suspend(pantiltencoders_cb);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "pantilt_play"), FALSE);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "pantilt_fps"), FALSE);
   }
}

void on_motor_yes_toggled(GtkToggleButton *togg, gpointer user_data) {
   if (gtk_toggle_button_get_active(togg)) {
      motors_resume(NULL, NULL);
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "motor_play"), TRUE);
   }
   else {
      motors_suspend();
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "motor_play"), FALSE);
   }
}

void on_motor_play_toggled(GtkToggleButton *togg, gpointer user_data ) {
  if (gtk_toggle_button_get_active(togg)) {
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "base_yes"), TRUE);
   }
   else {
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "base_yes"), FALSE);
   }
}

void on_base_yes_toggled(GtkToggleButton *togg, gpointer user_data ) {
   joystick_x=0.5;
   joystick_y=0.5;
   if (gtk_toggle_button_get_active(togg)) {
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "base_play"), TRUE);
   }
   else {
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "base_play"), FALSE);
   }
}

void on_base_play_toggled(GtkToggleButton *togg, gpointer user_data ) {
   static gboolean inicial = TRUE;

   if (gtk_toggle_button_get_active(togg)) {
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "base_back"), TRUE);
      if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "base_back"))))
         joystick_y=0.5+0.5*basey;
      else 
         joystick_y=0.5-0.5*basey;
      joystick_x=basex;
   }
   else {
      gtk_widget_set_sensitive(glade_xml_get_widget(xml, "base_back"), FALSE);
      joystick_x=0.5;
      joystick_y=0.5;
   }

   if (inicial)
      inicial = FALSE;
   else
      reasignVW();
}

void on_base_back_toggled(GtkToggleButton *togg, gpointer user_data ) {
   if (gtk_toggle_button_get_active(togg)) {
      joystick_y=0.5+0.5*basey;
   }
   else {
      joystick_y=0.5-0.5*basey;
   }
   joystick_x=basex;

   reasignVW();
}

void on_menu_map_activate() {
   if (!gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(xml, "toggle_map")))) {
      gtk_widget_hide(glade_xml_get_widget(xml, "map_frame"));
   }
   else {
      gtk_widget_show(glade_xml_get_widget(xml, "map_frame"));
   }
}

void on_menu_teleoperator_activate() {
   if (!gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(xml, "toggle_teleoperator")))) {
      gtk_widget_hide(glade_xml_get_widget(xml, "teleoperator_frame"));
   }
   else {
      gtk_widget_show(glade_xml_get_widget(xml, "teleoperator_frame"));
   }
}

void on_menu_visualization_activate() {
   if (!gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(xml, "toggle_visualization")))) {
      gtk_widget_hide(glade_xml_get_widget(xml, "visualization_frame"));
   }
   else {
      gtk_widget_show(glade_xml_get_widget(xml, "visualization_frame"));
   }
}

void on_menu_devices_activate() {
   if (!gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(xml, "toggle_devices")))) {
      gtk_widget_hide(glade_xml_get_widget(xml, "devices_frame"));
   }
   else {
      gtk_widget_show(glade_xml_get_widget(xml, "devices_frame"));
   }
}


/*************************************
 *   Funciones que solo usa la gui   *
 *************************************/

/* return -1 if point falls outside the canvas */
int xy2canvas(Tvoxel point, GnomeCanvasPoints* polygon, int i) {
   float xi, yi;

   xi = (point.x * odometrico[3] - point.y * odometrico[4] + odometrico[0])*escala;
   yi = (point.x * odometrico[4] + point.y * odometrico[3] + odometrico[1])*escala;
   /* Con esto cambiamos al sistema de referencia de visualizacion, centrado en algun punto xy y con alguna orientacion definidos por odometrico. Ahora cambiamos de ese sistema al del display, donde siempre hay un desplazamiento a la esquina sup. izda. y que las y se cuentan para abajo. */

   polygon->coords[i*2]       = xi  + width/4;
   polygon->coords[(i*2) + 1] = -yi + height/4;

   if ((polygon->coords[i*2]       < 0) || (polygon->coords[i*2]       > width))
      return -1; 
   if ((polygon->coords[(i*2) + 1] < 0) || (polygon->coords[(i*2) + 1] > height))
      return -1; 
   return 0;
}

void reasignVW() {
   double delta, deltapos;
   delta = (joystick_x-0.5)*2; /* entre +-1 */
   deltapos = fabs(delta); /* Para que no moleste el signo de delta en el factor de la funcion de control */

   if (delta<0)
      w = (float) joystick_maxRotVel*deltapos*deltapos*deltapos;
   else
      w = (float) -1.*joystick_maxRotVel*deltapos*deltapos*deltapos;

   /* TRASLACION=ejeY: Ajusta a un % de +-joystick_maxTranVel. OJO no funcion lineal del desplazamiento visual, sino con el a la cuarta, para aplanarla en el origen */

   delta = (joystick_y-0.5)*2; /* entre +-1 */
   deltapos = fabs(delta);/* Para que no moleste el signo de delta en el factor de la funcion de control */

   if (delta<0)
      v = (float) -1.*joystick_maxTranVel*deltapos*deltapos*deltapos;
   else
      v = (float) joystick_maxTranVel*deltapos*deltapos*deltapos;
}

/* Puesto que gtkCanvas no está incluido como widget en un
   proyecto glade gtk, lo tenemos que inicializar a mano.
   En Glade decimos a nuestro custom widget que se cree con
   esta función */
GtkWidget *create_canvas(gchar *widget_name,
                           gchar *string1,
                           gchar *string2,
                           gint int1,
                           gint int2) {
   int i;
   Tvoxel kaka;

   gtk_widget_pop_colormap();
   gtk_widget_pop_visual();

   /* Inicializamos el canvas */
   canvas = GNOME_CANVAS(gnome_canvas_new_aa());
   gtk_widget_set_size_request (GTK_WIDGET(canvas), width, height);
   gtk_widget_show(GTK_WIDGET(canvas));
   gnome_canvas_set_scroll_region(GNOME_CANVAS(canvas), -300, -300, 300, 300);
   gnome_canvas_set_center_scroll_region(GNOME_CANVAS(canvas), TRUE);
   center = gnome_canvas_points_new(2);

   /* Dibujamos el fondo blanco del canvas*/
   gdk_threads_enter();
   gnome_canvas_item_new(gnome_canvas_root(canvas),
                        GNOME_TYPE_CANVAS_RECT,
                        "x1", -300.0,
                        "y1", -300.0,
                        "x2", 300.0,
                        "y2", 300.0,
                        "fill_color", "white",
                        NULL);
   gdk_threads_leave();

   /* Añadimos poligono de laser */
   coordenadas = gnome_canvas_points_new(NUM_LASER+1);
   for(i=0 ; i < NUM_LASER; i++) {
      laser2xy(i, laser[i], &kaka);
      xy2canvas(kaka, coordenadas, i);
   }
   gdk_threads_enter();
   recta = gnome_canvas_item_new(gnome_canvas_root(canvas),
                           GNOME_TYPE_CANVAS_POLYGON,
                           NULL);
   gdk_threads_leave();

   /* Añadimos lineas de los sonars */
   for (i = 0; i < NUM_SONARS; i++) {
      sonars_points[i] = gnome_canvas_points_new(2);
      us2xy(i,0.,0.,&kaka);
      xy2canvas(kaka, sonars_points[i], 0);
      us2xy(i,us[i],0.,&kaka);
      xy2canvas(kaka, sonars_points[i], 1);
      gdk_threads_enter();
      sonars_lines[i] = gnome_canvas_item_new(gnome_canvas_root(canvas),
                           GNOME_TYPE_CANVAS_LINE,
                           "points", sonars_points[i],
                           "last-arrowhead", TRUE,
                           "arrow-shape-a", 4.0,
                           "arrow-shape-b", 4.0,
                           "arrow-shape-c", 4.0,
                           NULL);
      gdk_threads_leave();
   }

   /* Añadimos poligono del robot */
   ego = gnome_canvas_points_new(EGOMAX);
   ego_bord = gnome_canvas_points_new(NUM_SONARS);
   gdk_threads_enter();
   ego_pol = gnome_canvas_item_new(gnome_canvas_root(canvas),
                           GNOME_TYPE_CANVAS_POLYGON,
                           NULL);
   ego_bord_pol = gnome_canvas_item_new(gnome_canvas_root(canvas),
                           GNOME_TYPE_CANVAS_POLYGON,
                           NULL);
   gdk_threads_leave();

   return GTK_WIDGET(canvas);
}

GtkWidget *create_base_positioner( gchar *widget_name,
                              gchar *string1,
                              gchar *string2,
                              gint int1,
                              gint int2) {

   GnomeCanvasItem *background;

   /* Inicializamos el canvas */
   pos1 = GNOME_CANVAS(gnome_canvas_new_aa());
   gtk_widget_set_size_request (GTK_WIDGET(pos1), widthPos, heightPos);
   gtk_widget_show(GTK_WIDGET(pos1));
   pos1ph = gnome_canvas_points_new(2);
   pos1pv = gnome_canvas_points_new(2);

   pos1ph->coords[0] = 0;              /* x1 */
   pos1ph->coords[1] = heightPos-1;    /* y1 */
   pos1ph->coords[2] = widthPos;       /* x2 */
   pos1ph->coords[3] = heightPos-1;    /* y2 */

   pos1pv->coords[0] = widthPos / 2;   /* x1 */
   pos1pv->coords[1] = 1;              /* y1 */
   pos1pv->coords[2] = widthPos / 2;   /* x2 */
   pos1pv->coords[3] = heightPos-1;    /* y2 */

   gdk_threads_enter();
   /* Dibujamos el fondo blanco de borde negro */
   background = gnome_canvas_item_new(gnome_canvas_root(pos1),
                           GNOME_TYPE_CANVAS_RECT,
                           "x1", 1.0,
                           "y1", 1.0,
                           "x2", widthPos,
                           "y2", heightPos,
                           "outline_color", "black",
                           "fill_color", "white",
                           NULL);
   /* Dibujamos la linea horizontal */
   pos1h = gnome_canvas_item_new(gnome_canvas_root(pos1),
                           GNOME_TYPE_CANVAS_LINE,
                           "points", pos1ph,
                           "fill_color", "red",
                           "width_pixels", 2,
                           NULL);
   /* Dibujamos la linea vertical */
   pos1v = gnome_canvas_item_new(gnome_canvas_root(pos1),
                           GNOME_TYPE_CANVAS_LINE,
                           "points", pos1pv,
                           "fill_color", "red",
                           "width_pixels", 2,
                           NULL);
   gdk_threads_leave();
   gnome_canvas_set_scroll_region(pos1, 0.0, 0.0, widthPos, heightPos);
   gnome_canvas_set_center_scroll_region(pos1, TRUE);
   g_signal_connect (G_OBJECT (pos1), "button-release-event",
                  G_CALLBACK (on_base_positioner_button_press), NULL);
   return GTK_WIDGET(pos1);
}

GtkWidget *create_pantilt_positioner( gchar *widget_name,
                              gchar *string1,
                              gchar *string2,
                              gint int1,
                              gint int2) {

   GnomeCanvasItem *background;

   /* Inicializamos el canvas */
   pos2 = GNOME_CANVAS(gnome_canvas_new_aa());
   gtk_widget_set_size_request (GTK_WIDGET(pos2), widthPos, heightPos);
   gtk_widget_show(GTK_WIDGET(pos2));

   pos2ph = gnome_canvas_points_new(2);
   pos2pv = gnome_canvas_points_new(2);

   pos2ph->coords[0] = 0;              /* x1 */
   pos2ph->coords[1] = heightPos-1;    /* y1 */
   pos2ph->coords[2] = widthPos;       /* x2 */
   pos2ph->coords[3] = heightPos-1;    /* y2 */

   pos2pv->coords[0] = widthPos / 2;   /* x1 */
   pos2pv->coords[1] = 1;              /* y1 */
   pos2pv->coords[2] = widthPos / 2;   /* x2 */
   pos2pv->coords[3] = heightPos-1;    /* y2 */

   gdk_threads_enter();
   background = gnome_canvas_item_new(gnome_canvas_root(pos2),
                           GNOME_TYPE_CANVAS_RECT,
                           "x1", 1.0,
                           "y1", 1.0,
                           "x2", widthPos,
                           "y2", heightPos,
                           "outline_color", "black",
                           "fill_color", "white",
                           NULL);
   /* Dibujamos la linea horizontal */
   pos2h = gnome_canvas_item_new(gnome_canvas_root(pos2),
                           GNOME_TYPE_CANVAS_LINE,
                           "points", pos2ph,
                           "fill_color", "red",
                           "width_pixels", 2,
                           NULL);
   /* Dibujamos la linea vertical */
   pos2v = gnome_canvas_item_new(gnome_canvas_root(pos2),
                           GNOME_TYPE_CANVAS_LINE,
                           "points", pos2pv,
                           "fill_color", "red",
                           "width_pixels", 2,
                           NULL);
   gnome_canvas_set_scroll_region(pos2, 0.0, 0.0, widthPos, heightPos);
   gnome_canvas_set_center_scroll_region(pos2, TRUE);
   gdk_threads_leave();
   g_signal_connect (G_OBJECT (pos2), "button-press-event",
                  G_CALLBACK (on_pantilt_positioner_button_press), NULL);
   return GTK_WIDGET(pos2);
}

void appendSchemaTreeView (gchar *id) {
   GtkTreeIter          toplevel;
   GtkTreeModel         *model;
   gdouble              ips;

   model = gtk_tree_view_get_model(GTK_TREE_VIEW(view));
   ips = (gdouble)schemaGetInterval(id);

   gtk_tree_store_append(GTK_TREE_STORE(model), &toplevel, NULL);
   gtk_tree_store_set(GTK_TREE_STORE(model), &toplevel,
                     COL_NAME, id,
                     COL_SHOW, FALSE,
                     COL_PLAY, FALSE,
                     COL_IPS, ips,
                     -1);
   gtk_tree_view_set_model(view, GTK_TREE_MODEL(treestore));
}

void loadSchema(  GtkWidget *widget, gpointer user_data) {
   GtkWidget *file_selector;
   const gchar *selected_filename;
   gchar *plugin, *id;

   /* Obtenemos el nombre de fichero desde el diálogo */
   file_selector = GTK_WIDGET (user_data);
   selected_filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (file_selector));
   plugin = g_strdup(selected_filename);

   id = loadingSchema(plugin);
   if (id != NULL)
      /* loadSchemaGui(id); */
      appendSchemaTreeView(id);
}

/* Funcion que carga los esquemas de los directorios por defecto */
void loadDefaultSchemas() {
   int i;
   GDir *dir_tmp;
   const gchar *file_tmp;
   gchar fullpath[MAX_MESSAGE], *id;

   /* Para cada uno de los directorios */
   for(i = 0; directories[i]; i++) {
      dir_tmp = g_dir_open(directories[i], 0, NULL);
      /* Leemos sus ficheros */
      while((file_tmp = g_dir_read_name(dir_tmp)))
         /* Si tienen extension .jde intentamos cargarlos */
         if(g_str_has_suffix(file_tmp, ".jde")) {
            sprintf(fullpath, "%s/%s", directories[i], file_tmp);
            id = loadingSchema(fullpath);
            if (id != NULL)
               appendSchemaTreeView(id);
         }
      g_dir_close(dir_tmp);
   }
}

/*************************************
 *  Callbacks para los dispositivos  *
 *************************************/

void drawLaser() {
   int i;
   Tvoxel kaka;

   if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "laser_play")))) {
      for(i=0;i<NUM_LASER;i++) {
         laser2xy(i, laser[i], &kaka);
         xy2canvas(kaka, coordenadas, i);
      }
      laser2xy(0, 0., &kaka);
      xy2canvas(kaka, coordenadas, NUM_LASER);
      gdk_threads_enter();
      gnome_canvas_item_set(recta,
                           "points", coordenadas,
                           "fill_color-rgba", 0xEEEEEEFF,      /* Gris clarito */
                           "outline_color-rgba", 0x9B52B0FF,   /* Morado oscuro */
                           NULL);
      gdk_threads_leave();
   }
   else {
      gdk_threads_enter();
      gnome_canvas_item_set(recta,
                           "fill_color-rgba", 0x00000000,
                           "outline_color-rgba", 0x00000000,
                           NULL);
      gdk_threads_leave();
   }
}

void drawSonars() {
   int i;
   Tvoxel kaka;

   if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "sonars_play")))) {
      for (i = 0; i < NUM_SONARS; i++) {
         us2xy(i,0.,0.,&kaka);
         xy2canvas(kaka, sonars_points[i], 0);
         us2xy(i,us[i],0.,&kaka);
         xy2canvas(kaka, sonars_points[i], 1);
         gdk_threads_enter();
         gnome_canvas_item_set(sonars_lines[i],
                              "points", sonars_points[i],
                              "fill_color", "blue",
                              NULL);
         gdk_threads_leave();
      }
   }
   else {
      for (i = 0; i < NUM_SONARS; i++) {
         gdk_threads_enter();
         gnome_canvas_item_set(sonars_lines[i],
                              "fill_color-rgba", 0x00000000,
                              NULL);
         gdk_threads_leave();
      }
   }
}

void drawRobot() {
   Tvoxel kaka;
   int i;

   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "robot_play")))){
      if (debug[SCH_GUIXFORMS])
         g_debug(" ego ");

      /* relleno los nuevos */
      us2xy(15,0.,0.,&kaka);
      xy2canvas(kaka,ego,0);
      us2xy(3,0.,0.,&kaka);
      xy2canvas(kaka,ego,1);
      us2xy(4,0.,0.,&kaka);
      xy2canvas(kaka,ego,2);
      us2xy(8,0.,0.,&kaka);
      xy2canvas(kaka,ego,3);
      us2xy(15,0.,0.,&kaka);
      xy2canvas(kaka,ego,4);

      for(i=0;i<NUM_SONARS;i++)
      {
         us2xy(i,0.,0.,&kaka);
         xy2canvas(kaka,ego_bord, i);
      }

      xy2canvas(kaka, center, 0);

      /* pinto los nuevos */
      gdk_threads_enter();
      gnome_canvas_set_scroll_region(canvas, center->coords[0] - 200, center->coords[1] -200, center->coords[0] + 200, center->coords[1]+200);
      gnome_canvas_set_center_scroll_region(canvas, TRUE);
      gnome_canvas_item_set(ego_bord_pol,
                     "points", ego_bord,
                     "outline_color", "red",
                     "fill_color-rgba", 0x00000077,
                     NULL);
      gnome_canvas_item_set(ego_pol,
                     "points", ego,
                     "fill_color", "black",
                     NULL);
      gdk_threads_leave();
   }
   else {
      us2xy(15,0.,0.,&kaka);
      xy2canvas(kaka, center, 0);

      /* pinto los nuevos */
      gdk_threads_enter();
      gnome_canvas_set_scroll_region(canvas, center->coords[0] - 200, center->coords[1] -200, center->coords[0] + 200, center->coords[1]+200);
      gnome_canvas_set_center_scroll_region(canvas, TRUE);
      gnome_canvas_item_set(ego_pol,
                        "fill_color-rgba", 0x00000000,
                        "outline_color-rgba", 0x00000000,
                        NULL);
      gnome_canvas_item_set(ego_bord_pol,
                     "points", ego_bord,
                     "outline_color-rgba", 0x00000000,
                     "fill_color-rgba", 0x00000000,
                     NULL);
      gdk_threads_leave();
   }
}

void readVideoA() {
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imageA_play")))) {
      GtkImage *image = GTK_IMAGE(glade_xml_get_widget(xml, "imageA"));
      GdkPixbuf *imgBuff;

      /* We read from our buffer: colorA */
      imgBuff = gdk_pixbuf_new_from_data(colorA,GDK_COLORSPACE_RGB,0,8,320,240,960,NULL,NULL);

      gdk_threads_enter();
      gtk_image_set_from_pixbuf(image, imgBuff);
      gtk_widget_queue_draw(GTK_WIDGET(image));
      gdk_threads_leave();
   }
}

void readVideoB() {
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imageB_play")))) {
      GtkImage *image = GTK_IMAGE(glade_xml_get_widget(xml, "imageB"));
      GdkPixbuf *imgBuff;

      imgBuff = gdk_pixbuf_new_from_data(colorB,GDK_COLORSPACE_RGB,0,8,320,240,960,NULL,NULL);

      gdk_threads_enter();
      gtk_image_set_from_pixbuf(image, imgBuff);
      gtk_widget_queue_draw(GTK_WIDGET(image));
      gdk_threads_leave();
   }
}

/*************************************
 *   Threads                         *
 *************************************/

/* Función que quita el splash y muestra el programa principal 
   Es lanzada en un timer desde jde.c main() */
gboolean *destroySplash(gpointer data) {
   GtkWidget *win, *splash;

   splash = glade_xml_get_widget(xml, "winSplash");
   gtk_widget_destroy(splash);
   win = glade_xml_get_widget(xml, "winMain");
   gtk_widget_show(win);

   return FALSE;
}

/* Hebra que cuando se ejecuta, actualiza (si debe) el cotenido
   de los labels de los frames per second */
gboolean updateFPS() {
   char fpstext[80]="";

   /* ImageA */
   if (state[SCH_IMAGEA]==active) sprintf(fpstext,"%.1f fps",fpsA);
   else sprintf(fpstext," ");
   gtk_label_set_text(GTK_LABEL(glade_xml_get_widget(xml, "imageA_fps")), fpstext);

   /* ImageB */
   if (state[SCH_IMAGEB]==active) sprintf(fpstext,"%.1f fps",fpsB);
   else sprintf(fpstext," ");
   gtk_label_set_text(GTK_LABEL(glade_xml_get_widget(xml, "imageB_fps")), fpstext);

   /* Robot */
   if (state[SCH_ENCODERS]==active) sprintf(fpstext,"%.1f fps",fpsencoders);
   else sprintf(fpstext," ");
   gtk_label_set_text(GTK_LABEL(glade_xml_get_widget(xml, "robot_fps")), fpstext);

   /* Sonars */
   if (state[SCH_SONARS]==active) sprintf(fpstext,"%.1f fps",fpssonars);
   else sprintf(fpstext," ");
   gtk_label_set_text(GTK_LABEL(glade_xml_get_widget(xml, "sonars_fps")), fpstext);

   /* Laser */
   if (state[SCH_LASER]==active) sprintf(fpstext,"%.1f fps",fpslaser);
   else sprintf(fpstext," ");
   gtk_label_set_text(GTK_LABEL(glade_xml_get_widget(xml, "laser_fps")), fpstext);

   /* Pantilt */
   if (state[SCH_PANTILTENCODERS]==active) sprintf(fpstext,"%.1f fps",fpspantiltencoders);
   else sprintf(fpstext," ");
   gtk_label_set_text(GTK_LABEL(glade_xml_get_widget(xml, "pantilt_fps")), fpstext);

   return TRUE;
}


static void
cell_data_func_gpa (GtkTreeViewColumn *col,
                    GtkCellRenderer   *cell,
                    GtkTreeModel      *model,
                    GtkTreeIter       *iter,
                    gpointer           data)
{
   gchar   buf[32];
   GValue  val = {0, };
   gtk_tree_model_get_value(model, iter, COL_IPS, &val);
   g_snprintf(buf, sizeof(buf), "%.1f", g_value_get_float(&val));
   g_object_set(cell, "text", buf, NULL);
}

void cell_state_func (GtkTreeViewColumn   *col,
                      GtkCellRenderer     *cell,
                      GtkTreeModel        *model,
                      GtkTreeIter         *iter,
                      gpointer            data)
{
   gchar    buf[32];
   GValue   val = {0, };
   guint    state;

   gtk_tree_model_get_value(model, iter, COL_NAME, &val);
   g_snprintf(buf, sizeof(buf), "%s", g_value_get_string(&val));
   state = schemaGetState(buf);

   switch (state) {
      case slept:
         g_object_set(cell,
            "background", "Black",
            "foreground", "White",
            "text", "slept",
            NULL);
         break;
      case ready:
         g_object_set(cell,
            "background", "Orange",
            "foreground", "Black",
            "text", "ready",
            NULL);
         break;
      case winner:
         g_object_set(cell,
            "background", "Green",
            "foreground", "Black",
            "text", "winner",
            NULL);
         break;
      case checking:
         g_object_set(cell,
            "background", "Red",
            "foreground", "Black",
            "text", "checking",
            NULL);
         break;
   }
}

static void startTreeView() {
   GtkWidget            *container;
   GtkCellRenderer      *renderer;
   GtkTreeViewColumn    *col;

   container = glade_xml_get_widget(xml, "treeViewWindow");

   view = (GtkTreeView *)gtk_tree_view_new();

   treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, G_TYPE_FLOAT, G_TYPE_INT);

   gtk_tree_view_set_model(GTK_TREE_VIEW(view), GTK_TREE_MODEL(treestore));

   g_object_unref(treestore); /* destroy model automatically with view */
   gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)), GTK_SELECTION_NONE);


   /* --- Column #1 --- */
   col = gtk_tree_view_column_new();
   renderer = gtk_cell_renderer_text_new();
   gtk_tree_view_column_set_title(col, "Schema");
   gtk_tree_view_column_set_resizable(col, TRUE);
   gtk_tree_view_column_pack_start(col, renderer, TRUE);
   gtk_tree_view_column_add_attribute(col, renderer, "text", COL_NAME);
   gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
   g_object_set(renderer, "weight", PANGO_WEIGHT_BOLD, "weight-set", TRUE, NULL);

   /* --- Column #2 --- */
   col = gtk_tree_view_column_new();
   renderer = gtk_cell_renderer_text_new();
   gtk_tree_view_column_set_title(col, "State");
   gtk_tree_view_column_pack_start(col, renderer, TRUE);
   gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
   gtk_tree_view_column_set_cell_data_func(col, renderer, cell_state_func, NULL, NULL);
   g_object_set(renderer, "weight", PANGO_WEIGHT_BOLD, "weight-set", TRUE, NULL);
   g_object_set(renderer, "foreground-set", TRUE, NULL);
   g_object_set(renderer, "background-set", TRUE, NULL);

   /* --- Column #3 --- */
   col = gtk_tree_view_column_new();
   renderer = gtk_cell_renderer_toggle_new();
   gtk_tree_view_column_set_title(col, "Show");
   gtk_tree_view_column_set_resizable(col, TRUE);
   gtk_tree_view_column_pack_start(col, renderer, TRUE);
   gtk_tree_view_column_add_attribute(col, renderer, "active", COL_SHOW);
   gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
   g_signal_connect(renderer, "toggled", G_CALLBACK(schemaStartGui), treestore);

   /* --- Column #4 --- */
   col = gtk_tree_view_column_new();
   renderer = gtk_cell_renderer_toggle_new();
   gtk_cell_renderer_set_fixed_size(renderer, 100, -1);
   gtk_tree_view_column_set_title(col, "Play");
   gtk_tree_view_column_set_resizable(col, TRUE);
   gtk_tree_view_column_pack_start(col, renderer, TRUE);
   gtk_tree_view_column_add_attribute(col, renderer, "active", COL_PLAY);
   gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
   g_signal_connect(renderer, "toggled", G_CALLBACK(schemaStart), treestore);

   /* --- Column #5 --- */
   col = gtk_tree_view_column_new();
   renderer = gui_cell_renderer_spin_new(30.0, 60000.0, 10.0, 50.0, 50.0, 1.0, 0);
   gtk_tree_view_column_set_title(col, "Cycle time (milliseconds)");
   gtk_tree_view_column_set_resizable(col, TRUE);
   gtk_tree_view_column_set_max_width(col, 5);
   gtk_tree_view_column_pack_start(col, renderer, TRUE);
   gtk_tree_view_column_set_cell_data_func(col, renderer, cell_data_func_gpa, NULL, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
   g_object_set(renderer, "editable", TRUE, NULL);
   g_signal_connect(renderer, "edited", G_CALLBACK(schemaSetInterval), treestore);

   gtk_container_add(GTK_CONTAINER(container), GTK_WIDGET(view));

   gtk_widget_show_all(container);
}

/* Hebra que inicia ciertos parámetros y arranca el bucle principal
   de la gui */
void *guiStartup () {
   /* Coord del sistema odometrico respecto del visual */
   odometrico[0]=0.;
   odometrico[1]=0.;
   odometrico[2]=0.;
   odometrico[3]= cos(0.);
   odometrico[4]= sin(0.);

   startTreeView ();
   loadDefaultSchemas();
   start3d ();

   /* Entramos al bucle principal de GTK */
   gdk_threads_enter();
   gtk_main();
   gdk_threads_leave();

   return NULL;
}
