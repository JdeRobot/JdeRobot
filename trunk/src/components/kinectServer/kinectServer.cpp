/*
 *  Copyright (C) 1997-2010 JDE Developers Team
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
 *            Francisco Miguel Rivas Montero <fm.rivas@alumnos.urjc.es>	
 */

/** \file kinectServer.cpp
 * \brief kinectServer component main file
 */

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <gbxsickacfr/gbxiceutilacfr/safethread.h>
#include <jderobot/kinect.h>
#include <jderobot/kinectleds.h>
#include <jderobot/ptmotors.h>
#include <colorspaces/colorspacesmm.h>
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <tr1/memory>
#include <list>
#include <sstream>
#include <jderobotice/exceptions.h>
#include <math.h>
//#include <controller.h>
#include <cv.h>
#include <highgui.h>
#include <XnOS.h>
#include <XnCppWrapper.h>


namespace jdernect{
/**
* \brief Class wich contains all the functions and variables to make run the Robot Cameras
*/
	class CameraRGB: virtual public jderobot::Kinect {
public:
	CameraRGB(xn::Context *g_context,std::string& propertyPrefix, const jderobotice::Context& context)
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
	imageDescription->width = prop->getPropertyAsIntWithDefault(prefix+"ImageWidth",340);
	imageDescription->height = prop->getPropertyAsIntWithDefault(prefix+"ImageHeight",280);
	int fps = prop->getPropertyAsIntWithDefault(prefix+"fps",5);
	//we use formats acording to colorspaces
	std::string fmtStr = prop->getPropertyWithDefault(prefix+"Format","YUY2");//default format YUY2
	imageFmt = colorspaces::Image::Format::searchFormat(fmtStr);
	if (!imageFmt)
	throw 
		jderobotice::ConfigFileException(ERROR_INFO, "Format " + fmtStr + " unknown");
	imageDescription->size = imageDescription->width * imageDescription->height * CV_ELEM_SIZE(imageFmt->cvType);
	imageDescription->format = imageFmt->name;

	camera_selected=0;
	context.tracer().info("Starting thread for camera: " + cameraDescription->name);
	replyTask = new ReplyTask(&camera_selected,g_context,this, imageDescription->width, imageDescription->height,fps);

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
	}

	virtual void stopCameraStreaming(const Ice::Current&) {
		context.tracer().info("Should be made anything to stop camera streaming: " + cameraDescription->name);
	}

	virtual Ice::Int changeCamera(jderobot::KinectView v, const Ice::Current&) {
		if ((v == jderobot::ImageRGB) || (v==jderobot::ImageIR)){
			view=v;
			return 0;
		}
		else
			return -1;
	}
	virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr&, const Ice::Current&){
		
	}

