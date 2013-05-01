/*
 *  Copyright (C) 1997-2012 JDE Developers Teameldercare.camRGB
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

/** \file openniServer.cpp
 * \brief openniServer component main file
 */

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <gbxsickacfr/gbxiceutilacfr/safethread.h>
#include <jderobot/kinectleds.h>
#include <jderobot/camera.h>
#include <jderobot/pose3dmotors.h>
#include <jderobot/pointcloud.h>
#include <colorspaces/colorspacesmm.h>
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <tr1/memory>
#include <list>
#include <sstream>
#include <jderobotice/exceptions.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "myprogeo.h"
#include <OpenNI.h>


#ifdef WITH_NITE2
	#include "NiTE.h"
#endif


#define VID_MICROSOFT 0x45e
#define PID_NUI_MOTOR 0x02b0
#define NUM_THREADS 5
#define MAX_LENGHT 10000

#define CHECK_RC(rc, what)                                      \
if (rc != openni::STATUS_OK)                                         \
{                                                               \
        std::cout << what << " failed: " << openni::OpenNI::getExtendedError() << std::endl;     \
                                                    \
}

openni::VideoFrameRef		m_depthFrame;
openni::VideoFrameRef		m_colorFrame;
openni::Device			m_device;


#ifdef WITH_NITE2
	nite::UserTracker* m_pUserTracker;
	nite::UserTrackerFrameRef userTrackerFrame;
	nite::Status rcN;
#endif



