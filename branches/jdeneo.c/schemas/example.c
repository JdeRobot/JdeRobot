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

#include "../jde.h"
#include <libgnomecanvas/libgnomecanvas.h>

guint schema_thread_id;
function_arb myarbitration;
const gchar* pathgui = "master/master.glade";
GladeXML *schema_gui;
gboolean stop = FALSE;

G_MODULE_EXPORT guint schema_interval = 75;
G_MODULE_EXPORT gchar* schema_id = "example";
G_MODULE_EXPORT gchar* schema_path;

GnomeCanvas *example_canvas;
GnomeCanvasPoints *example_center;
float example_odometrico[5];
float example_escala=0.1;
float example_width = 270;
float example_height = 270;

GnomeCanvasItem *example_recta = NULL;
GnomeCanvasPoints *example_coordenadas;

#define EGOMAX 5
GnomeCanvasItem *example_ego_pol = NULL;
GnomeCanvasItem *example_ego_bord_pol = NULL;
GnomeCanvasPoints *example_ego;
GnomeCanvasPoints *example_ego_bord;
int example_numego=0;

/* Fuerzas */
GnomeCanvasPoints *example_repulsion;
GnomeCanvasItem *example_repulsion_line;
gboolean example_target_selected = FALSE;
GnomeCanvasPoints *example_target;
GnomeCanvasItem *example_target_line;
GnomeCanvasPoints *example_target1;
GnomeCanvasPoints *example_target2;
GnomeCanvasItem *example_target_line1;
GnomeCanvasItem *example_target_line2;
GnomeCanvasPoints *example_resul;
GnomeCanvasItem *example_resul_line;

/* Areas */
GnomeCanvasItem *example_vff_area;
GnomeCanvasPoints *example_vff;
GnomeCanvasItem *example_stop_area;
GnomeCanvasPoints *example_stop;
typedef enum { EXAMPLE_STOP, EXAMPLE_VFF, EXAMPLE_GOON } Areas;
int example_active_area = EXAMPLE_GOON;
int stop_x = 300;
int stop_y = 600;
int vff_x = 1000;
int vff_y = 3000;

gint target_x, target_y;
gdouble vffx, vffy;

#define MAX_TARGET   1000.0
#define ALPHA        0.7

/* Variables externas de modulación */
guint *goon_x;
guint *goon_y;
guint *goon_target_x;
guint *goon_target_y;
gboolean *goon_stop;
guint *vff_ext_x;
guint *vff_ext_y;
gdouble *vff_target_x;
gdouble *vff_target_y;
gboolean *vff_stop;
guint *stop_ext_x;
guint *stop_ext_y;

void xy2robot(float x, float y, Tvoxel *point) {
   /* Inversa de robot2xy sacada con Maple */
   (*point).x =  (y*robot[4] + x*robot[3] - robot[3]*robot[0] - robot[1]*robot[4])/(robot[4]*robot[4] + robot[3]*robot[3]);
   (*point).y = -(x*robot[4] - y*robot[3] + robot[3]*robot[1] - robot[0]*robot[4])/(robot[4]*robot[4] + robot[3]*robot[3]);
}

int response_level(float x) {
   if (x < 250) return 2000;
   else if (x < 500) return 1500;
   else if (x < 750) return 1200;
   else if (x < 1000) return 1000;
   else if (x < 2000) return 500;
   else if (x < 3000) return 200;
   else return 0;
}

