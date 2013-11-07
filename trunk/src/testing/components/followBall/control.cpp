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

#include "control.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
Control::Control ( Ice::CommunicatorPtr ic ) {
    this->ic = ic;
    Ice::PropertiesPtr prop = this->ic->getProperties();
    
    Ice::ObjectPrx base = ic->propertyToProxy("FollowBall.MotorsHeadSpeed.Proxy");
    if (base == 0)
        throw "Could not create proxy with Motors";
    
    this->motorsprx = jderobot::Pose3DMotorsPrx::checkedCast(base);
    if (this->motorsprx == 0)
        throw "Invalid Motors proxy";
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
Control::~Control () {
    if (this->ic)
        this->ic->destroy();
}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void Control::sendValues ( float panSpeed, float tiltSpeed ) {
    jderobot::Pose3DMotorsDataPtr pose(new jderobot::Pose3DMotorsData);
    pose->panSpeed = panSpeed + 0.00001;
    pose->tiltSpeed = tiltSpeed + 0.00001;
    this->motorsprx->setPose3DMotorsData(pose);
}
