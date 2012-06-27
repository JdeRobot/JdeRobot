/*
 *  Copyright (C) 1997-2011 JDE Developers Team
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

/** \file Pose3DMotorsGui.cpp
 * \brief SubClass of naooperatorgui to manage the widgets about the Pose3DMotors module
 */

#include "Pose3DMotorsGui.h" 
#include <iostream>
#include <cmath>

namespace kinectViewerGuiModules
{
/**
* \brief Pose3DMotorsGui class construtor
* \return void
*/
Pose3DMotorsGui::Pose3DMotorsGui(kinectViewerController::Pose3DMotorsController *m, Glib::RefPtr<Gnome::Glade::Xml> xml){
	this->refXml=xml;
	this->ptmc=m;

	refXml->get_widget("vbox_Pose3DMotors",w_vbox_Pose3DMotors);
	refXml->get_widget("button_up", w_up);
	refXml->get_widget("button_down", w_down);

	w_vbox_Pose3DMotors->show();
	w_up->signal_clicked().connect(sigc::mem_fun(this,&Pose3DMotorsGui::on_clicked_up));
	w_down->signal_clicked().connect(sigc::mem_fun(this,&Pose3DMotorsGui::on_clicked_down));
}

void Pose3DMotorsGui::on_clicked_up(){
	float position;

	position=ptmc->getPosition();
	position = position + 1;
	ptmc->setPosition(position);
}

void Pose3DMotorsGui::on_clicked_down(){
	float position;

	position=ptmc->getPosition();
	position = position - 1;
	ptmc->setPosition(position);
}



};
