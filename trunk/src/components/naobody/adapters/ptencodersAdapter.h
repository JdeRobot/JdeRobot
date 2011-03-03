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
 *            Eduardo Perdices <eperdes@gsyc.es>
 *            Francisco Miguel Rivas Montero <fm.rivas@alumnos.urjc.es>	
 */


#ifndef PTENCODERS_ADAPTER
#define PTENCODERS_ADAPTER

#include <iostream>
#include <pthread.h>	
#include "alproxy.h"
#include "almotionproxy.h"


namespace NaoAdapter{
/**
* \brief Class that contains all PTEncoders definitions.
*/
class ptencoders{
	private:
		std::string IP;
		int PORT;
		AL::ALMotionProxy * ptencodersProxy;

	public:

		ptencoders(char* IP, int port);
		int init();
		void terminate();
		float getPan();
		float getTilt();
};
}	

#endif
