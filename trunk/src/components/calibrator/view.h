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

#ifndef CALIBRATOR_VIEW_H
#define CALIBRATOR_VIEW_H

#include "module_dlt.h"
#include "module_extrinsics.h"
#include "module_rectifier.h"

#include <string>
#include <iostream>
#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <colorspaces/colorspacesmm.h>
#include <progeo/progeo.h>

/** OpenGL */
#include <GL/gl.h>              
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glut.h>

namespace calibrator {

class ModelColumns : public Gtk::TreeModel::ColumnRecord

{
	public:
		ModelColumns()
		{ add(m_col_id); add(m_col_name); }

		Gtk::TreeModelColumn<Glib::ustring> m_col_id; //The data to choose - this must be text.
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;

	};


	class View {
		public:
		//    View(Controller * controller);
		View(Glib::RefPtr<Gtk::ListStore> m_refTreeModel, Module_DLT * controllerCamera1, Module_Extrinsics * controller2, Module_Rectifier * module_rectifier);
		virtual ~View();

		/*Return true if the windows is visible*/
		bool isVisible();

		/*Display window*/
		void display(const colorspaces::Image& image);

		int get_active_camera();

		/** Calibrator **/
		int capture_on;

	private:
		Glib::RefPtr<Gnome::Glade::Xml> refXml;
		Gtk::Main gtkmain;
		Gtk::Window* mainwindow;
		Gtk::Image *gtk_patron;

		Gtk::RadioButton* mode[4];
		Gtk::Table* dlt_panel;
		Gtk::Table* extrinsics_panel;
		Gtk::Table* rectifier_panel;
		Gtk::Table* estereo_panel;
		Gtk::ComboBox* camera_set;
		Gtk::Button* button_Load_world;

		int active_camera;

		int active_panel;
		TPinHoleCamera camera;
		Gtk::Label* k[12];
		Gtk::Label* rt[16];

		DrawArea* world;

		void on_toggled_calibrator();
		void on_toggled_extrinsics();
		void on_toggled_rectifier();
		void on_toggled_estereo();
		void on_changed_camera_set();
		void button_Load_word_clicked();

		Module_DLT * module_dlt;
		Module_Extrinsics * module_extrinsics;
		Module_Rectifier * module_rectifier;

	};
};

#endif /*CALIBRATOR_H*/
