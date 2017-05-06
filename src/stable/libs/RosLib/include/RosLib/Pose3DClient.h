#ifndef POSE3DCLIENT_H
#define POSE3DCLIENT_H

#include "Ros_Ice.h"
#include <pose3d.h>
#include <geometry_msgs/Pose.h>
#include <RosIceMessage/Pose3D.h>
//#include "Pose3D.h"


class Pose3DClient : public Ros_Ice < jderobot::Pose3DPrx >
{

public:

    /**
     * @brief This constructor initializes the ROS topic using the nodeName parameter
     */

    Pose3DClient(int argc, char **argv, std::string nodeName);
    ~Pose3DClient();

    /**
     * @brief Method to get the Ice data from the JdeRobot simulator and publish it to the ROS topic
     */

    geometry_msgs::Pose publishROS();







};

#endif // POSE3DCLIENT_H
