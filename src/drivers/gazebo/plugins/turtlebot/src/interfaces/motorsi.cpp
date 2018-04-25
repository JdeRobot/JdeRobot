/*
 *  Copyright (C) 1997-2017 JDE Developers Team
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
 *  Authors :
 *       Francisco Perez Salgado <f.pererz475@gmai.com>
 */

#include "turtlebot/interfaces/motorsi.h"

using namespace turtlebot::interfaces;
using namespace jderobot;

MotorsI::MotorsI(turtlebot::TurtlebotControl* const control):
    control(control)
{}

MotorsI::~MotorsI() {}

float
MotorsI::getV(const Ice::Current&) {

    return control->robotMotors.v;
}

float
MotorsI::getW(const Ice::Current&) {

    return control->robotMotors.w;
}

float
MotorsI::getL(const Ice::Current&) {
    return 0.;
}

Ice::Int
MotorsI::setV(Ice::Float v, const Ice::Current&) {

    control->robotMotors.v = v;
    return 0;
}

Ice::Int
MotorsI::setW(Ice::Float _w, const Ice::Current&) {

    control->robotMotors.w = -_w;
    return 0;
}

Ice::Int
MotorsI::setL(Ice::Float /*l*/, const Ice::Current&) {
    return 0;
}