namespace openniServer{

pthread_mutex_t mutex;
int SELCAM;
std::vector<int> distances;
std::vector<int> pixelsID;
cv::Mat* srcRGB;
int colors[10][3];
int userGeneratorActive=0;
int m_width;
int m_height;
openni::VideoStream depth, color;
openni::VideoStream** m_streams;

int configWidth;
int configHeight;
int configFps;



void* updateThread(void*)
{
	openni::Status rc = openni::STATUS_OK;
	rc = openni::OpenNI::initialize();
	if (rc != openni::STATUS_OK)
	{
		std::cout << SELCAM << ": Initialize failed: "<<  openni::OpenNI::getExtendedError() <<std::endl;
	}

	openni::Array<openni::DeviceInfo> deviceList;
	openni::OpenNI::enumerateDevices(&deviceList);

	const char* deviceUri;
	//checking the number off connected devices
	if (deviceList.getSize() < 1)
	{
		std::cout << "Missing devices" << std::endl;
		openni::OpenNI::shutdown();
	}

	//getting the Uri of the selected device
	deviceUri = deviceList[SELCAM].getUri();

	//getting the device from the uri
	openni::VideoStream depth;
	rc = m_device.open(deviceUri);
	if (rc != openni::STATUS_OK)
	{
		std::cout << SELCAM << " : Couldn't open device " << deviceUri << ": " << openni::OpenNI::getExtendedError() << std::endl;
		openni::OpenNI::shutdown();
	}

	//NITE
	#ifdef WITH_NITE2
	m_pUserTracker = new nite::UserTracker;
	nite::NiTE::initialize();

	if (m_pUserTracker->create(&m_device) != nite::STATUS_OK)
	{
		std::cout << "OpenniServer: Couldn't create userTracker " << std::endl;
	}
	#endif


	
	m_device.setImageRegistrationMode( openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR );
	//m_device.Device::setDepthColorSyncEnabled(true);

	//depth
	rc = depth.create(m_device, openni::SENSOR_DEPTH);
	if (rc == openni::STATUS_OK)
	{
		rc = depth.start();
		if (rc != openni::STATUS_OK)
		{
			std::cout << "OpenniServer: Couldn't start depth stream: "<< openni::OpenNI::getExtendedError() << std::endl;
			depth.destroy();
		}
	}
	else
	{
		std::cout << "OpenniServer: Couldn't find depth stream: " <<  openni::OpenNI::getExtendedError() << std::endl;
	}

	//color
	rc = color.create(m_device, openni::SENSOR_COLOR);
	if (rc == openni::STATUS_OK)
	{
		rc = color.start();
		if (rc != openni::STATUS_OK)
		{
			std::cout << "OpenniServer: Couldn't start color stream: " << openni::OpenNI::getExtendedError() << std::endl;
			color.destroy();
		}
	}
	else
	{
		std::cout << "OpenniServer: Couldn't find color stream: " << openni::OpenNI::getExtendedError() << std::endl;
	}

	openni::VideoMode depthVideoMode;
	openni::VideoMode colorVideoMode;



	colorVideoMode.setResolution(configWidth,configHeight);
	colorVideoMode.setFps( configFps );
	colorVideoMode.setPixelFormat( openni::PIXEL_FORMAT_RGB888 );
	color.setVideoMode(colorVideoMode);
	depthVideoMode.setResolution(configWidth,configHeight);
	depthVideoMode.setFps( configFps );
	depthVideoMode.setPixelFormat( openni::PIXEL_FORMAT_DEPTH_1_MM );
	depth.setVideoMode(depthVideoMode);


	if (depth.isValid() && color.isValid())
	{
		depthVideoMode = depth.getVideoMode();
		colorVideoMode = color.getVideoMode();

		int depthWidth = depthVideoMode.getResolutionX();
		int depthHeight = depthVideoMode.getResolutionY();
		int colorWidth = colorVideoMode.getResolutionX();
		int colorHeight = colorVideoMode.getResolutionY();

		if (depthWidth == colorWidth &&
			depthHeight == colorHeight)
		{
			m_width = depthWidth;
			m_height = depthHeight;
		}
		else
		{
			std::cout <<  "Error - expect color and depth to be in same resolution: D: " << depthWidth << "x" << depthHeight << "C: " << colorWidth << "x" <<  colorHeight << std::endl;


		}
	}
	else if (depth.isValid())
	{
		depthVideoMode = depth.getVideoMode();
		m_width = depthVideoMode.getResolutionX();
		m_height = depthVideoMode.getResolutionY();
	}
	else if (color.isValid())
	{
		colorVideoMode = color.getVideoMode();
		m_width = colorVideoMode.getResolutionX();
		m_height = colorVideoMode.getResolutionY();
	}
	else
	{
		std::cout << "Error - expects at least one of the streams to be valid..." << std::endl;
	}
	
	distances.resize(m_width*m_height);
	pixelsID.resize(m_width*m_height);

	m_streams = new openni::VideoStream*[2];
	m_streams[0] = &depth;
	m_streams[1] = &color;
	

	depth.setMirroringEnabled(false);
	color.setMirroringEnabled(true);

	
	while(1){
		pthread_mutex_lock(&mutex);
		int changedIndex;

		openni::Status rc = openni::OpenNI::waitForAnyStream(m_streams, 2, &changedIndex);
		#ifndef WITH_NITE2
		if (rc != openni::STATUS_OK)
		{
			std::cout << "Wait failed" << std::endl;
		}

		switch (changedIndex)
		{
		case 0:
			depth.readFrame(&m_depthFrame);
			break;
		case 1:
			color.readFrame(&m_colorFrame);
			break;
		default:
			std::cout << "Error in wait" << std::endl;
			break;
		}
		//nite
		#else
		color.readFrame(&m_colorFrame);
		rcN = m_pUserTracker->readFrame(&userTrackerFrame);
		m_depthFrame = userTrackerFrame.getDepthFrame();
		if (rcN != nite::STATUS_OK)
		{
			std::cout << "GetNextData failed" << std::endl;
			//return;
		}
		#endif




		pthread_mutex_unlock(&mutex);
		//OJO it control
		usleep(1000);
   }
   return NULL;
}



/**
* \brief Class which contains all the functions and variables to make run the Robot Cameras
*/
	class CameraRGB: virtual public jderobot::Camera {
public:
	CameraRGB(std::string& propertyPrefix, const jderobotice::Context& context)
      : prefix(propertyPrefix),context(context),
	imageFmt(),
	imageDescription(new jderobot::ImageDescription()),
	cameraDescription(new jderobot::CameraDescription()),
	replyTask()
	{
	Ice::PropertiesPtr prop = context.properties();

	//fill cameraDescription
	cameraDescription->name = prop->getProperty(prefix+"Name");
	if (cameraDescription->name.size() == 0)
	throw 
		jderobotice::ConfigFileException(ERROR_INFO,"Camera name not configured");

	cameraDescription->shortDescription = prop->getProperty(prefix+"ShortDescription");

	//fill imageDescription
	imageDescription->width = configWidth;
	imageDescription->height = configHeight;
	int playerdetection = prop->getPropertyAsIntWithDefault(prefix+"PlayerDetection",0);
	#ifndef WITH_NITE2
		playerdetection=0;
	#endif
	int fps = prop->getPropertyAsIntWithDefault(prefix+"fps",5);
	//we use formats according to colorspaces
	std::string fmtStr = prop->getPropertyWithDefault(prefix+"Format","YUY2");//default format YUY2
	imageFmt = colorspaces::Image::Format::searchFormat(fmtStr);
	if (!imageFmt)
	throw 
		jderobotice::ConfigFileException(ERROR_INFO, "Format " + fmtStr + " unknown");
	imageDescription->size = imageDescription->width * imageDescription->height * CV_ELEM_SIZE(imageFmt->cvType);
	imageDescription->format = imageFmt->name;

	context.tracer().info("Starting thread for camera: " + cameraDescription->name);
	replyTask = new ReplyTask(this, imageDescription->width, imageDescription->height,fps, playerdetection);

	replyTask->start();//my own thread
	}

	virtual ~CameraRGB(){
		context.tracer().info("Stopping and joining thread for camera: " + cameraDescription->name);
		gbxiceutilacfr::stopAndJoin(replyTask);
	}
    
	virtual jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c){
		return imageDescription;
	}

	virtual jderobot::CameraDescriptionPtr getCameraDescription(const Ice::Current& c){
		return cameraDescription;
	}

