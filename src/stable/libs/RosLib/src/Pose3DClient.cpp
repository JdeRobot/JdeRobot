#include "Pose3DClient.h"

Pose3DClient::Pose3DClient(int argc, char **argv, std::string nodeName)
{
    initializeROS(argc,argv,nodeName);
    addRosPublisher <RosIceMessage::Pose3D> (nodeName,500);



}

Pose3DClient::~Pose3DClient()
{

}

geometry_msgs::Pose Pose3DClient::publishROS()
{

    geometry_msgs::Pose Pose3D;

    RosIceMessage::Pose3D Pose3DMsg;


    jderobot::Pose3DDataPtr Pose3DData;

    Pose3DData = this->Proxy->getPose3DData();

    if(Pose3DData->h == 0)
    {
        Pose3D.position.x = Pose3DData->x;
        Pose3D.position.y = Pose3DData->y;
        Pose3D.position.z = Pose3DData->z;
    }

    else
    {
        Pose3D.position.x = Pose3DData->x / Pose3DData->h;
        Pose3D.position.y = Pose3DData->y / Pose3DData->h;
        Pose3D.position.z = Pose3DData->z / Pose3DData->h;
    }


    Pose3D.orientation.w = Pose3DData->q0;
    Pose3D.orientation.x = Pose3DData->q1;
    Pose3D.orientation.y = Pose3DData->q2;
    Pose3D.orientation.z = Pose3DData->q3;

    Pose3DMsg.x = Pose3DData->x;
    Pose3DMsg.y = Pose3DData->y;
    Pose3DMsg.z = Pose3DData->z;
    Pose3DMsg.h = Pose3DData->h;

    Pose3DMsg.q0 = Pose3DData->q0;
    Pose3DMsg.q1 = Pose3DData->q1;
    Pose3DMsg.q2 = Pose3DData->q2;
    Pose3DMsg.q3 = Pose3DData->q3;


    rosPublish(Pose3DMsg);

    return Pose3D;
}

