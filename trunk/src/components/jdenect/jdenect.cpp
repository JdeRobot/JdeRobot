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
 *            Eduardo Perdices <eperdes@gsyc.es>
 *            Francisco Miguel Rivas Montero <fm.rivas@alumnos.urjc.es>	
 */

/** \file jdenect.cpp
 * \brief jdenect component main file
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
#include <controller.h>
#include <cv.h>
#include <highgui.h>


namespace jdernect{
/**
* \brief Class wich contains all the functions and variables to make run the Robot Cameras
*/
	class CameraRGB: virtual public jderobot::Kinect {
public:
	CameraRGB(MyFreenectDevice* d,std::string& propertyPrefix, const jderobotice::Context& context)
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
	replyTask = new ReplyTask(&camera_selected,d,this, imageDescription->width, imageDescription->height,fps);

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

	virtual Ice::Int changeCamera(const Ice::Current&) {
		if (camera_selected==0)
			camera_selected=1;
		else
			camera_selected=0;
		return camera_selected;
	}
	virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr&, const Ice::Current&){
		
	}

private:
	class ReplyTask: public gbxiceutilacfr::SafeThread{
	public:
		ReplyTask(int* s,MyFreenectDevice* d,CameraRGB* camera, int width, int height, int fps)
	: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycameravga(camera) {
		device =d;
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
		IplImage* src = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 1);
		IplImage* dst = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);
		IplImage* dst_resize = cvCreateImage(cvSize(mycameravga->imageDescription->width,mycameravga->imageDescription->height), IPL_DEPTH_8U, 3);
		
		
	
		while(!isStopping()){
		    IceUtil::Time t = IceUtil::Time::now();
		    reply->timeStamp.seconds = (long)t.toSeconds();
		    reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
			if ((*camera_selected)==0){
				device->getRGB(rgb);
				if ((mycameravga->imageDescription->width != 640) || (mycameravga->imageDescription->height != 480)){
					memcpy(dst->imageData,(unsigned char *)&rgb[0],640*480*3);
					cvResize(dst,dst_resize);
					memmove(&(reply->pixelData[0]),(unsigned char *) dst_resize->imageData,dst_resize->width*dst_resize->height * 3);
				}
				else{
					reply->pixelData=rgb;
				}
			}
			else{
				device->getIR(rgb);
				memcpy(src->imageData,(unsigned char *)&rgb[0],640*480);
				cvCvtColor(src,dst, CV_GRAY2RGB);
				if ((mycameravga->imageDescription->width != 640) || (mycameravga->imageDescription->height != 480)){
					cvResize(dst,dst_resize);
					memmove(&(reply->pixelData[0]),(unsigned char *) dst_resize->imageData,dst_resize->width*dst_resize->height * 3);
				}
				else{
					memmove(&(reply->pixelData[0]),(unsigned char *) dst->imageData,dst->width*dst->height * 3);
				}
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
	MyFreenectDevice* device;
	int* camera_selected;
	char *img; 
	
    };
    typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;


    std::string prefix;
    jderobotice::Context context;
    colorspaces::Image::FormatPtr imageFmt;
    jderobot::ImageDescriptionPtr imageDescription;
    jderobot::CameraDescriptionPtr cameraDescription;
    ReplyTaskPtr replyTask;
	int camera_selected;


  };


//*********************************************************************
	class CameraDEPTH: virtual public jderobot::Kinect {
public:
	CameraDEPTH(MyFreenectDevice* d,std::string& propertyPrefix, const jderobotice::Context& context)
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
	replyTask = new ReplyTask(d,this, imageDescription->width, imageDescription->height,fps);

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
	
	virtual Ice::Int changeCamera(const Ice::Current&) {
		return -1;
	}
	virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr&, const Ice::Current&){
		
	}

