/*
 *  Copyright (C) 1997-2012 JDE Developers Teameldercare.camRGB
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 *  Authors : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>
			
 */

#include "myparser.h"
#include <iostream>
#include "libglademm.h"
#include <iostream>
#include <fstream>


std::string typeInputBox="inputbox";
std::string typeToggle="toggle";
std::string typeButton="button";
Gtk::Button *button;
Gtk::HBox * w_main ;
std::ofstream outFile("salida.xml"); 


namespace eldercare{
	myparser::myparser(jderobot::remoteConfigPrx configPrx, int id){
		this->prx=configPrx;
		this->id=id;
	
	}

	myparser::~myparser(){
	}



void myparser::parse_sec_nodes(const xmlpp::Node* node, Gtk::VBox *parent)
{
  
	const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
	const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
	const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);

	if(nodeText && nodeText->is_white_space()) 
    	return;
    
  	const Glib::ustring nodename = node->get_name();
	//std::cout << nodename << std::endl;

  	if(!nodeText && !nodeComment && !nodename.empty()) 
  	{
		const Glib::ustring namespace_prefix = node->get_namespace_prefix();
		if(namespace_prefix.empty()){
			xmlpp::Node::NodeList list = node->get_children();
			//std::cout << "size: " << list.size() << std::endl;
			if (list.size() > 1){
				Gtk::VBox * w_temp = new Gtk::VBox();
				w_temp->set_name(node->get_name());
				parent->pack_start(*w_temp,true, true, 0);
				Gtk::Label *label= new Gtk::Label(nodename.c_str());
				w_temp->pack_start(*label,true, true, 0);
				for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
				{
	  				parse_sec_nodes(*iter, w_temp); 
				}
				Gtk::HSeparator *w_Hseparator=new Gtk::HSeparator();
				w_temp->pack_start(*w_Hseparator,true, true, 0);
			}
			else{
				const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*((node)->get_children().begin()));
				std::istringstream sTemp(nodeTextValue->get_content());
				//std::cout << "test: " << sTemp.str() << std::endl;
				if (typeInputBox.compare(sTemp.str().c_str()) == 0 ){
					Gtk::HBox * hbox = new Gtk::HBox();
					hbox->set_name(nodename.c_str());
					parent->pack_start(*hbox,true, true, 0);
					Gtk::Label *label= new Gtk::Label(nodename.c_str());
					Gtk::VSeparator *w_Vseparator=new Gtk::VSeparator();
					hbox->pack_start(*label,true, true, 0);
					hbox->pack_start(*w_Vseparator,true, true, 0);
					Gtk::Entry* entry = new Gtk::Entry();
					hbox->pack_start(*entry,false, false, 0);
				}
				else if (typeToggle.compare(sTemp.str().c_str()) == 0 ){
					Gtk::HBox * hbox = new Gtk::HBox();
					hbox->set_name(nodename.c_str());
					parent->pack_start(*hbox,true, true, 0);
					Gtk::Label *label= new Gtk::Label(nodename.c_str());
					Gtk::VSeparator *w_Vseparator=new Gtk::VSeparator();
					hbox->pack_start(*label,true, true, 0);
					hbox->pack_start(*w_Vseparator,true, true, 0);
					Gtk::CheckButton*button = new Gtk::CheckButton("checka");
					hbox->pack_start(*button,true, true, 0);
				}
				else if (typeButton.compare(sTemp.str().c_str()) == 0 ){
					//std::cout << "_-------------------------------" << std::endl;
					Gtk::HBox * hbox = new Gtk::HBox();
					hbox->set_name(nodename.c_str());
					parent->pack_start(*hbox,true, true, 0);
					Gtk::Label *label= new Gtk::Label(nodename.c_str());
					Gtk::VSeparator *w_Vseparator=new Gtk::VSeparator();
					hbox->pack_start(*label,true, true, 0);
					hbox->pack_start(*w_Vseparator,true, true, 0);
					//Gtk::Button *button  //test para prueba acceso jerarquico
					button = new Gtk::Button("save");
					hbox->pack_start(*button,true, true, 0);
				}
				else{
					//std::cout << "Invalid widget" << std::endl;
				}
			}
		}
    	else{
    		//std::cout << "Node name = " << namespace_prefix << ":" << nodename << std::endl;
		}
  	}
}




