#include "ventanas.h"

list <nVentana> ListaVentanas;

extern list <tNodo> ListaElementos;
extern list <tTransicion> ListaTransiciones;
extern int color;

extern string variables;
extern string funciones;
extern importar imp;
extern int tiempoIteracion;

extern GnomeCanvasGroup *root;

extern GladeXML *prog;

void destroy (GtkWidget *widget, gpointer data)
{

	gtk_widget_destroy (gtk_widget_get_toplevel(widget));

}

void restaurar_color (GtkWidget *widget, gpointer data)
{
	GnomeCanvasItem *item, *box;
	list<tNodo>::iterator posNodo;

	item = GNOME_CANVAS_ITEM (g_object_get_data (G_OBJECT (gtk_widget_get_toplevel(widget)), "item"));

	if (GNOME_IS_CANVAS_GROUP (item)){
		box = get_box (item);
		//gnome_canvas_item_set (box,			       
		//		"fill_color", NULL,
		//	       	NULL);		
		change_item_color (box,7);			

	}else{

		posNodo = ListaElementos.begin();

		while( (posNodo != ListaElementos.end()) && (posNodo->item != item))
	  		posNodo++;

		if (posNodo->idHijo !=0){
			
			change_item_color (item,2);

		}else{

			change_item_color (item,3);

		}

	}

}


void aplicar (GtkButton *button, gpointer data)
{
	double x1, x2, y1, y2;
	double x,y;

	list<tNodo>::iterator posNodo;
	list<tTransicion>::iterator posTransicion;

	GnomeCanvasItem *item, *box;
	GnomeCanvasItem *item_nombre;

	GList * list_items;

	GtkWidget *entry = GTK_WIDGET (g_object_get_data (G_OBJECT (button), "entry"));
                                           
	const char *texto = gtk_entry_get_text(GTK_ENTRY(entry));

	item = GNOME_CANVAS_ITEM (g_object_get_data (G_OBJECT (button), "item"));
	
	

	//cout << "Centro: " << x << " " << y << endl;
	
	posNodo = ListaElementos.begin();

	while( (posNodo != ListaElementos.end()) && (posNodo->item != item))
  		posNodo++;

	if (posNodo == ListaElementos.end()){

		posTransicion = ListaTransiciones.begin();

		while( (posTransicion != ListaTransiciones.end()) && (posTransicion->item != item) )
			posTransicion++;

		if (posTransicion->nombre != "")	
			gtk_object_destroy (GTK_OBJECT (posTransicion->item_nombre));

		list_items = GNOME_CANVAS_GROUP(item)->item_list;

		box = (GnomeCanvasItem *) g_list_nth_data (list_items, g_list_length (list_items)-1);  

		g_object_get (G_OBJECT (box), "x1", &x1, NULL);
		g_object_get (G_OBJECT (box), "y1", &y1, NULL);
		g_object_get (G_OBJECT (box), "x2", &x2, NULL);
		g_object_get (G_OBJECT (box), "y2", &y2, NULL);

		x = (x2+x1)/2;
		y = (y2+y1)/2;

		item_nombre = gnome_canvas_item_new (root,
	             gnome_canvas_text_get_type (),
             			            "text", texto,
             			            "x", (x2+x1)/2,
             			            "y", (y2+y1)/2 + 5 ,
             			            "font", "Sans 28",
				    	    "anchor", GTK_ANCHOR_N,
     			                    "fill_color", "black",
    			                     NULL); 

		posTransicion->nombre = texto;

		posTransicion->item_nombre = item_nombre;

	}
	else {
		if (posNodo->nombre != "")	
			gtk_object_destroy (GTK_OBJECT (posNodo->item_nombre));

		g_object_get (G_OBJECT (item), "x1", &x1, NULL);
		g_object_get (G_OBJECT (item), "y1", &y1, NULL);
		g_object_get (G_OBJECT (item), "x2", &x2, NULL);
		g_object_get (G_OBJECT (item), "y2", &y2, NULL);
			

		x = (x2+x1)/2;
		y = (y2+y1)/2;

		item_nombre = gnome_canvas_item_new (GNOME_CANVAS_GROUP(item->parent),
		             gnome_canvas_text_get_type (),
	             			            "text", texto,
	             			            "x", (x2+x1)/2,
	             			            "y", (y2+y1)/2,
	             			            "font", "Sans 28",
					    	    "anchor", GTK_ANCHOR_CENTER,
	     			                    "fill_color", "black",
	    			                     NULL); 


		posNodo->nombre = texto;

		posNodo->item_nombre = item_nombre;

	}

	gnome_canvas_item_raise_to_top (item_nombre);

	gtk_widget_destroy (gtk_widget_get_toplevel(GTK_WIDGET(button)));



}


