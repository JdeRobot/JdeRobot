 /*
  *  Copyright (C) 1997-2015 JDE Developers Team
  *
  *  This program is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program.  If not, see http://www.gnu.org/licenses/.
  *
  *  Author : Lihang Li<licalmer@gmail.com>
 
  */

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <pthread.h>
#include <jderobot/camera.h>
#include <IceUtil/Thread.h>
#include "parallelIce/cameraClient.h"

jderobot::cameraClient* camRGB=NULL;
jderobot::cameraClient* camDEPTH=NULL;

int main(int argc, char** argv)
{
    int status;
    Ice::CommunicatorPtr ic;
    Ice::PropertiesPtr prop;

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    try {
        ic = Ice::initialize(argc, argv);
        prop = ic->getProperties();
    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        return 1;
    } catch (const char* msg) {
        std::cerr <<"Error:" << msg << std::endl;
        return 1;
    }

    if (prop->getPropertyAsIntWithDefault("rtabmap.CameraRGBActive", 0)) {
        camRGB = new jderobot::cameraClient(ic, "rtabmap.CameraRGB.");
        if (camRGB != NULL) {
            std::cout << "rtabmap: RGB Camera loaded." << std::endl;
        }
        else {
            throw "rtabmap: faild to load RGB Camera.";
        }
    }

    if (prop->getPropertyAsIntWithDefault("rtabmap.CameraDEPTHActive", 0)) {
        camDEPTH = new jderobot::cameraClient(ic, "rtabmap.CameraDEPTH.");
        if (camDEPTH != NULL) {
            std::cout << "rtabmap: DEPTH Camera loaded." << std::endl;
        }
        else {
            throw "rtabmap: failed to load DEPTH Camera.";
        }
    }

    if (camRGB != NULL) {
        delete camRGB;
    }

    if (camDEPTH != NULL) {
        delete camDEPTH;
    }

    std::cout << "final" << std::endl;

    if (ic) {
        ic->destroy();
    }

    return status;
}
