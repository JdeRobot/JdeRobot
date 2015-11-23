#ifndef GUI_H
#define GUI_H

#include <QtGui>

#include "../../shared.h"
#include "../igui.h"

class Gui : public QWidget, public IGui
{
    Q_OBJECT

public:
   
    Gui();
    virtual ~Gui();

    int runGui(basic_component::Shared* sm);
    void update();

private:
    QLabel* labelImage;
    basic_component::Shared* sm;

signals:
    void signal_updateGUI();

public slots:
    virtual void on_updateGUI_recieved();

};
#endif // GUI_H
