#ifndef THREADCONTROL_H
#define THREADCONTROL_H

#include <iostream>
#include <sys/time.h>

#include "control.h"
#include "../shared.h"

#define cycle_control 20 //miliseconds

namespace basic_component_qt {
class ThreadControl
{
public:
    ThreadControl(Ice::CommunicatorPtr ic, Shared* sm);
    void start();

private:
    Control* control;

};
}

#endif // THREADCONTROL_H
