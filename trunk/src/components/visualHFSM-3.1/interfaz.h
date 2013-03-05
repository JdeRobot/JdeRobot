#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <list>

#include <gdk/gdkkeysyms.h>	//NOMBRES TECLAS

#include <gtk/gtk.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcebuffer.h>
#include <gtksourceview/gtksourcelanguage.h>
#include <gtksourceview/gtksourcelanguagemanager.h>

#include <libgnomecanvas/libgnomecanvas.h>
#include <libglade-2.0/glade/glade.h>
#include <libxml/tree.h>

#include <sys/types.h>
#include <unistd.h>

#include "transicion.h"
#include "xml.h"
#include "int2string.h"
#include "imprimir.h"

#include "tipos.h"
#include "ventanas.h"
#include "recta.h"

//#include <gtk/gtk.h>

using namespace std;



void
create_drag_box (GnomeCanvasGroup *group, char *box_name, double x1, double y1, GCallback callback);

gint
item_event (GnomeCanvasItem *item_group, GdkEvent *event, gpointer data);

gint
highlight_box (GnomeCanvasItem *item, GdkEvent *event, gpointer data);

gint
highlight_box_edit (GnomeCanvasItem *item, GdkEvent *event, gpointer data);

void 
mostrar_subautomata(int pidSub);

void 
ocultar_subautomata(int pidSub);

void 
on_menu_estado_nombrar();

void 
on_menu_estado_editar();

void 
on_menu_estado_marcar_inicial();

void 
on_menu_estado_copiar();

void 
on_menu_estado_eliminar();

void 
on_menu_transicion_nombrar();

void 
on_menu_transicion_editar();

void 
on_menu_transicion_eliminar();

void 
on_menu_pegar();


static GtkTreeModel *
create_and_fill_model ();

void
actualizar_tree_view();
/*
static GtkWidget *
create_view_and_model();*/

#endif
