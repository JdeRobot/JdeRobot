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

#ifndef NECKSPEED_H
#define NECKSPEED_H

#include <iostream>
#include <stdio.h>

#include <IceE/IceE.h>
#include <pose3dmotors.h>

#include "Singleton.h"
#include "JointControl.h"
#include "Common.h"

class NeckSpeed : public Singleton<NeckSpeed>, public jderobot::Pose3DMotors {
public:
    // Constructor
    NeckSpeed ();
        
    // Destructor
    virtual ~NeckSpeed ();
    
    // Another functions
    void init ( const std::string newName, AL::ALPtr<AL::ALBroker> parentBroker, float stiffness, float speed );
        
    /*Pose3DMotors*/
    Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& );
    jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& );
    jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& );

private:
    std::string name;
    float stiffness, speed;
    bool bPitch, bYaw, bRoll;
    AL::ALValue jointPitch, jointYaw, jointRoll;
    AL::ALPtr<AL::ALMotionProxy> motion;
    JointControl *pan, *tilt;
    
    float getTiltLimit ( float currentYaw, float currentVy );
    float target;
    
    static const float	MAX_PANU = 80.0f;
	static const float	MAX_PANL = 40.0f;
	static const float 	MAX_TILT = 45.0f;
	static const float 	MIN_TILT = -39.0f;
	static const float	MIN_PAN_ANGLE = 5.0f;
	static const float	MAX_PAN_ANGLE = 70.0f;
	static const float  MIN_TILT_ANGLE = 5.0f;
	static const float 	MAX_TILT_ANGLE = 30.0f;

	static const int    MAX_TILT_VECTOR =	25;
	static const float	MAXTILT[MAX_TILT_VECTOR];

	//Velocities are expressed in %  (18,12 is ok)
	static const float	VEL_PAN_MIN;
	static const float	VEL_PAN_MAX;		// Real robot
	static const float	VEL_TILT_MIN;
	static const float	VEL_TILT_MAX;		// Real robot
    
};
#endif // NECKSPEED_H
