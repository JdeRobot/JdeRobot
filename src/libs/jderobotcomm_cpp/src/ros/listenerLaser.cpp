#include <jderobot/comm/ros/listenerLaser.hpp>

namespace JdeRobotComm {

	ListenerLaser::ListenerLaser(int argc, char** argv, std::string nodeName, std::string topic){
		pthread_mutex_init(&mutex, NULL);
		if ("" == topic){
			this->on = false;
			std::cerr <<"Invalid laser topic" <<std::endl;
		}else{
			this->on = true;
			this->topic = topic;
			this->nodeName = nodeName;
			boost::thread t3(&ListenerLaser::listen, this);
		}
	}



	ListenerLaser::~ListenerLaser(){
		this->stop();
	}

	void 
	ListenerLaser::listen(){
		if (this->on){
			int a = 0;
			ros::init(a, nullptr, "laserListener");
			ros::NodeHandle nh;
			this->sub = nh.subscribe(this->topic, 1001, &ListenerLaser::lasercallback, this);
			std::cout << "listen from "+ this->topic << std::endl;
			ros::spin();
		}

	}

	void 
	ListenerLaser::stop(){
		ros::shutdown();
	}

	void 
	ListenerLaser::lasercallback(const sensor_msgs::LaserScanConstPtr& laser_msg){
		pthread_mutex_lock(&mutex);
		this->laserData = JdeRobotComm::translate_laser_messages(laser_msg);
		pthread_mutex_unlock(&mutex);

	}

	JdeRobotTypes::LaserData  
	ListenerLaser::getLaserData(){
		JdeRobotTypes::LaserData ld;
		pthread_mutex_lock(&mutex);
		ld = this->laserData;
		pthread_mutex_unlock(&mutex);
		return ld;
	}



}//NS