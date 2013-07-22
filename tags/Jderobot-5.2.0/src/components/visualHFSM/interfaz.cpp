#include "interfaz.h"

//#include <gtkmm.h>
//#include <gtkmm/main.h>

#define FICH_GLADE "insercion.glade"

GladeXML *prog = NULL;



#define CANVAS_SIZE 1500

using namespace std;

//------------AÑADIDO ESTRUCTURA SUBAUTOMATA



tSubAut reg_sub;
list <tSubAut> ListaSubAutomatas;
int idSubGlobal = 1; //Id máximo de subautomata en el esquema abierto, se usa para ir asignando id's correlativos en la creacion de nuevos niveles

int subautomata_mostrado = 1; //Id del subautomata que se esta mostrando en el canvas

GtkWidget* menu_estado;
GtkWidget* menu_transicion;
GtkWidget* menu_pegar;

GtkWidget* nombrar_estado;
GtkWidget* editar_estado;
GtkWidget* marcar_inicial_estado;
GtkWidget* copiar_estado;
GtkWidget* eliminar_estado;
GtkWidget* nombrar_transicion;
GtkWidget* editar_transicion;
GtkWidget* eliminar_transicion;
GtkWidget* pegar_estado;

GnomeCanvasItem *item_menu_estado;
GnomeCanvasItem *item_group_menu_estado;
GnomeCanvasItem *item_menu_transicion;
GnomeCanvasItem *item_menu_pegar;

GtkWidget *tree_view;

enum
{
  COLUMN = 0,
  NUM_COLS
} ;

GtkTreePath * arbolPath;

bool estado_clipboard = FALSE;
string estado_clipboard_codigo;
int estado_clipboard_idHijo;

bool estado_nombrado = FALSE;

//------------FIN AÑADIDO ESTRUCTURA SUBAUTOMATA


typedef struct reg {
	GnomeCanvasGroup *root;
	char *figura;
} reg; 

typedef struct aux {
	
	GnomeCanvasItem * origen;
	GnomeCanvasItem * destino;
	string codigo;
	int tiempo;
} aux;
 

typedef enum TYPE_WIDGET {
	TEXT_VIEW,
	ENTRY,
	SOURCE_VIEW
}Estado;

const string ayuda[5] = {"Para mover un estado (item), seleccionar el boton mover,\ny posteriormente arrastrar el estado. Para copiar, pulse el boton copiar y selecciona un estado.\nSe copiara automaticamenteen una posicion cercana al estado pulasado.\nPara eliminar un estado, seleccionar el boton y posteriormente pulsar con el boton izquierdo sobre el item.","Boton Estado: pinta un estado en el canvas. Selecionar y pinchar en el lugar donde se quiera posicionar\nBoton Transicion: pinta una transicion entre dos estados. Selecionar boton y selecionar los estados que conecta.","Guarda/Carga el automata en/desde un fichero xml.\nEstado inicial: marca que estado es el inicial.","Estado inicial: marca que estado es el inicial.\nNombrar: nombra el estado.\nEditar: inserta codigo en el estado/transicion.", "Generar C: genera fichero .c\nCompilar: genera ejecutable"};

string codigo = "";

string variables = "";
string funciones = "";

int tiempo = -1;

int id = 0;
 
list <tNodo> ListaElementos;

list <tTransicion> ListaTransiciones;

list <transicion_aux> ListaTransicionesTemporales;	// Lista de transiciones temporales. Usadas para pintar las transiciones despues de cargar un fichero.

list <GtkWidget *> ListaBotones;

//string fichero = "";		

string directorio = "";	// directorio donde se guarda el esquema

string nombreEsquema = "";	//nombre del esquema guardado

int tiempoIteracion = 100;

importar imp;

//tNodo *TListaFiguras;

tNodo registro;

static GtkWidget *window;	//ventana principal

GtkWidget *canvas;	// canvas principal

GnomeCanvasGroup *root;

GnomeCanvasGroup*  	new_group;

//Movemos estos dos botones aqui para poder acceder a su estado desde todo el programa
GtkWidget *botonLinea;
GtkWidget *botonEstado;

string botonPulsado = "Nada";	//Boton pulsado. Para diferenciar operaciones.

GnomeCanvasPoints* points; /* 2 puntos */

GnomeCanvasPoints* points2; /* 2 puntos */

GnomeCanvasItem *item_guardado;	// item guardado para operaciones

GnomeCanvasItem *item_transicion_saved;	// item guardado para operaciones

int origenX, origenY;	// Puntos (x,y) del canvas

int pinta_linea;

int color;	// Guarda Color de la figura editada

const char * compile_file;

char* probar;

double zoom = 1.0;

extern int cont_transiciones;


/* Prototipo de la funciónes que capturan las señales de los item */
gint
item_event (GnomeCanvasItem *item, GdkEvent *event, gpointer data);


void
cuadro_informacion (char *szMessage)
{
	GtkWidget *label;
	GtkWidget *dialog;

	dialog = gtk_dialog_new_with_buttons ("Information",
                                                 GTK_WINDOW (window),
                                                 GTK_DIALOG_MODAL,
                                                 GTK_STOCK_OK,
                                                 GTK_RESPONSE_ACCEPT,                                            
                                                 NULL);
	gtk_window_set_title (GTK_WINDOW (dialog),"Information");
	gtk_container_border_width(GTK_CONTAINER (dialog),5);

	//Creamos el mensaje
	label = gtk_label_new(szMessage);
	gtk_misc_set_padding(GTK_MISC(label), 10, 10);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)-> vbox), label, TRUE, TRUE, 0);
	gtk_widget_show (label);
		
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
			gtk_widget_destroy (dialog);
			
	cout << "cuadro_informacion" << endl;
}



void
compilar (GtkWindow *window, gpointer data){

	pid_t pid;

	if (nombreEsquema ==""){

		cuadro_informacion ((char*)"You must generate the code before you could compile it.");

	}else {
 		if ((pid = fork()) == -1) {
   			printf("Error al crear proceso hijo\n");
    			exit(1);
 		}
		else {
			if (pid == 0){
				printf("Compiling..\n");
				execl("/usr/bin/cmake", "cmake", (const char*)strcat ((char*)directorio.c_str(), "src/build"));
				execl("/usr/bin/make", "make", (const char*)strcat ((char*)directorio.c_str(), "src/build"));
//				execl("/usr/bin/make", "make", "-C", (const char*)strcat ((char*)directorio.c_str(), "src/"), "-f", "Makefile-mycomponent", NULL);		
			}
		}
	}
	
	cout << "compilar" << endl;
}

void
generar_c (GtkWindow *windows, gpointer data){

	pid_t pid;

	const char *filename = "";


	if (nombreEsquema ==""){

		cuadro_informacion ((char*)"You must save the project before you could generate the code.");

	}

	else {
		filename = (directorio + nombreEsquema + ".c").c_str();

  		
 		if ((pid = fork()) == -1) {
   			printf("Error al crear proceso hijo\n");
    			exit(1);
 		}
	
		if (pid == 0){
			printf("Generando Codigo.. %s\n", filename);
			execl("./generate", "generate",(directorio + nombreEsquema + ".xml").c_str(), (directorio + nombreEsquema + ".c").c_str(), NULL);

		}
	
	}
	
	cout << "generar_c" << endl;
}

void
marcar_estado_inicial (GnomeCanvasItem *item)
{
	list<tNodo>::iterator pos;
	GnomeCanvasItem *estado_inicial;
	tNodo n;

	pos = ListaElementos.begin();

	if (pos->estado_inicial != NULL){
		gtk_object_destroy(GTK_OBJECT(pos->estado_inicial)); 
		pos->estado_inicial = NULL;
	}

	while (pos->item != item){
		pos++;
	}
	

	estado_inicial = pinta_estado_inicial (GNOME_CANVAS_GROUP(item->parent), item);
	pos->estado_inicial=estado_inicial;

	n = *pos;

	ListaElementos.erase(pos);
	ListaElementos.push_front (n);
	
	cout << "marcar_estado_inicial" << endl;
}

void cargar_transiciones ()
{
	GList * list_items;
	GnomeCanvasGroup *group, *group_parent ;
	GnomeCanvasItem *box, *item_nombre;
	double x1, x2, y1, y2;
	list<transicion_aux>::iterator pos;
	list<tTransicion>::iterator pos2;
	list<tNodo>::iterator posNodos;

	pos = ListaTransicionesTemporales.begin();


	while (pos != ListaTransicionesTemporales.end())
	{
		if (pos->origen == pos->destino){
			gtk_object_destroy (GTK_OBJECT (pos->item));
			group = GNOME_CANVAS_GROUP (pinta_autotransicion (pos->origen));
			goto NOMBRE;
		}

		group_parent = GNOME_CANVAS_GROUP(pos->item);

		list_items = group_parent->item_list;

		box = (GnomeCanvasItem *) g_list_nth_data (list_items, 0);  

		get_bounds (box, &x1, &y1, &x2, &y2);
		
		group = repintar_transicion (GNOME_CANVAS_GROUP (root), pos->origen, pos->destino, (x1+x2)/2, (y1+y2)/2);		

		gnome_canvas_item_reparent (box, group);

		NOMBRE:
		/* Nombre Transicion */
		if (pos->nombre.empty())
			item_nombre = NULL;
			
		else
			item_nombre = gnome_canvas_item_new (root,
		             gnome_canvas_text_get_type (),
	             			            "text", pos->nombre.c_str(),
	             			            "x", (x2+x1)/2,
	             			            "y", (y2+y1)/2 + 5,
	             			            "font", "Sans 28",
					    	    		"anchor", GTK_ANCHOR_N,
	     			                    "fill_color", "black",
	    			                     NULL); 
		

		add_transicion_lista (GNOME_CANVAS_ITEM(group),pos->origen, pos->destino, item_nombre, pos->codigo, pos->tiempo);

		posNodos = ListaElementos.begin();

		while( (posNodos != ListaElementos.end()) && (posNodos->item != pos->origen) )
			posNodos++;

		posNodos->listaAdyacentes.push_back(GNOME_CANVAS_ITEM(group));

		if (pos->origen != pos->destino){
			posNodos = ListaElementos.begin();

			while( (posNodos != ListaElementos.end()) && (posNodos->item != pos->destino) )
				posNodos++;

			posNodos->listaAdyacentes.push_back(GNOME_CANVAS_ITEM(group));
		}

		pos++;
	}	
    
    cout << "cargar_transiciones" << endl;
}



bool ventana_timer (GtkWindow *window, GdkEventMotion *event, gpointer data){

	GtkWidget *ventana;

	ventana = comprobar_ventana_abierta (NULL, TIMER);

	if (ventana != NULL) 
		gtk_window_present (GTK_WINDOW(ventana));
	else{
		ventana = tiempo_iteracion();
		add_ventana (ventana, NULL, TIMER);
	}
	
	cout << "ventana_timer" << endl;
	
	return TRUE;

}


bool ventana_importar (GtkWindow *window, GdkEventMotion *event, gpointer data){

	GtkWidget *ventana;

	ventana = comprobar_ventana_abierta (NULL, LIBRERIAS);

	if (ventana != NULL) 
		gtk_window_present (GTK_WINDOW(ventana));
	else{
		ventana = importar_librerias();
		add_ventana (ventana, NULL, LIBRERIAS);
	}
	
	cout << "ventana_importar" << endl;
	
	return TRUE;

}



bool codigo_esquema (GtkWindow *window, GdkEventMotion *event, gpointer data)
{
	GtkWidget *ventana;

	ventana = comprobar_ventana_abierta (NULL, CODIGO);

	if (ventana != NULL) 
		gtk_window_present (GTK_WINDOW(ventana));
	else{
		ventana = new_code_windows ();
		add_ventana (ventana, NULL, CODIGO);
	}
	
	cout << "codigo_esquema" << endl;
	
	return TRUE;

}



void new_windows (GtkWindow *window, GdkEventMotion *event, gpointer data)
{
	
	static GtkWidget *window2;
	GtkWidget *label;



	/*Creamos Ventana*/
	window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	/*Situamos la ventana en el medio*/
	gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);

	/*Ponemos borde a la ventana*/
	gtk_container_set_border_width (GTK_CONTAINER (window2), 50);

	/*Ponemos el titulo*/
	gtk_window_set_title(GTK_WINDOW(window2), "Ayuda");

	/* An option menu to change the position of the value */
	label = gtk_label_new ((const char*)data);

	gtk_container_add (GTK_CONTAINER(window2), label);

	gtk_widget_show_all(window2);
	
	cout << "new_windows" << endl;

}



/* Crea una Caja de Botones con los parámetros específicos */
GtkWidget *create_bbox( gint  horizontal,
                        char *title,
                        gint  espacio,
                        gint  child_w,
                        gint  child_h,
                        gint  layout,
			gpointer data,
			gpointer text)
{
  GtkWidget *frame;
  GtkWidget *bbox;
  GtkWidget *button;

  frame = gtk_frame_new (title);
	gtk_container_set_border_width (GTK_CONTAINER (frame), 0);

  if (horizontal)
    bbox = gtk_hbutton_box_new ();
  else
    bbox = gtk_vbutton_box_new ();

  gtk_container_set_border_width (GTK_CONTAINER (bbox), 0);
  gtk_container_add (GTK_CONTAINER (frame), bbox);

  /* Establece la apariencia de la Caja de Botones */

  gtk_button_box_set_layout (GTK_BUTTON_BOX (bbox),  GtkButtonBoxStyle(layout));

  gtk_box_set_spacing (GTK_BOX (bbox), espacio);

	list <GtkWidget *> ::iterator pos;

	pos = ListaBotones.begin();
	while (pos!=ListaBotones.end()){
  		gtk_container_add (GTK_CONTAINER (bbox), *pos);
		pos++;
	}

	button = gtk_button_new_from_stock (GTK_STOCK_HELP);
	gtk_signal_connect(GTK_OBJECT(button), "button_release_event", GTK_SIGNAL_FUNC(new_windows), text); 
	
	cout << "create_bbox" << endl;

  return frame;
}


void frame_callback(GtkWindow *window, 
      GdkEvent *event, gpointer data)
{
	int x, y;
	char buf[10];
	x = event->configure.x;
	y = event->configure.y;
	sprintf(buf, "%d, %d", x, y);
	printf("Mover %d, %d\n", x, y);
	gtk_window_set_title(window, buf);
	
	cout << "frame_callback" << endl;
}

