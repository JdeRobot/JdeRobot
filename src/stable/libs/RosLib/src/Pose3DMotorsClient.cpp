#include "Pose3DMotorsClient.h"

Pose3DMotorsClient::Pose3DMotorsClient(int argc, char **argv, std::string nodeName)
{
    initializeROS(argc,argv,nodeName);
    addRosPublisher < RosIceMessage::Pose3DMotorsData > (nodeName,1000);
    addRosSubscriber(nodeName,1000,&Pose3DMotorsClient::rosCallback,this);



}

Pose3DMotorsClient::~Pose3DMotorsClient()
{

}



void Pose3DMotorsClient::rosCallback(RosIceMessage::Pose3DMotorsData pose3DMotorData)
{

    jderobot::Pose3DMotorsDataPtr Pose3DmotorsData = new jderobot::Pose3DMotorsData();

    Pose3DmotorsData->tilt = pose3DMotorData.tilt;
    Pose3DmotorsData->pan = pose3DMotorData.pan;

    this->Proxy->setPose3DMotorsData(Pose3DmotorsData);



}

