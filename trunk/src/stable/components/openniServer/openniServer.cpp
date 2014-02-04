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
#include "myprogeo.h"
#include <OpenNI.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/video/background_segm.hpp>
#include <signal.h>


#ifdef WITH_NITE2
	#include "NiTE.h"
#endif


#define VID_MICROSOFT 0x45e
#define PID_NUI_MOTOR 0x02b0
#define NUM_THREADS 5
#define MAX_LENGHT 10000
#define SAMPLE_READ_WAIT_TIMEOUT 2000
#define RETRY_MAX_TIMES 5

#define CHECK_RC(rc, what)                                      \
if (rc != openni::STATUS_OK)                                         \
{                                                               \
        std::cout << what << " failed: " << openni::OpenNI::getExtendedError() << std::endl;     \
                                                    \
}


#ifdef WITH_NITE2
	nite::UserTracker* m_pUserTracker;
	nite::UserTrackerFrameRef userTrackerFrame;
	nite::Status rcN;
#endif





//global configuration
openni::VideoFrameRef		m_depthFrame;
openni::VideoFrameRef		m_colorFrame;
openni::Device			m_device;
int cameraR, cameraD,cameraIR, ImageRegistration;
int colors[10][3];
int SELCAM;
pthread_mutex_t mutex;
bool componentAlive;
pthread_t updateThread;
int deviceMode; //videmode for device streamings

int retry_times = 0;

//block to wait the device initialization
IceUtil::Mutex controlMutex;
IceUtil::Cond sem;
int mirrorDepth, mirrorRGB;
int debug;


namespace openniServer{


std::vector<int> distances;
std::vector<int> pixelsID;
cv::Mat* srcRGB;
int userGeneratorActive=0;
openni::VideoStream depth, color, ir;
openni::VideoStream** m_streams;
openni::VideoMode depthVideoMode;
openni::VideoMode colorVideoMode;




int segmentationType; //0 ninguna, 1 NITE
int mainFPS;



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