void guardar(GtkWindow *windows, gpointer data)
{
	xmlDocPtr doc;	

	GtkWidget *dialog;
  	GtkFileFilter *filter;

	cout << directorio << endl;
	cout << (int)data << endl;
	
	cout << "guardar" << endl;

	if ( (((int)data ==0) && (directorio == "")) || ((int)data == 1) ){
  		dialog = gtk_file_chooser_dialog_new ("Save File", NULL, 
                              GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                              GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);

		gtk_window_set_title(GTK_WINDOW(dialog), "Save File");
		
  		gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER(dialog), TRUE);

  		filter = gtk_file_filter_new();
  		gtk_file_filter_set_name (filter, "xml");
  		gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);
  		gtk_file_filter_add_pattern (filter, "*.[x][m][l]");

  		filter = gtk_file_filter_new();
  		gtk_file_filter_set_name (filter, "All files");
  		gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);
  		gtk_file_filter_add_pattern (filter, "*");

  	}
	if (((int)data ==0) & (directorio != "")){

		doc = xml_new_doc ("VisualHFSM");

		//**************************************************Antes de llamar a guardar ponemos en las variables del nodo del nivel en el que nos 
		//**************************************************encontremos las variables globales que estuviesemos usando en ese momento.
		list<tSubAut>::iterator posNodoSub;	
		posNodoSub = ListaSubAutomatas.begin();
		
		while (posNodoSub->idSub != subautomata_mostrado and posNodoSub != ListaSubAutomatas.end())
			posNodoSub++;

		posNodoSub->ListaElementosSub = ListaElementos;		
		posNodoSub->ListaTransicionesSub = ListaTransiciones;
		posNodoSub->tiempoIteracionSub = tiempoIteracion;
		posNodoSub->variablesSub = variables;
		posNodoSub->funcionesSub = funciones;
		posNodoSub->impSub = imp;
		//**************************************************************************************************************************************

		xml_new_entry (doc);

		xmlSaveFile      ((directorio + nombreEsquema + ".xml").c_str(), doc);
	}
  	else if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
    		char *filename;
    
    		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

    		printf("File open: %s\n",filename);

		compile_file = filename;

		// Nombre y directorio fichero

		string fich = filename;
		int i;
		size_t pos;
  		for (i=0; i < (signed)fich.length(); i++)
  		{
   			if (fich.at(i) == '/')
				pos = i;
 		}	
		string fichero = fich.substr (++pos);

		directorio = fich.substr (0,pos);

		string nombre;

		for (i=0; i < (signed)fichero.length(); i++)
  		{
   			if (fichero.at(i) == '.')
				pos = i;
 		}	
		
		nombre = fichero.substr (0,pos);
		
		nombreEsquema = nombre;

		/*Ponemos el titulo*/
		gtk_window_set_title(GTK_WINDOW(window), nombre.c_str());

		doc = xml_new_doc ("VisualHFSM");

		//**************************************************Antes de llamar a guardar ponemos en las variables del nodo del nivel en el que nos 
		//**************************************************encontremos las variables globales que estuviesemos usando en ese momento.
		list<tSubAut>::iterator posNodoSub;	
		posNodoSub = ListaSubAutomatas.begin();
		
		while (posNodoSub->idSub != subautomata_mostrado and posNodoSub != ListaSubAutomatas.end())
			posNodoSub++;

		posNodoSub->ListaElementosSub = ListaElementos;		
		posNodoSub->ListaTransicionesSub = ListaTransiciones;
		posNodoSub->tiempoIteracionSub = tiempoIteracion;
		posNodoSub->variablesSub = variables;
		posNodoSub->funcionesSub = funciones;
		posNodoSub->impSub = imp;
		//**************************************************************************************************************************************

		xml_new_entry (doc);

		xmlSaveFile      (filename, doc);

    		g_free (filename);

 		gtk_widget_destroy (dialog);

	}
	else 
		gtk_widget_destroy (dialog);

}

void remarcar_boton (GtkFileChooser *window, gpointer data)
	{
		gtk_widget_grab_focus ((GtkWidget *)data);
		
		cout << "remarcar_boton" << endl;
	}

void  borrar_canvas ()
{

    cout << "borrar_canvas" << endl;

	list<tTransicion>::iterator posTransiciones;

	posTransiciones = ListaTransiciones.begin();
		
	while (posTransiciones != ListaTransiciones.end())
	{
		gtk_object_destroy (GTK_OBJECT ((posTransiciones->item_nombre)));
		gtk_object_destroy (GTK_OBJECT ((posTransiciones->item)));
		posTransiciones++;			
	}	

	ListaTransiciones.clear();

	list<tNodo>::iterator posNodos;

	posNodos = ListaElementos.begin();
		
	while (posNodos != ListaElementos.end())
	{
		gtk_object_destroy (GTK_OBJECT ((posNodos->item->parent)));
		posNodos++;			
	}	

	ListaElementos.clear();	

	gnome_canvas_update_now (GNOME_CANVAS(canvas));

}


void cargar(GtkWindow *windows, gpointer data)
{
    cout << "cargar" << endl;
	xmlDocPtr doc; 
	
	xmlNodePtr root;

	xmlNodePtr nodeSub;

	xmlNodePtr node;

	list<tSubAut>::iterator posSub;//Añadido para luego poder cargar en las variables globales la primera posicion de la lista de subAut.

	GtkFileFilter *filter;

	GtkWidget *dialog;

	dialog = gtk_file_chooser_dialog_new ("Open File",
				      NULL,
				      GTK_FILE_CHOOSER_ACTION_OPEN,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				      NULL);

	filter = gtk_file_filter_new();
	gtk_file_filter_set_name (filter, "xml");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);
	gtk_file_filter_add_pattern (filter, "*.[x][m][l]");

	filter = gtk_file_filter_new();
	gtk_file_filter_set_name (filter, "All files");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);
	gtk_file_filter_add_pattern (filter, "*");

	gtk_window_set_title(GTK_WINDOW(dialog), "Open File");

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  	{
		/* Dirección del fichero */
   		char *filename;

    		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

    		printf("Fichero abierto: %s\n",filename);

		/* Abrir XML */

		doc = xmlParseFile (filename);

		if (!doc) {
	  		g_print("Error al cargar documento XML\n");
	  	}
		else{
			item_guardado = NULL;
			root = xmlDocGetRootElement (doc);
	 		nodeSub = root->xmlChildrenNode;
			//node = nodeSub->xmlChildrenNode;
			
			
			/* Borrar Canvas */
			
			if (!ListaSubAutomatas.empty()){

				posSub = ListaSubAutomatas.begin();
		
				while (posSub != ListaSubAutomatas.end()){

					ListaElementos = posSub->ListaElementosSub;
					ListaTransiciones = posSub->ListaTransicionesSub;

					borrar_canvas();
					printf("Borrado nivel \n");

					posSub++;

				}

			}
			else
				borrar_canvas();

			ListaSubAutomatas.clear();
			
			/* Recuperamos los datos */

			while (nodeSub != NULL){
				g_print("Encontrado Sub: %s\n", nodeSub->name);//*****************************************************************
				
				node = nodeSub->xmlChildrenNode;				

				if (strcmp((const char *)nodeSub->name, "nombreEsquema")==0){

					string fich = (const char*)xmlNodeGetContent (nodeSub);
					if (fich != ""){
						size_t pos;

						pos = fich.rfind('/');
						printf("fich: %s\n",fich.c_str());//**********************************************************************
						nombreEsquema = fich.substr (++pos);

						printf("nombreEsquema: %s\n",nombreEsquema.c_str());//****************************************************


						directorio = filename;
						pos = directorio.rfind('/');
						directorio = directorio.substr (0,++pos);

						/*Ponemos el titulo*/
						gtk_window_set_title(GTK_WINDOW(window), nombreEsquema.c_str());
					}else{
						/*Ponemos el titulo*/
						gtk_window_set_title(GTK_WINDOW(window),"Default");
					}	

				}else if (strcmp((const char *)nodeSub->name, "SubAutomata")==0){

					g_print("Entra por subautomata\n");//**************************************************************
					
					while (node != NULL) {
						
			   			g_print("Encontrado nodo %s\n", node->name);

						if (strcmp((const char *)node->name, "idSub")==0){
							reg_sub.idSub = atoi((const char*)xmlNodeGetContent (node));
							if (reg_sub.idSub > idSubGlobal){
								idSubGlobal = reg_sub.idSub;
							}
						}
						else if (strcmp((const char *)node->name, "idPadre")==0)
							reg_sub.idPadre = atoi((const char*)xmlNodeGetContent (node));
						else if (strcmp((const char *)node->name, "tiempoIteracion")==0)
							reg_sub.tiempoIteracionSub = atoi((const char*)xmlNodeGetContent (node));
						else if (strcmp((const char *)node->name, "variables_aux")==0)
							reg_sub.variablesSub = (const char*)xmlNodeGetContent (node);
						else if (strcmp((const char *)node->name, "funciones_aux")==0)
							reg_sub.funcionesSub = (const char*)xmlNodeGetContent (node);
						else if (strcmp((const char *)node->name, "Librerias")==0){
							xmlNodePtr child = node->xmlChildrenNode;	
							string lib;				
							while (child != NULL){
								lib = (const char*)xmlNodeGetContent (child);	
								if (lib == "laser")
									reg_sub.impSub.laser = true;
								if (lib == "motor")
									reg_sub.impSub.motor = true;
								if (lib == "radar")
									reg_sub.impSub.radar = true;
								if (lib == "encoders")
									reg_sub.impSub.encoders = true;
								if (lib == "lat_lon")
									reg_sub.impSub.lat_lon = true;
								if (lib == "camara")
									reg_sub.impSub.camara = true;
								if (lib == "ptencoders")
									reg_sub.impSub.ptencoders = true;
						
								child = child->next;
							}
						}else//Aqui se extraen los datos de cada estado concreto
							xml_get_entry (node);
										
						node = node->next;

			  		}//While node


					/* Pintado de Transiciones */

					list<transicion_aux>::iterator pos, pos2;

					pos = ListaTransicionesTemporales.begin();
	
					while (pos != ListaTransicionesTemporales.end())
					{
						if (pos->destino == NULL) 
						{
							pos2 = ListaTransicionesTemporales.begin();
							while (pos2 != ListaTransicionesTemporales.end()){
								if (pos->destino_id == pos2->origen_xml)
									pos->destino = pos2->origen;
								pos2++;
							}
						}	
						pos++;
					}

					gnome_canvas_update_now (GNOME_CANVAS(canvas));

					cout << "item_guardado   " << item_guardado << endl;

					item_guardado = NULL;

					cargar_transiciones ();

					ListaTransicionesTemporales.clear();

					reg_sub.ListaElementosSub = ListaElementos;
					reg_sub.ListaTransicionesSub = ListaTransiciones;
					
					//Borramos el canvas a cada iteracion para que no se pinte todo a la vez, la información para poder
					//restaurar cada nivel en el canvas ya la tenemos guardada en cada nodo de la lista de subAutómatas
					//borrar_canvas();

					ListaElementos.clear();
					ListaTransiciones.clear();
				
					ListaSubAutomatas.push_back(reg_sub);

					//Borramos el subautomata que acabamos de pintar para al final de la carga pintar solo el primer nivel.
					ocultar_subautomata(reg_sub.idSub);


				}//else de SubAutomata

				nodeSub = nodeSub->next;

			}//While nodeSub

		}

		g_free (filename);

  	}	

	gtk_widget_destroy (dialog);
	
	//Pintamos el canvas con los elementos del subautomata 1, la raiz.
	printf("Antes de pintar sub\n");
	mostrar_subautomata(1);
	printf("Despues de pintar sub\n");

	//Cargamos en las variables "globales" las correspondientes al primer nodo de la lista de subautómatas.
	posSub = ListaSubAutomatas.begin();
	
	imp = posSub->impSub;
	funciones = posSub->funcionesSub;
	variables = posSub->variablesSub;
	tiempoIteracion = posSub->tiempoIteracionSub;

	ListaElementos = posSub->ListaElementosSub;
	ListaTransiciones = posSub->ListaTransicionesSub;

	//Dejamos preparada la variable idSubGlobal con un numero correlativo mayor al máximo cargado.
	idSubGlobal++;

	//Recargamos el tree view con los nuevos datos cargados
	actualizar_tree_view();	

}


void mostrar_subautomata(int pidSub)
{
    cout << "mostrar_subautomata" << endl;

	list<tSubAut>::iterator posSub;

	list <tNodo>::iterator Elem;
	list <tTransicion>::iterator Trans;


	posSub = ListaSubAutomatas.begin();
	
	while(posSub->idSub != pidSub){
		posSub++;
	}

	Elem = posSub->ListaElementosSub.begin();
	Trans = posSub->ListaTransicionesSub.begin();

	/*group = GNOME_CANVAS_GROUP (gnome_canvas_item_new (root,  
                                gnome_canvas_group_get_type (),
                                "x", 0,
                                "y", 0,
                                NULL));*/
	if (!posSub->ListaElementosSub.empty()){
		while(Elem != posSub->ListaElementosSub.end()){
		
			if (Elem->item != NULL) 
				gnome_canvas_item_show(Elem->item);	

			if (Elem->estado_inicial != NULL)
				gnome_canvas_item_show(Elem->estado_inicial);	

			if (Elem->item_nombre != NULL)
				gnome_canvas_item_show(Elem->item_nombre);	
	
			printf("Pintado nodo en el canvas\n");
		
			Elem++;
		
		}

		while (Trans != posSub->ListaTransicionesSub.end()){
		
			if(Trans->item != NULL)
				gnome_canvas_item_show(Trans->item);
		
			if(Trans->item_nombre != NULL)
				gnome_canvas_item_show(Trans->item_nombre);
	
			Trans++;
		
		}
	}


	printf("Antes de update en el mostrar\n");
	//gnome_canvas_update_now (GNOME_CANVAS(canvas));
	printf("Despues de update en el mostrar\n");
	subautomata_mostrado = pidSub;

}


void ocultar_subautomata(int pidSub)
{
    cout << "ocultar_subautomata" << endl;

	list<tSubAut>::iterator posSub;

	list <tNodo>::iterator Elem;
	list <tTransicion>::iterator Trans;


	posSub = ListaSubAutomatas.begin();
	
	while(posSub->idSub != pidSub){
		posSub++;
	}

	Elem = posSub->ListaElementosSub.begin();
	Trans = posSub->ListaTransicionesSub.begin();

	/*group = GNOME_CANVAS_GROUP (gnome_canvas_item_new (root,  
                                gnome_canvas_group_get_type (),
                                "x", 0,
                                "y", 0,
                                NULL));*/

	while(Elem != posSub->ListaElementosSub.end()){
		
		if (Elem->item != NULL) 
			gnome_canvas_item_hide(Elem->item);	

		if (Elem->estado_inicial != NULL)
			gnome_canvas_item_hide(Elem->estado_inicial);	

		if (Elem->item_nombre != NULL)
			gnome_canvas_item_hide(Elem->item_nombre);	
		
		Elem++;
		
	}

	while (Trans != posSub->ListaTransicionesSub.end()){
		
		if(Trans->item != NULL)
			gnome_canvas_item_hide(Trans->item);
		
		if(Trans->item_nombre != NULL)
			gnome_canvas_item_hide(Trans->item_nombre);
	
		Trans++;
		
	}

	gnome_canvas_update_now (GNOME_CANVAS(canvas));


}


void coordenadas(GtkWindow *window, GdkEvent *event, gpointer data)
{
    cout << "coordenadas" << endl;
	origenX = event->button.x;
	origenY = event->button.y;
	printf("Click Press %d, %d\n", origenX, origenY);

	if (event->button.button == 3){

		gtk_menu_popup(GTK_MENU(menu_pegar),NULL,NULL,NULL,NULL,event->button.button,event->button.time);	

	}
	
}


