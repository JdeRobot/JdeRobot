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
 *  REMIX of https://github.com/jderobot-varribas/gazeboplugin-quadrotor2/blob/2.1.0/include/quadrotor/interfaces/camerai.h
 *  Victor Arribas Raigadas <v.arribas.urjc@gmai.com>
 *  
 *  Authors:
 *       Francisco Perez Salgado <f.pererz475@gmai.com>
 */

#ifndef CAMERAI_H
#define CAMERAI_H

#include <turtlebot/interfaces/cameraibase.h>

#include <turtlebot/turtlebotsensors.hh>
#include <gazebo/common/Events.hh>


namespace turtlebot{
namespace interfaces{


class CameraI: public turtlebot::interfaces::CameraIBase {
public:
    CameraI (const turtlebot::TurtlebotSensors *sensor, int cam_id);
    virtual ~CameraI ();


private:
    cv::Mat imgCached;
    const turtlebot::TurtlebotSensors *sensor;
    const int cam_id;


/// Gazebo
private:
    void onCameraSensorBoostrap();
    void onCameraSensorUpdate();

    gazebo::event::ConnectionPtr cameraSensorConnection;


};


}}//NS

#endif // CAMERAI_H

