/*
 *  Copyright (C) 1997-2013 JDERobot Developers Team
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
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors : Borja Menéndez <borjamonserrano@gmail.com>
 *
 */

#include "handler.h"

#ifndef COMPILEFORNAO
#include "viewer.h"

#define cycle_handler 50 // miliseconds

void* thr_gui ( void* obj ) {
    Sensors* sensors = (Sensors*) obj;
    Viewer* viewer = new Viewer();
    
    struct timeval a, b;
    long diff;
    long totalb, totala;
    
    while (true) {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;
        
        // Update values
        cv::Mat image = sensors->getImage();
        viewer->display(image, sensors->getRH(), sensors->getGS(), sensors->getBV());
        
        //Sleep Algorithm
        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;
        diff = (totalb - totala) / 1000;
        if (diff < 0 || diff > cycle_handler)
            diff = cycle_handler;
        else
            diff = cycle_handler - diff;

        usleep(diff * 1000);
        if (diff < 33)
            usleep(33 * 1000);
    }
}
#endif

int main ( int argc, char* argv[] ) {            
    Ice::CommunicatorPtr ic;
    try {
        ic = Ice::initialize(argc, argv);
        
        Sensors* sensors = new Sensors(ic);
        Control* control = new Control(ic);
        #ifndef COMPILEFORNAO
        if ( (argc == 2) && (strcmp(argv[1], "--gui") == 0) ) {
            pthread_t t_gui;
            pthread_create(&t_gui, NULL, &thr_gui, (void*) sensors);
        }
        #endif
        Handler* handler = new Handler(sensors, control);
        handler->init();

        delete sensors;
        delete control;
        delete handler;
    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        exit(-1);
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        exit(-1);
    }
    
    return 1;
}
