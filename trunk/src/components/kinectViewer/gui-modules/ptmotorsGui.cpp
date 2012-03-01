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

/** \file ptmotorsGui.cpp
 * \brief SubClass of naooperatorgui to manage the widgets about the ptmotors module
 */

#include "ptmotorsGui.h" 
#include <iostream>
#include <cmath>

namespace kinectViewerGuiModules
{
/**
* \brief ptmotorsGui class construtor
* \return void
*/
ptmotorsGui::ptmotorsGui(kinectViewerController::PTMotorsController *m, Glib::RefPtr<Gnome::Glade::Xml> xml){
	this->refXml=xml;
	this->ptmc=m;

	refXml->get_widget("vbox_ptmotors",w_vbox_ptmotors);
	refXml->get_widget("button_up", w_up);
	refXml->get_widget("button_down", w_down);

	w_vbox_ptmotors->show();
	w_up->signal_clicked().connect(sigc::mem_fun(this,&ptmotorsGui::on_clicked_up));
	w_down->signal_clicked().connect(sigc::mem_fun(this,&ptmotorsGui::on_clicked_down));
}

void ptmotorsGui::on_clicked_up(){
	float position;

	position=ptmc->getPosition();
	position = position + 1;
	ptmc->setPosition(position);
}

void ptmotorsGui::on_clicked_down(){
	float position;

	position=ptmc->getPosition();
	position = position - 1;
	ptmc->setPosition(position);
}



};
