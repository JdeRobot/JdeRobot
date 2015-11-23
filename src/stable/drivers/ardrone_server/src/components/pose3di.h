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

#ifndef _DRONE_CAMERASERVER_H_
#define _DRONE_CAMERASERVER_H_

#include "../ardrone_sdk.h"
#include "../ardrone_driver.h"
#include <Ice/Ice.h>
#include <jderobot/pose3d.h>

namespace pose3D
{
	class Pose3DI : virtual public jderobot::Pose3D
	{
		public:
			Pose3DI(ARDroneDriver *driver);
			virtual ~Pose3DI();			
			virtual jderobot::Pose3DDataPtr getPose3DData(const Ice::Current&);
			virtual Ice::Int setPose3DData(const jderobot::Pose3DDataPtr& data, const Ice::Current&);		
		private:
			jderobot::Pose3DDataPtr pose3D;
			float deg2rad(float d);
			ARDroneDriver *driver;
	};
}

#endif
