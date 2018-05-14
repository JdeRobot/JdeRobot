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
 *       Pablo Moreno Vera <p.morenov@alumnos.urjc.es>
 */

#ifndef CAMERAI_H
#define CAMERAI_H

#include <warehouserobot/interfaces/cameraibase.h>

#include <warehouserobot/warehouserobotrsensors.hh>
#include <gazebo/common/Events.hh>


namespace warehouserobot{
namespace interfaces{


class CameraI: public warehouserobot::interfaces::CameraIBase {
public:
    CameraI (const warehouserobot::WarehouserobotSensors *sensor);
    virtual ~CameraI ();

    const int cam_id = WarehouserobotSensors::CAM_VENTRAL;

private:
    cv::Mat imgCached;
    const warehouserobot::WarehouserobotSensors *sensor;

/// Gazebo
private:
    void onCameraSensorBoostrap();
    void onCameraSensorUpdate();

    gazebo::event::ConnectionPtr cameraSensorConnection;


};


}}//NS

#endif // CAMERAI_H
