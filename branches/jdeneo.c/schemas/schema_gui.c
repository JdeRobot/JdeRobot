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

const gchar* pathgui = "gui/gui.glade";
gboolean schema_iteration(gpointer data);
guint schema_thread;
static gboolean loaded = FALSE;
static gboolean gui_loaded = FALSE;

/*******************************************************
 *                                                     *
 *                   CALLBACKS                         *
 *                                                     *
 *******************************************************/
void on_button_clicked(GtkButton *button, gpointer user_data) {
   g_print("Boton pulsado\n");
}

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
G_MODULE_EXPORT gchar* schema_id = "schema_gui";

/*
*       OPCIONAL:
*       Los esquemas que tienen gui podrán acceder a los elementos
*       de ella a través de esta variable _iniciada por jde_.
*/
GladeXML *schema_gui;

/*
*       Definimos una variable cuyo contenido es el tiempo en milisegundos
*       que hay entre cada llamada a schema_iteration.
*/
G_MODULE_EXPORT guint schema_interval = 1000;


/*******************************************************
 *                  Funciones                          *
 *******************************************************/

/*
*       OPCINAL: (Necesario si definido schema_gui)
*       Esta función conecta todos los callbacks del gui.
*
*       Es necesario que conecten los callbacks desde aquí
*       puesto que jde no tendrá acceso a las funciones.
*
*/
G_MODULE_EXPORT gboolean schema_gui_startup () {
   GtkWidget *win, *button;

   if (gui_loaded) {
      g_print ("--- Inicializando gui del esquema %s---\n\n", schema_id);
      win = glade_xml_get_widget(schema_gui, "window1");
      gtk_widget_show(win);
   }
   else {
      gui_loaded = TRUE;

      schema_gui = glade_xml_new (g_strconcat(schema_path, "/", pathgui, NULL), NULL, NULL);
      if (!schema_gui)
         g_error("Error. No se ha inicializado correctamente la interfaz del esquema %s\n", schema_id);

      /* Atamos los callbacks */
      button = glade_xml_get_widget(schema_gui, "button");
      g_signal_connect (G_OBJECT (button), "clicked",
                     G_CALLBACK (on_button_clicked), NULL);
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

/*
*       Esta función será un símbolo del plugin
*       que se podrá llamar a voluntad en cualquier
*       momento, una vez se haya cargado el plugin.
*
*/
G_MODULE_EXPORT void schema_startup (void) {
   GError *error;

   if(loaded) {
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
      loaded = TRUE;
      g_print ("\n--- Startup de esquema %s ---\n", schema_id);
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
   gchar label_text[20] = "";
   sprintf(label_text,"iteracion %d",i++);
   gtk_label_set_text(GTK_LABEL(glade_xml_get_widget(schema_gui, "label")), label_text);
   return TRUE;
}
