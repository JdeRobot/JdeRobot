/*
 *  Copyright (C) 2010 Eduardo Perdices García
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
 *   Authors : Eduardo Perdices García <eperdices@gsyc.es>,
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#ifndef GIRAFFECLIENT_VIEW_H
#define GIRAFFECLIENT_VIEW_H

#include <string>
#include <iostream>
#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include "controller.h"
#include <visionlib/colorspaces/colorspacesmm.h>

namespace giraffeClient {
  class View {
  public:

    View(Controller * controller);
    virtual ~View();

		/*Return true if the windows is visible*/
    bool isVisible();

    /*Display window*/
    void display(const colorspaces::Image& image);

		/*Set initial values*/
		void setInitialValues(float pan, float tilt, float left, float right);

		/*Set real values*/
		void setRealValues(float pan, float tilt, float left, float right);

  private:

		void button_pan_c_clicked();
		void button_tilt_c_clicked();
		void button_left_c_clicked();
		void button_right_c_clicked();
		void button_lookat_clicked();
		void pos_pan_changed();
		void pos_tilt_changed();
		void pos_left_changed();
		void pos_right_changed();


    Glib::RefPtr<Gnome::Glade::Xml> refXml;
    Gtk::Main gtkmain;
    Gtk::Window* mainwindow;
		Gtk::Button *button_pan_c;
		Gtk::Button *button_tilt_c;
		Gtk::Button *button_left_c;
		Gtk::Button *button_right_c;
		Gtk::Button *button_lookat;
 		Gtk::VScale *vscale_pos_pan;
 		Gtk::VScale *vscale_pos_tilt;
 		Gtk::VScale *vscale_pos_left;
 		Gtk::VScale *vscale_pos_right;
 		Gtk::HScale *hscale_look_x;
 		Gtk::HScale *hscale_look_y;
 		Gtk::HScale *hscale_look_z;
		Gtk::Label *label_val_pan;
		Gtk::Label *label_val_tilt;
		Gtk::Label *label_val_left;
		Gtk::Label *label_val_right;
		Gtk::Image *gtk_image;
   
		Controller * controller;
  };
}//namespace

#endif /*GIRAFFECLIENT_VIEW_H*/
