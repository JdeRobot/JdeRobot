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

#include "NaoServerMotions.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
NaoServerMotions::NaoServerMotions () {}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
NaoServerMotions::~NaoServerMotions () {}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void NaoServerMotions::init ( const std::string newName, AL::ALPtr<AL::ALBroker> parentBroker ) {
    this->name = newName;
    
    try {
        this->motion = parentBroker->getMotionProxy();
    } catch ( AL::ALError& e ) {
		std::cerr << "[NaoServerMotions ()::init(): " << e.toString() << std::endl;
	}
}

/*************************************************************
 * NAOMOTIONS
 *************************************************************/
Ice::Int NaoServerMotions::setMotion ( jderobot::MotionType motion, const Ice::Current& ) {
    MotionEnum myMotion = (MotionEnum) motion;
    int returned = 0;
    switch (myMotion) {
        case RIGHTKICK:
            returned = this->forwardKickRight();
            break;
        case LEFTKICK:
            returned = this->forwardKickLeft();
            break;
        case STANDUP_BACK:
            returned = this->standupBack();
            break;
        case STANDUP_FRONT:
            returned = this->standupFront();
            break;
        case INTERCEPT:
            returned = this->intercept();
            break;
        case RESETNAOQI:
            returned = this->resetNaoqi();
            break;
        default:
            std::cout << "Default..." << std::endl;
            break;
    }
    
    return returned;
}

int NaoServerMotions::forwardKickRight () {
    AL::ALValue names, times, keys;
    
    #include "motions/forwardStrongRight.txt"
    
    try {
        this->motion->angleInterpolationBezier(names, times, keys);
        return 1;
    } catch ( AL::ALError& e ) {
        std::cerr << "Error forward kicking right! " << std::endl;
        return 0;
    }
}

int NaoServerMotions::forwardKickLeft () {
    AL::ALValue names, times, keys;
    
    #include "motions/forwardStrongLeft.txt"
    
    try {
        this->motion->angleInterpolationBezier(names, times, keys);
        return 1;
    } catch ( AL::ALError& e ) {
        std::cerr << "Error forward kicking left! " << std::endl;
        return 0;
    }
}

int NaoServerMotions::standupBack () {
    AL::ALValue names, times, keys;
    
    #include "motions/StandUpBack.txt"
    
    try {
        this->motion->setStiffnesses("Body", 1.0f);
        this->motion->angleInterpolationBezier(names, times, keys);
        return 1;
    } catch ( AL::ALError& e ) {
        std::cerr << "Error forward standing up back! " << std::endl;
        return 0;
    }
}

int NaoServerMotions::standupFront () {
    AL::ALValue names, times, keys;
    
    #include "motions/StandUpFront.txt"
    
    try {
        this->motion->setStiffnesses("Body", 1.0f);
        this->motion->angleInterpolationBezier(names, times, keys);
        return 1;
    } catch ( AL::ALError& e ) {
        std::cerr << "Error forward standing up front! " << std::endl;
        return 0;
    }
}

int NaoServerMotions::intercept () {
    AL::ALValue names, times, keys;
    
    #include "motions/suicide.txt"
    
    try {
        this->motion->angleInterpolationBezier(names, times, keys);
        return 1;
    } catch ( AL::ALError& e ) {
        std::cerr << "Error forward intercepting! " << std::endl;
        return 0;
    }
}

int NaoServerMotions::resetNaoqi () {    
    std::cerr << "Reseting NaoQi..." << std::endl;
    return 1;
}
