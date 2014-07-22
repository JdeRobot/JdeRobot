#include "shared.h"
#include <cv.h>

#define w 400

namespace basic_component_qt {

    //Using cv::Mat Images
    
    void Shared::createImage(jderobot::ImageDataPtr data) {
	pthread_mutex_lock(&this->controlGui);
        image.create(data->description->height, data->description->width, CV_8UC3);
	pthread_mutex_unlock(&this->controlGui);
    }
    
    void Shared::updateImage(jderobot::ImageDataPtr data){
	pthread_mutex_lock(&this->controlGui);
        memcpy((unsigned char *) image.data ,&(data->pixelData[0]), image.cols*image.rows * 3);
	pthread_mutex_unlock(&this->controlGui);
    }
    
    void Shared::createEmptyImage() {
	pthread_mutex_lock(&this->controlGui);
	image.create(w, w, CV_8UC3);
	pthread_mutex_unlock(&this->controlGui);
    }
    cv::Mat Shared::getImage() {
	pthread_mutex_lock(&this->controlGui);
        cv::Mat result = image.clone();
	pthread_mutex_unlock(&this->controlGui);
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
