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

#ifndef CAMERAI_H
#define CAMERAI_H

#include <quadrotor/interfaces/cameraibase.h>

#include <quadrotor/quadrotorsensors.hh>
#include <gazebo/common/Events.hh>


namespace quadrotor{
namespace interfaces{


class CameraI: public quadrotor::interfaces::CameraIBase {
public:
    CameraI (const quadrotor::QuadRotorSensors *sensor);
    virtual ~CameraI ();

    const int cam_id = QuadRotorSensors::CAM_VENTRAL;

private:
    cv::Mat imgCached;
    const quadrotor::QuadRotorSensors *sensor;

/// Gazebo
private:
    void onCameraSensorBoostrap();
    void onCameraSensorUpdate();

    gazebo::event::ConnectionPtr cameraSensorConnection;


};


}}//NS

#endif // CAMERAI_H