void myparser::parse_ppal_nodes(const xmlpp::Node* node, Gtk::HBox *parent)
{
  
	const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
	const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
	const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);

	if(nodeText && nodeText->is_white_space()) 
    	return;
    
  	const Glib::ustring nodename = node->get_name();
	//std::cout << nodename << std::endl;

  	if(!nodeText && !nodeComment && !nodename.empty()) 
  	{
		const Glib::ustring namespace_prefix = node->get_namespace_prefix();
		if(namespace_prefix.empty()){
			xmlpp::Node::NodeList list = node->get_children();
			//std::cout << "size: " << list.size() << std::endl;
			if (list.size() > 1){
				Gtk::VBox * w_temp = new Gtk::VBox();
				w_temp->set_name(node->get_name());
				parent->pack_start(*w_temp,true, true, 0);
				Gtk::Label *label= new Gtk::Label(nodename.c_str());
				w_temp->pack_start(*label,true, true, 0);
				for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
				{
	  				parse_sec_nodes(*iter, w_temp); 
				}
				Gtk::HSeparator *w_Hseparator=new Gtk::HSeparator();
				w_temp->pack_start(*w_Hseparator,true, true, 0);
			}
			else{
				const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*((node)->get_children().begin()));
				std::istringstream sTemp(nodeTextValue->get_content());
				//std::cout << "test: " << sTemp.str() << std::endl;
				if (typeInputBox.compare(sTemp.str().c_str()) == 0 ){
					Gtk::HBox * hbox = new Gtk::HBox();
					hbox->set_name(nodename.c_str());
					parent->pack_start(*hbox,true, true, 0);
					Gtk::Label *label= new Gtk::Label(nodename.c_str());
					Gtk::VSeparator *w_Vseparator=new Gtk::VSeparator();
					hbox->pack_start(*label,true, true, 0);
					hbox->pack_start(*w_Vseparator,true, true, 0);
					Gtk::Entry* entry = new Gtk::Entry();
					hbox->pack_start(*entry,false, false, 0);
				}
				else if (typeToggle.compare(sTemp.str().c_str()) == 0 ){
					Gtk::HBox * hbox = new Gtk::HBox();
					hbox->set_name(nodename.c_str());
					parent->pack_start(*hbox,true, true, 0);
					Gtk::Label *label= new Gtk::Label(nodename.c_str());
					Gtk::VSeparator *w_Vseparator=new Gtk::VSeparator();
					hbox->pack_start(*label,true, true, 0);
					hbox->pack_start(*w_Vseparator,true, true, 0);
					Gtk::CheckButton*button = new Gtk::CheckButton("checka");
					hbox->pack_start(*button,true, true, 0);
				}
				else if (typeButton.compare(sTemp.str().c_str()) == 0 ){
					//std::cout << "_-------------------------------" << std::endl;
					Gtk::HBox * hbox = new Gtk::HBox();
					hbox->set_name(nodename.c_str());
					parent->pack_start(*hbox,true, true, 0);
					Gtk::Label *label= new Gtk::Label(nodename.c_str());
					Gtk::VSeparator *w_Vseparator=new Gtk::VSeparator();
					hbox->pack_start(*label,true, true, 0);
					hbox->pack_start(*w_Vseparator,true, true, 0);
					//Gtk::Button *button  //test para prueba acceso jerarquico
					button = new Gtk::Button("save");
					hbox->pack_start(*button,true, true, 0);
				}
				else{
					//std::cout << "Invalid widget" << std::endl;
				}
			}
		}
    	else{
    		//std::cout << "Node name = " << namespace_prefix << ":" << nodename << std::endl;
		}
  	}
}




