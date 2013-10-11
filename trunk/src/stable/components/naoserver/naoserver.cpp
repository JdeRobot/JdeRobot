/*
 *  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors : Borja Menéndez <borjamonserrano@gmail.com>
 *
 */

#include "naoserver.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
NaoServer::NaoServer ( AL::ALPtr<AL::ALBroker> broker, const std::string& name ) :
ALModule(broker, name) {
    NeckMotors* neckMotors = NeckMotors::getInstance();
    neckMotors->init("NeckMotors", getParentBroker(), 1.0, 0.1);
//    NeckSpeed* neckSpeed = NeckSpeed::getInstance();
//    neckSpeed->init("NeckSpeed", getParentBroker(), 1.0, 0.1);
    LeftShoulderMotors* lshoulderMotors = LeftShoulderMotors::getInstance();
    lshoulderMotors->init("LeftShoulderMotors", getParentBroker(), 1.0, 0.1);
    RightShoulderMotors* rshoulderMotors = RightShoulderMotors::getInstance();
    rshoulderMotors->init("RightShoulderMotors", getParentBroker(), 1.0, 0.1);
    LeftElbowMotors* lelbowMotors = LeftElbowMotors::getInstance();
    lelbowMotors->init("LeftElbowMotors", getParentBroker(), 1.0, 0.1);
    RightElbowMotors* relbowMotors = RightElbowMotors::getInstance();
    relbowMotors->init("RightElbowMotors", getParentBroker(), 1.0, 0.1);
    LeftHipMotors* lhipMotors = LeftHipMotors::getInstance();
    lhipMotors->init("LeftHipMotors", getParentBroker(), 1.0, 0.1);
    RightHipMotors* rhipMotors = RightHipMotors::getInstance();
    rhipMotors->init("RightHipMotors", getParentBroker(), 1.0, 0.1);
    LeftKneeMotors* lkneeMotors = LeftKneeMotors::getInstance();
    lkneeMotors->init("LeftKneeMotors", getParentBroker(), 1.0, 0.1);
    RightKneeMotors* rkneeMotors = RightKneeMotors::getInstance();
    rkneeMotors->init("RightKneeMotors", getParentBroker(), 1.0, 0.1);
    LeftAnkleMotors* lankleMotors = LeftAnkleMotors::getInstance();
    lankleMotors->init("LeftAnkleMotors", getParentBroker(), 1.0, 0.1);
    RightAnkleMotors* rankleMotors = RightAnkleMotors::getInstance();
    rankleMotors->init("RightAnkleMotors", getParentBroker(), 1.0, 0.1);
    
//    NaoFollowBall* naofollowball = NaoFollowBall::getInstance();
//    NaoFollowBall->init("NaoFollowBall", getParentBroker(), 1.0, 0.1);
    
//    NaoServerCamera* naoserverCamera = NaoServerCamera::getInstance();
//    naoserverCamera->init("NaoServerCamera", getParentBroker());
    NaoServerMotors* naoserverMotors = NaoServerMotors::getInstance();
    naoserverMotors->init("NaoServerMotors", getParentBroker());
    NaoServerMotions* naoserverMotions = NaoServerMotions::getInstance();
    naoserverMotions->init("NaoServerMotions", getParentBroker());
    
    myIcePort = 10000;
    
    pthread_create(&tIce, NULL, &NaoServer::iceThread, &myIcePort);
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
NaoServer::~NaoServer () {}

/*************************************************************
 * ICE THREAD
 *************************************************************/
void* NaoServer::iceThread ( void* obj) {
    std::stringstream endpoint;
    int *icePort = (int *) obj;
    
    Ice::CommunicatorPtr ic;
    Ice::PropertiesPtr prop;
    
    char* name = (char *) "";
    char* argv[] = {name};
    int argc = 0;
    
    try {
        endpoint << "default -p " << *icePort;
        
        ic = Ice::initialize(argc, argv);
		Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints("NaoServerAdapter", endpoint.str());
		
		adapter->add(NeckMotors::getInstance(), ic->stringToIdentity("NeckMotors"));
//		adapter->add(NeckSpeed::getInstance(), ic->stringToIdentity("NeckSpeed"));
		adapter->add(LeftShoulderMotors::getInstance(), ic->stringToIdentity("LeftShoulderMotors"));
		adapter->add(RightShoulderMotors::getInstance(), ic->stringToIdentity("RightShoulderMotors"));
		adapter->add(LeftElbowMotors::getInstance(), ic->stringToIdentity("LeftElbowMotors"));
		adapter->add(RightElbowMotors::getInstance(), ic->stringToIdentity("RightElbowMotors"));
		adapter->add(LeftHipMotors::getInstance(), ic->stringToIdentity("LeftHipMotors"));
		adapter->add(RightHipMotors::getInstance(), ic->stringToIdentity("RightHipMotors"));
		adapter->add(LeftKneeMotors::getInstance(), ic->stringToIdentity("LeftKneeMotors"));
		adapter->add(RightKneeMotors::getInstance(), ic->stringToIdentity("RightKneeMotors"));
		adapter->add(LeftAnkleMotors::getInstance(), ic->stringToIdentity("LeftAnkleMotors"));
		adapter->add(RightAnkleMotors::getInstance(), ic->stringToIdentity("RightAnkleMotors"));
		
//		adapter->add(NaoFollowBall::getInstance(), ic->stringToIdentity("NaoFollowBall"));
		
//		adapter->add(NaoServerCamera::getInstance(), ic->stringToIdentity("Camera"));
        adapter->add(NaoServerMotors::getInstance(), ic->stringToIdentity("Motors"));
        adapter->add(NaoServerMotions::getInstance(), ic->stringToIdentity("Motions"));
        
        adapter->activate();
        ic->waitForShutdown();
    } catch (const Ice::Exception& ex) {
        std::cout << "Ice::Exception: " << ex << std::endl;
    } catch (const char* msg) {
        std::cout << "Message: " << msg << std::endl;
    }
        
    if (ic) {
        try {
            ic->destroy();
        } catch (const Ice::Exception& e) {
            std::cout << "Ice::Exception trying to destroy the communicator: " << e << std::endl;
        }
    }
}