	//depth
	if (cameraD){
		rc = depth.create(m_device, openni::SENSOR_DEPTH);
		if (rc == openni::STATUS_OK)
		{
			//rc = depth.start();
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

		if (mirrorDepth){
			rc=depth.setMirroringEnabled(true);
			if (rc != openni::STATUS_OK)
			{
				std::cout << "OpenniServer: error at set depth mirror: " << openni::OpenNI::getExtendedError() << std::endl;
			}
		}
		else{
			rc=depth.setMirroringEnabled(false);
			if (rc != openni::STATUS_OK)
			{
				std::cout << "OpenniServer: error at set depth mirror: " << openni::OpenNI::getExtendedError() << std::endl;
			}
		}
	}

	//color
	if (cameraR){
		rc = color.create(m_device, openni::SENSOR_COLOR);
		if (rc == openni::STATUS_OK)
		{
			//rc = color.start();
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
		if (mirrorRGB){
			rc=color.setMirroringEnabled(true);
			if (rc != openni::STATUS_OK)
			{
				std::cout << "OpenniServer: error at set color mirror: " << openni::OpenNI::getExtendedError() << std::endl;
			}
		}
		else{
			rc=color.setMirroringEnabled(false);
			if (rc != openni::STATUS_OK)
			{
				std::cout << "OpenniServer: error at set color mirror: " << openni::OpenNI::getExtendedError() << std::endl;
			}
		}
	}







	if (cameraD){
		const openni::SensorInfo *depthSensorInfo = m_device.getSensorInfo(openni::SENSOR_DEPTH);
		rc= depth.setVideoMode(depthSensorInfo->getSupportedVideoModes()[deviceMode]);
		if (rc != openni::STATUS_OK)
		{
			std::cout << "OpenniServer: error at set depth videoMode: " << openni::OpenNI::getExtendedError() << std::endl;
		}
		if (debug){
			std::cout << "OpenniServer: depth modes" << std::endl;
			for(int i=0;i < depthSensorInfo->getSupportedVideoModes().getSize();i++)
			{
				openni::VideoMode videoMode = depthSensorInfo->getSupportedVideoModes()[i];
				std::cout << "fps: " << videoMode.getFps() << "x: " << videoMode.getResolutionX() << "y " <<  videoMode.getResolutionY() << std::endl;
			}
		}
		depthVideoMode = depth.getVideoMode();
		depth.start();

	}

	if (cameraR){
		const openni::SensorInfo *colorSensorInfo = m_device.getSensorInfo(openni::SENSOR_COLOR);
		rc= color.setVideoMode(colorSensorInfo->getSupportedVideoModes()[deviceMode]);
		if (rc != openni::STATUS_OK)
		{
			std::cout << "OpenniServer: error at set color videoMode: " << openni::OpenNI::getExtendedError() << std::endl;
			color.destroy();
		}
		if (debug){
			std::cout << "OpenniServer color modes:" << std::endl;
			for(int i=0;i < colorSensorInfo->getSupportedVideoModes().getSize();i++)
			{
				openni::VideoMode videoMode = colorSensorInfo->getSupportedVideoModes()[i];
				std::cout << "fps: " << videoMode.getFps() << "x: " << videoMode.getResolutionX() << "y " <<  videoMode.getResolutionY() << std::endl;
			}
		}
		colorVideoMode = color.getVideoMode();
		srcRGB = new cv::Mat(cv::Size(colorVideoMode.getResolutionX(),colorVideoMode.getResolutionY()),CV_8UC3);
		color.start();
	}



	if ((cameraR)&&(cameraD)&&(ImageRegistration)){
		rc=m_device.setImageRegistrationMode( openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR );

		if (rc != openni::STATUS_OK)
		{
			std::cout << "OpenniServer: error at set registration: " << openni::OpenNI::getExtendedError() << std::endl;
		}
	}




	std::cout << "COLOR: fps:" << color.getVideoMode().getFps() << "w" << color.getVideoMode().getResolutionX() << "h" << color.getVideoMode().getResolutionY() << std::endl;
	std::cout << "DEPTH: fps:" << depth.getVideoMode().getFps() << "w" << depth.getVideoMode().getResolutionX() << "h" << depth.getVideoMode().getResolutionY() << std::endl;

	if (cameraR && cameraD){
		rc=m_device.setDepthColorSyncEnabled(true);
		if (rc != openni::STATUS_OK)
		{
			std::cout << "OpenniServer: error at set syncronization: " << openni::OpenNI::getExtendedError() << std::endl;
		}
		if (depth.isValid() && color.isValid())
		{

			int depthWidth = depthVideoMode.getResolutionX();
			int depthHeight = depthVideoMode.getResolutionY();
			int colorWidth = colorVideoMode.getResolutionX();
			int colorHeight = colorVideoMode.getResolutionY();

			if (depthWidth == colorWidth &&
				depthHeight == colorHeight)
			{
			}
			else
			{
				std::cout <<  "Error - expect color and depth to be in same resolution: D: " << depthWidth << "x" << depthHeight << "C: " << colorWidth << "x" <<  colorHeight << std::endl;


			}
		}
	}

	distances.resize(depth.getVideoMode().getResolutionX()*depth.getVideoMode().getResolutionY());
	pixelsID.resize(depth.getVideoMode().getResolutionX()*depth.getVideoMode().getResolutionY());



	//NITE
		#ifdef WITH_NITE2

		if (segmentationType){
			m_pUserTracker = new nite::UserTracker;
			nite::NiTE::initialize();

			if (m_pUserTracker->create(&m_device) != nite::STATUS_OK)
			{
				std::cout << "OpenniServer: Couldn't create userTracker " << std::endl;
			}
		}
		#endif

	m_streams = new openni::VideoStream*[2];
	m_streams[0] = &depth;
	m_streams[1] = &color;

	sem.broadcast();


	//diferente en arm que en x86???
	int cycle=(float)(1/(float)mainFPS)*1000000;
	IceUtil::Time lastIT=IceUtil::Time::now();
	bool first=true;


	while(componentAlive){



		int changedIndex;

		openni::Status rc;
		try{
			rc=openni::OpenNI::waitForAnyStream(m_streams, 2, &changedIndex,SAMPLE_READ_WAIT_TIMEOUT);
			if (rc != openni::STATUS_OK)
			{
				std::cout<< "Wait failed! (timeout is " << SAMPLE_READ_WAIT_TIMEOUT <<  "ms) " << openni::OpenNI::getExtendedError() << std::endl;
				retry_times++;
				if (retry_times > RETRY_MAX_TIMES)
				{
					std::cout << "Retry Max Times exceeded!. Force Exit!!" << std::endl;
					exit(-1);
				}
				continue;
			}
		}
		catch ( std::exception& ex) {
			std::cerr << ex.what() << std::endl;
		}

		if (rc != openni::STATUS_OK)
		{
			std::cout << "Wait failed" << std::endl;
		}
		else if(first){
			std::cout << "OpenniServer initialized" << std::endl;
			first=false;
		}
		/*switch (changedIndex)
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
		}*/

		pthread_mutex_lock(&mutex);

		if (cameraD)
			depth.readFrame(&m_depthFrame);
		if (cameraR){
			color.readFrame(&m_colorFrame);
		}


		//nite

		#ifdef WITH_NITE2
			if (segmentationType){
				rcN = m_pUserTracker->readFrame(&userTrackerFrame);
				m_depthFrame = userTrackerFrame.getDepthFrame();
				if (rcN != nite::STATUS_OK)
				{
					std::cout << "GetNextData failed" << std::endl;
					//return;
				}
			}
		#endif




		pthread_mutex_unlock(&mutex);
		int delay = IceUtil::Time::now().toMicroSeconds() - lastIT.toMicroSeconds();
		if (delay > cycle ){
			if (debug==3)
				std::cout<<"-------- openniServer: MAIN openni timeout-" << std::endl;
		}
		else{
			if (delay <1 || delay > cycle)
				delay = 1;
			usleep(delay);
		}

		lastIT=IceUtil::Time::now();

   }
	if (cameraD){
		depth.stop();
		depth.destroy();
	}
	if (cameraR){
		color.stop();
		color.destroy();
	}

		#ifdef WITH_NITE2
	if (segmentationType){
		nite::NiTE::shutdown();
	}
		#endif


   return NULL;
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
		std::cout << "Camera name not configured" << std::endl;

	cameraDescription->shortDescription = prop->getProperty(prefix + "ShortDescription");

	//fill imageDescription
	imageDescription->width = colorVideoMode.getResolutionX();
	imageDescription->height = colorVideoMode.getResolutionY();
	int playerdetection = prop->getPropertyAsIntWithDefault(prefix+"PlayerDetection",0);

	#ifndef WITH_NITE2
		playerdetection=0;
	#endif
	int fps = prop->getPropertyAsIntWithDefault(prefix+"fps",5);
	//we use formats according to colorspaces
	std::string fmtStr = prop->getPropertyWithDefault(prefix+"Format","YUY2");//default format YUY2
	imageFmt = colorspaces::Image::Format::searchFormat(fmtStr);
	if (!imageFmt)
		std::cout <<  "Format " << fmtStr << " unknown" << std::endl;
	imageDescription->size = imageDescription->width * imageDescription->height * CV_ELEM_SIZE(imageFmt->cvType);
	imageDescription->format = imageFmt->name;

	std::cout << "Starting thread for camera: " << cameraDescription->name << std::endl;
	replyTask = new ReplyTask(this,fps, playerdetection);

	this->control=replyTask->start();//my own thread
	}

	virtual ~CameraRGB(){
		std::cout << "Stopping and joining thread for camera: " << cameraDescription->name << std::endl;
		replyTask->destroy();
		this->control.join();
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
		std::cout << "Should be made anything to start camera streaming: " << cameraDescription->name<< std::endl;
		return std::string("");
	}

	virtual void stopCameraStreaming(const Ice::Current&) {
		std::cout << "Should be made anything to stop camera streaming: " <<  cameraDescription->name << std::endl;
	}
	virtual void reset(const Ice::Current&)
	{
	}

	virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr&, const Ice::Current&){
		return 0;
	}

private:
	class ReplyTask: public IceUtil::Thread{
	public:
		ReplyTask(CameraRGB* camera, int fps, int playerdetection):mycameravga(camera),_done(false) {
		segmentation=playerdetection;
		this->fps=fps;
      }
		

