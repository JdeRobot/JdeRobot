/*
 *  Copyright (C) 1997-2010 JDE Developers Team
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
			Francisco Miguel Rivas Montero <fm.rivas@alumnos.urjc.es>
			
 */

/** \file naooperatorgui.cpp
 * \brief NaoOperator gui-glade definitions
 */

#include "naooperatorgui.h" 
#include <iostream>
#include <cmath>

namespace naooperator
{
  const std::string gladepath = std::string(GLADE_DIR) + std::string("/naooperatorgui.glade");

/**
* \brief NaoOperatorGui class construtor
* \param m a reference to a DevicesController::MotorsController wich is the controller that the gui uses to access to the device (NULL if not activated)
*/
  naooperatorgui::naooperatorgui(DevicesController::MotorsController *m, DevicesController::PTMotorsController *ptm, DevicesController::PTEncodersController *pte, DevicesController::BodyEncodersController *be,DevicesController::BodyMotorsController *bm) 
    : gtkmain(0,0),frameCount(0) 
{
	std::vector<float> parametres;

	motorsGui=NULL;
	ptmotorsGui=NULL;
	ptencodersGui=NULL;
    std::cout << "Loading glade\n";
    refXml = Gnome::Glade::Xml::create(gladepath);
	refXml->get_widget("NaoOperator",mainwindow);
	this->mainwindow->show();
	if (m!=NULL){
		this->motorsGui = new guiModules::motorsGui(m,refXml);
	}
	if ((ptm!=NULL) || (pte != NULL)){
		Gtk::VBox *vbox;
		refXml->get_widget("vbox_pte_canvas",vbox);
		vbox->show();
		refXml->get_widget("canvas_teleoperator",w_canvas_head);
		w_canvas_head->signal_unrealize();
		w_canvas_head->signal_realize();
		w_canvas_head->set_child_visible(TRUE);
		w_canvas_head->add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::VISIBILITY_NOTIFY_MASK);	
		if (ptm!=NULL){
			/*
			0. maxLongitude;
			1. minLongitude;
			2. maxLatitude;
			3. minLatitude;
			4. maxLongitudeSpeed;
			5. maxLatitudeSpeed;*/
			parametres=ptm->getPTMotorsParams();
			
		}
		else{
			parametres.push_back(100);
			parametres.push_back(0);
			parametres.push_back(100);
			parametres.push_back(0);
			parametres.push_back(100);
			parametres.push_back(100);
		}
	}
	if (ptm != NULL){
		this->ptmotorsGui = new guiModules::ptmotorsGui(ptm,refXml,w_canvas_head,parametres);
	}
	if (pte != NULL){
		this->ptencodersGui = new guiModules::ptencodersGui(pte,refXml,w_canvas_head,parametres);
	}	
	if (be != NULL){
		this->bodyencodersGui = new guiModules::bodyencodersGui(be,refXml);	
	}
	if (be != NULL){
		this->bodymotorsGui = new guiModules::bodymotorsGui(bm,refXml);	
	}
    refXml->get_widget("image7", gtkimage);
	refXml->get_widget("camera1",w_camera_run);
	refXml->get_widget("button_exit",w_button_exit);
	this->close=false;
	this->camera_run=false;
    // start the timer for calculating the number of frames per second
    // the images are being displayed at
    oldFrameTime = IceUtil::Time::now();

	mainwindow->signal_delete_event().connect(sigc::mem_fun(this,&naooperatorgui::on_delete_event_mainwindow));	
	w_button_exit->signal_clicked().connect(sigc::mem_fun(this,&naooperatorgui::on_clicked_exit));	
	w_camera_run->signal_toggled().connect(sigc::mem_fun(this,&naooperatorgui::on_toggled_camera_run));	
	
	while (gtkmain.events_pending())
    	gtkmain.iteration();
}
    
/**
* \brief NaoOperatorGui class destructor
* \return Nothing
*/
naooperatorgui::~naooperatorgui() 
{	
}

/**
* \brief Function that return the status of the MainWindow
* \return The value of the internal variable \e close
*/
bool 
naooperatorgui::isClosed()
{
	return close;
}

/**
* \brief Functions that returns is the MainWindow is visible
* \return bool value: true if the MainWindows is visible, false otherwise.
*/
bool 
naooperatorgui::isVisible()
{
	mainwindow->is_visible();
}

/**
* \brief Function that actually displays the image in a window and process events
* \return void
*/
void 
naooperatorgui::update( const colorspaces::Image& image )
{
		if (camera_run){
			colorspaces::ImageRGB8 img_rgb888(image);//conversion will happen if needed
			Glib::RefPtr<Gdk::Pixbuf> imgBuff =  Gdk::Pixbuf::create_from_data((const guint8*)img_rgb888.data,Gdk::COLORSPACE_RGB,false,8,img_rgb888.width,img_rgb888.height,img_rgb888.step);
    
    		gtkimage->clear();
    		gtkimage->set(imgBuff);
    		displayFrameRate();
    		mainwindow->resize(1,1);
    		while (gtkmain.events_pending())
      		gtkmain.iteration();
		}
		if (this->ptencodersGui  !=NULL){
			this->ptencodersGui->update();
		}
		if (this->bodyencodersGui !=NULL){
			this->bodyencodersGui->update();
		}
		mainwindow->resize(1,1);
    	while (gtkmain.events_pending())
      	gtkmain.iteration();
  	}

/**
* \brief Display the frame rate of the received images
* \return void
*/
void naooperatorgui::displayFrameRate()
{
	double diff;
	IceUtil::Time diffT;

	currentFrameTime = IceUtil::Time::now();
	diff = (currentFrameTime - oldFrameTime).toMilliSecondsDouble();
	if (diff < 1000.0)
		frameCount++;
	else{
		oldFrameTime = currentFrameTime;
		fps = frameCount*1000.0/diff;
		frameCount=0;
		// Display the frame rate
		std::stringstream fpsString;
		fpsString << "fps = " << int(fps);
		//fpslabel->set_label(fpsString.str());
		}
	}

/**
* \brief Function that describes what to do if the MainWindow is closed
* \return bool: true if the MaiWindows have been closed
*/
bool 
naooperatorgui::on_delete_event_mainwindow(GdkEventAny* event)
{
	close=true;
	return close;
}

/**
* \brief Function that describes what to do if the exit button is clicked
* \return bool: true if the MaiWindows have been closed
*/
void
naooperatorgui::on_clicked_exit(){
	close=true;
}

/**
* \brief Function that describes what to do if camera_run widget recieves a toggle event
* \return void
*/
void 
naooperatorgui::on_toggled_camera_run()
{
	if (this->camera_run)
		this->camera_run=false;
	else
		this->camera_run=true;
}

}//namespace
