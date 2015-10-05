
#include "EncodersInterface.h"

EncodersInterface::EncodersInterface(int argc, char **argv, std::string nodeName)
{
    initializeROS(argc,argv,nodeName);
    addRosSubscriber<RosIceMessage::EncodersData>("encoders_pioneer",1000,&EncodersInterface::rosCallback,this);

    encodersData = new jderobot::EncodersData();



}

EncodersInterface::~EncodersInterface()
{


}

jderobot::EncodersDataPtr EncodersInterface::getEncodersData(const Ice::Current &)
{
    return encodersData;
}

void EncodersInterface::setEncodersData(const jderobot::EncodersDataPtr &encodersData, const Ice::Current &)
{

}

void EncodersInterface::rosCallback(RosIceMessage::EncodersData encodersMsg)
{
    encodersData->robotx = encodersMsg.robotx * 1000;
    encodersData->roboty = encodersMsg.roboty * 1000;
    encodersData->robottheta = encodersMsg.robottheta;
    encodersData->robotcos = cos(encodersMsg.robottheta);
    encodersData->robotsin = sin(encodersMsg.robottheta);



}
