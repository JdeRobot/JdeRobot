#include "robot.h"

Robot::Robot(Comm::Communicator* jdrc)
{
    sensors = new Sensors(jdrc);
    actuators = new Actuators(jdrc);

    pthread_mutex_init (&mutex, NULL);

}

Sensors* Robot::getSensors()
{
    return this->sensors;
}

Actuators* Robot::getActuators()
{
    return this->actuators;
}

void Robot::update()
{
    return;
}




