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
#include <visionlib/colorspaces/colorspacesmm.h>
#include "viewer.h"
#include "easyiceconfig/EasyIce.h" 
#include <jderobot/comm/cameraClient.hpp>
#include <jderobot/types/image.h>

int main(int argc, char** argv){

	int status;
	cameraview::Viewer viewer;
	Ice::CommunicatorPtr ic;

	JdeRobotComm::CameraClient* camRGB;

	ic = EasyIce::initialize(argc,argv);

	camRGB = JdeRobotComm::getCameraClient(ic, "Cameraview.Camera");

	JdeRobotTypes::Image rgb;

	while(viewer.isVisible()){
		//jderobot::ImageDataPtr data = camRGB->getImageData(format);

		rgb = camRGB->getImage();
		viewer.display(rgb.data);
		viewer.displayFrameRate(0);
	}

	return status;
}
