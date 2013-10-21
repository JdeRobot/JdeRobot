/*
 *  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors : Borja Menéndez <borjamonserrano@gmail.com>
 *
 */

#include "NeckSpeed.h"

const float	NeckSpeed::VEL_PAN_MIN   =   0.02;
const float	NeckSpeed::VEL_PAN_MAX   =   0.3;		// Real robot
const float	NeckSpeed::VEL_TILT_MIN  =   0.02;
const float	NeckSpeed::VEL_TILT_MAX  =   0.2;		// Real robot

const float NeckSpeed::MAXTILT[MAX_TILT_VECTOR] = {14.0f, 14.0f, 14.0f, 14.0f, 14.0f,
                                                   16.0f, 18.0f, 20.0f, 22.0f, 26.0f,
                                                   30.0f, 30.0f, 30.0f, 30.0f, 30.0f,
                                                   26.0f, 22.0f, 20.0f, 18.0f, 16.0f,
                                                   14.0f, 14.0f, 14.0f, 14.0f, 14.0f};

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
NeckSpeed::NeckSpeed () {
    this->pan = new JointControl("HeadYaw", MIN_PAN_ANGLE / MAX_PANU, MAX_PAN_ANGLE / MAX_PANU, VEL_PAN_MIN, VEL_PAN_MAX);
	this->tilt = new JointControl("HeadPitch", MIN_TILT_ANGLE / MAX_TILT, MAX_TILT_ANGLE / MAX_TILT, VEL_TILT_MIN, VEL_TILT_MAX);
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
NeckSpeed::~NeckSpeed () {
    delete this->pan;
    delete this->tilt;
}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void NeckSpeed::init ( const std::string newName,
                       AL::ALPtr<AL::ALBroker> parentBroker,
                       float stiffness, float speed ) {
    this->name = newName;
    this->stiffness = stiffness;
    this->speed = speed;
    
    this->motion = parentBroker->getMotionProxy();
    
    this->pan->init(parentBroker);
	this->tilt->init(parentBroker);
}

float NeckSpeed::getTiltLimit ( float currentYaw, float currentVy ) {
	if (currentVy < 0)
		return 0.0;

	int idxm, idxM;

	idxm = (toDegrees(currentYaw) / 10) + (MAX_TILT_VECTOR / 2);

	if (currentYaw < 0){
		if (idxm > 0)
			idxM = idxm - 1;
		else
			idxM = idxm;
	}else{
		if (idxm < MAX_TILT_VECTOR - 1)
			idxM = idxm + 1;
		else
			idxM = idxm;
	}

	return toRadians((MAXTILT[idxM] + MAXTILT[idxm]) / 2.0);
}

/*************************************************************
 * POSE3DMOTORS
 *************************************************************/
Ice::Int NeckSpeed::setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
    this->pan->set(-data->panSpeed);
    this->tilt->set(data->tiltSpeed);
    
    // --- Head Control ---
	this->pan->move();

	// --- Tilt control ---
	if (this->tilt->getOutput() > 0.0)
		target = this->getTiltLimit(this->pan->getJointValue(), this->tilt->getOutput());
	else
		target = toRadians(MIN_TILT);
	this->tilt->setLimitedTarget(target);
	this->tilt->move();
};

jderobot::Pose3DMotorsDataPtr NeckSpeed::getPose3DMotorsData ( const Ice::Current& ) {
    return NULL;
};

jderobot::Pose3DMotorsParamsPtr NeckSpeed::getPose3DMotorsParams ( const Ice::Current& ) {
	return NULL;
};
