
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

#include <gdk/gdk.h>
/*#include <gdkmm-3.0/gdkmm.h> */
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
/*#include <gdk-pixbuf-2.0/gdkmm.h> */
#include <libgnomecanvas/libgnomecanvas.h>
#include <glade/glade.h>
#include <libxml/tree.h>

/*
#include <gdk.h>
#include <gtk.h>
#include <gdk-pixbuf.h>
#include <libgnomecanvas.h>
#include <glade/glade.h>
#include <tree.h>
*/

//extern "C" {		// Incluir ficheros de C
   #include "reemplazar.h"
// }


//#define filename ""

using namespace std;


typedef struct nombre_id {
	string id;
	string nombre;
} nombre_id; 


typedef struct interfaces {
	bool laser;
	bool motor;
	bool radar;
	bool encoders;
	bool lat_lon;
	bool camara;
	bool ptencoders;
} interfaces;

list <nombre_id> ListaNombres;

interfaces interIce;

int def = 0; 	// contador de estados sin identificar (nombre).  //Ya no se usa, se concatena el id del estado 'RSB'

//string varAux, funAux;//**********************Comentado por RSB

ofstream fs; 

string directorio, esquema, tiempo_esquema, estado_inicial;



string int2string(int n){
	std::stringstream flujo;

	flujo << n;

	return (flujo.str());
}

string DameNombre(string id){

	list<nombre_id>::iterator pos;

	pos = ListaNombres.begin();

	while ( (pos != ListaNombres.end()) & (pos->id != id) )
	{
		pos++;
	}
	return pos->nombre;
}


string DameNombrePadre(xmlDocPtr doc,int idPadre,int idSub){
	
	xmlNodePtr root, nodeS, nodeE, nodeN;

	root = xmlDocGetRootElement (doc);

	nodeS = root->xmlChildrenNode;

	bool encontrado;

	string nombrePadre;

	int idEstado;


	encontrado = FALSE;

	while (strcmp((const char*)nodeS->name, "SubAutomata")==0 and encontrado == FALSE) {

		nodeE = nodeS->xmlChildrenNode;

		if (atoi((const char*)xmlNodeGetContent (nodeE)) == idPadre) {
		
			while (strcmp((const char*)nodeE->name, "Estado")!=0){
				nodeE = nodeE->next;
			}

			while (strcmp((const char*)nodeE->name, "Estado")==0 and encontrado == FALSE){

				nodeN = nodeE->xmlChildrenNode;

				while (strcmp((const char*)nodeN->name, "hijo")!=0){
					nodeN = nodeN->next;
				}

				if (atoi((const char*)xmlNodeGetContent (nodeN)) == idSub) {
					nodeN = nodeN->prev;

					nombrePadre = (const char*)xmlNodeGetContent (nodeN);

					if (nombrePadre == ""){

						nodeN = nodeE->xmlChildrenNode;
						
						idEstado = atoi((const char*)xmlNodeGetContent (nodeN));
					
						nombrePadre = "default_"+int2string(idEstado);

					}

					encontrado = TRUE;
				}
				nodeE = nodeE->next;
			}
		}
		nodeS = nodeS->next;
	}	

	return nombrePadre;

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
		//}//añadido

			nodeL = nodeL->next;
	}
	
	fs << endl;
}


