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
 *  REMIX of https://github.com/jderobot-varribas/gazeboplugin-quadrotor2/blob/2.1.0/src/quadrotorsensors.cc
 *  Victor Arribas Raigadas <v.arribas.urjc@gmai.com>
 *  
 *  Authors:
 *       Francisco Perez Salgado <f.pererz475@gmai.com>
 */


#include "turtlebot/turtlebotsensors.hh"


using namespace turtlebot;
using namespace gazebo::physics;
using namespace gazebo::sensors;

TurtlebotSensors::TurtlebotSensors(){
    ONDEBUG_INFO(std::cout << _log_prefix << "TurtlebotSensors::TurtlebotSensors()" << std::endl;)
}

TurtlebotSensors::~TurtlebotSensors(){
    ONDEBUG_INFO(std::cout << _log_prefix << "TurtlebotSensors::~TurtlebotSensors()" << std::endl;)
}

void
TurtlebotSensors::Load(ModelPtr model){
    this->model = model;
    this->base_link_id = model->GetChildLink("rack")->GetId();

    SensorManager *sm = SensorManager::Instance();
    
    for (SensorPtr s: sm->GetSensors()){
        std::cout << s->Name() << std::endl;
//        if (s->GetParentId() != base_link_id) continue;
        std::string name = s->Name();
        if (name.find("laser") != std::string::npos)
            laser = std::static_pointer_cast<RaySensor>(s);
        if (name.find("cam_turtlebot_left") != std::string::npos)
            cam[CAM_LEFT] = std::static_pointer_cast<CameraSensor>(s);
        if (name.find("cam_turtlebot_right") != std::string::npos)
            cam[CAM_RIGHT] = std::static_pointer_cast<CameraSensor>(s);
        if (name.find("bumper") != std::string::npos){
            bumper = std::static_pointer_cast<ContactSensor>(s);
        }

        pose = model->GetWorldPose();
    }
}


void
TurtlebotSensors::Init(){
    for (int id=0; id<NUM_CAMS; id++){
        if (cam[id]){
            sub_cam[id] = cam[id]->ConnectUpdated(
                boost::bind(&TurtlebotSensors::_on_cam, this, id));
        }else
            std::cerr << _log_prefix << "\t cam["<<id<<"] was not connected (NULL pointer)" << std::endl;
    }

    if (laser){
        sub_laser = laser->ConnectUpdated(
            boost::bind(&TurtlebotSensors::_on_laser, this));
    }else
        std::cerr << _log_prefix << "\t laser was not connected (NULL pointer)" << std::endl;


    if (bumper){
        sub_bumper = bumper->ConnectUpdated(
            boost::bind(&TurtlebotSensors::_on_bumper, this));
        bumper->SetActive(true);
    }else
        std::cerr << _log_prefix << "\t bumper was not connected (NULL pointer)" << std::endl;

    ONDEBUG_INFO(std::cout << _log_prefix << "Initial Pose [x,y,z] " << pose.pos.x << ", " << pose.pos.y << ", " << pose.pos.z << std::endl;)

    //Pose3d
    updatePose = gazebo::event::Events::ConnectWorldUpdateBegin(
                boost::bind(&TurtlebotSensors::OnUpdate, this, _1));
}

void
TurtlebotSensors::debugInfo(){
    std::cout << _log_prefix << "Sensors of " << model->GetName() << std::endl;
    boost::format fmt(_log_prefix+"\t%1% (id: %2%)\n");

    std::cout << fmt % cam[CAM_LEFT]->Name() % cam[CAM_LEFT]->Id();
    std::cout << fmt % cam[CAM_RIGHT]->Name() % cam[CAM_RIGHT]->Id();
    std::cout << fmt % laser->Name() % laser->Id();
    std::cout << fmt % bumper->Name() % bumper->Id();
}

void
TurtlebotSensors::_on_cam(int id){
    //// Assumption: Camera (raw) data is constant, so after Camera boostrap
    /// Camera.data will be a constant pointer.
    /// Therefore, we can use the cv::Mat constructor to simply wrap this
    /// data and avoid copy overload.
    /// Warning: thread safe is not supplied, but one can never ensure it
    /// if CameraSensor thread is overriding data and there are no option
    /// to intercept this update.
    /// Warning 2: char* is NULL until CameraSensor.OnUpdate(), so it is not
    /// possible to bootstrap at Load neither Init step.

    if (img[id].empty()){

        const unsigned char *data = cam[id]->ImageData();
        if (data == 0)
            return;

        std::cout <<  _log_prefix << "\tbootstrap cam["<<id<<"]" << std::endl;
        uint32_t h = cam[id]->ImageHeight();
        uint32_t w = cam[id]->ImageWidth();
        img[id] = cv::Mat(h, w, CV_8UC3, (uint8_t*)data);

        cam[id]->DisconnectUpdated(sub_cam[id]); // close connection
     }
}


void
TurtlebotSensors::_on_laser(){
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
TurtlebotSensors::_on_bumper(){

    BumperD data;

    //bumper values
    std::map<std::string, gazebo::physics::Contact> contacts;
    contacts = bumper->Contacts("turtlebot::create::base::base_collision");

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
TurtlebotSensors::OnUpdate(const gazebo::common::UpdateInfo & /*_info*/){
    pose = model->GetWorldPose();
}
