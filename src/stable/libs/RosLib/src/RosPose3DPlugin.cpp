#include "RosPose3DPlugin.h"

namespace gazebo
{



RosPose3DPlugin::RosPose3DPlugin()
{

}

RosPose3DPlugin::~RosPose3DPlugin()
{

    delete RosPublisher;
    delete RosNode;
}

void RosPose3DPlugin::Load(physics::ModelPtr _parent, sdf::ElementPtr)
{
    model = _parent;
    updateConnection = event::Events::ConnectWorldUpdateBegin( boost::bind(&RosPose3DPlugin::OnUpdate, this) );

    RosNode = new ros::NodeHandle;
    RosPublisher = new ros::Publisher(RosNode->advertise<RosIceMessage::Pose3D>( this->GetHandle(), 500));

    ROS_INFO("ROS Publisher Loaded: %s\n", this->GetHandle().c_str());
}

void RosPose3DPlugin::OnUpdate()
{
    math::Pose position;

    position = model->GetWorldPose();

    RosIceMessage::Pose3D pose3dMsg;

    pose3dMsg.x = position.pos.x;
    pose3dMsg.y = position.pos.y;

    pose3dMsg.q0 = position.rot.w;
    pose3dMsg.q1 = position.rot.x;
    pose3dMsg.q2 = position.rot.y;
    pose3dMsg.q3 = position.rot.z;

    RosPublisher->publish(pose3dMsg);
}



}

