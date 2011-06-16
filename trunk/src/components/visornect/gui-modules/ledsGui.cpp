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

#include "ledsGui.h" 
#include <iostream>
#include <cmath>

namespace visornectGuiModules
{
/**
* \brief ptmotorsGui class construtor
* \return void
*/
ledsGui::ledsGui(visornectController::LedsController *m, Glib::RefPtr<Gnome::Glade::Xml> xml){
	this->refXml=xml;
	this->lc=m;

	refXml->get_widget("hbox_leds",w_hbox_leds);
	refXml->get_widget("buttonled_off", w_led_off);
	refXml->get_widget("buttonled_green", w_led_green);
	refXml->get_widget("buttonled_red", w_led_red);
	refXml->get_widget("buttonled_yellow", w_led_yellow);
	refXml->get_widget("buttonled_bgreen", w_led_bgreen);
	refXml->get_widget("buttonled_bred", w_led_bred);

	w_hbox_leds->show();

	w_led_off->signal_clicked().connect(sigc::mem_fun(this,&ledsGui::on_clicked_led_off));
	w_led_green->signal_clicked().connect(sigc::mem_fun(this,&ledsGui::on_clicked_led_green));
	w_led_red->signal_clicked().connect(sigc::mem_fun(this,&ledsGui::on_clicked_led_red));
	w_led_yellow->signal_clicked().connect(sigc::mem_fun(this,&ledsGui::on_clicked_led_yellow));
	w_led_bgreen->signal_clicked().connect(sigc::mem_fun(this,&ledsGui::on_clicked_led_bgreen));
	w_led_bred->signal_clicked().connect(sigc::mem_fun(this,&ledsGui::on_clicked_led_bred));
}


void ledsGui::on_clicked_led_off(){
	lc->setLedActiveOff();
}

void ledsGui::on_clicked_led_green(){
	lc->setLedActiveGreen();
}

void ledsGui::on_clicked_led_red(){
	lc->setLedActiveRed();
}

void ledsGui::on_clicked_led_yellow(){
	lc->setLedActiveYellow();
}

void ledsGui::on_clicked_led_bgreen(){
	lc->setLedActiveBGreen();
}

void ledsGui::on_clicked_led_bred(){
	lc->setLedActiveBYellow();
}


};
