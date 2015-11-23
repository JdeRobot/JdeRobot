#ifndef THREADGUI_H
#define THREADGUI_H


#include <iostream>
#include <sys/time.h>

#include "igui.h"

#include "../shared.h"

#define cycle_gui 50 //miliseconds

using namespace basic_component;

class ThreadGui
{
public:
    ThreadGui(Shared* sm, IGui* gui);
    void start();

private:
    IGui* gui;
    Shared* sm;
};
#endif // THREADGUI_H
