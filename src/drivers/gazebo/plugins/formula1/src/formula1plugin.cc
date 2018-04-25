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



#include "formula1/formula1plugin.hh"

GZ_REGISTER_MODEL_PLUGIN(formula1::Formula1Plugin)

using namespace formula1;
using namespace gazebo::physics;
using namespace gazebo::math;
using namespace gazebo::event;
using namespace gazebo::common;


Formula1Plugin::Formula1Plugin(){
    std::stringstream ss;
    ss << "["<<(void*)this<<"] ";
    ss >> _log_prefix;
    ONDEBUG_INFO(std::cout << _log_prefix << "Formula1Plugin::Formula1Plugin()" << std::endl;)
    sensors._log_prefix = _log_prefix;
    control._log_prefix = _log_prefix;
}

Formula1Plugin::~Formula1Plugin(){
    ONDEBUG_INFO(std::cout << _log_prefix << "Formula1Plugin::~Formula1Plugin()" << std::endl;)
    //icePlugin->stop();
}


void
Formula1Plugin::Load(ModelPtr _model, sdf::ElementPtr _sdf){
    ONDEBUG_INFO(std::cout << _log_prefix << "Formula1Plugin::Load()" << std::endl;)

    _log_prefix = "["+_model->GetName()+"] ";
    sensors._log_prefix = _log_prefix;
    control._log_prefix = _log_prefix;

    model = _model;
    
    sensors.Load(model);
    control.Load(model, _sdf);

    // Listen to the update event. This event is broadcast every
    // simulation iteration.
    updateConnection = Events::ConnectWorldUpdateBegin(
        boost::bind(&Formula1Plugin::OnUpdate, this, _1));

    sigintConnection = Events::ConnectSigInt(
        boost::bind(&Formula1Plugin::OnSigInt, this));

    this->InitializeIce(_sdf);
}


void
Formula1Plugin::Init(){
    ONDEBUG_INFO(std::cout << _log_prefix << "Formula1Plugin::Init()" << std::endl;)
    sensors.debugInfo();
    sensors.Init();

    sensors.cam[Formula1Sensors::CAM_LEFT]->SetActive(true);
    sensors.cam[Formula1Sensors::CAM_RIGHT]->SetActive(true);
    sensors.laser->SetActive(true);

    cameraproxy.registerCamera(sensors.cam[Formula1Sensors::CAM_LEFT]);
    cameraproxy.registerCamera(sensors.cam[Formula1Sensors::CAM_RIGHT]);
    cameraproxy.setActive(0);
    cameraproxy.setActive(1);

    control.Init(&sensors);

    icePlugin->start();
}


void
Formula1Plugin::OnUpdate(const UpdateInfo & ONDEBUG_VERBOSE(_info)){
    ONDEBUG_VERBOSE(std::cout << _log_prefix << "Formula1Plugin::OnUpdate()\n\t" << _info.simTime << std::endl;)
}

void
Formula1Plugin::OnSigInt(){
    ONDEBUG_INFO(std::cout << _log_prefix << "Formula1Plugin::OnSigInt()" << std::endl;)
    icePlugin->stop();
}


void
Formula1Plugin::Reset(){

}


void
Formula1Plugin::InitializeIce(sdf::ElementPtr _sdf){
    std::cout << _log_prefix << "Formula1Plugin::InitializeIce()" << std::endl;
    std::string iceConfigFile = "Formula1Plugin.cfg";
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
        id.properties->setProperty("Formula1.Adapter.Endpoints", "tcp -h localhost -p "+port); //ToDo: use regex replace instead hardcored text.

    std::cout << _log_prefix << "\tcreate Ice plugin..." << std::endl;
    icePlugin = Formula1IcePtr(new Formula1Ice(ic, &sensors, &control, &cameraproxy));
    icePlugin->_log_prefix = _log_prefix;
}
