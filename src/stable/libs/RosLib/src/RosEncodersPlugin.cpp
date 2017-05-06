#include "RosEncodersPlugin.h"

namespace gazebo
{



RosEncodersPlugin::RosEncodersPlugin()
{

}

RosEncodersPlugin::~RosEncodersPlugin()
{

    delete RosPublisher;
    delete RosNode;
}

void RosEncodersPlugin::Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
{
    model = _parent;
    updateConnection = event::Events::ConnectWorldUpdateBegin( boost::bind(&RosEncodersPlugin::OnUpdate, this) );

    RosNode = new ros::NodeHandle;
    RosPublisher = new ros::Publisher(RosNode->advertise<RosIceMessage::EncodersData>( this->GetHandle(), 500));

    ROS_INFO("ROS Publisher Loaded: %s\n", this->GetHandle().c_str());
}

void RosEncodersPlugin::OnUpdate()
{
    math::Pose position;

    position = model->GetWorldPose();

    RosIceMessage::EncodersData encodersMsg;

    math::Quaternion initial_q = position.rot;

    math::Vector3 initial_rpy = initial_q.GetAsEuler();


    double degrees = initial_rpy.z * 180.0 / M_PI;

    if (degrees < 0)
    {
        degrees = 360 + degrees;
    }



    encodersMsg.robotx = position.pos.x;
    encodersMsg.roboty = position.pos.y;
    encodersMsg.robottheta = degrees;



    RosPublisher->publish(encodersMsg);
}



}

