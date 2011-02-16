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

/** \file bodyencoders-controller.cpp
 * \brief bodyencoders controller
 * \return Nothing
 */

#include "bodyencoders-controller.h"

namespace DevicesController{

/**
* \brief BodyEncodersController class construtor
* \param prx a reference to a jderobot::BodyEncodersPrx
*/
BodyEncodersController::BodyEncodersController(jderobot::BodyEncodersPrx prx) {
	this->beprx = prx;
	this->leftLegClock=0;
	this->rightLegClock=0;
	this->leftArmClock=0;
	this->rightArmClock=0;
}

/**
* \brief NaoOperatorGui class destructor
* \return Nothing
*/
BodyEncodersController::~BodyEncodersController() {
	//delete &this->mprx;
}

/**
* \brief Funtion that the ruturns the values of the left leg motors position
* \return Returns a vector<float> containing the following information: HipYaw, HipYawPitch, HipRoll, KneePitch, AnklePitch, AnkleRoll (degrees)
*/
std::vector<float>
BodyEncodersController::getLeftLegValues(){
	std::vector<float> values;
	
	legData=this->beprx->getLegEncodersData(jderobot::Left);
	if (this->legData->clock != this->leftLegClock){
		this->leftLegClock=this->legData->clock;
		values.push_back(1);
		values.push_back(this->legData->hip.yaw);
		values.push_back(this->legData->hip.pitch);
		values.push_back(this->legData->hip.roll);
		values.push_back(this->legData->knee.pitch);
		values.push_back(this->legData->ankle.pitch);
		values.push_back(this->legData->ankle.roll);
	}
	else{
		values.push_back(0);
	}
	return values;
}	

/**
* \brief Funtion that the ruturns the values of the right leg motors position
* \return Returns a vector<float> containing the following information: HipYaw, HipYawPitch, HipRoll, KneePitch, AnklePitch, AnkleRoll (degrees)
*/
std::vector<float>
BodyEncodersController::getRightLegValues(){
	std::vector<float> values;
	
	legData=this->beprx->getLegEncodersData(jderobot::Right);

	if (this->legData->clock != this->rightLegClock){
		this->rightLegClock=this->legData->clock;
		values.push_back(1);
		values.push_back(this->legData->hip.yaw);
		values.push_back(this->legData->hip.pitch);
		values.push_back(this->legData->hip.roll);
		values.push_back(this->legData->knee.pitch);
		values.push_back(this->legData->ankle.pitch);
		values.push_back(this->legData->ankle.roll);
	}
	else{
		values.push_back(0);
	}
	return values;
}

/**
* \brief Funtion that the ruturns the values of the left arm motors position
* \return Returns a vector<float> containing the following information: ShoulderPitch, ShoulderRoll, ElbowYaw, ElbowRoll (degrees)
*/
std::vector<float>
BodyEncodersController::getLeftArmValues(){
	std::vector<float> values;
	
	armData=this->beprx->getArmEncodersData(jderobot::Left);

	if (this->armData->clock != this->leftArmClock){
		this->leftArmClock=this->armData->clock;
		values.push_back(1);
		values.push_back(this->armData->shoulder.pitch);
		values.push_back(this->armData->shoulder.yaw);
		values.push_back(this->armData->elbow.yaw);
		values.push_back(this->armData->elbow.roll);
	}
	else{
		values.push_back(0);
	}
	return values;
}

/**
* \brief Funtion that the ruturns the values of the right arm motors position
* \return Returns a vector<float> containing the following information: ShoulderPitch, ShoulderRoll, ElbowYaw, ElbowRoll (degrees)
*/
std::vector<float>
BodyEncodersController::getRightArmValues(){
	std::vector<float> values;
	
	armData=this->beprx->getArmEncodersData(jderobot::Right);

	if (this->armData->clock != this->rightArmClock){
		this->rightArmClock=this->armData->clock;
		values.push_back(1);
		values.push_back(this->armData->shoulder.pitch);
		values.push_back(this->armData->shoulder.yaw);
		values.push_back(this->armData->elbow.yaw);
		values.push_back(this->armData->elbow.roll);
	}
	else{
		values.push_back(0);
	}
	return values;
}

};
