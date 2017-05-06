
#include "Pose3DMotorsInterface.h"

Pose3DMotorsInterface::Pose3DMotorsInterface(int argc, char **argv, std::string nodeName)
{


    pose3DMotorsData = new jderobot::Pose3DMotorsData();
    pose3DMotorsParamsData = new jderobot::Pose3DMotorsParams();

    initializeROS(argc,argv,nodeName);
    addRosSubscriber<RosIceMessage::Pose3DMotorsData>(nodeName,500,&Pose3DMotorsInterface::rosCallback,this);
    addRosPublisher<RosIceMessage::Pose3DMotorsData>(nodeName + "_sub",500);

    pose3DMotorsParamsData->maxPan = 0;
    pose3DMotorsParamsData->minPan = 0;
    pose3DMotorsParamsData->maxTilt = 0;
    pose3DMotorsParamsData->minTilt = 0;
    pose3DMotorsParamsData->maxPanSpeed = 0;
    pose3DMotorsParamsData->maxTiltSpeed = 0;

    pose3DMotorsData->x = 0;
    pose3DMotorsData->y = 0;
    pose3DMotorsData->z = 0;

    pose3DMotorsData->roll = 0;
    pose3DMotorsData->panSpeed = 0;
    pose3DMotorsData->tiltSpeed = 0;



    //myfile.open("Ros.txt");


}

Pose3DMotorsInterface::~Pose3DMotorsInterface()
{


}


jderobot::Pose3DMotorsDataPtr Pose3DMotorsInterface::getPose3DMotorsData(const Ice::Current&)
{
    return pose3DMotorsData;
}

jderobot::Pose3DMotorsParamsPtr Pose3DMotorsInterface::getPose3DMotorsParams(const Ice::Current &)
{
    return pose3DMotorsParamsData;
}

Ice::Int Pose3DMotorsInterface::setPose3DMotorsData(const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current&)
{

    RosIceMessage::Pose3DMotorsData Msg;

    Msg.x = data->x;
    Msg.y = data->y;
    Msg.z = data->z;
    Msg.pan = data->pan;
    Msg.tilt = data->tilt;
    Msg.roll = data->roll;
    Msg.panSpeed = data->panSpeed;
    Msg.tiltSpeed = data->tiltSpeed;

    rosPublish(Msg);


    return 0;
}



void Pose3DMotorsInterface::rosCallback(RosIceMessage::Pose3DMotorsData pose3DMotorsMsg)
{

    pose3DMotorsData->pan = pose3DMotorsMsg.pan;
    pose3DMotorsData->tilt = pose3DMotorsMsg.tilt;

}

