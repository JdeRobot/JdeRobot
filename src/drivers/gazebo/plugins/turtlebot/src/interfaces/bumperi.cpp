/*
 *  Copyright (C) 1997-2017 JDeRobot Developers Team
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

#include "turtlebot/interfaces/bumperi.h"

using namespace turtlebot::interfaces;
using namespace jderobot;


BumperI::BumperI (const TurtlebotSensors *sensor):
    bumperdata(new BumperData()),
    sensor(sensor)
{}

BumperI::~BumperI ()
{}

BumperDataPtr
BumperI::getBumperData ( const Ice::Current& ){

    bumperdata->bumper = sensor->bumperData.bumper;
    bumperdata->state = sensor->bumperData.state;

    return bumperdata;
}