	void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
		IceUtil::Mutex::Lock sync(requestsMutex);
		requests.push_back(cb);
	}

    virtual void run(){

		jderobot::ImageDataPtr reply(new jderobot::ImageData);
		reply->description = mycameravga->imageDescription;
		reply->pixelData.resize(mycameravga->imageDescription->width*mycameravga->imageDescription->height*3);
		cv::Mat dst_resize;



		int cycle; // duración del ciclo

		cycle=(float)(1/(float)fps)*1000000;
		
		IceUtil::Time lastIT=IceUtil::Time::now();
		while(!(_done)){
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
				const nite::UserId* pLabels;
				if (segmentation){
					const nite::UserMap& userLabels = userTrackerFrame.getUserMap();
					pLabels= userLabels.getPixels();
				}
			#endif

			const openni::RGB888Pixel* pImageRow = (const openni::RGB888Pixel*)m_colorFrame.getData();
			int rowSize = m_colorFrame.getStrideInBytes() / sizeof(openni::RGB888Pixel);



			for (int y = 0; y < m_colorFrame.getHeight(); ++y)
			{
				const openni::RGB888Pixel* pImage = pImageRow;
				for (int x = 0; x < m_colorFrame.getWidth(); ++x, ++pImage)
				{
					switch(segmentation){
						case 0:
							srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 0] = pImage->r;
							srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 1] = pImage->g;
							srcRGB->data[(y*m_colorFrame.getWidth() + x)*3 + 2] = pImage->b;
							break;
						case 1:
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
							#endif
							break;
						case 2:

						default:
							std::cout << "openniServer: Error segmentation not supported" << std::endl;
							break;
					}


				}
				pImageRow += rowSize;
			}	
	
