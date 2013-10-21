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

/** \file motorsAdapter.cpp
 * \brief motorsAdapter definitios
 */

#include "motorsAdapter.h"

namespace NaoAdapter{ 
	/**
	* \brief motorsAdapter class constructor
	* \param IP the ip of the robot
	* \param port the port of the robot
	*/
	motion::motion(char* IP, int port) {
		this->IP=IP;
		this->PORT = port;
		this->lastv=0;
		this->lastw=0;
		this->last_side=0;
	}

	/**
	* \brief Funtion that create the proxy to the robot ALMotion.
	* \return 0 if ok, otherwise -1
	*/
	int motion::init() {

			try {
				std::cout << "Trying to conect to motion in " << this->IP << ":" << this->PORT << std::endl;
				this->motionProxy = new AL::ALMotionProxy(this->IP, this->PORT);
			} catch(AL::ALError& e) {
				std::cerr << "NaoBody: exception connecting to NaoQi: "<<e.toString()<< std::endl;
				return -1;
			}
			this->motionProxy->stiffnessInterpolation("Body",1.0,1.0);
			return 0;	
	}
	/**
	* \brief motorsAdapter class destructor
	*/

	void motion::terminate() {
	
	}
	/**
	* \brief walk function
	* \param v the linear speed
	* \param w the angular speed
	* \parem s the lateral component of the walk. (if v=0 and w=0 => sideways)
	*/
	int motion::walk(float v, float w, float side){
		w=-w;
		side=-side;
		if (v!=this->lastv){
			this->lastv=v;
			if (v<0){
				v=v*(-1);
				if (v>MAXV)
					v=MAXV;
				if (lastw==0){
					try{
						this->motionProxy->setWalkTargetVelocity(-1,0,0,v/MAXV);
						return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
				else{
					try{
						this->motionProxy->setWalkTargetVelocity(-1,lastw/MYMAXW,0,v/MAXV);
						return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
			}
			else if (v>0){
				if (v>MAXV)	
					v=MAXV;
				if (lastw==0){
					try{
						this->motionProxy->setWalkTargetVelocity(1,0,0,v/MAXV);
						return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
				else{
					try{
						this->motionProxy->setWalkTargetVelocity(1,lastw/MAXV,0,v/MAXV);
						return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
			}
			else{
				try{
					this->motionProxy->setWalkTargetVelocity(0,lastw/MAXV,0,lastw/MAXV);
					return 0;
				} catch(AL::ALError& e) {
					std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
					return -1;
				}
			}
		}
		if (side!=this->last_side){
			this->last_side=side;
			if (lastv<0){
				if (lastv>MAXV)
					lastv=MAXV;
				if (side==0){
					try{
						this->motionProxy->setWalkTargetVelocity(-1,0,0,(-1)*lastv/MAXV);
						return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
				else{
					try{
						this->motionProxy->setWalkTargetVelocity(-1,side/MYMAXW,0,(-1)*lastv/MAXV);
						return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
			}
			else if (lastv>0){
				if (lastv>MAXV)	
					lastv=MAXV;
				if (side==0){
					try{
						this->motionProxy->setWalkTargetVelocity(1,0,0,lastv/MAXV);
					return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
				else{
					try{
						this->motionProxy->setWalkTargetVelocity(1,side/MAXV,0,lastv/MAXV);
					return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
			}
			else{
				if (side>0){
					try{
						this->motionProxy->setWalkTargetVelocity(0,side/MAXV,0,side/MAXV);	
						return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
				else{
					try{
						this->motionProxy->setWalkTargetVelocity(0,side/MAXV,0,(-1)*side/MAXV);
						return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
			}
		}
		if (w!=this->lastw){
			this->lastw=w;
			if (lastv<0){
				if (lastv>MAXV)
					lastv=MAXV;
				if (w==0){
					try{
						this->motionProxy->setWalkTargetVelocity(-1,0,0,(-1)*lastv/MAXV);
						return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
				else{
					try{
						this->motionProxy->setWalkTargetVelocity(-1,0,w/MYMAXW,(-1)*lastv/MAXV);
						return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
			}
			else if (lastv>0){
				if (lastv>MAXV)	
					lastv=MAXV;
				if (w==0){
					try{
						this->motionProxy->setWalkTargetVelocity(1,0,0,lastv/MAXV);
						return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
				else{
					try{
						this->motionProxy->setWalkTargetVelocity(1,0,w/MYMAXW,lastv/MAXV);
						return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
			}
			else{
				if (w>0){
					try{
						this->motionProxy->setWalkTargetVelocity(0,0,w/MYMAXW,w/MAXV);	
						return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
				else{
					try{
						this->motionProxy->setWalkTargetVelocity(0,0,w/MYMAXW,(-1)*w/MAXV);
						return 0;
					} catch(AL::ALError& e) {
						std::cerr << "NaoBody: Exception while trying to change walk parametres:: "<<e.toString()<< std::endl;
						return -1;
					}
				}
			}
		}
		return -1;
	}
	
	/**
	* \brief Funtion that returns the linear speed of the robot
	* \return v
	*/
	float motion::getv(){
		return this->lastv;
	}

	/**
	* \brief Funtion that returns the angular speed of the robot
	* \return w
	*/
	float motion::getw(){
		return this->lastw;
	}

	/**
	* \brief Funtion that returns the lateral component of the gait
	* \return l
	*/
	float motion::getl(){
		return this->last_side;
	}
}