	virtual void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb,const Ice::Current& c){
		replyTask->pushJob(cb);
	}

	virtual std::string startCameraStreaming(const Ice::Current&){
		context.tracer().info("Should be made anything to start camera streaming: " + cameraDescription->name);
		return std::string("");
	}

	virtual void stopCameraStreaming(const Ice::Current&) {
		context.tracer().info("Should be made anything to stop camera streaming: " + cameraDescription->name);
	}

	virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr&, const Ice::Current&){
		return 0;
	}

private:
	class ReplyTask: public gbxiceutilacfr::SafeThread{
	public:
		ReplyTask(CameraRGB* camera, int width, int height, int fps, int playerdetection)
	: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycameravga(camera) {
		segmentation=playerdetection;
		this->fps=fps;
      }
		

	void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
		IceUtil::Mutex::Lock sync(requestsMutex);
		requests.push_back(cb);
	}

    virtual void walk(){

		jderobot::ImageDataPtr reply(new jderobot::ImageData);
		reply->description = mycameravga->imageDescription;
		reply->pixelData.resize(mycameravga->imageDescription->width*mycameravga->imageDescription->height*3);
		srcRGB = new cv::Mat(cv::Size(mycameravga->imageDescription->width,mycameravga->imageDescription->height),CV_8UC3);
		cv::Mat dst_resize;


		struct timeval a, b;
		int cycle; // duración del ciclo
		long totala;
		long totalpre=0;
		long diff;

		cycle=(float)(1/(float)fps)*1000000;
		
	
		while(!isStopping()){
			gettimeofday(&a,NULL);
			totala=a.tv_sec*1000000+a.tv_usec;
			pthread_mutex_lock(&mutex);
		    IceUtil::Time t = IceUtil::Time::now();
		    reply->timeStamp.seconds = (long)t.toSeconds();
		    reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;

			if (!m_colorFrame.isValid()){
				pthread_mutex_unlock(&mutex);			
				continue;
			}

			//nite
			#ifdef WITH_NITE2
			const nite::UserMap& userLabels = userTrackerFrame.getUserMap();
			const nite::UserId* pLabels = userLabels.getPixels();
			#endif

			const openni::RGB888Pixel* pImageRow = (const openni::RGB888Pixel*)m_colorFrame.getData();
			int rowSize = m_colorFrame.getStrideInBytes() / sizeof(openni::RGB888Pixel);

			for (int y = 0; y < m_colorFrame.getHeight(); ++y)
			{
				const openni::RGB888Pixel* pImage = pImageRow;
				for (int x = 0; x < m_colorFrame.getWidth(); ++x, ++pImage)
				{
					#ifdef WITH_NITE2
					if (segmentation){
						pixelsID[(y*m_colorFrame.getWidth() + x)]= *pLabels;
						if (*pLabels!=0)
		                {
		                    srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 0] = colors[*pLabels][0];
							srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 1] = colors[*pLabels][1];
							srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 2] = colors[*pLabels][2];
						}
						else{
							srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 0] = 0;
							srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 1] = 0;
							srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 2] = 0;
						}
						++pLabels;
					}
					else{
						srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 0] = pImage->r;
						srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 1] = pImage->g;
						srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 2] = pImage->b;
					}
					#else
						srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 0] = pImage->r;
						srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 1] = pImage->g;
						srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 2] = pImage->b;
					#endif
				}
				pImageRow += rowSize;
			}	
	

			//test
			//CalculateJoints();

			
			//cvFlip(srcRGB, NULL, 1);
			//std::cout << "camara:" <<  (int)mycameravga->imageDescription->width << "x" << (int) mycameravga->imageDescription->height << std::endl;
			//std::cout << "xtion: " <<  m_colorFrame.getWidth() << "x" << m_colorFrame.getHeight() << std::endl;

			if ((mycameravga->imageDescription->width != m_colorFrame.getWidth()) || (mycameravga->imageDescription->height != m_colorFrame.getHeight())){
				std::cout << "Assuming kinect device with resampled on device not working" << std::endl;
				resize(*srcRGB, dst_resize, srcRGB->size(), 0, 0, cv::INTER_LINEAR);
				memcpy(&(reply->pixelData[0]),(unsigned char *) dst_resize.data,dst_resize.cols*dst_resize.rows * 3);
			}
			else{
				memcpy(&(reply->pixelData[0]),(unsigned char *) srcRGB->data,srcRGB->rows*srcRGB->cols * 3);
			}

		    {//critical region start
			IceUtil::Mutex::Lock sync(requestsMutex);
		    while(!requests.empty()){
				jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front();
				requests.pop_front();
				cb->ice_response(reply);
			}

			}//critical region end

			pthread_mutex_unlock(&mutex);
			/*gettimeofday(&b,NULL);
			totalb=b.tv_sec*1000000+b.tv_usec;*/
			if (totalpre !=0){
				if ((totala - totalpre) > cycle ){
					std::cout<<"-------- openniServer: WARNING- RGB timeout-" << std::endl; 
				}
				else{
					usleep(cycle - (totala - totalpre));
				}
			}
			/*if (totalpre !=0){
				std::cout << "rgb: " <<  1000000/(totala-totalpre) << std::endl;
			}*/
			totalpre=totala;
		}
	}
	
	CameraRGB* mycameravga;
	IceUtil::Mutex requestsMutex;
	std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
	unsigned int m_nTexMapX;
	unsigned int m_nTexMapY;
	int m_width;
	int	m_height;
	openni::RGB888Pixel* m_pTexMap;
	int segmentation;
	int fps;
	
    };
    typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;


    std::string prefix;
	jderobotice::Context context;
    colorspaces::Image::FormatPtr imageFmt;
    jderobot::ImageDescriptionPtr imageDescription;
    jderobot::CameraDescriptionPtr cameraDescription;
    ReplyTaskPtr replyTask;


  };


