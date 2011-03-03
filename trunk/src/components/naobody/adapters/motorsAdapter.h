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


#ifndef MOTORS_ADAPTER
#define MOTORS_ADAPTER

#include <iostream>
#include <pthread.h>
#include "alproxy.h"
#include "almotionproxy.h"

#define REALNAO 0
#define CAMERA 0


#define RIGHT 1
#define LEFT 2

#define LEFT_SHOULDER 0
#define LEFT_ELBOW 1
#define RIGHT_SHOULDER 2
#define RIGHT_ELBOW 3
#define LEFT_HIP 4
#define RIGHT_HIP 5
#define LEFT_KNEE 6
#define RIGHT_KNEE 7
#define LEFT_ANKLE 8
#define RIGHT_ANKLE 9
#define HEAD 10
#define MIN_HEIGHT 150	

const float PI=3.1415926;

const float MAXVY=100;
const float MINV=0;
const float MAXV=100;
const float CHANGE_RANGE=1;
const float MYMAXW=100;
const float MYMINW=0;
const float MYMINR=3;
const float MYMAXR=0;
const float DISTANCE_A=0.08;
const float DISTANCE_B=10;
const float COM_SPACE=1;

namespace NaoAdapter{
/**
* \brief Class that contains all motors definitions.
*/
class motion{
	private:
		std::string IP;
		int PORT;
		AL::ALMotionProxy * motionProxy;
		float lastv;
		float lastw;
		float last_side;

	public:

		motion(char* IP, int port);
		int init();
		void terminate();
		int walk(float v, float w, float side);
		float getv();
		float getw();
		float getl();
};
}	
#endif
