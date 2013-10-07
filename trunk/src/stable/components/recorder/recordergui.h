/*
*  Copyright (C) 1997-2013 JDERobot Developers Team
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
 *   Authors : Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>,
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#ifndef RECORDER_GUI_H
#define RECORDER_GUI_H

#include <string>
#include <iostream>
#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <visionlib/colorspaces/colorspacesmm.h>

namespace recorder {
  class recordergui {
  public:

    recordergui();
    virtual ~recordergui();
	void update();
	void set_iteration(int value);
	void set_fps(int value);
	bool get_recording();
	bool get_active();



  private:
		void button_record_clicked();
		void button_stop_clicked();
		void button_exit_clicked();

		bool recording;
		bool exit;




		Glib::RefPtr<Gnome::Glade::Xml> refXml;
		Gtk::Main gtkmain;
		Gtk::Window* mainwindow;

		Gtk::Entry *w_entry_iteration;
		Gtk::Entry *w_entry_fps;
		Gtk::Button *w_record;
		Gtk::Button *w_stop;
		Gtk::Button *w_exit;
 		/*Gtk::VScale *vscale_pos_x;
 		Gtk::VScale *vscale_pos_y;
 		Gtk::VScale *vscale_pos_z;
 		Gtk::VScale *vscale_foa_x;
 		Gtk::VScale *vscale_foa_y;
 		Gtk::VScale *vscale_foa_z;
 		Gtk::VScale *vscale_fx;
 		Gtk::VScale *vscale_fy;
 		Gtk::VScale *vscale_u0;
 		Gtk::VScale *vscale_v0;
 		Gtk::VScale *vscale_roll;
		Gtk::Image *gtk_image;
		Gtk::Button* button_center;
   
		Controller * controller;*/
  };
}//namespace

#endif /*RECORDER_GUI_H*/
