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

#include "ardroneextrai.h"

namespace ardrone_extra
{

	ExtraI::ExtraI()
	{
		std::cout << "extra start" << std::endl;
	}
	
	ExtraI::~ExtraI()
	{
	
	}

	void ExtraI::toggleCam(Ice::Current const & c)
	{
		drone_toggleCam();
	}
	void ExtraI::land(Ice::Current const & c)
	{
	 	drone_land();
	}
	
	void ExtraI::takeoff(Ice::Current const & c)
	{
		drone_takeoff();
	}
	
	void ExtraI::reset(Ice::Current const & c)
	{
		drone_reset();
	}	
	
	void ExtraI::recordOnUsb(bool  record, Ice::Current const & c)
	{
		if (IS_ARDRONE2){
			record_video(record);
		}
	}
	
	void ExtraI::ledAnimation(Ice::Int type, Ice::Float duration, Ice::Float freq, Ice::Current const & c)
	{
		setLedAnimation(type, duration, freq);
	}
	
	void ExtraI::flightAnimation(Ice::Int type, Ice::Float duration, Ice::Current const & c)
	{
		if (IS_ARDRONE2){
			setFlightAnimation(type,duration);
		}	
		
	}
	
	void ExtraI::flatTrim(Ice::Current const & c)
	{
		drone_flatTrim();
	}
}
