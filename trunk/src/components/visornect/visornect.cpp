/*
 *  Copyright (C) 1997-2010 JDE Developers Team
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
 *  Authors : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <fm.rivas@alumnos.urjc.es>
			
 */

/** \file visornect.cpp
 * \brief VisorNect component master file
 */

/**
   \mainpage VisorNect teleoperador JDE Component 5.0 for humanoid robots
   \author Francisco Miguel Rivas Montero
   \author Jose Maria Cañas Plaza
   \date 2010, December
	\version 5.0

   \par Readme:
	JDE-5.0 and Naobody component are nedeed to run this component

   \htmlonly
   <marquee scrollamount="5" scrolldelay="25"><font color=red>
    VisorNect.</font></marquee>
   \endhtmlonly
   </ul>
*/






#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/kinect.h>
#include <jderobot/kinectleds.h>
#include <colorspaces/colorspacesmm.h>
#include "visornectgui.h"
#include "pthread.h"



#define MAX_COMPONENTS 20	

visornect::visornectgui* visornectgui_ptx;
Ice::ObjectPrx baseCameraRGB;
Ice::ObjectPrx baseCameraDEPTH;


/**
 * \brief Thread that performs the camera upgrade
 * \param arg: variable where any needed argument can be set (not used).
 */
void *camera_server(void* arg){

	try{
		jderobot::KinectPrx cDEPTHprx = jderobot::KinectPrx::checkedCast(baseCameraDEPTH);
		jderobot::KinectPrx cRGBprx = jderobot::KinectPrx::checkedCast(baseCameraRGB);
		
		if (0==cDEPTHprx)
			throw "Invalid proxy VisorNect.CameraDEPTH.Proxy";
		if (0==cRGBprx)
			throw "Invalid proxy VisorNect.CameraRGB.Proxy";
		visornectgui_ptx->setCameraPrx(cRGBprx);
		while(visornectgui_ptx->isVisible() && ! visornectgui_ptx->isClosed()){
			jderobot::ImageDataPtr dataRGB = cRGBprx->getImageData();
			jderobot::ImageDataPtr dataDEPTH = cDEPTHprx->getImageData();
			colorspaces::Image::FormatPtr fmtRGB = colorspaces::Image::Format::searchFormat(dataRGB->description->format);
			colorspaces::Image::FormatPtr fmtDEPTH = colorspaces::Image::Format::searchFormat(dataDEPTH->description->format);
			if (!fmtRGB)
				throw "Format not supported";
			if (!fmtDEPTH)
				throw "Format not supported";
			colorspaces::Image imageRGB(dataRGB->description->width,dataRGB->description->height,fmtRGB,&(dataRGB->pixelData[0]));
			colorspaces::Image imageDEPTH(dataDEPTH->description->width,dataDEPTH->description->height,fmtRGB,&(dataDEPTH->pixelData[0]));
			visornectgui_ptx->update(imageRGB,imageDEPTH);
			//usleep(0.06);
		}
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
	}
	pthread_exit(NULL);
}

/**
 * \brief Main program function code
 */
int main(int argc, char** argv){

	int status,i;
	Ice::CommunicatorPtr ic;
	int n_components=0;
	pthread_t threads[MAX_COMPONENTS];
	pthread_attr_t attr;
	Ice::PropertiesPtr prop;
	int camera_active=0;
	jderobot::PTMotorsPrx mprx;
	jderobot::KinectLedsPrx lprx;
	std::string path;
	

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	try{
		ic = Ice::initialize(argc,argv);
		prop = ic->getProperties();
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		return 1;
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
		return 1;
	}
	if (prop->getPropertyAsIntWithDefault("VisorNect.CameraRGBActive",0)){
		try{
			baseCameraRGB = ic->propertyToProxy("VisorNect.CameraRGB.Proxy");
			if (0==baseCameraRGB){
				throw "VisorNect: Could not create proxy with Camera";
			}
			else {
				camera_active=1;
				//pthread_create(&threads[n_components], &attr, camera_server,NULL);
				//n_components++;
				
			}
		}catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
			status = 1;
		}
		catch (const char* msg) {
			std::cerr << msg << std::endl;
			std::cout << "VisorNect: Not camera provided" << std::endl;
			status = 1;
		}
	}
	if (prop->getPropertyAsIntWithDefault("VisorNect.CameraDEPTHActive",0)){
		try{
			baseCameraDEPTH = ic->propertyToProxy("VisorNect.CameraDEPTH.Proxy");
			if (0==baseCameraDEPTH){
				throw "VisorNect: Could not create proxy with Camera";
			}
			else {
				camera_active=1;
				//pthread_create(&threads[n_components], &attr, camera_server,NULL);
				//n_components++;
				
			}
		}catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
			status = 1;
		}
		catch (const char* msg) {
			std::cerr << msg << std::endl;
			std::cout << "VisorNect: Not camera provided" << std::endl;
			status = 1;
		}
	}
	if (prop->getPropertyAsIntWithDefault("VisorNect.PTMotorsActive",0)){
		try{
			/*Contact to PTMotors proxy */
		    Ice::ObjectPrx basePTMotors = ic->propertyToProxy("VisorNect.PTMotors.Proxy");
		    if (0==basePTMotors)
				throw "VisorNect: Could not create proxy with PTMotors";
		    /*cast to PTMotorsPrx*/
		    mprx = jderobot::PTMotorsPrx::checkedCast(basePTMotors);
		    if (0==mprx)
				throw "VisorNect: Invalid proxy VisorNect.PTMotors.Proxy";
			/*creating all the devices controllers*/
			
		}catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
			return 1;
		}
		catch (const char* msg) {
			std::cerr << msg << std::endl;
			return 1;
		}
	}
	if (prop->getPropertyAsIntWithDefault("VisorNect.KinectLedsActive",0)){
		try{
			/*Contact to KinectLeds proxy */
		    Ice::ObjectPrx baseKinectLeds = ic->propertyToProxy("VisorNect.KinectLeds.Proxy");
		    if (0==baseKinectLeds)
				throw "VisorNect: Could not create proxy with KinectLeds";
		    /*cast to KinectLedsPrx*/
		    lprx = jderobot::KinectLedsPrx::checkedCast(baseKinectLeds);
		    if (0==lprx)
				throw "VisorNect: Invalid proxy VisorNect.KinectLeds.Proxy";
			/*creating all the devices controllers*/
			
		}catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
			return 1;
		}
		catch (const char* msg) {
			std::cerr << msg << std::endl;
			return 1;
		}
	}

	visornectgui_ptx = new visornect::visornectgui(mprx, lprx, prop->getProperty("VisorNect.WorldFile"));
	n_components++;
	if (camera_active){
		pthread_create(&threads[0], &attr, camera_server,NULL);
	}
	if (visornectgui_ptx == NULL)
		throw "VisorNect: Could not create the grafic interface";
	for (i = 0; i < n_components; i++) {
		pthread_join(threads[i], NULL);
	}
	if (ic)
		ic->destroy();
	return status;
}
