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
 *  Authors : Javier Vazquez Pereda  <javiervazper@yahoo.es>
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
#include "introrobgui.h"
#include "navega.h"




int main(int argc, char** argv){
  int status,i;
  introrob::introrobgui introrobgui_obj;
  introrob::navega navega_obj;
  Ice::CommunicatorPtr ic;
//	int laser[180];
  

  try{
    ic = Ice::initialize(argc,argv);

//contact to camera interface
    Ice::ObjectPrx baseCamera = ic->propertyToProxy("Introrob.Camera.Proxy");
    if (0==baseCamera)
      throw "Could not create proxy with Camera";

    /*cast to CameraPrx*/
    jderobot::CameraPrx cprx = jderobot::CameraPrx::checkedCast(baseCamera);
    if (0==cprx)
      throw "Invalid proxy Introrob.Camera.Proxy";

//contact to motors interface
    Ice::ObjectPrx baseMotors = ic->propertyToProxy("Introrob.Motors.Proxy");
    if (0==baseMotors)
      throw "Could not create proxy with motors";

    /*cast to CameraPrx*/
    jderobot::MotorsPrx mprx = jderobot::MotorsPrx::checkedCast(baseMotors);
    if (0==mprx)
      throw "Invalid proxy Introrob.Motors.Proxy";

//contact to laser interface
    Ice::ObjectPrx baseLaser = ic->propertyToProxy("Introrob.Laser.Proxy");
    if (0==baseLaser)
      throw "Could not create proxy with laser";

    /*cast to CameraPrx*/
    jderobot::LaserPrx lprx = jderobot::LaserPrx::checkedCast(baseLaser);
    if (0==lprx)
      throw "Invalid proxy Introrob.Laser.Proxy";

//contact to encoders interface
    Ice::ObjectPrx baseEncoders = ic->propertyToProxy("Introrob.Encoders.Proxy");
    if (0==baseEncoders)
      throw "Could not create proxy with encoders";
    jderobot::EncodersPrx eprx = jderobot::EncodersPrx::checkedCast(baseEncoders);
    if (0==eprx)
      throw "Invalid proxy Introrob.Encoders.Proxy";


    
    while(introrobgui_obj.isVisible() && ! introrobgui_obj.isClosed()){
      jderobot::ImageDataPtr data = cprx->getImageData();
      colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(data->description->format);
      if (!fmt)
	throw "Format not supported";

      colorspaces::Image image(data->description->width,
			       data->description->height,
			       fmt,
			       &(data->pixelData[0]));
      introrobgui_obj.updatecamera(image);

	if (introrobgui_obj.isYourcode() && ! introrobgui_obj.isStopped()){
//		navega_obj.with_image(image);
//		for (i=0;i<180;i++){
		jderobot::LaserDataPtr laserData = lprx->getLaserData();
		jderobot::EncodersDataPtr encodersData = eprx->getEncodersData();
	
printf("laserData->distanceData[0]: %d\nlaserData->distanceData[90]: %d\nlaserData->distanceData[179]: %d\n",laserData->distanceData[0],laserData->distanceData[90],laserData->distanceData[179]);
printf("encodersData->robotx: %.2f\nencodersData->roboty: %.2f\nencodersData->robottheta: %.2f\n",encodersData->robotx,encodersData->roboty,encodersData->robottheta);
//		}
		navega_obj.navegacion(data->description->width,
			       data->description->height,
			       &(data->pixelData[0]),
				laserData,
				encodersData);
//printf("r,g,b: %d,%d,%d\n",(int)data->pixelData[0],(int)data->pixelData[1],(int)data->pixelData[2]);
printf("navega_obj.getV(): %32f\n navega_obj.getW(): %.2f\n",navega_obj.getV(),navega_obj.getW());
		mprx->setV(navega_obj.getV());
		mprx->setW(navega_obj.getW());
	}	
	else if (!introrobgui_obj.isYourcode() && ! introrobgui_obj.isStopped()) {
		mprx->setV(introrobgui_obj.getV());
		mprx->setW(introrobgui_obj.getW());
	}
	else if (introrobgui_obj.isStopped()) {
		mprx->setV(0);
		mprx->setW(0);
	}

    }
	mprx->setV(0);
	mprx->setW(0);
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
