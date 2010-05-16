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
 *  Authors : Javier Vázuqez Pereda <javiervazper@yahoo.es>
 *
 */

#ifndef INTROROBGUI_H
#define INTROROBGUI_H

#include <gtkmm.h>
//#include <gnome.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <string>
#include <colorspaces/colorspacesmm.h>
#include <colorspaces/colorspacesmm.h>


namespace introrob{

  class introrobgui
  {
  public:
    introrobgui();
    ~introrobgui();
  
    bool isVisible();
    bool isClosed();

    //! function that actually displays the image in a window and process events
    void updatecamera( const colorspaces::Image& image );

	bool isYourcode();
	bool isStopped();
  	float getV();
	float getW();

  private:
    Glib::RefPtr<Gnome::Glade::Xml> refXml;
    Gtk::Image* gtkimage;
    Gtk::Window* mainwindow;
    Gtk::Label* fpslabel;
    Gtk::ToggleButton* stopbutton;
    Gtk::RadioButton* yourcode;
    Gtk::RadioButton* manual;
    Gtk::Button* resetw;
    Gtk::Button* resetv;
    Gtk::HScale* hscalev;
    Gtk::HScale* hscalew;
//    Gtk::HScale* hscalemap;
    Gtk::HBox* hbox2;
    Gtk::Adjustment* adj_hscalev;

//	Gnome::Canvas * canvasmap;

	bool yourcode_cond ;
	bool stop_cond ;
	bool close;

	float MAX_V, MAX_W;
	


    Gtk::Main gtkmain;
  
    //! display the frame rate of the received images
    void displayFrameRate();


	void on_clicked_resetv();
	void on_clicked_resetw();
	void on_clicked_yourcode();
	void on_clicked_manual();
	void on_toggled_stopbutton();
	bool on_change_value_hscalev(Gtk::ScrollType scroll, double new_value);
	bool on_change_value_hscalew(Gtk::ScrollType scroll, double new_value);
	bool on_delete_event_mainwindow(GdkEventAny* event);
	


      
    //! time variables for calculating number of frames per second 
    IceUtil::Time currentFrameTime,oldFrameTime;
    double fps;
    int frameCount;

	float v, w;


  };

}//namespace

#endif //INTROROBGUI_H
