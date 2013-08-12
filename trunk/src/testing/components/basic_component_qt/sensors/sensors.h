#ifndef SENSORS_H
#define SENSORS_H

//Qt
#include <QtGui>

//ICE
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

//INTERFACES
#include <jderobot/camera.h>

//Opencv
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class Sensors
{
public:
    Sensors(Ice::CommunicatorPtr ic);

    void update();
    cv::Mat getImage();

private:
    QMutex mutex;

    Ice::CommunicatorPtr ic;

    jderobot::CameraPrx cprx;

    cv::Mat image;

};

#endif // SENSORS_H
