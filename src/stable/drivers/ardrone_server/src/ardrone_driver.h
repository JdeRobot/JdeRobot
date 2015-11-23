/*
 *  Copyright (C) 1997-2015 JDE Developers Team
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
 *       Alberto Martín Florido <almartinflorido@gmail.com>	
 */

#ifndef _ARDRONE_DRIVER_H_
#define _ARDRONE_DRIVER_H_

class ARDroneDriver;


#include "ardrone_sdk.h"
#include "ardrone_parser.h"
#include <iostream>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>
#include <Ice/Ice.h>

#include "boost/date_time/posix_time/posix_time.hpp" 

#define _DEG2RAD 0.01745331111
#define _RAD2DEG 57.2957184819

#define DRIVER_USERNAME "ardrone_server"
#define DRIVER_APPNAME "ardrone_server"
#define CAMERA_QUEUE_SIZE (10)
#define NAVDATA_QUEUE_SIZE (25)
#define CONFIG_FILE_PATH "./config/config.xml"
#define CONFIG_FILE_TMP "/tmp"
#define LOOPRATE 30


class ARDroneDriver
{    
	public:
		ARDroneDriver();
		~ARDroneDriver();

		void run();
		bool parseConfigFile(char* filepath);
		double getParameter(char* param, double defaultVal);

		void initIce(int argc, char** argv);
		void initInterfaces();
		Ice::CommunicatorPtr getCommunicator();
		void configureDrone(char* configFile);

	private:
		Ice::CommunicatorPtr ic;
		ArDroneParser parser;
		ArDroneConfig *conf;
		float last_tm;				
		long int last_frame_id;
		long int copy_current_frame_id;
		int flying_state;
		bool inited;  
        bool record_usb;
};

#endif
