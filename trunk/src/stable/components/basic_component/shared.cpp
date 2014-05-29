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
 *            Francisco Pérez <f.perez475@gmail.com>
 *
 */

#include "shared.h"

#define w 400

//Shared memory class storage of shared resources of the component

namespace basic_component{

   Shared::Shared() {
	pthread_mutex_t controlGui = PTHREAD_MUTEX_INITIALIZER;
   }

   //GETTERS
   double Shared::getMotorV(){
      return this->motorVin;
   }
   
   double Shared::getMotorW(){
      return this->motorWin;      
   }
   
   double Shared::getMotorL(){
      return this->motorLin;
   }
   
   jderobot::LaserDataPtr Shared::getLaserData(){
      return this->laserData;
   }
   
   int Shared::getNumLasers(){
      return this->laserData->numLaser;
   }
   
   jderobot::IntSeq Shared::getDistancesLaser(){
      return this->laserData->distanceData;
   }
   
   jderobot::EncodersDataPtr Shared::getEncodersData(){
      return this->encodersData;
   }
   
   cv::Mat Shared::getImageCamera1(){
      pthread_mutex_lock(&this->controlGui);
        cv::Mat result = image1.clone();
	pthread_mutex_unlock(&this->controlGui);
        return result;
   }
   cv::Mat Shared::getImageCamera2(){
      pthread_mutex_lock(&this->controlGui);
        cv::Mat result = image2.clone();
	pthread_mutex_unlock(&this->controlGui);
        return result;
   }
   

   //SETTERS
   void Shared::setMotorV(float motorV){
      this->motorVout=motorV;
   }
   void Shared::setMotorW(float motorW){
      this->motorWout=motorW;
   }
   
   void Shared::setMotorL(float motorL){
      this->motorLout=motorL;
      
   }
    

/*
    void Shared::setImageData(jderobot::ImageDataPtr imageData) {
        pthread_mutex_lock(&this->controlGui);
        this->imageData1 = imageData;
        pthread_mutex_unlock(&this->controlGui);
    }

   void Shared::setPTEncoders(jderobot::PTEncodersDataPtr PTencodersData, int cameraId){
      if(cameraId==1)
         this->PTencodersData1=PTencodersData;
      else
         this->PTecondersData2=PTencodersData;
   }
   */

    //IMAGES MANAGEMENT (only for one camera, for others do the same with image2)

    void Shared::createImage(jderobot::ImageDataPtr data) {
	pthread_mutex_lock(&this->controlGui);
        image1.create(data->description->height, data->description->width, CV_8UC3);
	pthread_mutex_unlock(&this->controlGui);
    }

    void Shared::createEmptyImage() {
	pthread_mutex_lock(&this->controlGui);
	image1.create(w, w, CV_8UC3);
	pthread_mutex_unlock(&this->controlGui);
    }
    
    void Shared::updateImage(jderobot::ImageDataPtr data){
	pthread_mutex_lock(&this->controlGui);
        memcpy((unsigned char *) image1.data ,&(data->pixelData[0]), image1.cols*image1.rows * 3);
	pthread_mutex_unlock(&this->controlGui);
    }
    
    cv::Mat Shared::getImage() {
	pthread_mutex_lock(&this->controlGui);
        cv::Mat result = image1.clone();
	pthread_mutex_unlock(&this->controlGui);
        return result;
    }



/*********************** OBSOLETO *****************************************    
    colorspaces::Image* Shared::getImage() {
        
        pthread_mutex_lock(&this->controlGui);
        colorspaces::Image::FormatPtr fmt1 = colorspaces::Image::Format::searchFormat(imageData1->description->format);
        if (!fmt1)
            throw "Format not supported";
        image1 = new colorspaces::Image (imageData1->description->width, imageData1->description->height, fmt1, &(imageData1->pixelData[0])); // Prepare the image to use with openCV
        pthread_mutex_unlock(&this->controlGui); 
     
     
        return image1;
    }

    colorspaces::Image*  void Shared::createImage(jderobot::ImageDataPtr data) {
        
        pthread_mutex_lock(&this->controlGui);
        colorspaces::Image::FormatPtr fmt1 = colorspaces::Image::Format::searchFormat(data->description->format);
        if (!fmt1)
            throw "Format not supported";
        image1 = new colorspaces::Image (data->description->width, data->description->height, fmt1, &(data->pixelData[0])); // Prepare the image to use with openCV
        pthread_mutex_unlock(&this->controlGui);
        
        return image1;
    }

   void Shared::imageCameras2openCV(){
  
       	pthread_mutex_lock(&this->controlGui);

	//First camera
      	colorspaces::Image::FormatPtr fmt1 = colorspaces::Image::Format::searchFormat(imageData1->description->format);
     	if (!fmt1)
        	throw "Format not supported";
       	image1 = new colorspaces::Image (imageData1->description->width, imageData1->description->height, fmt1, &(imageData1->pixelData[0])); // Prepare the image to use with openCV

	//Second camera
      	colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat( imageData2->description->format);
      	if (!fmt2)
        	throw "Format not supported";
       	image2 = new colorspaces::Image ( imageData2->description->width,  imageData2->description->height, fmt2, &( imageData2->pixelData[0])); // Prepare the image to use with openCV

       	pthread_mutex_unlock(&this->controlGui);
   }
********************************** FIN OBSOLETO*************************/


    void Shared::RunNavigationAlgorithm(){
      
/*
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
   
        
*/  
    }


Shared::~Shared() {}   

}

