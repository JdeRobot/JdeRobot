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

#ifndef FORMULA1ICE_H
#define FORMULA1ICE_H


#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <easyiceconfig/EasyIce.h>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <formula1/interfaces/motorsi.h>
#include <formula1/interfaces/pose3di.h>
#include <formula1/interfaces/pushcamerai.h>
#include <formula1/interfaces/laseri.h>

#include <formula1/formula1sensors.hh>
#include <formula1/formula1control.hh>
#include <formula1/cameraproxy.hh>

#include <formula1/debugtools.h>

namespace formula1{

class Formula1Ice
{
public:
    Formula1Ice(Ice::CommunicatorPtr ic, const Formula1Sensors *sensors, Formula1Control *control, CameraProxy *camproxy);
    virtual ~Formula1Ice();

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
    const Formula1Sensors *sensor;
    Formula1Control *control;
    CameraProxy *camproxy;
    boost::mutex lock;

};

typedef boost::shared_ptr<Formula1Ice> Formula1IcePtr;

}//NS

#endif // QUADROTORICE_H