//*********************************************************************/
	class CameraDEPTH: virtual public jderobot::Camera {
public:
	CameraDEPTH(std::string& propertyPrefix, const jderobotice::Context& context)
      : prefix(propertyPrefix),context(context),
	imageFmt(),
	imageDescription(new jderobot::ImageDescription()),
	cameraDescription(new jderobot::CameraDescription()),
	replyTask()
	{
      
      
	Ice::PropertiesPtr prop = context.properties();

	//fill cameraDescription
	cameraDescription->name = prop->getProperty(prefix+"Name");
	if (cameraDescription->name.size() == 0)
	throw 
		jderobotice::ConfigFileException(ERROR_INFO,"Camera name not configured");

	cameraDescription->shortDescription = prop->getProperty(prefix+"ShortDescription");

	//fill imageDescription
	imageDescription->width = configWidth;
	int playerdetection = prop->getPropertyAsIntWithDefault(prefix+"PlayerDetection",0);
	#ifndef WITH_NITE2
		playerdetection=0;
	#endif
	imageDescription->height = configHeight;
	int fps = prop->getPropertyAsIntWithDefault(prefix+"fps",5);
	//we use formats acording to colorspaces
	std::string fmtStr = prop->getPropertyWithDefault(prefix+"Format","YUY2");//default format YUY2
	imageFmt = colorspaces::Image::Format::searchFormat(fmtStr);
	if (!imageFmt)
	throw 
		jderobotice::ConfigFileException(ERROR_INFO, "Format " + fmtStr + " unknown");
	imageDescription->size = imageDescription->width * imageDescription->height * CV_ELEM_SIZE(imageFmt->cvType);
	imageDescription->format = imageFmt->name;

	context.tracer().info("Starting thread for camera: " + cameraDescription->name);
	replyTask = new ReplyTask(this, imageDescription->width, imageDescription->height,fps, playerdetection);

	replyTask->start();//my own thread
	}

	virtual ~CameraDEPTH(){
		context.tracer().info("Stopping and joining thread for camera: " + cameraDescription->name);
		gbxiceutilacfr::stopAndJoin(replyTask);
	}
    
	virtual jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c){
		return imageDescription;
	}

	virtual jderobot::CameraDescriptionPtr getCameraDescription(const Ice::Current& c){
		return cameraDescription;
	}

	virtual void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb,const Ice::Current& c){
		replyTask->pushJob(cb);
	}

	virtual std::string startCameraStreaming(const Ice::Current&){
		context.tracer().info("Should be made anything to start camera streaming: " + cameraDescription->name);
		return std::string("");
	}

	virtual void stopCameraStreaming(const Ice::Current&) {
		context.tracer().info("Should be made anything to stop camera streaming: " + cameraDescription->name);
	}
	
	virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr&, const Ice::Current&){
		return 0;
	}

