/*
 *  Copyright (C) 1997-2014 JDE Developers Team
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
 *  Authors : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>
			
 */

/** \file kinect2Server.cpp
 * \brief kinect2Server component main file
 */

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/kinectleds.h>
#include <jderobot/camera.h>
#include <jderobot/pose3dmotors.h>
#include <jderobot/pointcloud.h>
#include <visionlib/colorspaces/colorspacesmm.h>
#include <tr1/memory>
#include <list>
#include <sstream>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <signal.h>
#include <log/Logger.h>
#include <ns/ns.h>
#include <iostream>
#include <signal.h>
#include <zlib.h>
#include <opencv2/opencv.hpp>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/threading.h>





//global configuration
int cameraR, cameraD,cameraI, ImageRegistration;
int SELCAM;
IceUtil::Mutex mutex;
bool componentAlive;
pthread_t updateThread;

int retry_times = 0;

//block to wait the device initialization
IceUtil::Mutex controlMutex;
IceUtil::Cond sem;
int mirrorDepth, mirrorRGB, mirrorIR;


namespace kinect2Server{


std::vector<int> distances;
std::vector<int> pixelsID;
cv::Mat srcRGB, srcIR, srcDEPTH;
int g_width, g_height;




int segmentationType; //0 ninguna, 1 NITE
int mainFPS;


void* updateThread(void*)
{
	//glfwInit();

	libfreenect2::Freenect2 freenect2;
	libfreenect2::Freenect2Device *dev = freenect2.openDefaultDevice();

	if(dev == 0)
	{
		std::cout << "no device connected or failure opening the default one!" << std::endl;
		return (NULL);
	}
	libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
	libfreenect2::FrameMap frames;

	dev->setColorFrameListener(&listener);
	dev->setIrAndDepthFrameListener(&listener);
	dev->start();

	std::cout << "Device serial: " << dev->getSerialNumber() << std::endl;
	std::cout << "Device firmware: " << dev->getFirmwareVersion() << std::endl;

	float cycle=(float)(1/(float)mainFPS)*1000000;
	IceUtil::Time lastIT=IceUtil::Time::now();
	bool first=true;
	while (componentAlive){
		listener.waitForNewFrame(frames);
		libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
		libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
		libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];

		if (first){
			g_height=ir->height;
			g_width=ir->width;
			sem.broadcast();
			first=false;
		}
		mutex.lock();
			cv::Mat(rgb->height, rgb->width, CV_8UC3, rgb->data).copyTo(srcRGB);
			cv::Mat(ir->height, ir->width, CV_32FC1, ir->data).copyTo(srcIR);
			srcIR=srcIR/20000.0f;
			cv::Mat(depth->height, depth->width, CV_32FC1, depth->data).copyTo(srcDEPTH);
		mutex.unlock();
		listener.release(frames);
		int process = IceUtil::Time::now().toMicroSeconds() - lastIT.toMicroSeconds();

		if (process > (int)cycle ){
			jderobot::Logger::getInstance()->warning("-------- kinect2ServerServer: Main thread timeout-" );
		}
		else{
			int delay = (int)cycle - process;
			if (delay <1 || delay > (int)cycle)
				delay = 1;

			//usleep(delay);
		}

		lastIT=IceUtil::Time::now();

	}
	dev->stop();
	dev->close();
   return (NULL);
}



