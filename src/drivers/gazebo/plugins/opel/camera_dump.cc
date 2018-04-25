#include "gazebo.hh"
#include "plugins/CameraPlugin.hh"
#include "common/common.hh"
#include "transport/transport.hh"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <easyiceconfig/EasyIce.h>

#include <jderobot/camera.h>

#include <visionlib/colorspaces/colorspacesmm.h>

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
      std::cout << "Load: " <<n << " " <<  this->parentSensor->Camera()->Name()<< std::endl;
    } 

    // Update the controller
    public: void OnNewFrame(const unsigned char *_image, 
        unsigned int _width, unsigned int _height, unsigned int _depth, 
        const std::string &_format)
    {
      //std::cout << "OnNewFrame: " <<n << " " <<  this->parentSensor->GetCamera()->GetName()<< std::endl;
		if(count==0){

			std::string name = this->parentSensor->Camera()->Name();


			std::cout <<" camera: " << name  << std::endl;
		
			std::vector<std::string> strs;
			boost::split(strs, name, boost::is_any_of("::"));
		


			nameConfig = std::string("--Ice.Config=" + strs[4] + ".cfg");
			nameCamera = std::string(strs[4]);
			
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
    public: std::string nameConfig;
    public: cv::Mat image;
	public: pthread_mutex_t mutex;
	public: std::string nameGlobal;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_SENSOR_PLUGIN(CameraDump)
}

class CameraI: virtual public jderobot::Camera {
	public:
		CameraI(std::string propertyPrefix, gazebo::CameraDump* camera)
			   : prefix(propertyPrefix), cameraI(camera) {
		
			std::cout << "Constructor CameraI" << std::endl;

			imageDescription = (new jderobot::ImageDescription());
			cameraDescription = (new jderobot::CameraDescription());
			cameraDescription->name = "camera Introrob";

        	replyTask = new ReplyTask(this);
		    replyTask->start(); // my own thread
		  
		}

		std::string getName () {
			return (cameraDescription->name);
		}

		std::string getRobotName () {
			return "RobotName";
		}

		virtual ~CameraI() {


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

		virtual void getImageData_async (const jderobot::AMD_ImageProvider_getImageDataPtr& cb,const std::string& format, const Ice::Current& c){
			replyTask->pushJob(cb, format);
		}

		virtual jderobot::ImageFormat getImageFormat(const Ice::Current& c)
		{
			jderobot::ImageFormat mFormats;
			mFormats.push_back(colorspaces::ImageRGB8::FORMAT_RGB8.get()->name);

			return mFormats;
		}

		virtual std::string startCameraStreaming(const Ice::Current&){

		}

		virtual void stopCameraStreaming(const Ice::Current&) {

		}

		virtual void reset(const Ice::Current&)
		{
		}

	private:
		class ReplyTask: public IceUtil::Thread {
			public:
				ReplyTask(CameraI* camera){
                    mycamera = camera;

				   	std::cout << "safeThread" << std::endl;
				}

				void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, std::string format){

					mFormat = format;
					IceUtil::Mutex::Lock sync(requestsMutex);
					requests.push_back(cb);
				}

				virtual void run(){
					jderobot::ImageDataPtr reply(new jderobot::ImageData);
					struct timeval a, b;
					int cycle = 48;
					long totalb,totala;
					long diff;
					
					int count =0 ;

					while(1){
						
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
				std::string mFormat;
		};

		typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;
		std::string prefix;

		colorspaces::Image::FormatPtr imageFmt;
		jderobot::ImageDescriptionPtr imageDescription;
		jderobot::CameraDescriptionPtr cameraDescription;
		ReplyTaskPtr replyTask;
		gazebo::CameraDump* cameraI;	
		
}; // end class CameraI


void *myMain(void* v)
{

	gazebo::CameraDump* camera = (gazebo::CameraDump*)v;

	char* name = (char*)camera->nameConfig.c_str();


    Ice::CommunicatorPtr ic;
    int argc = 1;

    Ice::PropertiesPtr prop;
	char* argv[] = {name};

    try {
        
        ic = EasyIce::initialize(argc, argv);
        prop = ic->getProperties();
        
        std::string Endpoints = prop->getProperty("CameraGazebo.Endpoints");
        std::cout << "CameraGazebo "<< camera->nameCamera <<" Endpoints > "  << Endpoints << std::endl;

        Ice::ObjectAdapterPtr adapter =
        ic->createObjectAdapterWithEndpoints("CameraGazebo", Endpoints);
		
        Ice::ObjectPtr object = new CameraI(std::string("CameraGazebo"),  camera);
        adapter->add(object, ic->stringToIdentity(camera->nameCamera));
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