			if (debug==2){
				cv::imshow("OpenniServer RGB", *srcRGB);
				cv::waitKey(1);
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


			int delay = IceUtil::Time::now().toMicroSeconds() - lastIT.toMicroSeconds();
			if (delay > cycle ){
				if (debug==3)
					std::cout<<"-------- openniServer: RGB openni timeout-" << std::endl;
			}
			else{
				if (delay <1 || delay > cycle)
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
		int segmentation;
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
		std::cout << "Camera name not configured" << std::endl;

	cameraDescription->shortDescription = prop->getProperty(prefix+"ShortDescription");

	//fill imageDescription
	imageDescription->width = depthVideoMode.getResolutionX();
	int playerdetection = prop->getPropertyAsIntWithDefault(prefix+"PlayerDetection",0);
	#ifndef WITH_NITE2
		playerdetection=0;
	#endif

	imageDescription->height = depthVideoMode.getResolutionY();
	int fps = prop->getPropertyAsIntWithDefault(prefix+"fps",5);
	//we use formats acording to colorspaces
	std::string fmtStr = prop->getPropertyWithDefault(prefix+"Format","YUY2");//default format YUY2
	imageFmt = colorspaces::Image::Format::searchFormat(fmtStr);
	if (!imageFmt)
		std::cout << "Format " <<  fmtStr << " unknown" << std::endl;
	imageDescription->size = imageDescription->width * imageDescription->height * CV_ELEM_SIZE(imageFmt->cvType);
	imageDescription->format = imageFmt->name;

	std::cout << "Starting thread for camera: " <<  cameraDescription->name << std::endl;
	replyTask = new ReplyTask(this, imageDescription->width, imageDescription->height,fps, playerdetection);

	this->control=replyTask->start();//my own thread
	}

	virtual ~CameraDEPTH(){
		std::cout << "Stopping and joining thread for camera: " << cameraDescription->name << std::endl;

		replyTask->destroy();
		this->control.join();
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
		std::cout << "Should be made anything to start camera streaming: " << cameraDescription->name << std::endl;
		return std::string("");
	}

	virtual void stopCameraStreaming(const Ice::Current&) {
		std::cout << "Should be made anything to stop camera streaming: "  << cameraDescription->name << std::endl;
	}
	virtual void reset(const Ice::Current&)
	{
	}
	
	virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr&, const Ice::Current&){
		return 0;
	}

private:
	class ReplyTask: public IceUtil::Thread{
	public:
		ReplyTask(CameraDEPTH* camera, int width, int height, int fps, int playerDetection)
	:mycameradepth(camera),_done(false) {
		segmentation=playerDetection;
		this->fps=fps;
		this->minToTrain=15;
      }
		

