#include "roomba/interfaces/bumperi.h"

using namespace roomba::interfaces;
using namespace jderobot;


BumperI::BumperI (const RoombaSensors *sensor):
    bumperdata(new BumperData()),
    sensor(sensor)
{}

BumperI::~BumperI ()
{}

BumperDataPtr
BumperI::getBumperData ( const Ice::Current& ){

    bumperdata->bumper = sensor->bumperData.bumper;
    bumperdata->state = sensor->bumperData.state;

    return bumperdata;
}







