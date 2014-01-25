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

#include "HingeMotors.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
HingeMotors::HingeMotors () {}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
HingeMotors::~HingeMotors () {}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void HingeMotors::init ( const std::string newName,
                         AL::ALPtr<AL::ALBroker> parentBroker,
                         float stiffness, float speed ) {
    this->name = newName;
    this->stiffness = stiffness;
    this->speed = speed;
    
    this->tilt = 0.0;
    this->pan = 0.0;
    this->roll = 0.0;
    
    try {
        this->motion = parentBroker->getMotionProxy();
        if (this->bPitch)
            this->motion->setStiffnesses(this->jointPitch, this->stiffness);
        if (this->bYaw)
            this->motion->setStiffnesses(this->jointYaw, this->stiffness);
        if (this->bRoll)
            this->motion->setStiffnesses(this->jointRoll, this->stiffness);
    } catch ( AL::ALError& e ) {
		std::cerr << "[HingeMotors ()::init(): " << e.toString() << std::endl;
	}
}

/*************************************************************
 * POSE3DMOTORS
 *************************************************************/
Ice::Int HingeMotors::setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
    if (this->bPitch) {
        this->tilt = data->tilt;
        this->motion->setAngles(this->jointPitch, data->tilt, this->speed);
    }
    
    if (this->bYaw) {
        this->pan = data->pan;
        this->motion->setAngles(this->jointYaw, data->pan, this->speed);
    }
    
    if (this->bRoll) {
        this->roll = data->roll;
        this->motion->setAngles(this->jointRoll, data->roll, this->speed);
    }
    
	return 0; 
};

jderobot::Pose3DMotorsDataPtr HingeMotors::getPose3DMotorsData ( const Ice::Current& ) {
    jderobot::Pose3DMotorsDataPtr pose3DMotorsData;

    if (this->bPitch)
        pose3DMotorsData->tilt = this->tilt;
    
    if (this->bYaw)
        pose3DMotorsData->pan = this->pan;
    
    if (this->bPitch)
        pose3DMotorsData->roll = this->roll;
    
	return pose3DMotorsData;
};

jderobot::Pose3DMotorsParamsPtr HingeMotors::getPose3DMotorsParams ( const Ice::Current& ) {
	return NULL;
};
