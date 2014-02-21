/*
 * poolWriteImages.cpp
 *
 *  Created on: 03/05/2013
 *      Author: frivas
 */

#include "poolWriteImages.h"

namespace recorder{
poolWriteImages::poolWriteImages(jderobot::CameraPrx prx, int freq, int poolSize, int cameraID,  std::string imageFormat, std::vector<int> compression_params) {
	// TODO Auto-generated constructor stub
	pthread_mutex_init(&(this->mutex), NULL);
	this->poolSize=poolSize;
	this->compression_params=compression_params;
	this->cameraID=cameraID;
	this->active=true;
	this->imageFormat=imageFormat;
	this->prx=prx;
	this->freq=freq;
	std::stringstream filePath;
	filePath << "data/images/camera" << this->cameraID << "/cameraData.jde";
	this->cycle = 1000.0/freq;
	this->outfile.open(filePath.str().c_str());
	gettimeofday(&lastTime,NULL);
}

poolWriteImages::~poolWriteImages() {
	this->outfile.close();
	// TODO Auto-generated destructor stub
}

bool poolWriteImages::getActive(){
	return this->active;
}

void poolWriteImages::consumer_thread(){
//	while(this->active){
		//std::cout << "consumidor entro" << std::endl;

		pthread_mutex_lock(&(this->mutex));
		if (this->images.size()>0){
			//std::cout << " camara: " << cameraID <<  this->images.size()  << std::endl;
			cv::Mat img2Save;

			this->images.begin()->copyTo(img2Save);
			this->images.erase(this->images.begin());

			long long int relative;
			relative=*(this->its.begin());
			this->its.erase(this->its.begin());
			pthread_mutex_unlock(&(this->mutex));

			std::stringstream buff;//create a stringstream

			buff << "data/images/camera" << cameraID << "/" << relative << "." << imageFormat;
			cv::imwrite(buff.str(), img2Save,this->compression_params);
			this->outfile << relative<< std::endl;
		}
		else
			pthread_mutex_unlock(&(this->mutex));
		usleep(1000);
		//std::cout << "consumidor salgo" << std::endl;

//	}
}

void poolWriteImages::producer_thread( struct timeval inicio){
//(cv::Mat image, long long int it){
	//std::cout << "productor entro" << std::endl;
//	while(this->active){
		jderobot::ImageDataPtr imageData=this->prx->getImageData();
		colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(imageData->description->format);
		cv::Mat image;
		image.create(cv::Size(imageData->description->width, imageData->description->height), CV_8UC3);
		memcpy((unsigned char *) image.data ,&(imageData->pixelData[0]), image.cols*image.rows * 3);
		struct timeval now;
		gettimeofday(&now,NULL);
		long long int relative;
		relative=((now.tv_sec*1000000+now.tv_usec) - (inicio.tv_sec*1000000+inicio.tv_usec))/1000;
		pthread_mutex_lock(&(this->mutex));
		while (this->images.size() > this->poolSize){
			pthread_mutex_unlock(&(this->mutex));
			usleep(100);
			pthread_mutex_lock(&(this->mutex));
		}
		this->images.push_back(image);
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

