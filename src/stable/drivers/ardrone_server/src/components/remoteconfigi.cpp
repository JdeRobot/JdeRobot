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

#include "remoteconfigi.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>

namespace remoteconfig{

	RemoteConfigI::RemoteConfigI(ARDroneDriver *driver)
	{
		std::cout << "remoteconfig start" << std::endl;
		this->driver=driver;
		idLocal=0;
	}

	RemoteConfigI::~RemoteConfigI()
	{
		delete driver;
	}

	Ice::Int RemoteConfigI::initConfiguration(const Ice::Current&)
	{
		std::cout << "RemoteConfig: initializing" << std::endl;
		if (idLocal==0){
			/* initialize random seed: */
			srand ( time(NULL) );

			/* generate secret number: */
			idLocal = rand() + 1;

			std::stringstream ss;//create a stringstream
			ss << idLocal << ".xml";//add number to the stream
	
			path=ss.str();
			f2.open(ss.str().c_str(), std::ofstream::out);
			std::cout << "-----------------" <<  idLocal << std::endl;
			return idLocal;
		}
		else
			return 0;
	}

	std::string RemoteConfigI::read(Ice::Int id, const Ice::Current&)
	{
		return std::string("");
	}

	Ice::Int RemoteConfigI::write(const std::string& data, Ice::Int id, const Ice::Current&)
	{
		if (id == idLocal){
			f2 << data << std::endl;
			return 1;
		}
		else{
			return 0;
		}
	}

	Ice::Int RemoteConfigI::setConfiguration(Ice::Int id, const Ice::Current&)
	{
		if (id == idLocal){
			id=0;
			idLocal=0;
			f2.close();
			std::cout << "RemoteConfig: file completed" << std::endl;
			driver->configureDrone((char*)path.c_str());

			if(remove(path.c_str())!=0){
				std::cout << "RemoteConfig: Error deleting file" << std::endl;
			}
			return 1;
		}
		return 0;
	}
}
