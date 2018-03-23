#include "jderobot/comm/ros/translators.hpp"
namespace Comm {

	float PI = 3.1415;

	int MAXRANGEIMGD = 8; //max length received from imageD



	void 
	depthToRGB(const cv::Mat& float_img, cv::Mat& rgb_img, std::string type ){
	  //Process images
		cv::Mat mono8_img;
		if (type.substr(type.length() - 3, 1) == "U"){
			mono8_img = float_img;
			rgb_img = cv::Mat(float_img.size(), CV_8UC3);
		}else{
			cv::Mat mono8_img = cv::Mat(float_img.size(), CV_8UC1);
		  	if(rgb_img.rows != float_img.rows || rgb_img.cols != float_img.cols){
		    	rgb_img = cv::Mat(float_img.size(), CV_8UC3);
		    }
		  	cv::convertScaleAbs(float_img, mono8_img, 255/MAXRANGEIMGD, 0.0);
		}
		
	  	cv::cvtColor(mono8_img, rgb_img, CV_GRAY2RGB);

	}

	JdeRobotTypes::LaserData 
	translate_laser_messages(const sensor_msgs::LaserScanConstPtr& scan)
	{
		JdeRobotTypes::LaserData data;

		data.values = scan->ranges;
		data.minRange = scan->range_min;
		data.maxRange = scan->range_max;
		/* 
		*      ROS Angle Map      JdeRobot Angle Map
		*            0                  PI/2
		*            |                   |
		*            |                   |
		*   PI/2 --------- -PI/2  PI --------- 0
		*            |                   |
		*            |                   |
		*/
		data.maxAngle = scan->angle_max  + PI/2;
		data.minAngle = scan->angle_min + PI/2;
		//nsec --> nanoseconds
		data.timeStamp = scan->header.stamp.sec + (scan->header.stamp.nsec *1e-9);


		return data;

	}


	JdeRobotTypes::BumperData 
	translate_bumper_messages(const kobuki_msgs::BumperEventConstPtr& bump)
	{
		JdeRobotTypes::BumperData data;

		data.state = bump->state;
		data.bumper = bump->bumper;
		
		//data.timeStamp = scan->header.stamp.sec + (scan->header.stamp.nsec *1e-9);


		return data;

	}




	JdeRobotTypes::Image 
	translate_image_messages(const sensor_msgs::ImageConstPtr& image_msg){
		JdeRobotTypes::Image img;
		cv_bridge::CvImagePtr cv_ptr;

		img.timeStamp = image_msg->header.stamp.sec + (image_msg->header.stamp.nsec *1e-9);
		img.format = "RGB8"; // we convert img_msg to RGB8 format
		img.width = image_msg->width;
		img.height = image_msg->height;
		cv::Mat img_data;

		try {

			//std::cout << image_msg->encoding << std::endl;
			//if (image_msg->encoding.compare(sensor_msgs::image_encodings::TYPE_32FC1)==0 || image_msg->encoding.compare(sensor_msgs::image_encodings::TYPE_16UC1)==0){

			if (image_msg->encoding.substr(image_msg->encoding.length() - 2 ) == "C1"){
				cv_ptr = cv_bridge::toCvCopy(image_msg);
				depthToRGB(cv_ptr->image, img_data, image_msg->encoding);
				

			}else{
				cv_ptr = cv_bridge::toCvCopy(image_msg, sensor_msgs::image_encodings::RGB8);
				img_data = 	cv_ptr->image;
			}
		} catch (cv_bridge::Exception& e) {

			ROS_ERROR("cv_bridge exception: %s", e.what());
		}

		img.data = img_data;

		return img;
	}


	JdeRobotTypes::Rgbd 
	translate_rgbd(const sensor_msgs::ImageConstPtr& rgb,const sensor_msgs::ImageConstPtr& d){
		JdeRobotTypes::Rgbd rgbd;
		rgbd.timeStamp = rgb->header.stamp.sec + (rgb->header.stamp.nsec *1e-9);
		rgbd.color = translate_image_messages(rgb);
		rgbd.depth = translate_image_messages(d);
		return rgbd;

	}

	geometry_msgs::Twist 
	translate_twist_messages(JdeRobotTypes::CMDVel cmdvel ){
		geometry_msgs::Twist vel;
		vel.linear.x = cmdvel.vx;
		vel.linear.y = cmdvel.vy;
		vel.linear.z = cmdvel.vz;

		vel.angular.x = cmdvel.ax;
		vel.angular.y = cmdvel.ay;
		vel.angular.z = cmdvel.az;

		return vel;

	}


	JdeRobotTypes::Pose3d 
	translate_odometry_messages(const nav_msgs::OdometryConstPtr& odom_msg)
	{
		JdeRobotTypes::Pose3d data;

		data.x= odom_msg->pose.pose.position.x;
		data.y= odom_msg->pose.pose.position.y;
		data.z= odom_msg->pose.pose.position.z;

		data.q[0]= odom_msg->pose.pose.orientation.w;
		data.q[1]= odom_msg->pose.pose.orientation.x;
		data.q[2]= odom_msg->pose.pose.orientation.y;
		data.q[3]= odom_msg->pose.pose.orientation.z;

		data.yaw = quat2Yaw(data.q);
		data.pitch = quat2Pitch(data.q);
		data.roll = quat2Roll(data.q);
		
		data.timeStamp = odom_msg->header.stamp.sec + (odom_msg->header.stamp.nsec *1e-9);


		return data;

	}






	float quat2Yaw(std::vector <float> q){
	    float rotateZa0=2.0*(q[1]*q[2] + q[0]*q[3]);
	    float rotateZa1=q[0]*q[0] + q[1]*q[1] - q[2]*q[2] - q[3]*q[3];
	    float rotateZ=0.0;
	    if(rotateZa0 != 0.0 && rotateZa1 != 0.0){
	        rotateZ=atan2(rotateZa0,rotateZa1);
	    }
	    return rotateZ;
	}

	float quat2Pitch(std::vector <float> q){
	    float rotateYa0=-2.0*(q[1]*q[3] - q[0]*q[2]);
	    float rotateY=0.0;
	    if(rotateYa0 >= 1.0){
	        rotateY = M_PI_2; // PI/2
	    }
	    else if (rotateYa0 <= -1.0){
	        rotateY = -M_PI_2; // -PI/2
	    }
	    else{
	        rotateY = asin(rotateYa0);
	    }

	    return rotateY;
	}

	float quat2Roll (std::vector <float> q){
	    float rotateXa0=2.0*(q[2]*q[3] + q[0]*q[1]);
	    float rotateXa1=q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
	    float rotateX=0.0;

	    if(rotateXa0 != 0.0 && rotateXa1 != 0.0){
	        rotateX=atan2(rotateXa0, rotateXa1);
	    }
	    return rotateX;
	}




} /* NS */
