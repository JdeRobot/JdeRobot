#ifndef GUI_H
#define GUI_H

#include <QtGui>

#include "../sensors/sensors.h"

class GUI:public QWidget
{
    Q_OBJECT

public:
    GUI(Sensors* sensors);
    void updateThreadGUI();

private:
    QLabel* labelImage;
    Sensors* sensors;

signals:
    void signal_updateGUI();

public slots:
    void on_updateGUI_recieved();

};

#endif // GUI_H
