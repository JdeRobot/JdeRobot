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

#ifndef NAOSERVERMOTORS_H
#define NAOSERVERMOTORS_H

#include <iostream>
#include <stdio.h>

#include "alcore/alptr.h"
#include <alproxies/almotionproxy.h>
#include "alcommon/alproxy.h"
#include "alcommon/albroker.h"
#include "alcommon/almodule.h"

#include "Singleton.h"

#include <IceE/IceE.h>
#include <motors.h>

class NaoServerMotors : public Singleton<NaoServerMotors>, public jderobot::Motors {
public:
    // Constructor
    NaoServerMotors ();
    
    // Destructor
    ~NaoServerMotors ();
    
    // Another functions
    void init ( const std::string newName, AL::ALPtr<AL::ALBroker> parentBroker );
    
    /*Motors*/
    Ice::Float getV ( const Ice::Current& );
    Ice::Int setV ( float v, const Ice::Current& );
    Ice::Float getW ( const Ice::Current& );
    Ice::Int setW ( float w, const Ice::Current& );
    Ice::Float getL ( const Ice::Current& );
    Ice::Int setL ( float l, const Ice::Current& );
    
private:
    float v, w, l;
    std::string name;
    AL::ALPtr<AL::ALMotionProxy> motion;
};
#endif // NAOSERVERMOTORS_H
