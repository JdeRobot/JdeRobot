#include "roscompat/listener.h"

//namespace roscompat{

//Global roscompat object for callbacks
roscomp* rc;
cv::Mat frame;
int n_frame;


void 
chatterCallback(const roscompat::Num::ConstPtr& msg){

  	n_frame = msg->num;
}

void 
cameracallback(const sensor_msgs::ImageConstPtr& image_msg) {
	rc->translate_image_messages(image_msg, frame);

}

cv::Mat 
listener::getNewFrame() {

	pthread_mutex_lock(&mutex);
	cv::Mat result = frame.clone();
//std::cout << "Orig (w,h) " << frame.size().width << ", " << frame.size().height << std::endl;
//std::cout << "Res (w,h) " << result.size().width << ", " << result.size().height << std::endl; 
        pthread_mutex_unlock(&mutex);
	return result;
}	

int
listener::getNFrame() {


	pthread_mutex_lock(&mutex);
	int result = n_frame;
        pthread_mutex_unlock(&mutex);
	return result;
}

void
listener::stop() {
	ros::shutdown();
}

void
listener::listen(int argc, char** argv) {



	pthread_mutex_init(&mutex, NULL);
	rc = new roscomp();


	ros::init(argc, argv, "listener");

	ros::NodeHandle n,nh;
	image_transport::ImageTransport it(nh);
	image_transport::Subscriber camera_sub = it.subscribe("cameratopic", 1000, cameracallback);
	ros::Subscriber sub = n.subscribe("chatter", 1001, chatterCallback);

	ros::spin();

}


//}//NS
