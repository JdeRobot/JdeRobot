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

#ifndef CONTROL_H
#define CONTROL_H

#include <iostream>
#include <stdio.h>
#include <Ice/Ice.h>
#include <jderobot/motors.h>
#include <jderobot/pose3dmotors.h>
#include <jderobot/pose3dencoders.h>
#include <jderobot/naomotions.h>
#include <jderobot/camera.h>
#include <math.h>
#include <visionlib/colorspaces/colorspacesmm.h>

#include "common.h"

class Control {
public:
    // Constructor
    Control ();
    
    // Destructor
    virtual ~Control ();
    
    // Setters
    void setArgs ( int argc, char** argv );
    void setMovement ( Motion m, float yaw_v, float pitch_w, float roll_l );
    void setAction ( Action a );
    void setDirectMovement ( bool b );
    void setDirectHead ( bool b );
    
    // Getters
    int getArgc ();
    char** getArgv ();
    
    jderobot::ImageDataPtr getImage ();
    
    // Another functions
    int connect ();
    
private:
    std::map<Motion, jderobot::Pose3DMotorsPrx> mapMotors;
    jderobot::MotorsPrx walk;
    jderobot::NaoMotionsPrx motions;
    jderobot::CameraPrx cameraprx;
    
    Ice::CommunicatorPtr ic;
    
    float head_tilt, head_pan;
    float motors_v, motors_w, motors_l;
    
    bool directMovement, directHead;
    
    int argc;
    char** argv;
    
    float normalizeMe ( float number, float negBound, float posBound );
}; // Class Control

#endif // CONTROL_H
