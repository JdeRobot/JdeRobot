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

gboolean schema_iteration(gpointer data);
guint schema_thread;
static gboolean started = FALSE;
int shared = 37;

/*******************************************************
 *                                                     *
 *                API Obligatoria                      *
 *                                                     *
 *******************************************************/

/*******************************************************
 *                  Variables                          *
 *******************************************************/

/*
*       Se declara un puntero a la ruta del esquema
*       y se exporta como símbolo, para poder ser
*       utilizado por las aplicaciones que carguen
*       este plugin
*
*/
G_MODULE_EXPORT gchar* schema_path;

/*
*       Cada esquema tiene un nombre que es su
*       identificador _único_. En caso de colisión
*       de nombres, jde.c tomará sólo el más
*       prioritario (El primero en cargarse).
*/
G_MODULE_EXPORT gchar* schema_id = "schema_export2";

/*
*       Definimos una variable cuyo contenido es el tiempo en milisegundos
*       que hay entre cada llamada a schema_iteration.
*/
G_MODULE_EXPORT guint schema_interval = 1000;


/*******************************************************
 *                  Funciones                          *
 *******************************************************/

/*
*       Esta función será un símbolo del plugin
*       que se podrá llamar a voluntad en cualquier
*       momento, una vez se haya cargado el plugin.
*
*/
G_MODULE_EXPORT void schema_startup (void) {
   GError *error;

   if(started) {
      schema_thread = gtk_timeout_add(schema_interval, (GtkFunction)schema_iteration, &error);
      if (!schema_thread) {
         g_printerr ("Failed on schema_startup: %s\n", error->message);
         schemaSetState(schema_id, slept);
      }
      else {
         schemaSetState(schema_id, winner);
      }
   }
   else {
      started = TRUE;

      g_print ("\n--- Startup de esquema %s ---\n", schema_id);

      /* La primera vez devuelve la tabla de símbolos que exporta */
      putSchemaVar((gpointer)&shared, "shared", schema_id);


   }
}

/*
*       Esta función será un símbolo del plugin
*       que se podrá llamar a voluntad en cualquier
*       momento, una vez se haya cargado el plugin.
*
*/
G_MODULE_EXPORT void schema_suspend (void) {
   g_print ("\n--- Suspendiendo esquema %s ---\n", schema_id);
   g_source_remove(schema_thread);
   schemaSetState(schema_id, slept);
}

/*
*       Función principal. Aquí se lleva a cabo
*       el trabajo sucio
*
*/
gboolean schema_iteration(gpointer data) {
   static int i = 0;
   g_print("Iteracion %d de %s. En interval tenemos: %d\n", ++i, schema_id, schema_interval);
   return TRUE;
}
