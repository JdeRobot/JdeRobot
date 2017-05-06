#include "RosCameraPlugin.h"

namespace gazebo
{

RosCameraPlugin::RosCameraPlugin()
{

}

RosCameraPlugin::~RosCameraPlugin()
{
    delete ImagePublisher;
    delete ImageNode;
    //delete RosPublisher;
    delete RosNode;
}

void RosCameraPlugin::Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf)
{
      // Don't forget to load the camera plugin

    CameraPlugin::Load(_parent,_sdf);
    RosNode = new ros::NodeHandle;
    ImageNode = new image_transport::ImageTransport(*RosNode);

    ImagePublisher = new image_transport::Publisher(ImageNode->advertise(this->parentSensor->GetCamera()->GetName(), 500));
    ROS_INFO("ROS Publisher Loaded: %s\n",this->parentSensor->GetCamera()->GetName().c_str());

}

void RosCameraPlugin::OnNewFrame(const unsigned char *_image, unsigned int _width, unsigned int _height, unsigned int _depth, const std::string &_format)
{
    cv::Mat image(_height,_width,CV_8UC3,(void *) _image);

    sensor_msgs::ImagePtr img_message = cv_bridge::CvImage(std_msgs::Header(), "rgb8", image).toImageMsg();
    //sensor_msgs::Image img_message = *(cv_bridge::CvImage(std_msgs::Header(), "rgb8", image).toImageMsg());


    ImagePublisher->publish(img_message);

   //RosPublisher->publish(img_message);

    //ROS_INFO("New Frame\n");
}


}

