/*
 *  Copyright (C) 1997-20011 JDE Developers Team
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
			Francisco Miguel Rivas Montero <fm.rivas@alumnos.urjc.es>
			
 */

#ifndef LEDS_KINECTVIEWERCONTROLLER_H
#define LEDS_KINECTVIEWERCONTROLLER_H
#include <string>
#include <iostream>
#include <jderobot/kinectleds.h>

namespace kinectViewerController{
	class LedsController{
	public:
		LedsController(jderobot::KinectLedsPrx prx);
		~LedsController();
		void setLedActiveOff();
		void setLedActiveGreen();
		void setLedActiveRed();
		void setLedActiveYellow();
		void setLedActiveBGreen();
		void setLedActiveBYellow();
		void setLedActiveBRed();


	private:
		jderobot::KinectLedsPrx lprx;
};
}

#endif
