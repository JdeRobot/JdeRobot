
#include <iostream>
#include <string>
#include <list>
#include <string.h>

//#include "nodo.h"
#include "transicion.h"
//#include "ventanas.h"
#include "int2string.h"
#include "xml.h"
#include "interfaz.h"


#include <libxml/tree.h>

int cont_nodos = 0;
//************************Añadida lista de subautomatas
extern list <tSubAut> ListaSubAutomatas;
//*****************************************************

extern list <tNodo> ListaElementos;
extern list <tTransicion> ListaTransiciones;
extern list <transicion_aux> ListaTransicionesTemporales;

extern string variables;
extern string funciones;
extern importar imp;
extern int tiempoIteracion;

extern string directorio;	
extern string nombreEsquema;

extern GnomeCanvasGroup *root;

extern GnomeCanvasItem *item_guardado;	// item guardado para operaciones

void actualizar_array_id (tId *array)
{
	list<tNodo>::iterator posNodo;

	posNodo = ListaElementos.begin();

	while(posNodo != ListaElementos.end()){		
 		array[cont_nodos].id = cont_nodos;
		array[cont_nodos].item = posNodo->item;
		cont_nodos++;
		posNodo++;
	}
			

}

int posicion (GnomeCanvasItem *nodo)
{
	int cont = 1; 
	list<tNodo>::iterator posNodos;	

	posNodos = ListaElementos.begin();

	while(posNodos->item != nodo){
		cont++;
  		posNodos++;
	}
	return cont;
}	


xmlDocPtr xml_new_doc (const gchar *name) {

	xmlNodePtr root;
	xmlDocPtr doc;

	doc = xmlNewDoc ((const xmlChar*)"1.0");

	root = xmlNewDocNode (doc, NULL, (const xmlChar*)name, NULL);
	
	xmlDocSetRootElement (doc, root);

	return doc;
}

void xml_new_entry (xmlDocPtr doc) {//****************Creado xml_new_entry_sub, trasladando el xml_new_entry original para cada uno de los subautomatas
									//****************de ListaSubAutomatas

	xmlNodePtr root;
	xmlNodePtr nodeSub;
	string cadena;
	list<tSubAut>::iterator posNodoSub;	
	
	root = xmlDocGetRootElement (doc);
		

	if (!ListaSubAutomatas.empty())
	{ 
		posNodoSub = ListaSubAutomatas.begin();
	
		while( posNodoSub != ListaSubAutomatas.end())
		{	
			/* nodo raiz */
			nodeSub = xmlNewChild (root, NULL, (const xmlChar*)"SubAutomata", NULL);

			cadena = int2string(posNodoSub->idSub);

			xmlNewChild (nodeSub, NULL, (const xmlChar*)"idSub", (const xmlChar*)cadena.c_str());

			cadena = int2string(posNodoSub->idPadre);

			xmlNewChild (nodeSub, NULL, (const xmlChar*)"idPadre", (const xmlChar*)cadena.c_str());


			//cargamos en las "listas globales" las listas del subautomata que estamos tratando, de este modo el procedimiento 
			//que habia antes para generar el xml a partir de esas listas puede seguir funcionando igual
			ListaElementos = posNodoSub->ListaElementosSub;
			ListaTransiciones = posNodoSub->ListaTransicionesSub;
			tiempoIteracion = posNodoSub->tiempoIteracionSub;
			variables = posNodoSub->variablesSub;
			funciones = posNodoSub->funcionesSub;
			imp = posNodoSub->impSub;
			
			
			//Llamamos al procedimiento que habia antes para componer la estructura del xml
			xml_new_entry_sub (doc, nodeSub);

  			posNodoSub++;

		}		
		
		//cont_nodos = 0;//***************************************************************Comentado por mi rsb
	}

	xmlNewChild (root, NULL, (const xmlChar*)"nombreEsquema", (const xmlChar*)(directorio + nombreEsquema).c_str());

}

