#include "roomba/interfaces/laseri.h"

using namespace roomba::interfaces;
using namespace jderobot;


LaserI::LaserI (const RoombaSensors *sensor):
    laserdata(new LaserData()),
    sensor(sensor)
{}

LaserI::~LaserI ()
{}

LaserDataPtr
LaserI::getLaserData ( const Ice::Current& ){

    laserdata->numLaser = sensor->laserData.values.size();
    laserdata->distanceData.resize(sizeof(int)*laserdata->numLaser);
    laserdata->minAngle = sensor->laserData.minAngle;
    laserdata->maxAngle = sensor->laserData.maxAngle;
    laserdata->minRange = sensor->laserData.minRange;
    laserdata->maxRange = sensor->laserData.maxRange*1000;


    //Update laser values
    for(int i = 0 ; i < laserdata->numLaser; i++){
       laserdata->distanceData[i] = sensor->laserData.values[i]*1000;
    }
    return laserdata;
}







