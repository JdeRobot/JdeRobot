/*
*
*  Copyright (C) 1997-2013 JDERobot Developers Team
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
 *           Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>
*
*/

#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <colorspaces/colorspacesmm.h>
#include "view.h"
#include "controller.h"
#include "common.h"


//namespace calibrator{

std::vector<CalibratorKinect::kinectData> sources;

int main(int argc, char** argv){
	int status;
	Ice::CommunicatorPtr ic;
	CalibratorKinect::View * view;
	CalibratorKinect::Controller * controller;
	int height, width, nCameras;

	try{
		ic = Ice::initialize(argc,argv);
		

		Ice::PropertiesPtr prop = ic->getProperties();
		std::string worldconf = prop->getProperty("CalibratorKinect.World.File");
		std::string camOutconf = prop->getProperty("CalibratorKinect.Camera.FileOut");
		width=prop->getPropertyAsIntWithDefault("CalibratorKinect.Config.Columns",0);
		height=prop->getPropertyAsIntWithDefault("CalibratorKinect.Config.Rows",0);
		nCameras=prop->getPropertyAsIntWithDefault("CalibratorKinect.nCameras",0);
		std::cout << "w=" << width << ", h=" << height << std::endl;

		sources.resize(nCameras);
		for (int i=0; i< nCameras; i++){
			std::ostringstream sTemp;
			std::ostringstream sTemp2;
			sTemp << "CalibratorKinect.CameraRGB." << i << ".";
			sources[i].RGB=new jderobot::cameraClient(ic, sTemp.str(),false);
			sTemp2 << "CalibratorKinect.CameraDEPTH." << i << ".";
			sources[i].DEPTH=new jderobot::cameraClient(ic, sTemp2.str(),false);


			if (sources[i].RGB==NULL)
				throw "Could not create proxy to RGB camera";
			else
				sources[i].RGB->start();
			if (sources[i].DEPTH==NULL)
				throw "Could not create proxy to DEPTH camera";
			else
				sources[i].DEPTH->start();
		}



		/*Create Controller and View*/
		controller = new CalibratorKinect::Controller(prop, width, height, nCameras);

		view = new CalibratorKinect::View(controller, worldconf, nCameras);


		while(view->isVisible()){
			view->display(sources);

			usleep(1000);
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
//}
