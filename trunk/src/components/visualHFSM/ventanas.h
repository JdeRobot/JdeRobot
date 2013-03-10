#ifndef VENTANAS_H
#define VENTANAS_H

#include <iostream>
#include <list>
#include <string.h>

#include "transicion.h"
#include "int2string.h"
 
#include <gtk/gtk.h>
#include <libgnomecanvas/libgnomecanvas.h>
#include <libglade-2.0/glade/glade.h>

#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcebuffer.h>
#include <gtksourceview/gtksourcelanguage.h>
#include <gtksourceview/gtksourcelanguagemanager.h>

#include "tipos.h"

/****EDICION DE ESTADOS****/

void cancelar (GtkWidget *widget, gpointer data);

void aplicar (GtkButton *button, gpointer data);

/*	Cambiar nombre		*/
GtkWidget * new_name_windows (GnomeCanvasItem *item);

/*	Edicion de elementos		*/
GtkWidget * new_text_windows (GnomeCanvasItem *item, gchar *type);

/****EDICION DEL ESQUEMA****/
GtkWidget *  new_code_windows ();

void set_importar(GtkButton *Button, gpointer data);
GtkWidget * importar_librerias();

void set_timer (GtkWindow *window, gpointer data);
GtkWidget * tiempo_iteracion();

/****LISTA VENTANAS****/

void add_ventana (GtkWidget *ventana, GnomeCanvasItem * item, tVentana tipo);
void eliminar_ventana (GtkWidget *ventana, gpointer data);

/*	Comprueba si la ventana correspondiente esta abierta	*/
GtkWidget * comprobar_ventana_abierta (GnomeCanvasItem * item, tVentana tipo);


/* Función útil a la hora de cambiar el color de relleno de un item
 * del canvas aleatoriamente
 */
void change_item_color (GnomeCanvasItem *item, int color);
#endif
