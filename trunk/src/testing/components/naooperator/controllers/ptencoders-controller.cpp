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

/** \file ptencoders-controller.cpp
 * \brief ptencoders controller
 */


#include "ptencoders-controller.h"

namespace DevicesController{

/**
* \brief PTEncodersController class construtor
* \param prx a reference to a jderobot::PTEncodersPrx
*/
PTEncodersController::PTEncodersController(jderobot::PTEncodersPrx prx) {
	this->pteprx = prx;
}

/**
* \brief PTEncodersController class destructor
*/
PTEncodersController::~PTEncodersController() {
	//delete &this->mprx;
}

/**
* \brief Function that returns the pantilt encoders data
* \return a std::vector<float> containing: [0] = 1 if new data is available, [1] = panAngle [2]= tiltAngle, otherwise [0]=0
*/
std::vector<float>
PTEncodersController::getValues(){
	std::vector<float> values;
	
	data=this->pteprx->getPTEncodersData();
	if (this->data->clock != this->clock){
		this->clock=this->data->clock;
		values.push_back(1);
		values.push_back(this->data->panAngle);
		values.push_back(this->data->tiltAngle);
	}
	else{
		values.push_back(0);
	}
	
	return values;
}	

};
