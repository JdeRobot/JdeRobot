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
#include <jderobot/laser.h>
#include <jderobot/encoders.h>
#include <colorspaces/colorspacesmm.h>
#include "view.h"

int main(int argc, char** argv){
  int status,i;
	introrob::View * view;
	introrob::Controller * controller;
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
		Ice::ObjectPrx camara = ic->propertyToProxy("Introrob.Camera.Proxy");
		if (0==camara)
			throw "Could not create proxy to camera server";

		// cast to CameraPrx
		jderobot::CameraPrx cprx = jderobot::CameraPrx::checkedCast(camara);
		if (0==cprx)
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

		// Create Controller and View
		controller = new introrob::Controller(mprx, eprx, lprx);
		view = new introrob::View(controller);

		while(view->isVisible()){
			controller->updatePioneerStatus ();

			// Get image
      jderobot::ImageDataPtr data = cprx->getImageData();
      colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(data->description->format);
      if (!fmt)
				throw "Format not supported";

      colorspaces::Image image(data->description->width,
			       data->description->height,
			       fmt,
			       &(data->pixelData[0]));

      view->display(image);

			usleep(10000);
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
