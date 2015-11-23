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

#include "NaoServerMotors.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
NaoServerMotors::NaoServerMotors () {}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
NaoServerMotors::~NaoServerMotors () {}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void NaoServerMotors::init ( const std::string newName, AL::ALPtr<AL::ALBroker> parentBroker ) {
    this->name = newName;
    
    try {
        this->motion = parentBroker->getMotionProxy();
        this->motion->setStiffnesses("Body", 1.0f);
        this->motion->walkInit();
    } catch ( AL::ALError& e ) {
		std::cerr << "[NaoServerMotors ()::init(): " << e.toString() << std::endl;
	}
}

/*************************************************************
 * MOTORS
 *************************************************************/
Ice::Float NaoServerMotors::getV ( const Ice::Current& ) {
    std::vector<float> result = this->motion->getRobotVelocity();
    return result[0];
}

Ice::Int NaoServerMotors::setV ( float v, const Ice::Current& ) {
    try {
        this->v = v;
        this->motion->setWalkTargetVelocity(this->v, this->l, this->w, 1.0);
        return 1;
    } catch ( AL::ALError& e ) {
        return 0;
    }
}

Ice::Float NaoServerMotors::getW ( const Ice::Current& ) {
    std::vector<float> result = this->motion->getRobotVelocity();
    return result[2];
}

Ice::Int NaoServerMotors::setW ( float w, const Ice::Current& ) {
    try {
        this->w = w;
        this->motion->setWalkTargetVelocity(this->v, this->l, this->w, 1.0);
        return 1;
    } catch ( AL::ALError& e ) {
        return 0;
    }
}

Ice::Float NaoServerMotors::getL ( const Ice::Current& ) {
    std::vector<float> result = this->motion->getRobotVelocity();
    return result[1];
}

Ice::Int NaoServerMotors::setL ( float l, const Ice::Current& ) {
    try {
        this->l = l;
        this->motion->setWalkTargetVelocity(this->v, this->l, this->w, 1.0);
        return 1;
    } catch ( AL::ALError& e ) {
        return 0;
    }
}
