/*
*  Copyright (C) 1997-2010 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *   Authors : Eduardo Perdices <eperdices@gsyc.es>,
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *             Alejandro Hernández Cordero <ahcorde@gmail.com>
 */

#include "view.h"

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>



namespace calibrator {

/*
Comentarios de procedimiento
*/

	View::View(Glib::RefPtr<Gtk::ListStore> m_refTreeModel, Module_DLT * module_dlt, Module_Extrinsics * module_extrinsics, Module_Rectifier * module_rectifier): gtkmain(0,0) {

		/*Init OpenGL*/
		if(!Gtk::GL::init_check(NULL, NULL))	{
			std::cerr << "Couldn't initialize GL\n";
			std::exit(1);
		}
		active_camera = 0;
		std::cout << "Loading glade\n";
		refXml = Gnome::Glade::Xml::create(std::string(GLADE_DIR) + std::string("/calibrator.glade"));



		/*Get widgets Panels*/
		refXml->get_widget("mainwindow",mainwindow);
		refXml->get_widget("image_patron",gtk_patron);
		refXml->get_widget("calibrator",mode[0]);
		refXml->get_widget("extrinsics",mode[1]);
		refXml->get_widget("rectifier",mode[2]);
		refXml->get_widget("estereo",mode[3]);
		refXml->get_widget("dlt_panel",dlt_panel);
		refXml->get_widget("extrinsics_panel",extrinsics_panel);
		refXml->get_widget("rectifier_panel",rectifier_panel);
		refXml->get_widget("estereo_panel",estereo_panel);


		/*Get matrix*/
		for(int i=1; i<4;i++){
			for(int j=1; j<5;j++){
				std::stringstream labelString;
				labelString << "c1k"<< int(i) << int(j);
				refXml->get_widget(labelString.str(),k[(i-1)*4+j-1]);


			}
		}

		for(int i=1; i<5;i++){
			for(int j=1; j<5;j++){
				std::stringstream labelString;
				labelString << "c1rt"<< int(i) << int(j);
				refXml->get_widget(labelString.str(),rt[(i-1)*4+j-1]);


			}
		}

		/* OpenGL World */
		refXml->get_widget_derived("gl_world",world);
		refXml->get_widget("button_load_world",button_Load_world);
		button_Load_world->signal_clicked().connect(sigc::mem_fun(this,&View::button_Load_word_clicked));
		

		/* Update World OpenGl*/
		world->setToCamera1();
		

		mode[0]->signal_toggled().connect(sigc::mem_fun(this,&View::on_toggled_calibrator));
		mode[1]->signal_toggled().connect(sigc::mem_fun(this,&View::on_toggled_extrinsics));
		mode[2]->signal_toggled().connect(sigc::mem_fun(this,&View::on_toggled_rectifier));
		mode[3]->signal_toggled().connect(sigc::mem_fun(this,&View::on_toggled_estereo));

		/* Get cameras and load camera combo */
		refXml->get_widget("camera_combo",camera_set);
		camera_set->set_model(m_refTreeModel);


		camera_set->set_active(0);
		camera_set->signal_changed().connect(sigc::mem_fun(this,&View::on_changed_camera_set));

		active_panel  = 0;

/* Calibrator */

		/*Create module_dlt*/
		this->module_dlt = module_dlt;
		this->module_dlt->get_widgets(refXml);

/* Extrinsics */
		this->module_extrinsics = module_extrinsics;
		this->module_extrinsics->get_widgets(refXml);
		this->module_extrinsics->set_mainwindow(mainwindow);
		

/* Module_Rectifier*/      

		this->module_rectifier = module_rectifier;
		this->module_rectifier->get_widgets(refXml);



		mainwindow->show();
/*
		xmlReader(&camera, "/home/caupolican/robotica/newnewnew/trunk/src/components/calibrator/calibration.xml");
		display_camerainfo(camera);
		xmlWriter(camera, "/home/caupolican/robotica/newnewnew/trunk/src/components/calibrator/calibration2.xml");
*/
	}

	View::~View() {
		delete this->module_dlt;
		delete this->module_extrinsics;
		delete this->module_rectifier;
	}