void pinta_nodo(GtkWindow *window, GdkEventMotion *event, gpointer data)
{
	int x, y;
	
	cout << "pinta_nodo" << endl;
	
	GnomeCanvasItem *item;
	GnomeCanvasGroup *group;

	list<tSubAut>::iterator posSub; //Añadido para poder iterar en la lista de subautomatas y actulizar el treeview con cada nuevo estado

	//GtkWidget *button;

	x = event->x;
	y = event->y;

	points = gnome_canvas_points_new(2); /* 2 puntos */
	points->coords[0]  = origenX;
	points->coords[1]  = origenY;
	points->coords[2]  = x;
	points->coords[3]  = y;

	printf("Click %d, %d\n", x, y);
	//printf("Pintamos Nodo \n");

	if ((strcmp(botonPulsado.c_str(), "Cuadrado")==0) and (event->state & GDK_BUTTON1_MASK)) 
	{

		group = GNOME_CANVAS_GROUP (gnome_canvas_item_new (root,  
                                        gnome_canvas_group_get_type (),
                                        "x", 0,
                                        "y", 0,
                                        NULL));

		item = gnome_canvas_item_new(group,
                        gnome_canvas_ellipse_get_type(),
                        "x1", (double) origenX-20,
                        "y1", (double) origenY-20,
                        "x2", (double) (origenX + 20),
                        "y2", (double) (origenY + 20),
                        "fill_color_rgba", 0x00ffffff,
                        "outline_color", "black",
                        "width_units", 1.0,
                        NULL);
	  

		registro.item = item;
		registro.item_nombre = NULL;
		registro.estado_inicial = NULL;
		registro.nombre = "";
		registro.codigo = "";
		registro.idHijo = 0;//***********Por defecto un estado no tiene hijos
		id ++;
	
		ListaElementos.push_back(registro);

		/*Añadimos control (señal) al item creado*/
		g_signal_connect (group, "event",
			    (GtkSignalFunc) item_event,
			    NULL);

		//Actualizamos datos necesarios para mostrar cambios en el tree view
		posSub = ListaSubAutomatas.begin();

		while(posSub->idSub != subautomata_mostrado)
		{
			posSub++;
		}

		//guardamos el estado de la lista de elementos en el nodo del subautomata para poder recargar bien el tree view
		posSub->ListaElementosSub = ListaElementos;

		actualizar_tree_view();
		
		gtk_tree_view_expand_to_path(GTK_TREE_VIEW(tree_view), arbolPath);		

	}

}

static gint nodo(GtkWindow *window, GdkEventMotion *event, gpointer data)
{
	int x, y;
	
	cout << "nodo" << endl;

	x = event->x;
	y = event->y;

	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(botonLinea),FALSE);

	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(botonEstado))){
		
		botonPulsado="Nada";
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(botonEstado),FALSE);

	}
	else{
		botonPulsado="Cuadrado";
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(botonEstado),TRUE);
	}


	/*if ((int)data == 1)
	{
		botonPulsado="EstadoInicial";
	}
	else{
		printf("Click %d, %d\n", x, y);

		botonPulsado="Cuadrado";
		
	}*/

	return TRUE;
}


static gint motion_notify_event(GtkWindow *window, GdkEventMotion *event, gpointer data)
{
	int x, y;
	
	cout << "motion_notify_event" << endl;

	x = event->x;
	y = event->y;

	printf("Click %d, %d\n", x, y);

	return TRUE;
}

void transicion(GtkWindow *window, GdkEventMotion *event, gpointer data)
{
	item_transicion_saved = NULL;
	
	cout << "transicion" << endl;

	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(botonEstado),FALSE);

	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(botonLinea))){
		
		botonPulsado="Nada";
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(botonLinea),FALSE);

	}
	else{
		botonPulsado="Linea";
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(botonLinea),TRUE);
	}

}


void mover(GtkWindow *window, GdkEventMotion *event, gpointer data)
{
	botonPulsado = "Mover";
	
	cout << "mover" << endl;
}

void eliminar(GtkWindow *window, GdkEventMotion *event, gpointer data)
{	
	botonPulsado = "Eliminar";
}

void copiar(GtkWindow *window, GdkEventMotion *event, gpointer data)
{
	botonPulsado = "Copiar";
	
	cout << "copiar" << endl;
}

void editar(GtkWindow *window, GdkEventMotion *event, gpointer data)
{
	botonPulsado = "Editar";
	
	cout << "editar" << endl;
}

void nombrar(GtkWindow *window, GdkEventMotion *event, gpointer data)
{
	botonPulsado = "Nombrar";
	
	cout << "nombrar" << endl;
}

void set_zoom (GtkWidget *window, GdkEvent *event, gpointer data){

	double incremento = 0.1;

	cout  <<  gdk_keyval_name  (event->key.keyval) << endl;
	cout  <<  event->key.keyval << endl;

	if (event->key.keyval == GDK_Up)
		zoom += incremento;
	if (event->key.keyval == GDK_Down)
		zoom -= incremento;
	if (event->key.keyval == GDK_KP_0)
		zoom = 1.0;

	gnome_canvas_set_pixels_per_unit ((GnomeCanvas*)canvas, zoom);
	
	cout << "set_zoom" << endl;

}


//------------Funcion arriba añadida para navegacion entre niveles jerarquicos
void arriba(GtkWindow *window, GdkEventMotion *event, gpointer data)
{

	list<tSubAut>::iterator posSub;

	list <tNodo>::iterator posNodo;
	//list <tTransicion>::iterator Trans;
	
	int idPadre;

	printf("Entra ARRIBA\n");

	botonPulsado = "Nada";//Para evitar interferencias con los eventos detectados en el canvas
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(botonEstado),FALSE);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(botonLinea),FALSE);

	posSub = ListaSubAutomatas.begin();

	while(posSub->idSub != subautomata_mostrado){
		posSub++;
	}

	idPadre = posSub->idPadre;

	if (idPadre != 0){
		printf("idPadre distinto de 0\n");
		if (!ListaElementos.empty()){

			printf("La lista NO esta vacia\n");
		
			//Guardamos el nivel actual
			posSub->ListaElementosSub = ListaElementos;		
			posSub->ListaTransicionesSub = ListaTransiciones;
			posSub->tiempoIteracionSub = tiempoIteracion;
			posSub->variablesSub = variables;
			posSub->funcionesSub = funciones;
			posSub->impSub = imp;

			ocultar_subautomata(subautomata_mostrado);

			posSub = ListaSubAutomatas.begin();
	
			while (posSub->idSub != idPadre and posSub != ListaSubAutomatas.end())
				posSub++;

			mostrar_subautomata(idPadre);

			ListaElementos.clear();
			ListaTransiciones.clear();

			//Cargamos el nivel que vamos a mostrar
			ListaElementos = posSub->ListaElementosSub;		
			ListaTransiciones = posSub->ListaTransicionesSub;
			tiempoIteracion = posSub->tiempoIteracionSub;
			variables = posSub->variablesSub;
			funciones = posSub->funcionesSub;
			imp = posSub->impSub;	

		}
		else
		{
			printf("La lista esta vacia\n");
		
			posSub = ListaSubAutomatas.begin();
	
			while (posSub->idSub != subautomata_mostrado and posSub != ListaSubAutomatas.end())
				posSub++;
			
			//Borramos el nodo que se había creado
			ListaSubAutomatas.erase(posSub);

			posSub = ListaSubAutomatas.begin();
	
			while (posSub->idSub != idPadre and posSub != ListaSubAutomatas.end())
				posSub++;

			posNodo = posSub->ListaElementosSub.begin();

			while (posNodo->idHijo != subautomata_mostrado and posNodo != posSub->ListaElementosSub.end())
				posNodo++;

			//Ponemos a 0 el hijo del estado que tenia como hijo al subautomata que se ha borrado.
			posNodo->idHijo = 0;

			change_item_color(posNodo->item, 3);

			mostrar_subautomata(idPadre);

			ListaElementos.clear();
			ListaTransiciones.clear();

			//Cargamos el nivel que vamos a mostrar
			ListaElementos = posSub->ListaElementosSub;		
			ListaTransiciones = posSub->ListaTransicionesSub;
			tiempoIteracion = posSub->tiempoIteracionSub;
			variables = posSub->variablesSub;
			funciones = posSub->funcionesSub;
			imp = posSub->impSub;

		}

		actualizar_tree_view();

		gtk_tree_path_up (arbolPath);	

		gtk_tree_view_expand_to_path(GTK_TREE_VIEW(tree_view), arbolPath);

	}
	else{

		printf("idPadre igual a 0\n");

		actualizar_tree_view();

		gtk_tree_view_expand_to_path(GTK_TREE_VIEW(tree_view), arbolPath);

	}

	printf("Se ejecuta ARRIBA\n");


}
//------------FIN Funcion arriba añadida para navegacion entre niveles jerarquicos

void actualizar_tree_view()
{
    cout << "actualizar_tree_view" << endl;
	list<tSubAut>::iterator posNodoSub;

	//Guardamos los datos del nivel en el que estamos
	posNodoSub = ListaSubAutomatas.begin();
		
	while (posNodoSub->idSub != subautomata_mostrado and posNodoSub != ListaSubAutomatas.end())
		posNodoSub++;
	
	//Guardamos el nivel actual
	posNodoSub->ListaElementosSub = ListaElementos;		
	posNodoSub->ListaTransicionesSub = ListaTransiciones;
	posNodoSub->tiempoIteracionSub = tiempoIteracion;
	posNodoSub->variablesSub = variables;
	posNodoSub->funcionesSub = funciones;
	posNodoSub->impSub = imp;

	//actualizamos los datos del modelo del tree_view
	gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view), create_and_fill_model());

}


void rellenar_arbol_recursivo(GtkTreeStore *treestore, GtkTreeIter toplevel, int idSubautomata)
{
    cout << "rellenar_arbol_recursivo" << endl;
	GtkTreeIter child;
	list<tSubAut>::iterator posNodoSub;
	list<tNodo>::iterator posEst;

	string cadena = "";

	int contSinNombre = 1;

	posNodoSub = ListaSubAutomatas.begin();

	while (posNodoSub->idSub != idSubautomata and posNodoSub != ListaSubAutomatas.end())
	{
		posNodoSub++;
	}

	if (!posNodoSub->ListaElementosSub.empty())
	{

		posEst = posNodoSub->ListaElementosSub.begin();

		while (posEst != posNodoSub->ListaElementosSub.end())
		{

			if (posEst->nombre != "")
			{
				cadena = posEst->nombre;
			}
			else
			{
				cadena = "NoName_"+int2string(contSinNombre);
				contSinNombre++;
			}

			gtk_tree_store_append(treestore, &child, &toplevel);
		    gtk_tree_store_set(treestore, &child, COLUMN, cadena.c_str(),-1);

			if (posEst->idHijo != 0)
			{
				//Con esta función recorreremos la lista de subautomatas recursivamente para ir añadiendo los datos correspondientes
				//en el treeview
				rellenar_arbol_recursivo(treestore, child, posEst->idHijo);

			}

			posEst++;		

		}	

	}
	
}

static GtkTreeModel *
create_and_fill_model (void)
{
    cout << "create_and_fill_model" << endl;
  	GtkTreeStore *treestore;
  	GtkTreeIter toplevel;

  	list<tSubAut>::iterator posNodoSub;
	list<tNodo>::iterator posEst;

	string cadena = "";

	int contSinNombre = 1;

  	treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING);

	if (!ListaSubAutomatas.empty())
	{ 
		posNodoSub = ListaSubAutomatas.begin();

		if (!posNodoSub->ListaElementosSub.empty())
		{
			posEst = posNodoSub->ListaElementosSub.begin();

			while (posEst != posNodoSub->ListaElementosSub.end())
			{

				if (posEst->nombre != "")
				{
					cadena = posEst->nombre;
				}
				else
				{
					cadena = "NoName_"+int2string(contSinNombre);
					contSinNombre++;
				}

				gtk_tree_store_append(treestore, &toplevel, NULL);
			    gtk_tree_store_set(treestore, &toplevel,
                 COLUMN, cadena.c_str(),
                 -1);

				if (posEst->idHijo != 0)
				{
					//Con esta función recorreremos la lista de subautomatas recursivamente para ir añadiendo los datos correspondientes
					//en el treeview
					rellenar_arbol_recursivo(treestore, toplevel, posEst->idHijo);

				}

				posEst++;		

			}		

		}

	}

  return GTK_TREE_MODEL(treestore);
}

void treeview_onRowActivated(GtkTreeView *treeview,
							 GtkTreePath *path,
							 GtkTreeViewColumn *col,
							 gpointer userdata){
							 
	cout << "treeview_onRowActivated" << endl;

	list<tSubAut>::iterator posSub;

	list<tNodo>::iterator posNodo;

	gint* indices;

	int prof_path;

	int niveles_restantes=0;
	int i=0;

	int sub_a_buscar = 1;
	int sub_a_mostrar = 1;

	int indice_leido = 0;
	int contNodo = 0;

	bool expandir = FALSE;


	botonPulsado = "Nada";//Para evitar interferencias con los eventos detectados en el canvas

	prof_path = gtk_tree_path_get_depth(path);

	

	if (prof_path == 1) {//hemos seleccionado el primer nivel con lo cual mostramos la raiz

		sub_a_mostrar = 1;

		printf("Selecionado el raiz*********\n");

		arbolPath = gtk_tree_path_new_first();
		expandir = FALSE;

	}
	else{

		//Quitamos el ultimo elemento del path porque queremos mostrar el subautomata con el ID que nos indique su padre
		//con lo cual solo necesitaremos recorrer la lista de subautomatas hasta su padre.
		gtk_tree_path_up (path);

		arbolPath = gtk_tree_path_copy(path);

		printf("path del doble click: %s\n", gtk_tree_path_to_string(path));

		//Obtenemos sus indices en un array
		indices = gtk_tree_path_get_indices(path);

		//Obtenemos su profundidad para saber hasta cuando iterar al recorrer la lista de subautomatas
		prof_path = gtk_tree_path_get_depth(path);

		niveles_restantes = prof_path;

		while (niveles_restantes != 0) {

			printf("Niveles restantes: %d\n", niveles_restantes);			
			indice_leido = indices[i];		

			printf("Indice leido: %d\n", indice_leido);

			posSub = ListaSubAutomatas.begin();
	
			printf("Se busca por: %d\n", sub_a_buscar);

			while(posSub->idSub != sub_a_buscar){
				posSub++;
			}

			posNodo = posSub->ListaElementosSub.begin();

			while(contNodo != indice_leido){
				posNodo++;
				contNodo++;

				printf("Contador: %d\n", contNodo);
			}

			sub_a_buscar = posNodo->idHijo;

			printf("Proxima busqueda: %d\n", sub_a_buscar);

			contNodo = 0;
			i++;
			niveles_restantes--;

		}

		sub_a_mostrar = sub_a_buscar;

		sub_a_buscar = 1;

		printf("Sub a mostrar: %d\n", sub_a_mostrar);

		expandir = TRUE;

	}


	//Ahora viene una gestion equivalente a la de el boton "UP"
	int idPadre;

	posSub = ListaSubAutomatas.begin();

	while(posSub->idSub != subautomata_mostrado){
		posSub++;
	}

	idPadre = posSub->idPadre;
	
	if (!ListaElementos.empty()){
	
		//Guardamos el nivel actual
		posSub->ListaElementosSub = ListaElementos;		
		posSub->ListaTransicionesSub = ListaTransiciones;
		posSub->tiempoIteracionSub = tiempoIteracion;
		posSub->variablesSub = variables;
		posSub->funcionesSub = funciones;
		posSub->impSub = imp;

		ocultar_subautomata(subautomata_mostrado);

		posSub = ListaSubAutomatas.begin();

		while (posSub->idSub != sub_a_mostrar and posSub != ListaSubAutomatas.end())
			posSub++;

		mostrar_subautomata(sub_a_mostrar);

		ListaElementos.clear();
		ListaTransiciones.clear();

		//Cargamos el nivel que vamos a mostrar
		ListaElementos = posSub->ListaElementosSub;		
		ListaTransiciones = posSub->ListaTransicionesSub;
		tiempoIteracion = posSub->tiempoIteracionSub;
		variables = posSub->variablesSub;
		funciones = posSub->funcionesSub;
		imp = posSub->impSub;	

	}
	else
	{
	
		posSub = ListaSubAutomatas.begin();

		while (posSub->idSub != subautomata_mostrado and posSub != ListaSubAutomatas.end())
			posSub++;
		
		//Borramos el nodo que se había creado
		ListaSubAutomatas.erase(posSub);

		posSub = ListaSubAutomatas.begin();

		while (posSub->idSub != idPadre and posSub != ListaSubAutomatas.end())
			posSub++;

		posNodo = posSub->ListaElementosSub.begin();

		while (posNodo->idHijo != subautomata_mostrado and posNodo != posSub->ListaElementosSub.end())
			posNodo++;

		//Ponemos a 0 el hijo del estado que tenia como hijo al subautomata que se ha borrado.
		posNodo->idHijo = 0;

		change_item_color(posNodo->item, 3);

		posSub = ListaSubAutomatas.begin();

		while (posSub->idSub != sub_a_mostrar and posSub != ListaSubAutomatas.end())
			posSub++;

		mostrar_subautomata(sub_a_mostrar);

		ListaElementos.clear();
		ListaTransiciones.clear();

		//Cargamos el nivel que vamos a mostrar
		ListaElementos = posSub->ListaElementosSub;		
		ListaTransiciones = posSub->ListaTransicionesSub;
		tiempoIteracion = posSub->tiempoIteracionSub;
		variables = posSub->variablesSub;
		funciones = posSub->funcionesSub;
		imp = posSub->impSub;

	}

	actualizar_tree_view();	

	if (expandir){ //Solo hacemos la expansion si no hemos hecho doble click en el raiz
		
		gtk_tree_view_expand_to_path(GTK_TREE_VIEW(tree_view), arbolPath);
	}

}

