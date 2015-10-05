#ifndef KINECTCLIENT_H
#define KINECTCLIENT_H

#include "Ros_Ice.h"
#include <pcl_ros/point_cloud.h>
#include <jderobot/pointcloud.h>


class KinectClient : public Ros_Ice< jderobot::pointCloudPrx >
{
public:

    KinectClient (int argc, char **argv, std::string nodeName, std::string topicName);

    void publishROS();


};

#endif // KINECTCLIENT_H
