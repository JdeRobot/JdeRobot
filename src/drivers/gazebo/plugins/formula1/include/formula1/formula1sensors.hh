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


#ifndef FORMULA1SENSORS_H
#define FORMULA1SENSORS_H

#include <iostream>
#include <boost/bind.hpp>
#include <boost/format.hpp>

#include <gazebo/gazebo.hh>
#include <gazebo/physics/Model.hh>
#include <gazebo/sensors/sensors.hh>
#include <gazebo/sensors/SensorManager.hh>
#include <gazebo/common/Events.hh>

#include <opencv2/core/core.hpp>

#include <formula1/debugtools.h>

namespace formula1{

class Formula1Sensors
{
public:
    enum{
        CAM_LEFT = 0,
        CAM_RIGHT,
        NUM_CAMS
    };

public:
    Formula1Sensors();
    virtual ~Formula1Sensors();

    void Load(gazebo::physics::ModelPtr model);
    void Init();
    void OnUpdate(const gazebo::common::UpdateInfo & _info);

    void debugInfo();

    std::string _log_prefix;

public:
    gazebo::sensors::CameraSensorPtr cam[NUM_CAMS];
    gazebo::sensors::RaySensorPtr laser;


private:
    gazebo::physics::ModelPtr model;
    uint32_t base_link_id;

private:
    gazebo::event::ConnectionPtr sub_cam[NUM_CAMS];
    gazebo::event::ConnectionPtr sub_laser;
    gazebo::event::ConnectionPtr updatePose;

    void _on_cam(int id);
    void _on_laser();
    void _on_pose();

public:
    cv::Mat img[NUM_CAMS];
    std::vector<float> laserValues;
    gazebo::math::Pose pose;

    double position;
};

}//NS

#endif // QUADROTORSENSORS_H
