#include "roscompat/roscompat.h"

//namespace roscompat{

roscomp::roscomp() {

	pthread_mutex_init(&mutex, NULL);
}

roscomp::~roscomp() {

}

void 
roscomp::translate_image_messages(const sensor_msgs::ImageConstPtr& msg, cv::Mat& image) {

	pthread_mutex_lock(&mutex);

	cv_bridge::CvImagePtr cv_ptr;

	try {

		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	} catch (cv_bridge::Exception& e) {

		ROS_ERROR("cv_bridge exception: %s", e.what());
		return;
	}

	image = cv_ptr->image;

//std::cout << "Trans (w,h) " << image.size().width << ", " << image.size().height << std::endl;
        pthread_mutex_unlock(&mutex);
}

void 
roscomp::translate_laser_messages(const roscompat::Num::ConstPtr& msg, std::vector<float>& laserdata) {
	pthread_mutex_lock(&mutex);
	laserdata = std::vector<float>(msg->numArr.begin(), msg->numArr.end());
        pthread_mutex_unlock(&mutex);
}
	
void 
roscomp::translate_pose3d_messages(const roscompat::Pose3d::ConstPtr& msg, std::vector<float>& pose) {
	pthread_mutex_lock(&mutex);
	pose.push_back(msg->x);
	pose.push_back(msg->y);
	pose.push_back(msg->z);
	pose.push_back(msg->h);
	pose.push_back(msg->q0);
	pose.push_back(msg->q1);
	pose.push_back(msg->q2);
	pose.push_back(msg->q3);
        pthread_mutex_unlock(&mutex);
}
	
void 
roscomp::translate_motor_messages(const roscompat::Motors::ConstPtr& msg, std::vector<float>& motors) {
	pthread_mutex_lock(&mutex);
	motors.push_back(msg->w);
	motors.push_back(msg->v);
	motors.push_back(msg->l);
        pthread_mutex_unlock(&mutex);
}

//}//NS
