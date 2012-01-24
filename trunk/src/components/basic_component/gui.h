/*
 *  Copyright (C) 1997-2011 JDERobot Developers Team
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
 *  Authors : Maikel González <m.gonzalezbai@gmail.com>
 *
 */

#ifndef MYCOMPONENT_GUI_H
#define MYCOMPONENT_GUI_H

#include <cv.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string>
#include <iostream>
#include <gtkmm.h>
#include <gtkglmm.h>
#include <gdkglmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <jderobot/camera.h>
#include <pthread.h>
#include <colorspaces/colorspacesmm.h>
#include "API.h"
#include "camera.h"
#include <libgnomecanvasmm.h> 
#include "canvasTeleoperateControl.h" 
#include "canvasTeleoperateCameras.h"
#include "canvasLaser.h"

namespace mycomponent {
    class CanvasWin; 
    class CanvasControl;
    class CanvasControlCameras;
    class CanvasLaser;
    class Api;
    class Gui {
	public:
	  
	    Gui(Api *api);	
	    virtual ~Gui();
	    
	    Api *api;
	    
	    
		//Public Methods
	    void isVisible();
	    void display(Api *api);
	    void ShowImages(Api *api);
	    void updateCanvasLaser();
            void updateCameras(Api *api);
void resetAPI(Api *api);
	    
		// Canvas
	    CanvasControl* canvas_control;
	    CanvasControlCameras* canvas_control_cameras;
	    CanvasLaser* canvas_laser;
	    colorspaces::Image* image1;	// Image camera1 processed to manipulate with openCV
	    colorspaces::Image* image2; // Image camera2 processed to manipulate with openCV

	private:
	    Gtk::Main gtkmain;
	    Glib::RefPtr<Gnome::Glade::Xml> refXml;
	    std::string gladepath; 

	    
		// Windows
	    Gtk::Window * canvaswindow; 
	    Gtk::Window * windowTeleoperate;
	    Gtk::Window * windowLaser;
	    Gtk::Window *secondarywindow;
	
		// Buttons	
	    bool showCanvasWin; 
	    Gtk::CheckButton * canvas_button; 
	    bool press_stop_button;
	    Gtk::Button *stop_button; 
	    Gtk::Button *yourCodeButton;
    	    bool yourCode_button_isPressed;
	    Gtk::Button *stopCodeButton;
            bool press_stopCodeButton;
	    Gtk::Button *exitButton;

		// Cameras
	    Gtk::Image *gtk_image1;
	    Gtk::Image *gtk_image2;
	    TPinHoleCamera myCamA, myCamB;	    

		// Private Methods
	    void initCameras();
	    void setCamara (const colorspaces::Image& image, int id);
	    void yourCodeButton_clicked();
	    void stopCodeButton_clicked();
	    void exitButton_clicked();
	    void teleOperate(Api *api);
	    void teleOperateCameras(Api *api);
	    void canvas_button_toggled(); 
	    void stop_button_clicked();

	    
    };//class
}//namespace
#endif //MYCOMPONENT_GUI_H