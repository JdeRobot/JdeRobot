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

/** \file ptmotors-controller.cpp
 * \brief ptmotors controller
 */

#include "ptmotors-controller.h"

namespace kinectViewerController{
/**
* \brief PTMotorsController class construtor
* \param prx a reference to a jderobot::PTMotorsPrx
*/
PTMotorsController::PTMotorsController(jderobot::PTMotorsPrx prx) {
	this->ptmprx = prx;
}


/**
* \brief PTMotorsController class destructor
*/
PTMotorsController::~PTMotorsController() {
	
}

/**
* \brief Function that returns the camera motor position
* \return a float value with the camere motor position
*/

float
PTMotorsController::getPosition(){
	jderobot::PTMotorsDataPtr p;

	p=this->ptmprx->getPTMotorsData();
	return p->latitude;
}

/**
* \brief Function that sets a camera motor position
* \param value the position of the camera
*/

int
PTMotorsController::setPosition(float value){
	jderobot::PTMotorsDataPtr p;

	p=this->ptmprx->getPTMotorsData();
	p->latitude=value;
	this->ptmprx->setPTMotorsData(p);
}

};
