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

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
NeckSpeed::NeckSpeed () {
    this->jointPitch = "HeadPitch";
    this->jointYaw = "HeadYaw";
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
NeckSpeed::~NeckSpeed () {}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void NeckSpeed::init ( const std::string newName,
                       AL::ALPtr<AL::ALBroker> parentBroker,
                       float stiffness, float speed ) {
    this->name = newName;
    this->stiffness = stiffness;
    this->speed = speed;
    
    try {
        this->motion = parentBroker->getMotionProxy();
        this->motion->setStiffnesses(this->jointPitch, this->stiffness);
        this->motion->setStiffnesses(this->jointYaw, this->stiffness);
    } catch ( AL::ALError& e ) {
		std::cerr << "[HingeMotors ()::init(): " << e.toString() << std::endl;
	}
}

/*************************************************************
 * POSE3DMOTORS
 *************************************************************/
Ice::Int NeckSpeed::setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
    this->motion->changeAngles(this->jointYaw, data->panSpeed * 0.42 / 2.0, std::abs(data->panSpeed));
    this->motion->changeAngles(this->jointPitch, data->tiltSpeed * 0.32 / 2.0, std::abs(data->tiltSpeed));
};

jderobot::Pose3DMotorsDataPtr NeckSpeed::getPose3DMotorsData ( const Ice::Current& ) {
    return NULL;
};

jderobot::Pose3DMotorsParamsPtr NeckSpeed::getPose3DMotorsParams ( const Ice::Current& ) {
	return NULL;
};
