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
	introrob::View *view;
	introrob::Controller *controller;
	introrob::Navegacion *navegacion;
  Ice::CommunicatorPtr ic;

	struct timeval a, b;
	int cycle = 100;
	long totalb,totala;
	long diff;

  try{
    ic = Ice::initialize(argc,argv);

	std::cout << "inicio Laser" << std::endl;
		// Contact to LASER interface
    Ice::ObjectPrx baseLaser = ic->propertyToProxy("Introrob.Laser.Proxy");
    if (0==baseLaser)
      throw "Could not create proxy with laser";

    // Cast to laser
    jderobot::LaserPrx lprx = jderobot::LaserPrx::checkedCast(baseLaser);
    if (0==lprx)
      throw "Invalid proxy Introrob.Laser.Proxy";

	std::cout << "inicio motors" << std::endl;
		// Contact to MOTORS interface
    Ice::ObjectPrx baseMotors = ic->propertyToProxy("Introrob.Motors.Proxy");
    if (0==baseMotors)
      throw "Could not create proxy with motors";
      
   
    // Cast to motors
    jderobot::MotorsPrx mprx = jderobot::MotorsPrx::checkedCast(baseMotors);
    if (0==mprx)
      throw "Invalid proxy Introrob.Motors.Proxy";
      
	std::cout << "inicio encoders" << std::endl;      
	// Contact to ENCODERS interface
    Ice::ObjectPrx baseEncoders = ic->propertyToProxy("Introrob.Encoders.Proxy");
    if (0==baseEncoders)
      throw "Could not create proxy with encoders";

    // Cast to encoders
    jderobot::EncodersPrx eprx = jderobot::EncodersPrx::checkedCast(baseEncoders);
    if (0==eprx)
      throw "Invalid proxy Introrob.Encoders.Proxy";
	std::cout << "fin encoders" << std::endl;



		/**************************************************************************/
		// Create Controller and View
		controller = new introrob::Controller(mprx, lprx, eprx);
		navegacion = new introrob::Navegacion();
		navegacion->run(controller); // hebra de control
		view = new introrob::View (controller, navegacion);

		navegacion->setView (view);


		while(view->isVisible()){
			gettimeofday(&a,NULL);
			totala=a.tv_sec*1000000+a.tv_usec;

			controller->update ();
            view->display(*controller->image1, *controller->image2);

			gettimeofday(&b,NULL);
			totalb=b.tv_sec*1000000+b.tv_usec;
			//std::cout << "Introrob takes " << (totalb-totala)/1000 << " ms" << std::endl;

			diff = (totalb-totala)/1000;
			if(diff < 0 || diff > cycle)
				diff = cycle;
			else
				diff = cycle-diff;

			/*Sleep Algorithm*/
			usleep(diff*1000);
			if(diff < 33)
				usleep(33*1000);
		}
  } catch (const Ice::Exception& ex) {
    std::cerr << ex << std::endl;
    status = 1;
  } catch (const char* msg) {
    std::cerr << msg << std::endl;
    status = 1;
  }

	/* Stop thread */
	navegacion->stop();
	navegacion->join();

  if (ic)
    ic->destroy();
  return status;
}
