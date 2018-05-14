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



#include "warehouserobot/warehouserobotplugin.hh"

GZ_REGISTER_MODEL_PLUGIN(warehouserobot::WarehouserobotPlugin)

using namespace warehouserobot;
using namespace gazebo::physics;
using namespace gazebo::math;
using namespace gazebo::event;
using namespace gazebo::common;


WarehouserobotPlugin::WarehouserobotPlugin(){
    std::stringstream ss;
    ss << "["<<(void*)this<<"] ";
    ss >> _log_prefix;
    ONDEBUG_INFO(std::cout << _log_prefix << "WarehouserobotPlugin::WarehouserobotPlugin()" << std::endl;)
    sensors._log_prefix = _log_prefix;
    control._log_prefix = _log_prefix;
}

WarehouserobotPlugin::~WarehouserobotPlugin(){
    ONDEBUG_INFO(std::cout << _log_prefix << "WarehouserobotPlugin::~WarehouserobotPlugin()" << std::endl;)
    //icePlugin->stop();
}


void
WarehouserobotPlugin::Load(ModelPtr _model, sdf::ElementPtr _sdf){
    _log_prefix = "["+_model->GetName()+"] ";
    sensors._log_prefix = _log_prefix;
    control._log_prefix = _log_prefix;

    model = _model;

    sensors.Load(model);
    control.Load(model, _sdf);

    // Listen to the update event. This event is broadcast every
    // simulation iteration.
    updateConnection = Events::ConnectWorldUpdateBegin(
        boost::bind(&WarehouserobotPlugin::OnUpdate, this, _1));

    sigintConnection = Events::ConnectSigInt(
        boost::bind(&WarehouserobotPlugin::OnSigInt, this));

    this->InitializeIce(_sdf);
}


void
WarehouserobotPlugin::Init(){
    ONDEBUG_INFO(std::cout << _log_prefix << "WarehouserobotPlugin::Init()" << std::endl;)
    sensors.debugInfo();
    sensors.Init();

    sensors.cam[WarehouserobotSensors::CAM_FRONTAL]->SetActive(true);
    sensors.laser->SetActive(true);
    sensors.bumper->SetActive(true);

    cameraproxy.registerCamera(sensors.cam[WarehouserobotSensors::CAM_FRONTAL]);
    cameraproxy.setActive(0);

    control.Init(&sensors);

    icePlugin->start();
}


void
WarehouserobotPlugin::OnUpdate(const UpdateInfo & ONDEBUG_VERBOSE(_info)){
    ONDEBUG_VERBOSE(std::cout << _log_prefix << "WarehouserobotPlugin::OnUpdate()\n\t" << _info.simTime << std::endl;)
}

void
WarehouserobotPlugin::OnSigInt(){
    ONDEBUG_INFO(std::cout << _log_prefix << "WarehouserobotPlugin::OnSigInt()" << std::endl;)
    icePlugin->stop();
}


void
WarehouserobotPlugin::Reset(){

}


void
WarehouserobotPlugin::InitializeIce(sdf::ElementPtr _sdf){
    std::cout << _log_prefix << "WarehouserobotPlugin::InitializeIce()" << std::endl;
    std::string iceConfigFile = "WarehouserobotPlugin.cfg";
    if(_sdf->HasElement("iceConfigFile"))
        iceConfigFile =  _sdf->GetElement("iceConfigFile")->GetValue()->GetAsString();
    std::cout << _log_prefix << "\tconfig: "<< iceConfigFile << std::endl;
#if 0
    Ice::StringSeq args;
    args.push_back("--Ice.Config=" + iceConfigFile);
    Ice::CommunicatorPtr ic = Ice::initialize(args);
#else
    Ice::InitializationData id;
    id.properties = Ice::createProperties();
#if 0
    id.properties->load(iceConfigFile);
#else /// EasyIce
    easyiceconfig::loader::loadIceConfig(iceConfigFile, id.properties);
#endif
    Ice::CommunicatorPtr ic = Ice::initialize(id);
#endif

    std::string port;
    /// Get Adapter port from _sdf (static bad)
    if(_sdf->HasElement("iceAdapterPort"))
        port = _sdf->GetElement("iceAdapterPort")->Get<std::string>();

    /// Get port from model <name> (wolrd file, good enough)
    std::string name = model->GetName();
    boost::regex re("[0-9]+$");
    boost::sregex_iterator eof, m1(name.begin(), name.end(), re);
    if (m1!=eof){
        port = (*m1)[0];
    }
    if (!port.empty())
        id.properties->setProperty("Warehouserobot.Adapter.Endpoints", "tcp -h localhost -p "+port); //ToDo: use regex replace instead hardcored text.

    std::cout << _log_prefix << "\tcreate Ice plugin..." << std::endl;
    icePlugin = WarehouserobotIcePtr(new WarehouserobotIce(ic, &sensors, &control));
    icePlugin->_log_prefix = _log_prefix;
}
