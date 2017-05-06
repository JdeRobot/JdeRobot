#ifndef ENCODERSCLIENT_H
#define ENCODERSCLIENT_H

#include "Ros_Ice.h"
#include <RosIceMessage/EncodersData.h>
//#include "EncodersData.h"
#include <encoders.h>


class EncodersClient : public Ros_Ice < jderobot::EncodersPrx >
{

public:

    /**
     * @brief This constructor initializes the ROS topic using the nodeName parameter
     */

    EncodersClient(int argc, char **argv, std::string nodeName);

    ~EncodersClient();

    /**
     * @brief Method to get the Ice data from the JdeRobot simulator and publish it to the ROS topic
     */


    RosIceMessage::EncodersData publishROS();

    //void rosCallback(RosIceMessage::EncodersData encodersMessage);






};

#endif // ENCODERSCLIENT_H
