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


#ifndef POSE3D_H
#define POSE3D_H


#include <jderobot/pose3d.h>
#include <roomba/roombasensors.hh>
#include <roomba/roombacontrol.hh>


namespace roomba{
namespace interfaces{

class Pose3DI : public jderobot::Pose3D {
public:
    Pose3DI (const roomba::RoombaSensors *sensor, roomba::RoombaControl *control);
    virtual ~Pose3DI ();

    virtual
    jderobot::Pose3DDataPtr getPose3DData ( const Ice::Current& );
    Ice::Int setPose3DData ( const jderobot::Pose3DDataPtr & data,
                                     const Ice::Current& );


protected:
    jderobot::Pose3DDataPtr data;
    const roomba::RoombaSensors* const sensor;
    roomba::RoombaControl* const control;
};

}}//NS
#endif // POSE3DI_H