/**
* \brief Class which contains all the functions and variables to make run the Robot Cameras
*/
	class CameraRGB: virtual public jderobot::Camera {
public:
	CameraRGB(std::string& propertyPrefix, const Ice::PropertiesPtr propIn)
      : prefix(propertyPrefix),
	imageFmt(),
	imageDescription(new jderobot::ImageDescription()),
	cameraDescription(new jderobot::CameraDescription()),
	replyTask()
	{
	Ice::PropertiesPtr prop = propIn;


	//fill cameraDescription
	cameraDescription->name = prop->getProperty(prefix+"Name");
	if (cameraDescription->name.size() == 0)
		jderobot::Logger::getInstance()->warning( "Camera name not configured" );

	cameraDescription->shortDescription = prop->getProperty(prefix + "ShortDescription");

	//fill imageDescription
	imageDescription->width = prop->getPropertyAsIntWithDefault(prefix+"width",5);;
	imageDescription->height = prop->getPropertyAsIntWithDefault(prefix+"height",5);;


	int fps = prop->getPropertyAsIntWithDefault(prefix+"fps",5);
	//we use formats according to colorspaces
	std::string fmtStr = prop->getPropertyWithDefault(prefix+"Format","ImageRGB8");//default format YUY2
	imageFmt = colorspaces::Image::Format::searchFormat(fmtStr);
	if (!imageFmt)
		jderobot::Logger::getInstance()->warning( "Format " + fmtStr + " unknown" );
	imageDescription->size = imageDescription->width * imageDescription->height * CV_ELEM_SIZE(imageFmt->cvType);
	imageDescription->format = imageFmt->name;

	// Set the formats allowed
	mFormats.push_back(colorspaces::ImageRGB8::FORMAT_RGB8.get()->name);



	jderobot::Logger::getInstance()->info( "Starting thread for camera: " + cameraDescription->name );
	replyTask = new ReplyTask(this,fps, mFormats[0]);

	this->control=replyTask->start();//my own thread
	}

	virtual ~CameraRGB(){
		jderobot::Logger::getInstance()->info( "Stopping and joining thread for camera: " + cameraDescription->name );
		replyTask->destroy();
		this->control.join();
	}
    
	virtual jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c){
		return (imageDescription);
	}

	virtual jderobot::CameraDescriptionPtr getCameraDescription(const Ice::Current& c){
		return (cameraDescription);
	}

	virtual jderobot::ImageFormat getImageFormat(const Ice::Current& c)
	{
		return (mFormats);
	}


	virtual void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, const std::string& format, const Ice::Current& c){
		replyTask->pushJob(cb,format);
	}

	virtual std::string startCameraStreaming(const Ice::Current&){
		jderobot::Logger::getInstance()->info( "Should be made anything to start camera streaming: " + cameraDescription->name);
		return (std::string(""));
	}

	virtual void stopCameraStreaming(const Ice::Current&) {
		jderobot::Logger::getInstance()->info( "Should be made anything to stop camera streaming: " +  cameraDescription->name );
	}
	virtual void reset(const Ice::Current&)
	{
	}

	virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr&, const Ice::Current&){
		return (0);
	}

private:
	class ReplyTask: public IceUtil::Thread{
	public:
		ReplyTask(CameraRGB* camera, int fps, std::string format):mycameravga(camera),_done(false),  mFormat(format) {
		this->fps=fps;
      }
		

	void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, std::string format){
	  mFormat = format;
		IceUtil::Mutex::Lock sync(requestsMutex);
		requests.push_back(cb);
	}

    virtual void run(){

		jderobot::ImageDataPtr reply(new jderobot::ImageData);
		reply->description = mycameravga->imageDescription;
		reply->pixelData.resize(mycameravga->imageDescription->width*mycameravga->imageDescription->height*3);
		cv::Mat dst_cvt;
		cv::Mat local_src;



		float cycle; // duración del ciclo

		cycle=(float)(1/(float)fps)*1000000;
		
		IceUtil::Time lastIT=IceUtil::Time::now();
		while(!(_done)){
			mutex.lock();
				srcRGB.copyTo(local_src);
			mutex.unlock();



			if (mirrorRGB)
				cv::flip(local_src,local_src,1);

			if ((mycameravga->imageDescription->width != local_src.cols) || (mycameravga->imageDescription->height != local_src.rows)){
				resize(local_src, dst_cvt, cv::Size(mycameravga->imageDescription->width,mycameravga->imageDescription->height), 0, 0, cv::INTER_LINEAR);
				dst_cvt.convertTo(dst_cvt,CV_8UC3);
				dst_cvt.copyTo(local_src);
			}

			if (mFormat == colorspaces::ImageRGB8::FORMAT_RGB8_Z.get()->name)
			{
			  size_t source_len = local_src.rows*local_src.cols * 3;
			  size_t compress_len = compressBound(source_len);
			  unsigned char* compress_buf = (unsigned char *) malloc(compress_len);

			  int r = compress2((Bytef *) compress_buf, (uLongf *) &compress_len, (Bytef *) &(local_src.data[0]), (uLong)source_len , 9);
			  if(r != Z_OK) {
			    jderobot::Logger::getInstance()->error("Compression Error");
			    switch(r) {
			      case Z_MEM_ERROR:
			        jderobot::Logger::getInstance()->error("Compression Error: Not enough memory to compress");
			        break;
			      case Z_BUF_ERROR:
			        jderobot::Logger::getInstance()->error("Compression Error: Target buffer too small");
			        break;
			      case Z_STREAM_ERROR:    // Invalid compression level
			        jderobot::Logger::getInstance()->error("Compression Error: Invalid compression level");
			        break;
			    }
			  }

			  reply->description->format=colorspaces::ImageRGB8::FORMAT_RGB8_Z.get()->name;
			  reply->pixelData.resize(compress_len);
			  memcpy(&(reply->pixelData[0]), (unsigned char *) compress_buf, compress_len);

			  if (compress_buf)
			    free(compress_buf);
			}
			else if (mFormat == colorspaces::ImageRGB8::FORMAT_RGB8.get()->name)
			{
			  memcpy(&(reply->pixelData[0]),(unsigned char *) local_src.data,local_src.cols*local_src.rows * 3);
			}
			else
			{
			  // TODO: Raise exception
			}


		    {//critical region start
			IceUtil::Mutex::Lock sync(requestsMutex);
		    while(!requests.empty()){
				jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front();
				requests.pop_front();
				cb->ice_response(reply);
			}

			}//critical region end


			int process = IceUtil::Time::now().toMicroSeconds() - lastIT.toMicroSeconds();

			if (process > (int)cycle ){
				jderobot::Logger::getInstance()->warning("-------- kinect2Server: RGB data timeout-" );
			}
			else{
				int delay = (int)cycle - process;
				if (delay <1 || delay > (int)cycle)
					delay = 1;

				usleep(delay);
			}

			lastIT=IceUtil::Time::now();
		}
	}
    virtual void destroy(){
		this->_done=true;
	}


	private:
		CameraRGB* mycameravga;
		IceUtil::Mutex requestsMutex;
		std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
		int fps;
		bool _done;
    std::string mFormat;

    };
    typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;


    std::string prefix;
    colorspaces::Image::FormatPtr imageFmt;
    jderobot::ImageDescriptionPtr imageDescription;
    jderobot::CameraDescriptionPtr cameraDescription;
    ReplyTaskPtr replyTask;
    IceUtil::ThreadControl control;
	jderobot::ImageFormat mFormats;


  };