GtkWidget * new_name_windows (GnomeCanvasItem *item)
{
	GladeXML *xml;
	GtkWidget *win; /*Main window*/
	GtkWidget *botonAplicar, *botonCancelar;

	//GladeInterface *inter;

	/*Inicializamos Glade*/
   	glade_init();

	/*Cargamos fichero .Glade donde esta guardada la estructura de la nueva ventana*/
  	xml = glade_xml_new("gui/nombre.glade", NULL, NULL);

	if (xml == NULL)
	{
		printf ("NO");
		return 0;
	}
	
	/*Cargamos la nueva ventana*/
	win = glade_xml_get_widget(xml, "nombre");

	/*Conectamos Señales*/	
	//glade_xml_signal_autoconnect(xml);

	/*Ponemos Titulo a la ventana*/	
	gtk_window_set_title(GTK_WINDOW(win), "Nombrar elemento");


	//xml = glade_get_widget_tree (win);

	/*Cargamos el boton*/	
	botonCancelar = glade_xml_get_widget(xml, "BotonCancelar");	
	botonAplicar = glade_xml_get_widget(xml, "BotonAplicar");
	
	/*Asignamos item*/ 
	g_object_set_data (G_OBJECT (botonAplicar), "item", item);
	g_object_set_data (G_OBJECT (win), "item", item);

	/*Asignamos Señal al boton*/
	//g_signal_connect (G_OBJECT (botonAplicar), "clicked", G_CALLBACK (on_button_clicked), NULL);
	g_signal_connect (G_OBJECT (botonAplicar), "clicked", G_CALLBACK (aplicar), 0);

	//g_signal_connect (G_OBJECT (botonCancelar), "clicked", G_CALLBACK (restaurar_color), 0);

	g_signal_connect (G_OBJECT (botonCancelar), "clicked", G_CALLBACK (destroy), 0);

	g_signal_connect (G_OBJECT (win), "destroy", G_CALLBACK (restaurar_color),0);
	
	g_signal_connect (G_OBJECT (win), "destroy", G_CALLBACK (eliminar_ventana),0);
	

	GtkWidget *entry = glade_xml_get_widget(xml, "entry1");

	g_object_set_data (G_OBJECT (botonAplicar), "entry", entry);


	/*  */
	list<tNodo>::iterator posNodo;
	list<tTransicion>::iterator posTransicion;
	
	posNodo = ListaElementos.begin();

	color = 3;

	while( (posNodo != ListaElementos.end()) && (posNodo->item != item) )
  		posNodo++;

	if (posNodo == ListaElementos.end()){

		posTransicion = ListaTransiciones.begin();

		while( (posTransicion != ListaTransiciones.end()) && (posTransicion->item != item) )
			posTransicion++;

		gtk_entry_set_text(GTK_ENTRY(entry), posTransicion->nombre.c_str());

		color = 7;
	}else
		gtk_entry_set_text(GTK_ENTRY(entry), posNodo->nombre.c_str());



	gtk_widget_show_all(win);

  	
	return win;

}

 


