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

#ifndef QUADROTORICE_H
#define QUADROTORICE_H


#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <easyiceconfig/EasyIce.h>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <quadrotor/interfaces/pose3di.h>
#include <quadrotor/interfaces/navdatai.h>
#include <quadrotor/interfaces/dronecontroli.h>
#include <quadrotor/interfaces/cmdveli.h>
#include <quadrotor/interfaces/pushcamerai.h>

#include <quadrotor/quadrotorsensors.hh>
#include <quadrotor/quadrotorcontrol.hh>
#include <quadrotor/cameraproxy.hh>

#include <quadrotor/debugtools.h>

namespace quadrotor{

class QuadrotorIce
{
public:
    QuadrotorIce(Ice::CommunicatorPtr ic, const QuadRotorSensors *sensors, QuadrotorControl *control, CameraProxy *camproxy);
    virtual ~QuadrotorIce();

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
    const QuadRotorSensors *sensor;
    QuadrotorControl *control;
    CameraProxy *camproxy;
    boost::mutex lock;

};

typedef boost::shared_ptr<QuadrotorIce> QuadrotorIcePtr;

}//NS

#endif // QUADROTORICE_H
