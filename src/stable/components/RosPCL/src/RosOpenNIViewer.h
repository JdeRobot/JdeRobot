#ifndef ROSOPENNIVIEWER_H
#define ROSOPENNIVIEWER_H

#include "Ros_Ice.h"

#include <pcl/visualization/cloud_viewer.h>
#include <pcl_ros/point_cloud.h>

class RosOpenNIViewer : public Ros_Ice< Ice::ObjectPrx >
{
public:

    RosOpenNIViewer (int argc, char **argv, std::string nodeName, std::string topicName);

    void rosCallback(const pcl::PointCloud < pcl::PointXYZRGBA >::ConstPtr &cloud);

private:

    pcl::visualization::CloudViewer viewer;
    //pcl::visualization::PCLVisualizer viewer;


};

#endif // ROSOPENNIVIEWER_H