	void View::read_matrix() {

 try
   {
      xercesc::XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
   }
   catch( xercesc::XMLException& e )
   {
      char* message = xercesc::XMLString::transcode( e.getMessage() );
      cerr << "XML toolkit initialization error: " << message << endl;
      xercesc::XMLString::release( &message );
      // throw exception here to return ERROR_XERCES_INIT
   }

   xercesc::XercesDOMParser * m_ConfigFileParser = new xercesc::XercesDOMParser;

		std::string configFile = "/home/caupolican/robotica/newnewnew/trunk/src/components/calibrator/calibration.xml";

		xercesc::DOMDocument* xmlDoc = NULL;

		// Test to see if the file is ok.
		struct stat fileStatus;

		int iretStat = stat(configFile.c_str(), &fileStatus);
	
		if( iretStat == ENOENT )
			throw ( std::runtime_error("Path file_name does not exist, or path is an empty string.") );
		else if( iretStat == ENOTDIR )
			throw ( std::runtime_error("A component of the path is not a directory."));
		else if( iretStat == ELOOP )
			throw ( std::runtime_error("Too many symbolic links encountered while traversing the path."));
		else if( iretStat == EACCES )
			throw ( std::runtime_error("Permission denied."));
		else if( iretStat == ENAMETOOLONG )
			throw ( std::runtime_error("File can not be read\n"));

		// Configure DOM parser.
		m_ConfigFileParser->setValidationScheme( xercesc::XercesDOMParser::Val_Never );
		m_ConfigFileParser->setDoNamespaces( false );
		m_ConfigFileParser->setDoSchema( false );
		m_ConfigFileParser->setLoadExternalDTD( false );


		try {
			m_ConfigFileParser->parse( configFile.c_str() );

			// no need to free this pointer - owned by the parent parser object
			xmlDoc = m_ConfigFileParser->getDocument();

			// Get the top-level element: NAme is "root". No attributes for "root"
			xercesc::DOMElement* elementRoot = xmlDoc->getDocumentElement();
			if( !elementRoot ) throw(std::runtime_error( "empty XML document" ));

			// Parse XML file for tags of interest: "ApplicationSettings"
			// Look one level nested within "root". (child of root)
			xercesc::DOMNodeList*      children = elementRoot->getChildNodes();
			const  XMLSize_t nodeCount = children->getLength();


			// For all nodes, children of "root" in the XML tree.
			for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
			{
				std::cout << "Camera: " << xx << std::endl;	
				xercesc::DOMNode* currentNode = children->item(xx);

				if( currentNode->getNodeType() &&  // true is not NULL
				currentNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE ) // is element 
				{
					std::cout << "Node " << xercesc::XMLString::transcode(currentNode->getNodeName()) << std::endl;

					xercesc::DOMNodeList*      children_level2 = currentNode->getChildNodes();
					const  XMLSize_t nodeCount_level2 = children_level2->getLength();
					for( XMLSize_t yy = 0; yy < nodeCount_level2; ++yy )
					{			
						xercesc::DOMNode* currentNode_level2 = children_level2->item(yy);
						std::cout << "Level 2 " << xercesc::XMLString::transcode(currentNode_level2->getNodeName()) << std::endl;

					}

				}
			}
		}
		catch( xercesc::XMLException& e )
		{
			char* message = xercesc::XMLString::transcode( e.getMessage() );
			ostringstream errBuf;
			errBuf << "Error parsing file: " << message << flush;
			xercesc::XMLString::release( &message );
		}
	}


	bool View::isVisible(){
		return mainwindow->is_visible();
	}