GtkWidget *example_create_canvas() {
   gtk_widget_pop_colormap();
   gtk_widget_pop_visual();

   example_canvas = GNOME_CANVAS(gnome_canvas_new_aa());
   gtk_widget_set_size_request (GTK_WIDGET(example_canvas), example_width, example_height);
   gtk_widget_show(GTK_WIDGET(example_canvas));
   gnome_canvas_set_scroll_region(GNOME_CANVAS(example_canvas), -30000, -30000, 30000, 30000);
   gnome_canvas_set_center_scroll_region(GNOME_CANVAS(example_canvas), TRUE);
   example_center = gnome_canvas_points_new(2);
   example_repulsion = gnome_canvas_points_new(2);
   example_target = gnome_canvas_points_new(2);
   example_resul = gnome_canvas_points_new(2);
   example_target1 = gnome_canvas_points_new(2);
   example_target2 = gnome_canvas_points_new(2);
   example_coordenadas = gnome_canvas_points_new(NUM_LASER+1);
   example_ego = gnome_canvas_points_new(EGOMAX);
   example_ego_bord = gnome_canvas_points_new(NUM_SONARS);
   example_stop = gnome_canvas_points_new(4);
   example_vff = gnome_canvas_points_new(4);

   gdk_threads_enter();
   gnome_canvas_item_new(gnome_canvas_root(example_canvas),
                        GNOME_TYPE_CANVAS_RECT,
                        "x1", -30000.0,
                        "y1", -30000.0,
                        "x2", 30000.0,
                        "y2", 30000.0,
                        "fill_color", "white",
                        NULL);
   example_recta = gnome_canvas_item_new(gnome_canvas_root(example_canvas),
                           GNOME_TYPE_CANVAS_POLYGON,
                           NULL);
   example_ego_pol = gnome_canvas_item_new(gnome_canvas_root(example_canvas),
                           GNOME_TYPE_CANVAS_POLYGON,
                           NULL);
   example_ego_bord_pol = gnome_canvas_item_new(gnome_canvas_root(example_canvas),
                           GNOME_TYPE_CANVAS_POLYGON,
                           NULL);
   example_repulsion_line = gnome_canvas_item_new(gnome_canvas_root(example_canvas),
                           GNOME_TYPE_CANVAS_LINE,
                           NULL);
   example_target_line = gnome_canvas_item_new(gnome_canvas_root(example_canvas),
                           GNOME_TYPE_CANVAS_LINE,
                           NULL);
   example_resul_line = gnome_canvas_item_new(gnome_canvas_root(example_canvas),
                           GNOME_TYPE_CANVAS_LINE,
                           NULL);
   example_target_line1 = gnome_canvas_item_new(gnome_canvas_root(example_canvas),
                           GNOME_TYPE_CANVAS_LINE,
                           NULL);
   example_target_line2 = gnome_canvas_item_new(gnome_canvas_root(example_canvas),
                           GNOME_TYPE_CANVAS_LINE,
                           NULL);
   example_stop_area = gnome_canvas_item_new(gnome_canvas_root(example_canvas),
                           GNOME_TYPE_CANVAS_POLYGON,
                           NULL);
   example_vff_area = gnome_canvas_item_new(gnome_canvas_root(example_canvas),
                           GNOME_TYPE_CANVAS_POLYGON,
                           NULL);

   gdk_threads_leave();

   return GTK_WIDGET(example_canvas);
}

void example_draw_laser() {
   int i;
   Tvoxel kaka;

   for(i=0;i<NUM_LASER;i++) {
      laser2xy(i, laser[i], &kaka);
      example_coordenadas->coords[i*2]       = kaka.x;
      example_coordenadas->coords[(i*2) + 1] = -kaka.y;
   }
   laser2xy(0, 0., &kaka);
   example_coordenadas->coords[NUM_LASER*2]       = kaka.x;
   example_coordenadas->coords[(NUM_LASER*2) + 1] = -kaka.y;

   gdk_threads_enter();
   gnome_canvas_item_set(example_recta,
                        "points", example_coordenadas,
                        "fill_color-rgba", 0xEEEEEEFF,
                        "outline_color-rgba", 0x9B52B0FF,
                        NULL);
   gdk_threads_leave();
}