private:
	class ReplyTask: public gbxiceutilacfr::SafeThread{
	public:
		ReplyTask(int* s,xn::Context *g_context,CameraRGB* camera, int width, int height, int fps)
	: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycameravga(camera) {
		context=g_context;
		camera_selected=s;
      }
		

	void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
		IceUtil::Mutex::Lock sync(requestsMutex);
		requests.push_back(cb);
	}

    virtual void walk(){
		jderobot::ImageDataPtr reply(new jderobot::ImageData);
		reply->description = mycameravga->imageDescription;
		reply->pixelData.resize(mycameravga->imageDescription->width*mycameravga->imageDescription->height*3);
		rgb.resize(640*480*3);
		IplImage* src = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);
		IplImage* dst_resize = cvCreateImage(cvSize(mycameravga->imageDescription->width,mycameravga->imageDescription->height), IPL_DEPTH_8U, 3);
		XnStatus rc;
		
		rc = context->FindExistingNode(XN_NODE_TYPE_IMAGE, g_image);
		g_image.GetMetaData(g_imageMD);
		g_nTexMapX = (((unsigned short)(g_imageMD.FullXRes()-1) / 512) + 1) * 512;
		g_nTexMapY = (((unsigned short)(g_imageMD.FullYRes()-1) / 512) + 1) * 512;
		g_pTexMap = (XnRGB24Pixel*)malloc(g_nTexMapX * g_nTexMapY * sizeof(XnRGB24Pixel));
		
		
	
		while(!isStopping()){
		    IceUtil::Time t = IceUtil::Time::now();
		    reply->timeStamp.seconds = (long)t.toSeconds();
		    reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;

			//test con nite
			//std::cout << "ante de captura el metadata" << std::endl;
			
			//std::cout << "despues de la captura" << std::endl;
			

			//memcpy(src->imageData,(unsigned char *)g_imageMD.Data(),640*480*3);
			rc = context->WaitAnyUpdateAll();
			if (1){
			//std::cout << "overlay image" << std::endl;
			const XnRGB24Pixel* pImageRow = g_imageMD.RGB24Data();
			XnRGB24Pixel* pTexRow = g_pTexMap + g_imageMD.YOffset() * g_nTexMapX;
	
			for (XnUInt y = 0; y < g_imageMD.YRes(); ++y)
			{
				const XnRGB24Pixel* pImage = pImageRow;
				XnRGB24Pixel* pTex = pTexRow + g_imageMD.XOffset();
	
				for (XnUInt x = 0; x < g_imageMD.XRes(); ++x, ++pImage, ++pTex)
				{
					src->imageData[(y*g_imageMD.XRes() + x)*3 + 0] = pImage->nRed;
					src->imageData[(y*g_imageMD.XRes() + x)*3 + 1] = pImage->nGreen;
					src->imageData[(y*g_imageMD.XRes() + x)*3 + 2] = pImage->nBlue;
				}
				pImageRow += g_imageMD.XRes();
				pTexRow += g_nTexMapX;
			}
			}		
		
			cvFlip(src, NULL, 1);

			if ((mycameravga->imageDescription->width != 640) || (mycameravga->imageDescription->height != 480)){
				cvResize(src,dst_resize);
				memmove(&(reply->pixelData[0]),(unsigned char *) dst_resize->imageData,dst_resize->width*dst_resize->height * 3);
			}
			else{
				memmove(&(reply->pixelData[0]),(unsigned char *) src->imageData,src->width*src->height * 3);
			}



			
		    {//critical region start
			IceUtil::Mutex::Lock sync(requestsMutex);
		    while(!requests.empty()){
				jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front();
				requests.pop_front();
				cb->ice_response(reply);
			}
			}//critical region end
		}
	}
	
	CameraRGB* mycameravga;
	IceUtil::Mutex requestsMutex;
	std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
	
	std::vector<uint8_t> rgb;
	xn::Context *context;
	int* camera_selected;
	char *img; 
	xn::ImageGenerator g_image;
	xn::ImageMetaData g_imageMD;
	XnRGB24Pixel* g_pTexMap;
	unsigned int g_nTexMapX,g_nTexMapY;
	
    };
    typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;


    std::string prefix;
    jderobotice::Context context;
    colorspaces::Image::FormatPtr imageFmt;
    jderobot::ImageDescriptionPtr imageDescription;
    jderobot::CameraDescriptionPtr cameraDescription;
    ReplyTaskPtr replyTask;
	int camera_selected;
	jderobot::KinectView view;


  };


//*********************************************************************
	class CameraDEPTH: virtual public jderobot::Kinect {
public:
	CameraDEPTH(xn::Context *g_context,std::string& propertyPrefix, const jderobotice::Context& context)
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
	imageDescription->width = prop->getPropertyAsIntWithDefault(prefix+"ImageWidth",340);
	imageDescription->height = prop->getPropertyAsIntWithDefault(prefix+"ImageHeight",280);
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
	replyTask = new ReplyTask(g_context,this, imageDescription->width, imageDescription->height,fps, &view);

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
	}

	virtual void stopCameraStreaming(const Ice::Current&) {
		context.tracer().info("Should be made anything to stop camera streaming: " + cameraDescription->name);
	}
	
	virtual Ice::Int changeCamera(jderobot::KinectView v, const Ice::Current&) {
		if ((v == jderobot::DepthRaw) || (v==jderobot::DepthImageView)){
			view=v;
			return 0;
		}
		else
			return -1;
	}
	virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr&, const Ice::Current&){
		
	}

