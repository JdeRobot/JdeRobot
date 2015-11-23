#include "threadgui.h"
#include <QtGui/QDesktopWidget>

threadGUI::threadGUI(Sensors* sensors)
{
    this->sensors = sensors;
    
    gui = new MainWindow();
    //Centramos la ventana en la pantalla
    int width = gui->frameGeometry().width();
    int height = gui->frameGeometry().height();

    QDesktopWidget wid;

    int screenWidth = wid.screen()->width();
    int screenHeight = wid.screen()->height();

    gui->setGeometry((screenWidth/2)-(width/2),(screenHeight/2)-(height/2),width,height);
    gui->setSensors(sensors);
    gui->show();

}

void threadGUI::run()
{
    struct timeval a, b;
    long totalb, totala;
    long diff;

    while (true) {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        gui->updateThreadGUI();

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;
        diff = (totalb - totala) / 1000;

        if (diff < 0 || diff > cycle_gui)
            diff = cycle_gui;
        else
            diff = cycle_gui - diff;


        /*Sleep Algorithm*/
        usleep(diff * 1000);
        if (diff < 33)
            usleep(33 * 1000);
    }
}