private:
	class ReplyTask: public gbxiceutilacfr::SafeThread{
	public:
		ReplyTask(MyFreenectDevice* d,CameraDEPTH* camera, int width, int height, int fps)
	: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycameravga(camera) {

		device = d;
      }
		

	void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
		IceUtil::Mutex::Lock sync(requestsMutex);
		requests.push_back(cb);
	}

    virtual void walk(){
		jderobot::ImageDataPtr reply(new jderobot::ImageData);
		reply->description = mycameravga->imageDescription;
		reply->pixelData.resize(mycameravga->imageDescription->width*mycameravga->imageDescription->height*3);
		depth.resize(640*480*3);
		IplImage* dst = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);
		IplImage* dst_resize = cvCreateImage(cvSize(mycameravga->imageDescription->width,mycameravga->imageDescription->height), IPL_DEPTH_8U, 3);
	
		while(!isStopping()){
		    IceUtil::Time t = IceUtil::Time::now();
		    reply->timeStamp.seconds = (long)t.toSeconds();
		    reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
			device->getDepth(depth);
		    if ((mycameravga->imageDescription->width != 640) || (mycameravga->imageDescription->height != 480)){
				memcpy(dst->imageData,(unsigned char *)&depth[0],640*480*3);
				cvResize(dst,dst_resize);
				memmove(&(reply->pixelData[0]),(unsigned char *) dst_resize->imageData,dst_resize->width*dst_resize->height * 3);
			}
			else{
				reply->pixelData=depth;
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
	
	MyFreenectDevice* device;
	std::vector<uint8_t> depth;
	
	char *img; 
	
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
* \brief Class wich contains all the functions and variables to controle the PTmotors module
*/
class PTmotorsI: virtual public jderobot::PTMotors {
	public:
		PTmotorsI(MyFreenectDevice* d, std::string& propertyPrefix, const jderobotice::Context& context): prefix(propertyPrefix),context(context),ptMotorsData(new jderobot::PTMotorsData()), ptMotorsParams(new jderobot::PTMotorsParams())
		{
			Ice::PropertiesPtr prop = context.properties();
			ptMotorsData->longitude=0;
			ptMotorsData->latitude=0;
			ptMotorsData->longitudeSpeed=0;
			ptMotorsData->latitudeSpeed=0;
			device=d;
			device->setTiltDegrees(0);
     	}

		virtual ~PTmotorsI(){};

		virtual  Ice::Int setPTMotorsData(const jderobot::PTMotorsDataPtr& p, const Ice::Current&){
			ptMotorsData=p;
			device->setTiltDegrees(p->latitude);
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
		MyFreenectDevice* device;
    };

/**
* \brief Class wich contains all the functions and variables to controle the KinectLeds module
*/
class KinectLedsI: virtual public jderobot::KinectLeds {
	public:
		KinectLedsI(MyFreenectDevice* d, std::string& propertyPrefix, const jderobotice::Context& context): prefix(propertyPrefix),context(context)
		{
			Ice::PropertiesPtr prop = context.properties();
			device=d;
			device->setLed(LED_GREEN);
     	}

		virtual ~KinectLedsI(){};

		virtual  void setLedActive(jderobot::KinectLedsAvailable led, const Ice::Current&){
			if (led==jderobot::OFF)
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
				device->setLed(LED_BLINK_RED_YELLOW);
		}

	private:
		std::string prefix;
		jderobotice::Context context;
		MyFreenectDevice* device;
    };

/**
* \brief Main Class of the component wich create the diferents devices activated using the Ice configuration file.
*/
class Component: public jderobotice::Component{
public:
	Component()
	:jderobotice::Component("jdenect"), cameras(0) {}

	virtual void start(){
		Ice::PropertiesPtr prop = context().properties();
		int cameraR = prop->getPropertyAsIntWithDefault(context().tag() + ".CameraRGB",0);
		int cameraD = prop->getPropertyAsIntWithDefault(context().tag() + ".CameraDEPTH",0);
		int nCameras;

		nCameras=cameraR + cameraD;
		std::cout << "NCAMERAS = " << nCameras << std::endl;
		cameras.resize(nCameras);
		if (nCameras>1){
			device = &freenect.createDevice<MyFreenectDevice>(0);
			device->startVideo();
			device->startDepth();
		}
		if (cameraR){
			std::string objPrefix(context().tag() + ".CameraRGB.");
			std::cout << "------------" << objPrefix << std::endl;
			std::string cameraName = prop->getProperty(objPrefix + "Name");
			if (cameraName.size() == 0){//no name specified, we create one using the index
				cameraName = "cameraR";
				prop->setProperty(objPrefix + "Name",cameraName);//set the value
				}
			context().tracer().info("Creating camera " + cameraName);
			cameras[0] = new CameraRGB(device,objPrefix,context());
			context().createInterfaceWithString(cameras[0],cameraName);
			//test camera ok
			std::cout<<"              -------- jdenect: Component: CameraRGB created successfully   --------" << std::endl;
		}
		if (cameraD){
			std::string objPrefix(context().tag() + ".CameraDEPTH.");
			std::string cameraName = prop->getProperty(objPrefix + "Name");
			if (cameraName.size() == 0){//no name specified, we create one using the index
				cameraName = "cameraD";
				prop->setProperty(objPrefix + "Name",cameraName);//set the value
				}
			context().tracer().info("Creating camera " + cameraName);
			cameras[1] = new CameraDEPTH(device,objPrefix,context());
			context().createInterfaceWithString(cameras[1],cameraName);
			//test camera ok
			std::cout<<"              -------- jdenect: Component: CameraDEPTH created successfully   --------" << std::endl;
		}
		
		if (prop->getPropertyAsIntWithDefault(context().tag() + ".PTMotorsActive",0)){
			std::string objPrefix4="ptmotors1";
			std::string ptmotorsName = "ptmotors1";
			context().tracer().info("Creating ptmotors1 " + ptmotorsName);
			ptmotors1 = new PTmotorsI(device,objPrefix4,context());
			context().createInterfaceWithString(ptmotors1,ptmotorsName);
			std::cout<<"              -------- jdenect: Component: PTMotors created successfully   --------" << std::endl;
		}
			
		if (prop->getPropertyAsIntWithDefault(context().tag() + ".KinectLedsActive",0)){
			std::string objPrefix4="kinectleds1";
			std::string Name = "kinectleds1";
			context().tracer().info("Creating kinectleds1 " + Name);
			kinectleds1 = new KinectLedsI(device,objPrefix4,context());
			context().createInterfaceWithString(kinectleds1,Name);
			std::cout<<"              -------- jdenect: Component: KinectLeds created successfully   --------" << std::endl;
		}

    }

    virtual ~Component(){
    }

  private:
    std::vector<Ice::ObjectPtr> cameras;
	MyFreenectDevice* device;
	Freenect::Freenect freenect;
	Ice::ObjectPtr ptmotors1;
	Ice::ObjectPtr kinectleds1;

  };

} //namespace

int main(int argc, char** argv){

  jdernect::Component component;

  jderobotice::Application app(component);

  return app.jderobotMain(argc,argv);

}
