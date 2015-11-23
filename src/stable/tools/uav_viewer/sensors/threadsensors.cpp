#include "threadsensors.h"

ThreadSensors::ThreadSensors(Sensors* sensors)
{
    this->sensors = sensors;
}

void ThreadSensors::run()
{
    struct timeval a, b, bfreq,afreq;
    long totalb, totala;
    long diff;
	
    gettimeofday(&bfreq, NULL);
    
    while (true) {
        gettimeofday(&a, NULL);
        //total a micro
        totala = a.tv_sec * 1000000 + a.tv_usec;
        this->sensors->update();

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;
        //diff ms
        diff = (totalb - totala) / 1000;
         
        gettimeofday(&afreq, NULL);
 


        if (diff < 0 || diff > cycle_control)
            diff = cycle_control;
        else
            diff = cycle_control - diff;

        /*Sleep Algorithm*/
        usleep(diff * 1000);
        if (diff < 33)
            usleep(33 * 1000);

    }
}
