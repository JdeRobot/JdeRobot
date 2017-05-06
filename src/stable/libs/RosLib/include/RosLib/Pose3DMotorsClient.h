#ifndef POSE3DMOTORSCLIENT_H
#define POSE3DMOTORSCLIENT_H

#include "Ros_Ice.h"
#include <jderobot/pose3dmotors.h>
#include <RosIceMessage/Pose3DMotorsData.h>
//#include "Pose3DMotorsData.h"
#include <visionlib/colorspaces/colorspacesmm.h>


class Pose3DMotorsClient : public Ros_Ice < jderobot::Pose3DMotorsPrx >
{

public:

    /**
     * @brief This constructor initializes the ROS topic using the nodeName parameter
     */

    Pose3DMotorsClient(int argc, char **argv, std::string nodeName);
    ~Pose3DMotorsClient();


    void rosCallback(RosIceMessage::Pose3DMotorsData pose3DMotorData);




};

#endif // POSE3DMOTORSCLIENT_H