private:
	class ReplyTask: public gbxiceutilacfr::SafeThread{
	public:
		ReplyTask(xn::Context *g_context,CameraDEPTH* camera, int width, int height, int fps, jderobot::KinectView* v)
	: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycameravga(camera) {

		context = g_context;
		vw=v;
      }
		

	void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
		IceUtil::Mutex::Lock sync(requestsMutex);
		requests.push_back(cb);
	}

    virtual void walk(){
		jderobot::ImageDataPtr reply(new jderobot::ImageData);
		reply->description = mycameravga->imageDescription;
		reply->pixelData.resize(mycameravga->imageDescription->width*mycameravga->imageDescription->height*3);
		IplImage* src = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);
		IplImage* dst_resize = cvCreateImage(cvSize(mycameravga->imageDescription->width,mycameravga->imageDescription->height), IPL_DEPTH_8U, 3);
		XnStatus rc;
		
		rc = context->FindExistingNode(XN_NODE_TYPE_DEPTH, g_depth);
		rc = context->FindExistingNode(XN_NODE_TYPE_IMAGE, g_image);
		
		g_nTexMapX = (((unsigned short)(g_depthMD.FullXRes()-1) / 512) + 1) * 512;
		g_nTexMapY = (((unsigned short)(g_depthMD.FullYRes()-1) / 512) + 1) * 512;
		g_pTexMap = (XnRGB24Pixel*)malloc(g_nTexMapX * g_nTexMapY * sizeof(XnRGB24Pixel));
		g_depth.GetAlternativeViewPointCap().SetViewPoint(g_image);
		g_depth.GetMetaData(g_depthMD);
		
		
	
		while(!isStopping()){
			rc = context->WaitAnyUpdateAll();
			if (*vw==jderobot::DepthRaw){
				g_depth.GetAlternativeViewPointCap().SetViewPoint(g_image);	
			}
			else{
				g_depth.GetAlternativeViewPointCap().ResetViewPoint();
			}

		    IceUtil::Time t = IceUtil::Time::now();
		    reply->timeStamp.seconds = (long)t.toSeconds();
		    reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
			//const XnDepthPixel* pDepth = (XnDepthPixel*) g_depthMD.Data();
			xnOSMemSet(g_pTexMap, 0, g_nTexMapX*g_nTexMapY*sizeof(XnRGB24Pixel));
			const XnDepthPixel* pDepthRow =(XnDepthPixel*) g_depthMD.Data();
			XnRGB24Pixel* pTexRow = g_pTexMap + g_depthMD.YOffset() * g_nTexMapX;
			cvZero(src);
			for (XnUInt y = 0; y < g_depthMD.YRes(); ++y)
			{
				const XnDepthPixel* pDepth = pDepthRow;
				XnRGB24Pixel* pTex = pTexRow + g_depthMD.XOffset();

				
				
				for (XnUInt x = 0; x < g_depthMD.XRes(); ++x, ++pDepth, ++pTex)
				{
					if (*pDepth != 0)
					{
        					int pval = (int)*pDepth/10; 
						
						int lb = pval & 0xff;
						switch (pval>>8) {
						case 0:
							src->imageData[(y*g_depthMD.XRes() + x)*3+0] = 255;
							src->imageData[(y*g_depthMD.XRes() + x)*3+1] = 255-lb;
							src->imageData[(y*g_depthMD.XRes() + x)*3+2] = 255-lb;
							break;
						case 1:
							src->imageData[(y*g_depthMD.XRes() + x)*3+0] = 255;
							src->imageData[(y*g_depthMD.XRes() + x)*3+1] = lb;
							src->imageData[(y*g_depthMD.XRes() + x)*3+2] = 0;
							break;
						case 2:
							src->imageData[(y*g_depthMD.XRes() + x)*3+0] = 255-lb;
							src->imageData[(y*g_depthMD.XRes() + x)*3+1] = 255;
							src->imageData[(y*g_depthMD.XRes() + x)*3+2] = 0;
							break;
						case 3:
							src->imageData[(y*g_depthMD.XRes() + x)*3+0] = 0;
							src->imageData[(y*g_depthMD.XRes() + x)*3+1] = 255;
							src->imageData[(y*g_depthMD.XRes() + x)*3+2] = lb;
							break;
						case 4:
							src->imageData[(y*g_depthMD.XRes() + x)*3+0] = 0;
							src->imageData[(y*g_depthMD.XRes() + x)*3+1] = 255-lb;
							src->imageData[(y*g_depthMD.XRes() + x)*3+2] = 255;
							break;
						case 5:
							src->imageData[(y*g_depthMD.XRes() + x)*3+0] = 0;
							src->imageData[(y*g_depthMD.XRes() + x)*3+1] = 0;
							src->imageData[(y*g_depthMD.XRes() + x)*3+2] = 255-lb;
							break;
						default:
							src->imageData[(y*g_depthMD.XRes() + x)*3+0] = 0;
							src->imageData[(y*g_depthMD.XRes() + x)*3+1] = 0;
							src->imageData[(y*g_depthMD.XRes() + x)*3+2] = 0;
							break;
						}
				
					}
				}
	
				pDepthRow += g_depthMD.XRes();
				pTexRow += g_nTexMapX;
			}
			cvFlip(src, NULL, 1);
		   if ((mycameravga->imageDescription->width != 640) || (mycameravga->imageDescription->height != 480)){
				cvResize(src,dst_resize);
				memmove(&(reply->pixelData[0]),(unsigned char *) dst_resize->imageData,dst_resize->width*dst_resize->height * 3);
			}
			else{
				memmove(&(reply->pixelData[0]),(unsigned char *) src->imageData,src->width*src->height * 3);
			}
		    
		    {//critical region start
			IceUtil::Mutex::Lock sync(requestsMutex);
		    while(!requests.empty()){
				jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front();
				requests.pop_front();
				cb->ice_response(reply);
			}
			}//critical region end
		}
	}
	
	CameraDEPTH* mycameravga;
	IceUtil::Mutex requestsMutex;
	std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
	
	xn::Context *context;
	
	char *img; 
	xn::ImageGenerator g_depth;
	xn::ImageMetaData g_depthMD;
	xn::ImageGenerator g_image;
	xn::ImageMetaData g_imageMD;
	XnRGB24Pixel* g_pTexMap;
	unsigned int g_nTexMapX,g_nTexMapY;	
	float g_pDepthHist[100000];
	jderobot::KinectView* vw;
	
	
    };
    typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;


    std::string prefix;
    jderobotice::Context context;
    colorspaces::Image::FormatPtr imageFmt;
    jderobot::ImageDescriptionPtr imageDescription;
    jderobot::CameraDescriptionPtr cameraDescription;
    ReplyTaskPtr replyTask;
	jderobot::KinectView view;

  };


