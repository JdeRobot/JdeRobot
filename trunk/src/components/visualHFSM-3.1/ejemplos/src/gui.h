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
#include "control_class.h"
#include "camera.h"
#include <libgnomecanvasmm.h> 
#include "canvasTeleoperateControl.h" 
#include "canvasTeleoperateCameras.h"

namespace mycomponent {
    class CanvasWin; 
    class CanvasControl;
    class CanvasControlCameras;
    class Control;
    class Gui {
      
	public:
	  
	    Gui(Control *control);	
	    virtual ~Gui();
	    
	    bool isVisible();
	    void display(const colorspaces::Image& image1, const colorspaces::Image& image2);
	    CanvasControl* canvas_control;
	    CanvasControlCameras* canvas_control_cameras;
 	    
	private:
	  //Private Attributes
	    
	    TPinHoleCamera myCamA, myCamB;	    
	    std::string gladepath; 
	    
	    // Canvas
	    bool showCanvasWin; 
	    Gtk::CheckButton * canvas_button; 
	    Gtk::Window * canvaswindow; 
	    Gtk::Window * windowTeleoperate; 
	    Gtk::Button *stop_button; 
	    bool press_stop_button;
		    
	    Glib::RefPtr<Gnome::Glade::Xml> refXml;
	    Gtk::Main gtkmain;

	    Gtk::Window *secondarywindow;
	    Gtk::Button *yourCodeButton;
	    Gtk::Button *stopCodeButton;
	    Gtk::Button *exitButton;
	    Gtk::Image *gtk_image1;
	    Gtk::Image *gtk_image2;
    	    Control* control;

	    bool yourCode_button_isPressed;

	  // Private Methods
	    void initCameras();
	    void setCamara (const colorspaces::Image& image, int id);
	    void yourCodeButton_clicked();
	    void stopCodeButton_clicked();
	    void exitButton_clicked();
	    void teleOperate();
	    void teleOperateCameras();
	    void canvas_button_toggled(); 
	    void stop_button_clicked();

	    
    };//class
}//namespace
#endif //MYCOMPONENT_GUI_H