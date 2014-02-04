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
 *  Authors : Roberto Calvo <rocapal [at] gsyc [dot] urjc [dot] es>
 *
 */

#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <visionlib/colorspaces/colorspacesmm.h>
#include "viewer.h"
#include <cv.h>
#include <highgui.h>



int main(int argc, char** argv){
	int status;
	rgbdCalibrator::Viewer viewer;
	Ice::CommunicatorPtr ic;

	try{
		ic = Ice::initialize(argc,argv);
		Ice::ObjectPrx base = ic->propertyToProxy("rgbdCalibrator.CameraColor.Proxy");
		if (0==base)
			throw "Could not create proxy";

		/*cast to CameraPrx*/
		jderobot::CameraPrx cprxColor = jderobot::CameraPrx::checkedCast(base);
		if (0==cprxColor)
			throw "Invalid proxy";

		Ice::ObjectPrx base2 = ic->propertyToProxy("rgbdCalibrator.CameraDepth.Proxy");
		if (base2 == 0)
			throw "Could not create proxy";

		/*cast to CameraPrx*/
		jderobot::CameraPrx cprxDepth = jderobot::CameraPrx::checkedCast(base2);
		if (0==cprxDepth)
			throw "Invalid proxy";


		while(viewer.isVisible()){


			jderobot::ImageDataPtr data = cprxColor->getImageData();
			colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(data->description->format);
			if (!fmt)
				throw "Format not supported";

			colorspaces::Image imgColor(data->description->width,
					data->description->height,
					fmt,
					&(data->pixelData[0]));



			jderobot::ImageDataPtr data2 = cprxDepth->getImageData();
			colorspaces::Image::FormatPtr fmt2 =  colorspaces::Image::Format::searchFormat(data2->description->format);
			if (!fmt2)
				throw "Format not supported";

			colorspaces::Image imgDepth(data2->description->width,
					data2->description->height,
					fmt2,
					&(data2->pixelData[0]));


			viewer.display(imgColor, imgDepth);
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