//Para componer cada subautomata
void xml_new_entry_sub (xmlDocPtr doc, xmlNodePtr ptrSub) {

	xmlNodePtr node, nodeTransicion, nodeTransiciones, nodeBox;
	string cadena;
	double x1, x2, y1, y2;
	list<tNodo>::iterator posNodos;	
	list<GnomeCanvasItem *>::iterator posAdy;
	list<tTransicion>::iterator posTransiciones;
	GnomeCanvasItem *box;

	if (!ListaElementos.empty())
	{ 
		posNodos = ListaElementos.begin();
	
		while( posNodos != ListaElementos.end())
		{	
			/* nodo raiz */
			node = xmlNewChild (ptrSub, NULL, (const xmlChar*)"Estado", NULL);

			if (posNodos->estado_inicial != NULL)
				xmlSetProp (node, (const xmlChar*)"estado_inicial", (const xmlChar*)"true");
			else
				xmlSetProp (node, (const xmlChar*)"estado_inicial", (const xmlChar*)"false");

			cadena = int2string(++cont_nodos);

			xmlNewChild (node, NULL, (const xmlChar*)"id", (const xmlChar*)cadena.c_str());
			
			/* Guardamos las coordenadas */
			get_bounds (posNodos->item, &x1,&y1,&x2,&y2);
		
			cadena = int2string(x1);
			
			xmlNewChild (node, NULL, (const xmlChar*)"origenX", (const xmlChar*)cadena.c_str());

			cadena = int2string(y1);

			xmlNewChild (node, NULL, (const xmlChar*)"origenY", (const xmlChar*)cadena.c_str());

			cadena = int2string(x2);

			xmlNewChild (node, NULL, (const xmlChar*)"destinoX", (const xmlChar*)cadena.c_str());

			cadena = int2string(y2); 

			xmlNewChild (node, NULL, (const xmlChar*)"destinoY", (const xmlChar*)cadena.c_str());
	
			/* Guardamos el nombre */
			xmlNewChild (node, NULL, (const xmlChar*)"nombre", (const xmlChar*)(char *)posNodos->nombre.c_str());

			/*Guardamos el ID del hijo*/
			//if (posNodos->idHijo != 0 )
				cadena = int2string(posNodos->idHijo);
				xmlNewChild (node, NULL, (const xmlChar*)"hijo", (const xmlChar*)(char *)cadena.c_str());
		
			/* Guardamos el codigo del estado */
			const xmlChar * code = xmlEncodeSpecialChars (doc,  (const xmlChar*)posNodos->codigo.c_str());	// Parseamos el código por 																caracteres especiales.		
			
			xmlNewChild (node, NULL, (const xmlChar*)"codigo", code);

			/* Guardamos las transiciones */
			nodeTransiciones = xmlNewChild (node, NULL, (const xmlChar*)"transiciones", NULL);

			posAdy = posNodos->listaAdyacentes.begin();

			while( posAdy != posNodos->listaAdyacentes.end())
			{
				posTransiciones = ListaTransiciones.begin();

				while( posTransiciones != ListaTransiciones.end())
				{
					if ((*posAdy == posTransiciones->item) & (posNodos->item == posTransiciones->origen))
					{
						nodeTransicion = xmlNewChild (nodeTransiciones, NULL, (const xmlChar*)"transicion", NULL);
						
						/* Guardamos puntos Box */
						box = get_box (posTransiciones->item);

						get_bounds (box, &x1,&y1,&x2,&y2);
					
						nodeBox = xmlNewChild (nodeTransicion, NULL, (const xmlChar*)"box", NULL);
						xmlSetProp (nodeBox, (const xmlChar*)"x1", (const xmlChar*)int2string(x1).c_str());
						xmlSetProp (nodeBox, (const xmlChar*)"y1", (const xmlChar*)int2string(y1).c_str());
						xmlSetProp (nodeBox, (const xmlChar*)"x2", (const xmlChar*)int2string(x2).c_str());
						xmlSetProp (nodeBox, (const xmlChar*)"y2", (const xmlChar*)int2string(y2).c_str());
						
						xmlNewChild (nodeTransicion , NULL, (const xmlChar*)"nombre", (const xmlChar*)posTransiciones->nombre.c_str());

						cadena = int2string(posicion(posTransiciones->destino));

						xmlNewChild (nodeTransicion , NULL, (const xmlChar*)"destino", (const xmlChar*)cadena.c_str());

						if (posTransiciones->codigo != ""){
							code = xmlEncodeSpecialChars (doc,  (const xmlChar*)posTransiciones->codigo.c_str());	// Parseamos el código por 																caracteres especiales.
							xmlNewChild (nodeTransicion , NULL, (const xmlChar*)"codigo", code);
						}else if (posTransiciones->tiempo != -1){
							cadena = int2string(posTransiciones->tiempo);
							xmlNewChild (nodeTransicion , NULL, (const xmlChar*)"tiempo", (const xmlChar*)cadena.c_str());
						}
					}	
					posTransiciones++;
				}	
				posAdy++;
			}	
  			posNodos++;

		}		
		cont_nodos = 0;
	}

	/* Guardamos datos del Subautomata */

	cadena = int2string(tiempoIteracion);

	xmlNewChild (ptrSub, NULL, (const xmlChar*)"tiempoIteracion", (const xmlChar*)cadena.c_str());

	node = xmlNewChild (ptrSub, NULL, (const xmlChar*)"Librerias", NULL);

	if (imp.laser)
		xmlNewChild (node, NULL, (const xmlChar*)"lib", (const xmlChar*)"laser");
	if (imp.motor)
		xmlNewChild (node, NULL, (const xmlChar*)"lib", (const xmlChar*)"motor");
	if (imp.radar)
		xmlNewChild (node, NULL, (const xmlChar*)"lib", (const xmlChar*)"radar");
	if (imp.encoders)
		xmlNewChild (node, NULL, (const xmlChar*)"lib", (const xmlChar*)"encoders");
	if (imp.lat_lon)
		xmlNewChild (node, NULL, (const xmlChar*)"lib", (const xmlChar*)"lat_lon");
	if (imp.camara)
		xmlNewChild (node, NULL, (const xmlChar*)"lib", (const xmlChar*)"camara");
	if (imp.ptencoders)
		xmlNewChild (node, NULL, (const xmlChar*)"lib", (const xmlChar*)"ptencoders");

	xmlNewChild (ptrSub, NULL, (const xmlChar*)"variables_aux", (const xmlChar*)variables.c_str());
		
	xmlNewChild (ptrSub, NULL, (const xmlChar*)"funciones_aux", (const xmlChar*)funciones.c_str());

}


