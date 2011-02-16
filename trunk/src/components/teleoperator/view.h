/*
 *  Copyright (C) 2010 Julio Vega
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

#ifndef TELEOPERATOR_VIEW_H
#define TELEOPERATOR_VIEW_H

#include <string>
#include <iostream>
#include <gtkmm.h>
//#include <gdkglconfig.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include "controller.h"
#include <colorspaces/colorspacesmm.h>
#include "drawarea.h"
#include <jderobot/containers.h> // contiene la definición de IntSeq


namespace teleoperator {
  class View {
		public:

		  View(Controller * controller);
		  virtual ~View();

			/*Return true if the windows is visible*/
		  bool isVisible();

		  /*Display window*/
		  void display(const colorspaces::Image& image1, const colorspaces::Image& image2);

		protected:
			float robotx;
			float roboty;
			float robottheta;

			int numSonars;
			std::vector <float> us;

			int numLasers;
			std::vector <float> distanceData;

		private:
			// GETS: lo que "cogemos" del controller
			void getEncoders ();
			void getSonars ();
			void getLaser ();

			// SETS: lo que "ponemos" a disposición del drawarea
			void setEncoders ();
			void setSonars ();
			void setLaser ();

			void stopButton_clicked();
			void upButton_clicked();
			void downButton_clicked();
			void leftButton_clicked();
			void rightButton_clicked();

			void stopButton1_clicked();
			void upButton1_clicked();
			void downButton1_clicked();
			void leftButton1_clicked();
			void rightButton1_clicked();

		  Glib::RefPtr<Gnome::Glade::Xml> refXml;
		  Gtk::Main gtkmain;
		  Gtk::Window* mainwindow;
			Gtk::Button *stopButton;
			Gtk::Button *upButton;
			Gtk::Button *downButton;
			Gtk::Button *leftButton;
			Gtk::Button *rightButton;
			Gtk::Button *stopButton1;
			Gtk::Button *upButton1;
			Gtk::Button *downButton1;
			Gtk::Button *leftButton1;
			Gtk::Button *rightButton1;
			Gtk::Image *gtk_image;
			Gtk::Image *gtk_image2;

			DrawArea* world;
			Controller* controller;
  };
} // namespace

#endif /*TELEOPERATOR_VIEW_H*/
