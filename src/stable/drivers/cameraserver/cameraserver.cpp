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
#include <jderobot/camera.h>
#include <jderobot/image.h>
#include <visionlib/colorspaces/colorspacesmm.h>


//Opencv
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <stdlib.h>
#include <list>

#include <zlib.h>
#include <log/Logger.h>
#include <jderobotutil/CameraHandler.h>
#include <jderobotutil/CameraTask.h>
#include <ns/ns.h>


namespace cameraserver{

class CameraI:  public jderobot::CameraHandler {
 public:
  CameraI(std::string propertyPrefix, Ice::CommunicatorPtr ic):jderobot::CameraHandler(propertyPrefix,ic){
    //we use formats acording to colorspaces
    std::string fmtStr = prop->getPropertyWithDefault(prefix+"Format","YUY2");//default format YUY2
    imageFmt = colorspaces::Image::Format::searchFormat(fmtStr);
    if (!imageFmt)
      throw  "Format " + fmtStr + " unknown";

    imageDescription->size = imageDescription->width * imageDescription->height * CV_ELEM_SIZE(imageFmt->cvType);
    imageDescription->format = imageFmt->name;

    // mirror image
    mirror = prop->getPropertyAsIntWithDefault(prefix+"Mirror",0);

    //fill pipeline cfg
    uri = prop->getProperty(prefix+"Uri");
    framerateN = prop->getPropertyAsIntWithDefault(prefix+"FramerateN",25);
    framerateD = prop->getPropertyAsIntWithDefault(prefix+"FramerateD",1);

    std::cout << "URI: " << uri << std::endl;

    if(uri.size()>3)
      capture.open(uri);
    else
      capture.open(atoi(uri.c_str()));

    if(capture.isOpened()){
      replyTask = new ReplyTask(this, framerateN, capture, cv::Size(imageDescription->width, imageDescription->height));

      // check client/server service mode
      int rpc = prop->getPropertyAsIntWithDefault("CameraSrv.DefaultMode",1);

      if(rpc){
        rpc_mode=true;
      }
      else{
        // check publish/subscribe service mode
        Ice::ObjectPrx obj = ic->propertyToProxy("CameraSrv.TopicManager");

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

      replyTask->start(); // my own thread

    }else{
      exit(-1);
    }
  }
  void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, const std::string& format, const Ice::Current& c){
    replyTask->pushJob(cb, format);
  }


 private:
  class ReplyTask: public jderobot::CameraTask {
   public:
      ReplyTask(const jderobot::Camera* camera, int fps, cv::VideoCapture& cap, cv::Size configSize):jderobot::CameraTask(camera,fps), capture(cap), configSize(configSize){

      }

      virtual void createCustomImage(cv::Mat& image){
        cv::Mat frame;
        if(!capture.isOpened()){
          exit(-1);
        }

        capture >> frame;

        if(!frame.data){
          capture.set(CV_CAP_PROP_POS_AVI_RATIO, 0.0);
          capture >> frame;
        }
        cv::cvtColor(frame, frame, CV_RGB2BGR);

        if(configSize!=frame.size())
          cv::resize(frame, frame,configSize);
        frame.copyTo(image);
      }

   private:
      cv::VideoCapture capture;
      cv::Size configSize;

  };
  cv::VideoCapture capture;

  std::string uri;
  int framerateN;
  int framerateD;
  bool rpc_mode;


  typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;
  ReplyTaskPtr replyTask;
};

} //namespace


jderobot::ns* namingService = NULL;


int main(int argc, char** argv)
{
	std::vector<Ice::ObjectPtr> cameras;

	Ice::CommunicatorPtr ic;
	try{
		ic = Ice::initialize(argc, argv);

		Ice::PropertiesPtr prop = ic->getProperties();



		// check default service mode
		/*int rpc = prop->getPropertyAsIntWithDefault("CameraSrv.DefaultMode",0);

		if(rpc!=0){
			// check publish/subscribe service mode
			Ice::ObjectPrx obj = ic->propertyToProxy("CameraSrv.TopicManager");

			if(obj==0){
				// no service mode configuration
				std::cerr << "Error: cameraserver needs server configuration mode\n" << std::endl;
				fflush(NULL);

				exit(0);
			}
		}*/

		std::string Endpoints = prop->getProperty("CameraSrv.Endpoints");

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

		int nCameras = prop->getPropertyAsInt("CameraSrv.NCameras");
		cameras.resize(nCameras);
		Ice::ObjectAdapterPtr adapter =ic->createObjectAdapterWithEndpoints("CameraServer", Endpoints);
		for (int i=0; i<nCameras; i++){//build camera objects
			std::stringstream objIdS;
			objIdS <<  i;
			std::string objId = objIdS.str();// should this be something unique??
			std::string objPrefix("CameraSrv.Camera." + objId + ".");
			std::string cameraName = prop->getProperty(objPrefix + "Name");
			Ice::ObjectPtr object = new cameraserver::CameraI(objPrefix, ic);

			adapter->add(object, ic->stringToIdentity(cameraName));

            if (namingService)
                namingService->bind(cameraName, Endpoints, object->ice_staticId());

		}
		adapter->activate();
		ic->waitForShutdown();

	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		exit(-1);
	} catch (const char* msg) {
		std::cerr << msg << std::endl;
		exit(-1);
	}

}
