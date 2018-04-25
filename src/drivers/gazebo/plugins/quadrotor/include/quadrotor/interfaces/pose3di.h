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


#ifndef POSE3DI_H
#define POSE3DI_H


#include <jderobot/pose3d.h>
#include <quadrotor/quadrotorsensors.hh>
#include <quadrotor/quadrotorcontrol.hh>


namespace quadrotor{
namespace interfaces{

class Pose3DI : public jderobot::Pose3D {
public:
    Pose3DI (const quadrotor::QuadRotorSensors *sensor, quadrotor::QuadrotorControl *control);
    virtual ~Pose3DI ();

    virtual
    jderobot::Pose3DDataPtr getPose3DData ( const Ice::Current& );
    Ice::Int setPose3DData ( const jderobot::Pose3DDataPtr & data,
                                     const Ice::Current& );


protected:
    jderobot::Pose3DDataPtr data;
    const quadrotor::QuadRotorSensors* const sensor;
    quadrotor::QuadrotorControl* const control;
};

class Pose3DI_altitude : public Pose3DI{
public:
    Pose3DI_altitude (const quadrotor::QuadRotorSensors *sensor, quadrotor::QuadrotorControl *control): Pose3DI(sensor, control){}
    jderobot::Pose3DDataPtr getPose3DData ( const Ice::Current& );
};


}}//NS
#endif // POSE3DI_H
