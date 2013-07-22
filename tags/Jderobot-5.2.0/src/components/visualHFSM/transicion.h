#ifndef TRANSICION_H
#define TRANSICION_H

#include <iostream>

#include <gtk/gtk.h>
#include <libgnomecanvas/libgnomecanvas.h>

#include <list>
#include <cstring>


#include "recta.h"
#include "nodo.h"
#include "interfaz.h"
using namespace std;

/*   Devuelve el identificador de la transición   */
int id_transicion ();

/*      */
GnomeCanvasItem * get_box (GnomeCanvasItem * item);

/*   Cambia el id dela transicion   */
bool cambiar_item_transicion (GnomeCanvasItem * item, GnomeCanvasItem * new_item);

/***** Pintar Transiciones *****/

/*** Pintar AutoTransicion ***/
GnomeCanvasItem *pinta_autotransicion (GnomeCanvasItem *item);

/***	Repintar transicion al mover elementos	***/
// origen_ref hace referencia al nodo que se está moviendo. Si se está moviendo el centro de la transicion poner origen_ref a NULL.
// Devuelve el grupo que hace referencia a los elementos de la transicion. Si origen_ref es NULL sólo se devuelven las lineas que componen la transicion.
GnomeCanvasGroup * repintar_transicion (GnomeCanvasGroup *root, GnomeCanvasItem *origen, GnomeCanvasItem *destino, double x, double y);

// Al mover nodos
// Devuelve el grupo completo de la transicion
GnomeCanvasGroup * repintar_transicion_nodo (GnomeCanvasGroup *root, GnomeCanvasItem *transicion, GnomeCanvasItem *nodo, char * direccion);

/***   Borra la transicion   ***/

//	type puede ser "origen", "destino" o "total". Depnde de si se quiere borrar toda la transcicion o la mitad de ella
void borrar_transicion (GnomeCanvasItem *origen, GnomeCanvasItem *destino, char * type);

//void borrar_transicion (GnomeCanvasItem *transicion, char * type);

/***	Lista Transiciones	***/
void add_transicion_lista (GnomeCanvasItem *transicion, GnomeCanvasItem *origen, GnomeCanvasItem *destino, GnomeCanvasItem * nombre, string codigo, int tiempo);

//	Eliminar transicion lista	//
void t_eliminar_transicion_lista (GnomeCanvasItem *transicion);

/***	Otros	***/
int numero_transiciones (GnomeCanvasItem *estado1, GnomeCanvasItem *estado2);

void eliminar_nombre (GnomeCanvasItem *transicion);

#endif