int myparser::parsePath(std::string filepath)
{

	/*	set_border_width(10);
	Gtk::VBox * w_main = new Gtk::VBox();
	Gtk::Button* m_button = new Gtk::Button("Hello World");
	Gtk::ToggleButton *w_reconstruct = new Gtk::ToggleButton("test");
	Gtk::HSeparator *w_Hseparator=new Gtk::HSeparator();


  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  	

	add(*w_main);

  // This packs the button into the Window (a container).
  	//add(*m_button);
	w_main->pack_start(*m_button,true, true, 0);
	w_main->pack_start(*w_Hseparator,true, true, 0);
	w_main->pack_start(*w_reconstruct,true, true, 0);

	show_all_children();*/

	
	std::cout << "File to parse: " << filepath << std::endl;


	w_main = new Gtk::HBox();
	//w_main->set_name("main");
	add(*w_main);
  
	#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
	try
	{
  		#endif //LIBXMLCPP_EXCEPTIONS_ENABLED 	


		xmlpp::DomParser parser;
    	parser.set_substitute_entities();
    	parser.parse_file(filepath);
    	if(parser)
    	{
      		const xmlpp::Node* pNode = parser.get_document()->get_root_node(); 
			set_title(pNode->get_name().c_str());
			w_main->set_name(pNode->get_name().c_str());

			xmlpp::Node::NodeList list = pNode->get_children();
			
			for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
			{
  				parse_ppal_nodes(*iter, w_main); 
			}
   	 	}
	#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
  	}
  	catch(const std::exception& ex)
  	{
    	std::cout << "Exception caught: " << ex.what() << std::endl;
  	}
  	#endif //LIBXMLCPP_EXCEPTIONS_ENABLED 

	show_all_children();


	std::cout <<  get_is_toplevel () << std::endl;
	std::cout <<  w_main->get_is_toplevel () << std::endl;

	button->signal_clicked().connect(sigc::mem_fun(this,&myparser::testF));


  	return 0;
}

int
myparser::imprime(Gtk::Container* w, int tab){
	Gtk::VBox compType; 
	std::string no1="gtkmm__GtkLabel";
	std::string no2="gtkmm__GtkVSeparator";
	std::string no3="GtkLabel";
	std::string no4="gtkmm__GtkHSeparator";
	/*std::string no2="";
	std::string no2="";
	std::string no2="";*/
	std::string checkString="gtkmm__GtkCheckButton";
	std::string entryString="gtkmm__GtkEntry";
 	std::string saveString="save";

	
	std::vector< Gtk::Widget*> ninos=w->get_children();
	for(std::vector< Gtk::Widget*>::iterator iter = ninos.begin(); iter != ninos.end(); ++iter)
	{
		if ((no1.compare((*iter)->get_name())) == 0 ){
			//std::cout << std::endl;
			continue;
		}
		else if ((no2.compare((*iter)->get_name())) == 0 ){
			//std::cout << std::endl;
			continue;
		}
		else if ((no3.compare((*iter)->get_name())) == 0 ){
			//std::cout << std::endl;
			continue;
		}
		else if ((no4.compare((*iter)->get_name())) == 0 ){
			//std::cout << std::endl;
			continue;
		}
		else if ((saveString.compare((*iter)->get_name())) == 0 ){
			//std::cout << std::endl;
			continue;
		}
		else if (((checkString.compare((*iter)->get_name())) == 0 )){
			if (((Gtk::CheckButton*)(*iter))->get_active())
				outFile << "1";					
			else
				outFile << "0";
			return 0;
		}
		else if (((entryString.compare((*iter)->get_name())) == 0 )){
			outFile << ((Gtk::Entry*)(*iter))->get_buffer()->get_text();
			return 0;
		}
		else{
			outFile << std::endl;

			for (int i=0;  i< tab; i++)
				outFile << "\t";
			outFile << "<" <<  (*iter)->get_name() << ">";
			if (imprime((Gtk::Container*)(*iter),tab+1)){
				for (int i=0;  i< tab; i++)
				outFile << "\t";
			}

			outFile << "</" <<  (*iter)->get_name() << ">"; //<< std::endl;
		}
	}
	outFile << std::endl;
	return 1;
}


void
myparser::testF(){
	//std::cout << "check" << std::endl;
	outFile << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << std::endl;
	outFile << "<" << w_main->get_name() << ">";
	imprime(w_main,1);
	outFile << "</" << w_main->get_name() << ">" << std::endl;
	outFile.close();
	char line[255];
	std::ifstream fileTest("out.xml");
	while(!fileTest.eof()){
		fileTest.getline(line, 255);
		std::cout << line << std::endl;
		this->prx->write(line,id);
	}
	this->prx->setConfiguration(id);
}

} //namespace
