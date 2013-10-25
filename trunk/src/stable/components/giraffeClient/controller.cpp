/*
 *  Copyright (C) 2010 Eduardo Perdices García
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *   Authors : Eduardo Perdices García <eperdices@gsyc.es>,
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include "controller.h"

namespace giraffeClient {

	Controller::Controller(RoboCompJointMotor::JointMotorPrx jprx) {
		this->gladepath = std::string(GLADE_DIR) + std::string("/giraffeClient.glade");
		this->jprx = jprx;
		this->camera = new CameraConf(this->jprx);
	}

  Controller::~Controller() {
		delete this->camera;
  }
  
  std::string
	Controller::getGladePath() {
		return this->gladepath;
  }

	void
	Controller::setMotorPos(int motor, float pos) {

		RoboCompJointMotor::MotorGoalPosition motorpos;

		/*Select the motor*/
		switch(motor) {
		case MOTOR_PAN:	
			motorpos.name = "neck";
			if(pos > MAX_PAN)
				motorpos.position = MAX_PAN;
			else if(pos < MIN_PAN)
				motorpos.position = MIN_PAN;
			else
				motorpos.position = pos;
			break;
		case MOTOR_TILT:	
			motorpos.name = "tilt";
			if(pos > MAX_TILT)
				motorpos.position = MAX_TILT;
			else if(pos < MIN_TILT)
				motorpos.position = MIN_TILT;
			else
				motorpos.position = pos;
			break;
		case MOTOR_LEFT:	
			motorpos.name = "leftPan";
			motorpos.position = pos;
			break;
		case MOTOR_RIGHT:	
			motorpos.name = "rightPan";
			motorpos.position = pos;
			break;
		default:
			motorpos.name = "neck";
			motorpos.position = 0.0;
		}

		motorpos.maxSpeed = 0.0;

		/*Call to server*/
		try{
			jprx->setPosition(motorpos);
		}catch(...) {
			std::cout << "Error: Couldn't change motor " << motorpos.name << " to position " << motorpos.position << std::endl;
		}

		std::cout << "Changed motor " << motorpos.name << " to position " << motorpos.position << std::endl;
	}

	void
	Controller::drawWorld(const colorspaces::Image& image) {
		IplImage src=image;

		this->camera->drawWorld(&src);
	}

	int
	Controller::lookAt(float x, float y, float z) {
  	RoboCompJointMotor::MotorGoalPositionList list;
  	RoboCompJointMotor::MotorGoalPosition g;
		float pan, tilt;

		this->camera->getAngles(x,y,z,pan,tilt);

		/*Set motors to change*/
		g.maxSpeed = 0;
		g.name = "neck";
		if(pan > MAX_PAN)
			g.position = MAX_PAN;
		else if(pan < MIN_PAN)
			g.position = MIN_PAN;
		else
			g.position = pan;
		list.push_back(g);

		g.maxSpeed = 0;
		g.name = "tilt";
		if(tilt > MAX_TILT)
			g.position = MAX_TILT;
		else if(tilt < MIN_TILT)
			g.position = MIN_TILT;
		else
			g.position = tilt;
		list.push_back(g);

		try {
			jprx->setSyncPosition(list);
		} catch(...) {
			std::cout << "Error: Couldn't set giraffe looking at " << x << ", " << y << ", " << z << " with angles " << pan << ", " << tilt << std::endl;
		}

		std::cout << "Giraffe looking at " << x << ", " << y << ", " << z << " with angles " << pan << ", " << tilt << std::endl;

///users/eperdices/robocomp/robocomp/Components/RoboLab/Experimental/headntpComp/src

		return 0;
	}

} /*namespace*/

