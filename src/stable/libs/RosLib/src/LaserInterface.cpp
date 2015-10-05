
#include "LaserInterface.h"

LaserInterface::LaserInterface(int argc, char **argv, std::string nodeName)
{
    initializeROS(argc,argv,nodeName);
    addRosSubscriber<RosIceMessage::Laser>("laser",500,&LaserInterface::rosCallback,this);

    laserData = new jderobot::LaserData();

    //myfile.open("Ros.txt");


}

LaserInterface::~LaserInterface()
{


}

jderobot::LaserDataPtr LaserInterface::getLaserData(const Ice::Current&)
{
    return laserData;
}


void LaserInterface::rosCallback(RosIceMessage::Laser laserMsg)
{

    laserData->numLaser = laserMsg.numLaser;
    laserData->distanceData.resize(sizeof(int) * laserData->numLaser);


    for(int i = 0 ; i < laserData->numLaser; ++i)
    {
       laserData->distanceData[i] = laserMsg.distanceData[i];
    }
/*
    for(int i = 0 ; i < laserMsg.numLaser; ++i)
    {
       myfile << laserMsg.distanceData[i] * 1000<< " ";
    }

    myfile <<laserMsg.numLaser<< std::endl;
*/



}

