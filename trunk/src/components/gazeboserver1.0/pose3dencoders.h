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
#include <jderobot/pose3dmotors.h>

#ifndef POSE3DENCODERS_H
#define POSE3DENCODERS_H

namespace gazebo {

    class Pose3DEncoders : public ModelPlugin {
    public:

        Pose3DEncoders();
        virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);
        virtual void Init();

        std::string namePose3DEncoders;
        pthread_mutex_t mutex;
        pthread_mutex_t mutexPose3DEncoders;
        
        

        struct pose3dencoders_t {
            float x;
            float y;
            float z;
            float pan;
            float tilt;
            float roll;
            int clock;

        };
        
        struct pose3dmotors_t{
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
            physics::JointPtr joint_pose3dencoders_tilt, joint_pose3dencoders_pan;
            physics::LinkPtr camera_link_pan, camera_link_tilt;
            pose3dmotors_t motor;
            pose3dencoders_t encoder;            
        };

        camera_t cameraLeft;
        camera_t cameraRight;

    private:
        
        double torque;
        void OnUpdate();
        physics::ModelPtr model;
        event::ConnectionPtr updateConnection;
        int count;
        int cycle;
        long totalb, totala, diff;
        struct timeval a, b;

    };


}

#endif