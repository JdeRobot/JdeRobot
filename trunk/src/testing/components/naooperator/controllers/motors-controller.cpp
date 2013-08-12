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

/** \file motors-controller.cpp
 * \brief motors controller
 */

#include "motors-controller.h"

namespace DevicesController{

/**
* \brief MotorsController class construtor
* \param prx a reference to a jderobot::MotorsPrx
*/
MotorsController::MotorsController(jderobot::MotorsPrx mprx) {
	this->mprx = mprx;
}

/**
* \brief MotorsController class destructor
*/
MotorsController::~MotorsController() {
	//delete &this->mprx;
}

/**
* \brief Funtion that returns the linear speed of the robot
* \return v
*/
float
MotorsController::getV()
{
	return this->mprx->getV();
}

/**
* \brief Funtion that returns the angular speed of the robot
* \return w
*/
float
MotorsController::getW()
{
	return this->mprx->getW();
}

/**
* \brief Funtion that returns the lateral speed of the robot
* \return l
*/
float
MotorsController::getL()
{
	return this->mprx->getL();
}

/**
* \brief Funtion that sets the linear speed of the robot
* \return v
*/
void
MotorsController::setV(float v)
{
	this->mprx->setV(v);
}

/**
* \brief Funtion that sets the angular speed of the robot
* \return v
*/
void
MotorsController::setW(float w)
{
	this->mprx->setW(w);
}

/**
* \brief Funtion that sets the lateral speed of the robot
* \return v
*/
void
MotorsController::setL(float l)
{
	this->mprx->setL(l);
}

};