//*********************************************************************/
	class CameraDEPTH: virtual public jderobot::Camera {
public:
	CameraDEPTH(std::string& propertyPrefix, const Ice::PropertiesPtr propIn)
      : prefix(propertyPrefix),
	imageFmt(),
	imageDescription(new jderobot::ImageDescription()),
	cameraDescription(new jderobot::CameraDescription()),
	replyTask()
	{
      
      
	Ice::PropertiesPtr prop = propIn;

	//fill cameraDescription
	cameraDescription->name = prop->getProperty(prefix+"Name");
	if (cameraDescription->name.size() == 0)
		jderobot::Logger::getInstance()->warning( "Camera name not configured" );

	cameraDescription->shortDescription = prop->getProperty(prefix+"ShortDescription");

	//fill imageDescription
	imageDescription->width = g_width;
	imageDescription->height = g_height;


	int fps = prop->getPropertyAsIntWithDefault(prefix+"fps",5);
	//we use formats acording to colorspaces
	std::string fmtStr = prop->getPropertyWithDefault(prefix+"Format","YUY2");//default format YUY2
	imageFmt = colorspaces::Image::Format::searchFormat(fmtStr);
	if (!imageFmt)
		jderobot::Logger::getInstance()->info( "Format " +  fmtStr + " unknown" );
	imageDescription->size = imageDescription->width * imageDescription->height * CV_ELEM_SIZE(imageFmt->cvType);
	imageDescription->format = imageFmt->name;

	// Image formats allowed
	mFormats.push_back(colorspaces::ImageRGB8::FORMAT_DEPTH8_16.get()->name);
    mFormats.push_back(colorspaces::ImageRGB8::FORMAT_DEPTH8_16_Z.get()->name);


	jderobot::Logger::getInstance()->info( "Starting thread for camera: " +  cameraDescription->name );
	replyTask = new ReplyTask(this, imageDescription->width, imageDescription->height,fps, mFormats[0]);


	this->control=replyTask->start();//my own thread
	}

	virtual ~CameraDEPTH(){
		jderobot::Logger::getInstance()->info( "Stopping and joining thread for camera: " + cameraDescription->name );

		replyTask->destroy();
		this->control.join();
	}
    
	virtual jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c){
	  return (imageDescription);
	}

	virtual jderobot::CameraDescriptionPtr getCameraDescription(const Ice::Current& c){
		return (cameraDescription);
	}

	virtual void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, const std::string& format, const Ice::Current& c){
		replyTask->pushJob(cb,format);
	}

	virtual std::string startCameraStreaming(const Ice::Current&){
		jderobot::Logger::getInstance()->info( "Should be made anything to start camera streaming: " + cameraDescription->name );
		return (std::string(""));
	}

	virtual void stopCameraStreaming(const Ice::Current&) {
		jderobot::Logger::getInstance()->info( "Should be made anything to stop camera streaming: "  + cameraDescription->name );
	}
	virtual void reset(const Ice::Current&)
	{
	}
	
	virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr&, const Ice::Current&){
		return (0);
	}

	virtual jderobot::ImageFormat getImageFormat(const Ice::Current& c)
	{
		return (mFormats);

	}

