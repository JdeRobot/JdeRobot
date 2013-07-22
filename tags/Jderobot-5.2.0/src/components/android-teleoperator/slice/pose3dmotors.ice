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
 *            Francisco Miguel Rivas Montero <fm.rivas@alumnos.urjc.es>	
 *  		 Javier Vazquez Pereda <javiervarper@yahoo.es>
 */

#ifndef POSE3DMOTORS_ICE
#define POSE3DMOTORS_ICE

#include "common.ice"

module jderobot{  

	/** 
   * Pose3DMotorsData a class that contains the pantilt data
   */
	class Pose3DMotorsData
	{
		float x;
		float y;
		float z;
		float pan;
		float tilt;
		float roll;
		float panSpeed;
		float tiltSpeed;
	};

	/** 
   * Pose3DMotorsParams a class that contains the motors parametres.
   */
	class Pose3DMotorsParams
	{
		float maxPan;
		float minPan;
		float maxTilt;
		float minTilt;
		float maxPanSpeed;
		float maxTiltSpeed;
	};


  /** 
   * Interface to the Pose3DMotors Actuators interaction.
   */
	interface Pose3DMotors
	{
		int setPose3DMotorsData(Pose3DMotorsData data);
		idempotent Pose3DMotorsData getPose3DMotorsData();
		idempotent Pose3DMotorsParams getPose3DMotorsParams();
	};


}; //module

#endif //Pose3DMOTORS_ICE
