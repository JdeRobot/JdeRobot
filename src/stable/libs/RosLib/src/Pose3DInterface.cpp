
#include "Pose3DInterface.h"

Pose3DInterface::Pose3DInterface(int argc, char **argv, std::string nodeName)
{
    initializeROS(argc,argv,nodeName);
    addRosSubscriber<RosIceMessage::Pose3D>("pose3d_pioneer",500,&Pose3DInterface::rosCallback,this);

    pose3DData = new jderobot::Pose3DData();

    //myfile.open("Ros.txt");


}

Pose3DInterface::~Pose3DInterface()
{


}


jderobot::Pose3DDataPtr Pose3DInterface::getPose3DData(const Ice::Current&)
{
    return pose3DData;
}

int Pose3DInterface::setPose3DData(const jderobot::Pose3DDataPtr&  Pose3DData, const Ice::Current&)
{
    std::cout <<"Not implemented yet\n";
    return 0;
}

void Pose3DInterface::rosCallback(RosIceMessage::Pose3D pose3DMsg)
{
    pose3DData->x = pose3DMsg.x * 1000;
    pose3DData->y = pose3DMsg.y * 1000;
    pose3DData->z = 0.0;
    pose3DData->h = 1.0;

    pose3DData->q0 = pose3DMsg.q0;
    pose3DData->q1 = pose3DMsg.q1;
    pose3DData->q2 = pose3DMsg.q2;
    pose3DData->q3 = pose3DMsg.q3;




}