	void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
		IceUtil::Mutex::Lock sync(requestsMutex);
		requests.push_back(cb);
	}

    virtual void run(){
		int test;
		

		jderobot::ImageDataPtr reply(new jderobot::ImageData);
		reply->description = mycameradepth->imageDescription;
		reply->pixelData.resize(mycameradepth->imageDescription->width*mycameradepth->imageDescription->height*3);
		cv::Mat dst_resize(cv::Size(mycameradepth->imageDescription->width, mycameradepth->imageDescription->height),CV_8UC3);
		cv::Mat src(cv::Size(mycameradepth->imageDescription->width, mycameradepth->imageDescription->height),CV_8UC3);
		int cycle; // duración del ciclo
		IceUtil::Time lastIT;


		//std::cout << "FPS depth: " << fps << std::endl;
		cycle=(float)(1/(float)fps)*1000000;

		lastIT=IceUtil::Time::now();
		while(!(_done)){
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
			const nite::UserId* pLabels;
			if (segmentation){
				const nite::UserMap& userLabels = userTrackerFrame.getUserMap();
				pLabels= userLabels.getPixels();
			}
			#endif



			const openni::DepthPixel* pDepth = (const openni::DepthPixel*)m_depthFrame.getData();
			int restOfRow = m_depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel) - m_depthFrame.getWidth();

			for (int y = 0; y < m_depthFrame.getHeight(); ++y)
			{	
				for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pDepth)
				{
					switch(segmentation){
						case 0:
							distances[(y*m_depthFrame.getWidth() + x)] = *pDepth;
							if (*pDepth != 0)
							{
								src.data[(y*m_depthFrame.getWidth()+ x)*3+0] = (float(*pDepth)/(float)MAX_LENGHT)*255.;
								src.data[(y*m_depthFrame.getWidth()+ x)*3+1] = (*pDepth)>>8;
								src.data[(y*m_depthFrame.getWidth()+ x)*3+2] = (*pDepth)&0xff;
							}
							break;
						case 1:
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
							#endif
							break;
						case 2:
							break;
						default:
							std::cout << "openniServer: Error segmentation not supported" << std::endl;
							break;
					}
				}

				pDepth += restOfRow;
			}
			if (debug==2){
				cv::imshow("OpenniServer DEPTH", src);
				cv::waitKey(1);
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
			

			int delay = IceUtil::Time::now().toMicroSeconds() - lastIT.toMicroSeconds();
			if (delay > cycle ){
				if (debug==3)
					std::cout<<"-------- openniServer: WARNING- DEPTH timeout-" << std::endl;
			}
			else{
				if (delay <1 || delay > cycle)
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

	
		int segmentation;
		int fps;
		int minToTrain;
		cv::BackgroundSubtractorMOG2 bg;
		cv::Mat fore;
		cv::Mat trainImage;
		bool _done;
	
	
    };
    typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;


    std::string prefix;
    colorspaces::Image::FormatPtr imageFmt;
    jderobot::ImageDescriptionPtr imageDescription;
    jderobot::CameraDescriptionPtr cameraDescription;
    ReplyTaskPtr replyTask;
    IceUtil::ThreadControl control;

  };

/**
* \brief Class wich contains all the functions and variables to serve point cloud interface
*/

	class pointCloudI: virtual public jderobot::pointCloud{
		public:
			pointCloudI (std::string& propertyPrefix, const Ice::PropertiesPtr propIn):
				prefix(propertyPrefix),data(new jderobot::pointCloudData()) {
					Ice::PropertiesPtr prop = propIn;

					int playerdetection = prop->getPropertyAsIntWithDefault("openniServer.PlayerDetection",0);
					int fps =prop->getPropertyAsIntWithDefault("openniServer.pointCloud.Fps",10);
					bool extra =(bool)prop->getPropertyAsIntWithDefault("openniServer.ExtraCalibration",0);
					#ifndef WITH_NITE2
						playerdetection=0;
					#endif
						pthread_mutex_init(&this->localMutex, NULL);
					   replyCloud = new ReplyCloud(this,prop->getProperty("openniServer.calibration"), playerdetection, depthVideoMode.getResolutionX(), depthVideoMode.getResolutionY(),fps, extra);
					   this->control=replyCloud->start();
				}