void cambiar_a (GtkWidget *widget, gpointer data){

	GtkWidget *tiempo = GTK_WIDGET (g_object_get_data (G_OBJECT (widget), "frameN"));
	GtkWidget *texto = GTK_WIDGET (g_object_get_data (G_OBJECT (widget), "frameT"));
 	//gint width, height;

	if (strcmp((gchar *)data, "texto")==0){
				
		gtk_widget_show (GTK_WIDGET (texto));

		gtk_widget_hide (GTK_WIDGET (tiempo));

		//width = (gint) (g_object_get_data (G_OBJECT (gtk_widget_get_toplevel(widget)), "width"));
		//height = (gint) (g_object_get_data (G_OBJECT (gtk_widget_get_toplevel(widget)), "height"));

		//gtk_window_resize (GTK_WINDOW(gtk_widget_get_toplevel(widget)),width,height);

	//	//GtkWidget *windowAux = gtk_window_new(GTK_WINDOW_POPUP);

	//	//gtk_widget_set_tooltip_window (GTK_WIDGET (tiempo), GTK_WINDOW(windowAux));
	//	//window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	}
	else{
		gtk_widget_show (GTK_WIDGET (tiempo));

		gtk_widget_hide (GTK_WIDGET (texto));

		//gtk_window_get_size (GTK_WINDOW(gtk_widget_get_toplevel(widget)),&width, &height);

		//g_object_set_data (G_OBJECT (gtk_widget_get_toplevel(widget)), "width", (gpointer)width);
		//g_object_set_data (G_OBJECT (gtk_widget_get_toplevel(widget)), "height", (gpointer)height);

		//gtk_window_resize (GTK_WINDOW(gtk_widget_get_toplevel(widget)),250,1);
	}

}

void aceptar (GtkWidget *widget, gpointer data)
{
	GtkTextIter start, end;
	GtkSourceBuffer *sBuf;
	GnomeCanvasItem *item;
	const char * texto;

	
	if (strcmp((gchar*)data, "variables")==0) {

		
		sBuf = GTK_SOURCE_BUFFER (g_object_get_data (G_OBJECT (widget), "bufferVar"));
		gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER(sBuf), &start);
		gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER(sBuf), &end);
		texto = gtk_text_buffer_get_text (GTK_TEXT_BUFFER(sBuf), &start, &end, true); 	
		variables = texto;

		sBuf = GTK_SOURCE_BUFFER (g_object_get_data (G_OBJECT (widget), "bufferFun"));
		gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER(sBuf), &start);
		gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER(sBuf), &end);
		texto = gtk_text_buffer_get_text (GTK_TEXT_BUFFER(sBuf), &start, &end, true); 
		funciones = texto;
		


	}else{
		item = GNOME_CANVAS_ITEM (g_object_get_data (G_OBJECT (widget), "item"));

		if (strcmp((gchar*)data, "estado")==0) {
			sBuf = GTK_SOURCE_BUFFER (g_object_get_data (G_OBJECT (widget), "buffer"));
			//gtk_text_buffer_get_selection_bounds (textbuffer, &start, &end);

			gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER(sBuf), &start);

			gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER(sBuf), &end);

			texto = gtk_text_buffer_get_text (GTK_TEXT_BUFFER(sBuf), &start, &end, true); 

		

			list<tNodo>::iterator pos;
		
			pos = ListaElementos.begin();

			while( (pos != ListaElementos.end()) & (pos->item != item) )
			{
	  			pos++;
			}
			pos->codigo = texto;
			
			//change_item_color (item, 3);
		}else if (strcmp((gchar*)data, "transicion")==0) {
			list<tTransicion>::iterator pos;
		
			pos = ListaTransiciones.begin();

			while( (pos != ListaTransiciones.end()) & (pos->item != item) )
			{
	  				pos++;
			}

			GtkWidget *toggle_button = GTK_WIDGET (g_object_get_data (G_OBJECT (widget), "rbTiempo"));

			if (GTK_TOGGLE_BUTTON (toggle_button)->active){
				GtkWidget *entry = GTK_WIDGET (g_object_get_data (G_OBJECT (widget), "entryTemp"));
	                                           
				texto = gtk_entry_get_text(GTK_ENTRY(entry));
				
				pos->tiempo = atoi(texto); 
				pos->codigo = "";

			}else{
				GtkWidget *entry = GTK_WIDGET (g_object_get_data (G_OBJECT (widget), "entryCond"));
	                                           
				texto = gtk_entry_get_text(GTK_ENTRY(entry));
				
				pos->codigo = texto;
				pos->tiempo = -1; 
			}

			//change_item_color (get_box (item), 7);
		}
	}
	
	gtk_widget_destroy (gtk_widget_get_toplevel(GTK_WIDGET(widget)));
	//g_signal_emit_by_name (GTK_OBJECT (window), "delete_event");

}

