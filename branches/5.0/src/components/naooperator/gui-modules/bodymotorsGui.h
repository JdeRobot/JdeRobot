/*
 *  Copyright (C) 1997-2009 JDE Developers Team
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

#ifndef NAOOPERATOR_BODYMOTORS_GUI_H
#define NAOOPERATOR_BODYMOTORS_GUI_H

#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <string>
#include <colorspaces/colorspacesmm.h>
#include "../controllers/bodymotors-controller.h"



namespace guiModules{
/**
* \brief Class that contains all the gui functions conected to the glade interface
*/
  class bodymotorsGui
  {
  public:
    bodymotorsGui(DevicesController::BodyMotorsController *m, Glib::RefPtr<Gnome::Glade::Xml> xml);
    ~bodymotorsGui();
	void update();


  private:
    Glib::RefPtr<Gnome::Glade::Xml> refXml;
	DevicesController::BodyMotorsController* bodymotors_ctr;
	Gtk::ToggleButton* w_encoders;
	Gtk::VScale *w_scale_l_shoulder_p;
	Gtk::VScale *w_scale_l_shoulder_r;
	Gtk::VScale *w_scale_l_elbow_y;
	Gtk::VScale *w_scale_l_elbow_r;
	Gtk::VScale *w_scale_r_shoulder_p;
	Gtk::VScale *w_scale_r_shoulder_r;
	Gtk::VScale *w_scale_r_elbow_y;
	Gtk::VScale *w_scale_r_elbow_r;
	Gtk::VScale *w_scale_l_hip_y;
	Gtk::VScale *w_scale_l_hip_p;
	Gtk::VScale *w_scale_l_hip_r;
	Gtk::VScale *w_scale_l_knee_p;
	Gtk::VScale *w_scale_l_ankle_p;
	Gtk::VScale *w_scale_l_ankle_r;
	Gtk::VScale *w_scale_r_hip_y;
	Gtk::VScale *w_scale_r_hip_p;
	Gtk::VScale *w_scale_r_hip_r;
	Gtk::VScale *w_scale_r_knee_p;
	Gtk::VScale *w_scale_r_ankle_p;
	Gtk::VScale *w_scale_r_ankle_r;
	Gtk::HScale *w_motors_speed;
	Gtk::ToggleButton* w_hold_hip_y;
	Gtk::ToggleButton* w_hold_hip_p;
	Gtk::ToggleButton* w_hold_hip_r;
	Gtk::ToggleButton* w_hold_knee_p;
	Gtk::ToggleButton* w_hold_ankle_p;

	gboolean on_scale_l_hip_y_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_l_hip_p_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_l_hip_r_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_l_knee_p_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_l_ankle_p_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_l_ankle_r_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_r_hip_y_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_r_hip_p_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_r_hip_r_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_r_knee_p_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_r_ankle_p_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_r_ankle_r_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_l_shoulder_p_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_l_shoulder_r_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_l_elbow_y_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_l_elbow_r_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_r_shoulder_p_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_r_shoulder_r_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_r_elbow_y_changed(Gtk::ScrollType scroll, double new_value);
	gboolean on_scale_r_elbow_r_changed(Gtk::ScrollType scroll, double new_value);
  };

}//namespace

#endif 
