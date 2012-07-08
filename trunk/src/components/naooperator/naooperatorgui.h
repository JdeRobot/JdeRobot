/*
 *  Copyright (C) 1997-2009 JDE Developers Team
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

#ifndef NAOOPERATOR_GUI_H
#define NAOOPERATOR_GUI_H

#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <string>
#include <colorspaces/colorspacesmm.h>
#include "controllers/motors-controller.h"
#include "controllers/ptmotors-controller.h"
#include "controllers/ptencoders-controller.h"
#include "controllers/bodyencoders-controller.h"
#include "controllers/bodymotors-controller.h"
#include "gui-modules/motorsGui.h"
#include "gui-modules/ptmotorsGui.h"
#include "gui-modules/ptencodersGui.h"
#include "gui-modules/bodyencodersGui.h"
#include "gui-modules/bodymotorsGui.h"



namespace naooperator{
/**
* \brief Class that contains all the gui functions conected to the glade interface
*/
  class naooperatorgui
  {
  public:
    naooperatorgui(DevicesController::MotorsController *m,DevicesController::PTMotorsController *ptm, DevicesController::PTEncodersController *pte, DevicesController::BodyEncodersController *be,DevicesController::BodyMotorsController *bm);
    ~naooperatorgui();
  
    bool isVisible();
    bool isClosed();
    void update( const colorspaces::Image& image );



  private:
    Glib::RefPtr<Gnome::Glade::Xml> refXml;
    Gtk::Image* gtkimage;
    Gtk::Window* mainwindow;
	Gtk::Button * w_button_exit;
	bool close;
	Gtk::CheckMenuItem* w_camera_run;
	bool camera_run;
    Gtk::Main gtkmain;
	guiModules::motorsGui *motorsGui;
	guiModules::ptmotorsGui *ptmotorsGui;
	guiModules::ptencodersGui *ptencodersGui;
	guiModules::bodyencodersGui *bodyencodersGui;
	guiModules::bodymotorsGui *bodymotorsGui;

	//!canvas teleperator shared with ptmotors and ptencoders 
	//! head Canvas
	Gtk::DrawingArea *w_canvas_head;
    
    void displayFrameRate();
	bool on_delete_event_mainwindow(GdkEventAny* event);
	void on_clicked_exit();
	void on_toggled_camera_run();
      
    //! time variables for calculating number of frames per second 
    IceUtil::Time currentFrameTime,oldFrameTime;
    double fps;
    int frameCount;
  };

}//namespace

#endif 
