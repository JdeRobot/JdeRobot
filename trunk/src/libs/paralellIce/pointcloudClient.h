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


#ifndef POINTCLOUDCLIENT_H_
#define POINTCLOUDCLIENT_H_

#include <IceUtil/IceUtil.h>
#include <iostream>
#include <Ice/Ice.h>
#include <jderobot/pointcloud.h>
#include <cv.h>
#include <sstream>
#include <fstream>

namespace jderobot {

class pointcloudClient: public IceUtil::Thread {
public:
	pointcloudClient(Ice::CommunicatorPtr ic, std::string prefix);
	virtual ~pointcloudClient();
	virtual void run();

	std::vector<jderobot::RGBPoint>  getData();


private:
	std::vector<jderobot::RGBPoint> data;
	jderobot::pointCloudPrx prx;
	long long int cycle;
	IceUtil::Mutex controlMutex;

};

} /* namespace jderobot */
#endif /* POINTCLOUDCLIENT_H_ */
