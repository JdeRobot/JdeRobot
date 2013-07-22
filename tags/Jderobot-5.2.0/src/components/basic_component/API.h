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


#ifndef BASIC_COMPONENT_API_H
#define BASIC_COMPONENT_API_H


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



namespace basic_component {
    class Api {
	public:
	virtual ~Api();
	
	    pthread_mutex_t controlGui;	
	
	
	    //FUNCTIONS 
	    void RunNavigationAlgorithm();

	    // GETS	    
	    double getMotorV();
	    double getMotorW();
	    double getMotorL();
	    jderobot::LaserDataPtr getLaserData();
	    int getNumLasers();
	    jderobot::IntSeq getDistancesLaser();
	    jderobot::EncodersDataPtr getEncodersData();
	    colorspaces::Image* getImageCamera1();
	    colorspaces::Image* getImageCamera2();

	    //SETS	    
	    void setMotorV(float motorV);
	    void setMotorW(float motorW);
	    void setMotorL(float motorL);
	    void setPTEncoders(jderobot::PTEncodersDataPtr* PTencodersData, int cameraId);
        	    
	    /*Shared Memory -- ignored by students*/
	    double motorVout;
	    double motorWout;
	    double motorLout;
	    double motorVin;
	    double motorWin;
	    double motorLin;
	    double v_normalized; // Used to teleoperate cameras (latitude)
	    double w_normalized; // Used to teleoperate cameras (longitude)
	    jderobot::EncodersDataPtr encodersData;
	    jderobot::LaserDataPtr laserData;
	    jderobot::ImageDataPtr imageData1; // Contains the image info
	    jderobot::ImageDataPtr imageData2; // Contains the image info
	    jderobot::PTEncodersDataPtr PTencodersData1;
	    jderobot::PTEncodersDataPtr PTecondersData2;
            jderobot::PTMotorsData* PTmotorsData1;
            jderobot::PTMotorsData* PTmotorsData2;
	    colorspaces::Image* image1;	// Image camera1 processed to manipulate with openCV
	    colorspaces::Image* image2; // Image camera2 processed to manipulate with openCV
	    bool guiVisible;
	    bool iterationControlActivated;
	    //Variables used in NavigationAlgorithm
	    int sentido; 
	    int accion;
	    
	private:
	    //Variables used in NavigationAlgorithm
	    colorspaces::Image* imageCamera1;
	    colorspaces::Image* imageCamera2;
	    
	    //Functions used in NavigationAlgorithm
   	    void imageCameras2openCV();	    

	    
     
    };//class
} // namespace
#endif /*BASIC_COMPONENT_Control_H*/