void example_draw_robot() {
   Tvoxel kaka;
   int i;

   us2xy(15,0.,0.,&kaka);
   example_ego->coords[0*2]       = kaka.x;
   example_ego->coords[(0*2) + 1] = -kaka.y;

   us2xy(3,0.,0.,&kaka);
   example_ego->coords[1*2]       = kaka.x;
   example_ego->coords[(1*2) + 1] = -kaka.y;

   us2xy(4,0.,0.,&kaka);
   example_ego->coords[2*2]       = kaka.x;
   example_ego->coords[(2*2) + 1] = -kaka.y;

   us2xy(8,0.,0.,&kaka);
   example_ego->coords[3*2]       = kaka.x;
   example_ego->coords[(3*2) + 1] = -kaka.y;

   us2xy(15,0.,0.,&kaka);
   example_ego->coords[4*2]       = kaka.x;
   example_ego->coords[(4*2) + 1] = -kaka.y;

   for(i=0;i<NUM_SONARS;i++) {
      us2xy(i, 0., 0., &kaka);
      example_ego_bord->coords[i*2]        = kaka.x;
      example_ego_bord->coords[(i*2) + 1]  = -kaka.y;
   }

   us2xy(15, 0., 0., &kaka);
   example_center->coords[0] = kaka.x - 20;
   example_center->coords[1] = -kaka.y - 20;
   example_center->coords[2] = kaka.x + 20;
   example_center->coords[3] = -kaka.y + 20;

   gdk_threads_enter();

   gnome_canvas_set_scroll_region(example_canvas, example_center->coords[0], example_center->coords[1], example_center->coords[2], example_center->coords[3]);
   gnome_canvas_set_center_scroll_region(example_canvas, TRUE);

   gnome_canvas_item_set(example_ego_bord_pol,
                     "points", example_ego_bord,
                     "fill_color-rgba", 0x333333AA,
                     NULL);
   gnome_canvas_item_set(example_ego_pol,
                     "points", example_ego,
                     "fill_color", "black",
                     NULL);
   gdk_threads_leave();
}

void example_draw_repulsion() {
   long lx = 0.3;
   long ly = 0;
   int i;
   Tvoxel kaka;

   /* Linea con las fuerzas de repulsión */
   laser2xy(1, 0., &kaka); /* Origen */
   example_repulsion->coords[0] = kaka.x;
   example_repulsion->coords[1] = -kaka.y;

   for( i = 0 ; i < NUM_LASER; i++ ) {
      laser2robot(i, response_level(laser[i]),&kaka);
      lx += kaka.x;
      ly += kaka.y;
   }

   example_repulsion->coords[2] = -lx / 20;
   example_repulsion->coords[3] = -ly / 20;

   robot2xy(example_repulsion->coords[2], example_repulsion->coords[3], &kaka);
   example_repulsion->coords[2] = kaka.x;
   example_repulsion->coords[3] = -kaka.y;

   gdk_threads_enter();
   gnome_canvas_item_set(example_repulsion_line,
                        "points", example_repulsion,
                        "fill_color", "blue",
                        "width-units", 30.0,
                        NULL);
   gdk_threads_leave();
}

void example_draw_target() {
   gdouble modulo;

   /* Acotamos */
   modulo = sqrt(pow(target_x - robot[0], 2) + pow(target_y + robot[1], 2));
   if (modulo > MAX_TARGET) {
      example_target->coords[0] = ((target_x - robot[0]) * (MAX_TARGET / modulo)) + robot[0];
      example_target->coords[1] = ((target_y + robot[1]) * (MAX_TARGET / modulo)) - robot[1];
   }
   else {
      example_target->coords[0] = target_x;
      example_target->coords[1] = target_y;
   }

   example_target->coords[2] = robot[0];
   example_target->coords[3] = -robot[1];

   gdk_threads_enter();
   gnome_canvas_item_set(example_target_line,
                        "points", example_target,
                        "fill_color", "green",
                        "width-units", 30.0,
                        NULL);
   gdk_threads_leave();
}

void example_draw_target_area() {
   example_target1->coords[0] = target_x - 100;
   example_target1->coords[1] = target_y - 100;
   example_target1->coords[2] = target_x + 100;
   example_target1->coords[3] = target_y + 100;
   example_target2->coords[0] = target_x - 100;
   example_target2->coords[1] = target_y + 100;
   example_target2->coords[2] = target_x + 100;
   example_target2->coords[3] = target_y - 100;


   gdk_threads_enter();
   gnome_canvas_item_set(example_target_line1,
                        "points", example_target1,
                        "fill_color", "red",
                        "width-units", 30.0,
                        "smooth", TRUE,
                        NULL);
   gnome_canvas_item_set(example_target_line2,
                        "points", example_target2,
                        "fill_color", "red",
                        "width-units", 30.0,
                        "smooth", TRUE,
                        NULL);
   gdk_threads_leave();
}

