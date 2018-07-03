#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/math/Angle.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/transport/transport.hh>
#include <stdio.h>
#include <ctime>

// JDErobot general ice component includes
#include <jderobot/motors.h>

// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <easyiceconfig/EasyIce.h>

namespace gazebo {
    class Motors : public ModelPlugin
    {
        public:

            Motors();
            void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);
            virtual void Init();

            std::string nameMotors;
            float vel, w;
            pthread_mutex_t mutex;
            pthread_mutex_t mutexMotor;

            struct motor_t {
                float v;
                float w;
                float l;
                double wheelMax;
                double wheelMin;
                double targetRightSteerPos,targetLeftSteerPos;
                std::time_t lastVtime;
                std::time_t lastWtime;
            };
            motor_t robotMotors;

        private:
            void PID();
            void OnUpdate();
            //void OnVelMsg(ConstPosePtr &_msg);
            transport::NodePtr node;
            transport::SubscriberPtr velSub;
            physics::JointPtr steerLeftJoint, steerRightJoint, frontLeftJoint, frontRightJoint;
            // Pointer to the model
            physics::ModelPtr model;
            //physics::LinkPtr link, leftFrontmotorsLink, rightFrontmotorsLink, leftRearmotorsLink, rightRearmotorsLink;
            // Pointer to the update event connection
            event::ConnectionPtr updateConnection;
            //double motorspeed[2];
            double motorspeed;
            double motorsteer;
            //double torque;
            //double frontmotorseparation;
            //double frontmotorsRadius;
            //double sum;
            int count;
            common::Time prevUpdateTime;



    };
}
