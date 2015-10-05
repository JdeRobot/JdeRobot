#include "RosOpenNIViewer.h"

void viewerOneOff (pcl::visualization::PCLVisualizer& viewer)
{
    viewer.setBackgroundColor (1.0, 1.0, 1.0);

}

RosOpenNIViewer::RosOpenNIViewer (int argc, char **argv, std::string nodeName, std::string topicName) : viewer ("PCL OpenNI Viewer")
{

    initializeROS(argc,argv,nodeName);
    addRosSubscriber(topicName,500,&RosOpenNIViewer::rosCallback,this);
    viewer.runOnVisualizationThreadOnce (viewerOneOff);
}

void RosOpenNIViewer::rosCallback(const pcl::PointCloud < pcl::PointXYZRGBA >::ConstPtr &cloud)
{
    if (!viewer.wasStopped())
      viewer.showCloud (cloud);
}
