#ifndef ROSKINECTGRABBER_H
#define ROSKINECTGRABBER_H

#include "image_transport/image_transport.h"
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <pcl/io/openni_grabber.h>
#include <pcl_ros/point_cloud.h>
//#include <RosPCL/RGB_Depth_Image.h>

#include "Ros_Ice.h"

class RosKinectGrabber : public Ros_Ice< Ice::ObjectPrx >
{

public:

    RosKinectGrabber (int argc, char **argv, std::string nodeName);
    ~RosKinectGrabber();

    void cloud_cb_ (const boost::shared_ptr<openni_wrapper::Image> &image, const boost::shared_ptr<openni_wrapper::DepthImage> &depth_image, float focallength_inverse);

    void run ();

private:

    image_transport::Publisher* ImagePublisher1;
    image_transport::Publisher* ImagePublisher2;




};

#endif // ROSKINECTGRABBER_H
