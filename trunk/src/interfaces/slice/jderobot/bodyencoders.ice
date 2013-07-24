/*
 *  Copyright (C) 1997-2010 JDE Developers Team
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
 *
 *  Authors : Jose María Cañas <jmplaza@gsyc.es>
 *            Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>	
 */

#ifndef BODYENCODERS_ICE
#define BODYENCODERS_ICE

#include <jderobot/common.ice>
#include <jderobot/body.ice>

module jderobot{  

	/**
	* Arm 
	*/
	class ArmEncodersData{
		BodyMotor shoulder;
		BodyMotor elbow;
		int clock;
	};
	
	/**
	*	Leg
	*/
	class LegEncodersData{
		BodyMotor hip;
		BodyMotor knee;
		BodyMotor ankle;
		int clock;
	};
	
	/**
	* 	camera odometry
	*/
	class OdometryData{
		seqFloat odometry;
	};

  /** 
   * Interface to the Humanoid Body Encoders.
   */
	interface BodyEncoders
	{
		idempotent ArmEncodersData getArmEncodersData(BodySide side);
		idempotent LegEncodersData getLegEncodersData(BodySide side);
		idempotent OdometryData getOdometryData(CameraBody camera);
	};

};

#endif 