private:
	class ReplyTask: public gbxiceutilacfr::SafeThread{
	public:
		ReplyTask(CameraDEPTH* camera, int width, int height, int fps, int playerDetection)
	: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycameradepth(camera) {
		segmentation=playerDetection;
		this->fps=fps;
      }
		

	void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
		IceUtil::Mutex::Lock sync(requestsMutex);
		requests.push_back(cb);
	}

    virtual void walk(){
		int test;
		

		jderobot::ImageDataPtr reply(new jderobot::ImageData);
		reply->description = mycameradepth->imageDescription;
		reply->pixelData.resize(mycameradepth->imageDescription->width*mycameradepth->imageDescription->height*3);
		cv::Mat dst_resize(cv::Size(mycameradepth->imageDescription->width, mycameradepth->imageDescription->height),CV_8UC3);
		cv::Mat src(cv::Size(mycameradepth->imageDescription->width, mycameradepth->imageDescription->height),CV_8UC3);

		struct timeval a, b;
		int cycle; // duración del ciclo
		long totalb,totala;
		long totalpre=0;
		long diff;

		//std::cout << "FPS depth: " << fps << std::endl;
		cycle=(float)(1/(float)fps)*1000000;


		
	
		while(!isStopping()){
			gettimeofday(&a,NULL);
			totala=a.tv_sec*1000000+a.tv_usec;
			pthread_mutex_lock(&mutex);
			src=cv::Scalar(0, 0, 0);

		    IceUtil::Time t = IceUtil::Time::now();
		    reply->timeStamp.seconds = (long)t.toSeconds();
		    reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
			if (!m_depthFrame.isValid()){
				pthread_mutex_unlock(&mutex);			
				continue;
			}

			//cvZero(src);

			//nite
			#ifdef WITH_NITE2
			const nite::UserMap& userLabels = userTrackerFrame.getUserMap();
			const nite::UserId* pLabels = userLabels.getPixels();
			#endif

			const openni::DepthPixel* pDepth = (const openni::DepthPixel*)m_depthFrame.getData();
			int restOfRow = m_depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel) - m_depthFrame.getWidth();

			for (int y = 0; y < m_depthFrame.getHeight(); ++y)
			{	
				for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pDepth)
				{
					#ifdef WITH_NITE2
					if ((*pLabels!=0)||(!segmentation)){
						distances[(y*m_depthFrame.getWidth() + x)] = *pDepth;
						if (*pDepth != 0)
						{
							src.data[(y*m_depthFrame.getWidth()+ x)*3+0] = (float(*pDepth)/(float)MAX_LENGHT)*255.;
							src.data[(y*m_depthFrame.getWidth()+ x)*3+1] = (*pDepth)>>8;
							src.data[(y*m_depthFrame.getWidth()+ x)*3+2] = (*pDepth)&0xff;

						}
						else{
							src.data[(y*m_depthFrame.getWidth()+ x)*3+0] = 0;
							src.data[(y*m_depthFrame.getWidth()+ x)*3+1] = 0;
							src.data[(y*m_depthFrame.getWidth()+ x)*3+2] = 0;
						}
					}
					else{
						src.data[(y*m_depthFrame.getWidth()+ x)*3+0] = 0;
						src.data[(y*m_depthFrame.getWidth()+ x)*3+1] = 0;
						src.data[(y*m_depthFrame.getWidth()+ x)*3+2] = 0;
					}
					++pLabels;
					#else
						distances[(y*m_depthFrame.getWidth() + x)] = *pDepth;
						if (*pDepth != 0)
						{
							src.data[(y*m_depthFrame.getWidth()+ x)*3+0] = (float(*pDepth)/(float)MAX_LENGHT)*255.;
							src.data[(y*m_depthFrame.getWidth()+ x)*3+1] = (*pDepth)>>8;
							src.data[(y*m_depthFrame.getWidth()+ x)*3+2] = (*pDepth)&0xff;
					}
					#endif
				}
				pDepth += restOfRow;
			}

		   if ((mycameradepth->imageDescription->width != m_depthFrame.getWidth()) || (mycameradepth->imageDescription->height != m_depthFrame.getHeight())){
				//cv::resize(src,dst_resize);
				cv::resize(src, dst_resize, dst_resize.size(), 0, 0, cv::INTER_LINEAR);
				std::cout << "resize depth" << std::endl;
				memcpy(&(reply->pixelData[0]),(unsigned char *) dst_resize.data,dst_resize.cols*dst_resize.rows * 3);
			}
			else{
				memcpy(&(reply->pixelData[0]),(unsigned char *) src.data,src.cols*src.rows * 3);
			}
		    
		    {//critical region start
			IceUtil::Mutex::Lock sync(requestsMutex);
		    while(!requests.empty()){
				jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front();
				requests.pop_front();
				cb->ice_response(reply);
			}
			}//critical region end
			pthread_mutex_unlock(&mutex);
			/*gettimeofday(&b,NULL);
			totalb=b.tv_sec*1000000+b.tv_usec;*/
			if (totalpre !=0){
				if ((totala - totalpre) > cycle ){
					std::cout<<"-------- openniServer: WARNING- DEPTH timeout-" << std::endl; 
				}
				else{
					usleep(cycle - (totala-totalpre));
				}
			}
			/*if (totalpre !=0){
				std::cout << "depth: " <<  1000000/(totala-totalpre) << std::endl;
			}*/
			totalpre=totala;
			
		}
	}
	
	CameraDEPTH* mycameradepth;
	IceUtil::Mutex requestsMutex;
	std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
	

	int segmentation;
	int fps;
	
	
    };
    typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;


    std::string prefix;
    jderobotice::Context context;
    colorspaces::Image::FormatPtr imageFmt;
    jderobot::ImageDescriptionPtr imageDescription;
    jderobot::CameraDescriptionPtr cameraDescription;
    ReplyTaskPtr replyTask;

  };