int
generar_lista_nombres(xmlDocPtr doc, fstream *fs)
{
	xmlNodePtr root;
	xmlNodePtr nodeS;
	xmlNodePtr nodeE;
	xmlNodePtr nodeN;

	string id;
	string nombre; 

	int idSub;
	int idPadre;

	list<nombre_id>::iterator pos;

	root = xmlDocGetRootElement (doc);

	nodeS = root->xmlChildrenNode;

	while (strcmp((const char*)nodeS->name, "SubAutomata")==0) {

	   	nodeE = nodeS->xmlChildrenNode;

		idSub = atoi((const char*)xmlNodeGetContent (nodeE));

		nodeE = nodeE->next;

		idPadre = atoi((const char*)xmlNodeGetContent (nodeE));

		nodeE = nodeE->next;//nos posicionamos en el primer estado

		if (idPadre == 0) {

			nombre_id t;

			while (strcmp((const char*)nodeE->name, "Estado")==0){

				nodeN = nodeE->xmlChildrenNode;

				id = (const char*)xmlNodeGetContent (nodeN);

				t.id = id;

				while (strcmp((const char *)nodeN->name, (const char *)("nombre")) != 0){
					nodeN = nodeN->next;
				}

				nombre = (const char*)xmlNodeGetContent (nodeN);

				if (nombre != "")
				{
					t.nombre = nombre;
				}
				else{
					t.nombre = "default_" + id;
				}
				
				nodeE = nodeE->next;

				ListaNombres.push_back(t);

			}

			//Componemos el struct de este subAutomata y borramos la lista para componer el siguiente
			pos = ListaNombres.begin();			

			*fs << "typedef enum Estado_Sub_"+int2string(idSub)+" {" << endl;
			*fs << "	"+pos->nombre;
			pos++;				

			while (pos != ListaNombres.end()){
			
				*fs << "," << endl;
				*fs << "	"+pos->nombre;
				
				pos++;				
			}
		
			*fs << endl;					
			*fs << "}Estado_Sub_"+int2string(idSub)+";" << endl;
			*fs << endl;

			//Antes de borrar la lista componemos un array con los nombres para luego poder mostrarlos en el GUI
			pos = ListaNombres.begin();

			*fs << "const char *Nombres_Sub_"+int2string(idSub)+"[] = {" << endl;
			*fs << "	\""+pos->nombre+"\"";
			pos++;		

			while (pos != ListaNombres.end()){
			
				*fs << "," << endl;
				*fs << "	\""+pos->nombre+"\"";
				
				pos++;				
			}

			*fs << endl;
			*fs << "};" << endl;
			*fs << endl;

			//****************************************************************************************************************************

			/*const char *myEnumTypeNames[] =
			{
				"enum1",
				"enum2",
				"enum3"
			};*/

			//****************************************************************************************************************************

			ListaNombres.clear();

		}
		else{//Si no es el nodo padre tenemos que hacer estados "Ghost"
			
			nombre_id t;

			while (strcmp((const char*)nodeE->name, "Estado")==0){

				nodeN = nodeE->xmlChildrenNode;

				id = (const char*)xmlNodeGetContent (nodeN);

				t.id = id;

				while (strcmp((const char *)nodeN->name, (const char *)("nombre")) != 0){
					nodeN = nodeN->next;
				}

				nombre = (const char*)xmlNodeGetContent (nodeN);

				if (nombre != "")
				{
					t.nombre = nombre;
				}
				else{
					t.nombre = "default_" + id;
				}

				nodeE = nodeE->next;

				ListaNombres.push_back(t);

			}

			//Componemos el struct de este subAutomata y borramos la lista para componer el siguiente
			pos = ListaNombres.begin();			

			*fs << "typedef enum Estado_Sub_"+int2string(idSub)+" {" << endl;
			*fs << "	"+pos->nombre+"," << endl;
			*fs << "	"+pos->nombre+"_ghost";
			pos++;				

			while (pos != ListaNombres.end()){
			
				//Al no ser el subautomata padre hacemos estados ghost
				*fs << "," << endl;			
				*fs << "	"+pos->nombre+"," << endl;
				*fs << "	"+pos->nombre+"_ghost";				
				pos++;				
			}
		
			*fs << endl;					
			*fs << "}Estado_Sub_"+int2string(idSub)+";" << endl;
			*fs << endl;

			//Antes de borrar la lista componemos un array con los nombres para luego poder mostrarlos en el GUI
			pos = ListaNombres.begin();

			*fs << "const char *Nombres_Sub_"+int2string(idSub)+"[] = {" << endl;
			*fs << "	\""+pos->nombre+"\"";
			pos++;		

			while (pos != ListaNombres.end()){
			
				*fs << "," << endl;
				*fs << "	\""+pos->nombre+"\"";
				
				pos++;				
			}

			*fs << endl;
			*fs << "};" << endl;
			*fs << endl;

			ListaNombres.clear();

		}	

		nodeS = nodeS->next;
    				
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
	xmlNodePtr root, nodeS, nodeE;
	int idSub;
	
	fs.open(path.c_str(), ofstream::out);
	if (fs.is_open()){									

		fs << "#ifndef MYCOMPONENT_CONTROL_H"<< endl;
		fs << "#define MYCOMPONENT_CONTROL_H"<< endl;
		fs << endl;
		fs << endl;
		fs << "#include <iostream>" << endl;
		fs << "#include <Ice/Ice.h>" << endl;
		fs << "#include <IceUtil/IceUtil.h>" << endl;
		fs << "#include <jderobot/camera.h>" << endl;
		fs << "#include <jderobot/motors.h>" << endl;
		fs << "#include <jderobot/ptmotors.h>" << endl;
		fs << "#include <jderobot/laser.h>" << endl;
		fs << "#include <jderobot/encoders.h>" << endl;
		fs << "#include <jderobot/ptencoders.h>" << endl;
		fs << "#include <colorspaces/colorspacesmm.h>" << endl;
		fs << "#include <pthread.h>" << endl;
		fs << endl;
		fs << endl;		
		fs << "namespace mycomponent {" << endl; 						
		fs << "		class Gui;" << endl;
		fs << "		class Control {" << endl;
		fs << "		public:" << endl;
		fs << endl;
		fs << "		virtual ~Control();" << endl;
		fs << endl;

		fs << "			//FUNCTIONS" << endl;
		fs << "			void handleCameras();" << endl;
		fs << "			void resetControl();" << endl;
	
		fs << "			//HANDLE THREADS" << endl;
		fs << "			pthread_t thr_gui;" << endl;


		root = xmlDocGetRootElement (doc);

		nodeS = root->xmlChildrenNode;

		while (strcmp((const char*)nodeS->name, "SubAutomata")==0) {
			
			nodeE = nodeS->xmlChildrenNode;

			idSub = atoi((const char*)xmlNodeGetContent (nodeE));

			fs << "			pthread_t thr_sub_"+int2string(idSub)+";" << endl;

			nodeS = nodeS->next;
			
		}

		fs << "			pthread_mutex_t controlGui;" << endl;

		fs << endl;
		fs << "			// INTERFACES DATA" << endl;
		fs << "			jderobot::EncodersDataPtr ed;" << endl;
		fs << "			jderobot::LaserDataPtr ld;" << endl;
		fs << "			jderobot::ImageDataPtr data1; // Contains the image info" << endl;
		fs << "			jderobot::ImageDataPtr data2; // Contains the image info" << endl;
		fs << "			jderobot::PTEncodersDataPtr pted1;" << endl;
		fs << "			jderobot::PTEncodersDataPtr pted2;" << endl;
		fs << endl;
		fs << "			// INTERFACES" << endl;
		fs << "			jderobot::MotorsPrx mprx;" << endl;
		fs << "			jderobot::EncodersPrx eprx;" << endl;
		fs << "			jderobot::LaserPrx lprx;" << endl;
		fs << "			jderobot::CameraPrx cprx1;" << endl;
		fs << "			jderobot::CameraPrx cprx2;" << endl;
		fs << "			jderobot::PTMotorsPrx ptmprx1;" << endl;
		fs << "			jderobot::PTEncodersPrx pteprx1;" << endl;
		fs << "			jderobot::PTMotorsPrx ptmprx2;" << endl;
		fs << "			jderobot::PTEncodersPrx pteprx2;" << endl;
	    fs << endl;
		fs << "			colorspaces::Image* image1;	// Prepare the image to use with openCV" << endl;
		fs << "			colorspaces::Image* image2;" << endl;
	 	fs << endl;
		fs << "		};//class" << endl;
		fs << "	} // namespace" << endl;
		fs << "#endif /*MYCOMPONENT_Control_H*/" << endl;
		
		return 0;
	}else{
		return 1;
	}
	
}


int navegaCPP (string path, xmlDocPtr doc)
{
	list<nombre_id>::iterator pos;
	fstream fs; 
	xmlNodePtr root, nodeS, nodeE, nodeN, nodeT;
	string id, codigo, nombre;using namespace std;

	int idSub;
	int idPadre;
	bool escrito_inicial = false;

	int ciclo;
	string varAux, funAux;
	
	fs.open(path.c_str(), ofstream::out);
	if (fs.is_open()){


		fs << "#include \"control_class.h\"" <<endl;
		fs << "#include \"gui.h\"" << endl;
		fs << endl;
		fs << endl;
		fs << "mycomponent::Control *control;" << endl;
		fs << endl;

		//ITERAR PARA DEFINIR LOS STRUCT DE LOS ESTADOS
		//...
		generar_lista_nombres(doc, &fs);
		fs << endl;
		//...

		//Variables para los automatas
		//...

		//fs << "		" << funAux << endl;
		//fs << endl;
		//fs << endl;
		//fs << "		" << varAux << endl;
		//fs << endl;
		//fs << endl;
		//...

		//DEFINIR ESTADOS INICIALES
		//...
		root = xmlDocGetRootElement (doc);

		nodeS = root->xmlChildrenNode;

		while (strcmp((const char*)nodeS->name, "SubAutomata")==0) {

		   	nodeE = nodeS->xmlChildrenNode;

			idSub = atoi((const char*)xmlNodeGetContent (nodeE));

			nodeE = nodeE->next;
			idPadre = atoi((const char*)xmlNodeGetContent (nodeE));
			nodeE = nodeE->next;//nos posicionamos en el primer estado

			escrito_inicial = false;

			while ((strcmp((const char*)nodeE->name, "Estado")==0) and (escrito_inicial == false)){
				if (strcmp((const char*)xmlGetProp (nodeE, (const xmlChar*)"estado_inicial"), (const char*)"true")==0){
				
					nombre_id t;	

					nodeN = nodeE->xmlChildrenNode;

					id = (const char*)xmlNodeGetContent (nodeN);

					t.id = id;

					while (strcmp((const char *)nodeN->name, (const char *)("nombre")) != 0){
						nodeN = nodeN->next;
					}

					nombre = (const char*)xmlNodeGetContent (nodeN);

					if (nombre != "")
					{
						t.nombre = nombre;
					}
					else{
						t.nombre = "default_" + id;
					}				
					
					//Si es un subautómata hijo tiene que empezar en estado "_ghost"
					if (idPadre == 0) {
						fs << "Estado_Sub_"+int2string(idSub)+" Sub_"+int2string(idSub)+"="+t.nombre+";" << endl;
					}else{
						fs << "Estado_Sub_"+int2string(idSub)+" Sub_"+int2string(idSub)+"="+t.nombre+"_ghost;" << endl;
					}					

					escrito_inicial = true;

				}

				nodeE = nodeE->next;
			}

			if (escrito_inicial == false){//Si el usuario no definió estado inicial para el nivel ponemos como inicial al primero
				nodeE = nodeS->xmlChildrenNode;
				nodeE = nodeE->next;
				nodeE = nodeE->next;//nos posicionamos en el primer estado

				nombre_id t;	

				nodeN = nodeE->xmlChildrenNode;

				id = (const char*)xmlNodeGetContent (nodeN);

				t.id = id;

				while (strcmp((const char *)nodeN->name, (const char *)("nombre")) != 0){
					nodeN = nodeN->next;
				}

				nombre = (const char*)xmlNodeGetContent (nodeN);

				if (nombre != "")
				{
					t.nombre = nombre;
				}
				else{
					t.nombre = "default_" + id;
				}				

				//Si es un subautómata hijo tiene que empezar en estado "_ghost"
				if (idPadre == 0) {
					fs << "Estado_Sub_"+int2string(idSub)+" Sub_"+int2string(idSub)+"="+t.nombre+";" << endl;
				}else{
					fs << "Estado_Sub_"+int2string(idSub)+" Sub_"+int2string(idSub)+"="+t.nombre+"_ghost;" << endl;
				}

			}

			nodeS = nodeS->next;			
			
		}

		fs << endl;
		fs << endl;

		fs << "namespace mycomponent {" << endl;
		fs << endl;

		//Comentar handleCameras?? si no selecciona camara fallará-----##############################################################################
		fs << "void Control::handleCameras(){" << endl;
   		fs << endl;
		fs << "		this->data1 = cprx1->getImageData();" << endl;
		fs << "		colorspaces::Image::FormatPtr fmt1 = colorspaces::Image::Format::searchFormat(this->data1->description->format);" << endl;
		fs << "		if (!fmt1)" << endl;
		fs << "		    throw \"Format not supported\";" << endl;
		fs << endl;
		fs << "		this->image1 = new colorspaces::Image (this->data1->description->width, this->data1->description->height, fmt1, &(this->data1->pixelData[0])); // Prepare the image to use with openCV " << endl;
		fs << endl;
		fs << "		this->data2 = cprx2->getImageData();" << endl;
		fs << "		colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat(this->data2->description->format);" << endl;
		fs << "		if (!fmt2)" << endl;
		fs << "			throw \"Format not supported\";" << endl;
		fs << endl;  
		fs << "		this->image2 = new colorspaces::Image (this->data2->description->width, this->data2->description->height, fmt2, &(this->data2->pixelData[0])); // Prepare the image to use with openCV" << endl;
	    fs << endl;
	    fs << "}" << endl;
		fs << endl;

		fs << "void Control::resetControl(){" << endl;
		fs << endl;
      	fs << "this->mprx->setV(0);" << endl;
        fs << "this->mprx->setW(0);" << endl;
		fs << "this->mprx->setL(0);" << endl;
		fs << endl;
		fs << "}" << endl;
		fs << endl;
   		
      	fs << "Control::~Control() {}" << endl;
		fs << endl;
      	fs << "}" << endl;
		fs << endl;

		fs << "void *showGui(void*){" << endl;
   		fs << "		mycomponent::Gui *gui;" << endl;
   		fs << "		struct timeval a, b;	" << endl;
   		fs << "		int cycle = 100;" << endl;
   		fs << "		long totalb,totala;" << endl;
   		fs << "		long diff;" << endl;
		fs << endl;
		fs << "using namespace std;" << endl;
		fs << "		string tab=\"\";" << endl;
		fs << endl;

   		fs << "		gui = new mycomponent::Gui(control);" << endl;
		fs << endl;

   		fs << "		while(true){" << endl;
		fs << endl;
		fs << "			gettimeofday(&a,NULL);" << endl;
      	fs << "			totala=a.tv_sec*1000000+a.tv_usec;" << endl;
      	fs << endl;
		fs << "			system(\"clear\");" << endl;
      	fs << endl;
//**********************Se genera el gui automatico**********************************************************************************************

		root = xmlDocGetRootElement (doc);

		nodeS = root->xmlChildrenNode;

		while (strcmp((const char*)nodeS->name, "SubAutomata")==0) {

		   	nodeE = nodeS->xmlChildrenNode;
			idSub = atoi((const char*)xmlNodeGetContent (nodeE));

			nodeE = nodeE->next;
			idPadre = atoi((const char*)xmlNodeGetContent (nodeE));

			if (idPadre == 0){
				fs << "			printf(\"%s\\n\", Nombres_Sub_"+int2string(idSub)+"[Sub_"+int2string(idSub)+"]);" << endl;
				fs << "			tab = tab+\"   \";" << endl;
				fs << endl;
			}else{
				fs << "			if (Sub_"+int2string(idSub)+"%2 == 0){" << endl; //Si no esta en un estado "_ghost"
				fs << "				printf(\"%s%s\\n\", tab.c_str(), Nombres_Sub_"+int2string(idSub)+"[Sub_"+int2string(idSub)+"/2]);" << endl;
				fs << "				tab = tab+\"   \";" << endl;
				fs << "			}" << endl;
				fs << endl;
			}

			nodeS = nodeS->next;
		}

      	fs << "			tab = \"\";" << endl;
	
      //Controll->update();
      //fs << "			control->handleCameras();" << endl;
      //fs << "			gui->display(*control->image1,*control->image2);" << endl;
      //fs << endl;
//**********************FIN Se genera el gui automatico******************************************************************************************

      
      	fs << "			gettimeofday(&b,NULL);" << endl;
      	fs << "			totalb=b.tv_sec*1000000+b.tv_usec;" << endl;
      //std::cout << "Introrob takes " << (totalb-totala)/1000 << " ms" << std::endl;
      	fs << "			diff = (totalb-totala)/1000;" << endl;
		fs << endl;

      	fs << "			if(diff < 0 || diff > cycle)" << endl;
        fs << "				diff = cycle;" << endl;
        fs << "			else" << endl;
        fs << "				diff = cycle-diff;" << endl;
		fs << endl;
      
      //Sleep Algorithm
      	fs << "			usleep(diff*1000);" << endl;
      	fs << "			if(diff < 33)" << endl;
        fs << "				usleep(33*1000);" << endl;
   		fs << "		}" << endl;
		fs << "}" << endl;
		fs << endl;
		fs << endl;
		fs << endl;

//---------------ITERAR PARA CREAR EL MOTOR TEMPORAL/CONTROL DE CADA HILO**********************************

	root = xmlDocGetRootElement (doc);

	nodeS = root->xmlChildrenNode;

	while (strcmp((const char*)nodeS->name, "SubAutomata")==0) {

		ListaNombres.clear();		

		nodeE = nodeS->xmlChildrenNode;

		idSub = atoi((const char*)xmlNodeGetContent (nodeE));

		nodeE = nodeE->next;

		idPadre = atoi((const char*)xmlNodeGetContent (nodeE));

		nodeE = nodeE->next;//Nos posicionamos en el primer estado

		//Saltamos todos los estados
		while (strcmp((const char*)nodeE->name, "Estado")==0){
			nodeE = nodeE->next;
		}

		nodeE = nodeE->next;//Pasamos los interfaces ICE
		nodeE = nodeE->next;//Pasamos las variables auxiliares
		nodeE = nodeE->next;//Nos colocamos en las funciones auxiliares
		
		funAux = (char*)xmlNodeGetContent (nodeE);

		//Hemos ido hasta las funciones aux porque hay que ponerlas antes del motor del hilo, sino no compilará
		fs << funAux <<endl;

		//Despues de eso volvemos a la posición adecuada para continuar componiento el motor de cada hilo
		nodeE = nodeS->xmlChildrenNode;//Saltamos idSub...
		nodeE = nodeE->next;//... y idPadre
		nodeE = nodeE->next;//Nos posicionamos en el primer estado y ya podemos continuar

		fs << "void *motor_sub_"+int2string(idSub)+"(void*){" << endl;//Cabecera del motor del subautomata
		fs << endl;

		nombre_id t;

		while (strcmp((const char*)nodeE->name, "Estado")==0){

			nodeN = nodeE->xmlChildrenNode;

			id = (const char*)xmlNodeGetContent (nodeN);

			t.id = id;

			while (strcmp((const char *)nodeN->name, (const char *)("nombre")) != 0){
				nodeN = nodeN->next;
			}

			nombre = (const char*)xmlNodeGetContent (nodeN);

			if (nombre != "")
			{
				t.nombre = nombre;
			}
			else{
				t.nombre = "default_" + id;
			}
			
			nodeE = nodeE->next;

			ListaNombres.push_back(t);

		}

		//Salimos posicionados en el tiempo de iteración

		ciclo = atoi((const char*)xmlNodeGetContent (nodeE)); //Ciclo de iteracion del subautomata

		nodeE = nodeE->next;//Pasamos los interfaces ICE
		nodeE = nodeE->next;//Nos colocamos en las variables auxiliares

		varAux = (char*)xmlNodeGetContent (nodeE);

		nodeE = nodeE->next;//Nos colocamos en las funciones auxiliares
		
		//funAux = (char*)xmlNodeGetContent (nodeE);

		fs << "		struct timeval a, b;" << endl;
	    fs << "		int cycle="+int2string(ciclo)+";" << endl;
	    fs << "		long totalb,totala;" << endl;
	    fs << "		long diff;" << endl;
		fs << "		time_t t_ini;"<< endl;
		fs << "		time_t t_fin;"<< endl;
		fs << "		double secs;" << endl;
		fs << "		bool t_activated;" << endl;
		fs << endl;
		fs << "		" << varAux << endl;
		fs << endl;
		fs << endl;
		fs << "		while(true){" << endl;
		fs << "		gettimeofday(&a,NULL);" << endl;
		fs << "		totala=a.tv_sec*1000000+a.tv_usec;" << endl;
		fs << endl;
		

		if (idPadre != 0) { //Si es el subautomata raiz

			fs << "		if (Sub_"+int2string(idPadre)+"=="+DameNombrePadre(doc,idPadre,idSub)+"){" << endl;
			fs << "			if(";

			pos = ListaNombres.begin();	

			fs << "Sub_"+int2string(idSub)+"=="+pos->nombre+"_ghost";

			pos++;

			while (pos != ListaNombres.end()) {

				fs << " || Sub_"+int2string(idSub)+"=="+pos->nombre+"_ghost";
				pos++;

			}

			fs << "){" << endl;
			fs << "			Sub_"+int2string(idSub)+"= (Estado_Sub_"+int2string(idSub)+")(Sub_"+int2string(idSub)+"-1);" << endl;
			fs << "		}" << endl;

		}	
		
			
		fs << "		//Switch de evaluación de transiciones" << endl;
		fs << "		switch (Sub_"+int2string(idSub)+"){" << endl;

		pos = ListaNombres.begin();	

		nodeE = nodeS->xmlChildrenNode;//Volvemos a recorrer el subautomata		
		nodeE = nodeE->next;
		nodeE = nodeE->next;//Nos posicionamos en el primer estado					

		while (pos != ListaNombres.end()){
			fs << "			case "+pos->nombre+":" << endl;
			fs << "			{" << endl;
			
			nodeN = nodeE->xmlChildrenNode;

			while (strcmp((const char *)nodeN->name, (const char *)("transiciones")) != 0){
				nodeN = nodeN->next;
			}//Nos colocamos en las transiciones del estado

			nodeT = nodeN->xmlChildrenNode;

			if (nodeT!=NULL){
		
				while (nodeT!=NULL){
					id = (const char*)xmlNodeGetContent (nodeT->xmlChildrenNode->next->next);
					if (nodeT->xmlChildrenNode->next->next->next != NULL){
						if(strcmp((const char*)nodeT->xmlChildrenNode->next->next->next->name,"codigo") == 0)
						{
							codigo = (const char*)xmlNodeGetContent (nodeT->xmlChildrenNode->next->next->next);
							fs << "					if (" << codigo << "){" << endl;
						//	fs << "						this->ventana->change_nodo_color (Estado_Actual, 3);" << endl;
						//	fs << "						Estado_Anterior = Estado_Actual;" << endl;
							fs << "						Sub_"+int2string(idSub)+"="<<  DameNombre(id) << ";" << endl;
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
							fs << "							Sub_"+int2string(idSub)+"="<<  DameNombre(id) << ";" << endl;
							fs << "							t_activated = false;" << endl;
						
							fs << "						}" << endl;
							fs << "					}" << endl;
						
						}
					}
					nodeT = nodeT->next;
				}//fin while de transiciones
			
			}

			fs << "				break;" << endl;
			fs << "			}" << endl;

			pos++;
			nodeE = nodeE->next;				

		}//fin while listaNombres para switch de evaluacion de transiciones

		fs << "		}" << endl; //Para cerrar el switch


		//Volvemos al principio del subautomata para hacer el switch de actuacion
		pos = ListaNombres.begin();	

		nodeE = nodeS->xmlChildrenNode;//Volvemos a recorrer el subautomata		
		nodeE = nodeE->next;
		nodeE = nodeE->next;//Nos posicionamos en el primer estado

		fs << "		//Switch de actuacion" << endl;
		fs << "		switch (Sub_"+int2string(idSub)+"){" << endl;

		while (pos != ListaNombres.end()){

			fs << "			case "+pos->nombre+":" << endl;
			fs << "			{" << endl;

			nodeN = nodeE->xmlChildrenNode;

			while (strcmp((const char *)nodeN->name, (const char *)("codigo")) != 0){
				nodeN = nodeN->next;
			}//Nos colocamos en el codigo del estado
		
			codigo = (const char*)xmlNodeGetContent (nodeN);

			fs << "			" << codigo << endl;

			fs << "				break;" << endl;
			fs << "			}" << endl;		

			pos++;
			nodeE = nodeE->next;		

		}//fin while listaNombres para el switch de actuacion

		fs << "		}" << endl; //Para cerrar el switch
			
			
		if (idPadre != 0) {
		
			fs << "}else{" << endl;
			fs << "		if(";

			pos = ListaNombres.begin();	

			fs << "Sub_"+int2string(idSub)+"=="+pos->nombre;

			pos++;

			while (pos != ListaNombres.end()) {

				fs << " || Sub_"+int2string(idSub)+"=="+pos->nombre;
				pos++;

			}

			fs << "){" << endl;
			fs << "			Sub_"+int2string(idSub)+"= (Estado_Sub_"+int2string(idSub)+")(Sub_"+int2string(idSub)+"+1);" << endl;
			fs << "		}" << endl;
			fs << "	}" << endl;

		}

		fs << endl;
		fs << endl;
		fs << "//Motor temporal del subautomata" << endl;
		fs << endl;

		fs << "		gettimeofday(&b,NULL);" << endl;
		fs << "		totalb=b.tv_sec*1000000+b.tv_usec;" << endl;
		fs << "		diff = (totalb-totala)/1000;" << endl;
		fs << "		if(diff < 0 || diff > cycle)" << endl;
		fs << "			diff = cycle;" << endl;
		fs << "		else" << endl;
		fs << "			diff = cycle-diff;" << endl;
		fs << endl;
		fs << "		//Sleep Algorithm" << endl;
		fs << "			usleep(diff*1000);" << endl;
		fs << "		if(diff < 33)" << endl;
		fs << "			usleep(33*1000);" << endl;
		fs << "	 }" << endl;
		fs << "}" << endl; 
		fs << endl;
		fs << endl;
		
		nodeS = nodeS->next;

	}//fin while subautomata


//---------------FIN DE ITERAR PARA CREAR EL MOTOR TEMPORAL/CONTROL DE CADA HILO***************************

		fs << "int main(int argc, char** argv){" << endl;
		fs << endl;

		fs << endl;

		fs << "		int status;" << endl;
		fs << "		Ice::CommunicatorPtr ic;" << endl;
 	    fs << "		struct timeval a, b;" << endl;
		fs << "		int cycle = 300;" << endl;
		fs << "		long totalb,totala;" << endl;
		fs << "		long diff;" << endl;
		fs << "		bool guiActivated=0;" << endl;
		fs << "		bool controlActivated=0;" << endl;
		fs << endl;


			//th_data data_1;
			//th_data data_2;
			//th_data data_3;
		   
		fs << "		control = new mycomponent::Control();" << endl;
		fs << endl;
		fs << "		pthread_mutex_init(&control->controlGui, NULL);" << endl;

		fs << "		controlActivated=1;" << endl;
		fs << "	    guiActivated=1;" << endl;

		//if(guiActivated){
      	fs << "		pthread_create(&control->thr_gui, NULL, &showGui, NULL);" << endl;//OBJETIVo2  
   		//}
		
		fs << "	  try{" << endl;
		fs << endl;
      
		fs << "		ic = Ice::initialize(argc,argv);" << endl;
		fs << "			//-----------------ICE----------------//" << endl;

//--------Se cargan los interfaces que usen todos los subautomatas en conjunto********

		
		interIce.laser = false;
		interIce.motor = false;
		interIce.radar = false;
		interIce.encoders = false;
		interIce.lat_lon = false;
		interIce.camara = false;
		interIce.ptencoders = false;

		root = xmlDocGetRootElement (doc);

		nodeS = root->xmlChildrenNode;

		while (strcmp((const char*)nodeS->name, "SubAutomata")==0) {

			nodeE = nodeS->xmlChildrenNode;

			while (strcmp((const char*)nodeE->name, "Librerias")!=0) {
				nodeE = nodeE->next;
			}

			nodeT = nodeE->xmlChildrenNode;
			
			string interface;
			while (nodeT != NULL){
				interface = (const char*)xmlNodeGetContent (nodeT);	
				if (interface == "laser")
					interIce.laser = true;
				if (interface == "motor")
					interIce.motor = true;
				if (interface == "radar")
					interIce.radar = true;
				if (interface == "encoders")
					interIce.encoders = true;
				if (interface == "lat_lon")
					interIce.lat_lon = true;
				if (interface == "camara")
					interIce.camara = true;
				if (interface == "ptencoders")
					interIce.ptencoders = true;
		
				nodeT = nodeT->next;
			}

			nodeS = nodeS->next;

		}

		//Añadimos los interfaces que hemos detectado como necesarios
		if (interIce.laser == true){
			fs << "		// Contact to LASER interface" << endl;
			fs << "		Ice::ObjectPrx baseLaser = ic->propertyToProxy(\"Mycomponent.Laser.Proxy\");" << endl;
			fs << "		if (0==baseLaser)" << endl;
			fs << "		throw \"Could not create proxy with laser\";" << endl;
			fs << endl;
			fs << "		// Cast to laser" << endl;
			fs << "		control->lprx = jderobot::LaserPrx::checkedCast(baseLaser);" << endl;
			fs << "		if (0== control->lprx)" << endl;
			fs << "			throw \"Invalid proxy Mycomponent.Laser.Proxy\";" << endl;
			fs << endl;
		}

		if (interIce.motor == true){
			fs << "		// Contact to MOTORS interface" << endl;
			fs << "		Ice::ObjectPrx baseMotors = ic->propertyToProxy(\"Mycomponent.Motors.Proxy\");" << endl;
			fs << "		if (0==baseMotors)" << endl;
			fs << "			throw \"Could not create proxy with motors\";" << endl;
			fs << endl;
			fs << "		// Cast to motors" << endl;
			fs << "		 control->mprx = jderobot::MotorsPrx::checkedCast(baseMotors);" << endl;
			fs << "		if (0== control->mprx)" << endl;
			fs << "			throw \"Invalid proxy Mycomponent.Motors.Proxy\";" << endl;
			fs << endl;
		}

		//if (interIce.radar == true){
		//}

		if (interIce.encoders == true){
			fs << "		// Contact to ENCODERS interface" << endl;
			fs << "		Ice::ObjectPrx baseEncoders = ic->propertyToProxy(\"Mycomponent.Encoders.Proxy\");" << endl;
			fs << "		if (0==baseEncoders)" << endl;
			fs << "			throw \"Could not create proxy with encoders\";" << endl;
			fs << endl;
			fs << "		// Cast to encoders" << endl;
			fs << "		control->eprx = jderobot::EncodersPrx::checkedCast(baseEncoders);" << endl;
			fs << "		if (0== control->eprx)" << endl;
			fs << "			throw \"Invalid proxy Mycomponent.Encoders.Proxy\";" << endl;
			fs << endl;
		}

		if (interIce.lat_lon == true){
			fs << "		// Contact to PTMOTORS interface" << endl;
			fs << "		Ice::ObjectPrx ptmotors1 = ic->propertyToProxy(\"Mycomponent.PTMotors1.Proxy\");" << endl;
			fs << "		if (0==ptmotors1)" << endl;
			fs << "			throw \"Could not create proxy with motors\";" << endl;
			fs << endl;
			fs << "		// Cast to ptmotors" << endl;
			fs << "		control->ptmprx1 = jderobot::PTMotorsPrx::checkedCast(ptmotors1);" << endl;
			fs << "		if (0== control->ptmprx1)" << endl;
			fs << "			throw \"Invalid proxy Mycomponent.PTMotors1.Proxy\";" << endl;
			fs << endl;
			fs << "		// Contact to PTMOTORS interface" << endl;
			fs << "		Ice::ObjectPrx ptmotors2 = ic->propertyToProxy(\"Mycomponent.PTMotors2.Proxy\");" << endl;
			fs << "		if (0==ptmotors2)" << endl;
			fs << "			throw \"Could not create proxy with motors\";" << endl;
			fs << "		// Cast to ptmotors" << endl;
			fs << endl;
			fs << "		control->ptmprx2 = jderobot::PTMotorsPrx::checkedCast(ptmotors2);" << endl;
			fs << "		if (0== control->ptmprx2)" << endl;
			fs << "			throw \"Invalid proxy Mycomponent.PTMotors2.Proxy\";" << endl;
			fs << endl;
		}

		if (interIce.camara == true){
			fs << "		// Get driver camera" << endl;
			fs << "		Ice::ObjectPrx camara1 = ic->propertyToProxy(\"Mycomponent.Camera1.Proxy\");" << endl;
			fs << "		if (0==camara1)" << endl;
			fs << "			throw \"Could not create proxy to camera1 server\";" << endl;
			fs << endl;
			fs << "		// cast to CameraPrx" << endl;
			fs << "		control->cprx1 = jderobot::CameraPrx::checkedCast(camara1);" << endl;
			fs << "		if (0== control->cprx1)" << endl;
			fs << "			throw \"Invalid proxy\";" << endl;
			fs << endl;
			fs << "		// Get driver camera" << endl;
			fs << "		Ice::ObjectPrx camara2 = ic->propertyToProxy(\"Mycomponent.Camera2.Proxy\");" << endl;
			fs << "		if (0==camara2)" << endl;
			fs << "			throw \"Could not create proxy to camera2 server\";" << endl;
			fs << endl;
			fs << "		// cast to CameraPrx" << endl;
			fs << "		control->cprx2 = jderobot::CameraPrx::checkedCast(camara2);" << endl;
			fs << "		if (0== control->cprx2)" << endl;
			fs << "			throw \"Invalid proxy\";" << endl;
			fs << endl;
		}

		if (interIce.ptencoders == true){
			fs << "		// Contact to PTENCODERS interface" << endl;
			fs << "		Ice::ObjectPrx ptencoders1 = ic->propertyToProxy(\"Mycomponent.PTEncoders1.Proxy\");" << endl;
			fs << "		if (0==ptencoders1)" << endl;
			fs << "			throw \"Could not create proxy with encoders\";" << endl;
			fs << endl;
			fs << "		// Cast to encoders" << endl;
			fs << "		control->pteprx1 = jderobot::PTEncodersPrx::checkedCast(ptencoders1);" << endl;
			fs << "		if (0== control->pteprx1)" << endl;
			fs << "			throw \"Invalid proxy Mycomponent.PTEncoders1.Proxy\";" << endl;
			fs << endl;
			fs << "		// Contact to PTENCODERS interface" << endl;
			fs << "		Ice::ObjectPrx ptencoders2 = ic->propertyToProxy(\"Mycomponent.PTEncoders2.Proxy\");" << endl;
			fs << "		if (0==ptencoders2)" << endl;
			fs << "			throw \"Could not create proxy with encoders\";" << endl;
			fs << endl;
			fs << "		// Cast to encoders" << endl;
			fs << "		control->pteprx2 = jderobot::PTEncodersPrx::checkedCast(ptencoders2);" << endl;
		  	fs << "		if (0== control->pteprx2)" << endl;
			fs << "			throw \"Invalid proxy Mycomponent.PTEncoders2.Proxy\";" << endl;
			fs << endl;
		}
		

//------------------------------fin carga interfaces ICE *****************************

		
		fs << "				//-----------------ICE----------------//" << endl;
		fs << "//****************************** Processing the Control ******************************///" << endl;
		fs << "			//---------------- ITERATIONS CONTROL -----------//" << endl;
		fs << endl;		
		fs << endl;
      	fs << "//Inicializacion de datos para los distintos hilos de control y creacion de hilos" << endl;


		root = xmlDocGetRootElement (doc);

		nodeS = root->xmlChildrenNode;

		while (strcmp((const char*)nodeS->name, "SubAutomata")==0) {
			
			nodeE = nodeS->xmlChildrenNode;

			idSub = atoi((const char*)xmlNodeGetContent (nodeE));

			fs << "		pthread_create(&control->thr_sub_"+int2string(idSub)+",NULL, &motor_sub_"+int2string(idSub)+", NULL);" << endl;

			nodeS = nodeS->next;
			
		}

		fs << endl;
		fs << endl;
		fs << "		pthread_join(control->thr_gui, NULL);" << endl;
		fs << endl;

		root = xmlDocGetRootElement (doc);

		nodeS = root->xmlChildrenNode;

		while (strcmp((const char*)nodeS->name, "SubAutomata")==0) {
			
			nodeE = nodeS->xmlChildrenNode;

			idSub = atoi((const char*)xmlNodeGetContent (nodeE));

			fs << "		pthread_join(control->thr_sub_"+int2string(idSub)+",NULL);" << endl;

			nodeS = nodeS->next;
			
		}

		fs << endl;		
		fs << endl;
      	fs << "	 }//del try" << endl;
		fs << "	 catch (const Ice::Exception& ex) {" << endl;
		fs << "		std::cerr << ex << std::endl;" << endl;
		fs << "		status = 1;" << endl;
		fs << "	 }" << endl;
		fs << "  catch (const char* msg) {" << endl;
      	fs << "		std::cerr << msg << std::endl;" << endl;
		fs << "		status = 1;" << endl;
		fs << "  }" << endl;
		fs << endl;
		fs << "if (ic)" << endl;
		fs << "	 ic->destroy();" << endl;
		fs << endl;
		fs << "return 0;" << endl;
		fs << endl;
      	fs << "}" << endl;
		fs << endl;

		
		fs.close();
		
		return 0;
	}else{
		return 1;
	}
	
}

int configuracionICE (string path)
{
	fstream fs; 
	
	fs.open(path.c_str(), ofstream::out);
	if (fs.is_open()){

		if (interIce.motor == true){
			fs << "Mycomponent.Motors.Proxy=motors1:tcp -h localhost -p 9999" << endl;
		}

		if (interIce.camara == true){
			fs << "Mycomponent.Camera1.Proxy=cameraA:tcp -h localhost -p 9999" << endl;
			fs << "Mycomponent.Camera2.Proxy=cameraB:tcp -h localhost -p 9999" << endl;				
		}

		//if (interIce.radar == true){
		//}

		if (interIce.encoders == true){
			fs << "Mycomponent.Encoders.Proxy=encoders1:tcp -h localhost -p 9999" << endl;
		}

		if (interIce.lat_lon == true){
			fs << "Mycomponent.PTMotors1.Proxy=ptmotors1:tcp -h localhost -p 9999" << endl;
			fs << "Mycomponent.PTMotors2.Proxy=ptmotors2:tcp -h localhost -p 9999" << endl;
		}

		if (interIce.laser == true){
			fs << "Mycomponent.Laser.Proxy=laser1:tcp -h localhost -p 9999" << endl;
		}

		if (interIce.ptencoders == true){
			fs << "Mycomponent.PTEncoders1.Proxy=ptencoders1:tcp -h localhost -p 9999" << endl;
			fs << "Mycomponent.PTEncoders2.Proxy=ptencoders2:tcp -h localhost -p 9999" << endl;	
		}				

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

	string rutaAcopiar="";


	
	
	string path;

	char *filename = argv[1];

	//strcpy(filename, "EstoEsUnaPrueba3.xml");
	
	printf("ARG: %s %s\n", argv[0], argv[1]);

	doc = xmlParseFile (argv[1]);
	


	/* Nombre y Directorio Esquema */


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

	printf("Directorio: %s\n",directorio.c_str());

	for (i=0; i < (int)fichero.length(); i++)
  	{
   		if (fichero.at(i) == '.')
			pos = i;
 	}	
		
	esquema = fichero.substr (0,pos);	// Nombre del fichero sin extension
	printf("Esquema: %s\n",esquema.c_str());	

	if (!doc) {
	  	printf("Error al cargar documento XML\n");
	}
	else{
		printf("Antes de importar codigo\n");
		//importarCodigoAux(doc);
		printf("Despues de importar codigo\n");
		path = directorio+ "src";

		mkdir(path.c_str(), 0777);
		printf("Creado path\n");
		//char* aCopiar = "";
		//strcpy (aCopiar,"cp -R ./datos_esquemas/my_component/* ");
  		//strcat (aCopiar,path.c_str());
		//printf ("Orden cp: %s",aCopiar);

		rutaAcopiar = "cp -R ./datos_esquemas/my_component/* "+directorio+"src";

		system(rutaAcopiar.c_str());

		//copiar_ficheros ("./datos_esquemas/my_component/", directorio+"src/");//-------------------------------------cambiada ubicacion
		
		path = directorio+ "src/control_class.h"; //----------------------------------------------------------------cambiado el nombre del .h
		navegaH (path, doc);
		
		path = directorio+ "/src/control_class.cpp";//----------------------------------------------------------cambiado nombre del .cpp
		navegaCPP (path, doc);

		path = directorio+ "/src/mycomponent.cfg";
		configuracionICE (path);
		
		printf("Ficheros Generados \n");

	}	
	
	return 0;
}
