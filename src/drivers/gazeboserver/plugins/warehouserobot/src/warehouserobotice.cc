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
 *       Francisco Perez Salgado <f.perez475@gmai.com>
 *       Pablo Moreno Vera <p.morenov@alumnos.urjc.es>
 */


#include <warehouserobot/warehouserobotice.hh>

using namespace warehouserobot;
using namespace warehouserobot::interfaces;
using namespace Ice;

WarehouserobotIce::WarehouserobotIce(CommunicatorPtr ic, const WarehouserobotSensors *sensors, WarehouserobotControlaControl *control, CameraProxy *camproxy):
    ic(ic),
    sensor(sensors),
    control(control)
    camproxy(camproxy)
{
    assert(ic != 0);
}

WarehouserobotIce::~WarehouserobotIce(){
    stop();
}

void
WarehouserobotIce::stop(){
    ONDEBUG_INFO(std::cout<<_log_prefix << "WarehouserobotIce::stop()" << std::endl;)
    lock.lock();
    if (ic && !ic->isShutdown()){
        std::cout<<_log_prefix << "Shuting down Ice..." << std::endl;
        adapter->deactivate();
        ic->shutdown();
    }
    lock.unlock();
}

void
WarehouserobotIce::start(){
    /// boost::thread "thread" lifetime survives "object" lifetime if
    /// thread was running when object is deleted
    /// Is same effect that detach()
    /// Therefore, there is no needed to manage object lifetime neither
    /// delete it.
   boost::thread(boost::bind(&WarehouserobotIce::run, this));
}

void
WarehouserobotIce::run(){
#if 0
    // Register to handle the signals that indicate when the server should exit.
    // It is safe to register for the same signal multiple times in a program,
    // provided all registration for the specified signal is made through Asio.
    // API changed from 1.47 to 1.59 (now requires io_service)
    boost::asio::io_service _io_service;
    boost::asio::signal_set _signals(_io_service);
    _signals.add(SIGINT);
    _signals.add(SIGTERM);
    _signals.async_wait(boost::bind(&WarehouserobotIce::stop, this));
#endif

    bootstrap();

    ic->waitForShutdown();

    std::cout<<_log_prefix << "Ice is down now" << std::endl;
}

void WarehouserobotIce::bootstrap(){
    lock.lock();
    if (ic->isShutdown()) return;

    prop = ic->getProperties();

    adapter = ic->createObjectAdapter("Warehouserobot.Adapter");
    std::cout<<_log_prefix << "Ice adapter listening at " << std::endl;
    std::cout<<_log_prefix << "\t" << adapter->getEndpoints()[0]->toString() << std::endl;

    std::string name;

    name = prop->getProperty("Warehouserobot.Motors.Name");
    ObjectPtr motorsi = new MotorsI(control);
    adapter->add(motorsi, ic->stringToIdentity(name));

    ObjectPtr warehouserobotcontroli = new WarehouserobotControlI(control, camproxy);
    name = prop->getProperty("Warehouserobot.Extra.Name");
    adapter->add(warehouserobotcontroli, ic->stringToIdentity(name));

    name = prop->getProperty("Warehouserobot.Pose3D.Name");
    ObjectPtr posei = new Pose3DI(sensor, control);
    adapter->add(posei, ic->stringToIdentity(name));

    ObjectPtr laseri = new LaserI(sensor);
    name = prop->getProperty("Warehouserobot.Laser.Name");
    adapter->add(laseri, ic->stringToIdentity(name));

    ObjectPtr bumperi = new BumperI(sensor);
    name = prop->getProperty("Warehouserobot.Bumper.Name");
    adapter->add(bumperi, ic->stringToIdentity(name));

    //ObjectPtr camerai = new CameraI(sensor);
    ObjectPtr camerai;
    {
        PushCameraI *_camerai = new PushCameraI();
        camproxy->registerConsumer(ICameraConsumerPtr(_camerai));
        camerai = ObjectPtr(_camerai);
    }
    name = prop->getProperty("Warehouserobot.Camera.Name");
    adapter->add(camerai, ic->stringToIdentity(name));

    adapter->activate();

    std::cout<<_log_prefix<< "Ice booststrap done." << std::endl;
    lock.unlock();
}
