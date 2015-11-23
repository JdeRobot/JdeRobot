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

#ifndef _DRONE_NAVDATAI_H_
#define _DRONE_NAVDATAI_H_

#include "../ardrone_sdk.h"
#include <jderobot/navdata.h>
#include <iostream>
#include <vector>
#include <utils/ardrone_gen_ids.h>
#include <ardrone_tool/ardrone_version.h>
#include <ardrone_tool/ardrone_tool.h>
#include <Ice/Ice.h>

namespace navdata
{
	class NavdataI: virtual public jderobot::Navdata
	{
		public:
			NavdataI();
			virtual ~NavdataI();
			virtual jderobot::NavdataDataPtr getNavdata(Ice::Current const & c);
		private:
			jderobot::NavdataDataPtr data;
			bool readCovParams();
			double calcAverage(std::vector<double> &vec);
			void resetCaliberation();   			
			bool do_caliberation;
			int max_num_samples;
			bool caliberated;
			double acc_bias[3];
			double gyro_bias[3];
			double vel_bias[3];
			std::vector< std::vector<double> > acc_samples;
			std::vector< std::vector<double> > gyro_samples;
			std::vector< std::vector<double> > vel_samples;	
			long int last_navdata_id;
			long int copy_current_navdata_id;					
	};
}
#endif
