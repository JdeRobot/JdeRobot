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

#ifndef SENSORS_H
#define SENSORS_H

#include <iostream>
#include <stdio.h>

#include <fstream>
#include <sstream>

#ifdef COMPILEFORNAO
// ICE
#include <IceE/IceE.h>
// OpenCV
#include <opencv/cv.h>
#include <opencv/cv.hpp>
// Interfaces
#include <camera.h>
#else
// ICE
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// Interfaces
#include <jderobot/camera.h>
#endif

class Sensors {
public:
    // Constructor
    Sensors ( Ice::CommunicatorPtr ic );
    
    // Destructor
    virtual ~Sensors ();
    
    // Getters
    float getErrorX ();
    float getErrorY ();
    cv::Mat& getImage ();
    
    int getRH ();
    int getGS ();
    int getBV ();
    
    // Another functions
    void update ();
    
private:
    Ice::CommunicatorPtr ic;
    jderobot::CameraPrx cameraprx;
    cv::Mat image;
    float errorX, errorY;
    int valueRH, valueGS, valueBV;
    int minValueRH, minValueGS, minValueBV, maxValueRH, maxValueGS, maxValueBV;
    struct timeval a;
    int filter; // 0: RGB, 1: HSV
    
    void updateValues ();
    void RGBtoHSV ( double r, double g, double b, double *h, double *s, double *v );
    
    pthread_mutex_t mutex;
};

#endif // SENSORS_H
