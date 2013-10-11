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

#ifndef NAOSERVER_H
#define NAOSERVER_H

#include <iostream>
#include <stdio.h>

#include "alcore/alptr.h"
#include "alproxies/alledsproxy.h"
#include "alproxies/almemoryproxy.h"
#include "alproxies/alsensorsproxy.h"
#include "alproxies/alsonarproxy.h"
#include "alproxies/alrobotposeproxy.h"
#include "alcommon/alproxy.h"
#include "alcommon/albroker.h"

// ICE utils includes
#include <IceE/IceE.h>

#include "hinges/NeckMotors.h"
//#include "hinges/NeckSpeed.h"
#include "hinges/LeftShoulderMotors.h"
#include "hinges/RightShoulderMotors.h"
#include "hinges/LeftElbowMotors.h"
#include "hinges/RightElbowMotors.h"
#include "hinges/LeftHipMotors.h"
#include "hinges/RightHipMotors.h"
#include "hinges/LeftKneeMotors.h"
#include "hinges/RightKneeMotors.h"
#include "hinges/LeftAnkleMotors.h"
#include "hinges/RightAnkleMotors.h"
//#include "hinges/NaoFollowBall.h"

//#include "functions/NaoServerCamera.h"
#include "functions/NaoServerMotors.h"
#include "functions/NaoServerMotions.h"

using namespace std;
using namespace jderobot;

namespace AL
{
class ALBroker;
}

using namespace AL;

class NaoServer : public AL::ALModule {
public:
    // Constructor
    NaoServer ( AL::ALPtr<AL::ALBroker> broker, const std::string& name );
    
    // Destructor
    virtual ~NaoServer ();
    
    // Ice thread
    static void* iceThread ( void *obj );
    
private:    
    pthread_t tIce;
    int myIcePort;
    /*
    NeckMotors* neckMotors;
    NeckSpeed* neckSpeed;
    LeftShoulderMotors* lshoulderMotors;
    RightShoulderMotors* rshoulderMotors;
    LeftElbowMotors* lelbowMotors;
    RightElbowMotors* relbowMotors;
    LeftHipMotors* lhipMotors;
    RightHipMotors* rhipMotors;
    LeftKneeMotors* lkneeMotors;
    RightKneeMotors* rkneeMotors;
    LeftAnkleMotors* lankleMotors;
    RightAnkleMotors* rankleMotors;
    
    NaoServerCamera* naoserverCamera;
    NaoServerMotors* naoserverMotors;
    NaoServerMotions* naoserverMotions;
    */
};
#endif // NAOSERVER_H
