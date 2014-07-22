/*
 * poolWritePointCloud.cpp
 *
 *  Created on: 11/05/2013
 *      Author: frivas
 */

#include "poolWritePointCloud.h"

namespace recorder {

poolWritePointCloud::poolWritePointCloud(jderobot::pointCloudPrx prx, int freq, int poolSize, int pclID) {
	// TODO Auto-generated constructor stub
	pthread_mutex_init(&(this->mutex), NULL);
	this->poolSize=poolSize;
	this->pclID=pclID;
	this->active=true;
	this->prx=prx;
	this->freq=freq;
	std::stringstream filePath;
	filePath << "data/pointClouds/pointCloud" << this->pclID << "/pointCloudData.jde";
	this->cycle = 1000.0/freq;
	this->outfile.open(filePath.str().c_str());
	gettimeofday(&lastTime,NULL);
}

poolWritePointCloud::~poolWritePointCloud() {
	this->outfile.close();
	// TODO Auto-generated destructor stub
}

bool poolWritePointCloud::getActive(){
	return this->active;
}

void poolWritePointCloud::consumer_thread(){
//	while(this->active){
		//std::cout << "consumidor entro" << std::endl;

		pthread_mutex_lock(&(this->mutex));
		if (this->pointCloud.size()>0){
			//std::cout << " camara: " << cameraID <<  this->images.size()  << std::endl;

			std::vector<jderobot::RGBPoint> data2Save(pointCloud[0]);
			this->pointCloud.erase(this->pointCloud.begin());
			long long int relative;
			relative=*(this->its.begin());
			this->its.erase(this->its.begin());
			pthread_mutex_unlock(&(this->mutex));


			std::stringstream idString;//create a stringstream
			idString << this->pclID;//add number to the stream
			std::stringstream relativeString;//create a stringstream
			relativeString << relative;//add number to the stream


			std::string Path="data/pointClouds/pointCloud" + idString.str() + "/" + relativeString.str();
			std::ofstream outfileBinary(Path.c_str(), std::ios::out | std::ios::binary);
			outfileBinary.write((const char *)&data2Save.front(), data2Save.size()*sizeof(jderobot::RGBPoint));
			outfileBinary.close();

			this->outfile << relative << " " << data2Save.size() <<   std::endl;
		}
		else
			pthread_mutex_unlock(&(this->mutex));
		usleep(1000);
		//std::cout << "consumidor salgo" << std::endl;

//	}
}

void poolWritePointCloud::producer_thread(struct timeval inicio){
	//std::cout << "productor entro" << std::endl;
//	while(this->active){
		jderobot::pointCloudDataPtr dataPtr=this->prx->getCloudData();


		struct timeval now;
		gettimeofday(&now,NULL);
		long long int relative;
		relative=((now.tv_sec*1000000+now.tv_usec) - (inicio.tv_sec*1000000+inicio.tv_usec))/1000;
		pthread_mutex_lock(&(this->mutex));
		while (this->pointCloud.size() > this->poolSize){
			pthread_mutex_unlock(&(this->mutex));
			usleep(100);
			pthread_mutex_lock(&(this->mutex));
		}
		this->pointCloud.push_back(dataPtr->p);
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

} /* namespace recorder */
