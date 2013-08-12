/*
 *
 *  Copyright (C) 1997-2011 JDERobot Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 *  Authors : Maikel González <m.gonzalezbai@gmail.com>
 *                 Jose María Cañas Plaza <jmplaza@gsyc.es>
 */

#include "control.h"


namespace wiimoteClient {

    Control::~Control() {
    }

    void Control::updateData(Api *api) {

        jderobot::AccelerometerDataPtr dataAcc = wiiprx->getAccData();
        jderobot::InfraredDataPtr dataIr = wiiprx->getIrData();
        jderobot::NunchukDataPtr dataNunchuk = wiiprx->getNunchukData();

        api->acc[0] = dataAcc->accelerometer[0];
        api->acc[1] = dataAcc->accelerometer[1];
        api->acc[2] = dataAcc->accelerometer[2];

        api->ir1[0] = dataIr->infrared1[0];
        api->ir1[1] = dataIr->infrared1[1];

        api->ir2[0] = dataIr->infrared2[0];
        api->ir2[1] = dataIr->infrared2[1];

        api->ir3[0] = dataIr->infrared3[0];
        api->ir3[1] = dataIr->infrared3[1];

        api->ir4[0] = dataIr->infrared4[0];
        api->ir4[1] = dataIr->infrared4[1];        
        
        //std::cout << dataIr->infrared1[0] << std::endl;
        
        api->nunchukAcc[0] = dataNunchuk->acc[0];
        api->nunchukAcc[1] = dataNunchuk->acc[1];
        api->nunchukAcc[2] = dataNunchuk->acc[2];
        
        api->nunchukButton = dataNunchuk->button;

        api->button = wiiprx->getButtonData();
        
        //std::cout << api->button << std::endl;
        //std::cout << api->nunchukAcc[0] << std::endl;

//        api->nunchukAcc[0] = dataNunchuk->acc[0];
//        api->nunchukAcc[1] = dataNunchuk->acc[1];
//        api->nunchukAcc[2] = dataNunchuk->acc[2];
//
        api->nunchukStick[0] = dataNunchuk->stick[0];
        api->nunchukStick[1] = dataNunchuk->stick[1];
//
//        api->nunchukButton = dataNunchuk->button;



    }

}