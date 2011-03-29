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
	const float Controller::V_MOTOR = 250.; // mm/s
	const float Controller::W_MOTOR = 20.; // deg/s

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

		this->setEncoders();
		this->setLaser();
		this->setCameraData1();
		this->setCameraData2();
		this->setPTEncoders1();
		this->setPTEncoders2();

		//pthread_mutex_init(&this->cameraMutex1, NULL);
		//pthread_mutex_init(&this->cameraMutex2, NULL);
		pthread_mutex_init(&this->encodersMutex, NULL);
		pthread_mutex_init(&this->laserMutex, NULL);
		pthread_mutex_init(&this->ptEncodersMutex1, NULL);
		pthread_mutex_init(&this->ptEncodersMutex2, NULL);
	}

	void Controller::setCameraData1() {
		printf ("antes\n");
		//pthread_mutex_lock(&this->cameraMutex1); // lock
		printf ("despues\n");

		// Get image1
    this->data1 = this->cprx1->getImageData();
    colorspaces::Image::FormatPtr fmt1 = colorspaces::Image::Format::searchFormat(this->data1->description->format);
    if (!fmt1)
			throw "Format not supported";

    this->image1 = new colorspaces::Image (this->data1->description->width,
		       this->data1->description->height,
		       fmt1,
		       &(this->data1->pixelData[0]));

		//pthread_mutex_unlock(&this->cameraMutex1); // unlock
	}

	void Controller::getCameraData1(unsigned char **imagen1) {
		//pthread_mutex_lock(&this->cameraMutex1); // lock

		// Set image1
		memset (*imagen1, 0, this->data1->description->width*this->data1->description->height*3);
		memcpy (*imagen1, &this->data1->pixelData[0], this->data1->description->width*this->data1->description->height*3);

		//pthread_mutex_unlock(&this->cameraMutex1); // unlock
	}

	void Controller::setCameraData2() {
		//pthread_mutex_lock(&this->cameraMutex2); // lock

		// Get image2
    this->data2 = this->cprx2->getImageData();
    colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat(this->data2->description->format);
    if (!fmt2)
			throw "Format not supported";

    this->image2 = new colorspaces::Image (this->data2->description->width,
		       this->data2->description->height,
		       fmt2,
		       &(this->data2->pixelData[0]));

		//pthread_mutex_unlock(&this->cameraMutex2); // unlock
	}

	void Controller::getCameraData2(unsigned char **imagen2) {
		//pthread_mutex_lock(&this->cameraMutex2); // lock

		// Set image2
		memset (*imagen2,0,this->data2->description->width*this->data2->description->height*3);
		memcpy (*imagen2, &this->data2->pixelData[0], this->data2->description->width*this->data2->description->height*3);

		//pthread_mutex_unlock(&this->cameraMutex2); // unlock
	}

	void Controller::stopMotors () {
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

	void Controller::setV (float v) {
		this->mprx->setV (v);
	}

	void Controller::setW (float w) {
		this->mprx->setW (w);
	}

	void Controller::setPT1 (float latitude, float longitude) {
		jderobot::PTMotorsData* myData;
		myData = new jderobot::PTMotorsData ();
		myData->latitude = latitude;
		myData->longitude = longitude;
		this->ptmprx1->setPTMotorsData (myData);
	}

	void Controller::setPT2 (float latitude, float longitude) {
		jderobot::PTMotorsData* myData;
		myData = new jderobot::PTMotorsData ();
		myData->latitude = latitude;
		myData->longitude = longitude;
		this->ptmprx2->setPTMotorsData (myData);
	}

	void Controller::getPosition (CvPoint3D32f* myPoint) {
		myPoint->x = this->ed->robotx;
		myPoint->y = this->ed->roboty;
		myPoint->z = this->ed->robottheta;
	}

	int Controller::getNumLasers () {
		return (this->ld->numLaser);
	}

	void Controller::getLaser (std::vector<float>* laser) {
		//pthread_mutex_lock(&this->laserMutex); // lock

		laser->clear();
		for (int k = 0; k < this->ld->numLaser; k++)
			laser->push_back (this->ld->distanceData[k]);

		//pthread_mutex_unlock(&this->laserMutex); // unlock
	}

	void Controller::setEncoders () {
		this->ed = this->eprx->getEncodersData(); // cogemos informacion de los encoders
	}

	void Controller::setLaser () {
		this->ld = this->lprx->getLaserData(); // cogemos informacion de los lasers
	}

	void Controller::setPTEncoders1() {}
	void Controller::setPTEncoders2() {}

	void Controller::update () {
		this->setEncoders(); // cogemos información de las cámaras
		this->setLaser(); // cogemos información de las cámaras
		this->setCameraData1(); // cogemos información de las cámaras
		this->setCameraData2(); // cogemos información de las cámaras
	}

  Controller::~Controller() {}
  
  std::string	Controller::getGladePath() {
		return this->gladepath;
  }
} // namespace

