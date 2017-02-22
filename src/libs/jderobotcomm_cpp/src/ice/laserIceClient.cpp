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
#include "jderobot/comm/ice/laserIceClient.hpp"

namespace JdeRobotComm {

LaserIceClient::LaserIceClient(Ice::CommunicatorPtr ic, std::string prefix) {

	this->prefix=prefix;
	Ice::PropertiesPtr prop;
	prop = ic->getProperties();
	this->refreshRate=0;
	int fps=prop->getPropertyAsIntWithDefault(prefix+".Fps",10);
	this->cycle=(float)(1/(float)fps)*1000000;


	Ice::ObjectPrx baseLaser = ic->propertyToProxy(prefix+".Proxy");

	if (0==baseLaser){
		this->on = false;
		std::cout << prefix + ".Proxy configuration not specified" <<std::endl;

	}
	else {

		try{
			this->prx = jderobot::LaserPrx::checkedCast(baseLaser);

			if (0 == this->prx){
				this->on = false;
	 	   		std::cout <<"Invalid proxy "+ prefix + ".Proxy" <<std::endl;
	 		}else{
	 			this->on = true;
	 			std::cout << prefix + " connected" << std::endl;
	 		}

		
		}catch (const Ice::ConnectionRefusedException& e) {
			std::cout << prefix +" inactive" << std::endl;
		}
		catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
		}
	}

	this->pauseStatus=false;

}

LaserIceClient::~LaserIceClient() {
	this->on=false;
}


void LaserIceClient::pause(){
	std::cout << "Paused" << std::endl;
	this->pauseStatus=true;
}

void LaserIceClient::resume(){
	std::cout << "resumed" << std::endl;
	this->controlMutex.lock();
		this->pauseStatus=false;
		this->sem.broadcast();
	this->controlMutex.unlock();
}

void LaserIceClient::run(){

	IceUtil::Time last;

	last=IceUtil::Time::now();
	while (this->on){
		if (pauseStatus){
			IceUtil::Mutex::Lock sync(this->controlMutex);
			this->sem.wait(sync);
		}

		try{
			jderobot::LaserDataPtr localLaser=this->prx->getLaserData();

			this->controlMutex.lock();
			this->laserData.minRange = localLaser->minRange/1000.0;
    		this->laserData.maxRange = localLaser->maxRange/1000.0;
    		this->laserData.minAngle = localLaser->minAngle;
    		this->laserData.maxAngle = localLaser->maxAngle;
    		this->laserData.values.resize(localLaser->numLaser);
	        for(int i = 0; i< localLaser->numLaser; i++ ){
	            this->laserData.values[i] = localLaser->distanceData[i]/1000.0;
	        }
			//this->laserData.values.resize(localLaser->distanceData.size());
			//std::copy( localLaser->distanceData.begin(), localLaser->distanceData.end(), this->laserData.values.begin() );

			this->controlMutex.unlock();
		}
		catch(...){
			jderobot::Logger::getInstance()->warning(prefix +"error during request (connection error)");
			usleep(5000);

		}


		if ((IceUtil::Time::now().toMicroSeconds() - last.toMicroSeconds()) > this->cycle ){
			jderobot::Logger::getInstance()->warning(prefix + ": Laser adquisition timeout-");
		}
		else{
			usleep(this->cycle - (IceUtil::Time::now().toMicroSeconds() - last.toMicroSeconds()));
		}
		this->refreshRate=(int)(1000000/(IceUtil::Time::now().toMicroSeconds() - last.toMicroSeconds()));
		last=IceUtil::Time::now();
	}
}

JdeRobotTypes::LaserData  LaserIceClient::getLaserData(){
	JdeRobotTypes::LaserData laser;
	this->controlMutex.lock();
	laser = this->laserData;
	this->controlMutex.unlock();
	return laser;
}

} /* NS */