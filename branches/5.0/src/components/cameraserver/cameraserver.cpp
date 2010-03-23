/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
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
 *  Authors : David Lobato Bravo <dav.lobato@gmail.com>
 *
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
#include "gstpipeline.h"
#include <stdlib.h>

namespace cameraserver{
  class CameraI: virtual public jderobot::Camera {
  public:
    CameraI(std::string& propertyPrefix, const jderobotice::Context& context)
      : prefix(propertyPrefix),context(context),pipeline(),
	imageFmt(),
	imageDescription(new jderobot::ImageDescription()),
	cameraDescription(new jderobot::CameraDescription()),
	replyTask()
    {
      
      
      Ice::PropertiesPtr prop = context.properties();

      //fill cameraDescription
      cameraDescription->name = prop->getProperty(prefix+"Name");
      if (cameraDescription->name.size() == 0)
    	  throw jderobotice::ConfigFileException(ERROR_INFO,"Camera name not configured");
      cameraDescription->shortDescription = prop->getProperty(prefix+"ShortDescription");
      cameraDescription->streamingUri = prop->getProperty(prefix+"StreamingUri");

      //fill imageDescription
      imageDescription->width = prop->getPropertyAsIntWithDefault(prefix+"ImageWidth",340);
      imageDescription->height = prop->getPropertyAsIntWithDefault(prefix+"ImageHeight",280);
      //we use formats acording to colorspaces
      std::string fmtStr = prop->getPropertyWithDefault(prefix+"Format","YUY2");//default format YUY2
      imageFmt = colorspaces::Image::Format::searchFormat(fmtStr);
      if (!imageFmt)
	throw jderobotice::ConfigFileException(ERROR_INFO, "Format " + fmtStr + " unknown");
      imageDescription->size = imageDescription->width * imageDescription->height * CV_ELEM_SIZE(imageFmt->cvType);
      imageDescription->format = imageFmt->name;

      //fill pipeline cfg
      pipelineCfg.name = prop->getProperty(prefix+"Name");
      pipelineCfg.srcpipeline = prop->getProperty(prefix+"SrcPipeline");
      pipelineCfg.uri = prop->getProperty(prefix+"Uri");
      pipelineCfg.framerateN = prop->getPropertyAsIntWithDefault(prefix+"FramerateN",25);
      pipelineCfg.framerateD = prop->getPropertyAsIntWithDefault(prefix+"FramerateD",1);
      pipelineCfg.width = imageDescription->width;
      pipelineCfg.height = imageDescription->height;
      pipelineCfg.format = imageFmt;

      //pipelineCfg.validate();FIXME: validate cfg before to use it


      context.tracer().info("Creating pipeline with config: " + pipelineCfg.toString());
      pipeline = new GSTPipeline(context,pipelineCfg);
      context.tracer().info("Starting thread for camera: " + cameraDescription->name);
      replyTask = new ReplyTask(this);
    }

    virtual ~CameraI(){
      context.tracer().info("Stopping pipeline");
      pipeline->stop();
      context.tracer().info("Stopping and joining thread for camera: " + cameraDescription->name);
      gbxiceutilacfr::stopAndJoin(replyTask);
    }
    
    virtual jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c){
      return imageDescription;
    }

    virtual jderobot::CameraDescriptionPtr getCameraDescription(const Ice::Current& c){
      return cameraDescription;
    }

    virtual void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb,
			       const Ice::Current& c){
      replyTask->pushJob(cb);
    }

    virtual std::string startCameraStreaming(const Ice::Current& c)
    {

    	std::string commandVLC = "vlc " + pipelineCfg.uri + " -I dummy --sout \"#transcode{vcodec=mp4v,acodec=aac}:rtp{dst=0.0.0.0,port=1234,sdp=" + cameraDescription->streamingUri + "}\" &";

		// system is blocked, the command should be run in background
    	int ret = system(commandVLC.c_str());

    	if (ret==0)
    	{
    		context.tracer().info("Starting Streaming in " + cameraDescription->name + ": (" + cameraDescription->streamingUri + ")");
    		return cameraDescription->streamingUri;
    	}
    	else
    		return NULL;
    }

    virtual void stopCameraStreaming(const Ice::Current& c)
    {

    	context.tracer().info("Stoping Streaming in " + cameraDescription->name);
    	system ("killall vlc");
    	return;
    }


  private:
    class ReplyTask: public gbxiceutilacfr::SafeThread{
    public:
      ReplyTask(CameraI* camera)
	: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycamera(camera) {
	start();//start thread
      }

      void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
	IceUtil::Mutex::Lock sync(requestsMutex);
	requests.push_back(cb);
      }

      virtual void walk(){
	jderobot::ImageDataPtr reply(new jderobot::ImageData);
	reply->description = mycamera->imageDescription;
	
	while(!isStopping()){
	  GstBuffer* buff = mycamera->pipeline->pull_buffer();
	  if (!buff){
	    mycamera->context.tracer().info("Pipeline return empty buffer. Waiting...");
	    IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(1));
	  }else{
	    IceUtil::Time t = IceUtil::Time::now();
	    reply->timeStamp.seconds = (long)t.toSeconds();
	    reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
	    reply->pixelData.resize(mycamera->imageDescription->size);
	    memmove( &(reply->pixelData[0]), (char*)buff->data, mycamera->imageDescription->size);//copy data to reply
	    gst_buffer_unref(buff);//release gstreamer buffer
	    
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
      }
      CameraI* mycamera;
      IceUtil::Mutex requestsMutex;
      std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
    };
    typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;


    std::string prefix;
    jderobotice::Context context;
    GSTPipelinePtr pipeline;
    Config pipelineCfg;
    colorspaces::Image::FormatPtr imageFmt;
    jderobot::ImageDescriptionPtr imageDescription;
    jderobot::CameraDescriptionPtr cameraDescription;
    ReplyTaskPtr replyTask;
  };


  class Component: public jderobotice::Component{
  public:
    Component()
      :jderobotice::Component("CameraSrv"), cameras(0) {}

    virtual void start(){
      Ice::PropertiesPtr prop = context().properties();
      int nCameras = prop->getPropertyAsInt(context().tag() + ".NCameras");
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
      }
    }

    virtual ~Component(){
    }

  private:
    std::vector<Ice::ObjectPtr> cameras;
  };

} //namespace

int main(int argc, char** argv){
  cameraserver::Component component;

  /*initializes gstreamer*/
  gst_init(&argc,&argv);

  jderobotice::Application app(component);
  return app.jderobotMain(argc,argv);
}
