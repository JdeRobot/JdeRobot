#include <boost/bind.hpp>
#include "gazebo.hh"
#include "physics/physics.h"
#include "common/common.h"
#include "transport/transport.h"
// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>


// JDErobot general ice component includes
#include <jderobot/pose3dmotors.h>

#ifndef PTMOTORS_H
#define PTMOTORS_H

namespace gazebo {
    
    class PTMotors : public ModelPlugin {
        
    public:
        
        PTMotors();
        virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);
        virtual void Init();

        std::string namePTMotors;
        pthread_mutex_t mutex;
        pthread_mutex_t mutexPTMotors;

        struct ptmotors_t{
            float x;
            float y;
            float z;
            float pan;
            float tilt;
            float roll;
            float panSpeed;
            float tiltSpeed;
            float maxPan;
            float minPan;
            float maxTilt;
            float minTilt;
            float maxPanSpeed;
            float maxTiltSpeed;
        };
        
        struct camera_t {
            physics::JointPtr joint_ptencoders;
            physics::LinkPtr camera_link;
            math::Quaternion initial_q;
            double degreesX;
            double degreesZ;
            math::Vector3 initial_rpy;
            math::Pose position;
        };

        camera_t cameraLeft;
        camera_t cameraRight;
        
        ptmotors_t robotPTMotors;
    private:
        
        double torque;
        void OnUpdate();
        physics::ModelPtr model;
        event::ConnectionPtr updateConnection;
        int count;
        
        
        
    };
    
}

#endif