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

/** \file bodymotors-controller.cpp
 * \brief bodymotors controller
 */

#include "bodymotors-controller.h"

namespace DevicesController{

/**
* \brief BodyMotorsController class construtor
* \param prx a reference to a jderobot::BodyMotorsPrx
*/
BodyMotorsController::BodyMotorsController(jderobot::BodyMotorsPrx prx) {
	this->bmprx = prx;

}

/**
* \brief NaoOperatorGui class destructor
* \return Nothing
*/
BodyMotorsController::~BodyMotorsController() {
	//delete &this->mprx;
}

/**
* \brief Funtion that the ruturns the limits of a motor
* \param name The name of the motor
* \param side The side of the motor
* \return Returns a vector<float> containing the following information: minAngle, maxAngle, maxSpeed
*/
std::vector<float>
BodyMotorsController::getMotorLimit(jderobot::MotorsName name, jderobot::BodySide side){
	jderobot::BodyMotorsParamPtr param;
	std::vector<float> limits;
	
	param= bmprx->getBodyMotorsParam(name,side);
	limits.push_back(param->minAngle);
	limits.push_back(param->maxAngle);
	limits.push_back(param->maxSpeed);
	return limits;
}

/**
* \brief Funtion that changes a motor position
* \param name The name of the motor
* \param side The side of the motor
* \param value The angle to the motor
* \param speed The speed to the motor
* \return 0 if ok, otherwise -1
*/
int
BodyMotorsController::setMotorPosition(jderobot::MotorsName name, jderobot::BodySide side, float value, float speed){
	return bmprx->setBodyMotorsData(name,side,value,speed);
}


};
