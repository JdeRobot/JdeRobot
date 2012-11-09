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
			sTemp << "CalibratorKinect.CameraRGB." << i << ".Proxy";
			Ice::ObjectPrx baseRGB = ic->propertyToProxy(sTemp.str());
			sTemp2 << "CalibratorKinect.CameraDEPTH." << i << ".Proxy";
			Ice::ObjectPrx baseDEPTH = ic->propertyToProxy(sTemp2.str());
			if (0==baseRGB)
				throw "Could not create proxy to RGB camera";
			if (0==baseDEPTH)
				throw "Could not create proxy to DEPTH camera";

			/*cast to CameraPrx*/
			sources[i].cRGBprx = jderobot::CameraPrx::checkedCast(baseRGB);
			if (0==sources[i].cRGBprx)
				throw "Invalid RGB proxy";

			sources[i].cDEPTHprx = jderobot::CameraPrx::checkedCast(baseDEPTH);
			if (0==sources[i].cDEPTHprx)
				throw "Invalid DEPTH proxy";
		}



		/*Create Controller and View*/
		controller = new CalibratorKinect::Controller(prop, width, height, nCameras);

		view = new CalibratorKinect::View(controller, worldconf, nCameras);


		while(view->isVisible()){
			/*Get image*/

			/*jderobot::ImageDataPtr dataRGB = sources[cam].cRGBprx->getImageData();
			colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(dataRGB->description->format);
			if (!fmt)
				throw "Format not supported";
			jderobot::ImageDataPtr dataDEPTH = sources[cam].cDEPTHprx->getImageData();
			colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat(dataDEPTH->description->format);
			if (!fmt2)
				throw "Format not supported";

			colorspaces::Image image(dataRGB->description->width, dataRGB->description->height, fmt, &(dataRGB->pixelData[0]));
			colorspaces::Image imageDEPTH(dataDEPTH->description->width, dataDEPTH->description->height, fmt2, &(dataDEPTH->pixelData[0]));*/

			view->display(sources);
			usleep(10*1000);
		}

/*libcalibrator_init(worldconf.c_str(),camInconf.c_str(),camOutconf.c_str());
libcalibrator_initgui();

unsigned char image[IMAGE_WIDTH*IMAGE_HEIGHT*3];

for(;;){
jderobot::ImageDataPtr data = cprx->getImageData();
colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(data->description->format);
if (!fmt)
throw "Format not supported";

colorspaces::Image img(data->description->width,
data->description->height,
fmt,
&(data->pixelData[0]));

if(img.width!=IMAGE_WIDTH || img.height!=IMAGE_HEIGHT){
throw "Image size not supported";
}*/

/* BGR to RGB */
/* int size=img.width*img.height;
for (int j=0; j<size; j++) {
image[j*3] = img.data[j*3+2];
image[j*3+1] = img.data[j*3+1];
image[j*3+2] = img.data[j*3];
}

libcalibrator_guibuttons();
libcalibrator_guidisplay((unsigned char*)image);
}*/

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
