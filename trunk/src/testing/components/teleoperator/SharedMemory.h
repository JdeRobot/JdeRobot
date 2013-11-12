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
#include "Gui.h"

#ifndef SHAREDMEMORY_H
#define	SHAREDMEMORY_H

void WriteLogFile(const char* szString);

class SharedMemory {
public:
    SharedMemory();
    SharedMemory(const SharedMemory& orig);
    virtual ~SharedMemory();

    pthread_mutex_t imagesData_mutex;

    typedef struct motorsData_t {
        double v;
        double w;
        double l;
    } motorsData_t;

    typedef struct pose3DMotorsData_t {
        float pan;
        float tilt;
    } pose3DMotorsData_t;

    jderobot::EncodersDataPtr encodersDataReceived;
    jderobot::LaserDataPtr laserDataReceived;
    jderobot::ImageDataPtr imageDataLeftReceived; // Contains the image info
    jderobot::ImageDataPtr imageDataRightReceived; // Contains the image info  
    jderobot::Pose3DEncodersDataPtr Pose3DEncodersDataReceivedLeft;
    jderobot::Pose3DEncodersDataPtr Pose3DEncodersDataReceivedRight;
    jderobot::Pose3DMotorsData* Pose3DMotorsDataRight;
    jderobot::Pose3DMotorsData* Pose3DMotorsDataLeft;
    pose3DMotorsData_t Pose3DMotorsDataToSendRight;
    pose3DMotorsData_t Pose3DMotorsDataToSendLeft;
    colorspaces::Image imageLeft2display;
    colorspaces::Image imageRight2display;
    bool imagesReady;
    bool laserReady;
    bool encodersReady;
    motorsData_t motorsDataToSend;
    motorsData_t motorsDataReceived;
    Glib::RefPtr<Gdk::Pixbuf> imgBuff2, imgBuff;
    std::string configIce;

    typedef struct ice_t {
        bool checkInit;
        bool activated;
        bool checkEnd;
    } ice_t;

    ice_t laserInterface, pose3DEncodersInterface, pose3DMotorsInterface, camerasInterface, motorsInterface, encodersInterface;

    int valor;
    bool exit;

private:

};

#endif	/* SHAREDMEMORY_H */

