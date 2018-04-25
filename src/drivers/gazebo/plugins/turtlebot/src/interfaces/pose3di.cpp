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
 *
 *  CLONE of https://github.com/jderobot-varribas/gazeboplugin-quadrotor2/blob/2.1.0/src/interfaces/pose3di.cpp
 *  Victor Arribas Raigadas <v.arribas.urjc@gmai.com>
 */


#include "turtlebot/interfaces/pose3di.h"

using namespace turtlebot::interfaces;
using namespace jderobot;

Pose3DI::Pose3DI (const turtlebot::TurtlebotSensors *sensor, turtlebot::TurtlebotControl *control):
    data(new Pose3DData(0,0,0,0,0,0,0,0)),
    sensor(sensor),
    control(control)
{}

Pose3DI::~Pose3DI ()
{}

Pose3DDataPtr
Pose3DI::getPose3DData ( const Ice::Current& ){
    gazebo::math::Pose pose = sensor->pose;

    data->x = pose.pos.x;
    data->y = pose.pos.y;
    data->z = pose.pos.z;
    data->h = 1;
    data->q0 = pose.rot.w;
    data->q1 = pose.rot.x;
    data->q2 = pose.rot.y;
    data->q3 = pose.rot.z;

    return data;
}

Ice::Int
Pose3DI::setPose3DData ( const jderobot::Pose3DDataPtr & data, const Ice::Current& ){
    gazebo::math::Pose pose(gazebo::math::Vector3(data->x, data->y, data->z),
                            gazebo::math::Quaternion(data->q0, data->q1, data->q2, data->q3));
    control->teleport(pose);
    return 0;
}
