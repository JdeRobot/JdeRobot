#ifndef GUI_H
#define GUI_H

#include <QtGui>

#include "../shared.h"

namespace basic_component_qt {
class Gui : public QWidget
{
    Q_OBJECT

public:
    Gui(Shared* sm);
    void updateThreadGUI();

private:
    QLabel* labelImage;
    Shared* sm;

signals:
    void signal_updateGUI();

public slots:
    void on_updateGUI_recieved();

};
}
#endif // GUI_H
