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
 *  Authors : Julio Vega <julio.vega@urjc.es>,
 *
 */

#ifndef INTROROB_VIEW_H
#define INTROROB_VIEW_H

#include <string>
#include <iostream>
#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include "controller.h"
#include <colorspaces/colorspacesmm.h>
#include "drawarea.h"

namespace introrob {
  class View {
		public:

		  View(Controller * controller);
		  virtual ~View();

			/*Return true if the windows is visible*/
		  bool isVisible();

			void prepare2draw (IplImage &image);

		  /*Display window*/
		  void display(const colorspaces::Image& image);

		protected:
			float robotx;
			float roboty;
			float robottheta;

			int numSonars;
			std::vector <float> us;

			int numLasers;
			std::vector <float> distanceData;

		private:
			bool isFollowing;

			// GETS: lo que "cogemos" del controller
			void getEncoders ();
			void getLaser ();

			// SETS: lo que "ponemos" a disposición del drawarea
			void setEncoders ();
			void setLaser ();

			void stopButton_clicked();
			void upButton_clicked();
			void downButton_clicked();
			void leftButton_clicked();
			void rightButton_clicked();
			void camera1Button_clicked();
			void camera2Button_clicked();
			void camera3Button_clicked();
			void camera4Button_clicked();
			void pioneerCameraButton_clicked();

		  Glib::RefPtr<Gnome::Glade::Xml> refXml;
		  Gtk::Main gtkmain;
		  Gtk::Window *mainwindow;
			Gtk::Button *stopButton;
			Gtk::Button *upButton;
			Gtk::Button *downButton;
			Gtk::Button *leftButton;
			Gtk::Button *rightButton;
			Gtk::Button *camera1Button;
			Gtk::Button *camera2Button;
			Gtk::Button *camera3Button;
			Gtk::Button *camera4Button;
			Gtk::Button *pioneerCameraButton;
			Gtk::Image *gtk_image;

			DrawArea* world;
			Controller* controller;
  };
} // namespace

#endif /*INTROROB_VIEW_H*/
