#ifndef THREADGUI_H
#define THREADGUI_H

#include <iostream>
#include <sys/time.h>

#include "gui.h"

#include "../shared.h"

#define cycle_gui 50 //miliseconds

namespace basic_component {
class ThreadGui
{
public:
    ThreadGui(Shared* sm);
    void start();

private:
    Gui* gui;
};
}
#endif // THREADGUI_H