	void View::display(const colorspaces::Image& image)
	{

		colorspaces::ImageRGB8 image_rgb8(image);//conversion will happen if needed
		Glib::RefPtr<Gdk::Pixbuf> imgBuff_rgb8 = Gdk::Pixbuf::create_from_data((const guint8*)image_rgb8.data,
			Gdk::COLORSPACE_RGB,
			false,
			8,
			image_rgb8.width,
			image_rgb8.height,
			image_rgb8.step); 
		gtk_patron->clear();
		gtk_patron->set(imgBuff_rgb8);


		// Execute selected module
		colorspaces::ImageRGB8 image_dlt = image_rgb8.clone();
		colorspaces::ImageRGB8 image_extrinsics = image_rgb8.clone();
		colorspaces::ImageRGB8 image_rectifier = image_rgb8.clone();


		switch (active_panel)
		{
		case 0:

		/* Calibrator */
				/*Manage image*/
				//		this->module_dlt->drawWorld(image);
				/*Set image */
				
				this->module_dlt->display(image_dlt);
				camera = this->module_dlt->getCam();
		break;
		case 1:
		/* Extrinsics */
				/*Set image*/
				this->module_extrinsics->display(image_extrinsics);
				camera = this->module_extrinsics->getCam();
		break;
		default:
		/* Module_Rectifier*/
				
				this->module_rectifier->display(image_rectifier);
		}

		
		/* Update information matrix */
		std::stringstream labelString;
		labelString << camera.k11;
		k[0]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k12;
		k[1]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k13;
		k[2]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k14;
		k[3]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k21;
		k[4]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k22;
		k[5]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k23;
		k[6]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k24;
		k[7]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k31;
		k[8]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k32;
		k[9]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k33;
		k[10]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k34;
		k[11]->set_label(labelString.str());


		labelString.str(""); labelString << camera.rt11;
		rt[0]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt12;
		rt[1]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt13;
		rt[2]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt14;
		rt[3]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt21;
		rt[4]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt22;
		rt[5]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt23;
		rt[6]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt24;
		rt[7]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt31;
		rt[8]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt32;
		rt[9]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt33;
		rt[10]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt34;
		rt[11]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt41;
		rt[12]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt42;
		rt[13]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt43;
		rt[14]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt44;
		rt[15]->set_label(labelString.str());

		/* Draw Camera */
		world->draw_camera(10, 20, 1);

		while (gtkmain.events_pending())
			gtkmain.iteration();


/* Module_Rectifier 

		colorspaces::ImageRGB8 img_rgb84(image);//conversion will happen if needed
		Glib::RefPtr<Gdk::Pixbuf> imgBuff4 = 
		Gdk::Pixbuf::create_from_data((const guint8*)img_rgb84.data,
			Gdk::COLORSPACE_RGB,
			false,
			8,
			img_rgb84.width,
			img_rgb84.height,
			img_rgb84.step);

		this->module_rectifier->gtkimage_notrectified->clear();
		this->module_rectifier->gtkimage_notrectified->set(imgBuff3);


		/* Image 
		gtkimage_notrectified->clear(); 
		gtkimage_notrectified->set(imgBuff4);
		mybuffer_notrectified = imgBuff4->get_pixels();
	
		/* Rectified Image 
		imgRectifiedBuff = imgBuff4->copy();
		gtkimage_rectified->clear(); 
		gtkimage_rectified->set(imgRectifiedBuff);
		mybuffer_rectified = imgRectifiedBuff->get_pixels();


		/* if the user have selected 4 points on each image the application rectify the second image 
		if ((counter_points_image == NUM_POINTS_RECTIFIER) and (counter_points_image_rectified == NUM_POINTS_RECTIFIER)){

			/* Calculate the solution matrix 
			if (flag_resolved == false){
				solve_equation_system();
				flag_resolved = true;
			}
	  
			/* Build the rectified image 
			build_rectified_image(mybuffer_notrectified, mybuffer_rectified);

		}

		/* Draw selected points 
		drawSelectedPoints(counter_points_image, points_image, mybuffer_notrectified);
		drawSelectedPoints(counter_points_image_rectified, points_image_rectified, mybuffer_rectified);

		/*Manage image
		this->module_extrinsics2->drawWorld(image4);

		/*Set image
		colorspaces::ImageRGB8 img_rgb84(image4);//conversion will happen if needed
		Glib::RefPtr<Gdk::Pixbuf> imgBuff4 = Gdk::Pixbuf::create_from_data((const guint8*)img_rgb84.data,
				    Gdk::COLORSPACE_RGB,
				    false,
				    8,
				    img_rgb84.width,
				    img_rgb84.height,
				    img_rgb84.step); 
		gtk_image22->clear();
		gtk_image22->set(imgBuff4);
*/
		/*Show window
		this->module_dlt->displayFrameRate(fpslabel);
		*/
/*

CvPoint pt1,pt2;
			if (1){
				colorspaces::ImageRGB8 img_rgb888(image);//conversion will happen if needed
				Glib::RefPtr<Gdk::Pixbuf> imgBuff =  Gdk::Pixbuf::create_from_data((const guint8*) img_rgb888.data,Gdk::COLORSPACE_RGB,false,8,img_rgb888.width,img_rgb888.height,img_rgb888.step);    
	    		gtk_image->clear();

				/*si queremos pintar las lineas
bool lines_rgb_active = true;
				if (lines_rgb_active){
					IplImage* src = cvCreateImage(cvSize(img_rgb888.width,img_rgb888.height), IPL_DEPTH_8U, 3);
					memcpy((unsigned char *) src->imageData, &(img_rgb888.data[0]),img_rgb888.width*img_rgb888.height * 3);
util->draw_room(src,0, world->lines, world->numlines);
					memmove(&(img_rgb888.data[0]),(unsigned char *) src->imageData,img_rgb888.width*img_rgb888.height * 3);
					
				}
gtk_image->set(imgBuff);
//	    		displayFrameRate();
	    		while (gtkmain.events_pending())
	      		gtkmain.iteration();
			}

			if (1){
	
//				colorspaces::ImageRGB8 img_rgb888(imageDEPTH);//conversion will happen if needed
colorspaces::ImageRGB8 img_rgb888(image);//conversion will happen if needed
				Glib::RefPtr<Gdk::Pixbuf> imgBuff =  Gdk::Pixbuf::create_from_data((const guint8*) img_rgb888.data,Gdk::COLORSPACE_RGB,false,8,img_rgb888.width,img_rgb888.height,img_rgb888.step);    
//	    		w_imageDEPTH->clear();
gtk_patron->clear();
bool lines_depth_active = true;
				if (lines_depth_active){
					IplImage* src = cvCreateImage(cvSize(img_rgb888.width,img_rgb888.height), IPL_DEPTH_8U, 3);
					memcpy((unsigned char *) src->imageData, &(img_rgb888.data[0]),img_rgb888.width*img_rgb888.height * 3);
					util->draw_room(src,1, world->lines, world->numlines);
					memmove(&(img_rgb888.data[0]),(unsigned char *) src->imageData,img_rgb888.width*img_rgb888.height * 3);
					
				}
//	    		w_imageDEPTH->set(imgBuff);
gtk_patron->set(imgBuff);
//	    		displayFrameRate();
*/
	    		while (gtkmain.events_pending())
	      		gtkmain.iteration();
/*
			}

bool reconstruct_depth_activate = true;
			if (reconstruct_depth_activate){
				add_depth_points(imageDEPTH, imageRGB);
				//reconstruct_depth_activate=false;
			}
*/



		//mainwindow->resize(1,1);
	}

int View::get_active_camera(){
	return active_camera;
}

/* Common Events */