void example_draw_resul() {
   Tvoxel kaka;
   gdouble tx, ty, rx, ry;

   /* Sacamos las coordenadas respecto al robot y las sumamos */
   xy2robot(example_target->coords[0], -example_target->coords[1], &kaka);
   tx = kaka.x;
   ty = kaka.y;
   xy2robot(example_repulsion->coords[2], -example_repulsion->coords[3], &kaka);
   rx = kaka.x;
   ry = kaka.y;

   vffx = (ALPHA * tx) + ((1 - ALPHA) * rx);
   vffy = (ALPHA * ty) + ((1 - ALPHA) * ry);

   *vff_target_x = vffx;
   *vff_target_y = vffy;

   robot2xy(vffx, vffy, &kaka);
   example_resul->coords[0] = kaka.x;
   example_resul->coords[1] = -kaka.y;

   laser2xy(1, 0., &kaka); /* Origen */
   example_resul->coords[2] = kaka.x;
   example_resul->coords[3] = -kaka.y;

   gdk_threads_enter();
   gnome_canvas_item_set(example_resul_line,
                        "points", example_resul,
                        "fill_color", "red",
                        "width-units", 30.0,
                        NULL);
   gdk_threads_leave();
}

void example_draw_vff_area() {
   Tvoxel kaka;

   robot2xy(0, -vff_x, &kaka);
   example_vff->coords[0] = kaka.x;
   example_vff->coords[1] = -kaka.y;
   robot2xy(0, vff_x, &kaka);
   example_vff->coords[2] = kaka.x;
   example_vff->coords[3] = -kaka.y;
   robot2xy(vff_y, vff_x, &kaka);
   example_vff->coords[4] = kaka.x;
   example_vff->coords[5] = -kaka.y;
   robot2xy(vff_y, -vff_x, &kaka);
   example_vff->coords[6] = kaka.x;
   example_vff->coords[7] = -kaka.y;

   gdk_threads_enter();
   gnome_canvas_item_set(example_vff_area,
                        "points", example_vff,
                        "join-style", GDK_JOIN_ROUND,
                        "outline_color", "green",
                        "width-units", 5.0,
                        NULL);
   if (example_active_area == EXAMPLE_VFF)
         gnome_canvas_item_set(example_vff_area,
                        "fill-color-rgba", 0x00FF0044,
                        NULL);
   else
         gnome_canvas_item_set(example_vff_area,
                        "fill-color-rgba", 0x00000000,
                        NULL);
   gdk_threads_leave();
}

void example_draw_stop_area() {
   Tvoxel kaka;

   robot2xy(0, -stop_x, &kaka);
   example_stop->coords[0] = kaka.x;
   example_stop->coords[1] = -kaka.y;
   robot2xy(0, stop_x, &kaka);
   example_stop->coords[2] = kaka.x;
   example_stop->coords[3] = -kaka.y;
   robot2xy(stop_y, stop_x, &kaka);
   example_stop->coords[4] = kaka.x;
   example_stop->coords[5] = -kaka.y;
   robot2xy(stop_y, -stop_x, &kaka);
   example_stop->coords[6] = kaka.x;
   example_stop->coords[7] = -kaka.y;

   gdk_threads_enter();
   gnome_canvas_item_set(example_stop_area,
                        "points", example_stop,
                        "join-style", GDK_JOIN_ROUND,
                        "outline_color", "red",
                        "width-units", 5.0,
                        NULL);
   if (example_active_area == EXAMPLE_STOP)
         gnome_canvas_item_set(example_stop_area,
                        "fill-color-rgba", 0xFF000044,
                        NULL);
   else
         gnome_canvas_item_set(example_stop_area,
                        "fill-color-rgba", 0x00000000,
                        NULL);
   gdk_threads_leave();
}