GtkWidget * new_text_windows (GnomeCanvasItem *item, gchar *type)
{
	GladeXML *xml;
	GtkWidget *win;	/*Main window*/
	GtkWidget *botonAceptar, *botonCancelar, *toggle_button_texto, *toggle_button_tiempo, *entry; 

	/*SourveView variables*/
	GtkWidget *pScrollWin, *sView;
 	PangoFontDescription *font_desc;
  	GtkSourceLanguageManager *lm;
  	GtkSourceBuffer *sBuf;
	GtkSourceLanguage *language = NULL;

	//GladeInterface *inter;

	/*Inicializamos Glade*/
   	glade_init();

	/*Cargamos fichero .Glade donde esta guardada la estructura de la nueva ventana*/
	if (strcmp((const char *)type, "transicion") == 0){
		xml=glade_xml_new("gui/EdicionTransicion.glade", NULL, NULL);

		if (xml == NULL)
		{
			printf ("NO");
		return 0;
		}
		/*Conectamos Señales*/	

		win = glade_xml_get_widget(xml, "window1");

		/*Ponemos Titulo a la ventana*/	
		gtk_window_set_title(GTK_WINDOW(win), "Código Transicion ");

		g_signal_connect (G_OBJECT (glade_xml_get_widget(xml,"rbTexto")), "released", G_CALLBACK (cambiar_a), (gpointer)"texto");

		g_object_set_data (G_OBJECT (glade_xml_get_widget(xml,"rbTexto")), "frameN", glade_xml_get_widget(xml, "frameN"));
		g_object_set_data (G_OBJECT (glade_xml_get_widget(xml,"rbTexto")), "frameT", glade_xml_get_widget(xml, "frameT"));

		g_signal_connect (G_OBJECT (glade_xml_get_widget(xml,"rbTiempo")), "released", G_CALLBACK (cambiar_a), (gpointer)"tiempo");

		g_object_set_data (G_OBJECT (glade_xml_get_widget(xml,"rbTiempo")), "frameN", glade_xml_get_widget(xml, "frameN"));
		g_object_set_data (G_OBJECT (glade_xml_get_widget(xml,"rbTiempo")), "frameT", glade_xml_get_widget(xml, "frameT"));
		
	}else {
  		xml=glade_xml_new("gui/EdicionNodo.glade", NULL, NULL);

		if (xml == NULL)
		{
			printf ("NO");
		return 0;
		}
	
		/*Cargamos la nueva ventana*/
		win = glade_xml_get_widget(xml, "dialog1");

		/*Ponemos Titulo a la ventana*/	
		gtk_window_set_title(GTK_WINDOW(win), "Código Estado");
		
		/*Cargamos el boton*/	
		//GtkWidget *numero = glade_xml_get_widget(xml, "button1");

		
	

		/*Conectamos Señales*/	
		glade_xml_signal_autoconnect(xml);

		/*Situamos la ventana en el medio*/
		//gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
	
		/*** Incorporar GtkSourceView ***/

		pScrollWin = glade_xml_get_widget(xml, "scrolledwindow");

		/* Now create a GtkSourceLanguagesManager */
		lm = gtk_source_language_manager_new();

		/* and a GtkSourceBuffer to hold text (similar to GtkTextBuffer) */
		sBuf = GTK_SOURCE_BUFFER (gtk_source_buffer_new (NULL));

		/* Create the GtkSourceView and associate it with the buffer */
		sView = gtk_source_view_new_with_buffer(sBuf);
		gtk_source_view_set_show_line_numbers ((GtkSourceView*)sView, TRUE);
	
		/* Set default Font name,size */
		font_desc = pango_font_description_from_string ("mono 10");
		gtk_widget_modify_font (sView, font_desc);
		pango_font_description_free (font_desc);

		/* get the Language for C source mimetype */
//		language = gtk_source_languages_manager_get_language_from_mime_type (lm,"text/x-c++src");
        language = gtk_source_language_manager_get_language(lm, "text/x-c++src");
		if (language == NULL)
		{
			g_print ("No language found for mime type `%s'\n", "text/x-c++src");
			g_object_set (G_OBJECT (sBuf), "highlight", FALSE, NULL);
		}
		else
		{
			gtk_source_buffer_set_language (sBuf, language);
			g_object_set (G_OBJECT (sBuf), "highlight", TRUE, NULL);
		}
		
		/* Attach the GtkSourceView to the scrolled Window */
		gtk_container_add (GTK_CONTAINER (glade_xml_get_widget(xml, "scrolledwindow")), sView);
		
		
	}
	/* Attach the GtkSourceView to the scrolled Window */
	//if (strcmp((const char *)type, "transicion") == 0)
	//	gtk_container_add (GTK_CONTAINER (glade_xml_get_widget(xml, "frameT")), sView);
	//else
	//	gtk_container_add (GTK_CONTAINER (glade_xml_get_widget(xml, "scrolledwindow")), sView);

	/*Cargamos el boton*/	
	botonCancelar = glade_xml_get_widget(xml, "cancelar");	
	botonAceptar = glade_xml_get_widget(xml,"aceptar");
	
	/*Asignamos elementos*/ 
	g_object_set_data (G_OBJECT (botonAceptar), "item", item);
	g_object_set_data (G_OBJECT (win), "item", item);

	/*Asignamos Señal al boton*/
	g_signal_connect (G_OBJECT (botonAceptar), "clicked", G_CALLBACK (aceptar), type);
	//g_signal_connect (G_OBJECT (botonCancelar), "clicked", G_CALLBACK (restaurar_color), 0);
	g_signal_connect (G_OBJECT (botonCancelar), "clicked", G_CALLBACK (destroy), 0);
	g_signal_connect (G_OBJECT (win), "destroy", G_CALLBACK (restaurar_color),0);
	g_signal_connect (G_OBJECT (win), "destroy", G_CALLBACK (eliminar_ventana),0);

	/* Visualizamo la ventana y establecemos el tamaño de la ventana*/
	//gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
	gtk_widget_show_all(win);
	//gtk_window_resize (GTK_WINDOW(win),640,480);
	//g_object_set_data (G_OBJECT (win), "width", (gpointer)640);
	//g_object_set_data (G_OBJECT (win), "height", (gpointer)480);
	
	//gtk_window_move (GTK_WINDOW(win), (gdk_screen_width ()/2) - (640/2), (gdk_screen_height ()/2) - (480/2));
	/* Cargamos Texto */

	if (strcmp(type, "estado")==0){

		
		list<tNodo>::iterator pos;
	
		pos = ListaElementos.begin();

		while(pos->item != item)
			pos++;
	
		color = 3;

		if (pos->codigo != ""){
			gtk_source_buffer_begin_not_undoable_action (sBuf);
			gtk_text_buffer_set_text (GTK_TEXT_BUFFER(sBuf), pos->codigo.c_str(),  (int)pos->codigo.length());
			gtk_source_buffer_end_not_undoable_action (sBuf);
		}
		
		/*Asignamos elementos*/ 
		g_object_set_data (G_OBJECT (botonAceptar), "buffer", sBuf);
	}
	else if (strcmp(type, "transicion")==0){
		list<tTransicion>::iterator pos;
	
		pos = ListaTransiciones.begin();

		while(pos->item != item)
	  		pos++;

		color = 7;

		GtkWidget *numero = glade_xml_get_widget(xml, "frameN");
		GtkWidget *texto = glade_xml_get_widget(xml, "frameT");

		gtk_widget_show (GTK_WIDGET (texto));
		gtk_widget_hide (GTK_WIDGET (numero));


		toggle_button_tiempo = glade_xml_get_widget(xml, "rbTiempo");
		g_object_set_data (G_OBJECT (botonAceptar), "rbTiempo", toggle_button_tiempo);

		toggle_button_texto = glade_xml_get_widget(xml, "rbTexto");
		g_object_set_data (G_OBJECT (botonAceptar), "rbTexto", toggle_button_texto);

		entry = glade_xml_get_widget(xml, "entry1");
		g_object_set_data (G_OBJECT (botonAceptar), "entryTemp", entry);
		
		entry = glade_xml_get_widget(xml, "entryCond");
		g_object_set_data (G_OBJECT (botonAceptar), "entryCond", entry);

		if (pos->codigo != "")
			gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(xml, "entryCond")), pos->codigo.c_str());

		if (pos->tiempo != -1){

			gtk_widget_show (GTK_WIDGET (numero));
			gtk_widget_hide (GTK_WIDGET (texto));
		
			GTK_TOGGLE_BUTTON (toggle_button_tiempo)->active = true;

			GTK_TOGGLE_BUTTON (toggle_button_texto)->active = false;

			gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(xml, "entry1")), int2string(pos->tiempo).c_str());
			
			gtk_window_resize (GTK_WINDOW(win),250,1);

		}

	}else{
		return false;
	}

	//gtk_widget_show_all(win);

  	//glade_xml_signal_autoconnect(xml);

	return win;
}

