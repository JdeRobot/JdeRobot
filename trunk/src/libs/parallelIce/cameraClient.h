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

#ifndef CAMERACLIENT_H_
#define CAMERACLIENT_H_

#include <IceUtil/IceUtil.h>
#include <iostream>
#include <Ice/Ice.h>
#include <colorspaces/colorspacesmm.h>
#include <jderobot/camera.h>
#include <cv.h>
#include <sstream>
#include <fstream>


namespace jderobot {

class cameraClient: public IceUtil::Thread {
public:
	cameraClient(Ice::CommunicatorPtr ic, std::string prefix, bool debug);
	virtual ~cameraClient();
	virtual void run();

	//callbacks
	cv::Mat getImage();
private:
	cv::Mat data;
	jderobot::CameraPrx prx;
	long long int cycle;
	//int type; //0 color 1 depth
	IceUtil::Mutex controlMutex;
	std::string prefix;
	bool debug;

};

} /* namespace jderobot */
#endif /* CAMERACLIENT_H_ */
