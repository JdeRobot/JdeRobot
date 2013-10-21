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
 *  		 Javier Vazquez Pereda <javiervarper@yahoo.es>
 */

#ifndef PTMOTORS_ICE
#define PTMOTORS_ICE

#include <jderobot/common.ice>

module jderobot{  

	/** 
   * PTMotorsData a class that contains the pantilt data
   */
	class PTMotorsData
	{
		float latitude;
		float latitudeSpeed;
		float longitude;
		float longitudeSpeed;
	};

	/** 
   * PTMotorsParams a class that contains the motors parametres.
   */
	class PTMotorsParams
	{
		float maxLongitude;
		float minLongitude;
		float maxLatitude;
		float minLatitude;
		float maxLongitudeSpeed;
		float maxLatitudeSpeed;
	};


  /** 
   * Interface to the PTMotors Actuators interaction.
   */
	interface PTMotors
	{
		int setPTMotorsData(PTMotorsData data);
		idempotent PTMotorsData getPTMotorsData();
		idempotent PTMotorsParams getPTMotorsParams();
	};


}; //module

#endif //PTMOTORS_ICE
