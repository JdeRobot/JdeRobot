#ifndef ROSCAMERAPLUGIN_H
#define ROSCAMERAPLUGIN_H

#include "ros/ros.h"
#include "image_transport/image_transport.h"
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>

#include <gazebo/plugins/CameraPlugin.hh>

//#include <gazebo_plugins/gazebo_ros_camera_utils.h>


namespace gazebo
{

class RosCameraPlugin : public CameraPlugin
{

public:
    RosCameraPlugin();
    ~RosCameraPlugin();

    void Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf);

protected:
    virtual void OnNewFrame(const unsigned char *_image, unsigned int _width, unsigned int _height,unsigned int _depth, const std::string &_format);

private:


    ros::NodeHandle* RosNode;
    //ros::Publisher* RosPublisher;
    image_transport::ImageTransport* ImageNode;
    image_transport::Publisher* ImagePublisher;


};

GZ_REGISTER_SENSOR_PLUGIN(RosCameraPlugin)



}



#endif // ROSCAMERAPLUGIN_H
