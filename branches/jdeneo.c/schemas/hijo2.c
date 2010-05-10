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

G_MODULE_EXPORT guint schema_interval = 1000;
G_MODULE_EXPORT gchar* schema_id = "hijo2";
G_MODULE_EXPORT gchar* schema_path;

gboolean precondiciones() {
   return (laser[90] >= 1000 && laser[90] <= 5000);
}

void schema_iteration() {
   v = 100.0;
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
      /* Buen momento para exportar símbolos */
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
}
