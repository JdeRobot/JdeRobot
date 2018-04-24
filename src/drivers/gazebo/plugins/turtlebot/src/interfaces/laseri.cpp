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
 *       Aitor Martinez Fernandez <aitor.martinez.fernandez@gmail.com>
 */

#include "turtlebot/interfaces/laseri.h"

using namespace turtlebot::interfaces;
using namespace jderobot;


LaserI::LaserI (const TurtlebotSensors *sensor):
    laserdata(new LaserData()),
    sensor(sensor)
{}

LaserI::~LaserI ()
{}

LaserDataPtr
LaserI::getLaserData ( const Ice::Current& ){

    laserdata->numLaser = sensor->laserData.values.size();
    laserdata->distanceData.resize(sizeof(int)*laserdata->numLaser);
    laserdata->minAngle = sensor->laserData.minAngle;
    laserdata->maxAngle = sensor->laserData.maxAngle;
    laserdata->minRange = sensor->laserData.minRange;
    laserdata->maxRange = sensor->laserData.maxRange*1000;


    //Update laser values
    for(int i = 0 ; i < laserdata->numLaser; i++){
       laserdata->distanceData[i] = sensor->laserData.values[i]*1000;
    }
    return laserdata;
}







