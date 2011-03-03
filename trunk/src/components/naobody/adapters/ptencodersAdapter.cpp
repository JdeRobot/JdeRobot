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

/** \file ptencodersAdapter.cpp
 * \brief ptencodersAdapter definitios
 */

#include "ptencodersAdapter.h"

const float PI=3.1415926;


namespace NaoAdapter{ 
	/**
	* \brief ptencodersAdapter class constructor
	* \param IP the ip of the robot
	* \param port the port of the robot
	*/
	ptencoders::ptencoders(char* IP, int port) {
		this->IP=IP;
		this->PORT = port;
	}

	/**
	* \brief Funtion that create the proxy to the robot ALMotion.
	* \return 0 if ok, otherwise -1
	*/
	int ptencoders::init() {

			try {
				std::cout << "Trying to conect to ptencoders in " << this->IP << ":" << this->PORT << std::endl;
				this->ptencodersProxy = new AL::ALMotionProxy(this->IP, this->PORT);
			} catch(AL::ALError& e) {
				std::cerr << "NaoBody: exception connecting to NaoQi: "<<e.toString()<< std::endl;
				return -1;
			}
			this->ptencodersProxy->stiffnessInterpolation("Body",1.0,1.0);
			return 0;	
	}
	/**
	* \brief ptencodersAdapter class destructor
	*/
	void ptencoders::terminate() {
		delete this->ptencodersProxy;
	}

	/**
	* \brief Function that returns the value in degrees of the pan angle 
	* \return The value of the pan angle in degrees
	*/
	float ptencoders::getPan(){
		std::vector<float> angles;

		try{
			angles=ptencodersProxy->getAngles("HeadYaw",true);
		} catch(AL::ALError& e) {
				std::cerr << "NaoBody: exception while getting Pan anglei: "<<e.toString()<< std::endl;
				return -1;
		}
		return angles[0]* 180 /PI;
	}

	/**
	* \brief Function that returns the value in degrees of the tilt angle 
	* \return The value of the tilt angle in degrees
	*/
	float ptencoders::getTilt(){
		std::vector<float> angles;
		
		try{
			angles=ptencodersProxy->getAngles("HeadPitch",true);
		} catch(AL::ALError& e) {
				std::cerr << "NaoBody: exception while getting Pan anglei: "<<e.toString()<< std::endl;
				return -1;
		}
		return angles[0]* 180 /PI;
	}
}