/**
* \brief Class wich contains all the functions and variables to controle the PTmotors module
*/
class PTmotorsI: virtual public jderobot::PTMotors {
	public:
		PTmotorsI(xn::Context *g_context, std::string& propertyPrefix, const jderobotice::Context& context): prefix(propertyPrefix),context(context),ptMotorsData(new jderobot::PTMotorsData()), ptMotorsParams(new jderobot::PTMotorsParams())
		{
			Ice::PropertiesPtr prop = context.properties();
			ptMotorsData->longitude=0;
			ptMotorsData->latitude=0;
			ptMotorsData->longitudeSpeed=0;
			ptMotorsData->latitudeSpeed=0;
			contextKinect=g_context;
			//device->setTiltDegrees(0);
     	}

		virtual ~PTmotorsI(){};

		virtual  Ice::Int setPTMotorsData(const jderobot::PTMotorsDataPtr& p, const Ice::Current&){
			ptMotorsData=p;
			//device->setTiltDegrees(p->latitude);
		};
	
		virtual jderobot::PTMotorsParamsPtr getPTMotorsParams(const Ice::Current&){
			return ptMotorsParams;
		};

		virtual jderobot::PTMotorsDataPtr getPTMotorsData (const Ice::Current&){
			return ptMotorsData;
		};

	private:
		std::string prefix;
		jderobotice::Context context;
		jderobot::PTMotorsDataPtr ptMotorsData;
		jderobot::PTMotorsParamsPtr ptMotorsParams;
		xn::Context *contextKinect;
    };

/**
* \brief Class wich contains all the functions and variables to controle the KinectLeds module
*/
class KinectLedsI: virtual public jderobot::KinectLeds {
	public:
		KinectLedsI(xn::Context *g_context, std::string& propertyPrefix, const jderobotice::Context& context): prefix(propertyPrefix),context(context)
		{
			Ice::PropertiesPtr prop = context.properties();
			contextKinect=g_context;
			//device->setLed(LED_GREEN);
     	}

		virtual ~KinectLedsI(){};

		virtual  void setLedActive(jderobot::KinectLedsAvailable led, const Ice::Current&){
			/*if (led==jderobot::OFF)
				device->setLed(LED_OFF);
			if (led==jderobot::GREEN)
				device->setLed(LED_GREEN);
			if (led==jderobot::RED)
				device->setLed(LED_RED);
			if (led==jderobot::YELLOW)
				device->setLed(LED_YELLOW);
			if (led==jderobot::BLINKGREEN)
				device->setLed(LED_BLINK_GREEN);
			if (led==jderobot::BLINKREDYELLOW)
				device->setLed(LED_BLINK_RED_YELLOW);*/
		}

