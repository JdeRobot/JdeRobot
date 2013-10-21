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

/** \file naobody.cpp
 * \brief naobody component main file
 */

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <gbxsickacfr/gbxiceutilacfr/safethread.h>
#include <jderobot/camera.h>
#include <colorspaces/colorspacesmm.h>
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <tr1/memory>
#include <list>
#include <sstream>
#include <jderobotice/exceptions.h>
#include <jderobot/motors.h>
#include <jderobot/ptencoders.h>
#include <jderobot/ptmotors.h>
#include <jderobot/bodyencoders.h>
#include <jderobot/bodymotors.h>
#include <math.h>
#include "adapters/cameraAdapter.h"
#include "adapters/motorsAdapter.h"
#include "adapters/ptencodersAdapter.h"
#include "adapters/ptmotorsAdapter.h"
#include "adapters/bodyencodersAdapter.h"
#include "adapters/bodymotorsAdapter.h"


namespace naooperator{
/**
* \brief Class wich contains all the functions and variables to make run the Robot Cameras
*/
	class CameraI: virtual public jderobot::Camera {
public:
	CameraI(std::string& propertyPrefix, const jderobotice::Context& context)
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
	std::string robotIP = prop->getProperty("naobody.robot.ip");
	int robotPORT = prop->getPropertyAsInt("naobody.robot.port");
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
	replyTask = new ReplyTask(this, imageDescription->width, imageDescription->height,fps,robotIP,robotPORT);

	replyTask->start();//my own thread
	}