void on_zoom_changed ( GtkRange *range, gpointer user_data) {
   example_escala = gtk_range_get_value(range);
   gnome_canvas_set_pixels_per_unit(example_canvas, gtk_range_get_value(range));
}

gboolean set_target(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
   double wx, wy;

   gnome_canvas_window_to_world (example_canvas, event->x, event->y, &wx, &wy);

   example_target_selected = TRUE;
   stop = FALSE;
   target_x = wx;
   target_y = wy;
   *goon_target_x = target_x;
   *goon_target_y = target_y;
   *goon_stop = FALSE;
   *vff_stop = FALSE;

   return TRUE;
}

int example_get_active_area() {
   gint i;
   gboolean vff = FALSE;
   Tvoxel kaka;

   for (i = 0; i < NUM_LASER; i++) {
      laser2robot(i, laser[i], &kaka);
      if (abs(kaka.x) <= vff_y && abs(kaka.y) <= vff_x) {
         vff = TRUE;
         if (abs(kaka.x) <= stop_y && abs(kaka.y) <= stop_x)
            return EXAMPLE_STOP;
      }
   }

   if (vff)
      return EXAMPLE_VFF;
   else
      return EXAMPLE_GOON;
}

gboolean precondiciones() {
   return TRUE;
}

gboolean schema_arbitration(gchar *id) {
   /* Si hay vacío de control */
   if (!schemaIsAnyReady("vff")) {
      v = 0.0;
      return FALSE;
   }
   else {
      /* Si hay solape, se ejecuta stop */
      if (!strcmp(id, "stop")) {
         return TRUE;
      }
      else
         return FALSE;
   }
}

void schema_iteration() {
   gdouble modulo;

   /* Pintamos laser y robot */
   example_draw_repulsion();
   example_draw_laser();
   example_draw_robot();
   example_active_area = example_get_active_area();
   example_draw_vff_area();
   example_draw_stop_area();

   if (example_target_selected) {
      example_draw_target_area();
      example_draw_target();
      example_draw_resul();

      modulo = sqrt(pow(target_x - robot[0],2) + pow(target_y + robot[1],2));
      if (modulo < 300) {
         stop = TRUE;
         *goon_stop = TRUE;
         *vff_stop = TRUE;
         v = w = 0;
      }
   }
}

gboolean schema_thread(gpointer data) {
   if (schemaGetState(schema_id) != slept) {
      if (precondiciones()) {
         schemaSetState(schema_id, ready);
         if (!schemaIsAnyWinner(schema_id)) {
            schemaSetState(schema_id, winner);
         }
         else {
            if (myarbitration(schema_id)) {
               schemaSetState(schema_id, winner);
            }
         }
      }
      else {
         if (!schemaIsAnyReady(schema_id)) {
            if (myarbitration(schema_id)) {
               schemaSetState(schema_id, winner);
            }
         }
      }

      if (schemaGetState(schema_id) == winner)
         schema_iteration();
   }
   return TRUE;
}

G_MODULE_EXPORT gboolean schema_gui_startup () {
   static int j = 0;
   GtkWidget *win;
   GtkWidget *hbox2;
   GtkWidget *scale;

   g_print ("--- Inicializando gui del esquema %s---\n\n", schema_id);
   if (j == 0) {
      j++;
      schema_gui = glade_xml_new (g_strconcat(schema_path, "/", pathgui, NULL), NULL, NULL);
      if (!schema_gui) {
         g_error("Error. No se ha inicializado correctamente la interfaz del esquema %s\n", schema_id);
         return FALSE;
      }
      /* Coord del sistema odometrico respecto del visual */
      example_odometrico[0]=0.;
      example_odometrico[1]=0.;
      example_odometrico[2]=0.;
      example_odometrico[3]= cos(0.);
      example_odometrico[4]= sin(0.);

      hbox2 = glade_xml_get_widget(schema_gui, "hbox2");
      example_canvas = GNOME_CANVAS(example_create_canvas());
      gtk_box_pack_start(GTK_BOX(hbox2), GTK_WIDGET(example_canvas), TRUE, TRUE, 0);

      g_signal_connect (G_OBJECT (example_canvas), "button-release-event",
                     G_CALLBACK (set_target), NULL);

      scale = glade_xml_get_widget(schema_gui, "example_canvas_scale");
      g_signal_connect (G_OBJECT (scale), "value-changed",
                     G_CALLBACK (on_zoom_changed), NULL);
   }
   else {
      win = glade_xml_get_widget(schema_gui, "window1");
      gtk_widget_show(win);
   }

   return TRUE;
}

