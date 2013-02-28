
//#include <string.h>

#include <stdio.h>

#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
//#include <boost/filesystem.hpp>



#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <list>

/*#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <libgnomecanvas/libgnomecanvas.h>
#include <libglade-2.0/glade/glade.h>
#include <libxml/tree.h>*/

#include <gdk.h>
#include <gtk.h>
#include <gdk-pixbuf.h>
#include <libgnomecanvas.h>
#include <glade/glade.h>
#include <tree.h>


//extern "C" {		// Incluir ficheros de C
   #include "reemplazar.h"
// }


//#define filename ""

using namespace std;


typedef struct nombre_id {
	string id;
	string nombre;
} nombre_id; 

list <nombre_id> ListaNombres;

int def = 0; 	// contador de estados sin identificar (nombre).

string varAux, funAux;

ofstream fs; 

string directorio, esquema, tiempo_esquema, estado_inicial;

string int2string(int n){
	std:: stringstream flujo;

	flujo << n;

	return (flujo.str());
}

string
DameNombre(string id){

	list<nombre_id>::iterator pos;

	pos = ListaNombres.begin();

	while ( (pos != ListaNombres.end()) & (pos->id != id) )
	{
		pos++;
	}
	return pos->nombre;
}

void
variablesH(xmlDocPtr doc){

	xmlNodePtr root;
	xmlNodePtr node;
	xmlNodePtr nodeL;
	char *libreria;

	root = xmlDocGetRootElement (doc);

	node = root->xmlChildrenNode;

	while (strcmp((const char*)node->name, "Librerias")!=0) 

	 	node = node->next;

	nodeL = node->xmlChildrenNode;

	while (nodeL!=NULL) {

		libreria = (char*)xmlNodeGetContent (nodeL);
		
		if (strcmp((const char*)libreria, "laser")==0){
			
			fs << "extern float laser[NUM_LASER];" << endl;
			
		} 
		else if (strcmp((const char*)libreria, "motor")==0){
			
			fs << "extern float v;" << endl;
			fs << "extern float w;" << endl;

		}
		else if (strcmp((const char*)libreria, "radar")==0){

		}	
		else if (strcmp((const char*)libreria, "ptencoders")==0){

		}		
		else if (strcmp((const char*)libreria, "encoders")==0){

			fs << "float robot[5];" << endl;

		}
		else if (strcmp((const char*)libreria, "lat_lon")==0){

		}		
		else if (strcmp((const char*)libreria, "camara")==0){	

			fs << "extern char imagenRGB[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3];" << endl;

		}
	

		nodeL = nodeL ->next;
	}
	
	fs << endl;
}

void
importarCodigoAux(xmlDocPtr doc){

	xmlNodePtr root;
	xmlNodePtr node, nodeE;
	string id, nombre;

	root = xmlDocGetRootElement (doc);

	node = root->xmlChildrenNode;
	
	while (strcmp((const char*)node->name, "Estado")==0) {
		if ( strcmp((const char*)xmlGetProp (node, (const xmlChar*)"estado_inicial"), (const char*)"true") == 0 ){
			nodeE = node->xmlChildrenNode;
	     			
			id = (const char*)xmlNodeGetContent (nodeE);
		
			while (strcmp((const char *)nodeE->name, (const char *)("nombre")) != 0) 		  				
				nodeE = nodeE->next;
			
			nombre = (const char*)xmlNodeGetContent (nodeE);

			if (nombre != "")
			{
				estado_inicial = nombre;
			}
			else{
				estado_inicial = DameNombre(id);
			}
			break;
		}
	 	node = node->next;
	}

	while (strcmp((const char*)node->name, "tiempoIteracion")!=0) 

	 	node = node->next;
	 	
	tiempo_esquema = (char*)xmlNodeGetContent (node);
	


	while (strcmp((const char*)node->name, "variables_aux")!=0) 

	 	node = node->next;
	
	varAux = (char*)xmlNodeGetContent (node);
	
	while (strcmp((const char*)node->name, "funciones_aux")!=0) 

	 	node = node->next;
	 	
	funAux = (char*)xmlNodeGetContent (node);
}


