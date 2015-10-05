#ifndef ROSMOTORSPLUGIN_H
#define ROSMOTORSPLUGIN_H

#include "ros/ros.h"
#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/transport/transport.hh>
#include <RosIceMessage/MotorData.h>
#include <RosIceMessage/MotorsSrv.h>


namespace gazebo
{

class RosMotorsPlugin : public ModelPlugin
{
public:

    RosMotorsPlugin();
    ~RosMotorsPlugin();

    virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);
    virtual void Init();



private:

    physics::ModelPtr model;

    void OnUpdate();


    transport::NodePtr node;
    transport::SubscriberPtr velSub;

    physics::JointPtr leftJoint, rightJoint;
    event::ConnectionPtr updateConnection;
    double wheelSpeed[2];
    double torque;
    double wheelSeparation;
    double wheelRadius;
    common::Time prevUpdateTime;

    physics::LinkPtr link, leftWheelLink, rightWheelLink;
    double sum;


    ros::NodeHandle* RosNode;
    ros::Subscriber* RosSubscriber;
    ros::ServiceServer* RosServer;

    RosIceMessage::MotorData motorMsg;

    void rosCallback(RosIceMessage::MotorData motorMsg);

    bool rosServerFunction(RosIceMessage::MotorsSrv::Request &req, RosIceMessage::MotorsSrv::Response &res);

};

GZ_REGISTER_MODEL_PLUGIN(RosMotorsPlugin)

}

#endif // ROSMOTORSPLUGIN_H
