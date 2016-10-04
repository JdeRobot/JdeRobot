#ifndef ROSCOMPAT_H
#define ROSCOMPAT_H

#include "ros/ros.h"

#include "Num.h"
#include "Pose3d.h"
#include "Motors.h"

#include "opencv2/core/core.hpp"
#include "image_transport/image_transport.h"
#include "cv_bridge/cv_bridge.h"
#include "sensor_msgs/image_encodings.h"

//namespace roscompat {

class roscomp{
	
public:

	roscomp();
	~roscomp();
	void translate_image_messages(const sensor_msgs::ImageConstPtr& msg, cv::Mat& image);
	void translate_laser_messages(const roscompat::Num::ConstPtr& msg, std::vector<float>& laserdata);
	void translate_pose3d_messages(const roscompat::Pose3d::ConstPtr& msg, std::vector<float>& pose);
	void translate_motor_messages(const roscompat::Motors::ConstPtr& msg, std::vector<float>& motors);

private:
	pthread_mutex_t mutex;

	cv::Mat cam_frame;
};//class

//}//NS

#endif // ROSCOMPAT_H