static GtkWidget *
create_view_and_model (void)
{
  cout << "create_view_and_model" << endl;
  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;
  GtkWidget *view;
  GtkTreeModel *model;

  view = gtk_tree_view_new();

  col = gtk_tree_view_column_new();
  //gtk_tree_view_column_set_title(col, (const gchar *)nombreEsquema);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, 
      "text", COLUMN);

  model = create_and_fill_model();
  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
  g_object_unref(model); 

  g_signal_connect(view,"row-activated", (GCallback) treeview_onRowActivated, NULL);

  return view;
}

void borrar_subautomata_recursivo(int sub_a_borrar){
    cout << "borrar_subautomata_recursivo" << endl;

	list<tSubAut>::iterator posNodoSub;
	list<tNodo>::iterator posEst;

	posNodoSub = ListaSubAutomatas.begin();

	while (posNodoSub->idSub != sub_a_borrar and posNodoSub != ListaSubAutomatas.end())
	{
		posNodoSub++;
	}

	posEst = posNodoSub->ListaElementosSub.begin();

	while (posEst != posNodoSub->ListaElementosSub.end())
	{

		if (posEst->idHijo != 0){
			borrar_subautomata_recursivo(posEst->idHijo);
		}

		posEst++;

	}

	ListaSubAutomatas.erase(posNodoSub);

}



int main( int argc, char *argv[])
{
	
	GtkWidget *boton;
	//GtkWidget *botonLinea;
	//GtkWidget *botonEstado;
	GtkWidget *botonGuardar;
	GtkWidget *botonCargar;
	GtkWidget *caja1;
	GtkWidget *scrolled;
	GtkWidget *scrolled_tree;
	GtkWidget *separator;

	GtkWidget *frame_horz;
	GtkWidget *vbox;
	const char * text = "Spread (espacio 40)";

	/*Inicializar parametros*/
	gtk_init(&argc, &argv);

	/*######################### Creacion de menus desplegables #############################*/

	menu_estado = gtk_menu_new();

	nombrar_estado = gtk_menu_item_new_with_label("Rename");	
	editar_estado = gtk_menu_item_new_with_label("Edit");
	marcar_inicial_estado = gtk_menu_item_new_with_label("Mark as initial");	
	copiar_estado = gtk_menu_item_new_with_label("Copy");
	eliminar_estado = gtk_menu_item_new_with_label("Delete");		

	gtk_menu_shell_append(GTK_MENU_SHELL(menu_estado), nombrar_estado);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_estado), editar_estado);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_estado), marcar_inicial_estado);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_estado), copiar_estado);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_estado), eliminar_estado);

	g_signal_connect_swapped (nombrar_estado, "activate", G_CALLBACK (on_menu_estado_nombrar), menu_estado);//Movido desde handler
	g_signal_connect_swapped (editar_estado, "activate", G_CALLBACK (on_menu_estado_editar), menu_estado);
	g_signal_connect_swapped (marcar_inicial_estado, "activate", G_CALLBACK (on_menu_estado_marcar_inicial), menu_estado);
	g_signal_connect_swapped (copiar_estado, "activate", G_CALLBACK (on_menu_estado_copiar), menu_estado);
	g_signal_connect_swapped (eliminar_estado, "activate", G_CALLBACK (on_menu_estado_eliminar), menu_estado);

	gtk_widget_show (nombrar_estado);
	gtk_widget_show (editar_estado);
	gtk_widget_show (marcar_inicial_estado);
	gtk_widget_show (copiar_estado);
	gtk_widget_show (eliminar_estado);


	menu_transicion = gtk_menu_new();

	nombrar_transicion = gtk_menu_item_new_with_label("Rename");	
	editar_transicion = gtk_menu_item_new_with_label("Edit");
	eliminar_transicion = gtk_menu_item_new_with_label("Delete");

	gtk_menu_shell_append(GTK_MENU_SHELL(menu_transicion), nombrar_transicion);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_transicion), editar_transicion);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_transicion), eliminar_transicion);

	g_signal_connect_swapped (nombrar_transicion, "activate", G_CALLBACK (on_menu_transicion_nombrar), menu_transicion);//Movido desde handler
	g_signal_connect_swapped (editar_transicion, "activate", G_CALLBACK (on_menu_transicion_editar), menu_transicion);
	g_signal_connect_swapped (eliminar_transicion, "activate", G_CALLBACK (on_menu_transicion_eliminar), menu_transicion);

	gtk_widget_show (nombrar_transicion);
	gtk_widget_show (editar_transicion);
	gtk_widget_show (eliminar_transicion);


	menu_pegar = gtk_menu_new();

	pegar_estado = gtk_menu_item_new_with_label("Paste");

	gtk_menu_shell_append(GTK_MENU_SHELL(menu_pegar), pegar_estado);

	g_signal_connect_swapped (pegar_estado, "activate", G_CALLBACK (on_menu_pegar), menu_pegar);

	gtk_widget_show (pegar_estado);

	/*####################### FIN Creacion de menus desplegables ############################*/


  	/*Creamos Ventana*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	/*Situamos la ventana en el medio*/
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_NONE);

	/*Ponemos tamaño a la ventana*/
	gtk_window_set_default_size(GTK_WINDOW(window), 1024, 768);
	gtk_window_maximize (GTK_WINDOW(window));
	
	/*Ponemos el titulo*/
	gtk_window_set_title(GTK_WINDOW(window), "Default");

	/*  gtk_window_set_decorated(GTK_WINDOW (window), FALSE);*/
	gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

	/*Accion (motion_notify_event) asignada al pulsar el boton*/
	gtk_signal_connect(GTK_OBJECT(window), "button_press_event", GTK_SIGNAL_FUNC(coordenadas), (gpointer)botonPulsado.c_str()); 
	gtk_signal_connect(GTK_OBJECT(window), "button_release_event", GTK_SIGNAL_FUNC(pinta_nodo), (gpointer)botonPulsado.c_str());
	//gtk_signal_connect(GTK_OBJECT(window), "2button_press_event", GTK_SIGNAL_FUNC(pegar), NULL); 
        gtk_signal_connect(GTK_OBJECT(window), "key-press-event", GTK_SIGNAL_FUNC(set_zoom), (gpointer)canvas);

	/*Accion (gtk_main_quit) asignada al pulsar el boton X (salir)*/
	gtk_signal_connect_object(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(gtk_main_quit), GTK_OBJECT(window));

	/*Creamos el boton*/
	botonLinea = gtk_toggle_button_new_with_label ("Transition");
	botonEstado = gtk_toggle_button_new_with_label ("Estate");
	botonGuardar = gtk_button_new_with_label ("Save");
	botonCargar = gtk_button_new_with_label ("Open");
//	botonGuardar = gtk_button_new_from_stock (GTK_STOCK_SAVE);
//	botonCargar = gtk_button_new_from_stock (GTK_STOCK_OPEN);
	
	/*Al pulsar el boton, muestra las coordenadas x,y por pantalla*/
	
	gtk_signal_connect(GTK_OBJECT(botonLinea), "button_release_event", GTK_SIGNAL_FUNC(transicion), (gpointer)botonPulsado.c_str()); 
	gtk_signal_connect(GTK_OBJECT(botonEstado), "button_release_event", GTK_SIGNAL_FUNC(nodo), (gpointer)0); 

	gtk_signal_connect(GTK_OBJECT(botonGuardar), "released", GTK_SIGNAL_FUNC(guardar), (gpointer)0); 
	gtk_signal_connect(GTK_OBJECT(botonCargar), "button_release_event", GTK_SIGNAL_FUNC(cargar), NULL); 

	
	gtk_container_border_width (GTK_CONTAINER (window), 25);

	
	/* ventana para tener barras de desplazamiento */
        scrolled = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);


	/* canvas en modo RGB */
        canvas = gnome_canvas_new_aa();
	
        gtk_container_add(GTK_CONTAINER(scrolled), canvas);

        gnome_canvas_set_scroll_region(GNOME_CANVAS(canvas),
                        0, 0, CANVAS_SIZE, CANVAS_SIZE);
	
	root = gnome_canvas_root(GNOME_CANVAS(canvas));

	/*######################################################### AÑADIENDO TREE VIEW ##########################################################*/

	/* ventana para tener barras de desplazamiento */
    scrolled_tree = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_tree), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);

	tree_view = create_view_and_model();

	gtk_container_add(GTK_CONTAINER(scrolled_tree), tree_view);

	arbolPath = gtk_tree_path_new_first();

	printf("*********************************** Profundidad del path: %d \n", gtk_tree_path_get_depth (arbolPath));

	//gtk_tree_view_expand_all(GTK_TREE_VIEW(tree_view));

	/*######################################################FIN DE AÑADIENDO TREE VIEW#######################################################*/
	

	/*Creamos una caja, contenemos el botons y la imagen en la caja y la caja en la ventana*/


	caja1 = gtk_hbox_new(FALSE, 0);

	separator = gtk_vseparator_new ();


	/*Cambiar color canvas*/
	
	GtkWidget *frame = gtk_frame_new (NULL);

	gtk_widget_set_size_request (frame, 650, 300);//el 150 era 300

	gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);

	gtk_container_add(GTK_CONTAINER(frame), scrolled);

	GtkWidget *frame_tree = gtk_frame_new (NULL);

	gtk_widget_set_size_request (frame_tree, 150, 300);

	gtk_container_add(GTK_CONTAINER(frame_tree), scrolled_tree);

	
	gtk_container_add(GTK_CONTAINER(caja1), frame_tree);

	gtk_container_add(GTK_CONTAINER(caja1), frame);

	
	frame_horz = gtk_frame_new ("Options");


	/* Redimensionar Frame */
	gtk_widget_set_size_request (frame_horz,300, -1);

 	gtk_box_pack_start (GTK_BOX (caja1), frame_horz, TRUE, TRUE, 10);
	

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
 	gtk_container_add (GTK_CONTAINER (frame_horz), vbox);

	
	text = "Navigation";
	
	string textAyuda = "Esto es una prueba";

	/*boton = gtk_button_new_with_label ("Mover");
	gtk_signal_connect(GTK_OBJECT(boton), "button_release_event", GTK_SIGNAL_FUNC(mover), 0); 
	ListaBotones.push_back(boton);

	boton = gtk_button_new_from_stock (GTK_STOCK_DELETE);
	gtk_signal_connect(GTK_OBJECT(boton), "button_release_event", GTK_SIGNAL_FUNC(eliminar), 0); 
	ListaBotones.push_back(boton);*/

	
	//###################Añadido boton Arriba
	boton = gtk_button_new_with_label ("UP");
	gtk_signal_connect(GTK_OBJECT(boton), "button_release_event", GTK_SIGNAL_FUNC(arriba), NULL); 
	ListaBotones.push_back(boton);


  	gtk_box_pack_start (GTK_BOX (vbox),
        create_bbox (TRUE, (char *)text, 40, 85, 20, GTK_BUTTONBOX_SPREAD,(gpointer)botonLinea , (gpointer)ayuda[0].c_str()), TRUE, TRUE, 0);

	ListaBotones.erase( ListaBotones.begin(), ListaBotones.end());
	

	text = "Figures";
	
	ListaBotones.push_back(botonLinea);
	ListaBotones.push_back(botonEstado);
//	boton = gtk_button_new_with_label ("Copy");
//	boton = gtk_button_new_from_stock (GTK_STOCK_COPY);
//	gtk_signal_connect(GTK_OBJECT(boton), "button_release_event", GTK_SIGNAL_FUNC(copiar), 0); 
//	ListaBotones.push_back(boton);	

	textAyuda  = "aa";
  	gtk_box_pack_start (GTK_BOX (vbox),
        create_bbox (TRUE, (char *)text, 40, 85, 20, GTK_BUTTONBOX_SPREAD,(gpointer)botonLinea, (gpointer)ayuda[1].c_str()), TRUE, TRUE, 0);

	ListaBotones.erase( ListaBotones.begin(), ListaBotones.end());

	text = "Save/Open";

	ListaBotones.push_back(botonGuardar);

	boton = gtk_button_new_with_label ("Save as");
//	boton = gtk_button_new_from_stock (GTK_STOCK_SAVE_AS);
	gtk_signal_connect(GTK_OBJECT(boton), "released", GTK_SIGNAL_FUNC(guardar), (gpointer)1); 

	ListaBotones.push_back(boton);

	ListaBotones.push_back(botonCargar);

  	gtk_box_pack_start (GTK_BOX (vbox),
        create_bbox (TRUE, (char *)text, 40, 85, 20, GTK_BUTTONBOX_SPREAD,(gpointer)botonLinea, (gpointer)ayuda[2].c_str()), TRUE, TRUE, 0);

	ListaBotones.erase( ListaBotones.begin(), ListaBotones.end());

