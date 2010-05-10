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
G_MODULE_EXPORT gchar* schema_id = "padre";
G_MODULE_EXPORT gchar* schema_path;

gboolean precondiciones() {
   return TRUE;
}

gboolean schema_arbitration(gchar *id) {
   /* Si hay un vacio, ninguno actua y paramos motores */
   if (!schemaIsAnyReady("hijo1")) {
      v = 0.0;
      return FALSE;
   }
   else {
      /* Si hay solape, se ejecuta hijo1 */
      if (strcmp(id, "hijo1")) {
         return TRUE;
      }
      else
         return FALSE;
   }
}

void schema_iteration() {
   static int i = 0;
   g_print("Iteracion %d de %s. En interval tenemos: %d. Laser[90] = %d v = %.3f\n", ++i, schema_id, schema_interval, laser[90], v);
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

G_MODULE_EXPORT void schema_startup (void) {
   GError *error;
   static gboolean loaded = FALSE;
   function hijo1_startup, hijo2_startup;

   if (!loaded) {
      /* Buen momento para exportar símbolos */
      loaded = TRUE;
   }
   else {
      /* Buen momento para importar símbolos */

      /* Coger arbitraje del padre */
      myarbitration = schemaGetArbitration(schema_id);

      /* Avisamos a nuestros hijos */
      schemaSetFather("hijo1", schema_id);
      schemaSetFather("hijo2", schema_id);

      /* Añadir arbitraje a hijos */
      schemaSetArbitration("hijo1", schema_arbitration);
      schemaSetArbitration("hijo2", schema_arbitration);

      /* Añadir grupo a hijos */
      schemaSetGroup("hijo1", 0);
      schemaSetGroup("hijo2", 0);

      /* Despertamos a hijos */
      hijo1_startup = schemaGetStartup("hijo1");
      hijo1_startup();
      schemaSetState("hijo1", checking);
      hijo2_startup = schemaGetStartup("hijo2");
      hijo2_startup();
      schemaSetState("hijo2", checking);

      /* Iniciamos el timeout */
      schema_thread_id = gtk_timeout_add(schema_interval, (GtkFunction)schema_thread, &error);
      if (!schema_thread_id) {
         g_printerr ("Failed on schema_startup: %s\n", error->message);
      }
   }
}

G_MODULE_EXPORT void schema_suspend (void) {
   function hijo1_suspend, hijo2_suspend;

   g_print ("\n--- Suspendiendo esquema %s ---\n", schema_id);
   g_source_remove(schema_thread_id);
   schemaSetState(schema_id, slept);
   v = 0.0;

   /* Dormimos a los hijos */
   hijo1_suspend = schemaGetSuspend("hijo1"); hijo1_suspend();
   hijo2_suspend = schemaGetSuspend("hijo2"); hijo2_suspend();
}
