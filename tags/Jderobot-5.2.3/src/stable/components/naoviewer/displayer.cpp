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

#include "displayer.h"

#define cycle_control 1000 //miliseconds

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
Displayer::Displayer ( Control* control, NaoOperator* naooperator) {
    this->control = control;
    this->naooperator = naooperator;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
Displayer::~Displayer () {
    delete this->control;
    delete this->naooperator;
}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void Displayer::init () {
    pthread_t thread_d;
    pthread_create(&thread_d, NULL, &thr_display, (void*) this);
}

void* thr_display ( void* obj ) {
    Displayer* displayer = (Displayer*) obj;
    
    jderobot::ImageDataPtr data = displayer->control->getImage();
    Sensors* sensors = new Sensors(data);
    
    struct timeval a, b;
    long diff;
    long totalb, totala;
    
    while (true) {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;
            
        jderobot::ImageDataPtr data = displayer->control->getImage();
        cv::Mat image1;
        
        if (displayer->naooperator->isFollowingBall()) {
            sensors->update(data);
            image1 = sensors->getImage();
            
            float panSpeed = sensors->getPanSpeed();
            float tiltSpeed = sensors->getTiltSpeed();
            
            displayer->control->setMovement(HEADSPEED, panSpeed, tiltSpeed, 0.0);
        }
        
        image1.create(cv::Size(data->description->width, data->description->height), CV_8UC3);

        memcpy((unsigned char *) image1.data ,&(data->pixelData[0]), image1.cols*image1.rows * 3);
/*        for(int x =0; x < image1.cols; x++){
            for(int y =0; y < image1.rows; y++){
                int indice = image1.step*y + x*image1.channels();
                
                image1.data[indice] = data->pixelData[indice];
                image1.data[indice+1] = data->pixelData[indice];
                image1.data[indice+2] = data->pixelData[indice];
            }
        }
*/        
        displayer->naooperator->display(image1);
        
        //Sleep Algorithm
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
}
