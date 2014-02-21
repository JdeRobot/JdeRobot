#ifndef THREADGUI_H
#define THREADGUI_H

#include <QtGui>

#include <iostream>
#include <sys/time.h>

#include "gui.h"

#include "../sensors/sensors.h"

#define cycle_gui 50 //miliseconds


class threadGUI:public QThread
{
public:
    threadGUI(Sensors* sensors);

private:
    GUI* gui;
    Sensors* sensors;
protected:
    void run();
};

#endif // THREADGUI_H
