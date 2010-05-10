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

guint schema_thread_id;
function_arb myarbitration;

G_MODULE_EXPORT guint schema_interval = 75;
G_MODULE_EXPORT gchar* schema_id = "goon";
G_MODULE_EXPORT gchar* schema_path;

guint goon_x;
guint goon_y;
guint target_x = 0;
guint target_y = 0;
gboolean stop = FALSE;

void xy2robot(float x, float y, Tvoxel *point) {
   /* Inversa de robot2xy sacada con Maple */
   (*point).x =  (y*robot[4] + x*robot[3] - robot[3]*robot[0] - robot[1]*robot[4])/(robot[4]*robot[4] + robot[3]*robot[3]);
   (*point).y = -(x*robot[4] - y*robot[3] + robot[3]*robot[1] - robot[0]*robot[4])/(robot[4]*robot[4] + robot[3]*robot[3]);
}

gboolean precondiciones() {
   gint i;
   Tvoxel kaka;

   if (target_x != 0 && target_y != 0 && !stop) {
      for (i = 0; i < NUM_LASER; i++) {
         laser2robot(i, laser[i], &kaka);
         if (abs(kaka.x) <= goon_y && abs(kaka.y) <= goon_x)
            return FALSE;
      }

      return TRUE;
   }
   else
      return FALSE;
}

void schema_iteration() {
   gdouble resul_mod;
   gdouble resul_deg;
   Tvoxel kaka;

   xy2robot(target_x, -target_y, &kaka);

   resul_mod = sqrt(pow(kaka.x, 2) + pow(kaka.y, 2));
   resul_deg = atan2(kaka.y, kaka.x) * RADTODEG;

   g_print("%.3f %.3f\n", resul_mod, resul_deg);

   if (abs(resul_deg) < 10) {
      v = 400;
      w = 0;
   }
   else if (abs(resul_deg) > 10 && abs(resul_deg) < 40) {
      v = 200;
      w = (resul_deg > 0) ? 50 : -50;
   }
   else {
      v = 0;
      w = (resul_deg > 0) ? 50 : -50;
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
         schemaSetState(schema_id, checking);
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

G_MODULE_EXPORT void schema_startup (void) {
   GError *error;
   static gboolean loaded = FALSE;

   if (!loaded) {
      putSchemaVar((gpointer)&goon_x, "goon_x", schema_id);
      putSchemaVar((gpointer)&goon_y, "goon_y", schema_id);
      putSchemaVar((gpointer)&target_x, "target_x", schema_id);
      putSchemaVar((gpointer)&target_y, "target_y", schema_id);
      putSchemaVar((gpointer)&stop, "stop", schema_id);

      loaded = TRUE;
   }
   else {
      /* Buen momento para importar símbolos */

      /* Avisar al padre ¿¿?? */
      myarbitration = schemaGetArbitration(schema_id);

      /* Si no es la primera vez, se inicia la hebra */
      schema_thread_id = gtk_timeout_add(schema_interval, (GtkFunction)schema_thread, &error);
      if (!schema_thread_id) {
         g_printerr ("Failed on schema_startup: %s\n", error->message);
      }
   }
}

G_MODULE_EXPORT void schema_suspend (void) {
   g_print ("\n--- Suspendiendo esquema %s ---\n", schema_id);
   g_source_remove(schema_thread_id);
   schemaSetState(schema_id, slept);
   //v = 0;
}
