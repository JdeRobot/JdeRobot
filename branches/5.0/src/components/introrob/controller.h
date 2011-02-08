/*
 *  Copyright (C) 1997-2011 JDERobot Developers Team
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

#ifndef INTROROB_CONTROLLER_H
#define INTROROB_CONTROLLER_H

#include <string>
#include <iostream>
#include <colorspaces/colorspacesmm.h>
#include <jderobot/motors.h>
#include <jderobot/encoders.h>
#include <jderobot/laser.h>
#include "pioneer.h"
#include "drawarea.h"

namespace introrob {
  class Controller {
		public:
			Controller (jderobot::MotorsPrx mprx, jderobot::EncodersPrx eprx, jderobot::LaserPrx lprx);
		  virtual ~Controller();
		  
		  std::string getGladePath();

			void drawScene();

			void playMotors ();
			void stopMotors ();
			void goLeft ();
			void goRight ();
			void goUp ();
			void goDown ();

			void updatePioneerStatus ();
			static const float V_MOTOR;
			static const float W_MOTOR;

			jderobot::EncodersDataPtr ed;
			jderobot::LaserDataPtr ld;

		private:
			std::string gladepath;
			jderobot::MotorsPrx mprx;
			jderobot::EncodersPrx eprx;
			jderobot::LaserPrx lprx;
  };
} // namespace

#endif /*INTROROB_CONTROLLER_H*/
