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
 *	      Sara Marugán Alonso <smarugan@gsyc.es>
 *
 */


#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceStorm/IceStorm.h>
#include <gbxsickacfr/gbxiceutilacfr/safethread.h>
#include <jderobot/camera.h>
#include <jderobot/image.h>
#include <colorspaces/colorspacesmm.h>
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <tr1/memory>
#include <list>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include "gstpipeline.h"
#include <stdlib.h>
#include <inttypes.h>
#include <libraw1394/raw1394.h>
#include <dc1394/control.h>

namespace cameraserver{
  class CameraI: virtual public jderobot::Camera {
  public:
    CameraI(std::string& propertyPrefix, const jderobotice::Context& context)
      : prefix(propertyPrefix),context(context),pipeline(),
	imageFmt(),
	imageDescription(new jderobot::ImageDescription()),
	cameraDescription(new jderobot::CameraDescription()),
	imageConsumer(),
	replyTask(),
	rpc_mode(false),
	camera1394(),
	firewire_mode(false)
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
			if ((prop->getProperty(prefix+"Invert") == "True"))
				pipelineCfg.invert = true;
			else
				pipelineCfg.invert = false;

      //pipelineCfg.validate();FIXME: validate cfg before to use it

      if(strncmp((char*)pipelineCfg.uri.c_str(),"dv",2)==0){
	firewire_mode=true;
	context.tracer().info("Creating firewire thread with config: " + pipelineCfg.toString());
      }
      else{
      	context.tracer().info("Creating pipeline with config: " + pipelineCfg.toString());
      	resetPipeline(pipelineCfg);
      }
      
      context.tracer().info("Starting thread for camera: " + cameraDescription->name);
      replyTask = new ReplyTask(this);


      // check client/server service mode
      int rpc = prop->getPropertyAsIntWithDefault("CameraSrv.DefaultMode",0);

      if(rpc!=0){
	rpc_mode=true;
      }

      // check publish/subscribe service mode
      Ice::ObjectPrx obj = context.communicator()->propertyToProxy("CameraSrv.TopicManager");

      if(obj!=0){
    	      // IceStorm publisher initialization
	      IceStorm::TopicManagerPrx topicManager = IceStorm::TopicManagerPrx::checkedCast(obj);
	      IceStorm::TopicPrx topic;
	      try{
		topic = topicManager->retrieve(cameraDescription->name);
	      }
	      catch(const IceStorm::NoSuchTopic&){
		topic = topicManager->create(cameraDescription->name);
	      }
	      Ice::ObjectPrx pub = topic->getPublisher()->ice_oneway();

	      imageConsumer=jderobot::ImageConsumerPrx::uncheckedCast(pub);
      }
      else{
	      imageConsumer=0;	
      }

    }

