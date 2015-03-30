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
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "cameraClient.h"
#include <zlib.h>


namespace jderobot {


cameraClient::cameraClient(Ice::CommunicatorPtr ic, std::string prefix) {

	this->prefix=prefix;
	Ice::PropertiesPtr prop;
	prop = ic->getProperties();
	Ice::ObjectPrx baseCamera;
	this->refreshRate=0;
	this->mImageFormat.empty();



	int fps=prop->getPropertyAsIntWithDefault(prefix+"Fps",10);
	this->cycle=(float)(1/(float)fps)*1000000;
	try{
		baseCamera = ic->propertyToProxy(prefix+"Proxy");
		if (0==baseCamera){
			throw prefix + "Could not create proxy with Camera";
		}
		else {
			this->prx= jderobot::CameraPrx::checkedCast(baseCamera);
			if (0==this->prx)
				throw "Invalid " + prefix + ".Proxy";
		}
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
		jderobot::Logger::getInstance()->error(prefix + " Not camera provided");
	}

	// Discover what format are supported.
	jderobot::ImageFormat formats = this->prx->getImageFormat();

	std::vector<std::string>::iterator it;
	it = std::find(formats.begin(), formats.end(), colorspaces::ImageRGB8::FORMAT_RGB8.get()->name);
	if (it != formats.end())
	{
		this->mImageFormat = colorspaces::ImageRGB8::FORMAT_RGB8.get()->name;
		it = std::find(formats.begin(), formats.end(), colorspaces::ImageRGB8::FORMAT_RGB8_Z.get()->name);
		if (it != formats.end())
			this->mImageFormat = colorspaces::ImageRGB8::FORMAT_RGB8_Z.get()->name;
	}
	else
	{
		it = std::find(formats.begin(), formats.end(), colorspaces::ImageRGB8::FORMAT_DEPTH8_16.get()->name);
		if (it != formats.end())
		{
			this->mImageFormat = colorspaces::ImageRGB8::FORMAT_DEPTH8_16.get()->name;
			it = std::find(formats.begin(), formats.end(), colorspaces::ImageRGB8::FORMAT_DEPTH8_16_Z.get()->name);
			if (it != formats.end())
				this->mImageFormat = colorspaces::ImageRGB8::FORMAT_DEPTH8_16_Z.get()->name;
		}
	}

	jderobot::Logger::getInstance()->info("Negotiated format " + this->mImageFormat + " for camera " + this->prx->getCameraDescription()->name);

	jderobot::ImageDataPtr data = this->prx->getImageData(this->mImageFormat);

	this->size=cv::Size(data->description->width,data->description->height);
	_done=false;
	this->pauseStatus=false;
}

cameraClient::cameraClient(Ice::CommunicatorPtr ic, std::string prefix, std::string proxy){

	this->prefix=prefix;
	Ice::PropertiesPtr prop;
	prop = ic->getProperties();
	Ice::ObjectPrx baseCamera;
	this->refreshRate=0;
	this->mImageFormat = colorspaces::ImageRGB8::FORMAT_RGB8.get()->name;

	int fps=prop->getPropertyAsIntWithDefault(prefix+"Fps",10);
	this->cycle=(float)(1/(float)fps)*1000000;

	try{
			baseCamera = ic->stringToProxy(proxy);
			if (0==baseCamera){
				throw prefix + "Could not create proxy with Camera";
			}
			else {
				this->prx= jderobot::CameraPrx::checkedCast(baseCamera);
				if (0==this->prx)
					throw "Invalid " + prefix + ".Proxy";
			}

	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
		jderobot::Logger::getInstance()->error(prefix + " Not camera provided");
	}

	jderobot::ImageDataPtr data = this->prx->getImageData(this->mImageFormat);

	this->size=cv::Size(data->description->width,data->description->height);
	_done=false;
	this->pauseStatus=false;
}

cameraClient::~cameraClient() {
	_done=true;
}


jderobot::ImageFormat cameraClient::getImageFormat()
{
	return this->prx->getImageFormat();
}

void cameraClient::setImageFormat (std::string format)
{
	mImageFormat = format;

	jderobot::Logger::getInstance()->info("Changed format " + this->mImageFormat + " for camera " + this->prx->getCameraDescription()->name);
};


void cameraClient::reset(){
	this->prx->reset();
}

void cameraClient::pause(){
	this->pauseStatus=true;
}

void cameraClient::resume(){
	this->controlMutex.lock();
	this->pauseStatus=false;
	this->sem.broadcast();
	this->controlMutex.unlock();
}


void
cameraClient::run(){
	jderobot::ImageDataPtr dataPtr;
	colorspaces::Image::FormatPtr fmt;
	IceUtil::Time last;

	int iterIndex = 0;
	int totalRefreshRate = 0;

	last=IceUtil::Time::now();
	while (!(_done)){
		iterIndex ++;
		if (pauseStatus){
			IceUtil::Mutex::Lock sync(this->controlMutex);
			this->sem.wait(sync);
		}

		try{

			dataPtr = this->prx->getImageData(this->mImageFormat);
			fmt = colorspaces::Image::Format::searchFormat(dataPtr->description->format);
			if (!fmt)
				throw "Format not supported";

			if (dataPtr->description->format == colorspaces::ImageRGB8::FORMAT_RGB8_Z.get()->name ||
				dataPtr->description->format == colorspaces::ImageRGB8::FORMAT_DEPTH8_16_Z.get()->name	)
			{

				size_t dest_len = dataPtr->description->width*dataPtr->description->height*3;
				size_t source_len = dataPtr->pixelData.size();

				unsigned char* origin_buf = (uchar*) malloc(dest_len);

				int r = uncompress((Bytef *) origin_buf, (uLongf *) &dest_len, (const Bytef *) &(dataPtr->pixelData[0]), (uLong)source_len);

				if(r != Z_OK) {
					fprintf(stderr, "[CMPR] Error:\n");
					switch(r) {
					case Z_MEM_ERROR:
						fprintf(stderr, "[CMPR] Error: Not enough memory to compress.\n");
						break;
					case Z_BUF_ERROR:
						fprintf(stderr, "[CMPR] Error: Target buffer too small.\n");
						break;
					case Z_STREAM_ERROR:    // Invalid compression level
						fprintf(stderr, "[CMPR] Error: Invalid compression level.\n");
						break;
					}
				}
				else
				{
					colorspaces::Image imageRGB(dataPtr->description->width,dataPtr->description->height,colorspaces::ImageRGB8::FORMAT_RGB8,&(origin_buf[0]));
					colorspaces::ImageRGB8 img_rgb888(imageRGB);//conversion will happen if needed
					this->controlMutex.lock();
					cv::Mat(cvSize(img_rgb888.width,img_rgb888.height), CV_8UC3, img_rgb888.data).copyTo(this->data);
					this->controlMutex.unlock();
					img_rgb888.release();
				}


				if (origin_buf)
					free(origin_buf);

			}
			else
			{
				colorspaces::Image imageRGB(dataPtr->description->width,dataPtr->description->height,fmt,&(dataPtr->pixelData[0]));
				colorspaces::ImageRGB8 img_rgb888(imageRGB);//conversion will happen if needed
				this->controlMutex.lock();
				cv::Mat(cvSize(img_rgb888.width,img_rgb888.height), CV_8UC3, img_rgb888.data).copyTo(this->data);
				this->controlMutex.unlock();
				img_rgb888.release();
			}

		}
		catch(...){
			jderobot::Logger::getInstance()->warning(prefix +"error during request (connection error)");
			usleep(5000);

		}

		int process = this->cycle - (IceUtil::Time::now().toMicroSeconds() - last.toMicroSeconds());

		if (process > (int)cycle ){
			jderobot::Logger::getInstance()->warning("--------" + prefix + " adquisition timeout-");
		}
		else{
			int delay = (int)cycle - process;
			if (delay <1 || delay > (int)cycle)
				delay = 1;

			usleep(delay);
		}


		int rate =(int)(1000000/(IceUtil::Time::now().toMicroSeconds() - last.toMicroSeconds()));
		totalRefreshRate =  totalRefreshRate + rate;
		this->refreshRate= totalRefreshRate / iterIndex;
		last=IceUtil::Time::now();

		if (iterIndex == INT_MAX) 
		{
			iterIndex = 0;
			jderobot::Logger::getInstance()->info( "*** Counter reset");
		}

	}
	this->data.release();
}

void cameraClient::stop_thread()
{
	_done = true;
}

void cameraClient::getImage(cv::Mat& image){


	this->controlMutex.lock();
	this->data.copyTo(image);
	this->controlMutex.unlock();

}

} /* namespace jderobot */
