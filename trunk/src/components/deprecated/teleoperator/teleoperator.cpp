/*
 *
 *  Copyright (C) 1997-2010 JDERobot Developers Team
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
 *  Authors : Julio Vega  <julio.vega@urjc.es>
 *
 */

#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <jderobot/motors.h>
#include <jderobot/laser.h>
#include <jderobot/encoders.h>
#include <jderobot/sonars.h>
#include <colorspaces/colorspacesmm.h>
#include "view.h"
#include "JointMotor.h"

int main(int argc, char** argv){
  int status,i;
	teleoperator::View * view;
	teleoperator::Controller * controller;
  Ice::CommunicatorPtr ic;
	RoboCompJointMotor::MotorParamsList motorsparams;
	RoboCompJointMotor::MotorStateMap motorsstate;

	float v, w;  

  try{
    ic = Ice::initialize(argc,argv);

		// Contact to MOTORS interface
    Ice::ObjectPrx baseMotors = ic->propertyToProxy("Teleoperator.Motors.Proxy");
    if (0==baseMotors)
      throw "Could not create proxy with motors";
    // Cast to motors
    jderobot::MotorsPrx mprx = jderobot::MotorsPrx::checkedCast(baseMotors);
    if (0==mprx)
      throw "Invalid proxy Teleoperator.Motors.Proxy";

		/*Get to giraffe server*/
		Ice::ObjectPrx baseGiraffe = ic->propertyToProxy("Teleoperator.JointMotor.Proxy");
		if (0==baseGiraffe)
			throw "Could not create proxy to giraffe server";

		/*cast to JointMotorPrx*/
		RoboCompJointMotor::JointMotorPrx jprx = RoboCompJointMotor::JointMotorPrx::checkedCast(baseGiraffe);
		if (0==jprx)
			throw "Invalid proxy";

		/*Get driver camera1*/		
		Ice::ObjectPrx camara1 = ic->propertyToProxy("Teleoperator.Camera1.Proxy");
		if (0==camara1)
			throw "Could not create proxy to camera server";

		/*cast to CameraPrx1*/
		jderobot::CameraPrx cprx1 = jderobot::CameraPrx::checkedCast(camara1);
		if (0==cprx1)
			throw "Invalid proxy";

		/*Get driver camera2*/		
		Ice::ObjectPrx camara2 = ic->propertyToProxy("Teleoperator.Camera2.Proxy");
		if (0==camara2)
			throw "Could not create proxy to camera server";

		/*cast to CameraPrx2*/
		jderobot::CameraPrx cprx2 = jderobot::CameraPrx::checkedCast(camara2);
		if (0==cprx2)
			throw "Invalid proxy";

		// Contact to ENCODERS interface
    Ice::ObjectPrx baseEncoders = ic->propertyToProxy("Teleoperator.Encoders.Proxy");
    if (0==baseEncoders)
      throw "Could not create proxy with encoders";

    // Cast to encoders
    jderobot::EncodersPrx eprx = jderobot::EncodersPrx::checkedCast(baseEncoders);
    if (0==eprx)
      throw "Invalid proxy Teleoperator.Encoders.Proxy";

		// Contact to LASER interface
    Ice::ObjectPrx baseLaser = ic->propertyToProxy("Teleoperator.Laser.Proxy");
    if (0==baseLaser)
      throw "Could not create proxy with laser";

    // Cast to laser
    jderobot::LaserPrx lprx = jderobot::LaserPrx::checkedCast(baseLaser);
    if (0==lprx)
      throw "Invalid proxy Teleoperator.Laser.Proxy";

		// Contact to SONAR interface
    Ice::ObjectPrx baseSonar = ic->propertyToProxy("Teleoperator.Sonar.Proxy");
    if (0==baseSonar)
      throw "Could not create proxy with sonar";
    // Cast to sonar
    jderobot::SonarsPrx sprx = jderobot::SonarsPrx::checkedCast(baseSonar);
    if (0==sprx)
      throw "Invalid proxy Teleoperator.Sonar.Proxy";

		/*Create Controller and View*/
		controller = new teleoperator::Controller(mprx, jprx, eprx, lprx, sprx);
		view = new teleoperator::View(controller);

		/*Show params of motors*/
		motorsparams = jprx->getAllMotorParams();
/*
		cout << "Motors params:" << endl;
		for(vector<RoboCompJointMotor::MotorParams>::iterator it = motorsparams.begin(); it != motorsparams.end(); it++) {
			cout << endl;
			cout << "Name: " << (*it).name << endl;
			cout << "Id: " << (int) (*it).busId << endl;
			cout << "minPos: " << (*it).minPos << endl;
			cout << "maxPos: " << (*it).maxPos << endl;
			cout << "maxVel: " << (*it).maxVelocity << endl;
			cout << "zeroPos: " << (*it).zeroPos << endl;
			cout << "inverted: " << (*it).invertedSign << endl;
		}
*/
		/*Get current pos of motors*/
		jprx->getAllMotorState(motorsstate);
//		view->setInitialValues(motorsstate["neck"].pos, motorsstate["tilt"].pos, motorsstate["leftPan"].pos, motorsstate["rightPan"].pos);
//		view->setRealValues(motorsstate["neck"].pos, motorsstate["tilt"].pos, motorsstate["leftPan"].pos, motorsstate["rightPan"].pos);

		while(view->isVisible()){
			controller->updatePioneerStatus ();
			//jprx->getAllMotorState(motorsstate);
//			view->setRealValues(motorsstate["neck"].pos, motorsstate["tilt"].pos, motorsstate["leftPan"].pos, motorsstate["rightPan"].pos);

			/*Get image1*/
      jderobot::ImageDataPtr data1 = cprx1->getImageData();
      colorspaces::Image::FormatPtr fmt1 = colorspaces::Image::Format::searchFormat(data1->description->format);
      if (!fmt1)
				throw "Format not supported";

      colorspaces::Image image1(data1->description->width,
			       data1->description->height,
			       fmt1,
			       &(data1->pixelData[0]));

			/*Get image2*/
      jderobot::ImageDataPtr data2 = cprx2->getImageData();
      colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat(data2->description->format);
      if (!fmt2)
				throw "Format not supported";

      colorspaces::Image image2(data2->description->width,
			       data2->description->height,
			       fmt2,
			       &(data2->pixelData[0]));

      view->display(image1, image2);

			usleep(10*1000);
		}
  }catch (const Ice::Exception& ex) {
    std::cerr << ex << std::endl;
    status = 1;
  } catch (const char* msg) {
    std::cerr << msg << std::endl;
    status = 1;
  }

  if (ic)
    ic->destroy();
  return status;
}
