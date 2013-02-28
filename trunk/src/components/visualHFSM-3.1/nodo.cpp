#include <iostream>
#include <list>

#include "recta.h"
#include "nodo.h"
#include "transicion.h"
 
#include <gtk/gtk.h>
#include <libgnomecanvas/libgnomecanvas.h>
//int cont_nodos = 0;


extern list <tNodo> ListaElementos;
extern GnomeCanvasGroup *root;

void copiar_nodo (GnomeCanvasItem * item, GnomeCanvasItem * item_cp)
{
	tNodo nodo;
	list<tNodo>::iterator posNodos;
	//GnomeCanvasItem * item_nombre;
	//double x2,x1,y2,y1;

	posNodos = ListaElementos.begin();

	while( (posNodos != ListaElementos.end()) && (posNodos->item != item) )
		posNodos++;

	nodo.item = item_cp;
	nodo.nombre = "";
	nodo.codigo = posNodos->codigo;
	nodo.estado_inicial = NULL;

//	g_object_get (G_OBJECT (item_cp), "x1", &x1, NULL);
//	g_object_get (G_OBJECT (item_cp), "y1", &y1, NULL);
//	g_object_get (G_OBJECT (item_cp), "x2", &x2, NULL);
//	g_object_get (G_OBJECT (item_cp), "y2", &y2, NULL);

	/*item_nombre = gnome_canvas_item_new (root,
	            gnome_canvas_text_get_type (),
             	    "text", posNodos->nombre.c_str(),
		    "x", (x2+x1)/2,
		    "y", (y2+y1)/2,
		    "font", "Sans 12",
		    "size", 32000,
		    "size_set", (gboolean)TRUE,
		    "scale",  (gdouble) 3,
		    "scale_set", (gboolean)TRUE,
	    	    "anchor", GTK_ANCHOR_N,
	            "fill_color", "black",
	                     NULL); */
	
	nodo.item_nombre = NULL;

	ListaElementos.push_back(nodo);
}

bool cambiar_item_adyacentes (GnomeCanvasItem * id_item, GnomeCanvasItem * old_item, GnomeCanvasItem * new_item)
{

	list<tNodo>::iterator posNodos;
	list<GnomeCanvasItem *>::iterator posAdy;
	bool b = false;

	posNodos = ListaElementos.begin();

	while( (posNodos != ListaElementos.end()) && (posNodos->item != id_item) )
		
		posNodos++;


	posAdy = posNodos->listaAdyacentes.begin();
	
	while( (posAdy != posNodos->listaAdyacentes.end()) && (*posAdy != old_item) )

		posAdy++;
	
	if (*posAdy == old_item){ 
		
		*posAdy = new_item;
		b = true;
	}


	return b;
}


void n_borrar_transicion_lista (GnomeCanvasItem * nodo, GnomeCanvasItem * transicion)
{
	list<tNodo>::iterator posNodo;
	list<GnomeCanvasItem *>::iterator posAdy;
		
	posNodo = ListaElementos.begin();

	while(posNodo->item != nodo)
  		posNodo++;
				
	posAdy = posNodo->listaAdyacentes.begin();

	while((*posAdy) != transicion)
  		posAdy++;

	posNodo->listaAdyacentes.erase(posAdy);

}

GnomeCanvasItem * pinta_estado_inicial (GnomeCanvasGroup *group, GnomeCanvasItem *item) {

	double x1,x2,y1,y2;
	GnomeCanvasItem * estado_inicial;

	g_object_get (G_OBJECT (item), "x1", &x1, NULL);
	g_object_get (G_OBJECT (item), "y1", &y1, NULL);
	g_object_get (G_OBJECT (item), "x2", &x2, NULL);
	g_object_get (G_OBJECT (item), "y2", &y2, NULL);

	estado_inicial = gnome_canvas_item_new(group,
                        gnome_canvas_ellipse_get_type(),
                        "x1", (double) x1 + 5,
                        "y1", (double) y1 + 5,
                        "x2", (double) x2 - 5,
                        "y2", (double) y2 - 5,
                        "fill_color_rgba", NULL,
                        "outline_color", "black",
                        "width_units", 1.0,
                        NULL);

	return estado_inicial;
}

void get_bounds (GnomeCanvasItem *nodo, double *x1, double *y1, double *x2, double *y2) {

	g_object_get (G_OBJECT (nodo), "x1", x1, NULL);
	g_object_get (G_OBJECT (nodo), "y1", y1, NULL);
	g_object_get (G_OBJECT (nodo), "x2", x2, NULL);
	g_object_get (G_OBJECT (nodo), "y2", y2, NULL);


}

gint item_event2 (GnomeCanvasItem *item_group, GdkEvent *event, gpointer data)
{
	cout << ""<< endl;
	return true;
}

