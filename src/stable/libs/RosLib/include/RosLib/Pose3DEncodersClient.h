#ifndef POSE3DENCODERSCLIENT_H
#define POSE3DENCODERSCLIENT_H

#include "Ros_Ice.h"
#include <jderobot/pose3dencoders.h>
#include <RosIceMessage/Pose3DEncodersData.h>
//#include "Pose3DEncodersData.h"


class Pose3DEncodersClient : public Ros_Ice < jderobot::Pose3DEncodersPrx >
{

public:

    /**
     * @brief This constructor initializes the ROS topic using the nodeName parameter
     */

    Pose3DEncodersClient(int argc, char **argv, std::string nodeName);
    ~Pose3DEncodersClient();

    /**
     * @brief Method to get the Ice data from the JdeRobot simulator and publish it to the ROS topic
     */

    void publishROS();





};

#endif // POSE3DENCODERSCLIENT_H
