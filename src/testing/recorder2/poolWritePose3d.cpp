/*
 * poolWritePose3d.cpp
 *
 *  Created on: 10/05/2013
 *      Author: frivas
 */

#include "poolWritePose3d.h"

namespace recorder{


poolWritePose3d::poolWritePose3d(jderobot::Pose3DPrx prx, int freq, int poolSize, int pose3dID) {
	// TODO Auto-generated constructor stub
	pthread_mutex_init(&(this->mutex), NULL);
	this->poolSize=poolSize;
	this->pose3dID=pose3dID;
	this->active=true;
	this->prx=prx;
	this->freq=freq;
	std::stringstream filePath;
	filePath << "data/pose3d/pose3d" << this->pose3dID << "/pose3dData.jde";
	this->cycle = 1000.0/freq;
	this->outfile.open(filePath.str().c_str());
	gettimeofday(&lastTime,NULL);
}

poolWritePose3d::~poolWritePose3d() {
	this->outfile.close();
	// TODO Auto-generated destructor stub
}

bool poolWritePose3d::getActive(){
	return this->active;
}

void poolWritePose3d::consumer_thread(){
//	while(this->active){


		pthread_mutex_lock(&(this->mutex));
		if (this->pose3dVec.size()>0){
			//std::cout << " camara: " << cameraID <<  this->images.size()  << std::endl;

			recorder::pose3d data2Save;
			data2Save = this->pose3dVec[0];
			this->pose3dVec.erase(this->pose3dVec.begin());
			long long int relative;
			relative=*(this->its.begin());
			this->its.erase(this->its.begin());
			pthread_mutex_unlock(&(this->mutex));


			std::stringstream idString;//create a stringstream
			idString << this->pose3dID;//add number to the stream
			std::stringstream relativeString;//create a stringstream
			relativeString << relative;//add number to the stream


			std::string Path="data/pose3d/pose3d" + idString.str() + "/" + relativeString.str();
			std::ofstream outfileBinary(Path.c_str(), std::ios::out | std::ios::binary);
			outfileBinary.write((const char *)&data2Save, sizeof(recorder::pose3d));
			outfileBinary.close();

			this->outfile << relative <<   std::endl;

		}
		else
			pthread_mutex_unlock(&(this->mutex));
		usleep(1000);


//	}
}

void poolWritePose3d::producer_thread(struct timeval inicio){
	//std::cout << "productor entro" << std::endl;
//	while(this->active){
		jderobot::Pose3DDataPtr dataPtr=this->prx->getPose3DData();
		recorder::pose3d data;
		data.q0=dataPtr->q0;
		data.q1=dataPtr->q1;
		data.q2=dataPtr->q2;
		data.q3=dataPtr->q3;
		data.x=dataPtr->x;
		data.y=dataPtr->y;
		data.z=dataPtr->z;
		data.h=dataPtr->h;
		//std::cout<<"x: "<<dataPtr->x<<", y: "<<dataPtr->y<<", z: "<<dataPtr->z<<std::endl;
		struct timeval now;
		gettimeofday(&now,NULL);
		long long int relative;
		relative=((now.tv_sec*1000000+now.tv_usec) - (inicio.tv_sec*1000000+inicio.tv_usec))/1000;
		pthread_mutex_lock(&(this->mutex));
		while (this->pose3dVec.size() > this->poolSize){
			pthread_mutex_unlock(&(this->mutex));
			usleep(100);
			pthread_mutex_lock(&(this->mutex));
		}
		this->pose3dVec.push_back(data);
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
