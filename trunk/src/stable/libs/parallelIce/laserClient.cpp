/*
 * laserClient.cpp
 *
 *  Created on: 23/07/2013
 *      Author: frivas
 */

#include "laserClient.h"

namespace jderobot {

laserClient::laserClient(Ice::CommunicatorPtr ic, std::string prefix, bool debug) {
	// TODO Auto-generated constructor stub
	this->prefix=prefix;
	this->debug= debug;
	Ice::PropertiesPtr prop;
	prop = ic->getProperties();

	int fps=prop->getPropertyAsIntWithDefault(prefix+"Fps",10);
	this->cycle=(float)(1/(float)fps)*1000000;
	try{
		Ice::ObjectPrx basePointCloud = ic->propertyToProxy(prefix+"Proxy");
		if (0==basePointCloud){
			throw prefix + " Could not create proxy";
		}
		else {
			this->prx = jderobot::LaserPrx::checkedCast(basePointCloud);
			if (0==this->prx)
				throw "Invalid proxy" + prefix;

		}
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
		std::cout <<  prefix + " Not laser provided" << std::endl;
	}
	_done=false;

}

laserClient::~laserClient() {
	// TODO Auto-generated destructor stub
	this->_done=true;
}


void laserClient::pause(){
	this->pauseStatus=true;
}

void laserClient::resume(){
	this->controlMutex.lock();
		this->pauseStatus=false;
		this->sem.broadcast();
	this->controlMutex.unlock();
}

void laserClient::run(){

	IceUtil::Time last;

	last=IceUtil::Time::now();
	while (!(_done)){
		if (pauseStatus){
			IceUtil::Mutex::Lock sync(this->controlMutex);
			this->sem.wait(sync);
		}

		jderobot::LaserDataPtr localLaser=this->prx->getLaserData();

		this->controlMutex.lock();
		this->data.resize(localLaser->distanceData.size());
		std::copy( localLaser->distanceData.begin(), localLaser->distanceData.end(), this->data.begin() );

		this->controlMutex.unlock();

		if ((IceUtil::Time::now().toMicroSeconds() - last.toMicroSeconds()) > this->cycle ){
			if (this->debug)
				std::cout<< prefix << ": pointCloud adquisition timeout-" << std::endl;
		}
		else{
			usleep(this->cycle - (IceUtil::Time::now().toMicroSeconds() - last.toMicroSeconds()));
		}
		this->refreshRate=(int)(1000000/(IceUtil::Time::now().toMicroSeconds() - last.toMicroSeconds()));
		last=IceUtil::Time::now();
	}
}

std::vector<int>  laserClient::getData(){
	std::vector<int> laser;
	this->controlMutex.lock();
	laser.resize(this->data.size());
	std::copy( this->data.begin(), this->data.end(), laser.begin() );
	this->controlMutex.unlock();
	return laser;
}

} /* namespace eldercare */
