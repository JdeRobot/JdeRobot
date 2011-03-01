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
 *  Authors : Julio Vega <julio.vega@urjc.es>
 *
 */

#include "controller.h"

namespace introrob {
	const float Controller::V_MOTOR = 200.; // mm/s
	const float Controller::W_MOTOR = 2.; // deg/s

	Controller::Controller(jderobot::MotorsPrx mprx, jderobot::EncodersPrx eprx, jderobot::LaserPrx lprx, jderobot::CameraPrx cprx1, jderobot::CameraPrx cprx2, jderobot::PTMotorsPrx ptmprx1, jderobot::PTEncodersPrx pteprx1, jderobot::PTMotorsPrx ptmprx2, jderobot::PTEncodersPrx pteprx2) {
		this->gladepath = std::string("./introrob.glade");

		// Obtenemos los enlaces de componentes del Pioneer
		this->mprx = mprx;
		this->eprx = eprx;
		this->lprx = lprx;
		this->cprx1 = cprx1;
		this->cprx2 = cprx2;
		this->ptmprx1 = ptmprx1;
		this->pteprx1 = pteprx1;
		this->ptmprx2 = ptmprx2;
		this->pteprx2 = pteprx2;

		//this->ptmprx1->setPTMotorsData (new jderobot::PTMotorsData(0., 0.));
		//this->ptmprx2->setPTMotorsData (new jderobot::PTMotorsData(0., 0.));

		this->ed = this->eprx->getEncodersData(); // cogemos informacion de los encoders
		this->ld = this->lprx->getLaserData(); // cogemos informacion de los lasers
		this->getCameraData(); // cogemos información de las cámaras
	}

	void Controller::getCameraData() {
		// Get image1
    data1 = this->cprx1->getImageData();
    colorspaces::Image::FormatPtr fmt1 = colorspaces::Image::Format::searchFormat(data1->description->format);
    if (!fmt1)
			throw "Format not supported";

    image1 = new colorspaces::Image (data1->description->width,
		       data1->description->height,
		       fmt1,
		       &(data1->pixelData[0]));

		// Get image2
    data2 = this->cprx2->getImageData();
    colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat(data2->description->format);
    if (!fmt2)
			throw "Format not supported";

    image2 = new colorspaces::Image (data2->description->width,
		       data2->description->height,
		       fmt2,
		       &(data2->pixelData[0]));
	}

  Controller::~Controller() {}
  
  std::string	Controller::getGladePath() {
		return this->gladepath;
  }

	void Controller::stopMotors () {
		this->mprx->setV (V_MOTOR/1.1);
		this->mprx->setW (W_MOTOR/1.1);
		usleep (400000);
		this->mprx->setV (V_MOTOR/1.2);
		this->mprx->setW (W_MOTOR/1.2);
		usleep (400000);
		this->mprx->setV (V_MOTOR/1.4);
		this->mprx->setW (W_MOTOR/1.4);
		usleep (400000);
		this->mprx->setV (V_MOTOR/1.6);
		this->mprx->setW (W_MOTOR/1.6);
		usleep (400000);
		this->mprx->setV (V_MOTOR/1.8);
		this->mprx->setW (W_MOTOR/1.8);
		usleep (400000);
		this->mprx->setV (V_MOTOR/2.);
		this->mprx->setW (W_MOTOR/2.);
		usleep (400000);
		this->mprx->setV (V_MOTOR/2.5);
		this->mprx->setW (W_MOTOR/2.5);
		usleep (400000);
		this->mprx->setV (V_MOTOR/3.);
		this->mprx->setW (W_MOTOR/3.);
		usleep (400000);
		this->mprx->setV (V_MOTOR/4.);
		this->mprx->setW (W_MOTOR/4.);
		usleep (400000);
		this->mprx->setV (V_MOTOR/6.);
		this->mprx->setW (W_MOTOR/6.);
		usleep (400000);
		this->mprx->setV (V_MOTOR/8.);
		this->mprx->setW (W_MOTOR/8.);
		usleep (400000);
		this->mprx->setV (0.0);
		this->mprx->setW (0.0);
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

	void Controller::updatePioneerStatus () {
		this->ed = this->eprx->getEncodersData(); // cogemos informacion de los encoders
		this->ld = this->lprx->getLaserData(); // cogemos informacion de los lasers
		this->getCameraData(); // cogemos información de las cámaras
	}
} // namespace

