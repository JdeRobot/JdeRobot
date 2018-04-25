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
 *  CLONE of https://github.com/jderobot-varribas/gazeboplugin-quadrotor2/blob/2.1.0/include/quadrotor/interfaces/pose3di.h
 *  Victor Arribas Raigadas <v.arribas.urjc@gmai.com>
 */


#ifndef POSE3D_H
#define POSE3D_H


#include <jderobot/pose3d.h>
#include <turtlebot/turtlebotsensors.hh>
#include <turtlebot/turtlebotcontrol.hh>


namespace turtlebot{
namespace interfaces{

class Pose3DI : public jderobot::Pose3D {
public:
    Pose3DI (const turtlebot::TurtlebotSensors *sensor, turtlebot::TurtlebotControl *control);
    virtual ~Pose3DI ();

    virtual
    jderobot::Pose3DDataPtr getPose3DData ( const Ice::Current& );
    Ice::Int setPose3DData ( const jderobot::Pose3DDataPtr & data,
                                     const Ice::Current& );


protected:
    jderobot::Pose3DDataPtr data;
    const turtlebot::TurtlebotSensors* const sensor;
    turtlebot::TurtlebotControl* const control;
};

}}//NS
#endif // POSE3DI_H
