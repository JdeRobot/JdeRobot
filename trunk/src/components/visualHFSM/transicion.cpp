#include "transicion.h"
int cont_transiciones = 0;



//extern list <transicion_aux> ListaTransicionesTemporales;
//extern list <GtkWidget *> ListaBotones;
//extern GnomeCanvasGroup *root;

GnomeCanvasItem * get_box (GnomeCanvasItem * item)
{
	GList * list_items;

	list_items = GNOME_CANVAS_GROUP (item)->item_list;
	
	return (GnomeCanvasItem *) g_list_nth_data (list_items, g_list_length(list_items)-1);
}

/*   Cambia el id dela transicion   */
bool cambiar_item_transicion (GnomeCanvasItem * item, GnomeCanvasItem * new_item)
{
	list<tTransicion>::iterator posLinea;
	bool b = false;

	posLinea = ListaTransiciones.begin();

	while( (posLinea != ListaTransiciones.end()) && (posLinea->item != item) )
	{

		posLinea++;

	}

	if (posLinea->item == item){

		posLinea->item = new_item;
		b = true;

	}

	return b;
}

/***** Pintar AutoTransicion *****/
/*   */

int num_autotransiciones (GnomeCanvasItem *nodo)
{
	list<tTransicion>::iterator posLinea;
	int n = 0;

	posLinea = ListaTransiciones.begin();

	while( posLinea != ListaTransiciones.end() )
	
	{
		if ((posLinea->origen == nodo) && (posLinea->destino == nodo))
			n++;

		posLinea++;
		
	}

	return n;
}

GnomeCanvasItem *
pinta_autotransicion (GnomeCanvasItem *nodo)
{
	double I1x1, I1x2, I1y1, I1y2; // Puntos del orgien
	double x1,y1;
	int n;
	GnomeCanvasPoints *points = gnome_canvas_points_new (4);
	GnomeCanvasItem * item;
	GnomeCanvasGroup * group;
	GnomeCanvas *canvas;
	
	g_object_get (G_OBJECT (nodo), "x1", &I1x1, NULL);
	g_object_get (G_OBJECT (nodo), "y1", &I1y1, NULL);
	g_object_get (G_OBJECT (nodo), "x2", &I1x2, NULL);
	g_object_get (G_OBJECT (nodo), "y2", &I1y2, NULL);
	
	x1 = (I1x1 + I1x2)/2;
	y1 = (I1y1 + I1y2)/2 + abs(I1y1 - I1y2)/2;
	
	canvas = nodo->canvas;

	group = GNOME_CANVAS_GROUP (gnome_canvas_item_new (GNOME_CANVAS_GROUP(canvas->root),  
                                        gnome_canvas_group_get_type (),
                                        "x", 0,
                                        "y", 0,
                                        NULL));

	/*item = gnome_canvas_item_new(group,
                        	gnome_canvas_ellipse_get_type(),
                        	"x1", x1-7,
                        	"y1", y1,
                        	"x2", x1+7,
                        	"y2", y1+14,
                        	//"fill_color_rgba", 0x00ffffff,
                        	"outline_color", "black",
                        	"width_units", 1.0,
                        	NULL);
                	
        points->coords[0] = x1-0.5;
	points->coords[1] = y1+14;
	points->coords[2] = x1+0.5;
	points->coords[3] = y1+14 + 0.75;
	
        gnome_canvas_item_new(group, gnome_canvas_line_get_type (),
                	"points", points,
			"fill_color", NULL, 
			"width_units", 1.0,  
			//"smooth", TRUE,
			//"first_arrowhead", TRUE,
                        "last_arrowhead", TRUE,
			"arrow_shape_a", 3.0, 
			"arrow_shape_b", 8.0,
			"arrow_shape_c", 3.0, 
			NULL);*/
	n = num_autotransiciones (nodo);

	points->coords[0] = x1-8;
	points->coords[1] = y1-1;
	points->coords[2] = x1-8;
	points->coords[3] = y1+20 + (n*20);
	points->coords[4] = x1+8;
	points->coords[5] = y1+20 + (n*20);
	points->coords[6] = x1+8;
	points->coords[7] = y1-1;
	item = gnome_canvas_item_new(group, gnome_canvas_line_get_type (),
                	"points", points,
			"fill_color", "orange", 
			"width_units", 1.0,  
			"smooth", TRUE,
			"join-style", GDK_JOIN_BEVEL,
			//"first_arrowhead", TRUE,
                        "last_arrowhead", TRUE,
			"arrow_shape_a", 3.0, 
			"arrow_shape_b", 8.0,
			"arrow_shape_c", 3.0, 
			NULL);
        create_drag_box (group, (char *)"box", x1+1, y1+20+(n*20), G_CALLBACK (highlight_box_edit));
        
        return GNOME_CANVAS_ITEM(group);
}