			virtual ~pointCloudI(){
				std::cout << "Stopping and joining thread for pointCloud" << std::endl;
				replyCloud->destroy();
				this->control.join();
			}
		

		virtual jderobot::pointCloudDataPtr getCloudData(const Ice::Current&){
				data=replyCloud->getCloud();
				return data;
			};
		   
		   private:
			 class ReplyCloud :public IceUtil::Thread{
		       public: 
		       	ReplyCloud (pointCloudI* pcloud, std::string filepath,  int playerDetection, int widthIn, int heightIn, int fpsIn, bool extra) : data(new jderobot::pointCloudData()), data2(new jderobot::pointCloudData()), _done(false)
		        	{
					path=filepath;
					segmentation=playerDetection;
					cWidth = widthIn;
					cHeight = heightIn;
					fps=fpsIn;
					myCloud=pcloud;
					mypro=NULL;
					withExtraCalibration=extra;
				}
		       
		        void run()
		        {
		        	mypro= new openniServer::myprogeo(1,cWidth,cHeight);
		        	mypro->load_cam((char*)path.c_str(),0, cWidth, cHeight,withExtraCalibration );
				



				int cycle; // duración del ciclo


				cycle=(float)(1/(float)fps)*1000000;
				IceUtil::Time lastIT=IceUtil::Time::now();
				while(!(_done)){
					float distance;
					pthread_mutex_lock(&mutex);
					//creamos una copia local de la imagen de color y de las distancias.
					cv::Mat localRGB;
					if (srcRGB->rows != 0)
						srcRGB->copyTo(localRGB);
					std::vector<int> localDistance(distances);
					pthread_mutex_unlock(&mutex);
					pthread_mutex_lock(&(this->myCloud->localMutex));
					data2->p.clear();
					for( unsigned int i = 0 ; (i < cWidth*cHeight)&&(distances.size()>0); i=i+9) {
							distance=(float)localDistance[i];
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


									if (withExtraCalibration){
										mypro->applyExtraCalibration(&auxP.x, &auxP.y, &auxP.z);
									}

									if ( segmentation){
										auxP.id=pixelsID[i];
									}
									if (srcRGB->rows != 0){
										auxP.r=(float)(int) (unsigned char)localRGB.data[3*i];
										auxP.g=(float)(int) (unsigned char)localRGB.data[3*i+1];
										auxP.b=(float)(int) (unsigned char)localRGB.data[3*i+2];
									}
									data2->p.push_back(auxP);
								}
							//}
						}
					pthread_mutex_unlock(&(this->myCloud->localMutex));

					int delay = IceUtil::Time::now().toMicroSeconds() - lastIT.toMicroSeconds();
					if (delay > cycle ){
						if (debug==3)
							std::cout<<"-------- openniServer: POINTCLOUD openni timeout-" << std::endl;
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
		          return data;
		       }

		    virtual void destroy(){
				this->_done=true;
			}



		   private:
		       myprogeo *mypro;
				int cWidth;
				int cHeight;
				int fps;
				jderobot::pointCloudDataPtr data, data2;
				jderobot::RGBPoint auxP;
				std::string path;
				int segmentation;
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
openniServer::CameraRGB *camRGB;
openniServer::CameraDEPTH *camDEPTH;
openniServer::pointCloudI *pc1;

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
	ic->shutdown();



	pthread_join(updateThread, NULL);



	m_device.close();
	openni::OpenNI::shutdown();
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
			return 1;
	}
	catch (const char* msg) {
			std::cerr <<"Error :" << msg << std::endl;
			return 1;
	}
	std::string componentPrefix("openniServer");

