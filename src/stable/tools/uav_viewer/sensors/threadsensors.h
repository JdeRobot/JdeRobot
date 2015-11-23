#ifndef THREADSENSORS_H
#define THREADSENSORS_H

#include <QtGui>

#include <iostream>
#include <sys/time.h>

#include "sensors.h"

#define cycle_control 15 //miliseconds

class ThreadSensors:public QThread
{
public:
    ThreadSensors(Sensors* sensors);

private:
    Sensors* sensors;

protected:
    void run();

};

#endif // THREADSENSORS_H
