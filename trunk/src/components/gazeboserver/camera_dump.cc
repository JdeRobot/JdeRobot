#include "gazebo.hh"
#include "plugins/CameraPlugin.hh"
#include "common/common.h"
#include "transport/transport.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>

// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <gbxsickacfr/gbxiceutilacfr/safethread.h>

// JDErobot general ice component includes
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <jderobotice/exceptions.h>
#include <jderobotice/context.h>

#include <jderobot/camera.h>

#include <colorspaces/colorspacesmm.h>

#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

void *myMain(void* v);

namespace gazebo
{   
  class CameraDump : public CameraPlugin
  { 
    public: CameraDump() : CameraPlugin(),count(0) 
	{
		pthread_mutex_init (&mutex, NULL);
		n = round(rand()*1000);
	}

    public: void Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf)
    {
      // Don't forget to load the camera plugin
      CameraPlugin::Load(_parent,_sdf);
      std::cout << "Load: " <<n << " " <<  this->parentSensor->GetCamera()->GetName()<< std::endl;
    } 

    // Update the controller
    public: void OnNewFrame(const unsigned char *_image, 
        unsigned int _width, unsigned int _height, unsigned int _depth, 
        const std::string &_format)
    {
      //std::cout << "OnNewFrame: " <<n << " " <<  this->parentSensor->GetCamera()->GetName()<< std::endl;
		if(count==0){
			std::vector<std::string> tokens;
			nameCamera = this->parentSensor->GetCamera()->GetName();
  			boost::split(tokens, nameCamera, boost::is_any_of("::"));
  			boost::split(tokens, tokens[2], boost::is_any_of("("));
			nameGlobal = tokens[0];
			// El nombre del fichero de configuracion tiene que coincidir con el de la cámara en el .world y el .cfg 
			nameCamera = std::string("--Ice.Config=" + tokens[0] + ".cfg"); 
			
			if (count == 0){
				pthread_t thr_gui;
				pthread_create(&thr_gui, NULL, &myMain, (void*)this);
			}

			image.create(_height, _width, CV_8UC3);
			count++;
		}
		pthread_mutex_lock (&mutex);
        memcpy((unsigned char *) image.data, &(_image[0]), _width*_height * 3);
		
		//cv::imshow(nameCamera, image);
		//cv::waitKey(10);
					
		pthread_mutex_unlock (&mutex);
		
    }         

    private: int count;
    private: int n;
    public: std::string nameCamera;
    public: cv::Mat image;
	public: pthread_mutex_t mutex;
	public: std::string nameGlobal;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_SENSOR_PLUGIN(CameraDump)
}

class CameraI: virtual public jderobot::Camera {
	public:
		CameraI(std::string propertyPrefix, const jderobotice::Context& context, gazebo::CameraDump* camera)
			   : prefix(propertyPrefix),context(context), cameraI(camera) {
		
			std::cout << "Constructor CameraI" << std::endl;

			imageDescription = (new jderobot::ImageDescription());

        	replyTask = new ReplyTask(this);
		    replyTask->start(); // my own thread
		  
		}

		std::string getName () {
			return (cameraDescription->name);
		}

		std::string getRobotName () {
			return ((context.properties())->getProperty(context.tag()+".RobotName"));
		}

		virtual ~CameraI() {
			context.tracer().info("Stopping and joining thread for camera: " + cameraDescription->name);
			gbxiceutilacfr::stopAndJoin(replyTask);
		}

