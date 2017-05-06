#ifndef POSE3DMOTORSINTERFACE_H
#define POSE3DMOTORSINTERFACE_H


#include <jderobot/pose3dmotors.h>
#include <RosIceMessage/Pose3DMotorsData.h>
//#include "Pose3DMotorsData.h"
#include "Ros_Ice.h"

#include <fstream>

class Pose3DMotorsInterface : virtual public jderobot::Pose3DMotors, public Ros_Ice< Ice::ObjectPrx >
{

public:
    Pose3DMotorsInterface(int argc, char **argv, std::string nodeName);

    virtual ~Pose3DMotorsInterface();

    virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData(const Ice::Current&);
    virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const Ice::Current&);
    virtual Ice::Int setPose3DMotorsData(const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current&);

    void rosCallback(RosIceMessage::Pose3DMotorsData pose3DMotorsMsg);

private:

    //RosIceMessage::MotorsData MotorsMsg;
    jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
    jderobot::Pose3DMotorsParamsPtr pose3DMotorsParamsData;

    std::ofstream myfile;


};


#endif // POSE3DMOTORSINTERFACE_H