GnomeCanvasGroup * repintar_transicion (GnomeCanvasGroup *root, GnomeCanvasItem *origen, GnomeCanvasItem *destino, double x, double y)
{

	GnomeCanvasItem *item1,*item2;
	GnomeCanvasGroup *group;
	
	double I1x1, I1x2, I1y1, I1y2; // Puntos del orgien
	double I2x1, I2x2, I2y1, I2y2; // Puntos del destino
	

	g_object_get (G_OBJECT (origen), "x1", &I1x1, NULL);
	g_object_get (G_OBJECT (origen), "y1", &I1y1, NULL);
	g_object_get (G_OBJECT (origen), "x2", &I1x2, NULL);
	g_object_get (G_OBJECT (origen), "y2", &I1y2, NULL);

	g_object_get (G_OBJECT (destino), "x1", &I2x1, NULL);
	g_object_get (G_OBJECT (destino), "y1", &I2y1, NULL);
	g_object_get (G_OBJECT (destino), "x2", &I2x2, NULL);
	g_object_get (G_OBJECT (destino), "y2", &I2y2, NULL);

	//Punto medio circulo origen
	double Xm1 = ((I1x2 + I1x1) / 2);
	double Ym1 = ((I1y2 + I1y1) / 2);
	punto pCirculoOrigen = crear_punto (Xm1, Ym1);

	//Punto medio circulo destino
	double Xm2 = ((I2x2 + I2x1) / 2);
	double Ym2 = ((I2y2 + I2y1) / 2);
	punto pCirculoDestino = crear_punto (Xm2, Ym2);

	// Punto Medio Transicion
	punto pMedioTransicion = crear_punto (x, y);

	recta recta_origen_pm = crear_recta (pCirculoOrigen, pMedioTransicion);

	recta recta_destino_pm = crear_recta (pCirculoDestino, pMedioTransicion);

	// Ecuacion Recta Perpendicular que pasa por el punto medio del circulo origen
	recta recta_perpendicular_circulo_origen = recta_perpendicular (recta_origen_pm, pCirculoOrigen);
	
	// Ecuacion Recta Paralela	d=(ax+by+c)/(a^2 + b^2)^1/2
	recta recta_paralela1 = recta_paralela (recta_perpendicular_circulo_origen, pCirculoOrigen, -20);


	// Ecuacion Recta Perpendicular que pasa por el punto medio del circulo destino
	recta recta_perpendicular_circulo_destino = recta_perpendicular (recta_destino_pm, pCirculoDestino);
	
	// Ecuacion Recta Paralela	d=(ax+by+c)/(a^2 + b^2)^1/2
	recta recta_paralela2 = recta_paralela (recta_perpendicular_circulo_destino, pCirculoDestino, -20);

	double YI1, XI1; 
	punto pInterseccion = interseccion_rectas (recta_origen_pm, recta_paralela1);
	punto_get_values (pInterseccion, &XI1, &YI1);
	
	double YI2, XI2;
	pInterseccion = interseccion_rectas (recta_destino_pm, recta_paralela2);
	punto_get_values (pInterseccion, &XI2, &YI2);

	group = GNOME_CANVAS_GROUP (gnome_canvas_item_new (root,
                                             gnome_canvas_group_get_type (),
                                             "x", 0,
                                             "y", 0,
                                             NULL));

	GnomeCanvasPoints *points = gnome_canvas_points_new (2);
	


	points->coords[0] = XI1;
	points->coords[1] = YI1;
	points->coords[2] = x;
	points->coords[3] = y;

	item1 = gnome_canvas_item_new(group, gnome_canvas_line_get_type (),
                        	"points", points,
				"fill_color", "orange", 
				"width_units", 1.0,  
				//"first_arrowhead", TRUE,
                              	//"last_arrowhead", TRUE,
				//"arrow_shape_a", 8.0, 
				//"arrow_shape_b", 8.0,
				//"arrow_shape_c", 8.0, 
				NULL);

	points->coords[0] = x;
	points->coords[1] = y;
	points->coords[2] = XI2;
	points->coords[3] = YI2;

	item2 = gnome_canvas_item_new(group, gnome_canvas_line_get_type (),
                        	"points", points,
				"fill_color", "orange", 
				"width_units", 1.0,  
				//"first_arrowhead", TRUE,
                              	"last_arrowhead", TRUE,
				"arrow_shape_a", 8.0, 
				"arrow_shape_b", 8.0,
				"arrow_shape_c", 8.0, 
				NULL);

	gnome_canvas_points_free (points);


	return group;
	

}