int
generar_lista_nombres(xmlDocPtr doc, fstream *fs)
{
	xmlNodePtr root;
	xmlNodePtr node;
	xmlNodePtr nodeE;

	string id;
	string nombre; 

	root = xmlDocGetRootElement (doc);

	node = root->xmlChildrenNode;

	while (strcmp((const char*)node->name, "Estado")==0) {

	   	nombre_id t;

		printf("Encontrado nodo %s\n", node->name);
		
		nodeE = node->xmlChildrenNode;

		id= (const char*)xmlNodeGetContent (nodeE);
		
		t.id = id;

		nodeE = nodeE->next;
		
		/*Cogemos el nombre del estado origen*/
		while (strcmp((const char *)nodeE->name, (const char *)("nombre")) != 0) 		  				
			nodeE = nodeE->next;

		nombre = (const char*)xmlNodeGetContent (nodeE);

		if (nombre != "")
		{
			t.nombre = nombre;
		}
		else{
			t.nombre = "default_" + int2string(def);
			def++;
		}

		*fs << "				"<< t.nombre;

		node = node->next;

		if (strcmp((const char*)node->name, "Estado")==0) 
			*fs << "," << endl;
		else
			*fs << "," << endl << "				fin"<< endl;

		ListaNombres.push_back(t);		
    				
	}
	return 0;
}


char InvierteMayusculas(char c)
{
	return c^0x20;
}

string s_toupper(string cadena)
{
   unsigned int i;
   
   for(i = 0; i<cadena.length(); i++) 
      cadena.at(i) = toupper(cadena.at(i)); //<--convierte a minusculas la cadena

   return cadena;
}

string s_reemplazar(string cadena, string subcadena, string reemplazo)
{

   int indice;

   for(;;)
   {
     indice = posicion((char *)cadena.c_str(), (char *)subcadena.c_str());
     if(indice==-1)
   	break;
     else
	cadena.replace(indice,subcadena.length(),reemplazo); 
   }

   return cadena;
}

string get_file_name (string file)
{
	size_t found;

	found=file.rfind("/");
	
	return file.substr (++found);

}

int copiar_ficheros (string dir_origen, string dir_destino)
{
	string files_dir,buff, sCadena, sfile;
	//char *file, *introrob = (char *)"introrob";
	char cadena[500];
	fstream fo,fs;
	DIR *dir;
	struct dirent * fichero;
	/*
	 * fichero->d_name 				Nombre del fichero
	 * fichero->d_type 				determina el tipo de fichero
	 */

	//abrimos una instancia al directorio
	//files_dir = "/home/dyunta/Automatas/src/datos_esquemas/introrob/";
	dir=opendir(dir_origen.c_str());		//  n = scandir("dir", &namelist, 0, alphasort);

	//si devuelve null, es que ha habido algun error
	if (dir!=NULL)
	{
		//bucle para leer todos los archivos del directorio
		while ((fichero=readdir(dir)) != NULL )
		{
			//si NO es un directorio...
			if(fichero->d_type!=DT_DIR)
			{
				sfile = fichero->d_name;
				//file = (char*)malloc(15 + 1);
				//strcpy(file, sfile.c_str());
				//reemplazar(file, introrob, (char *)esquema.c_str());
				sfile = s_reemplazar(sfile, "introrob", esquema);
				sfile = dir_destino + sfile;
				fs.open(sfile.c_str(), ofstream::out);
			
				
				sfile = dir_origen + fichero->d_name;
				fo.open(sfile.c_str(), ifstream::in);
	
				fo.getline(cadena, 500);
				sCadena = cadena;
				//fo >> cadena;
				while (!fo.eof())
				{
		
					sCadena = s_reemplazar(sCadena, "introrob", esquema);
					sCadena = s_reemplazar(sCadena, "INTROROB", s_toupper(esquema));
					sCadena = s_reemplazar(sCadena, "int cycle = 100;//", "int cycle = "+ tiempo_esquema +";");

					fs << sCadena << endl;

					fo.getline(cadena, 500);

					sCadena = cadena;
				}

				fs.close();
				fo.close();
			}	
			else  
				if (*fichero->d_name!= '.'){	// Si no es un archivo oculto
					files_dir = dir_destino + fichero->d_name + "/";
					mkdir(files_dir.c_str(), 0777);
					//boost::filesystem::create_directory(files_dir.c_str());
					copiar_ficheros (dir_origen + fichero->d_name + "/", files_dir);
					//cout << fichero->d_name << endl;	
				}			
		}
		closedir(dir);
		
		return 1;
	}else{
		closedir(dir);
		cout << "no se ha podido abrir el directorio: " << dir_origen << endl;
		return 0;
	}

}
int cfg (string path, xmlDocPtr doc)
{
	fstream fs; 
	
	fs.open(path.c_str(), ofstream::out);
	if (fs.is_open()){
		xmlNodePtr root;
		xmlNodePtr node;
		xmlNodePtr nodeL;
		char *libreria;

		root = xmlDocGetRootElement (doc);

		node = root->xmlChildrenNode;

		while (strcmp((const char*)node->name, "Librerias")!=0) 

	 		node = node->next;

		nodeL = node->xmlChildrenNode;

		while (nodeL!=NULL) {

			libreria = (char*)xmlNodeGetContent (nodeL);
		
			if (strcmp((const char*)libreria, "laser")==0){
			
				fs << "Introrob.Laser.Proxy=laser1:tcp -h localhost -p 9999" << endl;
			
			} 
			else if (strcmp((const char*)libreria, "motor")==0){
			
				fs << "Introrob.Motors.Proxy=motors1:tcp -h localhost -p 9999" << endl;

			}
			else if (strcmp((const char*)libreria, "radar")==0){
				fs << "Introrob.Sonars.Proxy=sonar1:tcp -h localhost -p 9999" << endl;
			}	
			else if (strcmp((const char*)libreria, "ptencoders")==0){
				fs << "Introrob.PTEncoders1.Proxy=ptencoders1:tcp -h localhost -p 9999" << endl;
				fs << "Introrob.PTEncoders2.Proxy=ptencoders2:tcp -h localhost -p 9999" << endl;
			}		
			else if (strcmp((const char*)libreria, "encoders")==0){

				fs << "Introrob.Encoders.Proxy=encoders1:tcp -h localhost -p 9999" << endl;

			}
			else if (strcmp((const char*)libreria, "lat_lon")==0){
				fs << "Introrob.PTMotors1.Proxy=ptmotors1:tcp -h localhost -p 9999" << endl;
				fs << "Introrob.PTMotors2.Proxy=ptmotors2:tcp -h localhost -p 9999" << endl;
			}		
			else if (strcmp((const char*)libreria, "camara")==0){	

				fs << "Introrob.Camera1.Proxy=cameraA:tcp -h localhost -p 9999" << endl;
				fs << "Introrob.Camera2.Proxy=cameraB:tcp -h localhost -p 9999" << endl;


			}
	
		nodeL = nodeL ->next;
		}
	
		fs << endl;
		fs.close();
		
		return 0;
	}else{
		return 1;
	}
	
}

