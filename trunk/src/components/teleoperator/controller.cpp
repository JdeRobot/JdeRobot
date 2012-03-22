/*
 *  Copyright (C) 2010 Julio Vega
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
 *  Authors : Julio Vega <julio.vega@urjc.es>
 *
 */

#include "controller.h"

namespace teleoperator {
	const float Controller::V_MOTOR = 2.0;
	const float Controller::W_MOTOR = 1.0;
	const float Controller::MOTOR_PAN = 0.3;
	const float Controller::MOTOR_TILT = 0.3;
	const int Controller::MOUSELEFT = 1;
	const int Controller::MOUSEMIDDLE = 2;
	const int Controller::MOUSERIGHT = 3;
	const int Controller::MOUSEWHEELUP = 4;
	const int Controller::MOUSEWHEELDOWN = 5;

	Controller::Controller(jderobot::MotorsPrx mprx, RoboCompJointMotor::JointMotorPrx jprx, jderobot::EncodersPrx eprx, jderobot::LaserPrx lprx, jderobot::SonarsPrx sprx) {
		this->gladepath = std::string(GLADE_DIR) + std::string("/teleoperatorgui.glade");

		// Obtenemos los enlaces de componentes del Pioneer
		this->mprx = mprx;
		this->jprx = jprx;
		this->eprx = eprx;
		this->lprx = lprx;
		this->sprx = sprx;

		this->motorsparams = this->jprx->getAllMotorParams();
		this->jprx->getAllMotorState(this->motorsstate);

		this->ed = this->eprx->getEncodersData(); // cogemos informacion de los encoders
		this->sd = this->sprx->getSonarsData(); // cogemos informacion de los sonares
		this->ld = this->lprx->getLaserData(); // cogemos informacion de los lasers
		this->motorsparams = this->jprx->getAllMotorParams();
		this->jprx->getAllMotorState(this->motorsstate);

		this->camera = new CameraConf(this->jprx);
		this->camera2 = new CameraConf(this->jprx);
/*		this->virtualcam.posx = -150.;
		this->virtualcam.posy = -150.;
		this->virtualcam.posz = 150.;
		this->virtualcam.foax = 0.;
		this->virtualcam.foay = 0.;
		this->virtualcam.foaz = 0.;
		this->virtualcam.roll = 0.;
		this->mypioneer.posx = 0.;
		this->mypioneer.posy = 0.;
		this->mypioneer.posz = 0.;
		this->mypioneer.foax = 0.;
		this->mypioneer.foay = 0.;
		this->mypioneer.foaz = 0.;
		this->mypioneer.roll = 0.;*/
	}

  Controller::~Controller() {
		delete this->camera;
		delete this->camera2;
  }
  
  std::string	Controller::getGladePath() {
		return this->gladepath;
  }

	void Controller::stopMotors () {
		this->mprx->setW (0.0);
		this->mprx->setV (0.0);
	}

	void Controller::goLeft () {
		this->mprx->setW (W_MOTOR);
	}

	void Controller::goRight () {
		this->mprx->setW (-W_MOTOR);
	}

	void Controller::goUp () {
		this->mprx->setV (V_MOTOR);
	}

	void Controller::goDown () {
		this->mprx->setV (-V_MOTOR);
	}

	void Controller::stopMotors1 () {
		RoboCompJointMotor::MotorGoalPosition motorpos1;
		RoboCompJointMotor::MotorGoalPosition motorpos2;
		this->jprx->getAllMotorState(motorstate);

		motorpos1.name = "neck";
		motorpos1.position = 0.0;
		motorpos1.maxSpeed = 0.0;

		motorpos2.name = "tilt";
		motorpos2.position = 0.0;
		motorpos2.maxSpeed = 0.0;

		/*Call to server*/
		try{
			this->jprx->setPosition(motorpos1);
		}catch(...) {
			std::cout << "Error: Couldn't change motor " << motorpos1.name << " to position " << motorpos1.position << std::endl;
		}

		/*Call to server*/
		try{
			this->jprx->setPosition(motorpos2);
		}catch(...) {
			std::cout << "Error: Couldn't change motor " << motorpos2.name << " to position " << motorpos2.position << std::endl;
		}
	}

	void Controller::goLeft1 () {
		RoboCompJointMotor::MotorGoalPosition motorpos;
		this->jprx->getAllMotorState(motorstate);

		motorpos.name = "neck";
		motorpos.position = motorstate["neck"].pos - MOTOR_PAN;

		if (motorpos.position < MIN_PAN)
			motorpos.position = MIN_PAN;

		motorpos.maxSpeed = 0.0;

		/*Call to server*/
		try{
			this->jprx->setPosition(motorpos);
		}catch(...) {
			std::cout << "Error: Couldn't change motor " << motorpos.name << " to position " << motorpos.position << std::endl;
		}
	}

	void Controller::goRight1 () {
		RoboCompJointMotor::MotorGoalPosition motorpos;
		this->jprx->getAllMotorState(motorstate);

		motorpos.name = "neck";
		motorpos.position = motorstate["neck"].pos + MOTOR_PAN;

		if (motorpos.position > MAX_PAN)
			motorpos.position = MAX_PAN;

		motorpos.maxSpeed = 0.0;

		/*Call to server*/
		try{
			this->jprx->setPosition(motorpos);
		}catch(...) {
			std::cout << "Error: Couldn't change motor " << motorpos.name << " to position " << motorpos.position << std::endl;
		}
	}

	void Controller::goUp1 () {
		RoboCompJointMotor::MotorGoalPosition motorpos;
		this->jprx->getAllMotorState(motorstate);

		motorpos.name = "tilt";
		motorpos.position = motorstate["tilt"].pos + MOTOR_TILT;

		if (motorpos.position > MAX_TILT)
			motorpos.position = MAX_TILT;

		motorpos.maxSpeed = 0.0;

		/*Call to server*/
		try{
			this->jprx->setPosition(motorpos);
		}catch(...) {
			std::cout << "Error: Couldn't change motor " << motorpos.name << " to position " << motorpos.position << std::endl;
		}
	}

	void Controller::goDown1 () {
		RoboCompJointMotor::MotorGoalPosition motorpos;
		this->jprx->getAllMotorState(motorstate);

		motorpos.name = "tilt";
		motorpos.position = motorstate["tilt"].pos - MOTOR_TILT;

		if (motorpos.position < MIN_TILT)
			motorpos.position = MIN_TILT;

		motorpos.maxSpeed = 0.0;

		/*Call to server*/
		try{
			this->jprx->setPosition(motorpos);
		}catch(...) {
			std::cout << "Error: Couldn't change motor " << motorpos.name << " to position " << motorpos.position << std::endl;
		}
	}

	void Controller::drawWorld(const colorspaces::Image& image1, const colorspaces::Image& image2) {
		IplImage src1=image1;
		IplImage src2=image2;
		this->camera->drawWorld(&src1);
		this->camera2->drawWorld(&src2);
	}

	void Controller::updatePioneerStatus () {
		this->motorsparams = this->jprx->getAllMotorParams();
		this->jprx->getAllMotorState(this->motorsstate);

		this->ed = this->eprx->getEncodersData(); // cogemos informacion de los encoders
		this->sd = this->sprx->getSonarsData(); // cogemos informacion de los sonares
		this->ld = this->lprx->getLaserData(); // cogemos informacion de los lasers
	}
} /*namespace*/

