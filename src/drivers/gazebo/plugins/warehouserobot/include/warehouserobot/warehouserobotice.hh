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

#ifndef WAREHOUSEROBOTICE_H
#define WAREHOUSEROBOTICE_H


#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <easyiceconfig/EasyIce.h>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <warehouserobot/interfaces/motorsi.h>
#include <warehouserobot/interfaces/pose3di.h>
#include <warehouserobot/interfaces/laseri.h>
#include <warehouserobot/interfaces/bumperi.h>
#include <warehouserobot/interfaces/pushcamerai.h>

#include <warehouserobot/warehouserobotsensors.hh>
#include <warehouserobot/warehouserobotcontrol.hh>
#include <warehouserobot/cameraproxy.hh>

#include <warehouserobot/debugtools.h>

namespace warehouserobot{

class WarehouserobotIce
{
public:
    RoombaIce(Ice::CommunicatorPtr ic, const WarehouserobotSensors *sensors, WarehouserobotControl *control);
    virtual ~WarehouserobotIce();

    void run();
    void start();
    void stop();

    std::string _log_prefix;

protected:
    void bootstrap();

private:
    Ice::CommunicatorPtr ic;
    Ice::PropertiesPtr prop;
    Ice::ObjectAdapterPtr adapter;

private:
    const WarehouserobotSensors *sensor;
    WarehouserobotControl *control;
    CameraProxy *camproxy;
    boost::mutex lock;

};

typedef boost::shared_ptr<WarehouserobotIce> WarehouserobotIcePtr;

}//NS

#endif // WAREHOUSEROBOTRICE_H
