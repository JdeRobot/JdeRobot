#ifndef TIPOS_H
#define TIPOS_H

#include <libgnomecanvas/libgnomecanvas.h>
#include <list>
#include <string>
using namespace std;

typedef struct punto {
	double x;
	double y;
} punto; 

typedef struct recta {
	double A;
	double B;
	double C;
} recta; 


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

extern list <tNodo> ListaElementos;
extern list <tTransicion> ListaTransiciones;

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

typedef struct importar {
	bool laser;
	bool motor;
	bool radar;
	bool encoders;
	bool lat_lon;
	bool camara;
	bool ptencoders;
} importar; 


typedef enum TYPE_VENTANA {
	NOMBRAR,
	EDITAR,
	CODIGO,
	LIBRERIAS,
	TIMER
}tVentana;

typedef struct nodoVentana {
	GnomeCanvasItem * item;		// item correspondiente a la ventana
	tVentana tipo;			// Nombre del nodo. "" por defecto
	GtkWidget *ventana;		// Ventana
} nVentana; 

typedef struct tSubAut {
	list <tNodo> ListaElementosSub;
	list <tTransicion> ListaTransicionesSub;
	int tiempoIteracionSub;	
	string variablesSub;
	string funcionesSub;
	importar impSub;
	int idSub;
	int idPadre;
} tSubAut; 

#endif
