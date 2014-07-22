#ifndef SENSORS_H
#define SENSORS_H

//ICE
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include "../shared.h"

//INTERFACES
#include <jderobot/camera.h>

//Opencv
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/algorithm/string.hpp>

namespace basic_component {

class Control
{
public:

    Control(Ice::CommunicatorPtr ic, basic_component::Shared* sm);	//constructor
    void update();					

private:

    Shared* sm;	//Shared memory

    Ice::CommunicatorPtr ic;
    jderobot::CameraPrx cprx;
};
}

#endif // SENSORS_H
