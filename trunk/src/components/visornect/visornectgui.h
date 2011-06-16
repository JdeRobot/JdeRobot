#ifndef VISORNECT_VIEW_H
#define VISORNECT_VIEW_H

#include <string>
#include <iostream>
#include <gtkmm.h>
#include <libglademm.h>
#include "drawarea.h"
#include <colorspaces/colorspacesmm.h>
#include <jderobot/kinect.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include "myprogeo.h"
#include <cv.h>
#include <highgui.h>
#include "util3d.h"
#include "controllers/ptmotors-controller.h"
#include "gui-modules/ptmotorsGui.h"
#include "controllers/leds-controller.h"
#include "gui-modules/ledsGui.h"

namespace visornect {
  class visornectgui {
		public:

		  visornectgui(visornectController::PTMotorsController* ptmc_in, visornectController::LedsController* lc_in, std::string path, std::string path_rgb, std::string path_ir);
		  virtual ~visornectgui();

			/*Return true if the windows is visible*/
		  bool isVisible();


		  /*Display window*/
		  void update( const colorspaces::Image& imageRGB, const colorspaces::Image& imageDEPTH );
		bool isClosed();
		void setCameraPrx(jderobot::KinectPrx c);

		private:
			
	
			Glib::RefPtr<Gnome::Glade::Xml> refXml;
		  	Gtk::Main gtkmain;
		  	Gtk::Window *mainwindow;
			Gtk::Window *w_window_controller;
			Gtk::Image* w_imageRGB;
			Gtk::Image* w_imageDEPTH;
			Gtk::Button * w_change_ir_rgb;
			Gtk::EventBox *w_event_rgb;
			Gtk::EventBox *w_event_depth;
			Gtk::ToggleButton *w_reconstruct;
			Gtk::ToggleButton *w_camera_pos;
			Gtk::ToggleButton *w_lines_rgb;
			Gtk::ToggleButton *w_lines_depth;
			Gtk::ImageMenuItem *w_view_controller;
			Gtk::VBox * w_reconstruct_selection;
			Gtk::RadioButton *w_radio_depth;
			Gtk::RadioButton *w_radio_rgb;
			Gtk::Button *w_button_clear_lines;
			util3d* util;
			DrawArea* world;
			visornectGuiModules::ptmotorsGui* ptmGui;
			visornectGuiModules::ledsGui* lGui;

			void displayFrameRate();
			IceUtil::Time currentFrameTime,oldFrameTime;
    		double fps;
    		int frameCount;
			jderobot::KinectPrx cam;
			jderobot::KinectLedsPrx leds;
			myprogeo *mypro;
			bool lines_depth_active;
			bool lines_rgb_active;

			void on_clicked_change_ir_rgb();
			bool on_clicked_event_rgb(GdkEventButton* event);
			bool on_clicked_event_depth(GdkEventButton* event);
			bool reconstruct_depth_activate;
			void on_reconstruct_depth();
			void add_depth_points(const colorspaces::Image& imageDEPTH, const colorspaces::Image& imageRGB);
			void add_cameras_position();
			void add_camera_position();
			void on_w_lines_rgb_toggled();
			void on_w_lines_depth_toggled();
			void on_w_view_controller_activate();
			void on_w_radio_depth_activate();
			void on_w_radio_rgb_activate();
			void on_clicked_clear_lines();
			



  };
} // namespace

#endif /*VISORNECT_VIEW_H*/