G_MODULE_EXPORT gboolean schema_gui_suspend () {
   GtkWidget *win;
   g_print ("--- Suspendiendo gui del esquema %s ---\n\n", schema_id);
   win = glade_xml_get_widget(schema_gui, "window1");
   gtk_widget_hide(win);
   return TRUE;
}

G_MODULE_EXPORT void schema_startup (void) {
   GError *error;
   static gboolean loaded = FALSE;
   function hijo1_startup, hijo2_startup;

   if (!loaded) {
      loaded = TRUE;
   }
   else {
      /* Coger arbitraje del padre */
      myarbitration = schemaGetArbitration(schema_id);

      /* Avisamos a nuestros hijos */
      schemaSetFather("goon", schema_id);
      schemaSetFather("stop", schema_id);
      schemaSetFather("vff", schema_id);

      /* Añadir arbitraje a hijos */
      schemaSetArbitration("goon", schema_arbitration);
      schemaSetArbitration("stop", schema_arbitration);
      schemaSetArbitration("vff", schema_arbitration);

      /* Añadir grupo a hijos */
      schemaSetGroup("goon", 0);
      schemaSetGroup("stop", 0);
      schemaSetGroup("vff", 0);

      /* Importamos símbolos */
      goon_x = getSchemaVar("goon", "goon_x");
      goon_y = getSchemaVar("goon", "goon_y");
      goon_target_x = getSchemaVar("goon", "target_x");
      goon_target_y = getSchemaVar("goon", "target_y");
      goon_stop = getSchemaVar("goon", "stop");
      vff_ext_x = getSchemaVar("vff", "vff_x");
      vff_ext_y = getSchemaVar("vff", "vff_y");
      vff_target_x = getSchemaVar("vff", "target_x");
      vff_target_y = getSchemaVar("vff", "target_y");
      vff_stop = getSchemaVar("vff", "stop");
      stop_ext_x = getSchemaVar("stop", "stop_x");
      stop_ext_y = getSchemaVar("stop", "stop_y");

      /* Modulamos */
      *goon_x = vff_x;
      *goon_y = vff_y;
      *vff_ext_x = vff_x;
      *vff_ext_y = vff_y;
      *stop_ext_x = stop_x;
      *stop_ext_y = stop_y;

      /* Despertamos a hijos */
      hijo1_startup = schemaGetStartup("goon");
      hijo1_startup();
      schemaSetState("goon", checking);
      hijo2_startup = schemaGetStartup("stop");
      hijo2_startup();
      schemaSetState("stop", checking);
      hijo2_startup = schemaGetStartup("vff");
      hijo2_startup();
      schemaSetState("vff", checking);

      /* Iniciamos el timeout */
      schema_thread_id = gtk_timeout_add(schema_interval, (GtkFunction)schema_thread, &error);
      if (!schema_thread_id) {
         g_printerr ("Failed on schema_startup: %s\n", error->message);
      }
   }
}

G_MODULE_EXPORT void schema_suspend (void) {
   function goon_suspend, stop_suspend, vff_suspend;

   g_print ("\n--- Suspendiendo esquema %s ---\n", schema_id);
   g_source_remove(schema_thread_id);
   schemaSetState(schema_id, slept);
   v = 0.0;
   stop = FALSE;

   /* Dormimos a los hijos */
   goon_suspend = schemaGetSuspend("goon"); goon_suspend();
   stop_suspend = schemaGetSuspend("stop"); stop_suspend();
   vff_suspend  = schemaGetSuspend("vff");  vff_suspend();
}
