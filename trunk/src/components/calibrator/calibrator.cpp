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
*  Authors : Sara Marugán Alonso <smarugan@gsyc.es>,
 *           Eduardo Perdices <eperdices@gsyc.es>
*
*/

#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <colorspaces/colorspacesmm.h>
#include "view.h"
#include "controller.h"

int main(int argc, char** argv){
	int status;
	Ice::CommunicatorPtr ic;
	calibrator::View * view;
	calibrator::Controller * controller;

	try{
		ic = Ice::initialize(argc,argv);
		Ice::ObjectPrx base = ic->propertyToProxy("Calibrator.Camera.Proxy");
		if (0==base)
			throw "Could not create proxy";

		/*cast to CameraPrx*/
		jderobot::CameraPrx cprx = jderobot::CameraPrx::checkedCast(base);
		if (0==cprx)
			throw "Invalid proxy";

		Ice::PropertiesPtr prop = ic->getProperties();
		std::string worldconf = prop->getProperty("Calibrator.World.File");
		std::string camOutconf = prop->getProperty("Calibrator.Camera.FileOut");

		/*Create Controller and View*/
		controller = new calibrator::Controller(prop);
		view = new calibrator::View(controller);

		while(view->isVisible()){

			/*Get image*/
			jderobot::ImageDataPtr data = cprx->getImageData();
			colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(data->description->format);
			if (!fmt)
			throw "Format not supported";

			colorspaces::Image image(data->description->width,
			data->description->height,
			fmt,
			&(data->pixelData[0]));

			view->display(image);
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