GtkSourceBuffer *crear_source_view (GtkWidget * scroll)
{
	/*SourveView variables*/
	GtkWidget *sView;
 	PangoFontDescription *font_desc;
  	GtkSourceLanguageManager *lm;
  	GtkSourceBuffer *sBuf;
	GtkSourceLanguage *language = NULL;

	/*** GtkSourceView ***/

	/* Now create a GtkSourceLanguagesManager */
	lm = gtk_source_language_manager_new();

	/* and a GtkSourceBuffer to hold text (similar to GtkTextBuffer) */
	sBuf = GTK_SOURCE_BUFFER (gtk_source_buffer_new (NULL));

	/* Create the GtkSourceView and associate it with the buffer */
	sView = gtk_source_view_new_with_buffer(sBuf);
	gtk_source_view_set_show_line_numbers ((GtkSourceView*)sView, TRUE);

	/* Set default Font name,size */
	font_desc = pango_font_description_from_string ("mono 10");
	gtk_widget_modify_font (sView, font_desc);
	pango_font_description_free (font_desc);

	/* get the Language for C source mimetype */
	//language = gtk_source_languages_manager_get_language_from_mime_type (lm,"text/x-c++src");
	language = gtk_source_language_manager_get_language(lm, "text/x-c++src");
	if (language == NULL)
	{
		g_print ("No language found for mime type `%s'\n", "text/x-c++src");
		g_object_set (G_OBJECT (sBuf), "highlight", FALSE, NULL);
	}
	else
	{
		gtk_source_buffer_set_language (sBuf, language);
		g_object_set (G_OBJECT (sBuf), "highlight", TRUE, NULL);
	}

	/* Attach the GtkSourceView to the scrolled Window */
	gtk_container_add (GTK_CONTAINER (scroll), sView);

	return sBuf;

}

