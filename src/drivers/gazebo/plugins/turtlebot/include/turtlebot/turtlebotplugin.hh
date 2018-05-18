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
 *
 *  REMIX of https://github.com/jderobot-varribas/gazeboplugin-quadrotor2/blob/2.1.0/include/quadrotor/quadrotorplugin.hh
 *  Victor Arribas Raigadas <v.arribas.urjc@gmai.com>
 *  
 *  Authors:
 *       Francisco Perez Salgado <f.pererz475@gmai.com>
 */

#ifndef TURTLEBOTPLUGIN_H
#define TURTLEBOTPLUGIN_H

#include <boost/bind.hpp>

#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/math/Pose.hh>

#include <turtlebot/turtlebotsensors.hh>
#include <turtlebot/turtlebotcontrol.hh>
#include <turtlebot/turtlebotice.hh>

#include <turtlebot/debugtools.h>

namespace turtlebot{

class TurtlebotPlugin : public gazebo::ModelPlugin {
public:
    TurtlebotPlugin();
    ~TurtlebotPlugin();

    std::string _log_prefix;

private:
    TurtlebotSensors sensors;
    TurtlebotControl control;


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
    TurtlebotIcePtr icePlugin;

};

}//NS

#endif // TurtlebotPlugin_H
