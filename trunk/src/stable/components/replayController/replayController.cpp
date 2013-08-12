/*
 *  Copyright (C) 1997-2013 JDE Developers TeamkinectViewer.camRGB
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
 *  Author : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>

 */

#include <jderobot/replayControl.h>
#include "replayControllerGui.h"

#include <pthread.h>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

int main(int argc, char** argv){
	Ice::CommunicatorPtr ic;
	Ice::PropertiesPtr prop;
	std::string prefix("replayController.");
	jderobot::replayControlPrx prx;

	try{
		ic = Ice::initialize(argc,argv);
		prop = ic->getProperties();
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		return 1;
	}
	catch (const char* msg) {
		std::cerr <<"Error :" << msg << std::endl;
		return 1;
	}


	try{
		Ice::ObjectPrx base = ic->propertyToProxy(prefix+"control.Proxy");
		if (0==base){
			throw "replayController: Could not create proxy with replayControl";
		}
		else {
			prx= jderobot::replayControlPrx::checkedCast(base);
			if (0==prx)
				throw "Invalid " + prefix + ".Proxy";
		}
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
		std::cout << "replayController: Not camera provided" << std::endl;
	}

	int ips=prop->getPropertyAsIntWithDefault(prefix+"IPS",10);
	float cycle=(float)(1/(float)ips)*1000000;

	replayController::replayControllergui* gui = new replayController::replayControllergui(prx);
	struct timeval post;
	long long int totalpre=0;
	long long int totalpost=0;

	while (gui->update()){
		gettimeofday(&post,NULL);
		totalpost=post.tv_sec*1000000+post.tv_usec;
		if (totalpre !=0){
			if ((totalpost - totalpre) > cycle ){
				std::cout<<"-------- replayController: camera adquisition timeout-" << std::endl;
			}
			else{
				usleep(cycle - (totalpost - totalpre));
			}
		}
		totalpre=totalpost;

	}


}
