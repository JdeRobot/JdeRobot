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

#ifndef ROOMBAICE_H
#define ROOMBAICE_H


#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <easyiceconfig/EasyIce.h>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <roomba/interfaces/motorsi.h>
#include <roomba/interfaces/pose3di.h>
#include <roomba/interfaces/laseri.h>
#include <roomba/interfaces/bumperi.h>

#include <roomba/roombasensors.hh>
#include <roomba/roombacontrol.hh>

#include <roomba/debugtools.h>

namespace roomba{

class RoombaIce
{
public:
    RoombaIce(Ice::CommunicatorPtr ic, const RoombaSensors *sensors, RoombaControl *control);
    virtual ~RoombaIce();

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
    const RoombaSensors *sensor;
    RoombaControl *control;
    boost::mutex lock;

};

typedef boost::shared_ptr<RoombaIce> RoombaIcePtr;

}//NS

#endif // QUADROTORICE_H
