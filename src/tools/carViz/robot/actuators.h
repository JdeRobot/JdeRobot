#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <QMutex>

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <jderobot/types/cmdvel.h>
#include "jderobot/comm/communicator.hpp"
#include <jderobot/comm/motorsClient.hpp>

class Actuators
{
public:
    Actuators(Comm::Communicator* jdrc);
    
    float getMotorV();
    float getMotorW();
    float getMotorL();

    //SETS
    void setMotorV(float motorV);
    void setMotorW(float motorW);
    void setMotorL(float motorL);
    void setMotorSTOP();

private:

    QMutex mutex;

    Comm::Communicator* jdrc;

    // ICE INTERFACES
    Comm::MotorsClient* motorsClient;

};
#endif // ACTUATORS_H
