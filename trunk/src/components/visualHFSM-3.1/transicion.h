#include <iostream>

#include <gtk/gtk.h>
#include <libgnomecanvas/libgnomecanvas.h>



using namespace std;


typedef struct tipoTransicion {
	//int id;
	GnomeCanvasItem * item;	// Item que representa la transición. Está formado a su vez por 3 item: 2 líneas y un punto de acción en la unión de ambas (box).
				// Posicion 0 inicio recta, posicion 1 final recta, posicion 2 box.
	//int origenX, origenY;		
	//int destinoX, destinoY;
	//double pMedioX, pMedioY;	// Posición del punto de acción de la transición.
	GnomeCanvasItem * origen;	// Nodo origen
	GnomeCanvasItem * destino;	// Nodo destino
	string nombre;			// Nombre de la transicion. "" por defecto
	GnomeCanvasItem * item_nombre;	// Item del nombre de la transición.
	//GnomeCanvasItem * orientacion;	// Triangulo flecha
	string codigo;
	int tiempo;
} tTransicion; 

typedef struct transicion_aux {		// Transiciones temporales. Usadas para pintar las transiciones despues de cargar un fichero.
	GnomeCanvasItem *origen;
	GnomeCanvasItem *destino;
	int origen_xml;
	int destino_id;
	int tiempo;
	string codigo;
	string nombre;
	GnomeCanvasItem *item;
} transicion_aux;

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
