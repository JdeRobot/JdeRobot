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

/** \file bodyencodersGui.cpp
 * \brief SubClass of naooperatorgui to manage the widgets about the bodyencoders module
 */

#include "bodyencodersGui.h" 
#include <iostream>
#include <cmath>

namespace guiModules
{
/**
* \brief bodyencodersGui class construtor
* \return void
*/
bodyencodersGui::bodyencodersGui(DevicesController::BodyEncodersController *m, Glib::RefPtr<Gnome::Glade::Xml> xml)
{
	bodyencoders_ctr = m;
	this->refXml=xml;
	/* As all the widget are hided we have to show the widgets that this module use from the glade interface*/
	Gtk::VBox *vbox;

	refXml->get_widget("vbox_body",vbox);
	refXml->get_widget("toggle_sensors_move", w_encoders);
	vbox->show();

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
}

void
bodyencodersGui::update(){
	if (this->w_encoders->get_active()){
		std::vector<float> leftLeg = bodyencoders_ctr->getLeftLegValues();
		if (leftLeg[0]==1){
			w_scale_l_hip_y->set_value(leftLeg[1]);
			w_scale_l_hip_p->set_value(leftLeg[2]);
			w_scale_l_hip_r->set_value(leftLeg[3]);
			w_scale_l_knee_p->set_value(leftLeg[4]);
			w_scale_l_ankle_p->set_value(leftLeg[5]);	
			w_scale_l_ankle_r->set_value(leftLeg[6]);
		}
		std::vector<float> rightLeg = bodyencoders_ctr->getRightLegValues();
		if (rightLeg[0]==1){
			w_scale_r_hip_y->set_value(rightLeg[1]);
			w_scale_r_hip_p->set_value(rightLeg[2]);
			w_scale_r_hip_r->set_value(rightLeg[3]);
			w_scale_r_knee_p->set_value(rightLeg[4]);
			w_scale_r_ankle_p->set_value(rightLeg[5]);	
			w_scale_r_ankle_r->set_value(rightLeg[6]);
		}
		std::vector<float> leftArm = bodyencoders_ctr->getLeftArmValues();
		if (leftArm[0]==1){
			w_scale_l_shoulder_p->set_value(leftArm[1]);
			w_scale_l_shoulder_r->set_value(leftArm[2]);
			w_scale_l_elbow_y->set_value(leftArm[3]);
			w_scale_l_elbow_r->set_value(leftArm[4]);
		}
		std::vector<float> rightArm = bodyencoders_ctr->getRightArmValues();
		if (rightArm[0]==1){
			w_scale_r_shoulder_p->set_value(rightArm[1]);
			w_scale_r_shoulder_r->set_value(rightArm[2]);
			w_scale_r_elbow_y->set_value(rightArm[3]);
			w_scale_r_elbow_r->set_value(rightArm[4]);
		}
	}
}



}//namespace
