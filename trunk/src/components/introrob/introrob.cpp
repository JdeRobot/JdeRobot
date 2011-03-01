/*
 *
 *  Copyright (C) 1997-2011 JDERobot Developers Team
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
#include <jderobot/ptmotors.h>
#include <jderobot/laser.h>
#include <jderobot/encoders.h>
#include <jderobot/ptencoders.h>
#include <colorspaces/colorspacesmm.h>
#include "view.h"

int main(int argc, char** argv){
  int status,i;
	introrob::View * view;
	introrob::Controller * controller;
	introrob::Navegacion navegacion;
  Ice::CommunicatorPtr ic;

	float v, w;  

  try{
    ic = Ice::initialize(argc,argv);

		// Contact to MOTORS interface
    Ice::ObjectPrx baseMotors = ic->propertyToProxy("Introrob.Motors.Proxy");
    if (0==baseMotors)
      throw "Could not create proxy with motors";
    // Cast to motors
    jderobot::MotorsPrx mprx = jderobot::MotorsPrx::checkedCast(baseMotors);
    if (0==mprx)
      throw "Invalid proxy Introrob.Motors.Proxy";

		// Get driver camera
		Ice::ObjectPrx camara1 = ic->propertyToProxy("Introrob.Camera1.Proxy");
		if (0==camara1)
			throw "Could not create proxy to camera1 server";

		// cast to CameraPrx
		jderobot::CameraPrx cprx1 = jderobot::CameraPrx::checkedCast(camara1);
		if (0==cprx1)
			throw "Invalid proxy";

		// Get driver camera
		Ice::ObjectPrx camara2 = ic->propertyToProxy("Introrob.Camera2.Proxy");
		if (0==camara2)
			throw "Could not create proxy to camera2 server";

		// cast to CameraPrx
		jderobot::CameraPrx cprx2 = jderobot::CameraPrx::checkedCast(camara2);
		if (0==cprx2)
			throw "Invalid proxy";

		// Contact to ENCODERS interface
    Ice::ObjectPrx baseEncoders = ic->propertyToProxy("Introrob.Encoders.Proxy");
    if (0==baseEncoders)
      throw "Could not create proxy with encoders";

    // Cast to encoders
    jderobot::EncodersPrx eprx = jderobot::EncodersPrx::checkedCast(baseEncoders);
    if (0==eprx)
      throw "Invalid proxy Introrob.Encoders.Proxy";

		// Contact to LASER interface
    Ice::ObjectPrx baseLaser = ic->propertyToProxy("Introrob.Laser.Proxy");
    if (0==baseLaser)
      throw "Could not create proxy with laser";

    // Cast to laser
    jderobot::LaserPrx lprx = jderobot::LaserPrx::checkedCast(baseLaser);
    if (0==lprx)
      throw "Invalid proxy Introrob.Laser.Proxy";

		// Contact to PTMOTORS interface
    Ice::ObjectPrx ptmotors1 = ic->propertyToProxy("Introrob.PTMotors1.Proxy");
    if (0==ptmotors1)
      throw "Could not create proxy with motors";

    // Cast to ptmotors
    jderobot::PTMotorsPrx ptmprx1 = jderobot::PTMotorsPrx::checkedCast(ptmotors1);
    if (0==ptmprx1)
      throw "Invalid proxy Introrob.PTMotors1.Proxy";

		// Contact to PTMOTORS interface
    Ice::ObjectPrx ptmotors2 = ic->propertyToProxy("Introrob.PTMotors2.Proxy");
    if (0==ptmotors2)
      throw "Could not create proxy with motors";

    // Cast to ptmotors
    jderobot::PTMotorsPrx ptmprx2 = jderobot::PTMotorsPrx::checkedCast(ptmotors2);
    if (0==ptmprx2)
      throw "Invalid proxy Introrob.PTMotors2.Proxy";

		// Contact to PTENCODERS interface
    Ice::ObjectPrx ptencoders1 = ic->propertyToProxy("Introrob.PTEncoders1.Proxy");
    if (0==ptencoders1)
      throw "Could not create proxy with encoders";

    // Cast to encoders
    jderobot::PTEncodersPrx pteprx1 = jderobot::PTEncodersPrx::checkedCast(ptencoders1);
    if (0==pteprx1)
      throw "Invalid proxy Introrob.PTEncoders1.Proxy";

		// Contact to PTENCODERS interface
    Ice::ObjectPrx ptencoders2 = ic->propertyToProxy("Introrob.PTEncoders2.Proxy");
    if (0==ptencoders2)
      throw "Could not create proxy with encoders";

    // Cast to encoders
    jderobot::PTEncodersPrx pteprx2 = jderobot::PTEncodersPrx::checkedCast(ptencoders2);
    if (0==pteprx2)
      throw "Invalid proxy Introrob.PTEncoders2.Proxy";

		/**************************************************************************/
		// Create Controller and View
		controller = new introrob::Controller(mprx, eprx, lprx, cprx1, cprx2, ptmprx1, pteprx1, ptmprx2, pteprx2);

		navegacion.run(controller); // hebra de control

		view = new introrob::View (controller, &navegacion);

		while(view->isVisible()){
			controller->updatePioneerStatus ();
      view->display(*controller->image1, *controller->image2);
			usleep(10000);
		}
  } catch (const Ice::Exception& ex) {
    std::cerr << ex << std::endl;
    status = 1;
  } catch (const char* msg) {
    std::cerr << msg << std::endl;
    status = 1;
  }

	/* Stop thread */
	navegacion.stop();
	navegacion.join();

  if (ic)
    ic->destroy();
  return status;
}
