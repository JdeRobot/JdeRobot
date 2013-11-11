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

#define cycle_handler 50 // miliseconds

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
Handler::Handler ( Sensors* sensors, Control* control ) {
    this->sensors = sensors;
    this->control = control;
    
    this->panSpeed = 0.0;
    this->tiltSpeed = 0.0;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
Handler::~Handler () {
    delete this->sensors;
    delete this->control;
}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void Handler::init () {
    struct timeval a, b;
    long diff;
    long totalb, totala;
    
    while (true) {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;
        
        // Update values
        this->sensors->update();
        this->analyzeAndGetValues(this->sensors->getErrorX(), this->sensors->getErrorY());
        this->control->sendValues(this->panSpeed, this->tiltSpeed);
        
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

void Handler::analyzeAndGetValues ( float errorX, float errorY ) {
    this->panSpeed = errorX / (320 / 2.0);
    this->tiltSpeed = -errorY / (240 / 2.0);
}