int navegaH (string path, xmlDocPtr doc)
{
	fstream fs; 
	
	fs.open(path.c_str(), ofstream::out);
	if (fs.is_open()){
		fs << "#ifndef SIGUE_LINEA_NAVEGA_H"<< endl;
		fs << "#define SIGUE_LINEA_NAVEGA_H"<< endl;
		fs << ""<< endl;
		fs << "#include <string>"<< endl;
		fs << "#include <iostream>"<< endl;
		fs << "#include <cv.h>"<< endl;
		fs << "#include \"ventana.h\""<< endl;
		fs << "#include \"navegacion.h\""<< endl;
		fs << "#include \"controller.h\""<< endl;
		fs << ""<< endl;
		fs << "namespace " << esquema << "{" << endl; 
		fs << ""<< endl;
		fs << "	class Controller;"<< endl;
		fs << "	class Navegacion;"<< endl;
		fs << ""<< endl;
		fs << "	class Navega {"<< endl;
		fs << "		private:"<< endl;
		
		/* Enumeracion de los estados */	
		fs << "			typedef enum Estado {" << endl;
		generar_lista_nombres(doc, &fs);
		fs << "			}Estado;" << endl;
		
		fs << "			int cont;"<< endl;
		fs << "			Estado Estado_Actual;"<< endl;
		fs << "			Estado Estado_Anterior;"<< endl;
		fs << "			int Estado_Final;"<< endl;
		fs << ""<< endl;
		fs << "			//Var Estados Temporales"<< endl;
		fs << "			time_t t_ini;"<< endl;
		fs << "			time_t t_fin;"<< endl;
		fs << "			double secs;"<< endl;
		fs << "			bool t_activated;"<< endl;
		fs << ""<< endl;
		fs << "			CvPoint2D32f destino;"<< endl;
		fs << "			Controller* controller;"<< endl;
		fs << "			Navegacion* navegacion;"<< endl;
		fs << "			Ventana* ventana;"<< endl;
		fs << ""<< endl;
		fs << "		public:"<< endl;
		fs << "			Navega (Controller* controller, Navegacion* navegacion);"<< endl;
		fs << "			~Navega ();"<< endl;
		fs << "			void iteracionGrafica();"<< endl;
		fs << "			void iteracionControl();"<< endl;
		fs << "	};"<< endl;
		fs << "} // namespace"<< endl;
		fs << ""<< endl;
		fs << "#endif /*SIGUE_LINEA_NAVEGA_H*/"<< endl;
		fs.close();
		
		return 0;
	}else{
		return 1;
	}
	
}


