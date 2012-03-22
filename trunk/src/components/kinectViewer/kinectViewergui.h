#ifndef kinectViewer_VIEW_H
#define kinectViewer_VIEW_H

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

namespace kinectViewer {
  class kinectViewergui {
		public:

		  kinectViewergui(jderobot::KinectPrx rgb,jderobot::KinectPrx depth,kinectViewerController::PTMotorsController* ptmc_in, kinectViewerController::LedsController* lc_in, std::string path, std::string path_rgb, std::string path_ir, int width, int height);
		  virtual ~kinectViewergui();

			/*Return true if the windows is visible*/
		  bool isVisible();


		  /*Display window*/
		  void update( const colorspaces::Image& imageRGB, const colorspaces::Image& imageDEPTH );
		bool isClosed();

		private:
			
	
			Glib::RefPtr<Gnome::Glade::Xml> refXml;
		  	Gtk::Main gtkmain;
		  	Gtk::Window *mainwindow;
			Gtk::Window *w_window_controller;
			Gtk::Image* w_imageRGB;
			Gtk::Image* w_imageDEPTH;
			Gtk::ToggleButton * w_change_ir_rgb;
			Gtk::ToggleButton * w_change_depth;
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
			kinectViewerGuiModules::ptmotorsGui* ptmGui;
			kinectViewerGuiModules::ledsGui* lGui;
			int cWidth, cHeight;

			void displayFrameRate();
			IceUtil::Time currentFrameTime,oldFrameTime;
    		double fps;
    		int frameCount;
			jderobot::KinectPrx cam_rgb;
			jderobot::KinectPrx cam_depth;
			jderobot::KinectLedsPrx leds;
			myprogeo *mypro;
			bool lines_depth_active;
			bool lines_rgb_active;

			void on_clicked_change_ir_rgb();
			void on_clicked_change_depth();
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

#endif /*kinectViewer_VIEW_H*/
