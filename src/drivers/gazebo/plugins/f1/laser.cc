#include <boost/bind.hpp>
#include "gazebo.hh"
#include "physics/physics.hh"
#include "physics/MultiRayShape.hh"
#include "common/common.hh"
#include "transport/transport.hh"

#include "plugins/RayPlugin.hh"

#include <boost/algorithm/string.hpp>


#include <iostream>

// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <easyiceconfig/EasyIce.h>

#include <jderobot/laser.h>


class LaserD
{
public:
    std::vector<float> values;
    double minAngle = 0;
    double maxAngle = 3.1416;
    double minRange = 0;
    double maxRange = 10; //10 m
};

void *mainLaser(void* v);

namespace gazebo
{     
    class LaserDump : public RayPlugin
  	{ 
  	
		public: LaserDump() : RayPlugin()
		{
			std::cout << "LaserDump Constructor" <<std::endl;
			count = 0;
			pthread_mutex_init (&mutex, NULL);	
		}
		
	    public: void Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf)
		{
		  // Don't forget to load the camera plugin
		  RayPlugin::Load(_parent,_sdf);
		  this->parentSensor =  std::dynamic_pointer_cast<sensors::RaySensor>(_parent);

		} 

		// Update the controller
		public: void OnNewLaserScans()
		{

			if(count == 0){
				count++;

				std::string name = this->parentSensor->ParentName();
				std::cout <<" laser: " << name  << std::endl;
				
				std::vector<std::string> strs;
				boost::split(strs, name, boost::is_any_of("::"));
				
				std::cout << "strs[0]: " << strs[0] << std::endl;

				nameLaser = std::string("--Ice.Config=" + strs[0] + "_laser.cfg");

				pthread_t thr_gui;
				pthread_create(&thr_gui, NULL, &mainLaser, (void*)this);
			}
		
			physics::MultiRayShapePtr laser = this->parentSensor->LaserShape();

			LaserD data;
			std::vector<float> laserValues(laser->GetSampleCount ());

			laserValues.resize(laser->GetSampleCount ());
			for (int i = 0; i< laser->GetSampleCount (); i++){
				laserValues[i] = laser->GetRange(i);
			}

			data.values = laserValues;

			pthread_mutex_lock (&mutex); 
			laserData = data;
			pthread_mutex_unlock (&mutex); 
		}
		sensors::RaySensorPtr parentSensor;
		int count;
		std::string nameLaser;
		LaserD laserData;
		pthread_mutex_t mutex;
  	};

  // Register this plugins with the simulator
  GZ_REGISTER_SENSOR_PLUGIN(LaserDump)
}

class LaserI: virtual public jderobot::Laser {
	public:
		LaserI (gazebo::LaserDump* laser) 
		{
			this->laser = laser;
		}

		virtual ~LaserI(){};

		virtual jderobot::LaserDataPtr getLaserData(const Ice::Current&) {
		    jderobot::LaserDataPtr laserData (new jderobot::LaserData());
			pthread_mutex_lock (&laser->mutex); 
			laserData->numLaser = laser->laserData.values.size();
			laserData->distanceData.resize(sizeof(int)*laserData->numLaser);

			laserData->minAngle = laser->laserData.minAngle;
    		laserData->maxAngle = laser->laserData.maxAngle;
    		laserData->minRange = laser->laserData.minRange;
    		laserData->maxRange = laser->laserData.maxRange*1000;
			
			//Update laser values
			for(int i = 0 ; i < laserData->numLaser; i++){
			   laserData->distanceData[i] = laser->laserData.values[i]*1000;
			}
			pthread_mutex_unlock (&laser->mutex); 
			return laserData;
		};

	private:
		int laser_num_readings;
		gazebo::LaserDump* laser;
};

void *mainLaser(void* v) 
{

	gazebo::LaserDump* laser = (gazebo::LaserDump*)v;
	
	char* name = (char*)laser->nameLaser.c_str();

    Ice::CommunicatorPtr ic;
    int argc = 1;

    Ice::PropertiesPtr prop;
	char* argv[] = {name};
    try {
        
        ic = EasyIce::initialize(argc, argv);
        prop = ic->getProperties();
        
        std::string Endpoints = prop->getProperty("Laser.Endpoints");
        std::cout << "Laser Endpoints > " << Endpoints << std::endl;
        
        Ice::ObjectAdapterPtr adapter =
            ic->createObjectAdapterWithEndpoints("Laser", Endpoints);
        Ice::ObjectPtr object = new LaserI(laser);
        adapter->add(object, ic->stringToIdentity("Laser"));
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