/*	text = "Edicion Estados";

	//Creamos el boton
	
	boton = gtk_button_new_with_label ("EstadoInicial");
	gtk_signal_connect(GTK_OBJECT(boton), "button_release_event", GTK_SIGNAL_FUNC(nodo), (gpointer)1); 
	ListaBotones.push_back(boton);

	boton = gtk_button_new_with_label ("Nombrar");
	gtk_signal_connect(GTK_OBJECT(boton), "button_release_event", GTK_SIGNAL_FUNC(nombrar), NULL); 
	ListaBotones.push_back(boton);

	boton = gtk_button_new_from_stock (GTK_STOCK_EDIT);		
	gtk_signal_connect(GTK_OBJECT(boton), "button_release_event", GTK_SIGNAL_FUNC(editar), NULL); 
	ListaBotones.push_back(boton);

  	gtk_box_pack_start (GTK_BOX (vbox),
        create_bbox (TRUE, (char *)text, 40, 85, 20, GTK_BUTTONBOX_SPREAD,(gpointer)botonLinea, (gpointer)ayuda[3].c_str()), TRUE, TRUE, 0);

	ListaBotones.erase( ListaBotones.begin(), ListaBotones.end());
*/
	text = "Subautomata data";

	boton = gtk_button_new_with_label ("Intefaces");
	ListaBotones.push_back(boton);
	gtk_signal_connect(GTK_OBJECT(boton), "button_release_event", GTK_SIGNAL_FUNC(ventana_importar), NULL);

	boton = gtk_button_new_with_label ("Timer");
	ListaBotones.push_back(boton);
	gtk_signal_connect(GTK_OBJECT(boton), "button_release_event", GTK_SIGNAL_FUNC(ventana_timer), NULL);

	boton = gtk_button_new_with_label ("Variables");
	ListaBotones.push_back(boton);
	gtk_signal_connect(GTK_OBJECT(boton), "button_release_event", GTK_SIGNAL_FUNC(codigo_esquema), NULL); 


	gtk_box_pack_start (GTK_BOX (vbox),
        create_bbox (TRUE, (char *)text, 40, 85, 20, GTK_BUTTONBOX_SPREAD,(gpointer)botonLinea, (gpointer)ayuda[4].c_str()), TRUE, TRUE, 0);

	ListaBotones.erase( ListaBotones.begin(), ListaBotones.end());

	text = "Code and Compile";

	GtkWidget* img1= gtk_image_new_from_file("./imagenes/compilar.jpg"); 
	boton = gtk_button_new_with_label ("Generate code");
	gtk_button_set_image((GtkButton*)boton,img1); 
	ListaBotones.push_back(boton);
	gtk_signal_connect(GTK_OBJECT(boton), "button_release_event", GTK_SIGNAL_FUNC(generar_c), NULL);

	boton = gtk_button_new_with_label ("Compile"); // Compilar
	ListaBotones.push_back(boton);
	gtk_signal_connect(GTK_OBJECT(boton), "button_release_event", GTK_SIGNAL_FUNC(compilar), NULL); 

	gtk_box_pack_start (GTK_BOX (vbox),
        create_bbox (TRUE, (char *)text, 40, 85, 20, GTK_BUTTONBOX_SPREAD,(gpointer)botonLinea, (gpointer)ayuda[4].c_str()), TRUE, TRUE, 0);

	ListaBotones.erase( ListaBotones.begin(), ListaBotones.end());

	gtk_container_add(GTK_CONTAINER(window), caja1);

	gtk_widget_show_all(window);

	//**********************************************************************Creamos el primer nodo en la lista de subautomatas
	//**********************************************************************Habra que eliminarlo, hacer este guardado en cambios
	//**********************************************************************de nivel o cuando se guarde el esquema.
	reg_sub.ListaElementosSub = ListaElementos;
	reg_sub.ListaTransicionesSub = ListaTransiciones;
	reg_sub.idSub = idSubGlobal;
	reg_sub.idPadre = 0;

	ListaSubAutomatas.push_back(reg_sub);

	idSubGlobal++;
    //************************************************************************************************************************


	/*Bucle*/
	gtk_main();

	return 0;
}


void desconectarLinea (GnomeCanvasItem *transicion, GnomeCanvasItem *estado)
{
    cout << "desconectarLinea" << endl;
	list<tNodo>::iterator pos;

	list<GnomeCanvasItem *>::iterator pos2;


				
	pos = ListaElementos.begin();

	while(pos->item != estado)
	{
  		pos++;
	}
				
	pos2 = pos->listaAdyacentes.begin();

	while((*pos2) != transicion)
	{
  		pos2++;
	}
	pos->listaAdyacentes.erase(pos2);

}


void eliminarLinea (GnomeCanvasItem *item, gpointer data)
{
    cout << "eliminarLinea" << endl;
	if ((botonPulsado == "Eliminar") || (botonPulsado == "Mover")) {
		list<tTransicion>::iterator pos;

		pos = ListaTransiciones.begin();

			

		while(pos != ListaTransiciones.end())
		{
  					pos++;
		}

		pos = ListaTransiciones.begin();
		
		while(pos->item != item)
		{
  			pos++;
		}
		GnomeCanvasItem * origen = pos->origen;
	
		GnomeCanvasItem * destino = pos->destino;

		desconectarLinea (item,origen);
				
		desconectarLinea (item,destino);

		ListaTransiciones.erase(pos);

		gtk_object_destroy (GTK_OBJECT (item));

	}
}
void
change_item_width (gpointer data, gpointer user_data)
{
    cout << "change_item_width" << endl;
	gnome_canvas_item_set (GNOME_CANVAS_ITEM (data),
			      "width_units", (double)(int)user_data,
			       NULL);
}

static gint
item_event_linea (GnomeCanvasItem *item, GdkEvent *event, gpointer data)
{
    cout << "item_event_linea" << endl;
	GList * list_items;
	
	switch (event->type) {
	case GDK_2BUTTON_PRESS:
		if ((event->button.button == 1) & (botonPulsado == "Editar")){
			/* Cambia el color del item */	
			cout << "Cambiar color: " << endl;	
			id = (int)item;	
			change_item_color (item,0);
			new_text_windows (item,(gchar*)"transicion");
			return TRUE;
		}
		break;
	case GDK_BUTTON_PRESS:
		if ((event->button.button == 3) & (botonPulsado == "Eliminar")){
			eliminarLinea(item, NULL);	
			cout << "Eliminado: " << endl;
			return TRUE;
		} 
		break;
	case GDK_ENTER_NOTIFY:
		/* Establece la linea ancha */	
		if (botonPulsado != "Nombrar"){	
			list_items = GNOME_CANVAS_GROUP (item)->item_list;
			g_list_foreach (list_items, change_item_width,  (gpointer) 3);
		}
		return TRUE;

	case GDK_LEAVE_NOTIFY:
		/* Establece la linea estrecha */
		if (botonPulsado != "Nombrar"){	
			list_items = GNOME_CANVAS_GROUP (item)->item_list;
			g_list_foreach (list_items, change_item_width,  (gpointer) 1);
		}
		return TRUE;

	default:
		break;
	}

	return FALSE;

}



gint
highlight_box (GnomeCanvasItem *item, GdkEvent *event, gpointer data)
{
      cout << "highlight_box" << endl;
      GdkCursor *fleur;
      static double x, y; /* used to keep track of motion coordinates */
      double new_x, new_y;
      double x1,x2,y1,y2;      
      list<tTransicion>::iterator posLinea;
      GnomeCanvasItem *origen, *destino, *arrow, *box;
      GnomeCanvasGroup *group, *group_parent;
      GList * list_items;
      GtkWidget *ventana;
      bool b;

      switch (event->type) {
      case GDK_ENTER_NOTIFY:
            gnome_canvas_item_set (item,
                               "fill_color", "red",
                               NULL);

            break;

      case GDK_LEAVE_NOTIFY:
            if (!(event->crossing.state & GDK_BUTTON1_MASK))
                  gnome_canvas_item_set (item,
                                     "fill_color_rgba", NULL,
                                     NULL);
            break;

      case GDK_BUTTON_PRESS:
	    		
	    if ((event->button.button == 1) && (botonPulsado == "Nombrar")) {
			
			change_item_color (item,0);

			ventana = comprobar_ventana_abierta (item->parent, NOMBRAR);
		
			if (ventana != NULL) 
				gtk_window_present (GTK_WINDOW(ventana));
			else{
					ventana = new_name_windows (item->parent);
				add_ventana (ventana, item->parent, NOMBRAR);
			}
			
	    }else if (event->button.button == 1) {
		/* Recuerda la posición inicial */			
	    	x = event->button.x;
	    	y = event->button.y;

	    }else if (event->button.button == 3) { //*******************************añadido menu contextual con boton derecho.
			
			printf("Entra con boton derecho transicion\n");

			gtk_menu_popup(GTK_MENU(menu_transicion),NULL,NULL,NULL,NULL,event->button.button,event->button.time);

			item_menu_transicion = item;

			return TRUE;
		} 
		break;

      case GDK_MOTION_NOTIFY:
	    //if (botonPulsado == "Mover"){

		if (event->motion.state & GDK_BUTTON1_MASK) { // Si está pulsado el botón 1

			botonPulsado = "Nada";
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(botonEstado),FALSE);
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(botonLinea),FALSE);

	          	fleur = gdk_cursor_new (GDK_FLEUR);
           		 gnome_canvas_item_grab (item,
                 	             GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK,
                 	             fleur,
                  	            event->button.time);
            		gdk_cursor_unref (fleur);
	
			new_x = event->motion.x;
			new_y = event->motion.y;
		
			group_parent = GNOME_CANVAS_GROUP(item->parent);

			list_items = group_parent->item_list;

			box = (GnomeCanvasItem *) g_list_nth_data (list_items, g_list_length (list_items)-1);  

			g_object_get (G_OBJECT (box), "x1", &x1, NULL);
			g_object_get (G_OBJECT (box), "y1", &y1, NULL);
			g_object_get (G_OBJECT (box), "x2", &x2, NULL);
			g_object_get (G_OBJECT (box), "y2", &y2, NULL);

			cout << "Puntos item: " << x1 << "," << y1 << "," << x2 << "," << y2 <<endl;
			
			gnome_canvas_item_set (box,
						"x1", x1 + (new_x - x),
                        			"y1", y1 + (new_y - y),
                        			"x2", x2 + (new_x - x),
                        			"y2", y2 + (new_y - y),
                				NULL);

		
			
			posLinea = ListaTransiciones.begin();

			while((posLinea->item != GNOME_CANVAS_ITEM(group_parent) ))	//cogemos el padre (grupo) del item (box)
			{
					posLinea++;
			}

			origen = posLinea->origen;
			destino = posLinea->destino;

			/* Mover Item_Nombre Estado */
			if ( posLinea->item_nombre != NULL )
				gnome_canvas_item_move ((posLinea->item_nombre), new_x - x, new_y - y);
			
			/* Actualizar Posición */
			x = new_x;
			y = new_y;
			

			/* Eliminamos Lineas de la Transicion  Previa */
					
			while (g_list_length (list_items) > 1){
			
				arrow = (GnomeCanvasItem *) g_list_nth_data (list_items, 0);
				list_items=g_list_next(list_items);
				gtk_object_destroy (GTK_OBJECT (arrow));
				
			}


			group = repintar_transicion (GNOME_CANVAS_GROUP (item->canvas->root), origen, destino, (x1+x2)/2, (y1+y2)/2);		

			gnome_canvas_item_reparent (box, group);

			b = cambiar_item_transicion (GNOME_CANVAS_ITEM (group_parent), GNOME_CANVAS_ITEM (group));

			cambiar_item_adyacentes (origen, GNOME_CANVAS_ITEM (group_parent), GNOME_CANVAS_ITEM (group));

			cambiar_item_adyacentes (destino, GNOME_CANVAS_ITEM (group_parent), GNOME_CANVAS_ITEM (group));
		}

	    //}
            break;
      case GDK_BUTTON_RELEASE:
            gnome_canvas_item_ungrab (item, event->button.time);

	    if ((event->button.button == 1) & (botonPulsado == "Eliminar"))
	    {
		group_parent = GNOME_CANVAS_GROUP(item->parent);
		
		eliminar_nombre(GNOME_CANVAS_ITEM(group_parent));
		
		t_eliminar_transicion_lista (GNOME_CANVAS_ITEM(group_parent));
		
		gtk_object_destroy (GTK_OBJECT (group_parent));

	    }
	    else if ((event->button.button == 1) & (botonPulsado == "Mover")) 
			 gnome_canvas_item_ungrab (item, event->button.time);
            break;
      case GDK_2BUTTON_PRESS:
	    if ((event->button.button == 1) & (botonPulsado == "Editar")){

		/* Cambia el color del item */	
	    	change_item_color (item,0);

			ventana = comprobar_ventana_abierta (item->parent, EDITAR);
		
			if (ventana != NULL) 
				gtk_window_present (GTK_WINDOW(ventana));
			else{
					ventana = new_text_windows (item->parent ,(gchar*)"transicion");
				add_ventana (ventana, item->parent, EDITAR);
			}
	    }
	    break;

      default:
            break;
      }

      return FALSE;
}

gint
highlight_box_edit (GnomeCanvasItem *item, GdkEvent *event, gpointer data)
{
      cout << "hightlight_box_edit" << endl;
      static double x, y; /* used to keep track of motion coordinates */
      list<tTransicion>::iterator posLinea;
      GnomeCanvasGroup *group_parent;
      GtkWidget *ventana;

      switch (event->type) {
      case GDK_ENTER_NOTIFY:
            gnome_canvas_item_set (item,
                               "fill_color", "red",
                               NULL);

            break;

      case GDK_LEAVE_NOTIFY:
            if (!(event->crossing.state & GDK_BUTTON1_MASK))
                  gnome_canvas_item_set (item,
                                     "fill_color_rgba", NULL,
                                     NULL);
            break;

      case GDK_BUTTON_PRESS:
	    		
	    if ((event->button.button == 1) && (botonPulsado == "Nombrar")) {
			
		change_item_color (item,0);

		ventana = comprobar_ventana_abierta (item->parent, NOMBRAR);
		
		if (ventana != NULL) 
			gtk_window_present (GTK_WINDOW(ventana));
		else{
	    		ventana = new_name_windows (item->parent);
			add_ventana (ventana, item->parent, NOMBRAR);
		}
			
	    }else if (event->button.button == 1) {
		/* Recuerda la posición inicial */			
	    	x = event->button.x;
	    	y = event->button.y;

	    }else if (event->button.button == 3) { //*******************************añadido menu contextual con boton derecho.
			
			printf("Entra con boton derecho autotransicion\n");

			gtk_menu_popup(GTK_MENU(menu_transicion),NULL,NULL,NULL,NULL,event->button.button,event->button.time);

			item_menu_transicion = item;

			return TRUE;
		} 
            break;
      case GDK_MOTION_NOTIFY:
	   
            break;
      case GDK_BUTTON_RELEASE:
            gnome_canvas_item_ungrab (item, event->button.time);

	    if ((event->button.button == 1) & (botonPulsado == "Eliminar"))
	    {
		group_parent = GNOME_CANVAS_GROUP(item->parent);

		eliminar_nombre(GNOME_CANVAS_ITEM(group_parent));

		t_eliminar_transicion_lista (GNOME_CANVAS_ITEM(group_parent));
		
		gtk_object_destroy (GTK_OBJECT (group_parent));

	    }
	   
            break;
      case GDK_2BUTTON_PRESS:
	    if ((event->button.button == 1) & (botonPulsado == "Editar")){

		/* Cambia el color del item */	
	    	change_item_color (item,0);

		ventana = comprobar_ventana_abierta (item->parent, EDITAR);
		
		if (ventana != NULL) 
			gtk_window_present (GTK_WINDOW(ventana));
		else{
	    		ventana = new_text_windows (item->parent ,(gchar*)"transicion");
			add_ventana (ventana, item->parent, EDITAR);
		}
	    }
	    break;

      default:
            break;
      }

      return FALSE;
}

