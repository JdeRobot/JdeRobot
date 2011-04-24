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

#ifndef GIRAFFECLIENT_VIEW_H
#define GIRAFFECLIENT_VIEW_H

#include <string>
#include <iostream>
#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include "controller.h"
#include <colorspaces/colorspacesmm.h>
#include <progeo/progeo.h>

namespace calibrator {
  class View {
  public:

    View(Controller * controller);
    virtual ~View();

		/*Return true if the windows is visible*/
    bool isVisible();

    /*Display window*/
    void display(const colorspaces::Image& image);

  private:

		void button_center_clicked();
		void button_save_clicked();
		void button_KRT_clicked();
		void button_Load_clicked();
		void pos_x_changed();
		void pos_y_changed();
		void pos_z_changed();
		void foa_x_changed();
		void foa_y_changed();
		void foa_z_changed();
		void fx_changed();
		void fy_changed();
		void u0_changed();
		void v0_changed();
		void roll_changed();

        Glib::RefPtr<Gnome::Glade::Xml> refXml;
        Gtk::Main gtkmain;
        Gtk::Window* mainwindow;
        Gtk::VScale *vscale_pos_x;
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
        Gtk::Button* button_save;
        Gtk::Button* button_Load;

        Controller * controller;
        
        Gtk::Window* window;
        Gtk::Label labelK[13];
        Gtk::Label labelRT[13];
        Gtk::Table m_table;
  };
}//namespace

#endif /*GIRAFFECLIENT_VIEW_H*/
