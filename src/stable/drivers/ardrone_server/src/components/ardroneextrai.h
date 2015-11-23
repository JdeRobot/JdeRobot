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

#ifndef _DRONE_EXTRAI_H_
#define _DRONE_EXTRAI_H_

#include "../teleop_twist.h"
#include <jderobot/ardroneextra.h>
#include <Ice/Ice.h>

namespace ardrone_extra
{
	class ExtraI: virtual public jderobot::ArDroneExtra
	{
		public:
			ExtraI();
			virtual ~ExtraI();
			virtual void land(Ice::Current const & c);
			virtual void takeoff(Ice::Current const & c);
			virtual void reset(Ice::Current const & c);
			virtual void toggleCam(Ice::Current const & c);
			virtual void recordOnUsb(bool  record, Ice::Current const & c);
			virtual void ledAnimation(Ice::Int type, Ice::Float duration, Ice::Float freq, Ice::Current const & c);
			virtual void flightAnimation(Ice::Int type, Ice::Float duration, Ice::Current const & c);
			virtual void flatTrim(Ice::Current const & c);			
		private:
					
	};
}
#endif
