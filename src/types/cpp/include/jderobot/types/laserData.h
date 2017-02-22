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

#ifndef JDEROBOTTYPES_LASERDATA_H
#define JDEROBOTTYPES_LASERDATA_H

#include <vector>

namespace JdeRobotTypes {

	class LaserData {
	public:
		std::vector<float> values;
	    float minAngle = 0; //Angle of first value (rads)
	    float maxAngle = 0; // Angle of last value (rads)
	    float minRange = 0; // Max Range posible (meters)
	    float maxRange = 0; //Min Range posible (meters)
	    double timeStamp = 0; //seconds
	};


} //NS

#endif // JDEROBOTTYPES_LASERDATA_H