/**
* \brief Class wich contains all the functions and variables to serve point cloud interface
*/

	class pointCloudI: virtual public jderobot::pointCloud{
		public:
			pointCloudI (std::string& propertyPrefix, const jderobotice::Context& context):
				prefix(propertyPrefix),context(context),data(new jderobot::pointCloudData()) {
					Ice::PropertiesPtr prop = context.properties();

					int playerdetection = prop->getPropertyAsIntWithDefault("openniServer.PlayerDetection",0);
					int fps =prop->getPropertyAsIntWithDefault("openniServer.pointCloud.Fps",10);
					#ifndef WITH_NITE2
						playerdetection=0;
					#endif
					   replyCloud = new ReplyCloud(this,prop->getProperty("openniServer.calibration"), playerdetection, configWidth, configHeight,fps);
					   replyCloud->start();
				}
		

		virtual jderobot::pointCloudDataPtr getCloudData(const Ice::Current&){
				data=replyCloud->getCloud();
				return data;
			};
		   
		   private:
			 class ReplyCloud :public gbxiceutilacfr::SafeThread{ 
		       public: 
		       	ReplyCloud (pointCloudI* pcloud, std::string filepath,  int playerDetection, int widthIn, int heightIn, int fpsIn) : gbxiceutilacfr::SafeThread(pcloud->context.tracer()), data(new jderobot::pointCloudData()), data2(new jderobot::pointCloudData())
		        	{
					path=filepath;
					segmentation=playerDetection;
					cWidth = widthIn;
					cHeight = heightIn;
					fps=fpsIn;
				}
		       
		        void walk()
		        {
				mypro= new openniServer::myprogeo();
				mypro->load_cam((char*)path.c_str(),0, cWidth, cHeight);
				
				struct timeval a, b;
				int cycle; // duración del ciclo
				long totala;
				long totalpre=0;

				cycle=(float)(1/(float)fps)*1000000;

				while(!isStopping()){
					float distance;
					gettimeofday(&a,NULL);
					totala=a.tv_sec*1000000+a.tv_usec;
					pthread_mutex_lock(&mutex);
					data2->p.clear();
					for( unsigned int i = 0 ; (i < cWidth*cHeight)&&(distances.size()>0); i=i+9) {
							distance=(float)distances[i];
							if (distance!=0){
								//if (((unsigned char)srcRGB->data[3*i]!=0) && ((unsigned char)srcRGB->data[3*i+1]!=0) && ((unsigned char)srcRGB->data[3*i+2]!=0)){
									float xp,yp,zp,camx,camy,camz;
									float ux,uy,uz; 
									float x,y;
									float k;
									float c1x, c1y, c1z;
									float fx,fy,fz;
									float fmod;
									float t;
									float Fx,Fy,Fz;
				
									mypro->mybackproject(i % cWidth, i / cWidth, &xp, &yp, &zp, &camx, &camy, &camz,0);
				
									//vector unitario
									float modulo;
				
									modulo = sqrt(1/(((camx-xp)*(camx-xp))+((camy-yp)*(camy-yp))+((camz-zp)*(camz-zp))));
									mypro->mygetcamerafoa(&c1x, &c1y, &c1z, 0);
	
									fmod = sqrt(1/(((camx-c1x)*(camx-c1x))+((camy-c1y)*(camy-c1y))+((camz-c1z)*(camz-c1z))));
									fx = (c1x - camx)*fmod;
									fy = (c1y - camy)*fmod;
									fz = (c1z - camz) * fmod;
									ux = (xp-camx)*modulo;
									uy = (yp-camy)*modulo;
									uz = (zp-camz)*modulo;
									Fx= distance*fx + camx;
									Fy= distance*fy + camy;
									Fz= distance*fz + camz;
									// calculamos el punto real 
									t = (-(fx*camx) + (fx*Fx) - (fy*camy) + (fy*Fy) - (fz*camz) + (fz*Fz))/((fx*ux) + (fy*uy) + (fz*uz));
									auxP.x=t*ux + camx;
									auxP.y=t*uy+ camy;
									auxP.z=t*uz + camz;
									if ( segmentation){
										auxP.id=pixelsID[i];
									}
									auxP.r=(float)(int) (unsigned char)srcRGB->data[3*i];
									auxP.g=(float)(int) (unsigned char)srcRGB->data[3*i+1];
									auxP.b=(float)(int) (unsigned char)srcRGB->data[3*i+2];
									data2->p.push_back(auxP);
								}
							//}
						}
					pthread_mutex_unlock(&mutex);
					if (totalpre !=0){
						if ((totala - totalpre) > cycle ){
							std::cout<<"-------- openniServer: WARNING- POINTCLOUD timeout-" << std::endl; 
						}
						else{
							usleep(cycle - (totala - totalpre));
						}
					}
					/*if (totalpre !=0){
						std::cout << "cloud: " <<  1000000/(totala-totalpre) << std::endl;
					}*/
					totalpre=totala;
				}
		        }
		        myprogeo *mypro;
				int cWidth;
				int cHeight;
				int fps;
				jderobot::pointCloudDataPtr data, data2;
				jderobot::RGBPoint auxP;
				std::string path;
				int segmentation;
		        
		       jderobot::pointCloudDataPtr getCloud()
		       {
		          pthread_mutex_lock(&mutex);
				data->p=data2->p;
				pthread_mutex_unlock(&mutex);
		          return data;
		       }
				
		      
		    	};	

			typedef IceUtil::Handle<ReplyCloud> ReplyCloudPtr;
			ReplyCloudPtr replyCloud;
			std::string prefix;
			jderobotice::Context context;
			jderobot::pointCloudDataPtr data;
			
			
		};




