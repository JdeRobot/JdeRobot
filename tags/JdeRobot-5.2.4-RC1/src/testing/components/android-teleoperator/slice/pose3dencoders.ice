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


#ifndef POSE3DENCODERS_ICE
#define POSE3DENCODERS_ICE

#include "common.ice"


module jderobot{  
	/**
	* ptencoders dat information 
	*/
  class Pose3DEncodersData
  {
		float x;
		float y;
		float z;
  	float pan;
		float tilt;
		float roll;
		int clock;
  };


  /** 
   * Interface to the ptencoders sensor.
   */
  interface Pose3DEncoders
  {
	idempotent	Pose3DEncodersData getPose3DEncodersData();
  };

}; //module

#endif //Pose3DENCODERS_ICE
