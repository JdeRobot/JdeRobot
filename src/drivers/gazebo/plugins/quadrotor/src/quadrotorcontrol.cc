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



#include "quadrotor/quadrotorcontrol.hh"

const std::string quadrotor::QuadrotorStateDescription[] = {"Unknown", "Flying", "Landed", "TakingOff", "Landing"};


using namespace quadrotor;
using namespace gazebo::math;
using namespace gazebo::physics;
using namespace gazebo::common;


QuadrotorControl::QuadrotorControl():
    my_state(QuadrotorState::Unknown)
{}

QuadrotorControl::~QuadrotorControl(){

}

template <typename T> T sdf2value(const sdf::ElementPtr &_sdf, std::string _str){ return _sdf->GetElement(_str)->Get<T>(); }
template <typename T> T sdf2value(const sdf::ElementPtr &_sdf, std::string _str, T _default){ if (_sdf->HasElement(_str)) return sdf2value<T>(_sdf, _str); else return _default; }

void
QuadrotorControl::Load(LinkPtr _base_link, sdf::ElementPtr _sdf){
    base_link = _base_link;
    inertial = _base_link->GetInertial();
    mass = inertial->GetMass();

    fly_state_thresholds.first = sdf2value<double>(_sdf, "landCompletedAt", 0.2);
    fly_state_thresholds.second = sdf2value<double>(_sdf, "takeoffCompletedAt", 1.5);
    takeoff_speed = sdf2value<double>(_sdf, "takeoffSpeed", 3);
    land_speed = sdf2value<double>(_sdf, "landSpeed", 1);

    controllers.roll.Load(_sdf, "rollpitch");
    controllers.pitch.Load(_sdf, "rollpitch");
    controllers.yaw.Load(_sdf, "yaw");
    controllers.velocity_x.Load(_sdf, "velocityXY");
    controllers.velocity_y.Load(_sdf, "velocityXY");
    controllers.velocity_z.Load(_sdf, "velocityZ");
}

void
QuadrotorControl::Init(QuadRotorSensors *sensors){
    this->sensors = sensors;

    updateConnection = gazebo::event::Events::ConnectWorldUpdateBegin(
        boost::bind(&QuadrotorControl::OnUpdate, this, _1));
}

void
QuadrotorControl::OnUpdate(const gazebo::common::UpdateInfo & _info){
    _update_state(_info);
#if 1
    _control_loop_hector(_info);
#else
    _control_loop_novel(_info);
#endif
}


void
QuadrotorControl::takeoff(){
    if (my_state == QuadrotorState::Landed || my_state == QuadrotorState::Landing){
        my_state = QuadrotorState::TakingOff;
        std::cout<<_log_prefix << "QuadrotorState::TakingOff" << std::endl;
    }
}

void
QuadrotorControl::land(){
    if (my_state == QuadrotorState::Flying || my_state == QuadrotorState::TakingOff){
        my_state = QuadrotorState::Landing;
        std::cout<<_log_prefix << "QuadrotorState::Landing" << std::endl;
    }
}

void
QuadrotorControl::setTargetVelocity(Twist twist){
    velocity_command = twist;
}

void
QuadrotorControl::teleport(gazebo::math::Pose pose){
    base_link->GetModel()->SetWorldPose(pose);
}


void
QuadrotorControl::_update_state(const gazebo::common::UpdateInfo & /*_info*/){
    //Pose pose = base_link->GetWorldPose();
    double altitude = sensors->altitude;
    /// **altitude** is a big deal due sensor limitations.
    /// "real" sensor has a very tiny range (3 meters). It should
    /// be merged with barometer to fit a real best hardware case.
    /// otherways there is impossible to merge absolute obstacle aware
    /// position with real altotude feedbak.

    switch(my_state){
    case Unknown:
        if (altitude >= /*model.height?*/ fly_state_thresholds.second)
            my_state = QuadrotorState::Flying;
        else
            my_state = QuadrotorState::Landed;
        std::cout<<_log_prefix << boost::format("\tboostrap quadrotor state as %1% (altitude=%2%)") % QuadrotorStateDescription[my_state] % altitude <<std::endl;
    break;
    case Landing:
        if (altitude < fly_state_thresholds.first){
            my_state = QuadrotorState::Landed;
            std::cout<<_log_prefix << "QuadrotorState::Landed" << std::endl;
        }
    break;
    case TakingOff:
        if (altitude >= fly_state_thresholds.second){
            my_state = QuadrotorState::Flying;
            std::cout<<_log_prefix << "QuadrotorState::Flying" << std::endl;
        }
    break;
    default: break;
    }
}


void
QuadrotorControl::_control_loop_novel(const gazebo::common::UpdateInfo & /*_info*/){
    //// Forces and velocities are handled bt a physics engine
    /// This means that we must apply a counter-gravity force
    /// each cicle to model "fly" state.
    /// ToDo[enhancement]: apply battery status to counter-gravity,
    /// so when battery is drained, force tends to zero and
    /// quadrotor will fall.

    // getting gravity dynamically to allow on-demand changes.
    Vector3 gravity = base_link->GetWorld()->GetPhysicsEngine()->GetGravity();
    Vector3 counter_gravity = mass*-gravity;
    //base_link->AddForce(counter_gravity);

    Pose pose = base_link->GetWorldPose();

    Vector3 vel_model = base_link->GetRelativeLinearVel();
    Vector3 vel_world = base_link->GetWorldLinearVel(); //pose.rot.RotateVectorReverse(vel_model);
    Vector3 up_down_vel = Vector3(0,0,0.001);


    switch(my_state){
    case Landing:
        vel_model = vel_model-up_down_vel;// = pose.rot.RotateVector(vel_world-up_down_vel);
        base_link->SetLinearVel(vel_model);
    break;
    case TakingOff:
        vel_model = vel_model+up_down_vel;// pose.rot.RotateVector(vel_world+up_down_vel);
        base_link->SetLinearVel(vel_model);
    break;
    default: break;
    }

    if (my_state != QuadrotorState::Landed){
        base_link->AddForce(counter_gravity);
    }


#if 0 //Testing
    switch(my_state){
    case Landed: takeoff() break;
    case Flying: land() break;
    }
#endif
}

