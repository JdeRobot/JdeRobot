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

#ifndef NAOSERVERMOTIONS_H
#define NAOSERVERMOTIONS_H

#include <iostream>
#include <stdio.h>

#include "alcore/alptr.h"
#include <alproxies/almotionproxy.h>
#include "alcommon/alproxy.h"
#include "alcommon/albroker.h"
#include "alcommon/almodule.h"

#include "Singleton.h"

#include <IceE/IceE.h>
#include <naomotions.h>

typedef enum MotionEnum {
    RIGHTKICK,
    LEFTKICK,
    STANDUP_BACK,
    STANDUP_FRONT,
    INTERCEPT,
    CHANGECAMERA,
    RESETNAOQI
} MotionEnum;

class NaoServerMotions : public Singleton<NaoServerMotions>, public jderobot::NaoMotions {
public:
    // Constructor
    NaoServerMotions ();
    
    // Destructor
    ~NaoServerMotions ();
    
    // Another functions
    void init ( const std::string newName, AL::ALPtr<AL::ALBroker> parentBroker );
    
    /*NaoMotions*/
    Ice::Int setMotion ( jderobot::MotionType motion, const Ice::Current& );
    
private:
    std::string name;
    AL::ALPtr<AL::ALMotionProxy> motion;
    
    int forwardKickRight ();
    int forwardKickLeft ();
    int standupBack ();
    int standupFront ();
    int intercept ();
    int resetNaoqi ();
};

#endif // NAOSERVERMOTIONS_H
