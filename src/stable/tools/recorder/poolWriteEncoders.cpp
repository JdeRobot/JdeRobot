/*
 * poolWriteEncoders.cpp
 *
 *  Created on: 23/07/2013
 *      Author: frivas
 */

#include "poolWriteEncoders.h"

namespace recorder {

poolWriteEncoders::poolWriteEncoders(jderobot::EncodersPrx prx, int freq, int poolSize, int encoderID) {
	// TODO Auto-generated constructor stub
	pthread_mutex_init(&(this->mutex), NULL);
	this->poolSize=poolSize;
	this->encoderID=encoderID;
	this->active=true;
	this->prx=prx;
	this->freq=freq;
	std::stringstream filePath;
	filePath << "data/encoders/encoder" << this->encoderID << "/encoderData.jde";
	this->cycle = 1000.0/freq;
	this->outfile.open(filePath.str().c_str());
	gettimeofday(&lastTime,NULL);
}

poolWriteEncoders::~poolWriteEncoders() {
	this->outfile.close();
	// TODO Auto-generated destructor stub
}

bool poolWriteEncoders::getActive(){
	return this->active;
}

void poolWriteEncoders::consumer_thread(){
//	while(this->active){


		pthread_mutex_lock(&(this->mutex));
		if (this->encoders.size()>0){
			//std::cout << " camara: " << cameraID <<  this->images.size()  << std::endl;

			recorder::encoders data2Save;
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


			std::string Path="data/encoders/encoder" + idString.str() + "/" + relativeString.str();
			std::ofstream outfileBinary(Path.c_str(), std::ios::out | std::ios::binary);
			outfileBinary.write((const char *)&data2Save, sizeof(recorder::encoders));
			outfileBinary.close();

			this->outfile << relative <<   std::endl;

		}
		else
			pthread_mutex_unlock(&(this->mutex));
		usleep(1000);


//	}
}

void poolWriteEncoders::producer_thread(struct timeval inicio){
	//std::cout << "productor entro" << std::endl;
//	while(this->active){
		jderobot::EncodersDataPtr dataPtr=this->prx->getEncodersData();
		recorder::encoders data;

		data.robotx=dataPtr->robotx;
		data.roboty=dataPtr->roboty;
		data.robottheta=dataPtr->robottheta;
		data.robotcos=dataPtr->robotcos;
		data.robotsin=dataPtr->robotsin;

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

} /* namespace gbRGBD2PC */
