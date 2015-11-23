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

#ifdef COMPILEFORNAO
// ICE
#include <IceE/IceE.h>
// Interfaces
#include <pose3dmotors.h>
#else
// ICE
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
// Interfaces
#include <jderobot/pose3dmotors.h>
#endif

class Control {
public:
    // Constructor
    Control ( Ice::CommunicatorPtr ic );
    
    // Destructor
    virtual ~Control ();
    
    // Another functions
    void sendValues ( float panSpeed, float tiltSpeed );

private:
    Ice::CommunicatorPtr ic;
    jderobot::Pose3DMotorsPrx motorsprx;
};

#endif // CONTROL_H
