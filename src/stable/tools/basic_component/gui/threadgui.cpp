#include "threadgui.h"

ThreadGui::ThreadGui(Shared* sm, IGui* g)
{
    this->sm = sm;
    gui = g;
}

void ThreadGui::start()
{
    //Calculates the refreshing time of the gui.
    struct timeval a, b;
    long totalb, totala;
    long diff;

    while (true) {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        if (!this->sm->isClosed()) {
            //updates the gui (image shown in this component)
            gui->update();
	}else {
	    break;
        }

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
