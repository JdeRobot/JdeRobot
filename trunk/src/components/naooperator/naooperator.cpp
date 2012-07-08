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

/** \file naooperator.cpp
 * \brief NaoOperator component master file
 */

/**
   \mainpage NaoOperator teleoperador JDE Component 5.0 for humanoid robots
   \author Francisco Miguel Rivas Montero
   \author Jose Maria Cañas Plaza
   \date 2010, December
	\version 5.0

   \par Readme:
	JDE-5.0 and Naobody component are nedeed to run this component

   \htmlonly
   <marquee scrollamount="5" scrolldelay="25"><font color=red>
    NaoOperator.</font></marquee>
   \endhtmlonly
   </ul>
*/






#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <jderobot/motors.h>
#include <jderobot/laser.h>
#include <jderobot/encoders.h>
#include <colorspaces/colorspacesmm.h>
#include "naooperatorgui.h"
#include "pthread.h"
#include "controllers/motors-controller.h"
#include "controllers/ptmotors-controller.h"
#include "controllers/ptencoders-controller.h"
#include "controllers/bodyencoders-controller.h"
#include "controllers/bodymotors-controller.h"


#define MAX_COMPONENTS 20	

naooperator::naooperatorgui* naooperatorgui_ptx;
Ice::ObjectPrx baseCamera;

/**
 * \brief Thread that performs the camera upgrade
 * \param arg: variable where any needed argument can be set (not used).
 */
