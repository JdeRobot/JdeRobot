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
 *            Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include "control.h"

namespace introrob{

Control::~Control() {}   


   void Control::UpdateSensorsICE(Api *api){      

      api->motorVin=this->mprx->getV();
//      if(api->motorVin!=0)
//            std::cout << "v: " << this->mprx->getV() <<std::endl;

      api->motorWin=this->mprx->getW();
//     if(api->motorWin!=0)
//	   std::cout << "w: " << this->mprx->getW() <<std::endl;


      api->motorLin=this->mprx->getL();
      api->encodersData=this->eprx->getEncodersData();
      	
      api->laserData=this->lprx->getLaserData();

      pthread_mutex_lock(&api->controlGui);            
      api->imageData1 = this->cprx1->getImageData();
      api->imageData2 = this->cprx2->getImageData();
      pthread_mutex_unlock(&api->controlGui);      

      api->Pose3Dencoders1=this->p3deprx1->getPose3DEncodersData();
      

   }

   // Send the actuators info to Gazebo with ICE
   void Control::SetActuatorsICE(Api *api){
      
      if(api->motorWout<5&&api->motorWout>-5)
            this->mprx->setW(0.);
      this->mprx->setW(api->motorWout);
      this->mprx->setL(api->motorLout);
           
      api->Pose3DmotorsData1 = new jderobot::Pose3DMotorsData();
      api->Pose3DmotorsData2 = new jderobot::Pose3DMotorsData();
            
      api->Pose3DmotorsData1->tilt = api->v_normalized;
      api->Pose3DmotorsData1->pan = api->w_normalized;
      api->Pose3DmotorsData2->tilt = api->v_normalized;
      api->Pose3DmotorsData2->pan = api->w_normalized;
      
      
      this->p3dmprx2->setPose3DMotorsData(api->Pose3DmotorsData2);
      this->p3dmprx1->setPose3DMotorsData(api->Pose3DmotorsData1);
      this->mprx->setV(api->motorVout); 
   }

}

