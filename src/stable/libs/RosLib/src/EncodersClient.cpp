#include "EncodersClient.h"

EncodersClient::EncodersClient(int argc, char **argv, std::string nodeName)
{
    initializeROS(argc,argv,nodeName);
    addRosPublisher <RosIceMessage::EncodersData> (nodeName,500);



}

EncodersClient::~EncodersClient()
{

}

RosIceMessage::EncodersData EncodersClient::publishROS()
{

    RosIceMessage::EncodersData encodersMsg;

    jderobot::EncodersDataPtr encodersData;

    encodersData = this->Proxy->getEncodersData();

    encodersMsg.robotx = encodersData->robotx;
    encodersMsg.roboty = encodersData->roboty;
    encodersMsg.robottheta = encodersData->robottheta;
    encodersMsg.robotcos = encodersData->robotcos;
    encodersMsg.robotsin = encodersData->robotsin;


    rosPublish(encodersMsg);

    return encodersMsg;
}

/*
void EncodersClient::rosCallback(RosIceMessage::EncodersData encodersMessage)
{



}

*/