GnomeCanvasGroup * repintar_transicion_nodo (GnomeCanvasGroup *root, GnomeCanvasItem *transicion, GnomeCanvasItem *nodo, char * direccion)
{

	GList * list_items, *list_aux;	
	GnomeCanvasItem *item, *box, *arrow;
	GnomeCanvasGroup *group;
	GnomeCanvasPoints *points;
	double x1,y1,x2,y2, x, y;

	group = GNOME_CANVAS_GROUP (transicion);
	list_items = group->item_list;

	box = GNOME_CANVAS_ITEM(g_list_nth_data (list_items, 2));
	

	g_object_get (G_OBJECT (box), "x1", &x1, NULL);
	g_object_get (G_OBJECT (box), "y1", &y1, NULL);
	g_object_get (G_OBJECT (box), "x2", &x2, NULL);
	g_object_get (G_OBJECT (box), "y2", &y2, NULL);

	x = (x1+x2)/2;
	y = (y1+y2)/2;
		

	double I1x1, I1x2, I1y1, I1y2; // Puntos del nodo
	
	g_object_get (G_OBJECT (nodo), "x1", &I1x1, NULL);
	g_object_get (G_OBJECT (nodo), "y1", &I1y1, NULL);
	g_object_get (G_OBJECT (nodo), "x2", &I1x2, NULL);
	g_object_get (G_OBJECT (nodo), "y2", &I1y2, NULL);
			
	//Punto medio nodo
	double Xm1 = ((I1x2 + I1x1) / 2);
	double Ym1 = ((I1y2 + I1y1) / 2);
	punto pNodo = crear_punto (Xm1, Ym1);

	// Punto Medio Transicion
	punto pMedioTransicion = crear_punto (x, y);

	recta recta_nodo_pm = crear_recta (pNodo, pMedioTransicion);

	// Ecuacion Recta Perpendicular que pasa por el punto medio del circulo origen
	recta recta_perpendicular_nodo = recta_perpendicular (recta_nodo_pm, pNodo);
	
	// Ecuacion Recta Paralela	d=(ax+by+c)/(a^2 + b^2)^1/2
	recta recta_paralela_nodo = recta_paralela (recta_perpendicular_nodo, pNodo, -20);

	double YI1, XI1; 
	punto pInterseccion = interseccion_rectas (recta_nodo_pm, recta_paralela_nodo);
	punto_get_values (pInterseccion, &XI1, &YI1);

	points = gnome_canvas_points_new (2);

	if (strcmp ( direccion, (char *) "origen" ) == 0)
	{
		arrow = (GnomeCanvasItem *) g_list_nth_data (list_items, 0);
		list_items=g_list_next(list_items);
		gtk_object_destroy (GTK_OBJECT (arrow));
				

		points->coords[0] = XI1;
		points->coords[1] = YI1;
		points->coords[2] = x;
		points->coords[3] = y;

		item = gnome_canvas_item_new(group, gnome_canvas_line_get_type (),	// Si se define el "parent" del item como root da fallo de segmentacion
	                        	"points", points,				
					"fill_color", "orange", 
					"width_units", 1.0,  
					NULL);

		//list_items = g_list_insert (list_items, (gpointer)item, 0);	
		list_aux = g_list_nth  (list_items, 2);					// Reordenar los elementos dentro del grupo
		list_items = g_list_remove_link (list_items, list_aux);
		list_items = g_list_insert (list_items, (gpointer)item, 0);
	}
	else if (strcmp ( direccion, (char *) "destino" ) == 0)
	{
		arrow = (GnomeCanvasItem *) g_list_nth_data (list_items, 1);
		//list_aux = g_list_nth  (list_items, 2);	
		gtk_object_destroy (GTK_OBJECT (arrow));
		//list_items = g_list_concat (list_items, list_aux);
		

		points->coords[0] = x;
		points->coords[1] = y;
		points->coords[2] = XI1;
		points->coords[3] = YI1;

		item = gnome_canvas_item_new(group, gnome_canvas_line_get_type (),
	                        	"points", points,
					"fill_color", "orange", 
					"width_units", 1.0,  
	                              	"last_arrowhead", TRUE,
					"arrow_shape_a", 8.0, 
					"arrow_shape_b", 8.0,
					"arrow_shape_c", 8.0, 
					NULL);

		//list_items = g_list_insert (list_items, (gpointer)item, 1);
		list_aux = g_list_nth  (list_items, 2);
		list_items = g_list_remove_link (list_items, list_aux);
		list_items = g_list_insert (list_items, (gpointer)item, 1);
	}
	
	group->item_list = list_items;

	//gnome_canvas_item_reparent (box, transicion);

	return GNOME_CANVAS_GROUP (group);
	
}