GtkWidget * new_code_windows ()
{
	GladeXML *xml;
	GtkWidget *win; /*Main window*/
	GtkWidget *pScrollWinVar, *pScrollWinFun;
	GtkSourceBuffer *sBufVar, *sBufFun;
	GtkWidget *botonAceptar, *botonCancelar;
	
	xml = glade_xml_new("gui/variables.glade", NULL, NULL);

	if (xml == NULL)
	{
		printf ("NO");
	return 0;
	}
	
	/*Cargamos la nueva ventana*/
	win = glade_xml_get_widget(xml, "dialog1");

	/*Conectamos Señales*/	
	glade_xml_signal_autoconnect(xml);

	/*Ponemos Titulo a la ventana*/	
	gtk_window_set_title(GTK_WINDOW(win), "Código Componente");

	/*Situamos la ventana en el medio*/
	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);

	/* scrolled Window */
	
	pScrollWinVar = glade_xml_get_widget(xml, "scrolledwindowV");
	pScrollWinFun = glade_xml_get_widget(xml, "scrolledwindowF");
	
	sBufVar = crear_source_view (pScrollWinVar);
	sBufFun = crear_source_view (pScrollWinFun);

	/*Añadimos texto variables a la ventana*/
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER(sBufVar), variables.c_str(), (int)variables.length());
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER(sBufFun), funciones.c_str(), (int)funciones.length());

	/*Cargamos el boton*/	
	botonCancelar = glade_xml_get_widget(xml, "cancelar");	
	botonAceptar = glade_xml_get_widget(xml,"aceptar");
	
	/*Asignamos elementos*/ 
	g_object_set_data (G_OBJECT (botonAceptar), "bufferVar", sBufVar);	
	g_object_set_data (G_OBJECT (botonAceptar), "bufferFun", sBufFun);

	/*Asignamos Señal al boton*/
	g_signal_connect (G_OBJECT (botonCancelar), "clicked", G_CALLBACK (aceptar), (gchar *)"variables");
	g_signal_connect (G_OBJECT (botonAceptar), "clicked", G_CALLBACK (aceptar), (gchar *)"variables");
	//NO hace falta asignar esta señal. Ya esta la señal asignada al crear la ventana con glade*/
	//g_signal_connect (G_OBJECT (botonCancelar), "clicked", G_CALLBACK (destroy), 0);

	g_signal_connect (G_OBJECT (win), "destroy", G_CALLBACK (eliminar_ventana),0);


	/*Asignamos Señal al boton*/
//	g_signal_connect (G_OBJECT (glade_xml_get_widget(prog,"aceptar")), "clicked", G_CALLBACK (aceptar), (gchar *)"variables");

	/*NO hace falta asignar esta señal. Ya esta la señal asignada al crear la ventana con glade*/
	//g_signal_connect (G_OBJECT (glade_xml_get_widget(prog,"aceptar")), "destroy", G_CALLBACK (cancelar), (gpointer)win);

	gtk_widget_show_all(win);

	return win;
}

