#ifndef BASIC_COMPONENT_QT_SHARED_MEM
#define BASIC_COMPONENT_QT_SHARED_MEM


#include <iostream>
#include <jderobot/camera.h>
#include <pthread.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace basic_component_qt {

    class Shared {
        
    public:
        virtual ~Shared();
        
        void updateImage(jderobot::ImageDataPtr data);
        void createImage(jderobot::ImageDataPtr data);
	void createEmptyImage();
        cv::Mat getImage();	//cv::Mat
	/*
	IplImage* getImage();	//IplImage*
	*/
        
    private:
   	pthread_mutex_t controlGui;
        cv::Mat image;	//cv::Mat 
        /*
	IplImage* image; //IplImage
	*/
        
        
    };
}
#endif /* defined(____shared__) */
