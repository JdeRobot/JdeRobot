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
 *
 */

#include "control.h"

namespace basic_component {

    Control::~Control() {
    }

    void Control::UpdateSensorsICE(Api *api) {

        pthread_mutex_lock(&api->controlGui);
        api->imageData1 = this->cprx1->getImageData();
        pthread_mutex_unlock(&api->controlGui);

    }

    // Send the actuators info to Gazebo with ICE

    void Control::SetActuatorsICE(Api *api) {

    }

}

