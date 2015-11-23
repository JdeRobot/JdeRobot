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
 *              José María Cañas <jmplaza@gsyc.es>
 *
 */

#ifndef CONTROLICE_H
#define	CONTROLICE_H

#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/motors.h>
#include <jderobot/encoders.h>
#include <jderobot/laser.h>
#include <jderobot/camera.h>
#include <jderobot/pose3dmotors.h>
#include <jderobot/pose3dencoders.h>
#include <visionlib/colorspaces/colorspacesmm.h>
#include "SharedMemory.h"
#include <opencv2/core/core.hpp>

class controlICE {
public:
    controlICE(SharedMemory *interfacesData);
    controlICE(const controlICE& orig);
    virtual ~controlICE();

    void getData();
    void sendData();
    void initLaser();
    void endLaser();
    void initPose3DMotors();
    void endPose3DMotors();
    void initPose3DEncoders();
    void endPose3DEncoders();
    void initEncoders();
    void endEncoders();
    void initMotors();
    void endMotors();
    void initCameras();
    void endCameras();
    void checkInterfaces();

private:

    void createImage1();
    void createImage2();


    SharedMemory *interfacesData;

    Ice::CommunicatorPtr icLaser;
    Ice::CommunicatorPtr icPose3DEncoders;
    Ice::CommunicatorPtr icPose3DMotors;
    Ice::CommunicatorPtr icCameras;
    Ice::CommunicatorPtr icMotors;
    Ice::CommunicatorPtr icEncoders;

    jderobot::LaserPrx lprx;
    jderobot::CameraPrx cprxLeft;
    jderobot::CameraPrx cprxRight;
    jderobot::Pose3DEncodersPrx p3deprxRight;
    jderobot::Pose3DEncodersPrx p3deprxLeft;
    jderobot::Pose3DMotorsPrx p3dmprxRight;
    jderobot::Pose3DMotorsPrx p3dmprxLeft;
    jderobot::MotorsPrx mprx;
    jderobot::EncodersPrx eprx;

    IplImage* image;
    IplImage* image2;
};

#endif	/* CONTROLICE_H */