int navegaCPP (string path, xmlDocPtr doc)
{
	list<nombre_id>::iterator pos;
	fstream fs; 
	xmlNodePtr root, node, nodeE, nodeT;
	string id, codigo, nombre;
	
	fs.open(path.c_str(), ofstream::out);
	if (fs.is_open()){
	
		fs << "#include \"navega.h\""<< endl;
		fs << endl;
		fs << "namespace " << esquema << "{" << endl; 
		
		/* Añadimos Funciones Auxiliares a los estados y transiciones*/
		fs << "		" << funAux << endl;
		
		/* FUNCION iteracion principal */
		fs << "		void Navega::iteracionControl () {" << endl; 

		/* Añadimos Variables Auxiliares a los estados y transiciones*/
		fs << "			" << varAux << endl;

		fs << "			if (Estado_Actual!= Estado_Final){" << endl;
		fs << endl;
		fs << "				switch (Estado_Actual) {" << endl;
	
		root = xmlDocGetRootElement (doc);

		node = root->xmlChildrenNode;

		while (strcmp((const char*)node->name, "Estado") == 0) {
	    			
//			printf("Encontrado nodo %s\n", node->name);

		//	xml_get_entry (node);
			nodeE = node->xmlChildrenNode;
	     			
			id = (const char*)xmlNodeGetContent (nodeE);
		
			while (strcmp((const char *)nodeE->name, (const char *)("nombre")) != 0) 		  				
				nodeE = nodeE->next;
			
			nombre = (const char*)xmlNodeGetContent (nodeE);

			if (nombre != "")
			{
				fs << "				case " << nombre << ":" << endl;
			}
			else{
				fs << "				case " << DameNombre(id) << ":" << endl;
			}
			//fs << "					this->ventana->change_nodo_color (Estado_Actual, 0);"<< endl;
			nodeE = nodeE->next;
			
			codigo = (const char*)xmlNodeGetContent (nodeE);
		
			fs << "					" << codigo << endl;

			nodeE = nodeE->next;

			nodeT = nodeE->xmlChildrenNode;

			if (nodeT!=NULL){

				//nodeT = nodeE->xmlChildrenNode;

				while (nodeT!=NULL){
					id = (const char*)xmlNodeGetContent (nodeT->xmlChildrenNode->next->next);
					if (nodeT->xmlChildrenNode->next->next->next != NULL){
						if(strcmp((const char*)nodeT->xmlChildrenNode->next->next->next->name,"codigo") == 0)
						{
							codigo = (const char*)xmlNodeGetContent (nodeT->xmlChildrenNode->next->next->next);
							fs << "					if (" << codigo << " ){" << endl;
						//	fs << "						this->ventana->change_nodo_color (Estado_Actual, 3);" << endl;
						//	fs << "						Estado_Anterior = Estado_Actual;" << endl;
							fs << "						Estado_Actual = "<<  DameNombre(id) << ";" << endl;
							fs << "					}" << endl;
						}else if (strcmp((const char*)nodeT->xmlChildrenNode->next->next->next->name,"tiempo") == 0){
							int tiempoTrans = atoi((const char*)xmlNodeGetContent (nodeT->xmlChildrenNode->next->next->next));
			
							fs << "					if (!t_activated){" << endl;
							fs << "						t_ini = time(NULL);" << endl;
	  		
							fs << "						t_activated = true;" << endl;
							fs << "					}else{" << endl;
							fs << "						t_fin = time(NULL);" << endl;
							fs << "						secs = difftime(t_fin, t_ini);" << endl;
							fs << "						if (secs > (double) "<< ((double)tiempoTrans/1000) <<"){" << endl;
						//	fs << "							this->ventana->change_nodo_color (Estado_Actual, 3);" << endl;
						//	fs << "							Estado_Anterior = Estado_Actual;" << endl;
							fs << "							Estado_Actual = "<<  DameNombre(id) << ";" << endl;
							fs << "							t_activated = false;" << endl;
							
							fs << "						}" << endl;
							fs << "					}" << endl;
							
						}
					}
					nodeT = nodeT->next;
				}	
							

			}//else{
			//	fs << "					Estado_Actual = -1;" << endl;
			//}
	
			fs << "					break;" << endl;
			node = node->next;

	  	}

	
		fs << "				}" << endl;
		fs << "			}" << endl;
		fs << "		}" << endl;

		fs << "		void Navega::iteracionGrafica () {" << endl;
		fs << "			/* TODO: ADD YOUR GRAPHIC CODE HERE */" << endl;
		fs << "			CvPoint3D32f aa,bb;" << endl;
		fs << "			CvPoint2D32f destino;" << endl;
		fs << "			CvPoint3D32f a,b;" << endl;
		fs << "			CvPoint3D32f c,d;" << endl;
		fs << "			CvPoint3D32f color;" << endl;

		fs << "			this->navegacion->drawProjectionLines();" << endl;
		fs << "	" << endl;

		/* Código Ejecución Grafica. */

		fs << "			if (Estado_Actual != Estado_Anterior){" << endl;
		fs << "				this->ventana->change_nodo_color (Estado_Anterior, 3);" << endl;
		fs << "			}" << endl;
		fs << "	" << endl;
		fs << "			this->ventana->change_nodo_color (Estado_Actual, 0);" << endl;
		fs << "			Estado_Anterior = Estado_Actual;" << endl;

		fs << "		}" << endl;
		fs << "		Navega::Navega (Controller* controller, Navegacion* navegacion) {" << endl;
		fs << "			this->controller = controller;" << endl;
		fs << "			this->navegacion = navegacion;" << endl;
		
		fs << "			Estado_Actual = "<< estado_inicial <<";" << endl;
		fs << "			Estado_Anterior = "<< estado_inicial <<";" << endl;
		fs << "			Estado_Final = fin; "<< endl;
		fs << "			this->ventana = new Ventana(\"" << directorio+esquema+".xml" << "\");"<< endl;
		
		fs << "		}" << endl;
		fs << "" << endl;
		fs << "		Navega::~Navega () {}" << endl;
		fs << "}" << endl;
		
		fs.close();
		
		return 0;
	}else{
		return 1;
	}
	
}