private:
	class ReplyTask: public IceUtil::Thread{
	public:
		ReplyTask(CameraDEPTH* camera, int width, int height, int fps, std::string format)
	:mycameradepth(camera),_done(false), mFormat(format) {
		this->fps=fps;
      }
		

	void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, std::string format){
		IceUtil::Mutex::Lock sync(requestsMutex);
		requests.push_back(cb);
	}

    virtual void run(){
		int test;
		

		jderobot::ImageDataPtr reply(new jderobot::ImageData);
		reply->description = mycameradepth->imageDescription;
		reply->pixelData.resize(mycameradepth->imageDescription->width*mycameradepth->imageDescription->height*3);
		cv::Mat local_src;
		cv::Mat dst_c1(g_height,g_width,CV_8UC1);
		cv::Mat dst_c2(g_height,g_width,CV_8UC1);
		cv::Mat dst_c3(g_height,g_width,CV_8UC1);
		cv::Mat dst_cvt(g_height,g_width,CV_8UC3);


		int cycle; // duración del ciclo
		IceUtil::Time lastIT;


		cycle=(float)(1/(float)fps)*1000000;

		lastIT=IceUtil::Time::now();
		while(!(_done)){
			mutex.lock();
				srcDEPTH.copyTo(local_src);
			mutex.unlock();

			cv::Mat temp32gray(local_src.rows, local_src.cols, CV_32FC1);
			temp32gray=(local_src/4500.0f)*255;

			for (int y = 0; y < local_src.rows; ++y)
			{
			  for (int x = 0; x < local_src.cols; ++x){
				dst_c1.at<unsigned char>(y,x)= int(temp32gray.at<float>(y,x));
			    dst_c2.at<unsigned char>(y,x)= int(local_src.at<float>(y,x)) >> 8;
			    dst_c3.at<unsigned char>(y,x)= int(local_src.at<float>(y,x))&0xff;
			  }
			}

			std::vector<cv::Mat > channels;
			channels.push_back(dst_c1);
			channels.push_back(dst_c2);
			channels.push_back(dst_c3);
			cv::merge(channels,dst_cvt);

			if (mirrorDepth)
			  cv::flip(dst_cvt,dst_cvt,1);




			if (mFormat == colorspaces::ImageRGB8::FORMAT_DEPTH8_16_Z.get()->name)
			{
	      cv::imshow("test", dst_cvt);
	      cv::waitKey(1);
			  size_t source_len = dst_cvt.rows*dst_cvt.cols * 3;
			  size_t compress_len = compressBound(source_len);
			  unsigned char* compress_buf = (unsigned char *) malloc(compress_len);

			  int r = compress2((Bytef *) compress_buf, (uLongf *) &compress_len, (Bytef *) &(dst_cvt.data[0]), (uLong)source_len , 9);
			  if(r != Z_OK) {
			    jderobot::Logger::getInstance()->error("Compression Error");
			    switch(r) {
			      case Z_MEM_ERROR:
			        jderobot::Logger::getInstance()->error("Compression Error: Not enough memory to compress");
			        break;
			      case Z_BUF_ERROR:
			        jderobot::Logger::getInstance()->error("Compression Error: Target buffer too small");
			        break;
			      case Z_STREAM_ERROR:    // Invalid compression level
			        jderobot::Logger::getInstance()->error("Compression Error: Invalid compression level");
			        break;
			    }
			  }

			  reply->description->format=colorspaces::ImageRGB8::FORMAT_DEPTH8_16_Z.get()->name;
			  reply->pixelData.resize(compress_len);
			  memcpy(&(reply->pixelData[0]), (unsigned char *) compress_buf, compress_len);

			  if (compress_buf)
			    free(compress_buf);
			}
			else if (mFormat == colorspaces::ImageRGB8::FORMAT_DEPTH8_16.get()->name)
			{
			  memcpy(&(reply->pixelData[0]),(unsigned char *) dst_cvt.data,dst_cvt.cols*dst_cvt.rows * 3);
			}
			else
			{
			  // TODO: Raise exception
			}

		    {//critical region start
			IceUtil::Mutex::Lock sync(requestsMutex);
		    while(!requests.empty()){
				jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front();
				requests.pop_front();
				cb->ice_response(reply);
			}
			}//critical region end
			

			int process = IceUtil::Time::now().toMicroSeconds() - lastIT.toMicroSeconds();

			if (process > (int)cycle ){
				jderobot::Logger::getInstance()->warning("-------- kinect2Server: Depth data timeout-" );
			}
			else{
				int delay = (int)cycle - process;
				if (delay <1 || delay > (int)cycle)
					delay = 1;

				usleep(delay);
			}
			
			lastIT=IceUtil::Time::now();
		}
	}
	
    virtual void destroy(){
		this->_done=true;
	}


	private:
		CameraDEPTH* mycameradepth;
		IceUtil::Mutex requestsMutex;
		std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
    std::string mFormat;

		int fps;
		bool _done;
	
	
    };
    typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;


    std::string prefix;
    colorspaces::Image::FormatPtr imageFmt;
    jderobot::ImageDescriptionPtr imageDescription;
    jderobot::CameraDescriptionPtr cameraDescription;
    ReplyTaskPtr replyTask;
    IceUtil::ThreadControl control;
	jderobot::ImageFormat mFormats;

  };


