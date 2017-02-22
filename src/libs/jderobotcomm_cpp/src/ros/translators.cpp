#include "jderobot/comm/ros/translators.hpp"
namespace JdeRobotComm {

	float PI = 3.1415;

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
		data.maxAngle = scan->angle_max  - PI/2;
		data.minAngle = scan->angle_min - PI/2;
		//nsec --> nanoseconds
		data.timeStamp = scan->header.stamp.sec + (scan->header.stamp.nsec *1e-9);


		return data;

	}




} /* NS */