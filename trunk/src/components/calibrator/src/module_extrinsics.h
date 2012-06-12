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
 *            Alejandro Hernández Cordero <ahcorde@gmail.com>
 */

#ifndef GIRAFFECLIENT_CONTROLLER_H
#define GIRAFFECLIENT_CONTROLLER_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <opencv/cv.h>
#include <progeo/progeo.h>
#include <colorspaces/colorspacesmm.h>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <opencv/cv.h>
#include "drawarea.h"

using namespace std;

namespace calibrator {
	class Module_Extrinsics {
		public:
			Module_Extrinsics(Ice::PropertiesPtr prop);
			virtual ~Module_Extrinsics();
		
			void get_widgets(Glib::RefPtr<Gnome::Glade::Xml> refXml);
			void display(const colorspaces::Image& image);
			TPinHoleCamera getCam();
			void set_mainwindow(Gtk::Window* mainwindow);


			void button_Load_clicked(const char* filename);

		private:
			#define IMAGE_WIDTH 320
			#define IMAGE_HEIGHT 240
			#define CALIBRATION_PI 3.1415926

			Gtk::Image *gtk_image;
			std::string getGladePath();

			HPoint3D * getPos();
			HPoint3D * getFoa();
			float getFdistX();
			float getFdistY();
			float getU0();
			float getV0();
			float getRoll();
			
			void setPos(float x, float y, float z);
			void setFoa(float x, float y, float z);
			void setFdistX(float value);
			void setFdistY(float value);
			void setU0(float value);
			void setV0(float value);	
			void setRoll(float value);
			void changeDrawCenter();
			int saveParameters(const char* fileName);
			int load_world();
			int load_world_line(FILE *myfile);
			void resetLines();

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

			Gtk::Button* button_center;
			Gtk::Button* button_save;
			Gtk::Button* button_Load;

			Gtk::Window* mainwindow;
			Gtk::Window* window;
			Gtk::Label labelK[13];
			Gtk::Label labelRT[13];
			Gtk::Table m_table;


			void button_center_clicked();
			void button_save_clicked();
			void button_KRT_clicked();
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



			void init(Ice::PropertiesPtr prop);
			int load_camera_config(Ice::PropertyDict pd);
			void drawLine(IplImage * src, HPoint3D pini, HPoint3D pend);


			std::string gladepath;
			std::string world;
			std::string camOut;

			float lastx, lasty, lastz;
			bool drawCenter;
			vector<HPoint3D> lines;
			TPinHoleCamera camera;
  };

} /*namespace*/

#endif /*GIRAFFECLIENT_CONTROLLER_H*/
