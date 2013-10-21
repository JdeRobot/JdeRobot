/*
 *  Copyright (C) 1997-2010 JDE Developers Team
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

/** \file bodymotorsGui.cpp
 * \brief SubClass of naooperatorgui to manage the widgets about the bodymotors module
 */

#include "bodymotorsGui.h" 
#include <iostream>
#include <cmath>
#include <jderobot/body.h>

namespace guiModules
{
/**
* \brief bodymotorsGui class construtor
* \return void
*/
bodymotorsGui::bodymotorsGui(DevicesController::BodyMotorsController *m, Glib::RefPtr<Gnome::Glade::Xml> xml)
{
	bodymotors_ctr = m;
	this->refXml=xml;
	/* As all the widget are hided we have to show the widgets that this module use from the glade interface*/
	Gtk::VBox *vbox;
	Gtk::HBox *hbox;
	std::vector<float> limits;

	refXml->get_widget("vbox_body",vbox);
	refXml->get_widget("toggle_sensors_move", w_encoders);
	vbox->show();
	refXml->get_widget("vbox_hold_options",vbox);
	vbox->show();
	refXml->get_widget("hbox_body_speed",hbox);
	hbox->show();

	refXml->get_widget("vscale_l_shoulder_p",w_scale_l_shoulder_p);
	refXml->get_widget("vscale_l_shoulder_r",w_scale_l_shoulder_r);
	refXml->get_widget("vscale_l_elbow_y",w_scale_l_elbow_y);
	refXml->get_widget("vscale_l_elbow_r",w_scale_l_elbow_r);
	refXml->get_widget("vscale_r_shoulder_p",w_scale_r_shoulder_p);
	refXml->get_widget("vscale_r_shoulder_r",w_scale_r_shoulder_r);
	refXml->get_widget("vscale_r_elbow_y",w_scale_r_elbow_y);
	refXml->get_widget("vscale_r_elbow_r",w_scale_r_elbow_r);
	refXml->get_widget("vscale_l_hip_yaw",w_scale_l_hip_y);
	refXml->get_widget("vscale_l_hip_pitch",w_scale_l_hip_p);
	refXml->get_widget("vscale_l_hip_roll",w_scale_l_hip_r);
	refXml->get_widget("vscale_l_knee_pitch",w_scale_l_knee_p);
	refXml->get_widget("vscale_l_ankle_pitch",w_scale_l_ankle_p);
	refXml->get_widget("vscale_l_ankle_roll",w_scale_l_ankle_r);
	refXml->get_widget("vscale_r_hip_yaw",w_scale_r_hip_y);
	refXml->get_widget("vscale_r_hip_pitch",w_scale_r_hip_p);
	refXml->get_widget("vscale_r_hip_roll",w_scale_r_hip_r);
	refXml->get_widget("vscale_r_knee_pitch",w_scale_r_knee_p);
	refXml->get_widget("vscale_r_ankle_pitch",w_scale_r_ankle_p);
	refXml->get_widget("vscale_r_ankle_roll",w_scale_r_ankle_r);
	refXml->get_widget("hscale_motors_speed",w_motors_speed);
	refXml->get_widget("toggle_hip_y",w_hold_hip_y);
	refXml->get_widget("toggle_hip_pitch",w_hold_hip_p);
	refXml->get_widget("toggle_hip_roll",w_hold_hip_r);
	refXml->get_widget("toggle_knees",w_hold_knee_p);
	refXml->get_widget("toggle_ankle_pitch",w_hold_ankle_p);
	
	std::vector<Gtk::VScale *> lscales;
	std::vector<Gtk::VScale *> rscales;

	lscales.push_back(w_scale_l_hip_y);
	lscales.push_back(w_scale_l_hip_p);
	lscales.push_back(w_scale_l_hip_r);
	lscales.push_back(w_scale_l_knee_p);
	lscales.push_back(w_scale_l_ankle_p);
	lscales.push_back(w_scale_l_ankle_r);
	lscales.push_back(w_scale_l_shoulder_p);
	lscales.push_back(w_scale_l_shoulder_r);
	lscales.push_back(w_scale_l_elbow_y);
	lscales.push_back(w_scale_l_elbow_r);
	rscales.push_back(w_scale_r_hip_y);
	rscales.push_back(w_scale_r_hip_p);
	rscales.push_back(w_scale_r_hip_r);
	rscales.push_back(w_scale_r_knee_p);
	rscales.push_back(w_scale_r_ankle_p);
	rscales.push_back(w_scale_r_ankle_r);
	rscales.push_back(w_scale_r_shoulder_p);
	rscales.push_back(w_scale_r_shoulder_r);
	rscales.push_back(w_scale_r_elbow_y);
	rscales.push_back(w_scale_r_elbow_r);
	
	jderobot::MotorsName name;
	for (int i=0; i< 10; i++){
		name=jderobot::MotorsName(i);
		limits=bodymotors_ctr->getMotorLimit(name, jderobot::Left);
		lscales[i]->set_range(limits[0],limits[1]);		
		limits=bodymotors_ctr->getMotorLimit(name, jderobot::Right);
		rscales[i]->set_range(limits[0],limits[1]);
	}
	w_motors_speed->set_range(0,limits[2]);

	w_scale_l_hip_y->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_l_hip_y_changed));
	w_scale_l_hip_p->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_l_hip_p_changed));
	w_scale_l_hip_r->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_l_hip_r_changed));
	w_scale_l_knee_p->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_l_knee_p_changed));
	w_scale_l_ankle_p->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_l_ankle_p_changed));
	w_scale_l_ankle_r->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_l_ankle_r_changed));
	w_scale_r_hip_y->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_r_hip_y_changed));
	w_scale_r_hip_p->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_r_hip_p_changed));
	w_scale_r_hip_r->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_r_hip_r_changed));
	w_scale_r_knee_p->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_r_knee_p_changed));
	w_scale_r_ankle_p->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_r_ankle_p_changed));
	w_scale_r_ankle_r->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_r_ankle_r_changed));
	w_scale_l_shoulder_p->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_l_shoulder_p_changed));
	w_scale_l_shoulder_r->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_l_shoulder_r_changed));
	w_scale_l_elbow_y->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_l_elbow_y_changed));
	w_scale_l_elbow_r->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_l_elbow_r_changed));
	w_scale_r_shoulder_p->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_r_shoulder_p_changed));
	w_scale_r_shoulder_r->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_r_shoulder_r_changed));
	w_scale_r_elbow_y->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_r_elbow_y_changed));
	w_scale_r_elbow_r->signal_change_value().connect(sigc::mem_fun(this,&bodymotorsGui::on_scale_r_elbow_r_changed));
}


