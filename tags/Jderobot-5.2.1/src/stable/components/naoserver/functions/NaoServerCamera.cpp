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
    colorFilter = SimpleColorFilter::getInstance();
    _ImageHandler = ImageHandler::getInstance();

    camera_initialized = false;

    camConfUpper.createDictionary();
    camConfLower.createDictionary();

    pthread_mutex_init(&mutex, NULL);

    this->imgDebug.arrayReserve(ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS * sizeof(char));
    this->colorSrc = new char[ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS];

    jderobot::ImageDescriptionPtr imgDescription(new jderobot::ImageDescription);
    imgDescription->width = ImageInput::IMG_WIDTH;
    imgDescription->height = ImageInput::IMG_HEIGHT;
    imgDescription->size = ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS;
    imgDescription->format = "YUV888";

    newImageTaken = false;

    struct timeval current;
    long current_m;

    gettimeofday(&current, NULL);
    current_m = current.tv_sec * 1000000 + current.tv_usec;

    lastTimeStamp = current_m;//newImage->fTimeStamp;
    wtime = 0;
}

void* NaoServerCamera::EntryPoint ( void* pthis ) {
    NaoServerCamera *pt = NaoServerCamera::getInstance();
    pt->Capture();
}

void NaoServerCamera::Capture () {
    struct timeval s1;

    while (true) {
        AL::ALImage* newImage = NULL;
        try {
            newImage = (AL::ALImage*) (pcamera->getDirectRawImageLocal(fLEMname));
        } catch ( AL::ALError& e ) {
            cerr << "[Perception::getImageLocal()] Error: " << e.toString() << endl;
        }

        pthread_mutex_lock(&mutex);
        memcpy(imgBuff, (char*)(newImage->getData()), ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS_YUV);

        pthread_mutex_unlock(&mutex);

        gettimeofday(&s1, NULL);
        newImageTaken = true;
        image_ts = s1.tv_sec * 1000000 + s1.tv_usec;

        try {
            pcamera->releaseDirectRawImage(fLEMname);
        } catch ( AL::ALError& e ) {
            cerr << "Error in releasing image: " << e.toString() << endl;
        }
    }
}

NaoServerCamera::~NaoServerCamera () {
    try {
        pcamera->unsubscribe(fLEMname);
    } catch (AL::ALError& e) {
        cerr << "Unable to unsubscribe Lem: " << e.toString() << endl;
    }

    cvReleaseImage(&cvImage);
    cvReleaseImage(&cvAux);

    delete this->colorSrc;
}

void NaoServerCamera::init ( const string newName, AL::ALPtr<AL::ALBroker> parentBroker ) {
    try {
        pcamera = new AL::ALVideoDeviceProxy(parentBroker);
        cam = pcamera->getParam(kCameraSelectID);
        fLEMname = pcamera->subscribe(string("GVM"), kQQVGA,
                                    kYUV422InterlacedColorSpace, 30);
//        fLEMname = pcamera->subscribe(string("playerGVM"), kQVGA,
//                                    kYUV422InterlacedColorSpace, 30);
    } catch ( AL::ALError& e ) {
        cerr << "[Perception::initCamera()] " << e.toString() << endl;
    }

    filterParams.arraySetSize(6);

    cvImage = cvCreateImage(cvSize(ImageInput::IMG_WIDTH, ImageInput::IMG_HEIGHT), ImageInput::BYTES_PER_CHANNEL, 2);
    cvAux = cvCreateImage(cvSize(ImageInput::IMG_WIDTH, ImageInput::IMG_HEIGHT), ImageInput::BYTES_PER_CHANNEL, ImageInput::IMG_CHANNELS);

    camera_initialized = true;

    for (int i = 0; i < 2; i++)
        loadCameraParams(UPPER_CAMERA_CONFIG_FILE, LOWER_CAMERA_CONFIG_FILE);

    setCam(ImageInput::UPPER_CAMERA);
	
    int rc;
    rc =  pthread_create(&capture_thread, NULL, NaoServerCamera::EntryPoint, this);

    if (rc){
        std::cout << "ERROR; return code from pthread_create() is " << rc << std::endl;
        exit(-1);
    }
}

/*Camera parameters*/
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
        if (cameraID == ImageInput::LOWER_CAMERA)
            conf = &camConfLower;
        else if (cameraID == ImageInput::UPPER_CAMERA)
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

void NaoServerCamera::setCam(const int cam) {
//    if (!camera_initialized)
//        initCamera();

    try {
        pcamera->setParam(kCameraSelectID, cam);
        this->cam = cam;
    } catch (ALError& e) {
        cerr << "Unable to change camera: " << e.toString() << endl;
    }
}

jderobot::ImageDataPtr NaoServerCamera::getImageData () {
    jderobot::ImageDataPtr reply(new jderobot::ImageData);
    reply->description = new jderobot::ImageDescription();
    reply->description->width = ImageInput::IMG_WIDTH;
    reply->description->height = ImageInput::IMG_HEIGHT;
    reply->description->size = ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS;
    reply->description->format = "RGB8";
	
    int imgSize = ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS * sizeof(char);

    IceUtil::Time t = IceUtil::Time::now();
    reply->timeStamp.seconds = (long)t.toSeconds();
    reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
    reply->pixelData.resize(imgSize);

    IplImage* src;

    this->newImage();
    src = cvCreateImage(cvSize(ImageInput::IMG_WIDTH,ImageInput::IMG_HEIGHT),
				ImageInput::BYTES_PER_CHANNEL, ImageInput::IMG_CHANNELS);

    _ImageHandler->getImageRGBInverted(src->imageData, false, 0);
    
    memmove( &(reply->pixelData[0]), (char *) src->imageData, imgSize);//copy data to reply

    cvReleaseImage(&src);

    return reply;
}

bool NaoServerCamera::newImage () {
    if (newImageTaken) {
        long end, start;
        struct timeval s1;

        pthread_mutex_lock(&mutex);

        gettimeofday(&s1, NULL);
        start = s1.tv_sec * 1000000 + s1.tv_usec;
        _ImageHandler->setImage(cam, imgBuff, colorFilter);
        gettimeofday(&s1, NULL);
        end = s1.tv_sec * 1000000 + s1.tv_usec;

        pthread_mutex_unlock(&mutex);

        newImageTaken = false;

        wtime = (end-start);
        return true;
    } else {
        return false;
    }
}
