#ifndef POSE3DENCODERSINTERFACE_H
#define POSE3DENCODERSINTERFACE_H


#include <jderobot/pose3dencoders.h>
#include <RosIceMessage/Pose3DEncodersData.h>
//#include "Pose3DEncodersData.h"
#include "Ros_Ice.h"

#include <fstream>

class Pose3DEncodersInterface : virtual public jderobot::Pose3DEncoders, public Ros_Ice< Ice::ObjectPrx >
{

public:
    Pose3DEncodersInterface(int argc, char **argv, std::string nodeName);

    virtual ~Pose3DEncodersInterface();

    jderobot::Pose3DEncodersDataPtr getPose3DEncodersData(const Ice::Current&);

    void rosCallback(RosIceMessage::Pose3DEncodersData pose3DEncodersMsg);

private:

    //RosIceMessage::EncodersData encodersMsg;
    jderobot::Pose3DEncodersDataPtr pose3DEncodersData;

    std::ofstream myfile;


};


#endif // POSE3DENCODERSINTERFACE_H
