//
//  shared.cpp
//  
//
//  Created by Francisco Perez on 18/02/14.
//
//

#include "shared.h"
#include <cv.h>

#define w 400

namespace basic_component {

    pthread_mutex_t controlGui;

    //Using cv::Mat Images

    Shared::Shared() {
	pthread_mutex_init(&controlGui,NULL);
	guiClosed = false;
    }
    
    void Shared::setClosed(bool value) {
	pthread_mutex_lock(&controlGui);
	guiClosed = value;
	pthread_mutex_unlock(&controlGui);
    }
 
    bool Shared::isClosed() {
	return guiClosed;
    }

    void Shared::createImage(jderobot::ImageDataPtr data) {
	pthread_mutex_lock(&controlGui);
        this->image.create(data->description->height, data->description->width, CV_8UC3);
	pthread_mutex_unlock(&controlGui);
    }
    
    void Shared::updateImage(jderobot::ImageDataPtr data){
	pthread_mutex_lock(&controlGui);
        memcpy((unsigned char *) this->image.data ,&(data->pixelData[0]), this->image.cols*image.rows * 3);
	pthread_mutex_unlock(&controlGui);
    }
    
    void Shared::createEmptyImage() {
	pthread_mutex_lock(&controlGui);
	this->image.create(w, w, CV_8UC3);
	pthread_mutex_unlock(&controlGui);
    }
    cv::Mat Shared::getImage() {
	pthread_mutex_lock(&controlGui);
        cv::Mat result = this->image.clone();
	pthread_mutex_unlock(&controlGui);
        return result;
    }

/*
    //Using IplImages

    void Shared::createImage(jderobot::ImageDataPtr data) {
        mutex.lock();
        image = cvCreateImage(cvSize(data->description->width,data->description->height), 8 ,3);
	memcpy((unsigned char *) image->imageData,&(data->pixelData[0]),image->width*image->height * 3);
        mutex.unlock();
    }
    
    void Shared::updateImage(jderobot::ImageDataPtr data){
        mutex.lock();
        memcpy((unsigned char *) image->imageData,&(data->pixelData[0]),image->width*image->height * 3);
        mutex.unlock();
    }
   
    IplImage* Shared::getImage() {
        mutex.lock();
        IplImage* result = cvCreateImage(cvSize(image->width,image->height), 8 ,3); 
	cvCopy(image, result);
        mutex.unlock();
        return result;
    }
*/
    
    Shared::~Shared() {};
}
