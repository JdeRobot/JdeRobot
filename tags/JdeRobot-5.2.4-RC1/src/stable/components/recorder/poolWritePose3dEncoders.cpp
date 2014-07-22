/*
 * poolWritePose3dEncoders.cpp
 *
 *  Created on: 10/05/2013
 *      Author: frivas
 */

#include "poolWritePose3dEncoders.h"

namespace recorder{


poolWritePose3dEncoders::poolWritePose3dEncoders(jderobot::Pose3DEncodersPrx prx, int freq, int poolSize, int encoderID) {
	// TODO Auto-generated constructor stub
	pthread_mutex_init(&(this->mutex), NULL);
	this->poolSize=poolSize;
	this->encoderID=encoderID;
	this->active=true;
	this->prx=prx;
	this->freq=freq;
	std::stringstream filePath;
	filePath << "data/pose3dencoders/pose3dencoder" << this->encoderID << "/pose3dencoderData.jde";
	this->cycle = 1000.0/freq;
	this->outfile.open(filePath.str().c_str());
	gettimeofday(&lastTime,NULL);
}

poolWritePose3dEncoders::~poolWritePose3dEncoders() {
	this->outfile.close();
	// TODO Auto-generated destructor stub
}

bool poolWritePose3dEncoders::getActive(){
	return this->active;
}

void poolWritePose3dEncoders::consumer_thread(){
//	while(this->active){


		pthread_mutex_lock(&(this->mutex));
		if (this->encoders.size()>0){
			//std::cout << " camara: " << cameraID <<  this->images.size()  << std::endl;

			recorder::pose3dencoders data2Save;
			data2Save = this->encoders[0];
			this->encoders.erase(this->encoders.begin());
			long long int relative;
			relative=*(this->its.begin());
			this->its.erase(this->its.begin());
			pthread_mutex_unlock(&(this->mutex));


			std::stringstream idString;//create a stringstream
			idString << this->encoderID;//add number to the stream
			std::stringstream relativeString;//create a stringstream
			relativeString << relative;//add number to the stream


			std::string Path="data/pose3dencoders/pose3dencoder" + idString.str() + "/" + relativeString.str();
			std::ofstream outfileBinary(Path.c_str(), std::ios::out | std::ios::binary);
			outfileBinary.write((const char *)&data2Save, sizeof(recorder::pose3dencoders));
			outfileBinary.close();

			this->outfile << relative <<   std::endl;

		}
		else
			pthread_mutex_unlock(&(this->mutex));
		usleep(1000);


//	}
}

void poolWritePose3dEncoders::producer_thread(struct timeval inicio){
	//std::cout << "productor entro" << std::endl;
//	while(this->active){
		jderobot::Pose3DEncodersDataPtr dataPtr=this->prx->getPose3DEncodersData();
		recorder::pose3dencoders data;
		data.clock=dataPtr->clock;
		data.maxPan=dataPtr->maxPan;
		data.maxTilt=dataPtr->maxTilt;
		data.minPan=dataPtr->minPan;
		data.minTilt=dataPtr->minTilt;
		data.pan=dataPtr->pan;
		data.roll=dataPtr->roll;
		data.tilt=dataPtr->tilt;
		data.x=dataPtr->x;
		data.y=dataPtr->y;
		data.z=dataPtr->z;

		struct timeval now;
		gettimeofday(&now,NULL);
		long long int relative;
		relative=((now.tv_sec*1000000+now.tv_usec) - (inicio.tv_sec*1000000+inicio.tv_usec))/1000;
		pthread_mutex_lock(&(this->mutex));
		while (this->encoders.size() > this->poolSize){
			pthread_mutex_unlock(&(this->mutex));
			usleep(100);
			pthread_mutex_lock(&(this->mutex));
		}
		this->encoders.push_back(data);
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
