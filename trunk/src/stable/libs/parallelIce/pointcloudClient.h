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
#include <log/Logger.h>

namespace jderobot {

class pointcloudClient: public IceUtil::Thread {
public:
	pointcloudClient(Ice::CommunicatorPtr ic, std::string prefix, bool debug);
	virtual ~pointcloudClient();
	virtual void run();

	std::vector<jderobot::RGBPoint>  getData();
	int getRefreshRate(){return refreshRate;};
	void pause();
	void resume();
	bool getPause(){return pauseStatus;};


private:
	std::string prefix;
	std::vector<jderobot::RGBPoint> data;
	jderobot::pointCloudPrx prx;
	long long int cycle;
	IceUtil::Mutex controlMutex;
	bool debug;
	bool _done;
	int refreshRate;
	bool pauseStatus;

	IceUtil::Cond sem;

};

} /* namespace jderobot */
#endif /* POINTCLOUDCLIENT_H_ */