	cameraR = prop->getPropertyAsIntWithDefault(componentPrefix + ".CameraRGB",0);
	cameraD = prop->getPropertyAsIntWithDefault(componentPrefix + ".CameraDEPTH",0);
	ImageRegistration = prop->getPropertyAsIntWithDefault(componentPrefix + ".ImageRegistration",1);
	int motors = prop->getPropertyAsIntWithDefault(componentPrefix + ".Pose3DMotorsActive",0);
	int leds = prop->getPropertyAsIntWithDefault(componentPrefix + ".KinectLedsActive",0);
	int pointCloud = prop->getPropertyAsIntWithDefault(componentPrefix + ".pointCloudActive",0);
	openniServer::segmentationType= prop->getPropertyAsIntWithDefault(componentPrefix + ".PlayerDetection",0);
	debug = prop->getPropertyAsIntWithDefault(componentPrefix + ".Debug",0);
	mirrorDepth = prop->getPropertyAsIntWithDefault(componentPrefix + ".CameraDEPTH.Mirror",0);
	mirrorRGB = prop->getPropertyAsIntWithDefault(componentPrefix + ".CameraRGB.Mirror",0);
	deviceMode=prop->getPropertyAsIntWithDefault(componentPrefix + ".Mode", 0);
	std::string Endpoints = prop->getProperty(componentPrefix + ".Endpoints");
	Ice::ObjectAdapterPtr adapter =ic->createObjectAdapterWithEndpoints(componentPrefix, Endpoints);



	if (openniServer::segmentationType){
		cameraR=1;
		cameraD=1;
	}

	SELCAM = prop->getPropertyAsIntWithDefault(componentPrefix + ".deviceId",0);
	std::cout << "OpenniServer: Selected device: " << SELCAM << std::endl;
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
	pthread_mutex_init(&mutex, NULL);
	if ((nCameras>0)||(pointCloud)){
		pthread_create(&updateThread, NULL, &openniServer::updateThread, NULL);
	}


	//bloqueo hasta que se inicialice el dispositivo
	IceUtil::Mutex::Lock sync(controlMutex);
	sem.wait(sync);

	sync.release();


	if (cameraR){
		std::string objPrefix(componentPrefix + ".CameraRGB.");
		std::string cameraName = prop->getProperty(objPrefix + "Name");
		if (cameraName.size() == 0){//no name specified, we create one using the index
			cameraName = "cameraR";
			prop->setProperty(objPrefix + "Name",cameraName);//set the value
			}
		std::cout << "Creating camera " << cameraName << std::endl;
		camRGB = new openniServer::CameraRGB(objPrefix,prop);
		adapter->add(camRGB, ic->stringToIdentity(cameraName));
		std::cout<<"              -------- openniServer: Component: CameraRGB created successfully(" << Endpoints << "@" << cameraName << std::endl;
	}
	if (cameraD){
		std::string objPrefix(componentPrefix + ".CameraDEPTH.");
		std::string cameraName = prop->getProperty(objPrefix + "Name");
		if (cameraName.size() == 0){//no name specified, we create one using the index
			cameraName = "cameraD";
			prop->setProperty(objPrefix + "Name",cameraName);//set the value
			}
		std::cout << "Creating camera " <<  cameraName << std::endl;
		camDEPTH = new openniServer::CameraDEPTH(objPrefix,prop);
		adapter->add(camDEPTH, ic->stringToIdentity(cameraName));
		//test camera ok
		std::cout<<"              -------- openniServer: Component: CameraDEPTH created successfully(" << Endpoints << "@" << cameraName << std::endl;
	}
	if (pointCloud){
		std::string objPrefix(componentPrefix + ".PointCloud.");
		std::string Name = prop->getProperty(objPrefix + "Name");
		std::cout << "Creating pointcloud1 " << Name << std::endl;
		pc1 = new openniServer::pointCloudI(objPrefix,prop);
		adapter->add(pc1 , ic->stringToIdentity(Name));
		std::cout<<"              -------- openniServer: Component: PointCloud created successfully(" << Endpoints << "@" << Name << std::endl;
	}
	adapter->activate();
	ic->waitForShutdown();
	adapter->destroy();

	if (!killed)
		exitApplication(0);
	return 0;

}
