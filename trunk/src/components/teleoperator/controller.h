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
 *  Authors : Julio Vega <julio.vega@urjc.es>,
 *
 */

#ifndef TELEOPERATOR_CONTROLLER_H
#define TELEOPERATOR_CONTROLLER_H

#include <string>
#include <iostream>
#include <colorspaces/colorspacesmm.h>
#include <jderobot/motors.h>
#include <jderobot/encoders.h>
#include <jderobot/laser.h>
#include <jderobot/sonars.h>
#include "pioneer.h"
#include "JointMotor.h"
#include "cameraConf.h"
#include "drawarea.h"

namespace teleoperator {
  class Controller {
		public:
			Controller (jderobot::MotorsPrx mprx, RoboCompJointMotor::JointMotorPrx jprx, jderobot::EncodersPrx eprx, jderobot::LaserPrx lprx, jderobot::SonarsPrx sprx);
		  virtual ~Controller();
		  
		  std::string getGladePath();

			void drawWorld(const colorspaces::Image& image1, const colorspaces::Image& image2);
			void drawScene();

			void stopMotors ();
			void goLeft ();
			void goRight ();
			void goUp ();
			void goDown ();
			void stopMotors1 ();
			void goLeft1 ();
			void goRight1 ();
			void goUp1 ();
			void goDown1 ();
			void updatePioneerStatus ();
			static const float V_MOTOR;
			static const float W_MOTOR;
			static const float MOTOR_PAN;
			static const float MOTOR_TILT;
			static const int MOUSELEFT;
			static const int MOUSEMIDDLE;
			static const int MOUSERIGHT;
			static const int MOUSEWHEELUP;
			static const int MOUSEWHEELDOWN;

			jderobot::EncodersDataPtr ed;
			jderobot::SonarsDataPtr sd;
			jderobot::LaserDataPtr ld;

		private:
			std::string gladepath;
			jderobot::MotorsPrx mprx;
			RoboCompJointMotor::JointMotorPrx jprx;
			RoboCompJointMotor::MotorParamsList motorsparams;
			RoboCompJointMotor::MotorStateMap motorsstate;
			jderobot::EncodersPrx eprx;
			jderobot::LaserPrx lprx;
			jderobot::SonarsPrx sprx;

			CameraConf * camera;
			CameraConf * camera2;
			RoboCompJointMotor::MotorStateMap motorstate;
  };
} // namespace

#endif /*TELEOPERATOR_CONTROLLER_H*/
