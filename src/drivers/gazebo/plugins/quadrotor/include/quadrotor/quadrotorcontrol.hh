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

#ifndef QUADROTORCONTROL_H
#define QUADROTORCONTROL_H


#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>

#include <quadrotor/control/twist.hh>
#include <quadrotor/control/pidcontroller.hh>

#include <quadrotor/quadrotorsensors.hh>


namespace quadrotor{

enum QuadrotorState{Unknown, Flying, Landed, TakingOff, Landing};
extern const std::string QuadrotorStateDescription[];

class QuadrotorControl {
public:
    QuadrotorControl();
    virtual ~QuadrotorControl();
    void setSensors(QuadRotorSensors *sensors);

/// Control commands
public:
    void takeoff();
    void land();
    void setTargetVelocity(gazebo::math::Twist twist);
    void teleport(gazebo::math::Pose pose);

    std::string _log_prefix;

/// Control
protected:
    void _update_state(const gazebo::common::UpdateInfo & _info);
    void _control_loop_novel(const gazebo::common::UpdateInfo & _info);
    void _control_loop_hector(const gazebo::common::UpdateInfo & _info);

    double mass;
    QuadrotorState my_state;
    Controllers controllers;
    gazebo::common::Time last_simTime;
    gazebo::math::Twist velocity_command;

    std::pair<double,double> fly_state_thresholds;
    double takeoff_speed;
    double land_speed;

    QuadRotorSensors *sensors;


/// Gazebo
public:
    void Load(gazebo::physics::LinkPtr _base_link, sdf::ElementPtr _sdf);
    void Init(QuadRotorSensors* sensors);
    void OnUpdate(const gazebo::common::UpdateInfo & _info);


private:
    gazebo::event::ConnectionPtr updateConnection;
    gazebo::physics::LinkPtr base_link;
    gazebo::physics::InertialPtr inertial;

};


typedef boost::shared_ptr<QuadrotorControl> QuadrotorControlPtr;

}//NS

#endif // QUADROTORPLUGIN_H
