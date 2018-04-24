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

#ifndef QUADROTORPLUGIN_H
#define QUADROTORPLUGIN_H

#include <boost/bind.hpp>

#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/math/Pose.hh>

#include <quadrotor/quadrotorsensors.hh>
#include <quadrotor/quadrotorcontrol.hh>
#include <quadrotor/quadrotorice.hh>
#include <quadrotor/cameraproxy.hh>

#include <quadrotor/debugtools.h>


namespace quadrotor{

class QuadrotorPlugin : public gazebo::ModelPlugin {
public:
    QuadrotorPlugin();
    ~QuadrotorPlugin();

    std::string _log_prefix;

private:
    QuadRotorSensors sensors;
    QuadrotorControl control;
    CameraProxy cameraproxy;


/// Gazebo
protected:
    void Load(gazebo::physics::ModelPtr _model, sdf::ElementPtr _sdf);
    void Init();
    void OnUpdate(const gazebo::common::UpdateInfo & _info);
    void Reset();
    void OnSigInt();

private:
    gazebo::physics::ModelPtr model;
    gazebo::event::ConnectionPtr updateConnection;
    gazebo::event::ConnectionPtr sigintConnection;


/// Ice
protected:
    void InitializeIce(sdf::ElementPtr _sdf);

private:
    QuadrotorIcePtr icePlugin;

};

}//NS

#endif // QUADROTORPLUGIN_H