void
set_importar(GtkButton *button, gpointer data){

	GladeXML *xml = GLADE_XML (g_object_get_data (G_OBJECT (button), "xml"));
	// Actualizamos 
	GtkWidget *toggle_button = glade_xml_get_widget(xml, "cbLaser");

	imp.laser = GTK_TOGGLE_BUTTON (toggle_button)->active;

	toggle_button = glade_xml_get_widget(xml, "cbMotor");

	imp.motor = GTK_TOGGLE_BUTTON (toggle_button)->active;

	toggle_button = glade_xml_get_widget(xml, "cbRadar");

	imp.radar = GTK_TOGGLE_BUTTON (toggle_button)->active;

	toggle_button = glade_xml_get_widget(xml, "cbEncoders");

	imp.encoders = GTK_TOGGLE_BUTTON (toggle_button)->active;

	toggle_button = glade_xml_get_widget(xml, "cbLat_lon");

	imp.lat_lon = GTK_TOGGLE_BUTTON (toggle_button)->active;

	toggle_button = glade_xml_get_widget(xml, "cbCamara");

	imp.camara = GTK_TOGGLE_BUTTON (toggle_button)->active;

	toggle_button = glade_xml_get_widget(xml, "cbPtencoders");

	imp.ptencoders = GTK_TOGGLE_BUTTON (toggle_button)->active;


	gtk_widget_destroy (gtk_widget_get_toplevel(GTK_WIDGET(button)));
	//gtk_widget_destroy(GTK_WIDGET(data));
}

GtkWidget *
importar_librerias(){
	
	GladeXML *xml;	
	GtkWidget *win; /*Main window*/

	//GladeInterface *inter;

	/*Inicializamos Glade*/
   	glade_init();

	/*Cargamos fichero .Glade donde esta guardada la estructura de la nueva ventana*/
  	xml=glade_xml_new("gui/Importar.glade", NULL, NULL);

	if (xml == NULL)
	{
		printf ("NO");
		return NULL;
	}
	
	/*Cargamos la nueva ventana*/
	win = glade_xml_get_widget(xml, "window1");

	/*Conectamos Señales*/	
	glade_xml_signal_autoconnect(xml);

//	 g_signal_connect (G_OBJECT (glade_xml_get_widget(prog,"button1")),
  //                        "clicked", G_CALLBACK (gtk_main_quit), NULL);

	/*Ponemos Titulo a la ventana*/	
	gtk_window_set_title(GTK_WINDOW(win), "Actuadores y Sensores");

	/*Situamos la ventana en el medio*/
	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
	
	
	/*Cargamos el boton*/	
	GtkWidget *botonAceptar = glade_xml_get_widget(xml, "aceptar");
	GtkWidget *botonCancelar = glade_xml_get_widget(xml, "cancelar");
	
	g_signal_connect (G_OBJECT (botonAceptar), "clicked", G_CALLBACK (set_importar),(gpointer)win);
	g_signal_connect (G_OBJECT (botonCancelar), "clicked", G_CALLBACK (destroy), 0);
	g_signal_connect (G_OBJECT (win), "destroy", G_CALLBACK (eliminar_ventana),0);

	/*Asignamos elementos*/ 
	g_object_set_data (G_OBJECT (botonAceptar), "xml", xml);	


	// Actualizamos 
	GtkWidget *toggle_button = glade_xml_get_widget(xml, "cbLaser");

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle_button), imp.laser);

	toggle_button = glade_xml_get_widget(xml, "cbMotor");

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle_button), imp.motor);

	toggle_button = glade_xml_get_widget(xml, "cbRadar");

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle_button), imp.radar);

	toggle_button = glade_xml_get_widget(xml, "cbEncoders");

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle_button), imp.encoders);

	toggle_button = glade_xml_get_widget(xml, "cbLat_lon");

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle_button), imp.lat_lon);
		
	toggle_button = glade_xml_get_widget(xml, "cbCamara");

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle_button), imp.camara);

	toggle_button = glade_xml_get_widget(xml, "cbPtencoders");

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle_button), imp.ptencoders);

	gtk_widget_show_all(win);

	return win;
}