	virtual ~CameraI(){
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

private:
	class ReplyTask: public gbxiceutilacfr::SafeThread{
	public:
		ReplyTask(CameraI* camera, int width, int height, int fps, std::string ip, int port )
	: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycamera(camera) {

		naocamera= new NaoAdapter::NaoCamera(width,height,fps,(char*)ip.c_str(),port);
		naocamera->init();
		img=(char*)malloc(mycamera->imageDescription->width*mycamera->imageDescription->height*3*sizeof(char));
      }
		

	void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
		IceUtil::Mutex::Lock sync(requestsMutex);
		requests.push_back(cb);
	}

    virtual void walk(){
		jderobot::ImageDataPtr reply(new jderobot::ImageData);
		reply->description = mycamera->imageDescription;
	
		while(!isStopping()){
		    IceUtil::Time t = IceUtil::Time::now();
		    reply->timeStamp.seconds = (long)t.toSeconds();
		    reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
			naocamera->updateImage();
			naocamera->getImage((unsigned char *)img);
		    reply->pixelData.resize(mycamera->imageDescription->width*mycamera->imageDescription->height*3*3);
	
		    memmove( &(reply->pixelData[0]),(unsigned char*) img, mycamera->imageDescription->size);
		    
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
	
	CameraI* mycamera;
	IceUtil::Mutex requestsMutex;
	std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
	
	NaoAdapter::NaoCamera *naocamera;
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
* \brief Class wich contains all the functions and variables to controle the Robot locomotion
*/
class MotorsI: virtual public jderobot::Motors {
	public:
		MotorsI(std::string& propertyPrefix, const jderobotice::Context& context)
       : prefix(propertyPrefix),context(context)
	{
		Ice::PropertiesPtr prop = context.properties();
		std::string robotIP = prop->getProperty("naobody.robot.ip");
		int robotPORT = prop->getPropertyAsInt("naobody.robot.port");
		naomotion=new NaoAdapter::motion((char*)robotIP.c_str(),robotPORT);
		naomotion->init();

	}

	virtual ~MotorsI(){};

	virtual float getV(const Ice::Current&){
		return naomotion->getv();	
	};

	virtual float getW(const Ice::Current&){
		return naomotion->getw();
	};
	virtual float getL(const Ice::Current&){
		return naomotion->getl();
	};
	virtual  Ice::Int setV(Ice::Float v, const Ice::Current&){
		std::cout << "cambia el valor de v: " << v << std::endl;
		return naomotion->walk(v,naomotion->getw(),naomotion->getl());
	};
	virtual  Ice::Int setW(Ice::Float w, const Ice::Current&){
		std::cout << "cambia el valor de w: " << w << std::endl;
		return naomotion->walk(naomotion->getv(),w,naomotion->getl());
	};
	virtual  Ice::Int setL(Ice::Float l, const Ice::Current&){
		return naomotion->walk(naomotion->getv(),naomotion->getw(),l);
	};

         std::string prefix;
         jderobotice::Context context;

		NaoAdapter::motion* naomotion;

    };

/**
* \brief Class wich contains all the functions and variables to controle the PTencoders module
*/
class PTEncodersI: virtual public jderobot::PTEncoders {
	public:
		PTEncodersI(std::string& propertyPrefix, const jderobotice::Context& context): prefix(propertyPrefix),context(context), ptEncodersData(new jderobot::PTEncodersData())
		{
			Ice::PropertiesPtr prop = context.properties();
			std::string robotIP = prop->getProperty("naobody.robot.ip");
			int robotPORT = prop->getPropertyAsInt("naobody.robot.port");
			naoPTEncoders=new NaoAdapter::ptencoders((char*)robotIP.c_str(),robotPORT);
			naoPTEncoders->init();
			local_clock=-1;
			local_pan=-1;
			local_tilt=-1;
     	}

		virtual ~PTEncodersI(){};

		virtual jderobot::PTEncodersDataPtr getPTEncodersData(const Ice::Current&){
			std::vector<float> odometry;
			
			ptEncodersData->panAngle=naoPTEncoders->getPan();
			ptEncodersData->tiltAngle=naoPTEncoders->getTilt();
			//std::cout << "valores de los motores: " << ptEncodersData->panAngle << ", " << ptEncodersData->tiltAngle << std::endl;
	
			/* if something to update change clock value */
			if ((local_pan != ptEncodersData->panAngle ) || (local_tilt != ptEncodersData->tiltAngle)){
				local_pan=ptEncodersData->panAngle;
				local_tilt=ptEncodersData->tiltAngle;
				if (local_clock > 9999999)
					local_clock=0;
				else
					local_clock++;
				ptEncodersData->clock=local_clock;
			}
			else{
			}
			return ptEncodersData; 		
		};

		std::string prefix;
		jderobotice::Context context;
		jderobot::PTEncodersDataPtr ptEncodersData;
		NaoAdapter::ptencoders* naoPTEncoders;
		float local_pan, local_tilt;
		float local_clock;
    };

/**
* \brief Class wich contains all the functions and variables to controle the PTmotors module
*/
class PTmotorsI: virtual public jderobot::PTMotors {
	public:
		PTmotorsI(std::string& propertyPrefix, const jderobotice::Context& context): prefix(propertyPrefix),context(context),ptMotorsData(new jderobot::PTMotorsData()), ptMotorsParams(new jderobot::PTMotorsParams())
		{
			Ice::PropertiesPtr prop = context.properties();
			std::string robotIP = prop->getProperty("naobody.robot.ip");
			int robotPORT = prop->getPropertyAsInt("naobody.robot.port");
			naoPTMotors=new NaoAdapter::ptmotors((char*)robotIP.c_str(),robotPORT);
			naoPTMotors->init(ptMotorsParams);
			ptMotorsData->longitude=0;
			ptMotorsData->latitude=0;
			ptMotorsData->longitudeSpeed=0;
			ptMotorsData->latitudeSpeed=0;
     	}

		virtual ~PTmotorsI(){};

		virtual  Ice::Int setPTMotorsData(const jderobot::PTMotorsDataPtr& p, const Ice::Current&){
			return naoPTMotors->changeHeadPosition(p);	
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
		NaoAdapter::ptmotors* naoPTMotors;
    };

/**
* \brief Class wich contains all the functions and variables to controle the BodyEncoders module
*/
class BodyEncodersI: virtual public jderobot::BodyEncoders {
	public:
		BodyEncodersI(std::string& propertyPrefix, const jderobotice::Context& context): prefix(propertyPrefix),context(context),leftArmEncodersData(new jderobot::ArmEncodersData()),leftLegEncodersData(new jderobot::LegEncodersData()),rightArmEncodersData(new jderobot::ArmEncodersData()),rightLegEncodersData(new jderobot::LegEncodersData()), odometryData(new jderobot::OdometryData())
		{
			Ice::PropertiesPtr prop = context.properties();
			std::string robotIP = prop->getProperty("naobody.robot.ip");
			int robotPORT = prop->getPropertyAsInt("naobody.robot.port");
			naoBodyEncoders=new NaoAdapter::bodyencoders((char*)robotIP.c_str(),robotPORT);
			naoBodyEncoders->init();
			odometryData->odometry.resize(9);
     	}

		virtual ~BodyEncodersI(){};

		virtual jderobot::ArmEncodersDataPtr getArmEncodersData (jderobot::BodySide side, const Ice::Current&){
			if (side==jderobot::Left){
				naoBodyEncoders->getArmPosition(leftArmEncodersData, jderobot::Left);
				return leftArmEncodersData;
			}
			else{
				naoBodyEncoders->getArmPosition(rightArmEncodersData, jderobot::Right);
				return rightArmEncodersData;
			}
			
		};

		virtual jderobot::LegEncodersDataPtr getLegEncodersData (jderobot::BodySide side, const Ice::Current&){
			if (side==jderobot::Left){
				naoBodyEncoders->getLegPosition(leftLegEncodersData, jderobot::Left);
				return leftLegEncodersData;
			}
			else{
				naoBodyEncoders->getLegPosition(rightLegEncodersData, jderobot::Right);
				return rightLegEncodersData;
			}
		};
	
		virtual jderobot::OdometryDataPtr getOdometryData (jderobot::CameraBody camera, const Ice::Current&){
			odometryData->odometry=naoBodyEncoders->getOdometry(camera);
			return odometryData;
		};

	private:
		std::string prefix;
		jderobotice::Context context;
		jderobot::ArmEncodersDataPtr leftArmEncodersData;
		jderobot::LegEncodersDataPtr leftLegEncodersData;
		jderobot::ArmEncodersDataPtr rightArmEncodersData;
		jderobot::LegEncodersDataPtr rightLegEncodersData;
		jderobot::OdometryDataPtr odometryData;
		NaoAdapter::bodyencoders* naoBodyEncoders;
		
    };

/**
* \brief Class wich contains all the functions and variables to controle the BodyMotors module
*/
class BodyMotorsI: virtual public jderobot::BodyMotors {
	public:
		BodyMotorsI(std::string& propertyPrefix, const jderobotice::Context& context): prefix(propertyPrefix),context(context),motorsParam(new jderobot::BodyMotorsParam())
		{
			Ice::PropertiesPtr prop = context.properties();
			std::string robotIP = prop->getProperty("naobody.robot.ip");
			int robotPORT = prop->getPropertyAsInt("naobody.robot.port");
			naoBodyMotors=new NaoAdapter::bodymotors((char*)robotIP.c_str(),robotPORT);
			naoBodyMotors->init();
     	}

		virtual ~BodyMotorsI(){};

		virtual jderobot::BodyMotorsParamPtr getBodyMotorsParam(jderobot::MotorsName name, jderobot::BodySide side, const Ice::Current&){
			naoBodyMotors->getMotorLimit(name,side, motorsParam);
			return motorsParam;
		};

		virtual Ice::Int setBodyMotorsData(jderobot::MotorsName name, jderobot::BodySide side, Ice::Float angle, Ice::Float speed, const Ice::Current&){
			return naoBodyMotors->setMotorPosition(name,side,(float)angle, (float)speed);
		};


	private:
		std::string prefix;
		jderobotice::Context context;
		jderobot::BodyMotorsParamPtr motorsParam;
		NaoAdapter::bodymotors* naoBodyMotors;
		
    };


/**
* \brief Main Class of the component wich create the diferents devices activated using the Ice configuration file.
*/
class Component: public jderobotice::Component{
public:
	Component()
	:jderobotice::Component("naobody"), cameras(0), motors1(0), ptmotors1(0), ptencoders1(0), bodyencoders1(0), bodymotors1(0)  {}

	virtual void start(){
		Ice::PropertiesPtr prop = context().properties();
		int nCameras = prop->getPropertyAsIntWithDefault(context().tag() + ".NCameras",0);

		cameras.resize(nCameras);
		for (int i=0; i<nCameras; i++){//build camera objects
			std::stringstream objIdS;
			objIdS <<  i;
			std::string objId = objIdS.str();// should this be something unique??
			std::string objPrefix(context().tag() + ".Camera." + objId + ".");
			std::string cameraName = prop->getProperty(objPrefix + "Name");
			if (cameraName.size() == 0){//no name specified, we create one using the index
				cameraName = "camera" + objId;
				prop->setProperty(objPrefix + "Name",cameraName);//set the value
				}
			context().tracer().info("Creating camera " + cameraName);
			cameras[i] = new CameraI(objPrefix,context());
			context().createInterfaceWithString(cameras[i],cameraName);
			//test camera ok
			std::cout<<"              -------- NaoBody: Component: Cameras created successfully   --------" << std::endl;
		}
		if (prop->getPropertyAsIntWithDefault(context().tag() + ".MotorsActive",0)){
			std::string objPrefix2="motors1";
			std::string motorsName = "motors1";
			context().tracer().info("Creating motors " + motorsName);
			motors1 = new MotorsI(objPrefix2,context());
			context().createInterfaceWithString(motors1,motorsName);
			std::cout<<"              -------- NaoBody: Component: Motors created successfully   --------" << std::endl;
		}
		if (prop->getPropertyAsIntWithDefault(context().tag() + ".PTEncodersActive",0)){
			std::string objPrefix3="ptencoders1";
			std::string ptencodersName = "ptencoders1";
			context().tracer().info("Creating ptencoders1 " + ptencodersName);
			ptencoders1 = new PTEncodersI(objPrefix3,context());
			context().createInterfaceWithString(ptencoders1,ptencodersName);
			std::cout<<"              -------- NaoBody: Component: PTEncoders created successfully   --------" << std::endl;
		}
		if (prop->getPropertyAsIntWithDefault(context().tag() + ".PTMotorsActive",0)){
			std::string objPrefix4="ptmotors1";
			std::string ptmotorsName = "ptmotors1";
			context().tracer().info("Creating ptmotors1 " + ptmotorsName);
			ptmotors1 = new PTmotorsI(objPrefix4,context());
			context().createInterfaceWithString(ptmotors1,ptmotorsName);
			std::cout<<"              -------- NaoBody: Component: PTMotors created successfully   --------" << std::endl;
		}
		if (prop->getPropertyAsIntWithDefault(context().tag() + ".BodyEncodersActive",0)){
			std::string objPrefix5="bodyencoders1";
			std::string BodyEncodersName = "bodyencoders1";
			context().tracer().info("Creating BodyEncoders1 " + BodyEncodersName);
			bodyencoders1 = new BodyEncodersI(objPrefix5,context());
			context().createInterfaceWithString(bodyencoders1,BodyEncodersName);
			std::cout<<"              -------- NaoBody: Component: BodyEncoders created successfully   --------" << std::endl;
		}
		if (prop->getPropertyAsIntWithDefault(context().tag() + ".BodyMotorsActive",0)){
			std::string objPrefix6="bodymotors1";
			std::string BodyMotorsName = "bodymotors1";
			context().tracer().info("Creating BodyMotors1 " + BodyMotorsName);
			bodymotors1 = new BodyMotorsI(objPrefix6,context());
			context().createInterfaceWithString(bodymotors1,BodyMotorsName);
			std::cout<<"              -------- NaoBody: Component: BodyMotors created successfully   --------" << std::endl;
		}
    }

    virtual ~Component(){
    }

  private:
    std::vector<Ice::ObjectPtr> cameras;
    Ice::ObjectPtr motors1;
    Ice::ObjectPtr ptmotors1;
    Ice::ObjectPtr ptencoders1;
    Ice::ObjectPtr bodyencoders1;
	Ice::ObjectPtr bodymotors1;
  };

} //namespace

int main(int argc, char** argv){

  naooperator::Component component;

  jderobotice::Application app(component);

  return app.jderobotMain(argc,argv);

}
