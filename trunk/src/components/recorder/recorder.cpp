#include <iostream>
#include <fstream>
#include <pthread.h> 

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <jderobot/motors.h>
#include <jderobot/ptmotors.h>
#include <jderobot/laser.h>
#include <jderobot/encoders.h>
#include <jderobot/ptencoders.h>
#include <jderobot/pose3dmotors.h>
#include <jderobot/pose3dencoders.h>
#include <jderobot/pointcloud.h>
#include <colorspaces/colorspacesmm.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

int main(int argc, char** argv){

   int accion=0;
   int sentido=10;

   int status;
   Ice::CommunicatorPtr ic;
   struct timeval a, b, inicio;
   int cycle = 100;
   long totalb,totala;
   long diff;
   
   // INTERFACE
   jderobot::MotorsPrx mprx;
   jderobot::EncodersPrx eprx;
   jderobot::LaserPrx lprx;
  std::vector<jderobot::CameraPrx> cprx;
	
   jderobot::PTEncodersPrx pteprx1;   
   jderobot::PTEncodersPrx pteprx2;
   jderobot::Pose3DEncodersPrx pose3DEncoders1;
   jderobot::Pose3DEncodersPrx pose3DEncoders2;
   std::vector <jderobot::pointCloudPrx> prx;
  

   //INTERFACE DATA
   jderobot::EncodersDataPtr ed;
   jderobot::LaserDataPtr ld;   

	jderobot::ImageDataPtr imageData;


   jderobot::ImageDataPtr imageData1;
   jderobot::ImageDataPtr imageData2;
   jderobot::ImageDataPtr imageData3;
   jderobot::ImageDataPtr imageData4;
   jderobot::PTEncodersDataPtr PTencodersData1;
   jderobot::PTEncodersDataPtr PTencodersData2;
	jderobot::Pose3DEncodersDataPtr pose3DEncodersData1;
    jderobot::Pose3DEncodersDataPtr pose3DEncodersData2;
   jderobot::pointCloudDataPtr kinectData;

    int avEncoders = 0;
    int avLaser = 0;
    int avCamera1 = 0;
    int avCamera2 = 0;
int avCamera3 = 0;
int avCamera4 = 0;
    int ptencoders = 0;
    int ptencoders2 = 0;
    int avPose3DEncoders1 = 0;
    int avPose3DEncoders2 = 0;
    int avKinect1 = 0;
	int avKinect2 = 0;
    
    int Hz = 10;
    int muestrasLaser = 180;

	int nCameras=0;
	int nDepthSensors=0;
    
   
   //---------------- INPUT ARGUMENTS ---------------//

   if (argc<2){
      std::cout << std::endl << "USE: ./mycomponent --Ice.Config=mycomponent.cfg" << std::endl;      
   }
   
   //---------------- INPUT ARGUMENTS -----------------//
  
   try{
   
      Ice::PropertiesPtr prop;
    
      ic = Ice::initialize(argc,argv);
      
      prop = ic->getProperties();
      
      Hz = prop->getPropertyAsInt("Recorder.Hz");
      cycle = 1000.0/Hz;
      
      avEncoders = prop->getPropertyAsInt("Recorder.Encoders.bool");
      if(avEncoders==1){
         // Contact to ENCODERS interface
         Ice::ObjectPrx baseEncoders = ic->propertyToProxy("Recorder.Encoders.Proxy");
         if (0==baseEncoders)
            throw "Could not create proxy with encoders";
         
         // Cast to encoders
         eprx = jderobot::EncodersPrx::checkedCast(baseEncoders);
         if (0== eprx)
            throw "Invalid proxy Mycomponent.Encoders.Proxy";
      }
      
      avLaser = prop->getPropertyAsInt("Recorder.Laser.bool");
      if(avLaser==1){
         // Contact to LASER interface
         Ice::ObjectPrx baseLaser = ic->propertyToProxy("Recorder.Laser.Proxy");
         if (0==baseLaser)
            throw "Could not create proxy with laser";
         
         // Cast to laser
         lprx = jderobot::LaserPrx::checkedCast(baseLaser);
         if (0== lprx)
            throw "Invalid proxy Mycomponent.Laser.Proxy";
      }
     nCameras = prop->getPropertyAsIntWithDefault("Recorder.nCameras",0);
	if (nCameras > 0 ){
					struct stat buf;
					char dire[]="./images/";
					if( stat( dire, &buf ) == -1 )
					{
						system("mkdir images");
					}
	}
	for (int i=0; i< nCameras; i++){
		std::stringstream sProxy;
	         // Get driver camera
			sProxy << "Recorder.Camera" << i+1 << ".Proxy";
	         Ice::ObjectPrx camara = ic->propertyToProxy(sProxy.str());
	         if (0==camara)
	            throw "Could not create proxy to camera1 server";
	         // cast to CameraPrx
	         jderobot::CameraPrx cprxAux = jderobot::CameraPrx::checkedCast(camara);
	         if (0== cprxAux)
	            throw "Invalid proxy";
			else
				cprx.push_back(cprxAux);
      }

      ptencoders = prop->getPropertyAsInt("Recorder.PTEncoders1.bool");
      if(ptencoders){
         // Contact to PTENCODERS interface
         Ice::ObjectPrx ptencoders1 = ic->propertyToProxy("Recorder.PTEncoders1.Proxy");
         if (0==ptencoders1)
            throw "Could not create proxy with encoders";
         
         // Cast to encoders
         pteprx1 = jderobot::PTEncodersPrx::checkedCast(ptencoders1);
         if (0== pteprx1)
            throw "Invalid proxy Mycomponent.PTEncoders1.Proxy";
      }
      
      ptencoders2 = prop->getPropertyAsInt("Recorder.PTEncoders2.bool");
      if(ptencoders2){
      // Contact to PTENCODERS interface
      Ice::ObjectPrx ptencoders2 = ic->propertyToProxy("Recorder.PTEncoders2.Proxy");
      if (0==ptencoders2)
         throw "Could not create proxy with encoders";
      
      // Cast to encoders
      pteprx2 = jderobot::PTEncodersPrx::checkedCast(ptencoders2);
      if (0== pteprx2)
         throw "Invalid proxy Recorder.PTEncoders2.Proxy";
      }
      
     
      ////pose3DEncoders1
      avPose3DEncoders1 = prop->getPropertyAsInt("Recorder.Pose3DEncoders1.bool");
      if(avPose3DEncoders1){
         // Contact to pose3DEncoders interface
         Ice::ObjectPrx pose3DEncodersIce1 = ic->propertyToProxy("Recorder.Pose3DEncoders1.Proxy");
         if (0==pose3DEncodersIce1)
            throw "Could not create proxy with encoders";
         
         // Cast to encoders
         pose3DEncoders1 =jderobot::Pose3DEncodersPrx::checkedCast(pose3DEncodersIce1);
         if (0== pose3DEncoders1)
            throw "Invalid proxy Recorder.pose3DEncoders1.Proxy";
      }
      //pose3DEncoders2
      avPose3DEncoders2 = prop->getPropertyAsInt("Recorder.Pose3DEncoders2.bool");
      if(avPose3DEncoders2){
         // Contact to pose3DEncoders interface
         Ice::ObjectPrx pose3DEncodersIce2 = ic->propertyToProxy("Recorder.Pose3DEncoders2.Proxy");
         if (0==pose3DEncodersIce2)
            throw "Could not create proxy with encoders";
         
         // Cast to encoders
         pose3DEncoders2 = jderobot::Pose3DEncodersPrx::checkedCast(pose3DEncodersIce2);
         if (0== pose3DEncoders2)
            throw "Invalid proxy Recorder.pose3DEncoders2.Proxy";
      }
      nDepthSensors = prop->getPropertyAsIntWithDefault("Recorder.nDethSensors",0);
	 for (int i=0; i< nDepthSensors; i++){
		std::stringstream sProxy;
	         // Get driver camera
			sProxy << "Recorder.DepthSensor" << i+1 << ".Proxy";
		
		 Ice::ObjectPrx kinect = ic->propertyToProxy(sProxy.str());
         if (0==kinect){
         throw "Could not create proxy with Kinect1"; 
         }
         // Cast to KINECT
         jderobot::pointCloudPrx prxAux = jderobot::pointCloudPrx::checkedCast(kinect);
         if (0== prxAux){
         	throw std::string("Invalid proxy Recorder.Kinect1.Proxy");
         }
		else
			prx.push_back(prxAux);
	}
      
      
      //-----------------ICE----------------//
      
      std::cout << "Load ICE" << std::endl;
      
      //****************************** Processing the Control ******************************///
      

      //---------------- ITERATIONS CONTROL -----------//
 
      muestrasLaser = prop->getPropertyAsInt("Recorder.Laser.Samples");
 
      std::ofstream outfile;
      
		std::string fileName = prop->getProperty("Recorder.FileName");
      outfile.open (fileName.c_str());
      
		std::string robotName = prop->getPropertyWithDefault("Recorder.Hostname","localhost");
		std::string robotPort = prop->getPropertyWithDefault("Recorder.Port","9999");
      
      long timeRelative = 0;
      long timeInicio = 0;
      gettimeofday(&inicio,NULL);   
      timeInicio = inicio.tv_sec*1000000+inicio.tv_usec;
            
      while(true){
         gettimeofday(&a,NULL);
         totala=a.tv_sec*1000000+a.tv_usec;
      

		/*cameras*/
		for (int i=0; i<nCameras ; i++){
			imageData = cprx[i]->getImageData();
			colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(imageData->description->format);
			cv::Mat image;
            	image.create(cv::Size(imageData->description->width, imageData->description->height), CV_8UC3);

            	memcpy((unsigned char *) image.data ,&(imageData->pixelData[0]), image.cols*image.rows * 3);
            
            	char buff[30]; // enough to hold all numbers up to 64-bits
            	sprintf(buff, "images/camera%d_%ld.jpg", i+1, timeRelative);
            	cv::imwrite(buff, image);
            	outfile << timeRelative << "\t"+robotName +":"+robotPort + ":Camera" << i+1 << ":\t" << buff  << std::endl;
		} 
         /* Encoders */ 
         if(avEncoders){
            ed = eprx->getEncodersData(); // cogemos informacion de los encoders
		      //std::cout << timeRelative << " " << "x: " << ed->robotx << " y: " <<ed->roboty << " z: " << ed->robottheta << std::endl;
            outfile << timeRelative << "\t"+robotName +":"+robotPort + ":Encoders:\t" << ed->robotx << "\t" <<ed->roboty << "\t" << ed->robottheta << std::endl;
         }
         
         /* Laser */
         if(avLaser){
            ld = lprx->getLaserData();
            outfile << timeRelative <<" "+robotName +":"+robotPort + ":Laser: ";
            for(int i = 0; i < muestrasLaser; i++){
               outfile <<ld->distanceData[i] << "\t";
            }
               outfile << std::endl;
            //std::cout << ld->distanceData[80] << std::endl;
         }
         
         /* PTencoders A */
         if(avPose3DEncoders1){
            pose3DEncodersData1 = pose3DEncoders1->getPose3DEncodersData();
            outfile << timeRelative <<" "<<robotName <<":"<<robotPort <<":Pose3DEncoders1: "<< pose3DEncodersData1->x << " " << pose3DEncodersData1->y << " " << pose3DEncodersData1->z << " "<< pose3DEncodersData1->pan << " " <<  pose3DEncodersData1->tilt << " " << pose3DEncodersData1->roll <<std::endl;
            //std::cout << timeRelative << " pan: " << pose3DEncodersData1->pan << " tilt:" <<  pose3DEncodersData1->tilt << std::endl;
         }
         
         /* PTencoders B */
         if(avPose3DEncoders2){
            pose3DEncodersData2 = pose3DEncoders2->getPose3DEncodersData();
            outfile << timeRelative <<" "<<robotName <<":"<<robotPort <<":Pose3DEncoders2: "<< pose3DEncodersData2->x << " " << pose3DEncodersData2->y << " " << pose3DEncodersData2->z << " "<< pose3DEncodersData2->pan << " " <<  pose3DEncodersData2->tilt<< " " << pose3DEncodersData2->roll << std::endl;
            
            //std::cout << timeRelative << " pan: " << pose3DEncodersData2->pan << " tilt:" <<  pose3DEncodersData2->tilt << std::endl;
         }
         
         /* PTencoders A deprecated?? */
         if(ptencoders){
            PTencodersData1 = pteprx1->getPTEncodersData();
            outfile << timeRelative << "\t"+robotName +":"+robotPort + ":PTEncoders1:\t";
            outfile << PTencodersData1->panAngle <<"\t" << PTencodersData1->tiltAngle << std::endl; 
         }
         
      /* PTencoders B deprecated?? */
        if(ptencoders2){
            PTencodersData2 = pteprx2->getPTEncodersData();
            outfile << timeRelative << "\t"+robotName +":"+robotPort + ":PTEncoders2:\t";
            outfile << PTencodersData2->panAngle <<"\t" << PTencodersData2->tiltAngle << std::endl; 
         }
         

		/* DepthSensors */
		for (int j=0; j< nDepthSensors ; j++){
            kinectData = prx[j]->getCloudData();
            outfile << timeRelative << "\t"+robotName +":"+robotPort + ":KinectData" << j+1 << ":\t"<< kinectData->p.size() << "\t";
            for(int i = 0; i < kinectData->p.size(); i++){
               float x,y,z;
               float r,g,b;
			   float id;
               x = kinectData->p[i].x;
               y = kinectData->p[i].y;
               z = kinectData->p[i].z;
               r = kinectData->p[i].r;
               g = kinectData->p[i].g;
               b = kinectData->p[i].b;
			   id = (float)kinectData->p[i].id;
               outfile << x << "\t" << y << "\t" << z << "\t" << r << "\t" << g << "\t" << b << "\t" << id <<"\t" ;
            }
            outfile << std::endl;
		} 
     
         gettimeofday(&b,NULL);
         totalb=b.tv_sec*1000000+b.tv_usec;
         std::cout << "Introrob takes " << (totalb-totala)/1000 << " ms" << std::endl;
          
         diff = (totalb-totala)/1000;
         if(diff < 0 || diff > cycle)
            diff = cycle;
         else
            diff = cycle-diff;
         
         //Sleep Algorithm
         usleep(diff*1000);
         if(diff < 10)
            usleep(10*1000);
            
        // std::cout << cycle <<" ->" << diff  << " ->" << timeRelative<< std::endl;
         timeRelative+= diff + (totalb-totala)/1000;
        // std::cout << "->" << diff  << " ->" << timeRelative<< std::endl;
      }
      
      outfile.close();
      
      //--------------ITERATIONS CONTROL-------------// 
   }
   catch (const Ice::Exception& ex) {
      std::cerr << ex << std::endl;
      status = 1;
   } 
   catch (const char* msg) {
      std::cerr << msg << std::endl;
      status = 1;
   }
   
   
   if (ic)
   ic->destroy();
   
   
   
   return 0;
}
