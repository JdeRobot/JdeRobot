#include "Pose3DEncodersClient.h"

Pose3DEncodersClient::Pose3DEncodersClient(int argc, char **argv, std::string nodeName)
{
    initializeROS(argc,argv,nodeName);
    addRosPublisher <RosIceMessage::Pose3DEncodersData> (nodeName,500);



}

Pose3DEncodersClient::~Pose3DEncodersClient()
{

}

void Pose3DEncodersClient::publishROS()
{

    RosIceMessage::Pose3DEncodersData Pose3DEncodersMsg;

    jderobot::Pose3DEncodersDataPtr Pose3DEncodersData;

    Pose3DEncodersData = this->Proxy->getPose3DEncodersData();

    Pose3DEncodersMsg.x = Pose3DEncodersData->x;
    Pose3DEncodersMsg.y = Pose3DEncodersData->y;
    Pose3DEncodersMsg.z = Pose3DEncodersData->z;
    Pose3DEncodersMsg.pan = Pose3DEncodersData->pan;
    Pose3DEncodersMsg.tilt = Pose3DEncodersData->tilt;
    Pose3DEncodersMsg.roll = Pose3DEncodersData->roll;

    Pose3DEncodersMsg.clock = Pose3DEncodersData->clock;
    Pose3DEncodersMsg.maxPan = Pose3DEncodersData->maxPan;
    Pose3DEncodersMsg.minPan = Pose3DEncodersData->minPan;
    Pose3DEncodersMsg.maxTilt = Pose3DEncodersData->maxTilt;
    Pose3DEncodersMsg.minTilt = Pose3DEncodersData->minTilt;



    rosPublish(Pose3DEncodersMsg);

}
