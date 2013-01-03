/*
 *  Copyright (C) 1997-2011 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Authors : Maikel González <m.gonzalezbai@gmail.com>,
 *              José María Cañas <jmplaza@gsyc.es>
 *
 */



#include <pthread.h>
#include "SharedMemory.h"
#include "Gui.h"
#include "controlICE.h"

#define cycle_control 50 //miliseconds
#define cycle_gui 50 //miliseconds

void *runGui(void* v);

using namespace std;

int main(int argc, char** argv) {
    struct timeval a, b;
    long totalb, totala;
    long diff;
    pthread_t thr_gui;
    SharedMemory *interfacesData;
    controlICE *control;
    try {
        interfacesData = new SharedMemory();
        pthread_mutex_init(&interfacesData->imagesData_mutex, NULL);
        interfacesData->exit = false;

        control = new controlICE(interfacesData);

        pthread_create(&thr_gui, NULL, &runGui, (void*) interfacesData);

        while (!interfacesData->exit) {
            gettimeofday(&a, NULL);
            totala = a.tv_sec * 1000000 + a.tv_usec;

            control->checkInterfaces(); //Check if interfaces are activated and init/end them
            control->getDataGazebo(); //Get sensor data from gazebo
            control->sendDataGazebo(); //Send data to Gazebo

            gettimeofday(&b, NULL);
            totalb = b.tv_sec * 1000000 + b.tv_usec;
            diff = (totalb - totala) / 1000;
            if (diff < 0 || diff > cycle_control)
                diff = cycle_control;
            else
                diff = cycle_control - diff;
            /*Sleep Algorithm*/
            usleep(diff * 1000);
            if (diff < 33)
                usleep(33 * 1000);
        }

        pthread_join(thr_gui, NULL);

    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }
    return 0;
}

void *runGui(void* v) {
    struct timeval a, b;
    long totalb, totala;
    long diff;
    Gui* gui;
    SharedMemory* interfacesDataGui = (SharedMemory*) v;
    gui = new Gui(interfacesDataGui);
    while (true) {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        gui->display(); //Show GUI

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

