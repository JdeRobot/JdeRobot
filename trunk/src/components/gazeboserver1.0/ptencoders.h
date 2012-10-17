#include <boost/bind.hpp>
#include "gazebo.hh"
#include "physics/physics.h"
#include "common/common.h"
#include "transport/transport.h"
// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>


// JDErobot general ice component includes
#include <jderobot/pose3dencoders.h>

#ifndef PTENCODERS_H
#define PTENCODERS_H

namespace gazebo {

    class PTEncoders : public ModelPlugin {
    public:

        PTEncoders();
        virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);
        virtual void Init();

        std::string namePTEncoders;
        pthread_mutex_t mutex;
        pthread_mutex_t mutexPTEncoders;

        struct ptencoders_t {
            float x;
            float y;
            float z;
            float pan;
            float tilt;
            float roll;
            int clock;

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
        
        ptencoders_t robotPTEncoders;
        
    private:
        
        double torque;
        void OnUpdate();
        physics::ModelPtr model;
        event::ConnectionPtr updateConnection;
        int count;

    };


}

#endif