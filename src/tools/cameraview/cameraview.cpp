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




#include "opencv2/opencv.hpp"

#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <visionlib/colorspaces/colorspacesmm.h>
#include "viewer.h"
#include "parallelIce/cameraClient.h"
#include "easyiceconfig/EasyIce.h" 
#include "roscompat/listenerCamera.h"

//Global variables for ROS translation.

cameraview::Viewer* viewer;

void *getInfoAsync(void* l) {

	listenerCamera* li = (listenerCamera*)l;

	while(viewer->isVisible()) {
	  	viewer->displayFrameRate(li->getNFrame());
		viewer->display(li->getNewFrame());
	}
	
	li->stop();
	exit (0);

}

int main(int argc, char** argv){

	viewer = new cameraview::Viewer();

	int status;	
	jderobot::cameraClient* camRGB;
	cv::Mat cam_frame;

	Ice::CommunicatorPtr ic = EasyIce::initialize(argc,argv);;
	Ice::PropertiesPtr prop = ic->getProperties();

	//0: Ice    other:ROS
	int driver = prop->getPropertyAsIntWithDefault("Cameraview.Server", 0);
	std::string topic = prop->getPropertyWithDefault("Cameraview.Topic", "usb_cam/image_raw");

	if (driver == 0) {   //Try to connect with ICE

		std::cout << "Receiving data from ICE interfaces" << std::endl;
		try{
		
			Ice::ObjectPrx base = ic->propertyToProxy("Cameraview.Camera.Proxy");


			if (0==base)
				throw "Could not create proxy";


			camRGB = new jderobot::cameraClient(ic,"Cameraview.Camera.");

			if (camRGB == NULL){
				throw "Invalid proxy";
			}
			camRGB->start();

			while(viewer->isVisible()){
				//jderobot::ImageDataPtr data = camRGB->getImageData(format);

				camRGB->getImage(cam_frame);
				viewer->display(cam_frame);
				viewer->displayFrameRate(camRGB->getRefreshRate());
			}
	
			camRGB->stop_thread();
			delete(camRGB);

		}catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
			status = 1;
		} catch (const char* msg) {
			std::cerr << msg << std::endl;
			status = 1;
		}
	}else{     //Try to connect ROS

		std::cout << "Receiving data from ROS messages" << std::endl;

		listenerCamera* l = new listenerCamera();
		pthread_t thr_info;
		pthread_create(&thr_info, NULL, &getInfoAsync, (void*) l);
		l->listen(argc,argv,topic);
		
	}

	if (ic)
		ic->destroy();

	return status;
}