void set_timer (GtkWindow *button, gpointer data){

	GtkWidget *entry = GTK_WIDGET (g_object_get_data (G_OBJECT (button), "entry"));
                                           
	const char *texto = gtk_entry_get_text(GTK_ENTRY(entry));

	tiempoIteracion = atoi(texto);

	/* Nombre asignado
	* Para cerrar la ventana, y dejar el estado del canvas como estaba, llamamos a la funcion cancelar con la ventana como parámetro, 
	* o emitimos una señal "clicked" por el boton cancelar. */


	//cancelar (window, data2);

	//gtk_signal_emit (GTK_OBJECT (data2), "clicked");
	gtk_widget_destroy (gtk_widget_get_toplevel(GTK_WIDGET(button)));
	//g_signal_emit_by_name (GTK_OBJECT ((GtkWidget *)data), "clicked");
}

GtkWidget *
tiempo_iteracion(){

	GladeXML *xml;
	GtkWidget *win; /*Main window*/

	//GladeInterface *inter;

	/*Inicializamos Glade*/
   	glade_init();

	/*Cargamos fichero .Glade donde esta guardada la estructura de la nueva ventana*/
  	xml=glade_xml_new("gui/timer.glade", NULL, NULL);

	if (xml == NULL)
	{
		printf ("NO");
		return NULL;
	}
	
	/*Cargamos la nueva ventana*/
	win = glade_xml_get_widget(xml, "timer");

	/*Conectamos Señales*/	
	glade_xml_signal_autoconnect(xml);

	/*Ponemos Titulo a la ventana*/	
	gtk_window_set_title(GTK_WINDOW(win), "Tiempo iteracion componente");

	prog = glade_get_widget_tree (win);

	/*Cargamos el boton*/	
	GtkWidget *botonAceptar = glade_xml_get_widget(xml, "BotonAplicar");
	GtkWidget *botonCancelar = glade_xml_get_widget(xml, "BotonCancelar");
	//GtkWidget *button = glade_xml_get_widget(xml, "BotonCancelar");
	
	/*Asignamos Señal al boton*/
	g_signal_connect (G_OBJECT (botonAceptar), "clicked", G_CALLBACK (set_timer), (gpointer)botonCancelar);
	g_signal_connect (G_OBJECT (botonCancelar), "clicked", G_CALLBACK (destroy), 0);
	g_signal_connect (G_OBJECT (win), "destroy", G_CALLBACK (eliminar_ventana),0);

//	g_signal_connect (G_OBJECT (glade_xml_get_widget(xml,"BotonCancelar")), "clicked", G_CALLBACK (cancelar), (gpointer)win);

	GtkWidget *entry = glade_xml_get_widget(xml, "entry1");

	/*Asignamos elementos*/ 
	g_object_set_data (G_OBJECT (botonAceptar), "entry", entry);

	gtk_entry_set_text(GTK_ENTRY(entry), int2string(tiempoIteracion).c_str());
	
	gtk_widget_show_all(win);

  	
	return win;


}

void eliminar_ventana (GtkWidget *ventana, gpointer data)
{
	list<nVentana>::iterator posVentana;

	posVentana = ListaVentanas.begin();
	printf ("VENTANA: %d \n", (int)posVentana->ventana);
	while( (posVentana != ListaVentanas.end()) && (posVentana->ventana != ventana) )
  		posVentana++;

	if (posVentana != ListaVentanas.end())
		 ListaVentanas.erase(posVentana);

}

void add_ventana (GtkWidget *ventana, GnomeCanvasItem * item, tVentana tipo)
{

	nVentana nodo;

	nodo.ventana = ventana;
	nodo.item = item;
	nodo.tipo = tipo;

	ListaVentanas.push_back (nodo);
}


GtkWidget * comprobar_ventana_abierta (GnomeCanvasItem * item, tVentana tipo)
{
	list<nVentana>::iterator posVentana;

	posVentana = ListaVentanas.begin();

	while( posVentana != ListaVentanas.end() ){
		if ((posVentana->tipo == tipo) && (posVentana->item == item))
			return posVentana->ventana;
		else 
  			posVentana++;
  	}

	return NULL;

	
}


/* Función útil a la hora de cambiar el color de relleno de un item
 * del canvas aleatoriamente
 */
void change_item_color (GnomeCanvasItem *item, int color)
{
	static const char *color_specs[] = {
		"red",
		"yellow",
		"green",
		"cyan",
		"blue",
		"magenta",
		"orange",
		NULL
	};

	//int n;

	//item_guardado = item;

	/* Coge un color aleatoriamente de la lista */

	//n = rand () % (sizeof (color_specs) / sizeof (color_specs[0]));

	gnome_canvas_item_set (item,			       
			"fill_color", color_specs[color],
	     		NULL);		
}