void xml_get_entry (xmlNodePtr child) {
	
	tNodo figura;
	
	GnomeCanvasItem *item, *item2;
	GnomeCanvasGroup *group, *groupT;

	double x1,x2,y1,y2;
	//const xmlChar* str;
	//const char* c_str;

	xmlNodePtr node;  //nodo pincipal
	xmlNodePtr nodeT; //nodo transiciones

	transicion_aux t;

	list<transicion_aux>::iterator pos;

	node = child->xmlChildrenNode;

	int id= atoi((const char*)xmlNodeGetContent (node));

	node = node->next;

//	str = xmlNodeGetContent (node);
//	c_str = (const char*)str;
//	
//	cout << "FIGURA: " << c_str << endl;
//	figura.figura = c_str;
//	node = node->next;

	/* Cargamos Puntos Nodo */
	x1= atoi((const char*)xmlNodeGetContent (node));
	node = node->next;
	y1 = atoi((const char*)xmlNodeGetContent (node));
	node = node->next;
	x2 = atoi((const char*)xmlNodeGetContent (node));
	node = node->next;
	y2 = atoi((const char*)xmlNodeGetContent (node));
	node = node->next;

	/* Cargamos Nombre y Código */
	figura.nombre = (const char*)xmlNodeGetContent (node);	
	node = node->next;
	figura.idHijo = atoi((const char*)xmlNodeGetContent (node));
	node = node->next;
	figura.codigo = (const char*)xmlNodeGetContent (node);
	node = node->next;
	
	group = GNOME_CANVAS_GROUP (gnome_canvas_item_new (root,  
                                gnome_canvas_group_get_type (),
                                "x", 0,
                                "y", 0,
                                NULL));
	if (figura.idHijo == 0) {

		item = gnome_canvas_item_new(group,
                        gnome_canvas_ellipse_get_type(),
                        "x1", x1,
                        "y1", y1,
                        "x2", x2,
                        "y2", y2,
                        "fill_color_rgba", 0x00ffffff,
                        "outline_color", "black",
                        "width_units", 1.0,
                        NULL);

	}
	else{

		item = gnome_canvas_item_new(group,
                        gnome_canvas_ellipse_get_type(),
                        "x1", x1,
                        "y1", y1,
                        "x2", x2,
                        "y2", y2,
                        "fill_color_rgba", 0x00ff66ff,
                        "outline_color", "black",
                        "width_units", 1.0,
                        NULL);

	}

	if ( strcmp((const char*)xmlGetProp (child, (const xmlChar*)"estado_inicial"), (const char*)"true") == 0 )
	{
		figura.estado_inicial = pinta_estado_inicial (group, item);
	}
	else 
		figura.estado_inicial = NULL;
	
	

	if (figura.nombre != ""){
		item2 = gnome_canvas_item_new (group,
	             gnome_canvas_text_get_type (),
             			            "text", figura.nombre.c_str(),
             			            "x", (x1 + x2)/2,
             			            "y", (y1 + y2)/2,
             			            "font", "Sans 28",
					    //"size", 32000,
					    //"size_set", (gboolean)TRUE,
					    //"scale",  (gdouble) 3,
					    //"scale_set", (gboolean)TRUE,
         			            "anchor", GTK_ANCHOR_CENTER,
     			                    "fill_color", "black",
    			                     NULL); 

		figura.item_nombre = item2;

	}else{
		figura.item_nombre = NULL;
	}	
	
	// guardamos el primer item pintado, para asignarlo despues al destino de algunas transiciones.
	if (item_guardado == NULL) 
		item_guardado = item;


 	/*Añadimos control (señal) al item creado*/
	g_signal_connect (group, "event",
			    G_CALLBACK (item_event),
			    NULL);

	//figura.id = (int)item;
	figura.item = item;

	if (node != NULL) {
	    			
		nodeT = node->xmlChildrenNode;
		
		while (nodeT != NULL)
		{
			t.origen = item; 
			t.destino = NULL; 
			cout << "ORIGEN: " << (int)item << endl;

			/*Recuperamos Puntos Box*/
			x1= atoi((const char*)xmlGetProp (nodeT->xmlChildrenNode, (const xmlChar*)"x1"));

			y1 = atoi((const char*)xmlGetProp (nodeT->xmlChildrenNode, (const xmlChar*)"y1"));

			x2 = atoi((const char*)xmlGetProp (nodeT->xmlChildrenNode, (const xmlChar*)"x2"));

			y2 = atoi((const char*)xmlGetProp (nodeT->xmlChildrenNode, (const xmlChar*)"y2"));


			groupT = GNOME_CANVAS_GROUP (gnome_canvas_item_new (root,  
                                        gnome_canvas_group_get_type (),
                                        "x", 0,
                                        "y", 0,
                                        NULL));

			create_drag_box (groupT, (char *)"box", (x1+x2)/2, (y1+y2)/2,  G_CALLBACK (highlight_box));
			t.item = GNOME_CANVAS_ITEM(groupT);

			t.nombre = (const char*)xmlNodeGetContent (nodeT->xmlChildrenNode->next);

			t.destino_id = atoi((const char*)xmlNodeGetContent (nodeT->xmlChildrenNode->next->next));
			cout << "Destino ID: " << t.destino_id << endl;

			if (nodeT->xmlChildrenNode->next->next->next != NULL){
				if (strcmp((const char*)nodeT->xmlChildrenNode->next->next->next->name,"tiempo") == 0){
					t.tiempo = atoi((const char*)xmlNodeGetContent (nodeT->xmlChildrenNode->next->next->next));
					t.codigo = (const char*)"";
				}
				else if(strcmp((const char*)nodeT->xmlChildrenNode->next->next->next->name,"codigo") == 0) {
					t.tiempo = -1;
					t.codigo = (const char*)xmlNodeGetContent (nodeT->xmlChildrenNode->next->next->next);
				}
			}else{
				t.tiempo = -1;
				t.codigo = "";
			}
			t.origen_xml = id;

			ListaTransicionesTemporales.push_back(t);

			/* añadimos transicion adyacente */
			//figura.listaAdyacentes.push_back(GNOME_CANVAS_ITEM(groupT));

			nodeT = nodeT->next;

		}
	}
	
	pos = ListaTransicionesTemporales.begin();
	
	while (pos != ListaTransicionesTemporales.end())
	{
		if (id == pos->destino_id) 
			pos->destino = item;
	
		pos++;
	}	

	ListaElementos.push_back(figura);

}