int main( int argc, char *argv[])
{
	xmlDocPtr doc; 
//	xmlNodePtr root;
//	xmlNodePtr node;
//	xmlNodePtr nodeE;
//	xmlNodePtr nodeT, nodeTT;


	
	
	string path;
	
//argv[0] = (char *)"/home/dyunta/Automatas/src/Ejemplos/sigue_linea/sigue_linea.xml";
//argv[1] = (char *)"/home/dyunta/Automatas/src/Ejemplos/sigue_linea/sigue_linea.c";
	char *filename = argv[1];

	//strcpy(filename, "EstoEsUnaPrueba3.xml");
	
	printf("ARG: %s %s\n", argv[0], argv[1]);

	doc = xmlParseFile (argv[1]);
	


	/* Nombre y Directorio Esquema */


 	printf("Fichero abierto: %s\n",filename);

	printf("Fichero abierto: %s\n",filename);

	string fich = filename;
	int i;
	size_t pos;
	for (i=0; i < (int)fich.length(); i++)
  	{
   		if (fich.at(i) == '/')
			pos = i;
 	}	
	string fichero = fich.substr (++pos);	// Nombre del fichero (ejemplo.xml)

	directorio = fich.substr (0,pos);	// Directorio del fichero (/home/usuario/.../)

	

	for (i=0; i < (int)fichero.length(); i++)
  	{
   		if (fichero.at(i) == '.')
			pos = i;
 	}	
		
	esquema = fichero.substr (0,pos);	// Nombre del fichero sin extension
		
	
	
	
	//fs << codigo << endl;
	//fs << "using namespace std;" << endl << endl;
	
	
	//switch (event->type) {

	//case GDK_BUTTON_RELEASE:
	
//	char cadena[500];

//	string sCadena;

//	char *subcadena = (char *)"introrob"; 
//	//const char* intr = "introrob";
//	//strcpy(subcadena, intr);
//	cout << subcadena << endl;
//	cout << esquema << endl;
//	
//	
	if (!doc) {
	  	printf("Error al cargar documento XML\n");
	}
	else{
		importarCodigoAux(doc);
		path = directorio+ "src";
//		char *c = (char *) directorio.c_str();
//		strcat (c, "src");
//		mkdir((const char *)c, 0777);
		mkdir(path.c_str(), 0777);
		copiar_ficheros ("./datos_esquemas/introrob/", directorio+"src/");
		
		path = directorio+ "src/navega.h";
		navegaH (path, doc);
		
		path = directorio+ "src/navega.cpp";
		navegaCPP (path, doc);
		
		//path = directorio + "src/" + esquema + ".cfg";
		//cfg (path, doc);
		
		printf("Ficheros Generados \n");

	}	
	//g_free (filename);
	
	return 0;
}
