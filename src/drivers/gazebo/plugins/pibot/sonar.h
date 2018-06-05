#include <boost/bind.hpp>
#include "gazebo.hh"
#include "physics/physics.hh"
#include "common/common.hh"
#include "transport/transport.hh"
#include "physics/MultiRayShape.hh"

#include "plugins/SonarPlugin.hh"

#include <boost/algorithm/string.hpp>

#include <iostream>

// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <easyiceconfig/EasyIce.h>

#include <jderobot/sonar.h>

#ifndef SONARMBOT_H
#define	SONARMBOT_H

namespace gazebo {

    class SonarD
    {
        public:
            float range;
            double minAngle = 0.01;
            double maxAngle = 0.01;
            double minRange = 0.03;
            double maxRange = 4.0; //4 m
    };
    
    
    class Sonar : public SonarPlugin {
    public:
        
        Sonar();
        
        virtual void Load(sensors::SensorPtr _parent, sdf::ElementPtr /*_sdf*/);
        
        pthread_mutex_t mutex;
        sensors::SonarSensorPtr parentSensor;
        int count;
        std::string nameSonar;
        SonarD sonarData;
        
    private:
        
        void OnUpdate();
        physics::ModelPtr model;
        event::ConnectionPtr updateConnection;
        
        
    };
    
}

#endif	/* POSE3D_H */
