#include <boost/bind.hpp>
#include "gazebo.hh"
#include "physics/physics.h"
#include "common/common.h"
#include "transport/transport.h"


// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>


// JDErobot general ice component includes
#include <jderobot/motors.h>
#include <jderobot/encoders.h>

#ifndef ENCODERS_H
#define	ENCODERS_H

namespace gazebo {
    
    
    class Encoders : public ModelPlugin {
    public:
        
        Encoders();
        
        virtual void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/);
        
        pthread_mutex_t mutex;
        pthread_mutex_t mutexEncoders;
        int count;
        struct encoders_t {
            float x;
            float y;
            float theta;
            float cos;
            float sin;
        };
        math::Quaternion initial_q;
        encoders_t robotEncoders;
        std::string nameEncoders;
        
    private:
        
        void OnUpdate();
        physics::ModelPtr model;
        math::Pose position;
        event::ConnectionPtr updateConnection;
        int cycle;
        long totalb, totala, diff;
        struct timeval a, b;        
        
    };
    
}

#endif	/* POSE_H */