static gint
box_event (GnomeCanvasItem *item, GdkEvent *event, gpointer data)
{
      cout << "box_event" << endl;
      if ((event->type != GDK_MOTION_NOTIFY) || !(event->motion.state & GDK_BUTTON1_MASK))
            return FALSE;

      return FALSE;
}

void
create_drag_box (GnomeCanvasGroup *group, char *box_name, double x1, double y1, GCallback callback)
{
    cout << "create_drag_box" << endl;
	GnomeCanvasItem *box;

	box = gnome_canvas_item_new (group,
                             gnome_canvas_rect_get_type (),
                             "fill_color", NULL,
                             "outline_color", "black",
                             "width_pixels", 0,
                             NULL);
 
	gnome_canvas_item_set (box,
                         "x1", x1 - 3,
                         "y1", y1 - 3,
                         "x2", x1 + 3,
                         "y2", y1 + 3,
                         NULL);

	g_signal_connect (box, "event",
                    callback,
                    NULL);

	g_object_set_data (G_OBJECT (group), box_name, box);
}

/***** Pintar Transicion *****/
/* modo indica si la transición existe y se repinta la transicion o no existe y se pinta una transición nueva  */
GnomeCanvasItem *
pinta_transicion (GnomeCanvasItem *origen, GnomeCanvasItem *destino)
{
    cout << "pinta_transicion" << endl;
	GnomeCanvasPoints *pointsPoligono = gnome_canvas_points_new (2);
	GnomeCanvasGroup *group;
	GnomeCanvasItem *item1, *item2;
	

	// Puntos del orgien
	double I1x1;
	double I1x2;
	double I1y1;
	double I1y2;
				
//	gnome_canvas_item_get_bounds (origen,&I1x1,&I1y1,&I1x2,&I1y2);
	g_object_get (G_OBJECT (origen), "x1", &I1x1, NULL);
	g_object_get (G_OBJECT (origen), "y1", &I1y1, NULL);
	g_object_get (G_OBJECT (origen), "x2", &I1x2, NULL);
	g_object_get (G_OBJECT (origen), "y2", &I1y2, NULL);

	printf("origen, x1: %f, y1: %f, x2: %f, y2: %f \n", I1x1, I1y1, I1x2, I1y2);
				
	// Resolver desequilibrio //
			
	I1x1 += 1.5;
	I1y1 += 1.5;
	I1x2 -= 1.5;
	I1y2 -= 1.5;
	
	// Puntos del destino
	double I2x1;
	double I2x2;
	double I2y1;
	double I2y2;
				
//	gnome_canvas_item_get_bounds (destino,&I2x1,&I2y1,&I2x2,&I2y2);
	g_object_get (G_OBJECT (destino), "x1", &I2x1, NULL);
	g_object_get (G_OBJECT (destino), "y1", &I2y1, NULL);
	g_object_get (G_OBJECT (destino), "x2", &I2x2, NULL);
	g_object_get (G_OBJECT (destino), "y2", &I2y2, NULL);
	printf("destino, x1: %f, y1: %f, x2: %f, y2: %f \n", I2x1, I2y1, I2x2, I2y2);
				
	// Resolver desequilibrio //
			
	I2x1 += 1.5;
	I2y1 += 1.5;
	I2x2 -= 1.5;
	I2y2 -= 1.5;

	//Punto medio circulo origen
	double Xm1 = ((I1x2 + I1x1) / 2);
	double Ym1 = ((I1y2 + I1y1) / 2);
	punto pCirculoOrigen = crear_punto (Xm1, Ym1);

	//Punto medio circulo destino
	double Xm2 = ((I2x2 + I2x1) / 2);
	double Ym2 = ((I2y2 + I2y1) / 2);
	punto pCirculoDestino = crear_punto (Xm2, Ym2);

	// Ecuacion Recta Ax + By + C =0 
	recta recta_origen_destino = crear_recta (pCirculoOrigen, pCirculoDestino);

	// Ecuacion Recta Perpendicular que pasa por el punto medio del circulo origen
	recta recta_perpendicular_circulo_origen = recta_perpendicular (recta_origen_destino, pCirculoOrigen);


	// Ecuacion Recta Paralela1	d=(ax+by+c)/(a^2 + b^2)^1/2
	recta recta_paralela1 = recta_paralela (recta_perpendicular_circulo_origen, pCirculoOrigen, -20);

	// Intersecciones 1 entre recta perpendicular y paralela
	double YI1;
	double XI1;

	punto pInterseccion = interseccion_rectas (recta_origen_destino, recta_paralela1);
	punto_get_values (pInterseccion, &XI1, &YI1);

	// Ecuacion Recta Perpendicular que pasa por el punto medio del circulo destino
	recta recta_perpendicular_circulo_destino = recta_perpendicular (recta_origen_destino, pCirculoDestino);

	// Ecuacion Recta Paralela2
	recta_paralela1 = recta_paralela (recta_perpendicular_circulo_origen, pCirculoDestino, 20);

	// Intersecciones 2
	double YI2;
	double XI2;
	pInterseccion = interseccion_rectas (recta_origen_destino, recta_paralela1);
	punto_get_values (pInterseccion, &XI2, &YI2);

	GnomeCanvas *canvas = origen->canvas;

	group = GNOME_CANVAS_GROUP (gnome_canvas_item_new (GNOME_CANVAS_GROUP(canvas->root),  
                                        gnome_canvas_group_get_type (),
                                        "x", 0,
                                        "y", 0,
                                        NULL));
	double pMedioX, pMedioY;
	// Punto medio de la recta que uno los estados
	punto pMedioRecta = punto_medio (pCirculoOrigen, pCirculoDestino);
	punto_get_values (pMedioRecta, &pMedioX, &pMedioY);

	pointsPoligono->coords[0] = XI1;
	pointsPoligono->coords[1] = YI1;
	pointsPoligono->coords[2] = pMedioX;
	pointsPoligono->coords[3] = pMedioY;

	item1 = gnome_canvas_item_new(group, gnome_canvas_line_get_type (),
                	"points", pointsPoligono,
			"fill_color", "orange", 
			"width_units", 1.0,
			NULL);

	pointsPoligono->coords[0] = pMedioX;
	pointsPoligono->coords[1] = pMedioY;
	pointsPoligono->coords[2] = XI2;
	pointsPoligono->coords[3] = YI2;


	item2 = gnome_canvas_item_new(group, gnome_canvas_line_get_type (),
                	"points", pointsPoligono,
			"fill_color", "orange", 
			"width_units", 1.0,  
            "last_arrowhead", TRUE,
			"arrow_shape_a", 8.0, 
			"arrow_shape_b", 8.0,
			"arrow_shape_c", 8.0, 
			NULL);

	gnome_canvas_points_free (pointsPoligono);

	printf("pMedioX: %f, pMedioY: %f \n", pMedioX, pMedioY);

	create_drag_box (group, (char *)"box", pMedioX, pMedioY, G_CALLBACK (highlight_box));

	return GNOME_CANVAS_ITEM(group);
	

}


/* Llamada para la señales de eventos de los items del canvas. Si el
 * usuario arrastra el item con el botón 1, esté se moverá en consecuencia.
 * Si el usuario hace doble click sobre el item, su color cambiará
 * aleatoriamente. Si el usuario pulsa el botón 3 sobre un item, entoces 
 * el item será destruido. 
 * Cuando el puntero del ratón entra en un item, su ancho de linea se
 * establece a 3 unidades.
 * Cuando el puntero del ratón abandona un item, su ancho de linea se 
 * reestablece a 1 unidad.
 */


