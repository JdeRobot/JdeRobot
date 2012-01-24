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
 *
 */

#include "API.h"

namespace basic_component{

   double Api::getMotorV(){
      return this->motorVin;
   }
   
   double Api::getMotorW(){
      return this->motorWin;      
   }
   
   double Api::getMotorL(){
      return this->motorLin;
   }
   
   jderobot::LaserDataPtr Api::getLaserData(){
      return this->laserData;
   }
   
   int Api::getNumLasers(){
      return this->laserData->numLaser;
   }
   
   jderobot::IntSeq Api::getDistancesLaser(){
      return this->laserData->distanceData;
   }
   
   jderobot::EncodersDataPtr Api::getEncodersData(){
      return this->encodersData;
   }
   
   colorspaces::Image* Api::getImageCamera1(){
      return this->image1;
   }
   colorspaces::Image* Api::getImageCamera2(){
      return this->image2;
   }
   
   void Api::setMotorV(float motorV){
      this->motorVout=motorV;
   }
   void Api::setMotorW(float motorW){
      this->motorWout=motorW;
   }
   
   void Api::setMotorL(float motorL){
      this->motorLout=motorL;
      
   }
   /*
   void Api::setPTEncoders(jderobot::PTEncodersDataPtr PTencodersData, int cameraId){
      if(cameraId==1)
         this->PTencodersData1=PTencodersData;
      else
         this->PTecondersData2=PTencodersData;
   }
   */

   void Api::imageCameras2openCV(){
  
  
      colorspaces::Image::FormatPtr fmt1 = colorspaces::Image::Format::searchFormat(imageData1->description->format);
      if (!fmt1)
         throw "Format not supported";
       image1 = new colorspaces::Image (imageData1->description->width, imageData1->description->height, fmt1, &(imageData1->pixelData[0])); // Prepare the image to use with openCV

      colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat( imageData2->description->format);
      if (!fmt2)
         throw "Format not supported";
       image2 = new colorspaces::Image ( imageData2->description->width,  imageData2->description->height, fmt2, &( imageData2->pixelData[0])); // Prepare the image to use with openCV
   }


    void Api::RunNavigationAlgorithm(){
      float v, w, l;
      jderobot::LaserDataPtr laser;
      
      imageCameras2openCV();
      imageCamera1=getImageCamera1();
      imageCamera2=getImageCamera2();
      laser=getLaserData(); // Get the laser info
      printf("laser[45]: %d\n", laser->distanceData[45]);
      switch(accion){
              
              case 0:		// Robot hacia adelante
                      
                      if(( laser->distanceData[45] < 1000.0) or ( laser->distanceData[90] < 1000.0) or ( laser->distanceData[135] < 1000.0)){
                              setMotorV(0.);
                              //if ((x_ant == myPoint.x) and (y_ant == myPoint.y) and (z_ant == myPoint.z)){
                              accion=1;
                              printf("### Activado hacia Atras\n");
                              //}
                      }
                      else
                        setMotorV(100);
                      break;

              case 1:		// Robot hacia atras
                      if ((laser->distanceData[45] < 1100) or (laser->distanceData[90] < 1100) or (laser->distanceData[135] < 1100)){
                              setMotorV(-50.);
                              printf("### Llendo hacia atras\n");
                      }
                      else{
                              setMotorV(0.);
                              accion=2;
                      }

                      break;


              case 2:		// Robot girando.
                      if((laser->distanceData[45] < 1300) or (laser->distanceData[90] < 1300) or (laser->distanceData[135] < 1300)){
                              if(sentido%2==0){
                                      setMotorW(50.);
                              }
                              else{
                                      setMotorW(50.*(-1));
                              }
                              printf("### Girando: %d \n", sentido);
                      }
                      else{
                              setMotorW(0.);
                              accion=0;
                              sentido = (1 + rand() % 40);
                      }			

                      break;
      }
   
        
        
        
    }


Api::~Api() {}   

}

