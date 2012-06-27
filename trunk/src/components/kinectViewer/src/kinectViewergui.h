#ifndef kinectViewer_VIEW_H
#define kinectViewer_VIEW_H

#include <string>
#include <iostream>
#include <gtkmm.h>
#include <libglademm.h>
#include "drawarea.h"
#include <colorspaces/colorspacesmm.h>
#include <jderobot/camera.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include "myprogeo.h"
#include <cv.h>
#include <highgui.h>
#include "util3d.h"
#include "controllers/Pose3DMotors-controller.h"
#include "gui-modules/Pose3DMotorsGui.h"
#include "controllers/leds-controller.h"
#include "gui-modules/ledsGui.h"
#include "controllers/pointCloud-controller.h"

namespace kinectViewer {
  class kinectViewergui {
		public:

		  kinectViewergui(jderobot::CameraPrx rgb,jderobot::CameraPrx depth,kinectViewerController::PointCloudController* pointCloud_ctr ,kinectViewerController::Pose3DMotorsController* ptmc_in, kinectViewerController::LedsController* lc_in, std::string path, std::string path_rgb, std::string path_ir, int width, int height);
		  virtual ~kinectViewergui();

			/*Return true if the windows is visible*/
		  bool isVisible();


		  /*Display window*/
		  void updateAll( const colorspaces::Image& imageRGB, const colorspaces::Image& imageDEPTH );
		void updateRGB( const colorspaces::Image& imageRGB);
		void updateDEPTH(const colorspaces::Image& imageDEPTH );
		void updatePointCloud();
		bool isClosed();

		private:
			
			kinectViewerController::PointCloudController* pointCloud;

	
			Glib::RefPtr<Gnome::Glade::Xml> refXml;
		  	Gtk::Main gtkmain;
		  	Gtk::Window *mainwindow;
			Gtk::Window *w_window_controller;
			Gtk::Window *w_window_gl;
			Gtk::Image* w_imageRGB;
			Gtk::Image* w_imageDEPTH;
			Gtk::EventBox *w_event_rgb;
			Gtk::EventBox *w_event_depth;
			Gtk::ToggleButton *w_reconstruct;
			Gtk::ToggleButton *w_camera_pos;
			Gtk::ToggleButton *w_tg_gl;
			Gtk::ToggleButton *w_lines_rgb;
			Gtk::ToggleButton *w_lines_depth;
			Gtk::ToggleButton *w_toggle_rgb;
			Gtk::ToggleButton *w_toggle_depth;
			Gtk::ImageMenuItem *w_view_controller;
			Gtk::VBox * w_reconstruct_selection;
			Gtk::VBox * w_reconstruct_mode;
			Gtk::VBox * w_vbox_gl;
			Gtk::RadioButton *w_radio_depth;
			Gtk::RadioButton *w_radio_rgb;
			Gtk::RadioButton *w_radio_mode_pointcloud;
			Gtk::RadioButton *w_radio_mode_image;
			Gtk::Button *w_button_clear_lines;
			util3d* util;
			DrawArea* world;
			kinectViewerGuiModules::Pose3DMotorsGui* ptmGui;
			kinectViewerGuiModules::ledsGui* lGui;
			int cWidth, cHeight;
			int reconstructMode;
			int modesAvalables;
			int cam_rgb_active;
			int cam_depth_active;

			void displayFrameRate();
			IceUtil::Time currentFrameTime,oldFrameTime;
    		double fps;
    		int frameCount;
			jderobot::CameraPrx cam_rgb;
			jderobot::CameraPrx cam_depth;
			jderobot::KinectLedsPrx leds;
			myprogeo *mypro;
			bool lines_depth_active;
			bool lines_rgb_active;

			bool on_clicked_event_rgb(GdkEventButton* event);
			bool on_clicked_event_depth(GdkEventButton* event);
			bool reconstruct_depth_activate;
			void on_reconstruct_depth();
			void add_depth_pointsImage(const colorspaces::Image& imageDEPTH, const colorspaces::Image& imageRGB);
			void add_depth_pointsCloud();
			void add_cameras_position();
			void add_camera_position();
			void on_w_lines_rgb_toggled();
			void on_w_lines_depth_toggled();
			void on_w_view_controller_activate();
			void on_w_radio_depth_activate();
			void on_w_radio_rgb_activate();
			void on_w_radio_mode_pointcloud_activate();
			void on_w_radio_mode_image_activate();
			void on_clicked_clear_lines();
			void on_w_tg_gl_toggled();
			



  };
} // namespace

#endif /*kinectViewer_VIEW_H*/
