#ifndef TOGGLECAMPLUGIN_HH
#define TOGGLECAMPLUGIN_HH

#include <boost/bind.hpp>
#include "gazebo/gazebo.hh"
#include "gazebo/plugins/CameraPlugin.hh"
#include "gazebo/common/common.hh"
#include "gazebo/transport/transport.hh"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace gazebo {

class ToggleCamPlugin : public CameraPlugin {
public:
	ToggleCamPlugin();

	cv::Mat image;
	std::string nameGlobal;
protected:
    virtual void Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf);
    virtual void OnNewFrame(const unsigned char *_image, 
		unsigned int _width, unsigned int _height, unsigned int _depth, 
		const std::string &_format);
private:
	int count;
	int n;
    int camera_id_;
    int on_;
    transport::SubscriberPtr sub_;
    transport::PublisherPtr pub_;
    std::string img_topic_;
    std::string toggle_topic_;
    void ToggleCallback(const boost::shared_ptr<const msgs::Vector2d> &_msg);
pthread_mutex_t mutex;
}; //ToogeCamPlugin

} //gazebo

#endif
