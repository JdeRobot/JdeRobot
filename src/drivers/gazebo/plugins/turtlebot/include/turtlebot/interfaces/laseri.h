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

#ifndef LASER_H
#define LASER_H


#include <jderobot/laser.h>
#include <turtlebot/turtlebotsensors.hh>
#include <physics/MultiRayShape.hh>

namespace turtlebot{
namespace interfaces{

class LaserI : public jderobot::Laser {
public:
    LaserI (const turtlebot::TurtlebotSensors *sensor);
    virtual ~LaserI();

    virtual jderobot::LaserDataPtr getLaserData ( const Ice::Current& );

protected:
    jderobot::LaserDataPtr laserdata;
    const turtlebot::TurtlebotSensors* const sensor;
};

}}//NS
#endif // LASER_H
