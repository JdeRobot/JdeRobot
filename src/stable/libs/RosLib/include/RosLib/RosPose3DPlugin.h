
#ifndef ROSPOSE3DPLUGIN_H
#define	ROSPOSE3DPLUGIN_H

#include "ros/ros.h"
#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/physics/MultiRayShape.hh>
#include <gazebo/common/common.hh>
#include <gazebo/transport/transport.hh>
#include <RosIceMessage/Pose3D.h>


namespace gazebo
{

class RosPose3DPlugin : public ModelPlugin
{
public:

    RosPose3DPlugin();
    ~RosPose3DPlugin();

    virtual void Load(physics::ModelPtr _parent, sdf::ElementPtr);

private:

    void OnUpdate();
    physics::ModelPtr model;
    //math::Pose position;
    event::ConnectionPtr updateConnection;

    ros::NodeHandle* RosNode;
    ros::Publisher* RosPublisher;

};

GZ_REGISTER_MODEL_PLUGIN(RosPose3DPlugin)

}

#endif	/* ROSPOSE3DPLUGIN_H */

