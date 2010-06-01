/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
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
 *  Authors : Sara Marugán Alonso <smarugan@gsyc.es>
 *
 */

#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <colorspaces/colorspacesmm.h>
#include "libcalibrator.h"

#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 240


int main(int argc, char** argv){
  int status;
  Ice::CommunicatorPtr ic;

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
    std::string camInconf = prop->getProperty("Calibrator.Camera.FileIn");
    std::string camOutconf = prop->getProperty("Calibrator.Camera.FileOut");

    libcalibrator_init(worldconf.c_str(),camInconf.c_str(),camOutconf.c_str());
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
      }

      /* BGR to RGB */
      int size=img.width*img.height;
      for (int j=0; j<size; j++) {
		image[j*3] = img.data[j*3+2];
		image[j*3+1] = img.data[j*3+1];
		image[j*3+2] = img.data[j*3];
      }

      libcalibrator_guibuttons();
      libcalibrator_guidisplay((unsigned char*)image);
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
