#ifndef ENCODERSINTERFACE_H
#define ENCODERSINTERFACE_H

#include <jderobot/encoders.h>
#include <RosIceMessage/EncodersData.h>
//#include "EncodersData.h"
#include "Ros_Ice.h"


class EncodersInterface : virtual public jderobot::Encoders, public Ros_Ice< Ice::ObjectPrx >
{

public:
    EncodersInterface(int argc, char **argv, std::string nodeName);

    virtual ~EncodersInterface();

    virtual void setEncodersData(const jderobot::EncodersDataPtr&  encodersData, const Ice::Current&);

    virtual jderobot::EncodersDataPtr getEncodersData(const Ice::Current&);
    void rosCallback(RosIceMessage::EncodersData encodersMsg);

private:

    jderobot::EncodersDataPtr encodersData;



};

#endif // ENCODERSINTERFACE_H
