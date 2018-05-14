#include "warehouserobot/interfaces/bumperi.h"

using namespace warehouserobot::interfaces;
using namespace jderobot;


BumperI::BumperI (const WarehouserobotSensors *sensor):
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
