#include <boost/bind.hpp>
#include "gazebo.hh"
#include "physics/physics.h"
#include "physics/MultiRayShape.hh"
#include "common/common.h"
#include "transport/transport.h"

#include "plugins/RayPlugin.hh"

#include <iostream>

namespace gazebo
{     
    class LaserDump : public RayPlugin
  	{ 
		public: LaserDump() : RayPlugin()
		{
			std::cout << "LaserDump" <<std::endl;
		}
		
	    public: void Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf)
		{
		  // Don't forget to load the camera plugin
		  RayPlugin::Load(_parent,_sdf);
		  this->parentSensor =  boost::shared_dynamic_cast<sensors::RaySensor>(_parent);
		  
		} 

		// Update the controller
		public: void OnNewLaserScans()
		{
			std::cout << "LaserDump" <<std::endl;
			physics::MultiRayShapePtr laser = this->parentSensor->GetLaserShape(); 
			std::cout << laser->GetSampleCount ()<<  std::endl;
			for (int i = 0; i< laser->GetSampleCount (); i++){
				if(i%10==0)
					std::cout << std::endl;
				std::cout << laser->GetRange(i)<< "\t";
			}
		}
		
		sensors::RaySensorPtr parentSensor;

  	};

  // Register this plugins with the simulator
  GZ_REGISTER_SENSOR_PLUGIN(LaserDump)
}