gint
item_event (GnomeCanvasItem *item_group, GdkEvent *event, gpointer data)
{
    cout << "item_event" << endl;
	//static double x, y; /* used to keep track of motion coordinates */
	//double new_x, new_y;
	
	GdkCursor *fleur; /* Usado para cambiar el dibujo del cursor */

	GnomeCanvasItem *item, *item2, *item_transicion;
	GnomeCanvasGroup *group;
	GtkWidget *ventana;
	
	list<tTransicion>::iterator posLinea;
	list<tNodo>::iterator posNodo;
	list<tSubAut>::iterator posNodoSub;
	list<GnomeCanvasItem *>::iterator posAdy;
	list<GnomeCanvasItem *>::iterator posAux;

	int idHijo_aux;

	static gboolean dragging = FALSE;
	double event_x = event->button.x;
	double event_y = event->button.y;
	static double previous_x, previous_y;

	int idPath=0;
				
	aux ln;

	list <aux> ListaAux;
	
	list<aux>::iterator posRec;
	
	item = (GnomeCanvasItem *)g_list_nth_data (GNOME_CANVAS_GROUP(item_group)->item_list, 0);  

	switch (event->type) {

	case GDK_2BUTTON_PRESS:
		/*if ((event->button.button == 1) & (botonPulsado == "Editar")){
			
			// Cambia el color del item
			change_item_color (item,0);

			ventana = comprobar_ventana_abierta (item, EDITAR);
		
			if (ventana != NULL) 
				gtk_window_present (GTK_WINDOW(ventana));
			else{
	    			ventana = new_text_windows (item ,(gchar*)"estado");
				add_ventana (ventana, item, EDITAR);
			}
			
			return TRUE;
		}*/
		printf("Doble click en item.\n");

		posNodo = ListaElementos.begin();
	
		while (posNodo->item != item and posNodo != ListaElementos.end())
		{
			posNodo++;
			idPath++;
		}

		idHijo_aux = posNodo->idHijo;


		posNodoSub = ListaSubAutomatas.begin();
		
		while (posNodoSub->idSub != subautomata_mostrado and posNodoSub != ListaSubAutomatas.end())
			posNodoSub++;

		/* Gestion para navegacion en el tree view */

		if (posNodoSub->idPadre == 0){	
			printf("idPath: %d \n", idPath);	
			gtk_tree_path_free (arbolPath);	
			arbolPath = gtk_tree_path_new_from_string ((const char*)int2string(idPath).c_str());
			printf("Path con padre = 0 -- %s \n", gtk_tree_path_to_string (arbolPath));
		}
		else{
			gtk_tree_path_append_index (arbolPath, idPath);
			printf("Path con padre <> 0 -- %s \n", gtk_tree_path_to_string (arbolPath));
		}		

		/* FIN Gestion para navegacion en el tree view */

		if (idHijo_aux != 0){

			gnome_canvas_item_set (item,			       
			"fill_color", "green", // 0x00ff66ff,
	     		NULL);
			gnome_canvas_update_now (GNOME_CANVAS(canvas));

			posNodoSub = ListaSubAutomatas.begin();
		
			while (posNodoSub->idSub != subautomata_mostrado and posNodoSub != ListaSubAutomatas.end())
				posNodoSub++;
			
			//Guardamos el nivel actual
			posNodoSub->ListaElementosSub = ListaElementos;		
			posNodoSub->ListaTransicionesSub = ListaTransiciones;
			posNodoSub->tiempoIteracionSub = tiempoIteracion;
			posNodoSub->variablesSub = variables;
			posNodoSub->funcionesSub = funciones;
			posNodoSub->impSub = imp;

			ocultar_subautomata(subautomata_mostrado);

			posNodoSub = ListaSubAutomatas.begin();
		
			while (posNodoSub->idSub != idHijo_aux and posNodoSub != ListaSubAutomatas.end())
				posNodoSub++;

			mostrar_subautomata(posNodo->idHijo);

			ListaElementos.clear();
			ListaTransiciones.clear();

			//Cargamos el nivel que vamos a mostrar
			ListaElementos = posNodoSub->ListaElementosSub;		
			ListaTransiciones = posNodoSub->ListaTransicionesSub;
			tiempoIteracion = posNodoSub->tiempoIteracionSub;
			variables = posNodoSub->variablesSub;
			funciones = posNodoSub->funcionesSub;
			imp = posNodoSub->impSub;			
			
		}
		else{
			
			posNodo->idHijo = idSubGlobal;
			printf("id del hijo creado : %d \n", posNodo->idHijo);

			gnome_canvas_item_set (item,			       
			"fill_color", "green", // 0x00ff66ff,
	     		NULL);
			gnome_canvas_update_now (GNOME_CANVAS(canvas));

			posNodoSub = ListaSubAutomatas.begin();
		
			while (posNodoSub->idSub != subautomata_mostrado and posNodoSub != ListaSubAutomatas.end())
				posNodoSub++;

			//Guardamos el nivel actual
			posNodoSub->ListaElementosSub = ListaElementos;		
			posNodoSub->ListaTransicionesSub = ListaTransiciones;
			posNodoSub->tiempoIteracionSub = tiempoIteracion;
			posNodoSub->variablesSub = variables;
			posNodoSub->funcionesSub = funciones;
			posNodoSub->impSub = imp;
			ocultar_subautomata(subautomata_mostrado);

			ListaElementos.clear();
			ListaTransiciones.clear();

			tSubAut sub_aux;
			
			sub_aux.ListaElementosSub = ListaElementos;
			sub_aux.ListaTransicionesSub = ListaTransiciones;
			sub_aux.tiempoIteracionSub = 100;	
			sub_aux.variablesSub = "";
			sub_aux.funcionesSub = "";
			sub_aux.impSub.laser= FALSE;
			sub_aux.impSub.motor= FALSE;
			sub_aux.impSub.radar= FALSE;
			sub_aux.impSub.encoders= FALSE;
			sub_aux.impSub.lat_lon= FALSE;
			sub_aux.impSub.camara= FALSE;
			sub_aux.impSub.ptencoders= FALSE;
			sub_aux.idSub = idSubGlobal;
			sub_aux.idPadre = subautomata_mostrado;

			ListaSubAutomatas.push_back(sub_aux);

			posNodoSub = ListaSubAutomatas.begin();
		
			while (posNodoSub->idSub != idSubGlobal and posNodoSub != ListaSubAutomatas.end())
				posNodoSub++;

			//Cargamos el nivel que vamos a mostrar
			ListaElementos = posNodoSub->ListaElementosSub;		
			ListaTransiciones = posNodoSub->ListaTransicionesSub;
			tiempoIteracion = posNodoSub->tiempoIteracionSub;
			variables = posNodoSub->variablesSub;
			funciones = posNodoSub->funcionesSub;
			imp = posNodoSub->impSub;
			//mostrar_subautomata(idSubGlobal);
			subautomata_mostrado = idSubGlobal;
			
			idSubGlobal++;
		}

		actualizar_tree_view();
		gtk_tree_view_expand_to_path(GTK_TREE_VIEW(tree_view), arbolPath);

		break;

	case GDK_BUTTON_RELEASE:

		 //if ((event->button.button == 1) && dragging /*&& (botonPulsado == "Mover")*/) 
			 gnome_canvas_item_ungrab (item, event->button.time);
			 dragging = FALSE;
		
		break;

	case GDK_BUTTON_PRESS:

		dragging = TRUE;
		previous_x = event_x;
		previous_y = event_y;
		
		if ((event->button.button == 1) && (botonPulsado == "EstadoInicial")) {

			marcar_estado_inicial (item);
		}
		/*Pintamos Linea*/
		else if ((event->button.button == 1) && (botonPulsado == "Linea")) {
			
			if (item_transicion_saved == NULL){

				item_transicion_saved=item;		//Si es en el primer item que pulsamos lo guardamos
				
				break;
			}
			else if(item != item_transicion_saved)		//Si es en el segundo item que pulsamos y es distinto que el primero
			{		
				/* Cogemos los dos item pulsados, congemos sus puntos (coordenadas)
				   miranos que puntos tienen menor distancia.
				   pintamos una recta con esos puntos
				*/
			
				item2 = pinta_transicion (item_transicion_saved, item);
				
			}else if(item == item_transicion_saved){
			
				item2 = pinta_autotransicion (item);			
				
			}
			gnome_canvas_update_now (GNOME_CANVAS(canvas));

			g_object_set_data (G_OBJECT (item2), "id", (void*)(int)item2);

			// Guardamos que elementos conecta la linea
			tTransicion l;
			l.item = item2;
			l.origen = item_transicion_saved;
			l.destino = item;
			l.codigo = codigo;
			l.tiempo = tiempo;
			l.item_nombre = NULL;
			l.nombre = "";
			ListaTransiciones.push_back(l);
	

			// Guardamos en los elementos, las lineas que tienen conectadas.
			list<tNodo>::iterator pos;
	
			pos = ListaElementos.begin();

			while(pos->item != item_transicion_saved)
				pos++;
			pos->listaAdyacentes.push_back(item2);
		
			if(item != item_transicion_saved){
				pos = ListaElementos.begin();

				while(pos->item != item)
					pos++;
		
				pos->listaAdyacentes.push_back(item2);
			}
			
			item_transicion_saved = NULL;

		}
		else if ((event->button.button == 1) && (botonPulsado == "Nombrar")) {
			

			change_item_color (item,0);
	
			ventana = comprobar_ventana_abierta (item, NOMBRAR);
		
			if (ventana != NULL) 
				gtk_window_present (GTK_WINDOW(ventana));
			else{
	    		ventana = new_name_windows (item);
				add_ventana (ventana, item, NOMBRAR);
			}
			
			return TRUE;

		}else if ((event->button.button == 1) && (botonPulsado == "Copiar")) {
		
			
			cout << "Copiando: " << endl;
			
			double x1,x2,y1,y2;

			g_object_get (G_OBJECT (item), "x1", &x1, NULL);
			g_object_get (G_OBJECT (item), "y1", &y1, NULL);
			g_object_get (G_OBJECT (item), "x2", &x2, NULL);
			g_object_get (G_OBJECT (item), "y2", &y2, NULL);
			
			group = GNOME_CANVAS_GROUP (gnome_canvas_item_new (root,  
                                        gnome_canvas_group_get_type (),
                                        "x", 0,
                                        "y", 0,
                                        NULL));
	
			item2 = gnome_canvas_item_new(group,
                        	gnome_canvas_ellipse_get_type(),
                        	"x1", x1+50,
                        	"x2", x2+50,
                        	"y1", y1,
                        	"y2", y2,
                        	"fill_color_rgba", 0x00ffffff,
                        	"outline_color", "black",
                        	"width_units", 1.0,
                        	NULL);

			copiar_nodo (item, item2);

			g_signal_connect (group, "event",
			    (GtkSignalFunc) item_event,
			    NULL);


			return TRUE;

		} else if ((event->button.button == 3) & (botonPulsado == "Eliminar")){
			
			/* Destruye el item */			
			
			// Eliminar Transiciones
			posNodo = ListaElementos.begin();

			while((posNodo->item != item) & ( posNodo != ListaElementos.end() ))
  				posNodo++;

			
			if (posNodo != ListaElementos.end()) {
			
				posAdy = posNodo->listaAdyacentes.begin();

				while(posAdy != (posNodo->listaAdyacentes.end()))
				{

					posAux = posAdy;			

					posAdy++;

					eliminar_nombre(*posAux);
					t_eliminar_transicion_lista (*posAux);	

					gtk_object_destroy (GTK_OBJECT (*posAux));	
  				
				}

 				ListaElementos.erase(posNodo);

				cout << (int)*(posNodo->listaAdyacentes.begin()) << endl;

				gtk_object_destroy (GTK_OBJECT (item_group));
			}

			

			cout << "Eliminado: " << endl;
			return TRUE;
		} 

		else if ((event->button.button == 1) & (botonPulsado == "Editar")){//********************añadido evento editar con click simple
			
			// Cambia el color del item
			change_item_color (item,0);

			ventana = comprobar_ventana_abierta (item, EDITAR);
		
			if (ventana != NULL) 
				gtk_window_present (GTK_WINDOW(ventana));
			else{
	    		ventana = new_text_windows (item ,(gchar*)"estado");
				add_ventana (ventana, item, EDITAR);
			}
			
			return TRUE;
		}

		else if (event->button.button == 3) { //*******************************añadido menu contextual con boton derecho.
			
			printf("Entra con boton derecho estado\n");

			gtk_menu_popup(GTK_MENU(menu_estado),NULL,NULL,NULL,NULL,event->button.button,event->button.time);

			item_menu_estado = item;
			item_group_menu_estado = item_group;

			return TRUE;
		} 
		break;

	
	case GDK_MOTION_NOTIFY:
		
		//if (botonPulsado == "Mover"){
		if (estado_nombrado){

			actualizar_tree_view();
			
			if (subautomata_mostrado != 1)
				gtk_tree_view_expand_to_path(GTK_TREE_VIEW(tree_view), arbolPath);
			
			estado_nombrado=FALSE;
		}

			if ((event->motion.state & GDK_BUTTON1_MASK) && dragging) { // Si está pulsado el botón 1
				//Para eliminar el efecto de cualquier posible boton puslado

				botonPulsado = "Nada";
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(botonEstado),FALSE);
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(botonLinea),FALSE);

				/* cambiar la imagen del cursor*/
				fleur = gdk_cursor_new (GDK_FLEUR);
				/*gnome_canvas_item_grab (item,
                              		GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK,
                              		fleur,
                              		event->button.time);*/
   				gnome_canvas_item_grab(item,GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK, fleur, event->button.time);				
            			gdk_cursor_unref (fleur);



				/*gnome_canvas_item_move(item, event_x - previous_x,
        			event_y - previous_y);*/

				/* Obten la nueva posición y muevete por la
				 * diferencia  */
				
				/*new_x = event->motion.x;
				new_y = event->motion.y;*/

				double x1,x2,y1,y2;
				g_object_get (G_OBJECT (item), "x1", &x1, NULL);
				g_object_get (G_OBJECT (item), "y1", &y1, NULL);
				g_object_get (G_OBJECT (item), "x2", &x2, NULL);
				g_object_get (G_OBJECT (item), "y2", &y2, NULL);

				/*cout << "Puntos item: " << x1 << "," << y1 << "," << x2 << "," << y2 <<endl;
				
				gnome_canvas_item_set (item,
							"x1", x1 + (new_x - x),
                         				"y1", y1 + (new_y - y),
                         				"x2", x2 + (new_x - x),
                         				"y2", y2 + (new_y - y),
                        				NULL);*/

				gnome_canvas_item_set (item,
							"x1", x1 + (event_x - previous_x),
                         				"y1", y1 + (event_y - previous_y),
                         				"x2", x2 + (event_x- previous_x),
                         				"y2", y2 + (event_y- previous_y),
                        				NULL);

			
				/* Mover Item_Nombre Estado */
				posNodo = ListaElementos.begin();

				while((posNodo->item != item))
				{
  					posNodo++;
				}
				
				if ( posNodo->item_nombre != NULL )
					gnome_canvas_item_move ((posNodo->item_nombre), event_x - previous_x, event_y - previous_y);

				if ( posNodo->estado_inicial != NULL )
					gnome_canvas_item_move ((posNodo->estado_inicial), event_x - previous_x, event_y - previous_y);
			
					
				/* Repintar Transiciones */
				posAdy = posNodo->listaAdyacentes.begin();

				
				while(posAdy != (posNodo->listaAdyacentes.end()))
				{
					
					posLinea = ListaTransiciones.begin();

					while(posLinea->item != (*posAdy))
  						posLinea++;
								
				
					posAdy++;

					GnomeCanvasItem * origen;

					GnomeCanvasItem * destino;
					
					if ( posLinea->origen == posLinea->destino ){
						gnome_canvas_item_move ((posLinea->item), event_x - previous_x, event_y - previous_y);
						if (posLinea->nombre.compare("") != 0)
							gnome_canvas_item_move ((posLinea->item_nombre), event_x - previous_x, event_y - previous_y);
					}	
					else if ( posLinea->origen == item)
					{	

						origen = item;
						destino = posLinea->destino;

						item_transicion = (GnomeCanvasItem *) repintar_transicion_nodo (root, posLinea->item, item, (char *)"origen");
				
					} 
					else
					{
						origen = posLinea->origen;
						destino = item;
						item_transicion = (GnomeCanvasItem *) repintar_transicion_nodo (root, posLinea->item, item, (char *)"destino");
					}
					
				}

				/* Actualizar Posición */
				//x = new_x;
				//y = new_y;

				previous_x = event_x;
      			previous_y = event_y;
				
				return TRUE;
			}

		//}
		break;

	case GDK_ENTER_NOTIFY:
		/* Establece la linea ancha */	
		gnome_canvas_item_set (item,
			       "width_units", 3.0,
			       NULL);
		return TRUE;

	case GDK_LEAVE_NOTIFY:
		/* Establece la linea estrecha */
		gnome_canvas_item_set (item,
			       "width_units", 1.0,
			       NULL);
		return TRUE;

	default:
		break;
	}

	return FALSE;
}


void on_menu_estado_nombrar(){

    cout << "on_menu_estado_nombrar" << endl;

	GtkWidget *ventana;

	change_item_color (item_menu_estado,0);

	ventana = comprobar_ventana_abierta (item_menu_estado, NOMBRAR);

	if (ventana != NULL) 
	{
		gtk_window_present (GTK_WINDOW(ventana));
	}
	else{
		ventana = new_name_windows (item_menu_estado);
		add_ventana (ventana, item_menu_estado, NOMBRAR);
	}	

	estado_nombrado = TRUE;	

}

void 
on_menu_estado_editar(){

    cout << "on_menu_estado_editar" << endl;

	GtkWidget *ventana;

	change_item_color (item_menu_estado,0);

	ventana = comprobar_ventana_abierta (item_menu_estado, EDITAR);

	if (ventana != NULL) 
		gtk_window_present (GTK_WINDOW(ventana));
	else{
		ventana = new_text_windows (item_menu_estado ,(gchar*)"estado");
		add_ventana (ventana, item_menu_estado, EDITAR);
	}

}

void 
on_menu_estado_marcar_inicial(){
    cout << "on_menu_estado_marcar_inicial" << endl;

	marcar_estado_inicial (item_menu_estado);

}

void 
on_menu_estado_copiar(){

cout << "on_menu_estado_copiar" << endl;

	list<tNodo>::iterator posNodo;

	posNodo = ListaElementos.begin();

	while((posNodo->item != item_menu_estado) & ( posNodo != ListaElementos.end() ))
		posNodo++;

	estado_clipboard = TRUE;
	estado_clipboard_codigo = posNodo->codigo;
	estado_clipboard_idHijo = posNodo->idHijo;

}

void 
on_menu_estado_eliminar(){

    cout << "on_menu_estado_eliminar" << endl;
	
	list<tNodo>::iterator posNodo;
	list<GnomeCanvasItem *>::iterator posAdy;
	list<GnomeCanvasItem *>::iterator posAux;


	/* Destruye el item */			
			
	// Eliminar Transiciones
	posNodo = ListaElementos.begin();

	while((posNodo->item != item_menu_estado) & ( posNodo != ListaElementos.end() ))
		posNodo++;

	
	if (posNodo != ListaElementos.end()) {
	
		posAdy = posNodo->listaAdyacentes.begin();

		while(posAdy != (posNodo->listaAdyacentes.end()))
		{

			posAux = posAdy;			

			posAdy++;

			eliminar_nombre(*posAux);
			t_eliminar_transicion_lista (*posAux);	

			gtk_object_destroy (GTK_OBJECT (*posAux));	
		
		}

		if (posNodo->idHijo != 0){
			borrar_subautomata_recursivo(posNodo->idHijo);
		}

		ListaElementos.erase(posNodo);

		cout << (int)*(posNodo->listaAdyacentes.begin()) << endl;

		gtk_object_destroy (GTK_OBJECT (item_group_menu_estado));
	}

	

	cout << "Eliminado: " << endl;

	actualizar_tree_view();
		
	gtk_tree_view_expand_to_path(GTK_TREE_VIEW(tree_view), arbolPath);

}


void 
on_menu_transicion_nombrar(){

    cout << "on_menu_estado_nombrar" << endl;

	GtkWidget *ventana;

	change_item_color (item_menu_transicion,0);

	ventana = comprobar_ventana_abierta (item_menu_transicion->parent, NOMBRAR);

	if (ventana != NULL) 
		gtk_window_present (GTK_WINDOW(ventana));
	else{
		ventana = new_name_windows (item_menu_transicion->parent);
		add_ventana (ventana, item_menu_transicion->parent, NOMBRAR);
	}

}


void 
on_menu_transicion_editar(){
    cout << "on_menu_transicion_editar" << endl;

	GtkWidget *ventana;

	change_item_color (item_menu_transicion,0);

	ventana = comprobar_ventana_abierta (item_menu_transicion->parent, EDITAR);

	if (ventana != NULL) 
		gtk_window_present (GTK_WINDOW(ventana));
	else{
		ventana = new_text_windows (item_menu_transicion->parent ,(gchar*)"transicion");
		add_ventana (ventana, item_menu_transicion->parent, EDITAR);
	}

}


void 
on_menu_transicion_eliminar(){

    cout << "on_menu_transicion_eliminar" << endl;

	GnomeCanvasGroup *group_parent;

	group_parent = GNOME_CANVAS_GROUP(item_menu_transicion->parent);

	eliminar_nombre(GNOME_CANVAS_ITEM(group_parent));

	t_eliminar_transicion_lista (GNOME_CANVAS_ITEM(group_parent));
	
	gtk_object_destroy (GTK_OBJECT (group_parent));

}


void 
reemplazar_nodo_subautomata(int idSub, GnomeCanvasItem *item_buscar, GnomeCanvasItem* item_nuevo){

    cout << "reemplazar_nodo_subautomata" << endl;

	list<tSubAut>::iterator posNodoSub;
	list<tNodo>::iterator posNodo;
	list<tTransicion>::iterator posTrans;
	

	posNodoSub = ListaSubAutomatas.begin();

	//Nos posicionamos en el subautomata en el que queremos hacer el cambio
	while(posNodoSub->idSub != idSub)
	{
		posNodoSub++;
	}

	posNodo = posNodoSub->ListaElementosSub.begin();

	while(posNodo!= posNodoSub->ListaElementosSub.end())
	{
		if(posNodo->item == item_buscar){
			posNodo->item = item_nuevo;
		}

		posNodo++;

	}

	posTrans = posNodoSub->ListaTransicionesSub.begin();

	while(posTrans!= posNodoSub->ListaTransicionesSub.end())
	{
		if(posTrans->origen == item_buscar){
			posTrans->origen = item_nuevo;
		}

		if(posTrans->destino == item_buscar){
			posTrans->destino = item_nuevo;
		}

		posTrans++;
	}
}