/**
* \brief Class which contains all the functions and variables to make run the Robot Cameras
*/
	class CameraIR: virtual public jderobot::Camera {
public:
	CameraIR(std::string& propertyPrefix, const Ice::PropertiesPtr propIn)
	  : prefix(propertyPrefix),
	imageFmt(),
	imageDescription(new jderobot::ImageDescription()),
	cameraDescription(new jderobot::CameraDescription()),
	replyTask()
	{
	Ice::PropertiesPtr prop = propIn;

	//fill cameraDescription
	cameraDescription->name = prop->getProperty(prefix+"Name");
	if (cameraDescription->name.size() == 0)
		jderobot::Logger::getInstance()->warning( "Camera name not configured" );

	cameraDescription->shortDescription = prop->getProperty(prefix + "ShortDescription");
	imageDescription->width = g_width;
	imageDescription->height = g_height;


	int fps = prop->getPropertyAsIntWithDefault(prefix+"fps",5);
	//we use formats according to colorspaces
	std::string fmtStr = prop->getPropertyWithDefault(prefix+"Format","ImageGRAY8");//default format YUY2
	imageFmt = colorspaces::Image::Format::searchFormat(fmtStr);
	if (!imageFmt)
		jderobot::Logger::getInstance()->warning( "Format " + fmtStr + " unknown" );
	imageDescription->size = imageDescription->width * imageDescription->height * CV_ELEM_SIZE(imageFmt->cvType);
	imageDescription->format = imageFmt->name;

	// Image formats allowed
	mFormats.push_back(colorspaces::ImageGRAY8::FORMAT_GRAY8.get()->name);
  mFormats.push_back(colorspaces::ImageGRAY8::FORMAT_GRAY8_Z.get()->name);


	jderobot::Logger::getInstance()->info( "Starting thread for camera: " + cameraDescription->name );
	replyTask = new ReplyTask(this,fps, mFormats[0]);

	this->control=replyTask->start();//my own thread
	}

	virtual ~CameraIR(){
		jderobot::Logger::getInstance()->info( "Stopping and joining thread for camera: " + cameraDescription->name );
		replyTask->destroy();
		this->control.join();
	}

	virtual jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c){
		return (imageDescription);
	}

	virtual jderobot::CameraDescriptionPtr getCameraDescription(const Ice::Current& c){
		return (cameraDescription);
	}

	virtual void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, const std::string& format, const Ice::Current& c){
		replyTask->pushJob(cb,format);
	}

	virtual std::string startCameraStreaming(const Ice::Current&){
		jderobot::Logger::getInstance()->info( "Should be made anything to start camera streaming: " + cameraDescription->name);
		return (std::string(""));
	}

	virtual void stopCameraStreaming(const Ice::Current&) {
		jderobot::Logger::getInstance()->info( "Should be made anything to stop camera streaming: " +  cameraDescription->name );
	}
	virtual void reset(const Ice::Current&)
	{
	}

	virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr&, const Ice::Current&){
		return (0);
	}

	virtual jderobot::ImageFormat getImageFormat(const Ice::Current& c)
	{
		return (mFormats);

	}

