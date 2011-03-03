

#ifndef INTROROB_VIEW_H
#define INTROROB_VIEW_H

#include <string>
#include <iostream>
#include <gtkmm.h>
#include <libglademm.h>
#include "drawarea.h"
#include <colorspaces/colorspacesmm.h>
#include <jderobot/kinect.h>
#include <jderobot/kinectleds.h>
#include <jderobot/ptmotors.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>

namespace visornect {
  class visornectgui {
		public:

		  visornectgui(jderobot::PTMotorsPrx m, jderobot::KinectLedsPrx l, std::string path);
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
			Gtk::Image* w_imageRGB;
			Gtk::Image* w_imageDEPTH;
			Gtk::Button * w_up;
			Gtk::Button * w_down;
			Gtk::Button * w_change_ir_rgb;
			Gtk::Button *w_led_off;
			Gtk::Button *w_led_green;
			Gtk::Button *w_led_red;
			Gtk::Button *w_led_yellow;
			Gtk::Button *w_led_bgreen;
			Gtk::Button *w_led_bred;

			DrawArea* world;

			void displayFrameRate();
			IceUtil::Time currentFrameTime,oldFrameTime;
    		double fps;
    		int frameCount;
			jderobot::PTMotorsPrx mprx;
			jderobot::KinectPrx cam;
			jderobot::KinectLedsPrx leds;
			void on_clicked_up();
			void on_clicked_down();
			void on_clicked_change_ir_rgb();
			void on_clicked_led_off();
			void on_clicked_led_green();
			void on_clicked_led_red();
			void on_clicked_led_yellow();
			void on_clicked_led_bgreen();
			void on_clicked_led_bred();
			
  };
} // namespace

#endif /*INTROROB_VIEW_H*/
