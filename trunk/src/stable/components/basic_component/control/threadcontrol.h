#ifndef THREADSENSORS_H
#define THREADSENSORS_H

#include <iostream>
#include <sys/time.h>

#include "control.h"
#include "../shared.h"

#define cycle_control 20 //miliseconds

namespace basic_component {
class ThreadControl
{
public:
    ThreadControl(Ice::CommunicatorPtr ic, Shared* sm);
    void start();

private:
    Control* control;
};
}

#endif // THREADSENSORS_H
