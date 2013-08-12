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

/** \file ptmotorsAdapter.cpp
 * \brief ptmotorsAdapter definitios
 */

#include "ptmotorsAdapter.h"

const float PI=3.1415926;


namespace NaoAdapter{ 
	/**
	* \brief ptmotorsAdapter class constructor
	* \param IP the ip of the robot
	* \param port the port of the robot
	*/
	ptmotors::ptmotors(char* IP, int port) {
		this->IP=IP;
		this->PORT = port;
	}

	/**
	* \brief Funtion that create the proxy to the robot ALMotion.
	* \return 0 if ok, otherwise -1
	*/
	int ptmotors::init(jderobot::PTMotorsParamsPtr p) {
			AL::ALValue limits;

			try {
				std::cout << "Trying to conect to ptmotors in " << this->IP << ":" << this->PORT << std::endl;
				this->ptmotorsProxy = new AL::ALMotionProxy(this->IP, this->PORT);
			} catch(AL::ALError& e) {
				std::cerr << "NaoBody: exception connecting to NaoQi: "<<e.toString()<< std::endl;
				return -1;
			}
			this->ptmotorsProxy->stiffnessInterpolation("Body",1.0,1.0);
			try{
				limits=this->ptmotorsProxy->getLimits("HeadYaw");
			} catch(AL::ALError& e) {
				std::cerr << "NaoBody: exception connecting to NaoQi: "<<e.toString()<< std::endl;
				return -1;
			}
			p->minLongitude=((float) limits[0][0]) * 180/PI;
			p->maxLongitude=((float) limits[0][1]) * 180/PI;
			try{
				limits=this->ptmotorsProxy->getLimits("HeadPitch");
			} catch(AL::ALError& e) {
				std::cerr << "NaoBody: exception connecting to NaoQi: "<<e.toString()<< std::endl;
				return -1;
			}
			p->minLatitude=((float) limits[0][0]) * 180/PI;
			p->maxLatitude=((float) limits[0][1]) * 180/PI;
			p->maxLongitudeSpeed=100;
			p->maxLatitudeSpeed=100;


			return 0;	
	}
	/**
	* \brief ptmotorsAdapter class destructor
	*/
	void ptmotors::terminate() {
	
	}

	/**
	* \brief Function that conect to NaoQi to change the head position latitude, longitude -> yaw, pitch 
	* \param data a local reference to locate the data
	* \return 0 if ok, otherwise -1
	*/
	int ptmotors::changeHeadPosition(jderobot::PTMotorsDataPtr data){
		AL::ALValue names_y, names_p;
		AL::ALValue changes_y, changes_p;

		names_y.arrayPush("HeadYaw");
		names_p.arrayPush("HeadPitch");
		changes_y.arrayPush(data->longitude/180*PI);
		changes_p.arrayPush(data->latitude/180*PI);
		try{
			this->ptmotorsProxy->post.angleInterpolationWithSpeed(names_y,changes_y,data->longitudeSpeed/100);
		} catch(AL::ALError& e) {
			std::cerr << "NaoBody: exception trying to change head position (yaw): "<<e.toString()<< std::endl;
			return -1;
		}
		try{
			this->ptmotorsProxy->post.angleInterpolationWithSpeed(names_p,changes_p,data->latitudeSpeed/100);
		} catch(AL::ALError& e) {
			std::cerr << "NaoBody: exception trying to change head position (pitch): "<<e.toString()<< std::endl;
			return -1;
		}
		return 0;
	}
}
