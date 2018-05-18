/*
 *  Copyright (C) 1997-2016 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *  Authors :
 *       Aitor Martinez Fernandez <aitor.martinez.fernandez@gmail.com>
 */

#ifndef JDEROBOTCOMM_LISTENERBUMPER_H_
#define JDEROBOTCOMM_LISTENERBUMPER_H_


#include <ros/ros.h>
#include <kobuki_msgs/BumperEvent.h>
#include <boost/thread/thread.hpp>
#include <jderobot/types/bumperData.h>
#include <jderobot/comm/interfaces/bumperClient.hpp>
#include <jderobot/comm/ros/translators.hpp>
#include <sys/time.h>

namespace Comm {
	class ListenerBumper: public Comm::BumperClient {
		
	public:
		ListenerBumper(int argc, char** argv, std::string nodeName, std::string topic);
		~ListenerBumper();

		void start();
		void stop();
		virtual JdeRobotTypes::BumperData  getBumperData();
		

	private:
		pthread_mutex_t mutex;
		ros::Subscriber sub;
		std::string topic;
		std::string nodeName;
		int current_time;

		ros::AsyncSpinner* spinner;
		
		void bumpercallback (const kobuki_msgs::BumperEventConstPtr& bumper_msg);

		



	};//class

} //NS
#endif /* JDEROBOTCOMM_LISTENERBUMPER_H_ */