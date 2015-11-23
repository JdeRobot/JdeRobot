#ifndef SENSORS_H
#define SENSORS_H

//ICE
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

//INTERFACES
#include <jderobot/camera.h>
#include <jderobot/pose3dmotors.h>

//Opencv
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class Sensors {
public:
    Sensors(jderobot::ImageDataPtr data);

    void update ( jderobot::ImageDataPtr data );
    cv::Mat getImage ();
    
    float getPanSpeed ();
    float getTiltSpeed ();

private:
    float panSpeed, tiltSpeed;
    
    cv::Mat image;
};

#endif // SENSORS_H