void borrar_transicion (GnomeCanvasItem *origen, GnomeCanvasItem *destino, char * type)
{
	list<tTransicion>::iterator posLinea;
	list<tNodo>::iterator pos2;
	list<GnomeCanvasItem *>::iterator pos3;
	list<GnomeCanvasItem *>::iterator posAux;

	posLinea = ListaTransiciones.begin();

	while( (posLinea != ListaTransiciones.end()) && (posLinea->origen != origen) && (posLinea->destino != destino))
	{

		posLinea++;

	}


	if (strcmp ( type, (char *) "total" ) == 0){
		
		gtk_object_destroy (GTK_OBJECT (origen));
		gtk_object_destroy (GTK_OBJECT (destino));

	}
	else if (strcmp ( type, (char *) "origen" ) == 0){

		gtk_object_destroy (GTK_OBJECT (origen));

	}
	else if (strcmp ( type, (char *) "destino" ) == 0){

		gtk_object_destroy (GTK_OBJECT (destino));

	}else
		cout << "tipo incorrecto" << endl;

	


}
void add_transicion_lista (GnomeCanvasItem *transicion, GnomeCanvasItem *origen, GnomeCanvasItem *destino, GnomeCanvasItem * nombre, string codigo, int tiempo)
{

	tTransicion l;
	gchararray name;
	//char *name;

	l.item = transicion;
	l.origen = origen;
	l.destino = destino;
	l.codigo = codigo;
	l.tiempo = tiempo;
	l.item_nombre = nombre;
	if (nombre != NULL){
		g_object_get (G_OBJECT (nombre), "text", &name, NULL);
		l.nombre = name;
	}else
		l.nombre = "";

	ListaTransiciones.push_back(l);







}

/***	Eliminar la transicion de la lista	***/
void t_eliminar_transicion_lista (GnomeCanvasItem *transicion)
{
	list<tTransicion>::iterator posLinea;

	posLinea = ListaTransiciones.begin();

	while( (posLinea != ListaTransiciones.end()) && (posLinea->item != transicion) )
		posLinea++;

	n_borrar_transicion_lista (posLinea->origen,transicion);
	
	if (posLinea->destino != posLinea->origen)
		n_borrar_transicion_lista (posLinea->destino,transicion);

	ListaTransiciones.erase(posLinea);


}


int 
numero_transiciones (GnomeCanvasItem *estado1, GnomeCanvasItem *estado2)
{
	list<tTransicion>::iterator pos;
	int contador = 0;

	pos = ListaTransiciones.begin();

	while ( pos !=  ListaTransiciones.end() )
	{	
		if (((pos->origen == estado1) && (pos->destino == estado2)) || ((pos->origen == estado2) && (pos->destino == estado1)))
			contador++;
		pos++;
	}
	return contador;

}

void eliminar_nombre (GnomeCanvasItem *transicion)
{
	list<tTransicion>::iterator posLinea;

	posLinea = ListaTransiciones.begin();

	while( (posLinea != ListaTransiciones.end()) && (posLinea->item != transicion) )
		posLinea++;

	posLinea->nombre="";
	
	gtk_object_destroy (GTK_OBJECT (posLinea->item_nombre));


}

