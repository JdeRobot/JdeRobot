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
#include "alcore/alptr.h"
#include "alcommon/albroker.h"
#include "alcommon/almodule.h"
#include "alcommon/albrokermanager.h"
#include "alcommon/altoolsmain.h"

#include "alcore/alptr.h"
#include "alproxies/alledsproxy.h"
#include "alproxies/almemoryproxy.h"
#include "alproxies/alsensorsproxy.h"
#include "alproxies/alsonarproxy.h"
#include "alproxies/alrobotposeproxy.h"
#include "alcommon/alproxy.h"
#include "alcommon/albroker.h"

#include "Singleton.h"
#include "ImageInput.h"
#include "vision/colorFilter/SimpleColorFilter.h"
#include "ImageHandler.h"

#include <IceE/IceE.h>
#include <camera.h>

#include "opencv/cv.h"
#include "opencv/highgui.h"

using namespace std;
using namespace jderobot;

namespace AL
{
class ALBroker;
}
using namespace AL;

class NaoServerCamera : public Singleton<NaoServerCamera> {
public:
    // Constructor
    NaoServerCamera ();
    
    // Destructor
    ~NaoServerCamera ();

    void init ( const string newName, AL::ALPtr<AL::ALBroker> parentBroker );

	/*Camera*/
    jderobot::ImageDataPtr getImageData ();
    
    ImageHandler *getImageHandler() {return _ImageHandler;};

	inline SimpleColorFilter* getColorFilter() { return this->colorFilter; };
	inline int getCam() { return cam; };
	inline void lock() { pthread_mutex_lock(&mutex); };
	inline void unlock() { pthread_mutex_unlock(&mutex); };
	void setCam(const int cam);

	// External methods
	int getCamParam(const int param);

	virtual void setCam (CameraType cam, const Ice::Current& c);
	
	long getImageTs() {return image_ts;};

private:
    void initCamera();
    bool newImage();
    void loadCameraParams(string upper_file, string lower_file);

    static const string UPPER_CAMERA_CONFIG_FILE;
    static const string LOWER_CAMERA_CONFIG_FILE;

    int cam;
    bool camera_initialized;
    string fLEMname;
    AL::ALValue imgDebug, filterParams;
    char * colorSrc;
    IplImage *cvImage, *cvAux;
    pthread_mutex_t mutex;
    Dictionary	camConfUpper, camConfLower;
    SimpleColorFilter *colorFilter;

    bool imageTaken;

    AL::ALVideoDeviceProxy *pcamera;
    jderobot::ImageDescriptionPtr imgDescription;

    ImageHandler * _ImageHandler;

    long long lastTimeStamp;
    long wtime;

    //capture thread
	pthread_t capture_thread;
    bool newImageTaken;
    char imgBuff[ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS_YUV];
    long image_ts;

protected:
    static void* EntryPoint(void*);
    void Capture();
};
#endif