private:
	class ReplyTask: public IceUtil::Thread{
	public:
		ReplyTask(CameraIR* camera, int fps,  std::string format):mycameravga(camera),_done(false), mFormat(format) {
		this->fps=fps;
	  }


	void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, std::string format){
	  mFormat = format;
		IceUtil::Mutex::Lock sync(requestsMutex);
		requests.push_back(cb);
	}

	virtual void run(){

		jderobot::ImageDataPtr reply(new jderobot::ImageData);
		reply->description = mycameravga->imageDescription;
		reply->pixelData.resize(mycameravga->imageDescription->width*mycameravga->imageDescription->height);
		cv::Mat dst_cvt(g_height,g_width,CV_8UC1);
		cv::Mat local_src;



		float cycle; // duración del ciclo

		cycle=(float)(1/(float)fps)*1000000;

		IceUtil::Time lastIT=IceUtil::Time::now();
		while(!(_done)){
			mutex.lock();
				srcIR.copyTo(local_src);
			mutex.unlock();

			local_src=local_src*255;
			local_src.convertTo(dst_cvt,CV_8UC1);
			if (mirrorIR){
				cv::flip(dst_cvt,dst_cvt,1);
			}

			if (mFormat == colorspaces::ImageGRAY8::FORMAT_GRAY8_Z.get()->name)
      {
        size_t source_len = dst_cvt.rows*dst_cvt.cols;
        size_t compress_len = compressBound(source_len);
        unsigned char* compress_buf = (unsigned char *) malloc(compress_len);

        int r = compress2((Bytef *) compress_buf, (uLongf *) &compress_len, (Bytef *) &(dst_cvt.data[0]), (uLong)source_len , 9);
        if(r != Z_OK) {
          jderobot::Logger::getInstance()->error("Compression Error");
          switch(r) {
          case Z_MEM_ERROR:
            jderobot::Logger::getInstance()->error("Compression Error: Not enough memory to compress");
            break;
          case Z_BUF_ERROR:
            jderobot::Logger::getInstance()->error("Compression Error: Target buffer too small");
            break;
          case Z_STREAM_ERROR:    // Invalid compression level
            jderobot::Logger::getInstance()->error("Compression Error: Invalid compression level");
            break;
          }
        }

        reply->description->format=colorspaces::ImageGRAY8::FORMAT_GRAY8_Z.get()->name;
        reply->pixelData.resize(compress_len);
        memcpy(&(reply->pixelData[0]), (unsigned char *) compress_buf, compress_len);

        if (compress_buf)
          free(compress_buf);
      }
      else if (mFormat == colorspaces::ImageGRAY8::FORMAT_GRAY8.get()->name)
      {
        memcpy(&(reply->pixelData[0]),(unsigned char *) dst_cvt.data,dst_cvt.cols*dst_cvt.rows );
      }
      else
      {
        // TODO: Raise exception
      }

			{//critical region start
			IceUtil::Mutex::Lock sync(requestsMutex);
			while(!requests.empty()){
				jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front();
				requests.pop_front();
				cb->ice_response(reply);
			}

			}//critical region end



			int process = IceUtil::Time::now().toMicroSeconds() - lastIT.toMicroSeconds();

			if (process > (int)cycle ){
				jderobot::Logger::getInstance()->warning("-------- kinect2Server: RGB data timeout-" );
			}
			else{
				int delay = (int)cycle - process;
				if (delay <1 || delay > (int)cycle)
					delay = 1;

				usleep(delay);
			}

			lastIT=IceUtil::Time::now();
		}
	}
	virtual void destroy(){
		this->_done=true;
	}


	private:
		CameraIR* mycameravga;
		IceUtil::Mutex requestsMutex;
		std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
		int fps;
		bool _done;
		std::string mFormat;

	};
	typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;


	std::string prefix;
	colorspaces::Image::FormatPtr imageFmt;
	jderobot::ImageDescriptionPtr imageDescription;
	jderobot::CameraDescriptionPtr cameraDescription;
	ReplyTaskPtr replyTask;
	IceUtil::ThreadControl control;
	jderobot::ImageFormat mFormats;


  };




/**
* \brief Class wich contains all the functions and variables to serve point cloud interface
*/

	class pointCloudI: virtual public jderobot::pointCloud{
		public:
			pointCloudI (std::string& propertyPrefix, const Ice::PropertiesPtr propIn):
				prefix(propertyPrefix),data(new jderobot::pointCloudData()) {
					Ice::PropertiesPtr prop = propIn;

					int fps =prop->getPropertyAsIntWithDefault("kinect2Server.pointCloud.Fps",10);
					bool extra =(bool)prop->getPropertyAsIntWithDefault("kinect2Server.ExtraCalibration",0);

					pthread_mutex_init(&this->localMutex, NULL);
					//replyCloud = new ReplyCloud(this,prop->getProperty("kinect2Server.calibration"), playerdetection, depthVideoMode.getResolutionX(), depthVideoMode.getResolutionY(),fps, extra);
				   replyCloud = new ReplyCloud(this,prop->getProperty("kinect2Server.calibration"),320, 240,fps, extra);
				   this->control=replyCloud->start();

				}

			virtual ~pointCloudI(){
				jderobot::Logger::getInstance()->info( "Stopping and joining thread for pointCloud" );
				replyCloud->destroy();
				this->control.join();
			}
		

		virtual jderobot::pointCloudDataPtr getCloudData(const Ice::Current&){
				data=replyCloud->getCloud();
				return (data);
			};
		   
		   private:
			 class ReplyCloud :public IceUtil::Thread{
		       public: 
		       	ReplyCloud (pointCloudI* pcloud, std::string filepath, int widthIn, int heightIn, int fpsIn, bool extra) : data(new jderobot::pointCloudData()), data2(new jderobot::pointCloudData()), _done(false)
		        	{
					path=filepath;
					cWidth = widthIn;
					cHeight = heightIn;
					fps=fpsIn;
					myCloud=pcloud;
					withExtraCalibration=extra;
				}
		       
		        void run()
		        {
				



				int cycle; // duración del ciclo


				cycle=(float)(1/(float)fps)*1000000;
				IceUtil::Time lastIT=IceUtil::Time::now();
				while(!(_done)){
					float distance;
					mutex.lock();
					mutex.unlock();


					int delay = IceUtil::Time::now().toMicroSeconds() - lastIT.toMicroSeconds();
					if (delay > cycle ){
						jderobot::Logger::getInstance()->info("-------- kinect2Server: POINTCLOUD data timeout-" );
					}
					else{
						if (delay <1 || delay > cycle)
							delay = 1;
						usleep(delay);
					}

					
					lastIT=IceUtil::Time::now();
		        }
		    }
		        

		       jderobot::pointCloudDataPtr getCloud()
		       {
		        pthread_mutex_lock(&(this->myCloud->localMutex));
				data->p=data2->p;
				pthread_mutex_unlock(&(this->myCloud->localMutex));
		          return (data);
		       }

		    virtual void destroy(){
				this->_done=true;
			}



		   private:
				int cWidth;
				int cHeight;
				int fps;
				jderobot::pointCloudDataPtr data, data2;
				jderobot::RGBPoint auxP;
				std::string path;
				pointCloudI* myCloud;
				bool _done;
				bool withExtraCalibration;
		      
		    };

			typedef IceUtil::Handle<ReplyCloud> ReplyCloudPtr;
			ReplyCloudPtr replyCloud;
			std::string prefix;
			jderobot::pointCloudDataPtr data;
			pthread_mutex_t localMutex;
			IceUtil::ThreadControl control;

			
			
		};
} //namespace


