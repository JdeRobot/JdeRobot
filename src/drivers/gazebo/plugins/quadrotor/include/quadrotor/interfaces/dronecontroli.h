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


#ifndef DRONECONTROLI_H
#define DRONECONTROLI_H


#include <jderobot/ardroneextra.h>
#include <quadrotor/quadrotorcontrol.hh>
#include <quadrotor/cameraproxy.hh>


namespace quadrotor{
namespace interfaces{

class DroneControlI : public jderobot::ArDroneExtra {
public:
    DroneControlI (quadrotor::QuadrotorControl *control, quadrotor::CameraProxy *camproxy);
    virtual ~DroneControlI ();

    void land(const Ice::Current& /*c*/);
    void takeoff(const Ice::Current& /*c*/);
    void toggleCam(const Ice::Current& /*c*/);

    void reset(const Ice::Current& ){}
    void recordOnUsb(bool, const Ice::Current& ){}
    void ledAnimation(Ice::Int, Ice::Float, Ice::Float, const Ice::Current& ){}
    void flightAnimation(Ice::Int, Ice::Float, const Ice::Current& ){}
    void flatTrim(const Ice::Current& ){}

private:
    quadrotor::QuadrotorControl* const control;
    quadrotor::CameraProxy* const camproxy;
};


}}//NS
#endif // POSE3DI_H