    virtual ~CameraI(){
      context.tracer().info("Stopping pipeline");
      if(firewire_mode){
	cleanup(camera1394);
      }
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

    virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr &description, const Ice::Current& c){
      return 0;
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

    void resetPipeline(const Config &cfg){
      pipeline = new GSTPipeline(context,cfg);
    }

  private:
	/** cleans up firewire structures and frees the firewire bus.*/
	int cleanup(dc1394camera_t *camera) {
	    dc1394_video_set_transmission(camera, DC1394_OFF);
	    dc1394_capture_stop(camera);
	    return 0;
	}

	/** firewire driver init function. It will start all firewire required devices
	 *  and setting them the default configuration.
	 *  @return 0 if initialitation was successful or -1 if something went wrong.*/
	void firewire_deviceinit(){
		const int NUM_BUFFERS=1;
		dc1394_t * d;  
		dc1394error_t err;
        	dc1394video_mode_t res;
        	dc1394framerate_t f;
		dc1394camera_list_t *list;
		char device[128];
		strncpy(device,pipelineCfg.uri.c_str(),pipelineCfg.uri.length());
		device[pipelineCfg.uri.length()]='\0';

		// camera config
		int width=imageDescription->width;
		int fps=pipelineCfg.framerateN;
		int camera_id=atoi(&device[strlen(device)-1]);
	   
		list=NULL;
		d = dc1394_new ();
		err=dc1394_camera_enumerate (d, &list);

		if (list == NULL) {
			perror("Can't access firewire device\n");
		}
	  
		camera1394 = dc1394_camera_new (d, list->ids[camera_id].guid);
		if (!camera1394){
			dc1394_log_warning("Failed to initialize camera with id %d",camera_id);
		}

		if(width==320){
			res = DC1394_VIDEO_MODE_320x240_YUV422;
		}
		else{
		// This resolution is only compatible with framerate 15
			res = DC1394_VIDEO_MODE_640x480_YUV422;
		}


		if(fps==30){
			f = DC1394_FRAMERATE_30;
		}
		else if(fps==15){
			f = DC1394_FRAMERATE_15;
		}
		else{
			f = DC1394_FRAMERATE_15;
			fprintf(stderr,"Wrong framerate for camera with id %d, set to default 15 fps\n",camera_id);
		}

		err=dc1394_video_set_iso_speed(camera1394, DC1394_ISO_SPEED_400);
		//DC1394_ERR_CLN_RTN(err,cleanup(camera),"Could not set ISO speed");

		err=dc1394_video_set_mode(camera1394, res);
		//DC1394_ERR_CLN_RTN(err,cleanup(camera),"Could not set video mode");

		err=dc1394_video_set_framerate(camera1394, f);
		//DC1394_ERR_CLN_RTN(err,cleanup(camera),"Could not set framerate");

		err=dc1394_capture_setup(camera1394,NUM_BUFFERS, DC1394_CAPTURE_FLAGS_DEFAULT);
		//DC1394_ERR_CLN_RTN(err,cleanup(camera),"Could not setup camera-\nmake sure that the video mode and framerate are\nsupported by your camera");

		err=dc1394_video_set_transmission(camera1394, DC1394_ON);
		//DC1394_ERR_CLN_RTN(err,cleanup(camera),"Could not start camera iso transmission");

	    	dc1394_camera_free_list (list);
	}

	/** function to transform a buffer from uyvy to rgb.
	 *  @param src source buffer.
	 *  @param dest destination buffer where the transformation will be set.
	 *  @param NumPixels how many pixels per buffer.*/
	void uyvy2rgb (unsigned char *src, unsigned char *dest, unsigned long long int NumPixels)
	{
	/**
	 * Color conversion functions from Bart Nabbe.
	 * Corrected by Damien: bad coeficients in YUV2RGB.
	 */
	#define YUV2RGB(y, u, v, r, g, b)		\
	  r = y + ((v*1436) >> 10);			\
	  g = y - ((u*352 + v*731) >> 10);		\
	  b = y + ((u*1814) >> 10);			\
	  r = r < 0 ? 0 : r;				\
	  g = g < 0 ? 0 : g;				\
	  b = b < 0 ? 0 : b;				\
	  r = r > 255 ? 255 : r;			\
	  g = g > 255 ? 255 : g;			\
	  b = b > 255 ? 255 : b

	  register int i = (NumPixels << 1)-1;
	  register int j = NumPixels + ( NumPixels << 1 ) -1;
	  register int y0, y1, u, v;
	  register int r, g, b;

	  while (i > 0) {
	    y1 = (unsigned char) src[i--];
	    v  = (unsigned char) src[i--] - 128;
	    y0 = (unsigned char) src[i--];
	    u  = (unsigned char) src[i--] - 128;
	    YUV2RGB (y1, u, v, b, g, r);
	    dest[j--] = r;
	    dest[j--] = g;
	    dest[j--] = b;
	    YUV2RGB (y0, u, v, b, g, r);
	    dest[j--] = r;
	    dest[j--] = g;
	    dest[j--] = b;
	  }
	}

	void invertImage (char *src, unsigned char *dest, int width, int heigth) {
		int i, j;
		int posI, posJ;
		char temp[3];

		for (i=0, j=(width*heigth)-1; i<j; i++, j--) {
			posI = i*3;
			posJ = j*3;
			temp[0] = (unsigned char) src[posI];
			temp[1] = (unsigned char) src[posI+1];
			temp[2] = (unsigned char) src[posI+2];
			dest[posI]=(unsigned char) src[posJ];
			dest[posI+1]=(unsigned char) src[posJ+1];
			dest[posI+2]=(unsigned char) src[posJ+2];
			dest[posJ]=temp[0];
			dest[posJ+1]=temp[1];
			dest[posJ+2]=temp[2];
		}
	}

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

		if(mycamera->firewire_mode){
			mycamera->firewire_deviceinit();
		}
	
		bool new_frame;
		GstBuffer* buff;
		dc1394video_frame_t *frame;
		dc1394camera_t *camera=mycamera->camera1394;

		while(!isStopping()){
		  new_frame=true;
		  if(mycamera->firewire_mode){
			if (dc1394_capture_dequeue(camera, DC1394_CAPTURE_POLICY_WAIT, &frame)!=DC1394_SUCCESS){
				dc1394_log_error("Failed to capture from firewire camera");  
				new_frame=false;  
			}
		  }
		  else{
		  	buff = mycamera->pipeline->pull_buffer();
			if(!buff){
			  //mycamera->context.tracer().info("Pipeline return empty buffer.");
			  new_frame=false;
			  IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(100));
			  if (mycamera->pipeline->isEos()){
			    mycamera->context.tracer().info("Pipeline is eos.Restarting pipeline...");
			    mycamera->resetPipeline(mycamera->pipelineCfg);
			  }
			}
		  }

		  if (new_frame){
		    	IceUtil::Time t = IceUtil::Time::now();
		    	reply->timeStamp.seconds = (long)t.toSeconds();
		    	reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
		    	reply->pixelData.resize(mycamera->imageDescription->width*mycamera->imageDescription->height*3);

		    	if(mycamera->firewire_mode){
					  dc1394_capture_enqueue (camera, frame);

					  if (mycamera->imageFmt == colorspaces::ImageRGB8::FORMAT_RGB8){
							mycamera->uyvy2rgb((unsigned char*)frame->image,&(reply->pixelData[0]),frame->size[0]*frame->size[1]);
					  }
					  else{
					  // Format colorspaces::ImageRGB8::FORMAT_YUYV
							memmove( &(reply->pixelData[0]),frame->image, mycamera->imageDescription->size);
					  }
		    	}
		    	else{
							if (mycamera->pipelineCfg.invert) {
								mycamera->invertImage ((char*)buff->data, &(reply->pixelData[0]), mycamera->imageDescription->width, mycamera->imageDescription->height);
							} else {  
								memmove(&(reply->pixelData[0]), buff->data, mycamera->imageDescription->size);//copy data to reply
							}
		    	    gst_buffer_unref(buff);//release gstreamer buffer
		    	}

		    	// publish
		    	if(mycamera->imageConsumer!=0){
			 	   mycamera->imageConsumer->report(reply);
		    	}

		    	// response to data petition
		    	if(mycamera->rpc_mode){
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
	    jderobot::ImageConsumerPrx imageConsumer;
	    dc1394camera_t *camera1394;
	    bool rpc_mode;
	    bool firewire_mode;
  };


  class Component: public jderobotice::Component{
  public:
    Component()
      :jderobotice::Component("CameraSrv"), cameras(0) {}

    virtual void start(){
      Ice::PropertiesPtr prop = context().properties();

      // check default service mode
      int rpc = prop->getPropertyAsIntWithDefault("CameraSrv.DefaultMode",0);

      if(rpc==0){
	// check publish/subscribe service mode
	Ice::ObjectPrx obj = context().communicator()->propertyToProxy("CameraSrv.TopicManager");

	if(obj==0){
		// no service mode configuration
		std::cerr << "Error: cameraserver needs server configuration mode\n" << std::endl;
		fflush(NULL);

		exit(0);
	}
      }

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
