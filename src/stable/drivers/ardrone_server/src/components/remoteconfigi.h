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

#ifndef _DRONE_REMOTECONFIGI_H_
#define _DRONE_REMOTECONFIGI_H_

#include "../ardrone_driver.h"
#include <jderobot/remoteConfig.h>
#include <fstream>
#include <Ice/Ice.h>

namespace remoteconfig{
	class RemoteConfigI: virtual public jderobot::remoteConfig {
		public:
			RemoteConfigI(ARDroneDriver *driver);
			virtual ~RemoteConfigI();
			virtual Ice::Int initConfiguration(const Ice::Current&);
			virtual std::string read(Ice::Int id, const Ice::Current&);
			virtual Ice::Int write(const std::string& data, Ice::Int id, const Ice::Current&);
			virtual Ice::Int setConfiguration(Ice::Int id, const Ice::Current&);
		private:
			std::ofstream f2;
			int idLocal;
			std::string path;
			ARDroneDriver *driver;
	};
}
#endif
