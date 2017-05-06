
#ifndef RosEncodersPlugin_H
#define	RosEncodersPlugin_H

#include "ros/ros.h"
#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/physics/MultiRayShape.hh>
#include <gazebo/common/common.hh>
#include <gazebo/transport/transport.hh>
#include <RosIceMessage/EncodersData.h>


namespace gazebo
{

class RosEncodersPlugin : public ModelPlugin
{
public:

    RosEncodersPlugin();
    ~RosEncodersPlugin();

    virtual void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf);


private:

    void OnUpdate();
    physics::ModelPtr model;
    //math::Pose position;
    event::ConnectionPtr updateConnection;

    ros::NodeHandle* RosNode;
    ros::Publisher* RosPublisher;

};

GZ_REGISTER_MODEL_PLUGIN(RosEncodersPlugin)

}

#endif	/* RosEncodersPlugin_H */

