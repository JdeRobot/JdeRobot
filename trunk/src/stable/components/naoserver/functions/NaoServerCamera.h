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

#ifndef NAOSERVERCAMERA_H
#define NAOSERVERCAMERA_H

#include "alvision/alvisiondefinitions.h"
#include "alproxies/alvideodeviceproxy.h"
#include "alvision/alimage.h"

#include "alcore/alptr.h"
#include "alproxies/alledsproxy.h"
#include "alproxies/almemoryproxy.h"
#include "alproxies/alsensorsproxy.h"
#include "alproxies/alsonarproxy.h"
#include "alproxies/alrobotposeproxy.h"
#include "alproxies/almotionproxy.h"
#include "alcommon/alproxy.h"
#include "alcommon/albroker.h"
#include "alcommon/almodule.h"

#include "alcore/altypes.h"
#include "altools/alxplatform.h"
#include "alcommon/albrokermanager.h"
#include "alcommon/altoolsmain.h"

#include <IceE/IceE.h>
#include <camera.h>

#include <iostream>
#include <string>

#include "ImageConstants.h"
#include "Singleton.h"
#include "Dictionary.h"

#include "opencv/cv.h"
#include "opencv/highgui.h"

using namespace std;

namespace AL
{
class ALBroker;
}
using namespace AL;

class NaoServerCamera : public Singleton<NaoServerCamera>, public jderobot::Camera {
public:
    // Constructor
    NaoServerCamera ();
    
    // Destructor
    virtual ~NaoServerCamera ();
    
    /*Camera*/
    jderobot::ImageDescriptionPtr getImageDescription ( const Ice::Current& c );
    jderobot::ImageDataPtr getImageData ( const Ice::Current& c );
    jderobot::CameraDescriptionPtr getCameraDescription ( const Ice::Current& c );
    Ice::Int setCameraDescription ( const jderobot::CameraDescriptionPtr &description, const Ice::Current& c );
    std::string startCameraStreaming ( const Ice::Current& );
    void stopCameraStreaming ( const Ice::Current& );
    
    // Another functions
    void init ( const string newName, AL::ALPtr<AL::ALBroker> parentBroker );
    void update ();
    
private:
    AL::ALVideoDeviceProxy* pcamera;
    jderobot::ImageDescriptionPtr imgDescription;
    
    int cam;
    
    pthread_mutex_t mutex;
    Dictionary camConfUpper, camConfLower;
    
    static const string UPPER_CAMERA_CONFIG_FILE;
    static const string LOWER_CAMERA_CONFIG_FILE;
    std::string fLEMname;

protected:
    char img[IMG_WIDTH * IMG_HEIGHT * IMG_CHANNELS];
    
    static void* updateThread ( void* obj );
    void capture ();
    
    void getColorRGB ( char* source, char* destiny );
    void getValues ( char* image, int col, int row, char &r, char &g, char &b );
    void yuv2rgb ( unsigned char Y, unsigned char U, unsigned char V, unsigned char &R, unsigned char &G, unsigned char &B );
    void loadCameraParams ( const string upper_file, const string lower_file );
};

#endif // NAOSERVERCAMERA_H
