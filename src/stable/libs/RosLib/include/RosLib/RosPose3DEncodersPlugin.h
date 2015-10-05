#ifndef ROSPOSE3DENCODERSPLUGIN_H
#define ROSPOSE3DENCODERSPLUGIN_H

#include <ros/ros.h>
#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/transport/transport.hh>

#include <RosIceMessage/Pose3DEncodersData.h>
#include <RosIceMessage/Pose3DMotorsData.h>

namespace gazebo
{

class RosPose3DEncodersPlugin : public ModelPlugin
{

public:

    RosPose3DEncodersPlugin();
    ~RosPose3DEncodersPlugin();
    virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);
    virtual void Init();


    physics::JointPtr joint_pose3dencoders_tilt_left_camera, joint_pose3dencoders_pan_left_camera;
    physics::LinkPtr left_camera_link_pan, left_camera_link_tilt;

    RosIceMessage::Pose3DMotorsData motor_left_camera;
    RosIceMessage::Pose3DEncodersData encoder_left_camera;

    physics::JointPtr joint_pose3dencoders_tilt_right_camera, joint_pose3dencoders_pan_right_camera;
    physics::LinkPtr right_camera_link_pan, right_camera_link_tilt;

    RosIceMessage::Pose3DMotorsData motor_right_camera;
    RosIceMessage::Pose3DEncodersData encoder_right_camera;


    ros::NodeHandle* RosNode;
    ros::Publisher* RosPublisher_Camera_Left_Motor;
    ros::Publisher* RosPublisher_Camera_Right_Motor;

    ros::Publisher* RosPublisher_Camera_Left_Encoder;
    ros::Publisher* RosPublisher_Camera_Right_Encoder;

    ros::Subscriber* RosSubscriber_Camera_Left_Motor;
    ros::Subscriber* RosSubscriber_Camera_Right_Motor;




private:

    double torque;
    void OnUpdate();
    physics::ModelPtr model;
    event::ConnectionPtr updateConnection;

    void rosCallback_Left_Camera(RosIceMessage::Pose3DMotorsData pose3DMotor);
    void rosCallback_Right_Camera(RosIceMessage::Pose3DMotorsData pose3DMotor);
};

GZ_REGISTER_MODEL_PLUGIN(RosPose3DEncodersPlugin)


}


#endif // ROSPOSE3DENCODERSPLUGIN_H