Ice::CommunicatorPtr ic;
bool killed;
kinect2Server::CameraRGB *camRGB;
kinect2Server::CameraDEPTH *camDEPTH;
kinect2Server::CameraIR *camIR;
kinect2Server::pointCloudI *pc1;
jderobot::ns* namingService = NULL;

void exitApplication(int s){


	killed=true;
	componentAlive=false;

	if (camRGB!= NULL)
		delete camRGB;
	if (camDEPTH != NULL)
		delete camDEPTH;
	if (pc1 != NULL){
		delete pc1;
	}

	// NamingService
	if (namingService != NULL)
	{
		namingService->unbindAll();

		delete(namingService);
	}

	ic->shutdown();



	pthread_join(updateThread, NULL);



	exit(0);

}


int main(int argc, char** argv){

	componentAlive=true;
	killed=false;
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = exitApplication;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);


	Ice::PropertiesPtr prop;


	try{
			ic = Ice::initialize(argc,argv);
			prop = ic->getProperties();
	}
	catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
			return (1);
	}
	catch (const char* msg) {
			std::cerr <<"Error :" << msg << std::endl;
			return (1);
	}
	std::string componentPrefix("kinect2Server");


	// Analyze LOG section

	std::string logFile = prop->getProperty(componentPrefix + ".Log.File.Name");
	if (logFile.size()==0)
		jderobot::Logger::getInstance()->warning("You didn't set log file!");
	else
		jderobot::Logger::getInstance()->setFileLog(logFile);

	std::string logLevel = prop->getProperty(componentPrefix + ".Log.File.Level");
	if (logLevel.size()==0)
		jderobot::Logger::getInstance()->warning("You didn't set *.Log.File.Level key!");
	else
		jderobot::Logger::getInstance()->setFileLevel(jderobot::Levels(boost::lexical_cast<int>(logLevel)));

	std::string screenLevel = prop->getProperty(componentPrefix + ".Log.Screen.Level");
	if (screenLevel.size()==0)
		jderobot::Logger::getInstance()->warning("You didn't set *.Log.Screen.Level key!");
	else
		jderobot::Logger::getInstance()->setScreenLevel(jderobot::Levels(boost::lexical_cast<int>(screenLevel)));

	jderobot::Logger::getInstance()->info("Logger:: screenLevel=" + screenLevel + " logLevel=" + logLevel + " LogFile=" + logFile);




	cameraR = prop->getPropertyAsIntWithDefault(componentPrefix + ".CameraRGB",0);
	cameraD = prop->getPropertyAsIntWithDefault(componentPrefix + ".CameraDEPTH",0);
	cameraI = prop->getPropertyAsIntWithDefault(componentPrefix + ".CameraIR",0);

	ImageRegistration = prop->getPropertyAsIntWithDefault(componentPrefix + ".ImageRegistration",1);
	int pointCloud = prop->getPropertyAsIntWithDefault(componentPrefix + ".pointCloudActive",0);
	mirrorDepth = prop->getPropertyAsIntWithDefault(componentPrefix + ".CameraDEPTH.Mirror",0);
	mirrorRGB = prop->getPropertyAsIntWithDefault(componentPrefix + ".CameraRGB.Mirror",0);
	mirrorIR = prop->getPropertyAsIntWithDefault(componentPrefix + ".CameraIR.Mirror",0);
	kinect2Server::mainFPS=prop->getPropertyAsIntWithDefault(componentPrefix + ".Hz", 20);
	std::string Endpoints = prop->getProperty(componentPrefix + ".Endpoints");
	Ice::ObjectAdapterPtr adapter =ic->createObjectAdapterWithEndpoints(componentPrefix, Endpoints);

	SELCAM = prop->getPropertyAsIntWithDefault(componentPrefix + ".deviceId",0);
	jderobot::Logger::getInstance()->info( "kinect2Server: Selected device: " + SELCAM );
	int nCameras=0;


	nCameras=cameraR + cameraD + cameraI;
	//g_context =  new xn::Context;
	if ((nCameras>0)||(pointCloud)){
		pthread_create(&updateThread, NULL, &kinect2Server::updateThread, NULL);
	}


	//bloqueo hasta que se inicialice el dispositivo
	IceUtil::Mutex::Lock sync(controlMutex);
	sem.wait(sync);

	sync.release();

	// Naming Service
	int nsActive = prop->getPropertyAsIntWithDefault("NamingService.Enabled", 0);

	if (nsActive)
	{
		std::string ns_proxy = prop->getProperty("NamingService.Proxy");
		try
		{
			namingService = new jderobot::ns(ic, ns_proxy);
		}
		catch (Ice::ConnectionRefusedException& ex)
		{
			jderobot::Logger::getInstance()->error("Impossible to connect with NameService!");
			exit(-1);
		}
	}

	if (cameraR){
		std::string objPrefix(componentPrefix + ".CameraRGB.");
		std::string cameraName = prop->getProperty(objPrefix + "Name");
		if (cameraName.size() == 0){//no name specified, we create one using the index
			cameraName = "cameraR";
			prop->setProperty(objPrefix + "Name",cameraName);//set the value
			}
		jderobot::Logger::getInstance()->info("Creating camera " + cameraName );
		camRGB = new kinect2Server::CameraRGB(objPrefix,prop);
		adapter->add(camRGB, ic->stringToIdentity(cameraName));
		jderobot::Logger::getInstance()->info("              -------- kinect2Server: Component: CameraRGB created successfully(" + Endpoints + "@" + cameraName );


		if (namingService)
			namingService->bind(cameraName, Endpoints, camRGB->ice_staticId());



	}

	if (cameraD){
		std::string objPrefix(componentPrefix + ".CameraDEPTH.");
		std::string cameraName = prop->getProperty(objPrefix + "Name");
		if (cameraName.size() == 0){//no name specified, we create one using the index
			cameraName = "cameraD";
			prop->setProperty(objPrefix + "Name",cameraName);//set the value
			}
		jderobot::Logger::getInstance()->info( "Creating camera " +  cameraName );
		camDEPTH = new kinect2Server::CameraDEPTH(objPrefix,prop);
		adapter->add(camDEPTH, ic->stringToIdentity(cameraName));
		//test camera ok
		jderobot::Logger::getInstance()->info("              -------- kinect2Server: Component: CameraDEPTH created successfully(" + Endpoints + "@" + cameraName );

		if (namingService)
			namingService->bind(cameraName, Endpoints, camDEPTH->ice_staticId());

	}

	if (cameraI){
		std::string objPrefix(componentPrefix + ".CameraIR.");
		std::string cameraName = prop->getProperty(objPrefix + "Name");
		if (cameraName.size() == 0){//no name specified, we create one using the index
			cameraName = "cameraI";
			prop->setProperty(objPrefix + "Name",cameraName);//set the value
			}
		jderobot::Logger::getInstance()->info( "Creating camera " +  cameraName );
		camIR = new kinect2Server::CameraIR(objPrefix,prop);
		adapter->add(camIR, ic->stringToIdentity(cameraName));
		//test camera ok
		jderobot::Logger::getInstance()->info("              -------- kinect2Server: Component: CameraIR created successfully(" + Endpoints + "@" + cameraName );

		if (namingService)
			namingService->bind(cameraName, Endpoints, camIR->ice_staticId());

	}

	if (pointCloud){
		std::string objPrefix(componentPrefix + ".PointCloud.");
		std::string Name = prop->getProperty(objPrefix + "Name");
		jderobot::Logger::getInstance()->info( "Creating pointcloud1 " + Name );
		pc1 = new kinect2Server::pointCloudI(objPrefix,prop);
		adapter->add(pc1 , ic->stringToIdentity(Name));
		jderobot::Logger::getInstance()->info("              -------- kinect2Server: Component: PointCloud created successfully(" + Endpoints + "@" + Name );
	}
	adapter->activate();
	ic->waitForShutdown();
	adapter->destroy();

	if (!killed)
		exitApplication(0);
	return (0);

}
