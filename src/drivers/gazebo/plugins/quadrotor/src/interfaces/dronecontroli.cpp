/*
 *  Copyright (C) 1997-2015 JDE Developers Team
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
 *       Victor Arribas Raigadas <v.arribas.urjc@gmai.com>
 */


#include "quadrotor/interfaces/dronecontroli.h"


using namespace quadrotor::interfaces;
using namespace jderobot;


DroneControlI::DroneControlI (QuadrotorControl *control, CameraProxy *camproxy):
    control(control),
    camproxy(camproxy)
{}

DroneControlI::~DroneControlI ()
{}



void
DroneControlI::land(const Ice::Current& ){
    ONDEBUG_VERBOSE(std::cout << "send order: Land" <<std::endl;)
    control->land();
}



void
DroneControlI::takeoff(const Ice::Current& ){
    ONDEBUG_VERBOSE(std::cout << "send order: TakeOff" <<std::endl;)
    control->takeoff();
}


void
DroneControlI::toggleCam(const Ice::Current& ){
    camproxy->next();
    ONDEBUG_VERBOSE(std::cout << "toggled camera to "<<camproxy->getActive() <<std::endl;)
}
