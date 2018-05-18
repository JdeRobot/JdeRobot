//=================================================================================================
// Copyright (c) 2012, Johannes Meyer, TU Darmstadt
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Flight Systems and Automatic Control group,
//       TU Darmstadt, nor the names of its contributors may be used to
//       endorse or promote products derived from this software without
//       specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=================================================================================================

/*
* quadrotor motion controller:
*
* This software is a fragment of original source
*
* Created on: Oct 22, 2012
* Author: Hongrong huang
*
* Adapted on: Nov 15, 2015
* Author: Victor Arribas
*
*/

#include "quadrotor/quadrotorcontrol.hh"

/// Constant velocity landing to avoid previous fall down behavior.
#define VELOCITY_BASED_TAKEOFF

using namespace quadrotor;
using namespace gazebo::math;
using namespace gazebo::physics;
using namespace gazebo::common;


void
QuadrotorControl::_control_loop_hector(const gazebo::common::UpdateInfo & _info){
    double dt = (_info.simTime - last_simTime).Double();
    last_simTime = _info.simTime;
    if (dt == 0.0) return;

    // Max force (vel<=10m/s)
    //double max_force_ = mass*10/dt;

    // Get kinematics
    Pose pose = base_link->GetWorldPose();
    Vector3 euler = pose.rot.GetAsEuler();
    Vector3 linear_velocity = base_link->GetWorldLinearVel();
    Vector3 angular_velocity = base_link->GetWorldAngularVel();
    Vector3 acceleration = base_link->GetWorldLinearAccel();
    Vector3 inertia = inertia = inertial->GetPrincipalMoments();

    double velocity_command_linear_z = velocity_command.linear.z;
#ifdef VELOCITY_BASED_TAKEOFF
    // Inject landing/takingoff
    if (my_state == QuadrotorState::Landing)
        velocity_command_linear_z = std::min(velocity_command.linear.z, -land_speed);
    if (my_state == QuadrotorState::TakingOff)
        velocity_command_linear_z = std::max(velocity_command.linear.z, +takeoff_speed);
#endif

    // Get gravity
    Vector3 gravity = base_link->GetWorld()->GetPhysicsEngine()->GetGravity();
    Vector3 gravity_body = pose.rot.RotateVector(gravity);
    double gravity_module = gravity_body.GetLength();
    double load_factor = gravity_module * gravity_module / gravity.Dot(gravity_body);  // Get gravity

    // Rotate vectors to coordinate frames relevant for control
    Quaternion heading_quaternion(cos(euler.z/2),0,0,sin(euler.z/2));
    Vector3 velocity_xy = heading_quaternion.RotateVectorReverse(linear_velocity);
    Vector3 acceleration_xy = heading_quaternion.RotateVectorReverse(acceleration);
    Vector3 angular_velocity_body = pose.rot.RotateVectorReverse(angular_velocity);

    // update controllers
    Vector3 force(Vector3::Zero), torque(Vector3::Zero);

    double pitch_command =  controllers.velocity_x.update(velocity_command.linear.x, velocity_xy.x, acceleration_xy.x, dt) / gravity_module;
    double roll_command  = -controllers.velocity_y.update(velocity_command.linear.y, velocity_xy.y, acceleration_xy.y, dt) / gravity_module;
    torque.x = inertia.x *  controllers.roll.update(roll_command, euler.x, angular_velocity_body.x, dt);
    torque.y = inertia.y *  controllers.pitch.update(pitch_command, euler.y, angular_velocity_body.y, dt);
    torque.z = inertia.z *  controllers.yaw.update(velocity_command.angular.z, angular_velocity.z, 0, dt);
    force.z  = mass      * (controllers.velocity_z.update(velocity_command_linear_z,  linear_velocity.z, acceleration.z, dt) + load_factor * gravity_module);
    //if (max_force_ > 0.0 && force.z > max_force_) force.z = max_force_;
    if (force.z < 0.0) force.z = 0.0;

    if (my_state != QuadrotorState::Landed){
        base_link->AddRelativeForce(force);
        base_link->AddRelativeTorque(torque);
    }

#ifndef VELOCITY_BASED_TAKEOFF
    if (my_state == QuadrotorState::Landing){
        base_link->AddRelativeForce(-0.5*force);
        base_link->AddRelativeTorque(-0.5*torque);
    }

    if (my_state == QuadrotorState::TakingOff){
        base_link->AddRelativeForce(0.5*force);
        base_link->AddRelativeTorque(0.5*torque);
    }
#endif
}

