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


#ifndef QUADROTORSENSORS_H
#define QUADROTORSENSORS_H

#include <iostream>
#include <boost/bind.hpp>
#include <boost/format.hpp>

#include <gazebo/gazebo.hh>
#include <gazebo/physics/Model.hh>
#include <gazebo/sensors/sensors.hh>
#include <gazebo/sensors/SensorManager.hh>
#include <gazebo/common/Events.hh>

#include <opencv2/core/core.hpp>

#include <quadrotor/debugtools.h>

#define BROKEN_SonarSensor


namespace quadrotor{

class QuadRotorSensors
{
public:
    QuadRotorSensors();
    virtual ~QuadRotorSensors();

    void Load(gazebo::physics::ModelPtr model);
    void Init();

    void debugInfo();

    std::string _log_prefix;

public:
    gazebo::sensors::CameraSensorPtr cam_ventral;
    gazebo::sensors::CameraSensorPtr cam_frontal;
#ifdef BROKEN_SonarSensor
    gazebo::sensors::RaySensorPtr sonar;
#else
    gazebo::sensors::SonarSensorPtr sonar;
#endif
    gazebo::sensors::ImuSensorPtr imu;

private:
    gazebo::physics::ModelPtr model;
    uint32_t base_link_id;

private:
    gazebo::event::ConnectionPtr sub_cam_frontal;
    gazebo::event::ConnectionPtr sub_cam_ventral;
    gazebo::event::ConnectionPtr sub_sonar;
    gazebo::event::ConnectionPtr sub_imu;

    void _on_cam_frontal();
    void _on_cam_ventral();
    void _on_sonar();
    void _on_imu();

public:
    cv::Mat img_frontal;
    cv::Mat img_ventral;
    gazebo::math::Pose pose;
    double altitude;

};

}//NS

#endif // QUADROTORSENSORS_H
