/*
 *  Copyright (C) 1997-2013 JDE Developers TeamrgbdViz.camRGB
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
 *  Author : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>

 */

#ifndef rgbdViz_VIEW_H
#define rgbdViz_VIEW_H

#include <string>
#include <iostream>
#include <gtkmm.h>
#include <libglademm.h>
#include "drawarea.h"
#include <visionlib/colorspaces/colorspacesmm.h>
#include <jderobot/camera.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include "myprogeo.h"
#include <cv.h>
#include <highgui.h>
//#include "util3d.h"

namespace rgbdViz {
  class rgbdVizgui {
		public:

		  rgbdVizgui(bool rgb, bool depth,bool pointCloud , std::string path, std::string path_rgb, std::string path_ir, cv::Size sizeRGB, cv::Size sizeDEPTH, float cycle);
		  virtual ~rgbdVizgui();

			/*Return true if the windows is visible*/
		  bool isVisible();


		  /*Display window*/
		  void updateAll( cv::Mat imageRGB, cv::Mat imageDEPTH, std::vector<jderobot::RGBPoint> cloud );
		void updateRGB( cv::Mat imageRGB);
		void updateDEPTH(cv::Mat imageDEPTH );
		void updatePointCloud(std::vector<jderobot::RGBPoint> cloud);
		bool isClosed();
		float getCycle();

		private:
			float cycle;

	
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
			Gtk::ToggleButton *w_realDistance;
			Gtk::VBox * w_reconstruct_selection;
			Gtk::VBox * w_reconstruct_mode;
			Gtk::VBox * w_vbox_gl;
			Gtk::RadioButton *w_radio_depth;
			Gtk::RadioButton *w_radio_rgb;
			Gtk::RadioButton *w_radio_mode_pointcloud;
			Gtk::RadioButton *w_radio_mode_image;
			Gtk::Button *w_button_clear_lines;
			Gtk::VBox *w_images_tool;
			Gtk::Entry *w_entry;
			Gtk::Label *w_Distance;

			cv::Mat imageRGBlocal;
			cv::Mat imageDEPTHlocal;


			//util3d* util;
			DrawArea* world;
			cv::Size myDepthSize, myRGBSize;
			int reconstructMode;
			int modesAvalables;
			int cam_rgb_active;
			int cam_depth_active;

			void displayFrameRate();
			IceUtil::Time currentFrameTime,oldFrameTime;
    		double fps;
    		int frameCount;

			myprogeo *mypro;
			bool lines_depth_active;
			bool lines_rgb_active;

			bool on_clicked_event_rgb(GdkEventButton* event);
			bool on_clicked_event_depth(GdkEventButton* event);
			bool reconstruct_depth_activate;
			void on_reconstruct_depth();
			void add_depth_pointsImage(cv::Mat imageRGB, cv::Mat distance);
			void add_depth_pointsCloud(std::vector<jderobot::RGBPoint> cloud);
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
			

			cv::Mat* distance;
			//distance mutex
			IceUtil::Mutex m_distance;




  };
} // namespace

#endif /*rgbdViz_VIEW_H*/
