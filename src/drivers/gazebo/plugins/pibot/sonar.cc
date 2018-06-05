#include "sonar.h"

namespace gazebo
{     
	void *mainSonar(void* v);
    
	Sonar::Sonar() : SonarPlugin()
	{
		count = 0;
		pthread_mutex_init (&mutex, NULL);	
	}
	
    void Sonar::Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf)
	{
	  // Don't forget to load the camera plugin
	  SonarPlugin::Load(_parent,_sdf);
      //std::cout << "Load: " << this->parentSensor->Name()<< std::endl;
	  this->parentSensor =  std::dynamic_pointer_cast<sensors::SonarSensor>(_parent);
	  this->updateConnection = event::Events::ConnectWorldUpdateBegin(
            boost::bind(&Sonar::OnUpdate, this));

	} 

	// Update the controller
	void Sonar::OnUpdate()
	{

		if(count == 0){
			count++;

			std::string name = this->parentSensor->ParentName();
			std::cout <<" sonar: " << name  << std::endl;
			
			std::vector<std::string> strs;
			boost::split(strs, name, boost::is_any_of("::"));
			
			std::cout << "strs[0]: " << strs[0] << std::endl;

			nameSonar = std::string("--Ice.Config=" + strs[0] + "Sonar.cfg");

			pthread_t thr_gui;
			pthread_create(&thr_gui, NULL, &mainSonar, (void*)this);
		}
	
		float range = this->parentSensor->Range();

		SonarD data;
		data.range = range;

		pthread_mutex_lock (&mutex); 
		sonarData = data;
		pthread_mutex_unlock (&mutex); 
	}

	class SonarI: virtual public jderobot::Sonar {
		public:
			SonarI (gazebo::Sonar* sonar) 
			{
				this->sonar = sonar;
			}

			virtual ~SonarI(){};

			virtual jderobot::SonarDataPtr getSonarData(const Ice::Current&) {
			    jderobot::SonarDataPtr sonarData (new jderobot::SonarData());
				pthread_mutex_lock (&sonar->mutex); 

				sonarData->range = sonar->sonarData.range;
				sonarData->minAngle = sonar->sonarData.minAngle;
	    		sonarData->maxAngle = sonar->sonarData.maxAngle;
	    		sonarData->minRange = sonar->sonarData.minRange;
	    		sonarData->maxRange = sonar->sonarData.maxRange*1000;
				
				pthread_mutex_unlock (&sonar->mutex); 
				return sonarData;
			};

		private:
			float range;
			gazebo::Sonar* sonar;
	};

	void *mainSonar(void* v) 
	{

		gazebo::Sonar* sonar = (gazebo::Sonar*)v;
		
		char* name = (char*)sonar->nameSonar.c_str();

	    Ice::CommunicatorPtr ic;
	    int argc = 1;

	    Ice::PropertiesPtr prop;
		char* argv[] = {name};
	    try {
	        
	        ic = EasyIce::initialize(argc, argv);
	        prop = ic->getProperties();
	        
	        std::string Endpoints = prop->getProperty("Sonar.Endpoints");
	        std::cout << "Sonar Endpoints > " << Endpoints << std::endl;
	        
	        Ice::ObjectAdapterPtr adapter =
	            ic->createObjectAdapterWithEndpoints("Sonar", Endpoints);
	        Ice::ObjectPtr object = new SonarI(sonar);
	        adapter->add(object, ic->stringToIdentity("Sonar"));
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

	GZ_REGISTER_SENSOR_PLUGIN(Sonar)

}