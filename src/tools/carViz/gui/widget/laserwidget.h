#ifndef LASERWIDGET_H
#define LASERWIDGET_H


#include <iostream>
#include <math.h>
#include <QtWidgets>
#include <jderobot/types/laserData.h>

class LaserWidget: public QWidget
{
public:
    LaserWidget();
    void update(JdeRobotTypes::LaserData laserData);

protected:
    void paintEvent(QPaintEvent *);

    QMutex mutex;
    JdeRobotTypes::LaserData laserData;

};

#endif // LASERWIDGET_H
