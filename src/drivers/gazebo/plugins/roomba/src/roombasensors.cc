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


#include "roomba/roombasensors.hh"


using namespace roomba;
using namespace gazebo::physics;
using namespace gazebo::sensors;

RoombaSensors::RoombaSensors(){
    ONDEBUG_INFO(std::cout << _log_prefix << "RoombaSensors::RoombaSensors()" << std::endl;)
}

RoombaSensors::~RoombaSensors(){
    ONDEBUG_INFO(std::cout << _log_prefix << "RoombaSensors::~RoombaSensors()" << std::endl;)
}

void
RoombaSensors::Load(ModelPtr model){
    this->model = model;
    this->base_link_id = model->GetChildLink("base")->GetId();

    SensorManager *sm = SensorManager::Instance();
    
    for (SensorPtr s: sm->GetSensors()){
        std::cout << s->Name() << std::endl;
//        if (s->GetParentId() != base_link_id) continue;
        std::string name = s->Name();
        if (name.find("laser") != std::string::npos)
            laser = std::static_pointer_cast<RaySensor>(s);
        if (name.find("bumper") != std::string::npos){
            bumper = std::static_pointer_cast<ContactSensor>(s);
        }

        pose = model->GetWorldPose();
    }
}


void
RoombaSensors::Init(){
    
    if (laser){
        sub_laser = laser->ConnectUpdated(
            boost::bind(&RoombaSensors::_on_laser, this));
    }else
        std::cerr << _log_prefix << "\t laser was not connected (NULL pointer)" << std::endl;


    if (bumper){
        sub_bumper = bumper->ConnectUpdated(
            boost::bind(&RoombaSensors::_on_bumper, this));
        bumper->SetActive(true);
    }else
        std::cerr << _log_prefix << "\t bumper was not connected (NULL pointer)" << std::endl;

    ONDEBUG_INFO(std::cout << _log_prefix << "Initial Pose [x,y,z] " << pose.pos.x << ", " << pose.pos.y << ", " << pose.pos.z << std::endl;)

    //Pose3d
    updatePose = gazebo::event::Events::ConnectWorldUpdateBegin(
                boost::bind(&RoombaSensors::OnUpdate, this, _1));
}

void
RoombaSensors::debugInfo(){
    std::cout << _log_prefix << "Sensors of " << model->GetName() << std::endl;
    boost::format fmt(_log_prefix+"\t%1% (id: %2%)\n");

    std::cout << fmt % laser->Name() % laser->Id();
    std::cout << fmt % bumper->Name() % bumper->Id();
}

void
RoombaSensors::_on_laser(){
    LaserD data;

    //laser values
    MultiRayShapePtr laserV = this->laser->LaserShape();

    std::vector<float> laserValues(laserV->GetSampleCount ());
    for (int i = 0; i< laserV->GetSampleCount (); i++){
        laserValues[i] = laserV->GetRange(i);
    }

    data.values = laserValues;
    //This values don't work well
    //data.minAngle = this->laser->AngleMin().Radian();
    //data.maxAngle = this->laser->AngleMax().Radian();
    //data.minRange = this->laser->RangeMin();
    //data.maxAngle = this->laser->RangeMax();

    laserData = data;

}

void
RoombaSensors::_on_bumper(){

    BumperD data;

    //bumper values
    std::map<std::string, gazebo::physics::Contact> contacts;
    contacts = bumper->Contacts("roomba::base::base_collision");

    /*for (auto const& element : contacts) {
        std::cout << element.first << std::endl;
        std::cout << element.second.count << std::endl;
    }*/

    if (contacts.size() > 0) {
        data.bumper = 1;
        data.state = 1;
    }else
        data.state = 0;

    bumperData = data;
}

void
RoombaSensors::OnUpdate(const gazebo::common::UpdateInfo & /*_info*/){
    pose = model->GetWorldPose();
}
