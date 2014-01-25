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

#include "NaoServerCamera.h"

const string NaoServerCamera::UPPER_CAMERA_CONFIG_FILE =
        "/home/nao/bica/conf/ballPerception/cameraUpper.conf";
const string NaoServerCamera::LOWER_CAMERA_CONFIG_FILE =
        "/home/nao/bica/conf/ballPerception/cameraLower.conf";

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
NaoServerCamera::NaoServerCamera () {
    camConfUpper.createDictionary();
    camConfLower.createDictionary();

    pthread_mutex_init(&mutex, NULL);
    
    jderobot::ImageDescriptionPtr imgDescription(new jderobot::ImageDescription);
    imgDescription->width = IMG_WIDTH;
    imgDescription->height = IMG_HEIGHT;
    imgDescription->size = IMG_WIDTH * IMG_HEIGHT * IMG_CHANNELS;
    imgDescription->format = "YUV888";    
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
NaoServerCamera::~NaoServerCamera () {
    try {
        pcamera->unsubscribe(fLEMname);
    } catch (AL::ALError& e) {
        cerr << "Unable to unsubscribe Lem: " << e.toString() << endl;
    }
}

/*************************************************************
 * CAMERA
 *************************************************************/
jderobot::ImageDescriptionPtr NaoServerCamera::getImageDescription ( const Ice::Current& c ) {
	return imgDescription;
}

jderobot::ImageDataPtr NaoServerCamera::getImageData ( const Ice::Current& c ) {
/*    struct timeval a, b;
    long diff;
    long totala, totalb;
    
    gettimeofday(&a, NULL);
    totala = a.tv_sec * 1000000 + a.tv_usec;
*/
    int imgSize = IMG_WIDTH * IMG_HEIGHT * IMG_CHANNELS;
    
    jderobot::ImageDataPtr reply(new jderobot::ImageData);
    reply->description = new jderobot::ImageDescription();
    reply->description->width = IMG_WIDTH;
    reply->description->height = IMG_HEIGHT;
    reply->description->size = imgSize;
    reply->description->format = "RGB8";
    IceUtil::Time t = IceUtil::Time::now();
    reply->timeStamp.seconds = (long)t.toSeconds();
    reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
    reply->pixelData.resize(imgSize);
    
    pthread_mutex_lock(&mutex);
    
//    cout << "atrapado aqui" << endl;
    
    memcpy(&(reply->pixelData[0]), img, imgSize);
    
    pthread_mutex_unlock(&mutex);
/*
    gettimeofday(&b, NULL);
    totalb = b.tv_sec * 1000000 + b.tv_usec;
    diff = (totalb - totala) / 1000;
    
    std::cout << "getting an image consume: " << diff << std::endl;
*/
    return reply;
}

jderobot::CameraDescriptionPtr NaoServerCamera::getCameraDescription ( const Ice::Current& c ) {
	return NULL;
}

Ice::Int NaoServerCamera::setCameraDescription ( const jderobot::CameraDescriptionPtr &description, const Ice::Current& c ) {
	return 0;
}

std::string NaoServerCamera::startCameraStreaming ( const Ice::Current& ) {
	return "";
}

void NaoServerCamera::stopCameraStreaming ( const Ice::Current& ) {}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void NaoServerCamera::init ( const string newName, AL::ALPtr<AL::ALBroker> parentBroker ) {
    try {
        pcamera = new AL::ALVideoDeviceProxy(parentBroker);
        cam = pcamera->getParam(kCameraSelectID);
        fLEMname = pcamera->subscribe(string("playerGVM"), kQVGA,
//                                    kYUV422InterlacedColorSpace, 30);
                                    kRGBColorSpace, 15);
    } catch ( AL::ALError& e ) {
        cerr << "[Perception::initCamera()] " << e.toString() << endl;
    }
    
    loadCameraParams(UPPER_CAMERA_CONFIG_FILE, LOWER_CAMERA_CONFIG_FILE);
    
    try {
        pcamera->setParam(kCameraSelectID, UPPER_CAMERA);
        this->cam = cam;
    } catch (ALError& e) {
        cerr << "Unable to change camera: " << e.toString() << endl;
    }
    
    pthread_t update_thread;
    int rc;
    rc = pthread_create(&update_thread, NULL, NaoServerCamera::updateThread, NULL);

    if (rc){
        std::cout << "ERROR; return code from pthread_create() is " << rc << std::endl;
        exit(-1);
    }
}

void* NaoServerCamera::updateThread ( void* obj ) {
    NaoServerCamera* naoservercamera = NaoServerCamera::getInstance();
    naoservercamera->capture();
}

void NaoServerCamera::capture () {
    while (true) {
        AL::ALImage* newImage = NULL;
        try {
//            newImage = (AL::ALImage*) (pcamera->getDirectRawImageLocal(fLEMname));
            newImage = (AL::ALImage*) (pcamera->getImageLocal(fLEMname));
        } catch ( AL::ALError& e ) {
            cerr << "[Perception::getImageLocal()] Error: " << e.toString() << endl;
        }
/*        
        struct timeval a, b;
        long diff;
        long totala, totalb;
        
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;
*/
        pthread_mutex_lock(&mutex);
//        cout << "trapped here!" << endl;
//        this->getColorRGB((char*)(newImage->getData()), img);
        memcpy(img, (char*)(newImage->getData()), IMG_WIDTH * IMG_HEIGHT * IMG_CHANNELS);
        pthread_mutex_unlock(&mutex);
/*        
        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;
        diff = (totalb - totala) / 1000;
        
        std::cout << "updating the image consume: " << diff << std::endl;
*/
        try {
//            pcamera->releaseDirectRawImage(fLEMname);
            pcamera->releaseImage(fLEMname);
        } catch ( AL::ALError& e ) {
            cerr << "Error in releasing image: " << e.toString() << endl;
        }
    }
}

void NaoServerCamera::getColorRGB ( char* source, char* destiny ) {
    char r, g, b;
	int row, col;
	int pixel;

	for ( col = 0; col < IMG_WIDTH; col++ ) {
		for ( row = 0; row < IMG_HEIGHT; row++ ) {		
			// Get the values
			this->getValues(source, col, row, r, g, b);
			
			pixel = (row * IMG_WIDTH + col) * 3;

			destiny[pixel] = r;
			destiny[pixel+1] = g;
			destiny[pixel+2] = b;
		}		
	}
}

void NaoServerCamera::getValues ( char* image, int col, int row, char &r, char &g, char &b ) {
    unsigned char rt=0, gt=0, bt=0;
    int pixel, posSrc;

    pixel = row * IMG_WIDTH + col;
    posSrc = pixel * 2;

    /*Get real pixel color*/
    if (pixel % 2 == 0)
        this->yuv2rgb(image[posSrc], image[posSrc + 1], image[posSrc + 3], rt, gt, bt);
    else
        this->yuv2rgb(image[posSrc], image[posSrc - 1], image[posSrc + 1], rt, gt, bt);

    r = (char) bt;
    g = (char) gt;
    b = (char) rt;
}

void NaoServerCamera::yuv2rgb ( unsigned char Y, unsigned char U, unsigned char V,
                                unsigned char &R, unsigned char &G, unsigned char &B ) {
    int r = Y + ((1436 * (V - 128)) >> 10),
			g = Y - ((354 * (U - 128) + 732 * (V - 128)) >> 10),
			b = Y + ((1814 * (U - 128)) >> 10);

	if(r < 0) r = 0; else if(r > 255) r = 255;
	if(g < 0) g = 0; else if(g > 255) g = 255;
	if(b < 0) b = 0; else if(b > 255) b = 255;

	//Crop RGB
	R = (unsigned char) r;
	G = (unsigned char) g;
	B = (unsigned char) b;
}

void NaoServerCamera::loadCameraParams ( const string upper_file, const string lower_file ) {
    Dictionary *conf;
    ostringstream s;

    camConfUpper.loadFromFile(upper_file.c_str());
    camConfLower.loadFromFile(lower_file.c_str());

    int newkCameraBrightnessID, newkCameraContrastID, newkCameraSaturationID,
    newkCameraHueID, newkCameraRedChromaID, newkCameraBlueChromaID,
    newkCameraGainID, newkCameraLensXID, newkCameraLensYID,
    newkCameraAutoGainID, newkCameraAutoExpositionID,
    newkCameraAutoWhiteBalanceID;

    for (int cameraID = 1; cameraID >= 0; cameraID--) {
        if (cameraID == LOWER_CAMERA)
            conf = &camConfLower;
        else if (cameraID == UPPER_CAMERA)
            conf = &camConfUpper;
        else
            s << "[Perception::loadCameraParams()] Bad camera identifier (" << cameraID << endl;

        // Read the camera parameters
        if ((conf->getValueInt("kCameraBrightnessID", &newkCameraBrightnessID)) &&
                (conf->getValueInt("kCameraContrastID", &newkCameraContrastID)) &&
                (conf->getValueInt("kCameraSaturationID", &newkCameraSaturationID)) &&
                (conf->getValueInt("kCameraHueID", &newkCameraHueID)) &&
                (conf->getValueInt("kCameraRedChromaID", &newkCameraRedChromaID)) &&
                (conf->getValueInt("kCameraBlueChromaID", &newkCameraBlueChromaID)) &&
                (conf->getValueInt("kCameraGainID", &newkCameraGainID)) &&
                (conf->getValueInt("kCameraLensXID", &newkCameraLensXID)) &&
                (conf->getValueInt("kCameraLensYID", &newkCameraLensYID)) &&
                (conf->getValueInt("kCameraAutoGainID",	&newkCameraAutoGainID)) &&
                (conf->getValueInt("kCameraAutoExpositionID", &newkCameraAutoExpositionID)) &&
                (conf->getValueInt("kCameraAutoWhiteBalanceID", &newkCameraAutoWhiteBalanceID)))
            s << "[Perception] Loaded camera parameters for camera " << cameraID << endl;
        else
            s << "[Perception] Problem loading parameters for camera " << cameraID << endl;

        // Set the camera parameters
        try {
            pcamera->setParam(kCameraSelectID, cameraID);
            usleep(250000);

            pcamera->setParam(kCameraBrightnessID, newkCameraBrightnessID);
            pcamera->setParam(kCameraContrastID, newkCameraContrastID);
            pcamera->setParam(kCameraSaturationID, newkCameraSaturationID);
            pcamera->setParam(kCameraHueID, newkCameraHueID);
            pcamera->setParam(kCameraRedChromaID, newkCameraRedChromaID);
            pcamera->setParam(kCameraBlueChromaID, newkCameraBlueChromaID);
            pcamera->setParam(kCameraGainID, newkCameraGainID);
            pcamera->setParam(kCameraLensXID, newkCameraLensXID);
            pcamera->setParam(kCameraLensYID, newkCameraLensYID);

            // Automatic values
            pcamera->setParam(kCameraAutoGainID, newkCameraAutoGainID);
            pcamera->setParam(kCameraAutoExpositionID, newkCameraAutoExpositionID);
            pcamera->setParam(kCameraAutoWhiteBalanceID, newkCameraAutoWhiteBalanceID);
        } catch (ALError& e) {
            s << "Could not set cam param: " << e.toString() << endl;
        }
    }
}
