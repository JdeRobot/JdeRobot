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

#ifndef JDEROBOTCOMM_LISTENERLASER_H_
#define JDEROBOTCOMM_LISTENERLASER_H_


#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <boost/thread/thread.hpp>
#include <jderobot/types/laserData.h>
#include <jderobot/comm/interfaces/laserClient.hpp>
#include <jderobot/comm/ros/translators.hpp>

namespace JdeRobotComm {
	class ListenerLaser: public JdeRobotComm::LaserClient {
		
	public:
		ListenerLaser(int argc, char** argv, std::string nodeName, std::string topic);
		~ListenerLaser();

		
		void stop();
		virtual JdeRobotTypes::LaserData  getLaserData();
		

	private:
		pthread_mutex_t mutex;
		ros::Subscriber sub;
		std::string topic;
		std::string nodeName;
		
		void listen();
		void lasercallback (const sensor_msgs::LaserScanConstPtr& laser_msg);

		



	};//class

} //NS
#endif /* JDEROBOTCOMM_LISTENERLASER_H_ */