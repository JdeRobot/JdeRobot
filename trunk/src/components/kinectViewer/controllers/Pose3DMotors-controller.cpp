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

/** \file Pose3DMotors-controller.cpp
 * \brief Pose3DMotors controller
 */

#include "Pose3DMotors-controller.h"

namespace kinectViewerController{
/**
* \brief Pose3DMotorsController class construtor
* \param prx a reference to a jderobot::Pose3DMotorsPrx
*/
Pose3DMotorsController::Pose3DMotorsController(jderobot::Pose3DMotorsPrx prx) {
	this->ptmprx = prx;
}


/**
* \brief Pose3DMotorsController class destructor
*/
Pose3DMotorsController::~Pose3DMotorsController() {
	
}

/**
* \brief Function that returns the camera motor position
* \return a float value with the camere motor position
*/

float
Pose3DMotorsController::getPosition(){
	jderobot::Pose3DMotorsDataPtr p;

	p=this->ptmprx->getPose3DMotorsData();
	return p->tilt;
}

/**
* \brief Function that sets a camera motor position
* \param value the position of the camera
*/

int
Pose3DMotorsController::setPosition(float value){
	jderobot::Pose3DMotorsDataPtr p;

	p=this->ptmprx->getPose3DMotorsData();
	p->tilt=value;
	this->ptmprx->setPose3DMotorsData(p);
}

};
