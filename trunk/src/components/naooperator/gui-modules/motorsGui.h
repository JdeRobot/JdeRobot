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

#ifndef NAOOPERATOR_MOTORS_GUI_H
#define NAOOPERATOR_MOTORS_GUI_H

#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <string>
#include <colorspaces/colorspacesmm.h>
#include "../controllers/motors-controller.h"


namespace guiModules{
/**
* \brief Class that contains all the gui functions conected to the glade interface
*/
  class motorsGui
  {
  public:
    motorsGui(DevicesController::MotorsController *m, Glib::RefPtr<Gnome::Glade::Xml> xml);
    ~motorsGui();


  private:
    Glib::RefPtr<Gnome::Glade::Xml> refXml;
	float v,w,side;
	Gtk::Button *w_up_v;
	Gtk::Button *w_down_v;
	Gtk::Button *w_up_w;
	Gtk::Button *w_down_w;
	Gtk::Button *w_right_side;
	Gtk::Button *w_left_side;
	Gtk::Button *w_stop;
	Gtk::VScale *w_vscale_v;
	Gtk::HScale *w_hscale_w;
	//! Motors Controller
	DevicesController::MotorsController* motors_ctr;
	//! Walk Canvas
	Gtk::DrawingArea *w_canvas_walk;
	int pt_joystick_x_walk, pt_joystick_y_walk;
	//! Drawing attributes for the walk canvas
	Glib::RefPtr<Gdk::GC> gc_walk;
	//! Drawing colormap
	Glib::RefPtr<Gdk::Colormap> colormap;
	Gdk::Color color_white;
	Gdk::Color color_black;
	//! Constant variables to determine the ranges of W and V
	int MINV;
	//! Constant variables to determine the ranges of W and V
	int MINW;
	//! Constant variables to determine the ranges of W and V
	int MAXV;
	//! Constant variables to determine the ranges of W and V
	int MAXW;

    
	void on_clicked_up_v();
	void on_clicked_down_v();
	void on_clicked_up_w();
	void on_clicked_down_w();
	void on_clicked_right_side();
	void on_clicked_left_side();
	void on_clicked_stop();	
	gboolean on_expose_event_canvas_walk(GdkEventExpose *event);
	void canvas_body_controller(int type);
	gboolean on_button_press_canvas_walk(GdkEventButton *event);
	bool on_change_value_vscale_v(Gtk::ScrollType scroll, double value);
	bool on_change_value_hscale_w(Gtk::ScrollType scroll, double value);
	


      
  };

}//namespace

#endif 
