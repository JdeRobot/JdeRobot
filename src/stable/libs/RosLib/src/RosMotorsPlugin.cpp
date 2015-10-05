
#include "RosMotorsPlugin.h"

namespace gazebo
{

RosMotorsPlugin::RosMotorsPlugin()
{
    wheelSpeed[0] = wheelSpeed[1] = 0.0;
    motorMsg.motorV = 0;
    motorMsg.motorW = 0;
}

RosMotorsPlugin::~RosMotorsPlugin()
{
    delete RosServer;
    delete RosSubscriber;
    delete RosNode;
}

void RosMotorsPlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
{

    model = _model;

    node = transport::NodePtr(new transport::Node());
    node->Init(model->GetWorld()->GetName());


    if (!_sdf->HasElement("left_joint"))
    {
        gzerr << "Motors plugin missing <left_joint> element\n";
    }

    if (!_sdf->HasElement("right_joint"))
    {
        gzerr << "DiffDrive plugin missing <right_joint> element\n";
    }

    leftJoint = model->GetJoint(_sdf->GetElement("left_joint")->Get<std::string>());
    rightJoint = model->GetJoint(_sdf->GetElement("right_joint")->Get<std::string>());

    if(_sdf->HasElement("torque"))
    {
        torque = _sdf->GetElement("torque")->Get<double>();
    }
    else
    {
        gzwarn << "No torque value set for the DiffDrive plugin.\n";
        torque = 5.0;
    }

    if (!leftJoint)
    {
        gzerr << "Unable to find left joint[" << _sdf->GetElement("left_joint")->Get<std::string>() << "]\n";
    }

    if (!rightJoint)
    {
        gzerr << "Unable to find right joint[" << _sdf->GetElement("right_joint")->Get<std::string>() << "]\n";
    }

    updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&RosMotorsPlugin::OnUpdate, this));


    RosNode = new ros::NodeHandle;
    //RosPublisher = new ros::Publisher(RosNode->advertise<RosIceMessage::MotorData>( this->GetHandle(), 500));
    RosSubscriber = new ros::Subscriber(RosNode->subscribe(this->GetHandle() + "_subscriber", 500, &RosMotorsPlugin::rosCallback,this));
    RosServer = new ros::ServiceServer(RosNode->advertiseService(this->GetHandle() + "_server", &RosMotorsPlugin::rosServerFunction,this));

    ROS_INFO("ROS Subscriber Loaded: %s\n", this->GetHandle().c_str());
}

void RosMotorsPlugin::Init()
{
    wheelSeparation = leftJoint->GetAnchor(0).Distance(rightJoint->GetAnchor(0));
    std::cout << "Wheel Separation:" << wheelSeparation << std::endl;
    physics::EntityPtr parent = boost::dynamic_pointer_cast<physics::Entity > (this->leftJoint->GetChild());

    math::Box bb = parent->GetBoundingBox();

    wheelRadius = bb.GetSize().GetMax() * 0.5;
    std::cout << "Wheel Diameter:" << wheelRadius * 2 << std::endl;
}

void RosMotorsPlugin::OnUpdate()
{

    double vr, va;


    vr = motorMsg.motorV/100;

    va = motorMsg.motorW/10;



    wheelSpeed[1] = vr + va * wheelSeparation / 2.0;
    wheelSpeed[0] = vr - va * wheelSeparation / 2.0;

    double leftVelDesired = (wheelSpeed[1] / wheelRadius);
    double rightVelDesired = (wheelSpeed[0] / wheelRadius);
\
    leftJoint->SetParam("vel", 0, leftVelDesired);
    rightJoint->SetParam("vel", 0, rightVelDesired);

    leftJoint->SetParam("fmax", 0, torque);
    rightJoint->SetParam("fmax", 0,torque);
}

void RosMotorsPlugin::rosCallback(RosIceMessage::MotorData motorMsg)
{
    this->motorMsg = motorMsg;
}

bool RosMotorsPlugin::rosServerFunction(RosIceMessage::MotorsSrv::Request &req, RosIceMessage::MotorsSrv::Response &res)
{
    res.motorMsg = this->motorMsg;
    return true;

}

}
