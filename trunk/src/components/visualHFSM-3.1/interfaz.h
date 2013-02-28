#include <iostream>
#include <list>
#include "nodo.h"
#include "ventanas.h"
//#include <gtk/gtk.h>

using namespace std;

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
