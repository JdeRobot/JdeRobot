#include "encoders.h"

Encoders::Encoders(KobukiManager *kobuki)
{
    this->kobuki = kobuki;
}

void Encoders::setEncodersData(const jderobot::EncodersDataPtr&  encodersData,
                             const Ice::Current&)
{

}


jderobot::EncodersDataPtr Encoders::getEncodersData(const Ice::Current&)
{

    mutex.lock();
    jderobot::EncodersDataPtr encodersData(new jderobot::EncodersData());
    encodersData->robotx = kobuki->getRobotX();
    encodersData->roboty = kobuki->getRobotY();
    encodersData->robottheta = kobuki->getRobotTheta();
    mutex.unlock();
    return encodersData;
}

