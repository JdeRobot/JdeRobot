#ifndef THREAD_CONTROL_H
#define THREAD_CONTROL_H

#include <iostream>
#include <sys/time.h>

#include "kobukimanager.h"

#include "actuators/motors.h"
#include "sensors/encoders.h"

#include <Ice/Ice.h>


#define cycle_control 50 //miliseconds

class Thread_control
{
public:
    Thread_control(Ice::CommunicatorPtr ic, KobukiManager* kobuki_manager);

public: void run();

private: void initMotors();
private: void initEncoders();
private: Ice::CommunicatorPtr ic;
private: Ice::PropertiesPtr prop;
private: KobukiManager* kobuki_manager;
private: Motors* motors;
private: Encoders* encoders;

};

#endif // THREAD_CONTROL_H
