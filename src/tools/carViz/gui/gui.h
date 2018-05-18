#ifndef GUI_H
#define GUI_H

#include <QtWidgets>

#include "../robot/robot.h"

#include <jderobot/config/properties.hpp>

#include "widget/controlvw.h"
#include "widget/cameraswidget.h"
#include "widget/laserwidget.h"

class GUI:public QWidget
{
    Q_OBJECT

public:
    GUI(Robot* robot, Config::Properties props);
    void updateThreadGUI();

private:
    QPushButton* buttonStopRobot;

    controlVW* canvasVW;
    CamerasWidget* camerasWidget;
    LaserWidget* laserWidget;

    Robot* robot;

    QCheckBox* checkCameras;
    QCheckBox* checkLaser;

    QLabel* currentV;
    QLabel* currentW;
    QLabel* InfoCurrentV;
    QLabel* InfoCurrentW;


signals:
    void signal_updateGUI();

public slots:
    void on_updateGUI_recieved();
    void on_buttonStopRobot_clicked();

    void on_update_canvas_recieved(float v, float w);
    void on_checks_changed();

};

#endif // GUI_H
