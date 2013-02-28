#include <iostream>
#include <list>

#include <gtk/gtk.h>
#include <libgnomecanvas/libgnomecanvas.h>

using namespace std;


typedef struct tipoNodo {

	GnomeCanvasItem * item;	
	GnomeCanvasItem * estado_inicial;		
//	int origenX, origenY;		// Punto (x,y) donde se dibujara en el canvas
//	int destinoX, destinoY;		// Punto (x,y) donde se dibujara en el canvas
	string nombre;			// Nombre del nodo. "" por defecto
	string codigo;		// Codigo del estado
	GnomeCanvasItem * item_nombre;		// item que muestra el nombre del estado
	list <GnomeCanvasItem *> listaAdyacentes;  // Para estados -- Lista de lineas adyacentes 
	int idHijo;  //Id. que indica el subautomata dentro de la lista de subautomatas que se corresponde con el hijo de este estado
} tNodo; 

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