void *camera_server(void* arg){

	try{
		jderobot::CameraPrx cprx = jderobot::CameraPrx::checkedCast(baseCamera);
		if (0==cprx)
			throw "Invalid proxy NaoOperator.Camera.Proxy";
		while(naooperatorgui_ptx->isVisible() && ! naooperatorgui_ptx->isClosed()){
			jderobot::ImageDataPtr data = cprx->getImageData();
			colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(data->description->format);
			if (!fmt)
				throw "Format not supported";
			colorspaces::Image image(data->description->width,data->description->height,fmt,&(data->pixelData[0]));
			naooperatorgui_ptx->update(image);
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
	int camera_active=0;
	pthread_attr_t attr;
	DevicesController::MotorsController *motors_ctr=NULL;
	DevicesController::PTMotorsController *ptmotors_ctr=NULL;
	DevicesController::PTEncodersController *ptencoders_ctr=NULL;
	DevicesController::BodyEncodersController *bodyencoders_ctr=NULL;
	DevicesController::BodyMotorsController *bodymotors_ctr=NULL;
	Ice::PropertiesPtr prop;

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
	if (prop->getPropertyAsIntWithDefault("NaoOperator.MotorsActive",0)){
		try{
			/*Contact to Motors proxy */
		    Ice::ObjectPrx baseMotors = ic->propertyToProxy("NaoOperator.Motors.Proxy");
		    if (0==baseMotors)
				throw "NaoOperator: Could not create proxy with motors";
		    /*cast to MotorsPrx*/
		    jderobot::MotorsPrx mprx = jderobot::MotorsPrx::checkedCast(baseMotors);
		    if (0==mprx)
				throw "NaoOperator: Invalid proxy NaoOperator.Motors.Proxy";
			/*creating all the devices controllers*/
			
			motors_ctr = new DevicesController::MotorsController(mprx);	
		}catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
			return 1;
		}
		catch (const char* msg) {
			std::cerr << msg << std::endl;
			return 1;
		}
	}
	if (prop->getPropertyAsIntWithDefault("NaoOperator.PTMotorsActive",0)){
		try{
			/*Contact to PTMotors proxy */
		    Ice::ObjectPrx basePTMotors = ic->propertyToProxy("NaoOperator.PTMotors.Proxy");
		    if (0==basePTMotors)
				throw "NaoOperator: Could not create proxy with PTMotors";
		    /*cast to PTMotorsPrx*/
		    jderobot::PTMotorsPrx mprx = jderobot::PTMotorsPrx::checkedCast(basePTMotors);
		    if (0==mprx)
				throw "NaoOperator: Invalid proxy NaoOperator.PTMotors.Proxy";
			/*creating all the devices controllers*/
			
			ptmotors_ctr = new DevicesController::PTMotorsController(mprx);	
		}catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
			return 1;
		}
		catch (const char* msg) {
			std::cerr << msg << std::endl;
			return 1;
		}
	}
	if (prop->getPropertyAsIntWithDefault("NaoOperator.PTEncodersActive",0)){
		try{
			/*Contact to PTEncoders proxy */
		    Ice::ObjectPrx basePTEncoders = ic->propertyToProxy("NaoOperator.PTEncoders.Proxy");
		    if (0==basePTEncoders)
				throw "NaoOperator: Could not create proxy with ptencoders";
		    /*cast to PTEncodersPrx*/
		    jderobot::PTEncodersPrx mprx = jderobot::PTEncodersPrx::checkedCast(basePTEncoders);
		    if (0==mprx)
				throw "NaoOperator: Invalid proxy NaoOperator.PTEncoders.Proxy";
			/*creating all the devices controllers*/
			
			ptencoders_ctr = new DevicesController::PTEncodersController(mprx);	
		}catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
			return 1;
		}
		catch (const char* msg) {
			std::cerr << msg << std::endl;
			return 1;
		}
	}
	if (prop->getPropertyAsIntWithDefault("NaoOperator.BodyEncodersActive",0)){
		try{
			/*Contact to BodyEncoders proxy */
		    Ice::ObjectPrx baseBodyEncoders = ic->propertyToProxy("NaoOperator.BodyEncoders.Proxy");
		    if (0==baseBodyEncoders)
				throw "NaoOperator: Could not create proxy with BodyEncoders";
		    /*cast to BodyEncodersPrx*/
		    jderobot::BodyEncodersPrx mprx = jderobot::BodyEncodersPrx::checkedCast(baseBodyEncoders);
		    if (0==mprx)
				throw "NaoOperator: Invalid proxy NaoOperator.BodyEncoders.Proxy";
			/*creating all the devices controllers*/
			
			bodyencoders_ctr = new DevicesController::BodyEncodersController(mprx);	
		}catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
			return 1;
		}
		catch (const char* msg) {
			std::cerr << msg << std::endl;
			return 1;
		}
	}
	if (prop->getPropertyAsIntWithDefault("NaoOperator.BodyMotorsActive",0)){
		try{
			/*Contact to BodyMotors proxy */
		    Ice::ObjectPrx baseBodyMotors = ic->propertyToProxy("NaoOperator.BodyMotors.Proxy");
		    if (0==baseBodyMotors)
				throw "NaoOperator: Could not create proxy with BodyMotors";
		    /*cast to BodyMotorsPrx*/
		    jderobot::BodyMotorsPrx mprx = jderobot::BodyMotorsPrx::checkedCast(baseBodyMotors);
		    if (0==mprx)
				throw "NaoOperator: Invalid proxy NaoOperator.BodyMotors.Proxy";
			/*creating all the devices controllers*/
			
			bodymotors_ctr = new DevicesController::BodyMotorsController(mprx);	
		}catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
			return 1;
		}
		catch (const char* msg) {
			std::cerr << msg << std::endl;
			return 1;
		}
	}
	naooperatorgui_ptx = new naooperator::naooperatorgui(motors_ctr, ptmotors_ctr, ptencoders_ctr,bodyencoders_ctr, bodymotors_ctr);
	if (prop->getPropertyAsIntWithDefault("NaoOperator.CameraActive",0)){
		try{
			baseCamera = ic->propertyToProxy("NaoOperator.Camera.Proxy");
			if (0==baseCamera){
				camera_active=0;
				throw "NaoOperator: Could not create proxy with Camera";
			}
			else {
				camera_active=1;
				pthread_create(&threads[n_components], &attr, camera_server,NULL);
				n_components++;
				
			}
		}catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
			status = 1;
		}
		catch (const char* msg) {
			std::cerr << msg << std::endl;
			std::cout << "NaoOperator: Not camera provided" << std::endl;
			status = 1;
		}
	}
	if (naooperatorgui_ptx == NULL)
		throw "NaoOperator: Could not create the grafic interface";
	for (i = 0; i < n_components; i++) {
		pthread_join(threads[i], NULL);
	}
	if (ic)
		ic->destroy();
	return status;
}
