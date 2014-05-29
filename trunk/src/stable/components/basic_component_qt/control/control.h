#ifndef CONTROL_H
#define CONTROL_H

//Qt


//ICE
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

//INTERFACES
#include <jderobot/camera.h>

//Opencv
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/algorithm/string.hpp>
#include "../shared.h"

namespace basic_component_qt {

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

#endif // CONTROL_H
