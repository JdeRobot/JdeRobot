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
#include "pointcloudClient.h"

namespace jderobot {

pointcloudClient::pointcloudClient(Ice::CommunicatorPtr ic, std::string prefix) {
	// TODO Auto-generated constructor stub

	Ice::PropertiesPtr prop;
	prop = ic->getProperties();

	int fps=prop->getPropertyAsIntWithDefault("kinectViewer.pointCloud.Fps",10);
	this->cycle=(float)(1/(float)fps)*1000000;
	try{
		Ice::ObjectPrx basePointCloud = ic->propertyToProxy("kinectViewer.pointCloud.Proxy");
		if (0==basePointCloud){
			throw "kinectViewer: Could not create proxy with Camera";
		}
		else {
			this->prx = jderobot::pointCloudPrx::checkedCast(basePointCloud);
			if (0==this->prx)
				throw "Invalid proxy kinectViewer.pointCloud.Proxy";

		}
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
		std::cout << "kinectViewer: Not camera provided" << std::endl;
	}

}

pointcloudClient::~pointcloudClient() {
	// TODO Auto-generated destructor stub
}

void pointcloudClient::run(){

	struct timeval post;
	long long int totalpre=0;
	long long int totalpost=0;
	while (1){
		gettimeofday(&post,NULL);
		totalpost=post.tv_sec*1000000+post.tv_usec;

		jderobot::pointCloudDataPtr localCloud=this->prx->getCloudData();

		this->controlMutex.lock();
		this->data.resize(localCloud->p.size());
		std::copy( localCloud->p.begin(), localCloud->p.end(), this->data.begin() );

		this->controlMutex.unlock();
		if (totalpre !=0){
			if ((totalpost - totalpre) > this->cycle ){
				std::cout<<"-------- kinectViewer: pointCloud adquisition timeout-" << std::endl;
			}
			else{
				usleep(this->cycle - (totalpost - totalpre));
			}
		}
		totalpre=totalpost;
	}
}

std::vector<jderobot::RGBPoint>  pointcloudClient::getData(){
	std::vector<jderobot::RGBPoint> cloud;
	this->controlMutex.lock();
	cloud.resize(this->data.size());
	std::copy( this->data.begin(), this->data.end(), cloud.begin() );
	this->controlMutex.unlock();
	return cloud;
}

} /* namespace jderobot */
