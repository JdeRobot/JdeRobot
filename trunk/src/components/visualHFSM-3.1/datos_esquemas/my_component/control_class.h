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


#ifndef MYCOMPONENT_CONTROL_H
#define MYCOMPONENT_CONTROL_H


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
#include <pthread.h> //OBJETIVO2

namespace mycomponent {
    class Gui;
    class Control {
	public:
	    //Control (int argc, char** argv);
	virtual ~Control();
	
	    //FUNCTIONS
	    void iterationControl(int th_id);
	    void handleCameras();
	    void resetControl();
	    
	    //HANDLE THREADS
	    pthread_t thr_gui;//OBJETIVO2
		pthread_t thr_control_1;
		pthread_t thr_control_2;//Hilos jerarquicos
		pthread_t thr_control_3;
	    pthread_mutex_t controlGui;//OBJETIVO2

	
	    // INTERFACES DATA
	    jderobot::EncodersDataPtr ed;
	    jderobot::LaserDataPtr ld;
	    jderobot::ImageDataPtr data1; // Contains the image info
	    jderobot::ImageDataPtr data2; // Contains the image info
	    jderobot::PTEncodersDataPtr pted1;
	    jderobot::PTEncodersDataPtr pted2;
    
	    // INTERFACES
	    jderobot::MotorsPrx mprx;
	    jderobot::EncodersPrx eprx;
	    jderobot::LaserPrx lprx;
	    jderobot::CameraPrx cprx1;
	    jderobot::CameraPrx cprx2;
	    jderobot::PTMotorsPrx ptmprx1;
	    jderobot::PTEncodersPrx pteprx1;
	    jderobot::PTMotorsPrx ptmprx2;
	    jderobot::PTEncodersPrx pteprx2;
       
	    colorspaces::Image* image1;	// Prepare the image to use with openCV
	    colorspaces::Image* image2;
     
    };//class
} // namespace
#endif /*MYCOMPONENT_Control_H*/
