#ifndef TELEOPERATOR_H
#define TELEOPERATOR_H

#include <iostream>
#include "ros/ros.h"
#include <RosIceMessage/MotorData.h>
#include <geometry_msgs/Twist.h>




class TeleOperator
{

private:

    ros::NodeHandle* RosNode;
    ros::Publisher*  RosPublisher;
    ros::Subscriber* RosSubscriber;


public:

    /**
     * @brief This constructor initializes the ROS topic using the nodeName parameter
     */

    TeleOperator(int argc, char **argv, std::string nodeName);



    virtual ~TeleOperator();

    /**
     * @brief This callback method takes the data from Rviz and sends it to the MotorClient class
     */

    void rosCallback(geometry_msgs::Twist twistMessage);







};

#endif // TELEOPERATOR_H