	private:
		std::string prefix;
		jderobotice::Context context;
		xn::Context *contextKinect;
    };

/**
* \brief Main Class of the component wich create the diferents devices activated using the Ice configuration file.
*/
class Component: public jderobotice::Component{
public:
	Component()
	:jderobotice::Component("kinectServer"), cameras(0) {}

	virtual void start(){
		Ice::PropertiesPtr prop = context().properties();
		int cameraR = prop->getPropertyAsIntWithDefault(context().tag() + ".CameraRGB",0);
		int cameraD = prop->getPropertyAsIntWithDefault(context().tag() + ".CameraDEPTH",0);
		int nCameras;

		nCameras=cameraR + cameraD;
		g_context =  new xn::Context;
		std::cout << "NCAMERAS = " << nCameras << std::endl;
		cameras.resize(nCameras);
		if (nCameras>0){
			XnStatus rc;
			//xn::Context myc;

			xn::EnumerationErrors errors;
			rc = g_context->InitFromXmlFile("Sample-User.xml", &errors);
			if (rc == XN_STATUS_NO_NODE_PRESENT)
			{
				std::cout << "ERROR" << std::endl;
				XnChar strError[2048];
				errors.ToString(strError, 2048);
				printf("%s\n", strError);
				//return (rc);
			}
			else if (rc != XN_STATUS_OK)
			{
				printf("Open failed: %s\n", xnGetStatusString(rc));
				//return (rc);
			}
		}
		if (cameraR){
			std::string objPrefix(context().tag() + ".CameraRGB.");
			std::string cameraName = prop->getProperty(objPrefix + "Name");
			if (cameraName.size() == 0){//no name specified, we create one using the index
				cameraName = "cameraR";
				prop->setProperty(objPrefix + "Name",cameraName);//set the value
				}
			context().tracer().info("Creating camera " + cameraName);
			cameras[0] = new CameraRGB(g_context,objPrefix,context());
			context().createInterfaceWithString(cameras[0],cameraName);
			//test camera ok
			std::cout<<"              -------- kinectServer: Component: CameraRGB created successfully   --------" << std::endl;
		}
		if (cameraD){
			std::string objPrefix(context().tag() + ".CameraDEPTH.");
			std::string cameraName = prop->getProperty(objPrefix + "Name");
			if (cameraName.size() == 0){//no name specified, we create one using the index
				cameraName = "cameraD";
				prop->setProperty(objPrefix + "Name",cameraName);//set the value
				}
			context().tracer().info("Creating camera " + cameraName);
			cameras[1] = new CameraDEPTH(g_context,objPrefix,context());
			context().createInterfaceWithString(cameras[1],cameraName);
			//test camera ok
			std::cout<<"              -------- kinectServer: Component: CameraDEPTH created successfully   --------" << std::endl;
		}
		
		if (prop->getPropertyAsIntWithDefault(context().tag() + ".PTMotorsActive",0)){
			std::string objPrefix4="ptmotors1";
			std::string ptmotorsName = "ptmotors1";
			context().tracer().info("Creating ptmotors1 " + ptmotorsName);
			ptmotors1 = new PTmotorsI(g_context,objPrefix4,context());
			context().createInterfaceWithString(ptmotors1,ptmotorsName);
			std::cout<<"              -------- kinectServer: Component: PTMotors created successfully   --------" << std::endl;
		}
			
		if (prop->getPropertyAsIntWithDefault(context().tag() + ".KinectLedsActive",0)){
			std::string objPrefix4="kinectleds1";
			std::string Name = "kinectleds1";
			context().tracer().info("Creating kinectleds1 " + Name);
			kinectleds1 = new KinectLedsI(g_context,objPrefix4,context());
			context().createInterfaceWithString(kinectleds1,Name);
			std::cout<<"              -------- kinectServer: Component: KinectLeds created successfully   --------" << std::endl;
		}

    }

    virtual ~Component(){
    }

  private:
    std::vector<Ice::ObjectPtr> cameras;
	xn::Context *g_context;
	Ice::ObjectPtr ptmotors1;
	Ice::ObjectPtr kinectleds1;

  };

} //namespace

int main(int argc, char** argv){

  jdernect::Component component;

  jderobotice::Application app(component);

  return app.jderobotMain(argc,argv);

}
