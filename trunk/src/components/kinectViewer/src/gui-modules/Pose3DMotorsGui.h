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

#ifndef NAOOPERATOR_Pose3DMotors_GUI_H
#define NAOOPERATOR_Pose3DMotors_GUI_H

#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <string>
#include <colorspaces/colorspacesmm.h>
#include "../controllers/Pose3DMotors-controller.h"


namespace kinectViewerGuiModules{
/**
* \brief Class that contains all the gui functions conected to the glade interface
*/
  class Pose3DMotorsGui
  {
  public:
    Pose3DMotorsGui(kinectViewerController::Pose3DMotorsController *m, Glib::RefPtr<Gnome::Glade::Xml> xml);
    ~Pose3DMotorsGui();


  private:
	Glib::RefPtr<Gnome::Glade::Xml> refXml;
	kinectViewerController::Pose3DMotorsController *ptmc;
	Gtk::Button * w_up;
	Gtk::Button * w_down;
	Gtk::VBox *w_vbox_Pose3DMotors;
	void on_clicked_up();
	void on_clicked_down();


      
  };

}//namespace

#endif 
