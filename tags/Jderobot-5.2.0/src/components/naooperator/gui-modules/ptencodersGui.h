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

#ifndef NAOOPERATOR_PTENCODERS_GUI_H
#define NAOOPERATOR_PTENCODERS_GUI_H

#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <string>
#include <colorspaces/colorspacesmm.h>
#include "../controllers/ptencoders-controller.h"


namespace guiModules{
/**
* \brief Class that contains all the gui functions conected to the glade interface
*/
  class ptencodersGui
  {
  public:
    ptencodersGui(DevicesController::PTEncodersController *m, Glib::RefPtr<Gnome::Glade::Xml> xml, Gtk::DrawingArea *w,std::vector<float> parametres);
    ~ptencodersGui();
	void update();


  private:
    Glib::RefPtr<Gnome::Glade::Xml> refXml;
	DevicesController::PTEncodersController* ptencoders_ctr;
	Gtk::DrawingArea *w_canvas_head;
	Glib::RefPtr<Gdk::GC> gc_head;
	//! Drawing colormap
	Glib::RefPtr<Gdk::Colormap> colormap;
	Gdk::Color color_white;
	Gdk::Color color_black;
	int pt_joystick_x_head, pt_joystick_y_head;
	float tilt, pan;
	float MAXPAN, MAXTILT, MINPAN, MINTILT;

	gboolean on_expose_event_canvas_head(GdkEventExpose *event);
	void canvas_head_controller();
  };

}//namespace

#endif 
