/*
 *  Copyright (C) 1997-2012 JDE Developers Team
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
 *            Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>	
 */

#ifndef REMOTECLOUD_ICE
#define REMOTECLOUD_ICE

#include <jderobot/pointcloud.ice>

module jderobot{  

	interface remoteCloud extends pointCloud{
		
		int initConfiguration();
		string read(int id);
		int write (string data, int id);
		int setConfiguration(int id);
	};
};
#endif 

