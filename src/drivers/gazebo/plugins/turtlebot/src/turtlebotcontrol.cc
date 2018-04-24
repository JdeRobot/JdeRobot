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
 *  REMIX of https://github.com/jderobot-varribas/gazeboplugin-quadrotor2/blob/2.1.0/src/quadrotorcontrol.cc
 *  Victor Arribas Raigadas <v.arribas.urjc@gmai.com>
 *  
 *  Authors:
 *       Francisco Perez Salgado <f.pererz475@gmai.com>
 */



#include "turtlebot/turtlebotcontrol.hh"

using namespace turtlebot;
using namespace gazebo::math;
using namespace gazebo::physics;
using namespace gazebo::common;
using namespace gazebo::transport;

enum {
    RIGHT,
    LEFT
};

TurtlebotControl::TurtlebotControl(){

}

TurtlebotControl::~TurtlebotControl(){}

void
TurtlebotControl::Load(ModelPtr model, sdf::ElementPtr _sdf){
    // Get a pointer to the model
    this->model = model;

    this->node = NodePtr(new Node());
    this->node->Init(this->model->GetWorld()->GetName());

    //this->velSub = this->node->Subscribe(std::string("~/") + this->model->GetName() + "/vel_cmd", &Motors::OnVelMsg, this);
    if (!_sdf->HasElement("left_joint"))
        gzerr << "Motors plugin missing <left_joint> element\n";
    if (!_sdf->HasElement("right_joint"))
        gzerr << "DiffDrive plugin missing <right_joint> element\n";

    this->leftJoint = model->GetJoint(
            _sdf->GetElement("left_joint")->Get<std::string>());
    this->rightJoint = model->GetJoint(
            _sdf->GetElement("right_joint")->Get<std::string>());

    if (_sdf->HasElement("torque"))
        this->torque = _sdf->GetElement("torque")->Get<double>();
    else {
        gzwarn << "No torque value set for the DiffDrive plugin.\n";
        this->torque = 5.0;
    }
    if (!this->leftJoint)
        gzerr << "Unable to find left joint["
            << _sdf->GetElement("left_joint")->Get<std::string>() << "]\n";
    if (!this->rightJoint)
        gzerr << "Unable to find right joint["
            << _sdf->GetElement("right_joint")->Get<std::string>() << "]\n";
}

void
TurtlebotControl::Init(TurtlebotSensors *sensors){
    this->sensors = sensors;

    this->wheelSeparation = this->leftJoint->GetAnchor(0).Distance(this->rightJoint->GetAnchor(0));
    EntityPtr parent = boost::dynamic_pointer_cast<Entity > (this->leftJoint->GetChild());

    Box bb = parent->GetBoundingBox();

    this->wheelRadius = bb.GetSize().GetMax() * 0.5;

    updateConnection = gazebo::event::Events::ConnectWorldUpdateBegin(
        boost::bind(&TurtlebotControl::OnUpdate, this, _1));
}

void
TurtlebotControl::OnUpdate(const gazebo::common::UpdateInfo & /*_info*/){
    double vr, va; //vr -> velocidad lineal; va -> velocidad angular

    vr = robotMotors.v;
    va = robotMotors.w;

    this->wheelSpeed[LEFT] = vr + va * this->wheelSeparation / 2.0;
    this->wheelSpeed[RIGHT] = vr - va * this->wheelSeparation / 2.0;

    double leftVelDesired = (this->wheelSpeed[LEFT] / this->wheelRadius);
    double rightVelDesired = (this->wheelSpeed[RIGHT] / this->wheelRadius);
/*
    std::cout << "leftVelDesired " << leftVelDesired << std::endl;
    std::cout << "rightVelDesired " << rightVelDesired << std::endl;
    std::cout << "torque " << torque << std::endl;
*/
    this->leftJoint->SetParam("vel", 0, leftVelDesired);
    this->rightJoint->SetParam("vel", 0, rightVelDesired);

    this->leftJoint->SetParam("fmax", 0, this->torque);
    this->rightJoint->SetParam("fmax", 0, this->torque);
}

void
TurtlebotControl::teleport(gazebo::math::Pose pose){
    this->model->SetWorldPose(pose);
}
