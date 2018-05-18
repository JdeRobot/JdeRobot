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
 *  REMIX of https://github.com/jderobot-varribas/gazeboplugin-quadrotor2/blob/2.1.0/include/quadrotor/quadrotorice.hh
 *  Victor Arribas Raigadas <v.arribas.urjc@gmai.com>
 *  
 *  Authors:
 *       Francisco Perez Salgado <f.pererz475@gmai.com>
 */

#ifndef TURTLEBOTICE_H
#define TURTLEBOTICE_H


#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <easyiceconfig/EasyIce.h>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <turtlebot/interfaces/kinectPlugini.h>
#include <turtlebot/interfaces/motorsi.h>
#include <turtlebot/interfaces/pose3di.h>
#include <turtlebot/interfaces/camerai.h>
#include <turtlebot/interfaces/laseri.h>
#include <turtlebot/interfaces/bumperi.h>

#include <turtlebot/turtlebotsensors.hh>
#include <turtlebot/turtlebotcontrol.hh>

#include <turtlebot/debugtools.h>

namespace turtlebot{

class TurtlebotIce
{
public:
    TurtlebotIce(Ice::CommunicatorPtr ic, const TurtlebotSensors *sensors, TurtlebotControl *control);
    virtual ~TurtlebotIce();

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
    const TurtlebotSensors *sensor;
    TurtlebotControl *control;
    boost::mutex lock;

};

typedef boost::shared_ptr<TurtlebotIce> TurtlebotIcePtr;

}//NS

#endif // QUADROTORICE_H