void 
reemplazar_nombre_nodo_subautomata(int idSub, GnomeCanvasItem* nombre_buscar, GnomeCanvasItem* nombre_nuevo){
    cout << "reemplazar_nombre_nodo_subautomata" << endl;

	list<tSubAut>::iterator posNodoSub;
	list<tNodo>::iterator posNodo;

	posNodoSub = ListaSubAutomatas.begin();

	//Nos posicionamos en el subautomata en el que queremos hacer el cambio
	while(posNodoSub->idSub != idSub)
	{
		posNodoSub++;
	}

	posNodo = posNodoSub->ListaElementosSub.begin();

	while(posNodo!= posNodoSub->ListaElementosSub.end())
	{
		if(posNodo->item_nombre == nombre_buscar){
			posNodo->item_nombre = nombre_nuevo;
		}
		posNodo++;
	}
}


void
reemplazar_estado_inicial_subautomata(int idSub, GnomeCanvasItem* inicial_buscar, GnomeCanvasItem* inicial_nuevo){
    cout << "reemplazar_estado_inicial_subautomata" << endl;

	list<tSubAut>::iterator posNodoSub;
	list<tNodo>::iterator posNodo;

	posNodoSub = ListaSubAutomatas.begin();

	//Nos posicionamos en el subautomata en el que queremos hacer el cambio
	while(posNodoSub->idSub != idSub)
	{
		posNodoSub++;
	}

	posNodo = posNodoSub->ListaElementosSub.begin();

	while(posNodo!= posNodoSub->ListaElementosSub.end())
	{
		if(posNodo->estado_inicial == inicial_buscar){
			posNodo->estado_inicial = inicial_nuevo;
		}
		posNodo++;
	}
}


void
reemplazar_transicion_subautomata(int idSub, GnomeCanvasItem* trans_buscar, GnomeCanvasItem* trans_nueva){
    cout << "reemplazar_transicion_subautomata" << endl;

	list<tSubAut>::iterator posNodoSub;
	list<tNodo>::iterator posNodo;
	list<tTransicion>::iterator posTrans;
	list<GnomeCanvasItem*>::iterator posAdy;

	posNodoSub = ListaSubAutomatas.begin();

	//Nos posicionamos en el subautomata en el que queremos hacer el cambio
	while(posNodoSub->idSub != idSub)
	{
		posNodoSub++;
	}

	//Sustituimos la transicion en las listas de adyacentes de los estados
	posNodo = posNodoSub->ListaElementosSub.begin();

	while(posNodo != posNodoSub->ListaElementosSub.end()){
		
		posAdy = posNodo->listaAdyacentes.begin();
		while(posAdy != posNodo->listaAdyacentes.end()){
			
			if(*posAdy == trans_buscar){
				*posAdy = trans_nueva;
			}
			posAdy++;
		}

		posNodo++;
	}

	//Sustituimos la transicion en la lista de transiciones del subautomata
	posTrans = posNodoSub->ListaTransicionesSub.begin();

	while(posTrans != posNodoSub->ListaTransicionesSub.end()){
		
		if(posTrans->item == trans_buscar){
			posTrans->item = trans_nueva;
		}
		posTrans++;
	}
}


void
reemplazar_nombre_trans_subautomata(int idSub, GnomeCanvasItem* nombre_buscar, GnomeCanvasItem* nombre_nuevo){
    cout << "reemplazar_nombre_trans_subautomata" << endl;

	list<tSubAut>::iterator posNodoSub;
	list<tTransicion>::iterator posTrans;

	posNodoSub = ListaSubAutomatas.begin();

	//Nos posicionamos en el subautomata en el que queremos hacer el cambio
	while(posNodoSub->idSub != idSub)
	{
		posNodoSub++;
	}

	posTrans = posNodoSub->ListaTransicionesSub.begin();

	while(posTrans != posNodoSub->ListaTransicionesSub.end()){

		if(posTrans->item_nombre == nombre_buscar){
			posTrans->item_nombre = nombre_nuevo;
		}

		posTrans++;

	}

}


void pegado_recursivo(int sub_a_copiar, int sub_padre){
    cout << "pegado_recursivo" << endl;

	tSubAut nuevo_sub;
	list<tSubAut>::iterator posNodoSub;
	list<tNodo>::iterator posNodo;
	list<tTransicion>::iterator posTrans;

	GnomeCanvasItem *item, *item_nombre, *inicial; 
	GnomeCanvasGroup *group;
	
	//Para obtener el box de una transicion y poder copiar el nombre en caso de haberlo
	GnomeCanvasItem *box;
	GList * list_items;

	//estas variables son necesarias si tenemos que llamar a pegado_recursivo mas veces
	int padre = 0;
	int hijo = 0;

	double x1, y1, x2, y2;

	posNodoSub = ListaSubAutomatas.begin();

	while(posNodoSub->idSub != sub_a_copiar)
	{
		posNodoSub++;
	}

	//Asi copiamos las listas
	list <tNodo> listE (posNodoSub->ListaElementosSub);
	list <tTransicion> listT( posNodoSub->ListaTransicionesSub);

	nuevo_sub.ListaElementosSub = listE;
	nuevo_sub.ListaTransicionesSub = listT;
	nuevo_sub.tiempoIteracionSub = posNodoSub->tiempoIteracionSub;
	nuevo_sub.variablesSub = posNodoSub->variablesSub;
	nuevo_sub.funcionesSub = posNodoSub->funcionesSub;
	nuevo_sub.impSub = posNodoSub->impSub;
	nuevo_sub.idSub = idSubGlobal;
	nuevo_sub.idPadre = sub_padre;

	ListaSubAutomatas.push_back(nuevo_sub);

	posNodoSub = ListaSubAutomatas.begin();

	while(posNodoSub->idSub != idSubGlobal)
	{
		posNodoSub++;
	}

	posNodo = posNodoSub->ListaElementosSub.begin();

	//Recorremos la lista de elementos para duplicar todos los nodos y asi tener la copia
	//hacemos lo mismo con el item de estado inicial y con el item del nombre si fuera necesario
	while(posNodo != posNodoSub->ListaElementosSub.end())
	{
		get_bounds (posNodo->item, &x1, &y1, &x2, &y2);

		group = GNOME_CANVAS_GROUP (gnome_canvas_item_new (root,  
                                        gnome_canvas_group_get_type (),
                                        "x", 0,
                                        "y", 0,
                                        NULL));

		if (posNodo->idHijo == 0){						

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
		}else{

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

		/*Añadimos control (señal) al item creado*/
		g_signal_connect (group, "event",
			    (GtkSignalFunc) item_event,
			    NULL);

		//Lo ocultamos
		gnome_canvas_item_hide(item);

		//Sustituimos el item copiado por el recien creado en todas las referencias que haya
		//dentro de su subautomata
		reemplazar_nodo_subautomata(posNodoSub->idSub, posNodo->item, item);

		//Repetimos el proceso para el item_nombre y para el estado_inicial
		if (posNodo->item_nombre != NULL){
			item_nombre = gnome_canvas_item_new (group,
		             gnome_canvas_text_get_type (),
	             			            "text", posNodo->nombre.c_str(),
	             			            "x", (x2+x1)/2,
	             			            "y", (y2+y1)/2,
	             			            "font", "Sans 28",
					    	    "anchor", GTK_ANCHOR_CENTER,
	     			                    "fill_color", "black",
	    			                     NULL);

			gnome_canvas_item_raise_to_top (item_nombre);

			gnome_canvas_item_hide(item_nombre);

			reemplazar_nombre_nodo_subautomata(posNodoSub->idSub, posNodo->item_nombre, item_nombre);
		}

		if (posNodo->estado_inicial != NULL){
			inicial = gnome_canvas_item_new(group,
                        gnome_canvas_ellipse_get_type(),
                        "x1", x1 + 5,
                        "y1", y1 + 5,
                        "x2", x2 - 5,
                        "y2", y2 - 5,
                        "fill_color_rgba", NULL,
                        "outline_color", "black",
                        "width_units", 1.0,
                        NULL);

			gnome_canvas_item_hide(inicial);

			reemplazar_estado_inicial_subautomata(posNodoSub->idSub, posNodo->estado_inicial, inicial);
		}
		
		posNodo++;
	}//Terminamos de recorrer la lista de elementos


	posNodoSub = ListaSubAutomatas.begin();

	while(posNodoSub->idSub != idSubGlobal)
	{
		posNodoSub++;
	}

	//Ahora tenemos que hacer un proceso de sustitucion equivalente para las transiciones
	posTrans = posNodoSub->ListaTransicionesSub.begin();

	while(posTrans != posNodoSub->ListaTransicionesSub.end())
	{
		if(posTrans->origen == posTrans->destino){

			item = pinta_autotransicion(posTrans->destino);
			//La ocultamos
			gnome_canvas_item_hide(item);

		}else{

			item = pinta_transicion(posTrans->origen,posTrans->destino);
			//La ocultamos
			gnome_canvas_item_hide(item);

		}	

		reemplazar_transicion_subautomata(posNodoSub->idSub, posTrans->item, item);

		//Repetimos el proceso para el item_nombre
		if (posTrans->item_nombre != NULL){

			list_items = GNOME_CANVAS_GROUP(item)->item_list;
			box = (GnomeCanvasItem *) g_list_nth_data (list_items, g_list_length (list_items)-1);

			get_bounds (box, &x1, &y1, &x2, &y2);

			item_nombre = gnome_canvas_item_new (root,
						gnome_canvas_text_get_type (),
             			            "text", posTrans->nombre.c_str(),
             			            "x", (x2+x1)/2,
             			            "y", (y2+y1)/2 + 5 ,
             			            "font", "Sans 28",
				    	    "anchor", GTK_ANCHOR_N,
     			                    "fill_color", "black",
    			                     NULL);

			gnome_canvas_item_raise_to_top (item_nombre);
			gnome_canvas_item_hide(item_nombre);

			reemplazar_nombre_trans_subautomata(posNodoSub->idSub, posTrans->item_nombre, item_nombre);
		}

		posTrans++;
	}//Terminamos de recorrer la lista de transiciones

	//Terminadas las gestiones en el nivel hay que comprobar si hay más subautómatas hijos que se
	//desplieguen a partir de este nivel
	posNodo = posNodoSub->ListaElementosSub.begin();

	while(posNodo != posNodoSub->ListaElementosSub.end()){

		if (posNodo->idHijo != 0){

			padre = idSubGlobal;

			hijo = posNodo->idHijo;

			idSubGlobal++;

			printf("Entra por pegado recursivo, idsubglobal: %d\n",idSubGlobal);

			//El hijo es este porque ahora llamaremos al proceso de pegado recursivo que creará un
			//subautomata con esta ID
			posNodo->idHijo = idSubGlobal;

			pegado_recursivo(hijo, padre);

			//posNodo->idHijo = padre;
			printf("Entra por pegado recursivo, posNodo->idHijo: %d\n",posNodo->idHijo);

		}

		posNodo++;
	}

	//Aumentamos la variable idSubGlobal por si no se desplegaron hijos en la llamada
	//para que quede con un valor válido
	idSubGlobal++;

	printf("termina un pegado recursivo, idsubglobal: %d\n",idSubGlobal);

}

void
on_menu_pegar(){

    cout << "on_menu_pegar" << endl;

	GnomeCanvasItem *item; 
	GnomeCanvasGroup *group;

	tNodo reg;

	list<tSubAut>::iterator posSub; //Añadido para poder iterar en la lista de subautomatas y actulizar el treeview con cada nuevo estado

	points = gnome_canvas_points_new(2); /* 2 puntos */
	

	printf("menu pegar\n");
	printf("codigo: %s \n", estado_clipboard_codigo.c_str());
	printf("idHijo: %d \n", estado_clipboard_idHijo);
	
	
	points->coords[0]  = origenX;
	points->coords[1]  = origenY;
	points->coords[2]  = origenX;
	points->coords[3]  = origenY;

	printf("Click %d, %d\n", origenX, origenY);
	//printf("Pintamos Nodo \n");

	if (estado_clipboard) 
	{

		printf("Antes de group\n");	
		group = GNOME_CANVAS_GROUP (gnome_canvas_item_new (root,  
                                        gnome_canvas_group_get_type (),
                                        "x", 0,
                                        "y", 0,
                                        NULL));
		printf("despues de group\n");	
		if (estado_clipboard_idHijo == 0){
			
			printf("entra por el if\n");	

			item = gnome_canvas_item_new(group,
							gnome_canvas_ellipse_get_type(),
							"x1", (double) origenX-20,
							"y1", (double) origenY-20,
							"x2", (double) (origenX + 20),
							"y2", (double) (origenY + 20),
							"fill_color_rgba", 0x00ffffff,
							"outline_color", "black",
							"width_units", 1.0,
							NULL);
			printf("despues de crear item\n");

			reg.idHijo = estado_clipboard_idHijo;
			printf("despues de reg.idHijo\n");

		}else{

			printf("entra por el else\n");	

			item = gnome_canvas_item_new(group,
							gnome_canvas_ellipse_get_type(),
							"x1", (double) origenX-20,
							"y1", (double) origenY-20,
							"x2", (double) (origenX + 20),
							"y2", (double) (origenY + 20),
							"fill_color_rgba", 0x00ff66ff,
							"outline_color", "black",
							"width_units", 1.0,
							NULL);

			//Si el estado que habiamos copiado tenia hijos hay que hacer copia de toda la estructura que enlazase por debajo
			//Asignamos como hijo al nodo copiado la variable idSubGlobal porque ahora llamaremos a una funcion recursiva que empezará
			//creando un nuevo nodo en la lista de subautomatas y lo hará con esa idSubGlobal
			reg.idHijo = idSubGlobal;
			
	
			printf("Antes de llamar a pegado recursivo\n");
			//Las dos variables que el procedimiento necesita son el subautomata a buscar en la lista para poder copiartlo y cual será su padre
			pegado_recursivo(estado_clipboard_idHijo, subautomata_mostrado);
			printf("Despues de llamar a pegado recursivo\n");

		}		

		

		reg.item = item;
		reg.item_nombre = NULL;
		reg.estado_inicial = NULL;
		reg.nombre = "";
		reg.codigo = estado_clipboard_codigo;
		id ++;
	
		ListaElementos.push_back(reg);

		printf("Despues de añadir nuevo nodo en el nivel \n");

		/*Añadimos control (señal) al item creado*/
		g_signal_connect (group, "event",
			    (GtkSignalFunc) item_event,
			    NULL);

		estado_clipboard = FALSE;
		estado_clipboard_codigo = "";
		estado_clipboard_idHijo = 0;


		//Actualizamos datos necesarios para mostrar cambios en el tree view
		posSub = ListaSubAutomatas.begin();

		while(posSub->idSub != subautomata_mostrado)
		{
			posSub++;
		}

		//guardamos el estado de la lista de elementos en el nodo del subautomata para poder recargar bien el tree view
		posSub->ListaElementosSub = ListaElementos;

		actualizar_tree_view();
		
		gtk_tree_view_expand_to_path(GTK_TREE_VIEW(tree_view), arbolPath);		

	}

	printf("Pasa del if\n");

	estado_clipboard = FALSE;
	estado_clipboard_codigo = "";
	estado_clipboard_idHijo = 0;

}

