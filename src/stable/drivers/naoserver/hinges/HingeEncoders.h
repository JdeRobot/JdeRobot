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

#ifndef HINGEENCODERS_H
#define HINGEENCODERS_H

#include <iostream>
#include <stdio.h>

#include "alcore/alptr.h"
#include "alproxies/alledsproxy.h"
#include "alproxies/almemoryproxy.h"
#include "alproxies/alsensorsproxy.h"
#include "alproxies/alsonarproxy.h"
#include "alproxies/alrobotposeproxy.h"
#include "alproxies/almotionproxy.h"
#include "alcommon/alproxy.h"
#include "alcommon/albroker.h"
#include "alcommon/almodule.h"

#include <IceE/IceE.h>
#include <pose3dencoders.h>

class HingeEncoders : public jderobot::Pose3DEncoders {
    public:
        // Constructor
        HingeEncoders ();
        
        // Destructor
        ~HingeEncoders ();
        
        // Setters
        
        // Another functions
        void init ( const std::string newName, AL::ALPtr<AL::ALBroker> parentBroker );
        
        /*Pose3DEncoders*/
	    jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& );
	    
    protected:
        std::string name;
        bool bPitch, bYaw, bRoll;
        AL::ALValue jointPitch, jointYaw, jointRoll;
        AL::ALPtr<AL::ALMotionProxy> motion;
};
#endif // HINGEENCODERS_H
