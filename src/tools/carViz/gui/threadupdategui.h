#ifndef THREADUPDATEGUI_H
#define THREADUPDATEGUI_H

#include "../robot/robot.h"
#include "gui.h"

#include <QtWidgets>

#include <iostream>
#include <sys/time.h>

#include <jderobot/config/properties.hpp>

#define cycle_update_gui 50 //miliseconds

class ThreadUpdateGUI: public QThread
{
public:
    ThreadUpdateGUI(Robot *robot, Config::Properties props);

private:
    GUI* gui;
    Robot* robot;

protected:
    void run();
};

#endif // THREADUPDATEGUI_H
