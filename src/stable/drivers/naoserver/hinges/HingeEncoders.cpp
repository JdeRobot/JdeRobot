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

#include "HingeEncoders.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
HingeEncoders::HingeEncoders () {}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
HingeEncoders::~HingeEncoders () {}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void HingeEncoders::init ( const std::string newName,
                         AL::ALPtr<AL::ALBroker> parentBroker ) {
    this->name = newName;
    
    try {
        this->motion = parentBroker->getMotionProxy();
    } catch ( AL::ALError& e ) {
		std::cerr << "[HingeEncoders ()::init(): " << e.toString() << std::endl;
	}
}

/*************************************************************
 * POSE3DENCODERS
 *************************************************************/
jderobot::Pose3DEncodersDataPtr HingeEncoders::getPose3DEncodersData ( const Ice::Current& ) {
    jderobot::Pose3DEncodersDataPtr pose3DEncodersData;

    if (this->bPitch) {
        std::vector<float> commandAngles = this->motion->getAngles(this->jointPitch, false);
        pose3DEncodersData->tilt = commandAngles[0];
    }
    
    if (this->bYaw) {
        std::vector<float> commandAngles = this->motion->getAngles(this->jointYaw, false);
        pose3DEncodersData->pan = commandAngles[0];
    }
    
    if (this->bPitch) {
        std::vector<float> commandAngles = this->motion->getAngles(this->jointRoll, false);
        pose3DEncodersData->roll = commandAngles[0];
    }
    
	return pose3DEncodersData;
};
