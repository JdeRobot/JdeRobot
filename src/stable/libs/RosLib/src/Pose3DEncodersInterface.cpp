
#include "Pose3DEncodersInterface.h"

Pose3DEncodersInterface::Pose3DEncodersInterface(int argc, char **argv, std::string nodeName)
{
    initializeROS(argc,argv,nodeName);
    addRosSubscriber<RosIceMessage::Pose3DEncodersData>(nodeName,500,&Pose3DEncodersInterface::rosCallback,this);

    pose3DEncodersData = new jderobot::Pose3DEncodersData();

    pose3DEncodersData->x = 0;
    pose3DEncodersData->y = 0;
    pose3DEncodersData->z = 0;
    pose3DEncodersData->clock = 0;
    pose3DEncodersData->roll = 0;

    //myfile.open("Ros.txt");


}

Pose3DEncodersInterface::~Pose3DEncodersInterface()
{


}


jderobot::Pose3DEncodersDataPtr Pose3DEncodersInterface::getPose3DEncodersData(const Ice::Current&)
{
    return pose3DEncodersData;
}



void Pose3DEncodersInterface::rosCallback(RosIceMessage::Pose3DEncodersData pose3DEncodersMsg)
{

    pose3DEncodersData->pan = pose3DEncodersMsg.pan;
    pose3DEncodersData->tilt = pose3DEncodersMsg.tilt;
}

