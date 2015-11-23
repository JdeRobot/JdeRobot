/*
 * poolWriteLasers.cpp
 *
 *  Created on: 10/05/2013
 *      Author: frivas
 */

#include "poolWriteLasers.h"

namespace recorder{
poolWriteLasers::poolWriteLasers(jderobot::LaserPrx prx, int freq, int poolSize, int laserID) {
	// TODO Auto-generated constructor stub
	pthread_mutex_init(&(this->mutex), NULL);
	this->poolSize=poolSize;
	this->laserID=laserID;
	this->active=true;
	this->prx=prx;
	this->freq=freq;
	std::stringstream filePath;
	filePath << "data/lasers/laser" << this->laserID << "/laserData.jde";
	this->cycle = 1000.0/freq;
	this->outfile.open(filePath.str().c_str());
	gettimeofday(&lastTime,NULL);
}

poolWriteLasers::~poolWriteLasers() {
	this->outfile.close();
	// TODO Auto-generated destructor stub
}

bool poolWriteLasers::getActive(){
	return this->active;
}

void poolWriteLasers::consumer_thread(){
//	while(this->active){
		//std::cout << "consumidor entro" << std::endl;

		pthread_mutex_lock(&(this->mutex));
		if (this->lasers.size()>0){
			//std::cout << " camara: " << cameraID <<  this->images.size()  << std::endl;

			std::vector<int> data2Save(lasers[0]);
			this->lasers.erase(this->lasers.begin());
			long long int relative;
			relative=*(this->its.begin());
			this->its.erase(this->its.begin());
			pthread_mutex_unlock(&(this->mutex));


			std::stringstream idString;//create a stringstream
			idString << this->laserID;//add number to the stream
			std::stringstream relativeString;//create a stringstream
			relativeString << relative;//add number to the stream


			std::string Path="data/lasers/laser" + idString.str() + "/" + relativeString.str();
			std::ofstream outfileBinary(Path.c_str(), std::ios::out | std::ios::binary);
			outfileBinary.write((const char *)&data2Save.front(), data2Save.size()*sizeof(int));
			outfileBinary.close();

			this->outfile << relative << " " << data2Save.size() <<   std::endl;
		}
		else
			pthread_mutex_unlock(&(this->mutex));
		usleep(1000);
		//std::cout << "consumidor salgo" << std::endl;

//	}
}

void poolWriteLasers::producer_thread(struct timeval inicio){
	//std::cout << "productor entro" << std::endl;
//	while(this->active){
		jderobot::LaserDataPtr dataPtr=this->prx->getLaserData();
		/*std::cout << "size: "<< dataPtr->distanceData.size() << std::endl;
		std::cout << "numLaser:" << dataPtr->numLaser << std::endl;*/

		struct timeval now;
		gettimeofday(&now,NULL);
		long long int relative;
		relative=((now.tv_sec*1000000+now.tv_usec) - (inicio.tv_sec*1000000+inicio.tv_usec))/1000;
		pthread_mutex_lock(&(this->mutex));
		while (this->lasers.size() > this->poolSize){
			pthread_mutex_unlock(&(this->mutex));
			usleep(100);
			pthread_mutex_lock(&(this->mutex));
		}
		this->lasers.push_back(dataPtr->distanceData);
		this->its.push_back(relative);
		pthread_mutex_unlock(&(this->mutex));
		gettimeofday(&now,NULL);

		long long int totalNow=now.tv_sec*1000000+now.tv_usec;
		long long int totalLast=lastTime.tv_sec*1000000+lastTime.tv_usec;

		float sleepTime =this->cycle - (totalNow-totalLast)/1000.;

		//std::cout << "productor: " << this->cameraID << ", sleep: " << sleepTime << std::endl;
		if(sleepTime < 0 )
			sleepTime = 0;
		usleep(sleepTime*1000);
		gettimeofday(&lastTime,NULL);
		//std::cout << "productor salgo" << std::endl;
//	}
}


} //namespace


