/*
 *  Copyright (C) 1997-2011 JDERobot Developers Team
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Authors : Maikel González <m.gonzalezbai@gmail.com>,
 *
 */


#ifndef BASIC_COMPONENT_CONTROL_H
#define BASIC_COMPONENT_CONTROL_H


#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <jderobot/motors.h>
#include <jderobot/ptmotors.h>
#include <jderobot/laser.h>
#include <jderobot/encoders.h>
#include <jderobot/ptencoders.h>
#include <colorspaces/colorspacesmm.h>
#include <pthread.h>
#include "API.h"

namespace basic_component {

    class Control {
    public:
        virtual ~Control();

        //FUNCTIONS 
        void updateCameras(Api *api);
        void UpdateSensorsICE(Api *api);
        void SetActuatorsICE(Api *api);

        // ICE INTERFACES
        jderobot::MotorsPrx mprx;
        jderobot::EncodersPrx eprx;
        jderobot::LaserPrx lprx;
        jderobot::CameraPrx cprx1;
        jderobot::CameraPrx cprx2;
        jderobot::PTMotorsPrx ptmprx1;
        jderobot::PTEncodersPrx pteprx1;
        jderobot::PTMotorsPrx ptmprx2;
        jderobot::PTEncodersPrx pteprx2;

    }; //class
} // namespace
#endif /*BASIC_COMPONENT_Control_H*/
