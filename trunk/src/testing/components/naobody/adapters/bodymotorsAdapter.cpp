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

/** \file bodymotorsAdapter.cpp
 * \brief bodymotorsAdapter definitios
 */

#include "bodymotorsAdapter.h"

const float PI=3.1415926;


namespace NaoAdapter{ 
	/**
	* \brief bodymotorsAdapter class constructor
	* \param IP the ip of the robot
	* \param port the port of the robot
	*/
	bodymotors::bodymotors(char* IP, int port) {
		this->IP=IP;
		this->PORT = port;
	}

	/**
	* \brief Funtion that create the proxy to the robot ALMotion.
	* \return 0 if ok, otherwise -1
	*/
	int bodymotors::init() {

			try {
				std::cout << "Trying to conect to bodymotors in " << this->IP << ":" << this->PORT << std::endl;
				this->bodymotorsProxy = new AL::ALMotionProxy(this->IP, this->PORT);
			} catch(AL::ALError& e) {
				std::cerr << "NaoBody: exception connecting to NaoQi: "<<e.toString()<< std::endl;
				return -1;
			}
			this->bodymotorsProxy->stiffnessInterpolation("Body",1.0,1.0);
			return 0;	
	}
	/**
	* \brief bodymotorsAdapter class destructor
	*/
	void bodymotors::terminate() {
		delete this->bodymotorsProxy;
	}

	/**
	* \brief Funtion that sets motor position
	* \return 0 if ok, otherwise -1
	*/
	int bodymotors::setMotorPosition(jderobot::MotorsName name, jderobot::BodySide side, float angle, float speed){
		std::vector<std::string> names;
		std::vector<std::string>::iterator e;
		AL::ALValue names_ok;
		AL::ALValue angles;

		switch(name){
			case jderobot::HipYawPitch: 
				names.push_back("HipYawPitch");
				break;
			case jderobot::HipPitch: 
				names.push_back("HipPitch");
				break;
			case jderobot::HipRoll: 
				names.push_back("HipRoll");
				break;
			case jderobot::KneePitch: 
				names.push_back("KneePitch");
				break;
			case jderobot::AnklePitch: 
				names.push_back("AnklePitch");
				break;
			case jderobot::AnkleRoll: 
				names.push_back("AnkleRoll");
				break;
			case jderobot::ShoulderPitch: 
				names.push_back("ShoulderPitch");
				break;
			case jderobot::ShoulderRoll: 
				names.push_back("ShoulderRoll");
				break;
			case jderobot::ElbowYaw: 
				names.push_back("ElbowYaw");
				break;
			case jderobot::ElbowRoll: 
				names.push_back("ElbowRoll");
				break;
			default: 
				std::cout << "NaoBody: error, invalid motor name" << std::endl;
		}
		if (side==jderobot::Left){
			for (e=names.begin(); e!=names.end(); e++){
				names_ok.arrayPush("L"+(*e));
			}
		}
		else{
			for (e=names.begin(); e!=names.end(); e++){
				names_ok.arrayPush("R"+(*e));
			}
		}
		angles.arrayPush(angle/180*PI);
		try{
			angles=bodymotorsProxy->post.angleInterpolationWithSpeed(names_ok,angles,speed/100);
		} catch(AL::ALError& e) {
				std::cerr << "NaoBody: exception while setting arm angles: "<<e.toString()<< std::endl;
				return -1;
		}
		return 0;
	}

	/**
	* \brief Funtion that gets the limit of a motor
	* \param name The name of the motor
	* \param side The side of the motor
	* \param motorsParam A local reference to locate all the data
	* \return 0 if ok, otherwise -1
	*/
	int bodymotors::getMotorLimit(jderobot::MotorsName name, jderobot::BodySide side, jderobot::BodyMotorsParamPtr motorsParam){
		AL::ALValue limits;
		std::vector<std::string> names;
		std::vector<std::string>::iterator e;
		AL::ALValue names_ok;

		switch(name){
			case jderobot::HipYawPitch: 
				names.push_back("HipYawPitch");
				break;
			case jderobot::HipPitch: 
				names.push_back("HipPitch");
				break;
			case jderobot::HipRoll: 
				names.push_back("HipRoll");
				break;
			case jderobot::KneePitch: 
				names.push_back("KneePitch");
				break;
			case jderobot::AnklePitch: 
				names.push_back("AnklePitch");
				break;
			case jderobot::AnkleRoll: 
				names.push_back("AnkleRoll");
				break;
			case jderobot::ShoulderPitch: 
				names.push_back("ShoulderPitch");
				break;
			case jderobot::ShoulderRoll: 
				names.push_back("ShoulderRoll");
				break;
			case jderobot::ElbowYaw: 
				names.push_back("ElbowYaw");
				break;
			case jderobot::ElbowRoll: 
				names.push_back("ElbowRoll");
				break;
			default: 
				std::cout << "NaoBody: error, invalid motor name" << std::endl;
		}
		if (side==jderobot::Left){
			for (e=names.begin(); e!=names.end(); e++){
				names_ok.arrayPush("L"+(*e));
			}
		}
		else{
			for (e=names.begin(); e!=names.end(); e++){
				names_ok.arrayPush("R"+(*e));
			}
		}

		try{
			limits=this->bodymotorsProxy->getLimits(names_ok[0]);
		} catch(AL::ALError& e) {
			std::cerr << "NaoBody: exception connecting to NaoQi: "<<e.toString()<< std::endl;
			return -1;
		}
		motorsParam->minAngle=((float) limits[0][0]) * 180/PI;
		motorsParam->maxAngle=((float) limits[0][1]) * 180/PI;
		motorsParam->maxSpeed=100;
		return 0;	
	}

}
