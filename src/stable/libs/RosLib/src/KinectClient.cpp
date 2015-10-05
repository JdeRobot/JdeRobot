#include "KinectClient.h"

KinectClient::KinectClient (int argc, char **argv, std::string nodeName, std::string topicName)
{
    initializeROS(argc,argv,nodeName);
    addRosPublisher < pcl::PointCloud<pcl::PointXYZRGBA> >(topicName,500);
}


void KinectClient::publishROS()
{
    jderobot::pointCloudDataPtr point_cloud;

    point_cloud = this->Proxy->getCloudData();

    pcl::PointCloud<pcl::PointXYZRGBA> ros_cloud;
    pcl::PointXYZRGBA point;

    for(unsigned int i = 0; i < point_cloud->p.size(); ++i)
    {
       point.x = point_cloud->p[i].x;
       point.y = point_cloud->p[i].y;
       point.z = point_cloud->p[i].z;
       point.r = point_cloud->p[i].r;
       point.g = point_cloud->p[i].g;
       point.b = point_cloud->p[i].b;

       ros_cloud.push_back(point);
    }

    rosPublish(ros_cloud);


}