/**
* \brief Class wich contains all the functions and variables to controle the Pose3DMotors module
*/
/*class Pose3DMotorsI: virtual public jderobot::Pose3DMotors {
	public:
		Pose3DMotorsI(XN_USB_DEV_HANDLE* d, std::string& propertyPrefix, const jderobotice::Context& context): prefix(propertyPrefix),context(context),Pose3DMotorsData(new jderobot::Pose3DMotorsData()), Pose3DMotorsParams(new jderobot::Pose3DMotorsParams())
		{
			Ice::PropertiesPtr prop = context.properties();
			Pose3DMotorsData->tilt=0;
			Pose3DMotorsData->tiltSpeed=0;
			rc= XN_STATUS_OK;
			dev=d;
			rc=xnUSBSendControl( *dev, XN_USB_CONTROL_TYPE_VENDOR, 0x06, 1, 0x00, NULL, 0, 0 );
			CHECK_RC(rc,"led");
     	}

		virtual ~Pose3DMotorsI(){};

		virtual  Ice::Int setPose3DMotorsData(const jderobot::Pose3DMotorsDataPtr& p, const Ice::Current&){
			Pose3DMotorsData=p;
			uint8_t empty[0x1];
			//int angle = 25 * 2;
			rc = xnUSBSendControl(*dev, XN_USB_CONTROL_TYPE_VENDOR, 0x31, (XnUInt16)p->tilt, 0x0, empty, 0x0, 0);
			CHECK_RC(rc,"Changing angle");

		};
	
		virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const Ice::Current&){
			return Pose3DMotorsParams;
		};

		virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData (const Ice::Current&){
			return Pose3DMotorsData;
		};

	private:
		std::string prefix;
		jderobotice::Context context;
		jderobot::Pose3DMotorsDataPtr Pose3DMotorsData;
		jderobot::Pose3DMotorsParamsPtr Pose3DMotorsParams;
		XnStatus rc;
		XN_USB_DEV_HANDLE* dev;
    };*/

/**
* \brief Class wich contains all the functions and variables to controle the KinectLeds module
*/
/*class KinectLedsI: virtual public jderobot::KinectLeds {
	public:
		KinectLedsI(XN_USB_DEV_HANDLE* d, std::string& propertyPrefix, const jderobotice::Context& context): prefix(propertyPrefix),context(context)
		{
			Ice::PropertiesPtr prop = context.properties();
			dev=d;
     	}

		virtual ~KinectLedsI(){};

		virtual  void setLedActive(jderobot::KinectLedsAvailable led, const Ice::Current&){
			int iled;
			if (led==jderobot::OFF)
				iled=0;
			if (led==jderobot::GREEN)
				iled=1;
			if (led==jderobot::RED)
				iled=2;
			if (led==jderobot::YELLOW)
				iled=3;
			if (led==jderobot::BLINKYELLOW)
				iled=4;
			if (led==jderobot::BLINKGREEN)
				iled=5;
			if (led==jderobot::BLINKRED)
				iled=6;
			uint8_t empty[0x1];
			rc = xnUSBSendControl(*dev, XN_USB_CONTROL_TYPE_VENDOR, 0x6, iled, 0x0, empty, 0x0, 0);
			CHECK_RC(rc,"Changing led");
		}

	private:
		std::string prefix;
		jderobotice::Context context;
		XN_USB_DEV_HANDLE* dev;
    };*/

/**
* \brief Main Class of the component wich create the diferents devices activated using the Ice configuration file.
*/
class Component: public jderobotice::Component{
public:
	Component()
	:jderobotice::Component("openniServer"){}

