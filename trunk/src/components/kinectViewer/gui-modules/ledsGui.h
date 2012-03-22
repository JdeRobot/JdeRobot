/*
 *  Copyright (C) 1997-20011 JDE Developers Team
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
 *  Authors : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <fm.rivas@alumnos.urjc.es>
			
 */

#ifndef NAOOPERATOR_LEDS_GUI_H
#define NAOOPERATOR_LEDS_GUI_H

#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <string>
#include <colorspaces/colorspacesmm.h>
#include "../controllers/leds-controller.h"


namespace kinectViewerGuiModules{
/**
* \brief Class that contains all the gui functions conected to the glade interface
*/
  class ledsGui
  {
  public:
    ledsGui(kinectViewerController::LedsController *m, Glib::RefPtr<Gnome::Glade::Xml> xml);
    ~ledsGui();


  private:
	Glib::RefPtr<Gnome::Glade::Xml> refXml;
	kinectViewerController::LedsController *lc;
	Gtk::HBox *w_hbox_leds;
	Gtk::Button *w_led_off;
	Gtk::Button *w_led_green;
	Gtk::Button *w_led_red;
	Gtk::Button *w_led_yellow;
	Gtk::Button *w_led_bgreen;
	Gtk::Button *w_led_bred;
	
	void on_clicked_led_off();
	void on_clicked_led_green();
	void on_clicked_led_red();
	void on_clicked_led_yellow();
	void on_clicked_led_bgreen();
	void on_clicked_led_bred();
	



      
  };

}//namespace

#endif 
