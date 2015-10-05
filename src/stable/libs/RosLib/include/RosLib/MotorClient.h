#ifndef MOTORCLIENT_H
#define MOTORCLIENT_H

#include "Ros_Ice.h"
#include <jderobot/motors.h>
#include <RosIceMessage/MotorData.h>
//#include "MotorData.h"
#include <visionlib/colorspaces/colorspacesmm.h>


class MotorClient : public Ros_Ice < jderobot::MotorsPrx >
{

public:

    /**
     * @brief This constructor initializes the ROS topic using the nodeName parameter
     */

    MotorClient(int argc, char **argv, std::string nodeName);

    ~MotorClient();

    /**
     * @brief This callback method receives data from TeleOperator class and sends it to the JdeRobot Gazebo simulator using the inherited Ice Proxy
     */

    void rosCallback(RosIceMessage::MotorData motorData);




};

#endif // MOTORCLIENT_H
