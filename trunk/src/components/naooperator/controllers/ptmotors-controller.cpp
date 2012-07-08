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

/** \file ptmotors-controller.cpp
 * \brief ptmotors controller
 */

#include "ptmotors-controller.h"

namespace DevicesController{
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
	//delete &this->mprx;
}

/**
* \brief Function that gets the pantilt parametres
* \return a std::vector<float> containing: [0] maxLongitude, [1] minLongitude, [2] maxLatitude, [3] minLatitude, [4] maxLongitudeSpeed, [5] maxLatitudeSpeed
*/
std::vector<float>
PTMotorsController::getPTMotorsParams(){
	jderobot::PTMotorsParamsPtr p;
	std::vector<float> values;
		
/*
	1. maxLongitude;
	2. minLongitude;
	3. maxLatitude;
	4. minLatitude;
	5. maxLongitudeSpeed;
	6. maxLatitudeSpeed;*/

	p=ptmprx->getPTMotorsParams();
	values.push_back(p->maxLongitude);
	values.push_back(p->minLongitude);
	values.push_back(p->maxLatitude);
	values.push_back(p->minLatitude);
	values.push_back(p->maxLongitudeSpeed);
	values.push_back(p->maxLatitudeSpeed);
	return values;
}

/**
* \brief Function that sets the pantilt data
* \param longitude the value for the longitude angle
* \param longitudeSpeed the speed for the longitude motor
* \param latitude the value for the latitude angle
* \param latitudeSpeed the speed for the latitude motor
* \return 0 if ok, otherwise -1
*/
int
PTMotorsController::setPTMotorsData(float longitude, float longitudeSpeed, float latitude, float latitudeSpeed){
	jderobot::PTMotorsDataPtr p;

	p=ptmprx->getPTMotorsData();
	p->latitude=latitude;
	p->longitude=longitude;
	p->latitudeSpeed=latitudeSpeed;
	p->longitudeSpeed=longitudeSpeed;
	
	return ptmprx->setPTMotorsData(p);
}


};
