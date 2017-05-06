#ifndef LASERINTERFACE_H
#define LASERINTERFACE_H

#include <jderobot/laser.h>
#include <RosIceMessage/Laser.h>
//#include "Laser.h"
#include "Ros_Ice.h"

#include <fstream>

class LaserInterface : virtual public jderobot::Laser, public Ros_Ice< Ice::ObjectPrx >
{

public:
    LaserInterface(int argc, char **argv, std::string nodeName);

    virtual ~LaserInterface();

    virtual jderobot::LaserDataPtr getLaserData(const Ice::Current&);

    void rosCallback(RosIceMessage::Laser laserMsg);

private:

    //RosIceMessage::EncodersData encodersMsg;
    jderobot::LaserDataPtr laserData;

    std::ofstream myfile;


};

#endif // LASERINTERFACE_H