	virtual void start(){
		Ice::PropertiesPtr prop = context().properties();
		int cameraR = prop->getPropertyAsIntWithDefault(context().tag() + ".CameraRGB",0);
		int cameraD = prop->getPropertyAsIntWithDefault(context().tag() + ".CameraDEPTH",0);
		int motors = prop->getPropertyAsIntWithDefault(context().tag() + ".Pose3DMotorsActive",0);
		int leds = prop->getPropertyAsIntWithDefault(context().tag() + ".KinectLedsActive",0);
		int pointCloud = prop->getPropertyAsIntWithDefault(context().tag() + ".pointCloudActive",0);
		int playerdetection = prop->getPropertyAsIntWithDefault(context().tag() + ".PlayerDetection",0);
		configWidth=prop->getPropertyAsIntWithDefault("openniServer.Width", 320);
		configHeight=prop->getPropertyAsIntWithDefault("openniServer.Height",240);
		configFps=prop->getPropertyAsIntWithDefault("openniServer.Fps",30);
		
		

		SELCAM = prop->getPropertyAsIntWithDefault(context().tag() + ".deviceId",0);
		std::cout << "Selected device: " << SELCAM << std::endl;
		int nCameras=0;


		/*COLORS*/
		colors[0][0]=0;
		colors[0][1]=0;
		colors[0][2]=255;
		colors[1][0]=0;
		colors[1][1]=255;
		colors[1][2]=255;
		colors[2][0]=255;
		colors[2][1]=255;
		colors[2][2]=0;
		colors[3][0]=255;
		colors[3][1]=0;
		colors[3][2]=0;
		colors[4][0]=0;
		colors[4][1]=255;
		colors[4][2]=0;
		colors[5][0]=255;
		colors[5][1]=255;
		colors[5][2]=0;
		colors[6][0]=0;
		colors[6][1]=0;
		colors[6][2]=0;
		colors[7][0]=150;
		colors[7][1]=150;
		colors[7][2]=0;
		colors[8][0]=150;
		colors[8][1]=150;
		colors[8][2]=150;
		colors[9][0]=0;
		colors[9][1]=150;
		colors[9][2]=150;

		nCameras=cameraR + cameraD;
		//g_context =  new xn::Context;
		std::cout << "NCAMERAS = " << nCameras << std::endl;
		cameras.resize(nCameras);
		pthread_mutex_init(&mutex, NULL);
		if ((nCameras>0)||(pointCloud)){
			

			pthread_create(&threads[0], NULL, &openniServer::updateThread, NULL);

		}

		if ((motors) || (leds)){
			/*const XnUSBConnectionString *paths; 
			XnUInt32 count; 
			std::cout << "inicializo el dispositivo" << std::endl;
			rc = xnUSBInit();
			CHECK_RC(rc, "USB Initialization") ;
			//rc = xnUSBOpenDevice(VID_MICROSOFT, PID_NUI_MOTOR, NULL, NULL, &dev);
			CHECK_RC(rc,"Openning Device");
			rc = xnUSBEnumerateDevices(VID_MICROSOFT, PID_NUI_MOTOR, &paths, &count);
             	CHECK_RC(rc,"xnUSBEnumerateDevices failed");


	        	// Open first found device
        		rc = xnUSBOpenDeviceByPath(paths[SELCAM], &dev);
	        	CHECK_RC(rc,"xnUSBOpenDeviceByPath failed");*/
		}

		if (cameraR){
			std::string objPrefix(context().tag() + ".CameraRGB.");
			std::string cameraName = prop->getProperty(objPrefix + "Name");
			if (cameraName.size() == 0){//no name specified, we create one using the index
				cameraName = "cameraR";
				prop->setProperty(objPrefix + "Name",cameraName);//set the value
				}
			context().tracer().info("Creating camera " + cameraName);
			cameras[0] = new CameraRGB(objPrefix,context());
			context().createInterfaceWithString(cameras[0],cameraName);
			std::cout<<"              -------- openniServer: Component: CameraRGB created successfully   --------" << std::endl;
		}
		if (cameraD){
			std::string objPrefix(context().tag() + ".CameraDEPTH.");
			std::string cameraName = prop->getProperty(objPrefix + "Name");
			if (cameraName.size() == 0){//no name specified, we create one using the index
				cameraName = "cameraD";
				prop->setProperty(objPrefix + "Name",cameraName);//set the value
				}
			context().tracer().info("Creating camera " + cameraName);
			cameras[1] = new CameraDEPTH(objPrefix,context());
			context().createInterfaceWithString(cameras[1],cameraName);
			//test camera ok
			std::cout<<"              -------- openniServer: Component: CameraDEPTH created successfully   --------" << std::endl;
		}
		if (motors){
			/*std::string objPrefix4="Pose3DMotors1";
			std::string Pose3DMotorsName = "Pose3DMotors1";
			context().tracer().info("Creating Pose3DMotors1 " + Pose3DMotorsName);
			Pose3DMotors1 = new Pose3DMotorsI(&dev,objPrefix4,context());
			context().createInterfaceWithString(Pose3DMotors1,Pose3DMotorsName);
			std::cout<<"              -------- openniServer: Component: Pose3DMotors created successfully   --------" << std::endl;*/
		}
			
		if (leds){
			/*std::string objPrefix4="kinectleds1";
			std::string Name = "kinectleds1";
			context().tracer().info("Creating kinectleds1 " + Name);
			kinectleds1 = new KinectLedsI(&dev,objPrefix4,context());
			context().createInterfaceWithString(kinectleds1,Name);
			std::cout<<"              -------- openniServer: Component: KinectLeds created successfully   --------" << std::endl;
			*/
		}
		if (pointCloud){
			std::string objPrefix5="pointcloud1";
			std::string Name = "pointcloud1";
			context().tracer().info("Creating pointcloud1 " + Name);
			pointcloud1 = new pointCloudI(objPrefix5,context());
			context().createInterfaceWithString(pointcloud1,Name);
			std::cout<<"              -------- openniServer: Component: PointCloud created successfully   --------" << std::endl;
		}

		std::cout << "LISTOOOOOOOOOOOO" << std::endl;
		sleep(50);
    }

    virtual ~Component(){
    }

  private:
    std::vector<Ice::ObjectPtr> cameras;
	Ice::ObjectPtr Pose3DMotors1;
	Ice::ObjectPtr kinectleds1;
	Ice::ObjectPtr pointcloud1;
	pthread_t threads[NUM_THREADS];
	//XN_USB_DEV_HANDLE dev;

  };

} //namespace

int main(int argc, char** argv){

  openniServer::Component component;

	//usleep(1000);
     jderobotice::Application app(component);

     return app.jderobotMain(argc,argv);

}
