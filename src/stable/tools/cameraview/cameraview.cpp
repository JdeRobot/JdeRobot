/*
 *
 *  Copyright (C) 1997-2015 JdeRobot Developers Team
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
 *  Authors : 	Roberto Calvo <rocapal [at] gsyc [dot] es>
 *  			David Lobato Bravo <dav.lobato [dat] gmail [dot] com>
 *
 */

#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <visionlib/colorspaces/colorspacesmm.h>
#include "viewer.h"
#include "parallelIce/cameraClient.h"

int main(int argc, char** argv){
	int status;
	cameraview::Viewer viewer;
	Ice::CommunicatorPtr ic;

	jderobot::cameraClient* camRGB;

	try{
		ic = Ice::initialize(argc,argv);
		Ice::ObjectPrx base = ic->propertyToProxy("Cameraview.Camera.Proxy");
		Ice::PropertiesPtr prop = ic->getProperties();

		if (0==base)
			throw "Could not create proxy";


		camRGB = new jderobot::cameraClient(ic,"Cameraview.Camera.");

		if (camRGB == NULL){
			throw "Invalid proxy";
		}
		camRGB->start();

		cv::Mat rgb;

		while(viewer.isVisible()){
			//jderobot::ImageDataPtr data = camRGB->getImageData(format);

			camRGB->getImage(rgb);
			viewer.display(rgb);
			viewer.displayFrameRate(camRGB->getRefreshRate());
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

	camRGB->stop_thread();
	delete(camRGB);

	return status;
}
