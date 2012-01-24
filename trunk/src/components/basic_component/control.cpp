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

namespace mycomponent{

Control::~Control() {}   


   void Control::UpdateSensorsICE(Api *api){      

      api->motorVin=this->mprx->getV();
      api->motorWin=this->mprx->getW();
      api->motorLin=this->mprx->getL();
      api->encodersData=this->eprx->getEncodersData();
      api->laserData=this->lprx->getLaserData();
      
      pthread_mutex_lock(&api->controlGui);            
      api->imageData1 = this->cprx1->getImageData();
      api->imageData2 = this->cprx2->getImageData();
      pthread_mutex_unlock(&api->controlGui);      

      
//      api->laserDataGui=this->laserData;


   }

   // Send the actuators info to Gazebo with ICE
   void Control::SetActuatorsICE(Api *api){
      
      this->mprx->setV(api->motorVout); 
      this->mprx->setW(api->motorWout);
      this->mprx->setL(api->motorLout);
      
      api->PTmotorsData1 = new jderobot::PTMotorsData ();
      api->PTmotorsData2 = new jderobot::PTMotorsData ();

      api->PTmotorsData1->latitude = api->v_normalized;
      api->PTmotorsData1->longitude = api->w_normalized;
      api->PTmotorsData2->latitude = api->v_normalized;
      api->PTmotorsData2->longitude = api->w_normalized;
      
      this->ptmprx1->setPTMotorsData(api->PTmotorsData1);
      this->ptmprx2->setPTMotorsData(api->PTmotorsData2);

   }

}

