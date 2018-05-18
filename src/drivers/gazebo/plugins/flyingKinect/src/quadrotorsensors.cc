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


#include "quadrotor/quadrotorsensors.hh"


using namespace quadrotor;
using namespace gazebo::physics;
using namespace gazebo::sensors;

QuadRotorSensors::QuadRotorSensors(){
    ONDEBUG_INFO(std::cout << _log_prefix << "QuadRotorSensors::QuadRotorSensors()" << std::endl;)
}

QuadRotorSensors::~QuadRotorSensors(){
    ONDEBUG_INFO(std::cout << _log_prefix << "QuadRotorSensors::~QuadRotorSensors()" << std::endl;)
}

void
QuadRotorSensors::Load(ModelPtr model){
    this->model = model;
    this->base_link_id = model->GetChildLink("base_link")->GetId();

    SensorManager *sm = SensorManager::Instance();

    for (SensorPtr s: sm->GetSensors()){
        if (s->ParentId() != base_link_id) continue;
        std::string name = s->Name();
        if (name.find("imu_sensor") != std::string::npos)
            imu = std::static_pointer_cast<ImuSensor>(s);
        if (name.find("sonar") != std::string::npos)
            sonar = std::static_pointer_cast<
#ifdef BROKEN_SonarSensor
                    RaySensor
#else
                    SonarSensor
#endif
>(s);
        if (name.find("frontal") != std::string::npos)
            cam[CAM_FRONTAL] = std::static_pointer_cast<CameraSensor>(s);
        if (name.find("ventral") != std::string::npos)
            cam[CAM_VENTRAL] = std::static_pointer_cast<CameraSensor>(s);
    }

    // weak-fix for sonar value at boostrap (1/2)
    altitude = model->GetWorldPose().pos.z;
}


void
QuadRotorSensors::Init(){
    for (int id=0; id<NUM_CAMS; id++){
        if (cam[id]){
            sub_cam[id] = cam[id]->ConnectUpdated(
                boost::bind(&QuadRotorSensors::_on_cam, this, id));
        }else
            std::cerr << _log_prefix << "\t cam["<<id<<"] was not connected (NULL pointer)" << std::endl;
    }

    if (sonar){
        sub_sonar = sonar->ConnectUpdated(
            boost::bind(&QuadRotorSensors::_on_sonar, this));
    }else
        std::cerr << _log_prefix << "\t sonar was not connected (NULL pointer)" << std::endl;

    if (imu){
        sub_imu = imu->ConnectUpdated(
            boost::bind(&QuadRotorSensors::_on_imu, this));
    }else
        std::cerr << _log_prefix << "\t imu_sensor was not connected (NULL pointer)" << std::endl;

    // weak-fix for sonar value at boostrap (2/2)
    sonar->Update(true);
}

void
QuadRotorSensors::debugInfo(){
    std::cout << _log_prefix << "Sensors of " << model->GetName() << std::endl;
    boost::format fmt(_log_prefix+"\t%1% (id: %2%)\n");

    std::cout << fmt % cam[CAM_VENTRAL]->Name() % cam[CAM_VENTRAL]->Id();
    std::cout << fmt % cam[CAM_FRONTAL]->Name() % cam[CAM_FRONTAL]->Id();
    std::cout << fmt % sonar->Name() % sonar->Id();
    std::cout << fmt % imu->Name() % imu->Id();
}

void
QuadRotorSensors::_on_cam(int id){
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
QuadRotorSensors::_on_sonar(){
#ifdef BROKEN_SonarSensor
    assert(sonar->RangeCount() > 0);
    std::vector<double> ranges(sonar->RangeCount());
    sonar->Ranges(ranges);
    std::sort(ranges.begin(), ranges.end());
    //altitude = ranges[0];
    int c = std::ceil(ranges.size()*0.20); // smooth value by take 20% of minor values
    ranges.resize(c);
    altitude = std::accumulate(ranges.begin(), ranges.end(), 0.0, std::plus<double>())/(double)c;
#else
    altitude = sonar->GetRange();
#endif
}

void
QuadRotorSensors::_on_imu(){
    pose = model->GetWorldPose();
    pose.rot = imu->Orientation();
//    std::cout<<pose<<std::endl;
}
