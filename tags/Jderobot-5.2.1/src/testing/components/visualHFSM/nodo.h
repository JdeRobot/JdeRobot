#ifndef NODO_H
#define NODO_H

#include <iostream>
#include <list>
 
#include <gtk/gtk.h>
#include <libgnomecanvas/libgnomecanvas.h>

#include "tipos.h"

using namespace std;


/*	Copia y añade un nodo a la lista	*/
void copiar_nodo (GnomeCanvasItem * item, GnomeCanvasItem * item_cp);

/*	cambia la transicion asociada	*/
void cambiar_transicion (int id, GnomeCanvasItem * old_item, GnomeCanvasItem * new_item);

/*	Cambia la transicion adyacente al nodo "id_item"	*/
bool cambiar_item_adyacentes (GnomeCanvasItem * id_item, GnomeCanvasItem * old_item, GnomeCanvasItem * new_item);

/*	borrar transicion de la lista	*/
void n_borrar_transicion_lista (GnomeCanvasItem * nodo, GnomeCanvasItem * transicion);

/*		*/
GnomeCanvasItem * pinta_estado_inicial (GnomeCanvasGroup *group, GnomeCanvasItem *item);

void get_bounds (GnomeCanvasItem *nodo, double *x1, double *y1, double *x2, double *y2);

gint item_event2 (GnomeCanvasItem *item_group, GdkEvent *event, gpointer data);

#endif