		virtual jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c){
			return imageDescription;
		}

		virtual jderobot::CameraDescriptionPtr getCameraDescription(const Ice::Current& c){
			return cameraDescription;
		}

		virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr &description, const Ice::Current& c) {
			return 0;
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
		class ReplyTask: public gbxiceutilacfr::SafeThread {
			public:
				ReplyTask(CameraI* camera)
				: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycamera(camera) {
				   	std::cout << "safeThread" << std::endl;
				}

				void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
					IceUtil::Mutex::Lock sync(requestsMutex);
					requests.push_back(cb);
				}

				virtual void walk(){
					jderobot::ImageDataPtr reply(new jderobot::ImageData);
					struct timeval a, b;
					int cycle = 48;
					long totalb,totala;
					long diff;
					
					int count =0 ;

					while(!isStopping()){
						
						if(!mycamera->cameraI->image.data){
							usleep(100);
							continue;
						}
						if(count==0){
							pthread_mutex_lock (&mycamera->cameraI->mutex);
							mycamera->imageDescription->width = mycamera->cameraI->image.cols;
							mycamera->imageDescription->height = mycamera->cameraI->image.rows;
							mycamera->imageDescription->size = mycamera->cameraI->image.cols*mycamera->cameraI->image.rows*3;
							pthread_mutex_unlock (&mycamera->cameraI->mutex);

							mycamera->imageDescription->format = "RGB8";

							reply->description = mycamera->imageDescription;
							count++;
						}

						//std::cout << nameGlobal<< std::endl;
									
						gettimeofday(&a,NULL);
						totala=a.tv_sec*1000000+a.tv_usec;


						IceUtil::Time t = IceUtil::Time::now();
						reply->timeStamp.seconds = (long)t.toSeconds();
						reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
          				
          				pthread_mutex_lock (&mycamera->cameraI->mutex);
					    reply->pixelData.resize(mycamera->cameraI->image.rows*mycamera->cameraI->image.cols*3);
					    
					    memcpy( &(reply->pixelData[0]), (unsigned char *) mycamera->cameraI->image.data, mycamera->cameraI->image.rows*mycamera->cameraI->image.cols*3);
						pthread_mutex_unlock (&mycamera->cameraI->mutex);

					   { //critical region start
						   IceUtil::Mutex::Lock sync(requestsMutex);
						   while(!requests.empty()) {
							   jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front();
							   requests.pop_front();
							   cb->ice_response(reply);
						   }
					   } //critical region end
                  
						gettimeofday(&b,NULL);
						totalb=b.tv_sec*1000000+b.tv_usec;

						diff = (totalb-totala)/1000;
						diff = cycle-diff;

						//std::cout << "Gazeboserver takes " << diff << " ms " << mycamera->fileName << std::endl;

						if(diff < 33)
							diff = 33;


						/*Sleep Algorithm*/
						usleep(diff*1000);
					}
				}

				CameraI* mycamera;
				IceUtil::Mutex requestsMutex;
				std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
		};

		typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;
		std::string prefix;
		jderobotice::Context context;
		colorspaces::Image::FormatPtr imageFmt;
		jderobot::ImageDescriptionPtr imageDescription;
		jderobot::CameraDescriptionPtr cameraDescription;
		ReplyTaskPtr replyTask;
		gazebo::CameraDump* cameraI;	
		
}; // end class CameraI


void *myMain(void* v)
{

	gazebo::CameraDump* camera = (gazebo::CameraDump*)v;

	char* name = (char*)camera->nameCamera.c_str();

    Ice::CommunicatorPtr ic;
    int argc = 1;

    Ice::PropertiesPtr prop;
	char* argv[] = {name};

    try {
        
        ic = Ice::initialize(argc, argv);
        prop = ic->getProperties();
        
        std::string Endpoints = prop->getProperty("CameraGazebo.Endpoints");
        std::cout << "CameraGazebo "<< camera->nameGlobal <<" Endpoints > "  << Endpoints << std::endl;
        
        Ice::ObjectAdapterPtr adapter =
            ic->createObjectAdapterWithEndpoints("CameraGazebo", Endpoints);
            
		jderobotice::Context context;
		
        Ice::ObjectPtr object = new CameraI(std::string("CameraGazebo"), context, camera);
        adapter->add(object, ic->stringToIdentity(camera->nameGlobal));
        adapter->activate();
        ic->waitForShutdown();
    } catch (const Ice::Exception& e) {
        std::cerr << e << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }
    if (ic) {
        try {
            ic->destroy();
        } catch (const Ice::Exception& e) {
            std::cerr << e << std::endl;
        }
    }
    
}
