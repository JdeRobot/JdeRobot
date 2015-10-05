#include "MotorClient.h"

MotorClient::MotorClient(int argc, char **argv, std::string nodeName)
{
    initializeROS(argc,argv,nodeName);
    addRosPublisher <RosIceMessage::MotorData > (nodeName,1000);
    addRosSubscriber(nodeName,1000,&MotorClient::rosCallback,this);



}

MotorClient::~MotorClient()
{

}



void MotorClient::rosCallback(RosIceMessage::MotorData motorData)
{
    this->Proxy->setL(motorData.motorL);
    this->Proxy->setW(motorData.motorW);
    this->Proxy->setV(motorData.motorV);


}

