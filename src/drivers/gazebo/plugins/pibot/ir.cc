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

#include <jderobot/ir.h>


void *mainIR(void* v);

namespace gazebo
{   

  
  class IRD
    {
        public:
            int received;
    };

  class IR : public CameraPlugin
  { 
    public: IR() : CameraPlugin(),count(0) 
	{
		pthread_mutex_init (&mutex, NULL);
		n = round(rand()*1000);
		threshold = 150.0;
		brightness = 255;
	}

    public: void Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf)
    {
      // Don't forget to load the camera plugin
      CameraPlugin::Load(_parent,_sdf);
      std::cout << "Load: " <<  this->parentSensor->Camera()->Name()<< std::endl;
    } 

    // Update the controller
    public: void OnNewFrame(const unsigned char *_image, 
        unsigned int _width, unsigned int _height, unsigned int _depth, 
        const std::string &_format)
    {
		if(count==0){

			std::string name = this->parentSensor->Camera()->Name();
		
			std::vector<std::string> strs;
			boost::split(strs, name, boost::is_any_of("::"));
		


			nameIR = std::string("--Ice.Config=" + strs[4] + ".cfg");
			nameCamera = std::string(strs[4]);

			if (count == 0){
				pthread_t thr_gui;
				pthread_create(&thr_gui, NULL, &mainIR, (void*)this);
			}

			image.create(_height, _width, CV_8UC3);
			count++;
		}
					count++;

		pthread_mutex_lock (&mutex);
        memcpy((unsigned char *) image.data, &(_image[0]), _width*_height * 3);

        cv::cvtColor(image, gray_image, CV_BGR2GRAY);
        brightness = cv::mean(gray_image)[0];
					
		int received = 0;
		if (brightness <= threshold)
			received = 1;
		else
			received = 0;

		IRD data;
		data.received = received;

		irData = data;
		pthread_mutex_unlock (&mutex); 
		
    }         

    private: int count;
    private: int n;
    public: std::string nameIR, nameCamera;
    public: cv::Mat image, gray_image, result;
	public: pthread_mutex_t mutex;
	public: std::string nameGlobal;
	public: float brightness, threshold;
	public: IRD irData;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_SENSOR_PLUGIN(IR)
}

	class IRI: virtual public jderobot::IR {
		public:
			IRI (gazebo::IR* ir) 
			{
				this->ir = ir;
			}

			virtual ~IRI(){};

			virtual jderobot::IRDataPtr getIRData(const Ice::Current&) {
			    jderobot::IRDataPtr irData (new jderobot::IRData());
				pthread_mutex_lock (&ir->mutex); 
				irData->received = ir->irData.received;				
				pthread_mutex_unlock (&ir->mutex); 
				return irData;
			};

		private:
			int received;
			gazebo::IR* ir;
	};

	void *mainIR(void* v) 
	{

		gazebo::IR* ir = (gazebo::IR*)v;
		
		char* name = (char*)ir->nameIR.c_str();
		char* irname = (char*)ir->nameCamera.c_str();

	    Ice::CommunicatorPtr ic;
	    int argc = 1;

	    Ice::PropertiesPtr prop;
		char* argv[] = {name};
	    try {
	        
	        ic = EasyIce::initialize(argc, argv);
	        prop = ic->getProperties();
	        
	        std::string Endpoints = prop->getProperty("IR.Endpoints");
	        std::cout << irname << " Endpoints > " << Endpoints << std::endl;
	        
	        Ice::ObjectAdapterPtr adapter =
	            ic->createObjectAdapterWithEndpoints("IR", Endpoints);
	        Ice::ObjectPtr object = new IRI(ir);
	        adapter->add(object, ic->stringToIdentity(irname));
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

	    return NULL;
	}
