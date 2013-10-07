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
#include "drawarea.h"
#include "common.h"
//eclipse
#include "glibmm/ustring.h"
#include <sigc++-2.0/sigc++/functors/mem_fun.h>

namespace rgbdManualCalibrator {
  class View {
  public:

    View(Controller* controller, std::string path, int nCameras);
    virtual ~View();

		/*Return true if the windows is visible*/
    bool isVisible();

    /*Display window*/
  void display(std::vector<rgbdManualCalibrator::kinectData> sources);
		int getActiveCam();

  private:

	class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { add(m_col_id); add(m_col_name); }

    Gtk::TreeModelColumn<int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
  };
		

		void button_center_clicked();
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
		void combo_changed();
		void button_save_clicked();
		void button_colour_clicked();
		int nCameras;
		int cam;
		ModelColumns m_Columns;


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
		Gtk::ToggleButton *w_depth;
		Gtk::ToggleButton *w_fix;
		rgbdManualCalibrator::DrawArea* world;
		Gtk::Window* w_window_gl;
		Gtk::ComboBox *m_Combo;
		Gtk::Button* w_save;
		Gtk::ToggleButton* w_colour;
		bool trueColor;
   
		Controller * controller;
  };
}//namespace

#endif /*GIRAFFECLIENT_VIEW_H*/
