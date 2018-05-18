#include <jderobot/comm/ros/listenerBumper.hpp>

namespace Comm {

	ListenerBumper::ListenerBumper(int argc, char** argv, std::string nodeName, std::string topic){
		pthread_mutex_init(&mutex, NULL);
		if ("" == topic){
			this->on = false;
			std::cerr <<"Invalid bumper topic" <<std::endl;
		}else{
			this->on = true;
			this->topic = topic;
			this->nodeName = nodeName;

			const std::string name = std::string(this->nodeName);
			int a = 0;
			ros::init(a, nullptr, name);
			ros::NodeHandle nh;
			this->sub = nh.subscribe(this->topic, 1001, &ListenerBumper::bumpercallback, this);
			std::cout << "listen from "+ this->topic << std::endl;

			this->spinner = new ros::AsyncSpinner(1);


		}
	}



	ListenerBumper::~ListenerBumper(){
		this->stop();
	}

	void 
	ListenerBumper::start(){
		this->spinner->start();

	}

	void 
	ListenerBumper::stop(){
		this->spinner->stop();
		ros::shutdown();
	}

	void 
	ListenerBumper::bumpercallback(const kobuki_msgs::BumperEventConstPtr& bumper_msg){
		timeval curTime;
		if (this->bumperData.state > 0){
			pthread_mutex_lock(&mutex);
				gettimeofday(&curTime, NULL);
				this->current_time = curTime.tv_usec / 1000;
				this->bumperData = Comm::translate_bumper_messages(bumper_msg);
			pthread_mutex_unlock(&mutex);
		}

	}

	JdeRobotTypes::BumperData  
	ListenerBumper::getBumperData(){
		timeval curTime;
		JdeRobotTypes::BumperData ld;
		pthread_mutex_lock(&mutex);
			gettimeofday(&curTime, NULL);
			int t = curTime.tv_usec / 1000;
			if ((t - this->current_time) > 500){
				this->bumperData.state = 0;
			}
			ld = this->bumperData;
		pthread_mutex_unlock(&mutex);
		return ld;
	}



}//NS