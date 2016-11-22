
#ifndef ROSLISTENER_H
#define ROSLISTENER_H

#include "ros/ros.h"
#include "roscompat/roscompat.h"

#include "Num.h"
#include "Pose3d.h"

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "image_transport/image_transport.h"
#include "sensor_msgs/image_encodings.h"

//namespace roscompat {

class listener{
	
public:

	void listen(int argc, char** argv);
	void stop();
	cv::Mat getNewFrame();
	int getNFrame();

private:
	pthread_mutex_t mutex;

};//class

//}//NS

#endif // ROSLISTENER_H

	