	void View::on_toggled_calibrator(){
		active_panel = 0;
		std::cout << " Calibrator " << std::endl;
		dlt_panel->set_visible(true);
		extrinsics_panel->set_visible(false);
		rectifier_panel->set_visible(false);
		estereo_panel->set_visible(false);
	}
	void View::on_toggled_extrinsics(){
		active_panel = 1;
		std::cout << " Extrinsics " << std::endl;
		extrinsics_panel->set_visible(true);
		dlt_panel->set_visible(false);
		rectifier_panel->set_visible(false);
		estereo_panel->set_visible(false);
	}
	void View::on_toggled_rectifier(){
		active_panel = 2;
		std::cout << " rectifier " << std::endl;
		dlt_panel->set_visible(false);
		extrinsics_panel->set_visible(false);
		rectifier_panel->set_visible(true);
		estereo_panel->set_visible(false);
	}
	void View::on_toggled_estereo(){
		active_panel = 3;
		std::cout << " estereo " << std::endl;
		dlt_panel->set_visible(false);
		extrinsics_panel->set_visible(false);
		rectifier_panel->set_visible(false);
		estereo_panel->set_visible(true);
	}

	void View::on_changed_camera_set(){
		active_camera = camera_set->get_active_row_number();
		std::cout << "Camara activa " << active_camera << std::endl;
	}

	void View::button_Load_word_clicked()
	{

		int i=0;
		FILE *worldconfig;

		Gtk::FileChooserDialog dialog("Please choose a folder", Gtk::FILE_CHOOSER_ACTION_OPEN);
		dialog.set_transient_for(*mainwindow);

		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button("Select", Gtk::RESPONSE_OK);

		int result = dialog.run();

		//Handle the response:
		switch(result){
			case(Gtk::RESPONSE_OK):{

				this->module_extrinsics->button_Load_clicked(dialog.get_filename().data());
				world->readFile(dialog.get_filename());
				break;
			}

			case(Gtk::RESPONSE_CANCEL):{
				std::cout << "Cancel clicked." << std::endl;
				break;
			}

			default:{
				std::cout << "Unexpected button clicked." << std::endl;
				break;
			}
		}
	}

}//namespace
