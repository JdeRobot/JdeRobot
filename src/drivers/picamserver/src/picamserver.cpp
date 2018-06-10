#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceStorm/IceStorm.h>
#include <jderobot/camera.h>
#include <jderobot/image.h>


//Opencv
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string.h>
#include <sstream>
#include <cstdio>
#include <csignal>
#include <unistd.h>
#include <cstdlib>
#include <list>
#include <ctime>
#include <fstream>
#include <time.h>
#include "piCamCore.h"

#include <zlib.h>
#include <logger/Logger.h>
#include <jderobotutil/interfaceHandlers/CameraHandler.h>
#include <jderobotutil/interfaceHandlers/CameraTask.h>
#include <ns/ns.h>

#include "easyiceconfig/EasyIce.h"

bool flag=false; /** boolean to keep a check on signal */

namespace cameraserver{

class CameraI:  public jderobot::CameraHandler {
 public:
  CameraI(std::string propertyPrefix, Ice::CommunicatorPtr ic):jderobot::CameraHandler(propertyPrefix,ic){



      replyTask = new ReplyTask(this, prop, prop->getPropertyAsIntWithDefault("piCam.framerate", 20) + 5, piCam);
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


  }
  void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, const std::string& format, const Ice::Current& c){
    replyTask->pushJob(cb, format);
  }



 private:
  class ReplyTask: public jderobot::CameraTask {
   public:

      ReplyTask(jderobot::Camera* camera, Ice::PropertiesPtr prop, int fps, piCam::PiCamCore piC):jderobot::CameraTask(camera,fps), reply_piCam(piC) {

        reply_piCam.setCustomStateParams(prop);
        reply_piCam.open();

      }

      virtual void createCustomImage(cv::Mat& image){
        cv::Mat frame;

        if(!reply_piCam.isOpened()){
          exit(-1);
        }
        if(flag){
          reply_piCam.release();
          exit(-1);
        }

        reply_piCam.grab();

       reply_piCam.retrieve ( frame , piCam::PICAM_FORMAT_BGR);//get camera image


        frame.copyTo(image);
      }

   private:
      //cv::VideoCapture capture;
      piCam::PiCamCore reply_piCam; //Camera object
      cv::Size configSize;

  };
  //cv::VideoCapture capture;
  piCam::PiCamCore piCam; //Camera object

  std::string uri;
  int framerateN;
  int framerateD;
  bool rpc_mode;


  typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;
  ReplyTaskPtr replyTask;
};

} //namespace


jderobot::ns* namingService = NULL;


void signalHandler(int signum){   /*** signal handler to handle SIGINT signal */
   flag=true;
}

int main(int argc, char** argv)
{
  std::vector<Ice::ObjectPtr> cameras;
  signal(SIGINT,signalHandler);
  Ice::CommunicatorPtr ic;
  try{
    ic = EasyIce::initialize(argc, argv);

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
        std::cout << nsActive << '\n';
        if (nsActive)
        {
            std::string ns_proxy = prop->getProperty("NamingService.Proxy");
            try
            {
                namingService = new jderobot::ns(ic, ns_proxy);
            }
            catch (Ice::ConnectionRefusedException& ex)
            {
                LOG(ERROR) << "Impossible to connect with NameService!";
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
    std::cerr << ex<<" 1 " << std::endl;
    exit(-1);
  } catch (const char* msg) {
    std::cerr << msg<< " 2 " << std::endl;
    exit(-1);
  }

}
