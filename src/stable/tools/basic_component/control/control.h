#ifndef SENSORS_H
#define SENSORS_H

//Qt


//ICE
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <visionlib/colorspaces/colorspacesmm.h>

//INTERFACES
#include <jderobot/camera.h>

//Opencv
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/algorithm/string.hpp>
#include "../shared.h"

namespace basic_component {

class Control
{
public:

    Control(Ice::CommunicatorPtr ic, Shared* sm);	//constructor
    void update();					

private:

    Shared* sm;

    Ice::CommunicatorPtr ic;
    jderobot::CameraPrx cprx;
};
}

#endif // SENSORS_H
