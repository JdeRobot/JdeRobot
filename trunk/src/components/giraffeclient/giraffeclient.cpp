/*
 *  Copyright (C) 2010 Eduardo Perdices García
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *   Authors : Eduardo Perdices García <eperdices@gsyc.es>,
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <colorspaces/colorspacesmm.h>
#include "view.h"
#include <jderobot/jointmotor.h>

using namespace std;

int main(int argc, char** argv){

	int status;
	giraffeClient::View * view;
	giraffeClient::Controller * controller;
	Ice::CommunicatorPtr ic;
	RoboCompJointMotor::MotorParamsList motorsparams;
	RoboCompJointMotor::MotorStateMap motorsstate;

	try{
		ic = Ice::initialize(argc,argv);
		/*Get to giraffe server*/
		Ice::ObjectPrx base1 = ic->propertyToProxy("giraffeClient.JointMotor.Proxy");
		if (0==base1)
			throw "Could not create proxy to giraffe server";

		/*cast to JointMotorPrx*/
		RoboCompJointMotor::JointMotorPrx jprx = RoboCompJointMotor::JointMotorPrx::checkedCast(base1);
		if (0==jprx)
			throw "Invalid proxy";

		/*Get driver camera*/		
		Ice::ObjectPrx base2 = ic->propertyToProxy("giraffeClient.Camera.Proxy");
		if (0==base2)
			throw "Could not create proxy to camera server";

		/*cast to CameraPrx*/
		jderobot::CameraPrx cprx = jderobot::CameraPrx::checkedCast(base2);
		if (0==cprx)
			throw "Invalid proxy";	

		/*Create Controller and View*/
		controller = new giraffeClient::Controller(jprx);
		view = new giraffeClient::View(controller);

		/*Show params of motors*/
		motorsparams = jprx->getAllMotorParams();

		cout << "Motors params:" << endl;
		for(vector<RoboCompJointMotor::MotorParams>::iterator it = motorsparams.begin(); it != motorsparams.end(); it++) {
			cout << endl;
			cout << "Name: " << (*it).name << endl;
			cout << "Id: " << (int) (*it).busId << endl;
			cout << "minPos: " << (*it).minPos << endl;
			cout << "maxPos: " << (*it).maxPos << endl;
			cout << "maxVel: " << (*it).maxVelocity << endl;
			cout << "zeroPos: " << (*it).zeroPos << endl;
			cout << "inverted: " << (*it).invertedSign << endl;
		}

		/*Get current pos of motors*/
		jprx->getAllMotorState(motorsstate);
		view->setInitialValues(motorsstate["neck"].pos, motorsstate["tilt"].pos, motorsstate["leftPan"].pos, motorsstate["rightPan"].pos);
		view->setRealValues(motorsstate["neck"].pos, motorsstate["tilt"].pos, motorsstate["leftPan"].pos, motorsstate["rightPan"].pos);

		while(view->isVisible()){

			/*Get status of motors*/
			jprx->getAllMotorState(motorsstate);

			view->setRealValues(motorsstate["neck"].pos, motorsstate["tilt"].pos, motorsstate["leftPan"].pos, motorsstate["rightPan"].pos);

			/*Get image*/
      jderobot::ImageDataPtr data = cprx->getImageData();
      colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(data->description->format);
      if (!fmt)
				throw "Format not supported";

      colorspaces::Image image(data->description->width,
			       data->description->height,
			       fmt,
			       &(data->pixelData[0]));

      view->display(image);
			usleep(10*1000);
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
