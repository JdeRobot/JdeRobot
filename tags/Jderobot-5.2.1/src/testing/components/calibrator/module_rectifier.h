/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
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
 *  Authors : David Lobato Bravo <dav.lobato@gmail.com>
 *
 */

#ifndef CAMERAVIEW_VIEWER_H
#define CAMERAVIEW_VIEWER_H

#include <gtkmm.h>
#include <libglademm.h>
#include <Ice/Ice.h>
#include <string>
#include <colorspaces/colorspacesmm.h>




namespace calibrator{

class Module_Rectifier
{
	public:
		Module_Rectifier(Ice::PropertiesPtr prop);
		virtual ~Module_Rectifier();
			
		bool isVisible();
		void display( const colorspaces::Image& image );
		void get_widgets(Glib::RefPtr<Gnome::Glade::Xml> refXml);

		// data types
		struct Tpoint{
			int x;
			int y;
			int draw;
		} ;
		

// Glade objects
		Glib::RefPtr<Gnome::Glade::Xml> refXml;
		Gtk::Image* gtkimage_notrectified;
		Gtk::EventBox *eventbox_notrectified;
		Gtk::Image* gtkimage_rectified;
		Gtk::EventBox *eventbox_rectified;
		Gtk::Label* fpslabel;
		Gtk::Button* reset_button_rectifier;

	private:
		// FIXME GET THIS VALUE FROM THE BUFFER
		#define IMAGE_HEIGHT 240
		#define IMAGE_WIDTH 320
		#define NUM_POINTS 4
		#define NUM_EQU NUM_POINTS*2
		

		// Events
		void on_clicked_reset_button_rectifier();
		gboolean on_button_press_image_notrectified(GdkEventButton* event);
		gboolean on_button_press_image_rectified(GdkEventButton* event);

		// Functions
		void displayFrameRate();
		void solve_equation_system();
		void get_equation(Tpoint p, Tpoint p_prima, int **ecuacion);
		void linear_multiple_regression(double a_data[NUM_EQU*8], double b_data[NUM_EQU]);
		void calculate_allocation(int x,int y,Tpoint *p);
		void drawSelectedPoints(int counter, Tpoint* points, guint8* mybuffer_rectified);
		void build_rectified_image(guint8* mybuffer, guint8* mybuffer_rectified);

		//! time variables for calculating number of frames per second 
		IceUtil::Time currentFrameTime,oldFrameTime;
		double fps;
		int frameCount;

};

}//namespace

#endif //CAMERAVIEW_VIEWER_H