/**
* \brief Function that describes what to do if the Left HipYawPitch scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_l_hip_y_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::HipYawPitch, jderobot::Left, new_value, w_motors_speed->get_value());
		if (w_hold_hip_y->get_active()){
			w_scale_r_hip_y->set_value(new_value);
			bodymotors_ctr->setMotorPosition(jderobot::HipYawPitch, jderobot::Right, new_value, w_motors_speed->get_value());
		}
	}	
}

/**
* \brief Function that describes what to do if the Left HipPitch scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_l_hip_p_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::HipPitch, jderobot::Left, new_value, w_motors_speed->get_value());
		if (w_hold_hip_p->get_active()){
			w_scale_r_hip_p->set_value(new_value);
			bodymotors_ctr->setMotorPosition(jderobot::HipPitch, jderobot::Right, new_value, w_motors_speed->get_value());
		}
	}	
}


/**
* \brief Function that describes what to do if the Left HipRoll scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_l_hip_r_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::HipRoll, jderobot::Left, new_value, w_motors_speed->get_value());
		if (w_hold_hip_r->get_active()){
			w_scale_r_hip_r->set_value(new_value);
			bodymotors_ctr->setMotorPosition(jderobot::HipRoll, jderobot::Right, new_value, w_motors_speed->get_value());
		}
	}	
}


/**
* \brief Function that describes what to do if the Left KneePitch scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_l_knee_p_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::KneePitch, jderobot::Left, new_value, w_motors_speed->get_value());
		if (w_hold_knee_p->get_active()){
			w_scale_r_knee_p->set_value(new_value);
			bodymotors_ctr->setMotorPosition(jderobot::KneePitch, jderobot::Right, new_value, w_motors_speed->get_value());
		}
	}	
}

/**
* \brief Function that describes what to do if the Left AnklePitch scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_l_ankle_p_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::AnklePitch, jderobot::Left, new_value, w_motors_speed->get_value());
		if (w_hold_ankle_p->get_active()){
			w_scale_r_ankle_p->set_value(new_value);
			bodymotors_ctr->setMotorPosition(jderobot::AnklePitch, jderobot::Right, new_value, w_motors_speed->get_value());
		}
	}	
}


/**
* \brief Function that describes what to do if the Left AnkleRoll scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_l_ankle_r_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::AnkleRoll, jderobot::Left, new_value, w_motors_speed->get_value());
	}	
}


/**
* \brief Function that describes what to do if the Right HipYawPitch scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_r_hip_y_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::HipYawPitch, jderobot::Right, new_value, w_motors_speed->get_value());
		if (w_hold_hip_y->get_active()){
			w_scale_l_hip_y->set_value(new_value);
			bodymotors_ctr->setMotorPosition(jderobot::HipYawPitch, jderobot::Left, new_value, w_motors_speed->get_value());
		}
	}	
}

/**
* \brief Function that describes what to do if the Right HipPitch scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_r_hip_p_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::HipPitch, jderobot::Right, new_value, w_motors_speed->get_value());
		if (w_hold_hip_p->get_active()){
			w_scale_l_hip_p->set_value(new_value);
			bodymotors_ctr->setMotorPosition(jderobot::HipPitch, jderobot::Left, new_value, w_motors_speed->get_value());
		}
	}	
}


/**
* \brief Function that describes what to do if the Right HipRoll scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_r_hip_r_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::HipRoll, jderobot::Right, new_value, w_motors_speed->get_value());
		if (w_hold_hip_r->get_active()){
			w_scale_l_hip_r->set_value(new_value);
			bodymotors_ctr->setMotorPosition(jderobot::HipRoll, jderobot::Left, new_value, w_motors_speed->get_value());
		}
	}	
}


/**
* \brief Function that describes what to do if the Right KneePitch scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_r_knee_p_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::KneePitch, jderobot::Right, new_value, w_motors_speed->get_value());
		if (w_hold_knee_p->get_active()){
			w_scale_l_knee_p->set_value(new_value);
			bodymotors_ctr->setMotorPosition(jderobot::KneePitch, jderobot::Left, new_value, w_motors_speed->get_value());
		}
	}	
}

/**
* \brief Function that describes what to do if the Right AnklePitch scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_r_ankle_p_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::AnklePitch, jderobot::Right, new_value, w_motors_speed->get_value());
		if (w_hold_ankle_p->get_active()){
			w_scale_l_ankle_p->set_value(new_value);
			bodymotors_ctr->setMotorPosition(jderobot::AnklePitch, jderobot::Left, new_value, w_motors_speed->get_value());
		}
	}	
}


/**
* \brief Function that describes what to do if the Right AnkleRoll scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_r_ankle_r_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::AnkleRoll, jderobot::Right, new_value, w_motors_speed->get_value());
	}	
}


/**
* \brief Function that describes what to do if the Left ShoulderPitch scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_l_shoulder_p_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::ShoulderPitch, jderobot::Left, new_value, w_motors_speed->get_value());
	}	
}


/**
* \brief Function that describes what to do if the Left ShoulderRoll scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_l_shoulder_r_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::ShoulderRoll, jderobot::Left, new_value, w_motors_speed->get_value());
	}	
}


/**
* \brief Function that describes what to do if the Left ElbowYaw scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_l_elbow_y_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::ElbowYaw, jderobot::Left, new_value, w_motors_speed->get_value());
	}	
}


/**
* \brief Function that describes what to do if the Left ElbowRoll scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_l_elbow_r_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::ElbowRoll, jderobot::Left, new_value, w_motors_speed->get_value());
	}	
}


/**
* \brief Function that describes what to do if the Right ShoulderPitch scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_r_shoulder_p_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::ShoulderPitch, jderobot::Right, new_value, w_motors_speed->get_value());
	}	
}


/**
* \brief Function that describes what to do if the Right ShoulderRoll scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_r_shoulder_r_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::ShoulderRoll, jderobot::Right, new_value, w_motors_speed->get_value());
	}	
}


/**
* \brief Function that describes what to do if the Right ElbowYaw scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_r_elbow_y_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::ElbowYaw, jderobot::Right, new_value, w_motors_speed->get_value());
	}	
}


/**
* \brief Function that describes what to do if the Right ElbowRoll scale value changes
* \return void
*/
gboolean
bodymotorsGui::on_scale_r_elbow_r_changed(Gtk::ScrollType scroll, double new_value){

	if (!this->w_encoders->get_active()){
		bodymotors_ctr->setMotorPosition(jderobot::ElbowRoll, jderobot::Right, new_value, w_motors_speed->get_value());
	}	
}

}//namespace
