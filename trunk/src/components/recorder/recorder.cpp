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
#include "pose3dmotors.h"
#include "pose3dencoders.h"
#include "cloudPoints.h"
#include <colorspaces/colorspacesmm.h>

//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>

#include <cv.h>
#include <highgui.h>

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
   jderobot::CameraPrx cprx1;
   jderobot::CameraPrx cprx2;
   jderobot::PTEncodersPrx pteprx1;   
   jderobot::PTEncodersPrx pteprx2;
	jderobot::Pose3DEncodersPrx pose3DEncoders1;
	jderobot::Pose3DEncodersPrx pose3DEncoders2;
   jderobot::CloudPointsInterfacePrx prx;
  

   //INTERFACE DATA
   jderobot::EncodersDataPtr ed;
   jderobot::LaserDataPtr ld;   
   jderobot::ImageDataPtr imageData1;
   jderobot::ImageDataPtr imageData2;
   jderobot::PTEncodersDataPtr PTencodersData1;
   jderobot::PTEncodersDataPtr PTencodersData2;
	jderobot::Pose3DEncodersDataPtr pose3DEncodersData1;
	jderobot::Pose3DEncodersDataPtr pose3DEncodersData2;
   jderobot::CloudPointsDataPtr kinectData; 
    
    int avEncoders = 0;
    int avLaser = 0;
    int avCamera1 = 0;
    int avCamera2 = 0;
    int ptencoders = 0;
    int ptencoders2 = 0;
    int avPose3DEncoders1 = 0;
    int avPose3DEncoders2 = 0;
    int avKinect = 0;
    
    int Hz = 10;
    int muestrasLaser = 180;
    
   
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
      
      avCamera1 = prop->getPropertyAsInt("Recorder.Camera1.bool");
      if(avCamera1==1){
         // Get driver camera
         Ice::ObjectPrx camara1 = ic->propertyToProxy("Recorder.Camera1.Proxy");
         if (0==camara1)
            throw "Could not create proxy to camera1 server";
         
         // cast to CameraPrx
         cprx1 = jderobot::CameraPrx::checkedCast(camara1);
         if (0== cprx1)
            throw "Invalid proxy";
      }  
      
      avCamera2 = prop->getPropertyAsInt("Recorder.Camera2.bool");
      if(avCamera2==1){
         // Get driver camera
         Ice::ObjectPrx camara2 = ic->propertyToProxy("Recorder.Camera2.Proxy");
         if (0==camara2)
            throw "Could not create proxy to camera1 server";
         
         // cast to CameraPrx
         cprx2 = jderobot::CameraPrx::checkedCast(camara2);
         if (0== cprx2)
            throw "Invalid proxy";
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
      
      avKinect = prop->getPropertyAsInt("Recorder.Kinect1.bool");
      if(avKinect){
         // Contact to KINECT interface
         Ice::ObjectPrx kinect = ic->propertyToProxy("Recorder.Kinect1.Proxy");
         if (0==kinect){
         throw "Could not create proxy with Kinect1"; 
         }
         // Cast to KINECT
         prx = jderobot::CloudPointsInterfacePrx::checkedCast(kinect);
         if (0== prx){
         throw std::string("Invalid proxy Recorder.Kinect1.Proxy");
         }
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
      
         /* CameraA */
         if(avCamera1){
            imageData1 = cprx1->getImageData();
            colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(
                                          imageData1->description->format);

            if (!fmt)
               throw "Format not supported";

            cv::Mat image1;
            image1.create(cv::Size(imageData1->description->width, imageData1->description->height), CV_8UC3);

            memcpy((unsigned char *) image1.data ,&(imageData1->pixelData[0]), image1.cols*image1.rows * 3);
            
            char buff[30]; // enough to hold all numbers up to 64-bits
            sprintf(buff, "images/camera1_%ld.jpg", timeRelative);
            cv::imwrite(buff, image1);
            outfile << timeRelative << "\t"+robotName +":"+robotPort + ":Camera1:\t" << buff  << std::endl;
         }
         
         /* Camera B */
         if(avCamera2){
            imageData2 = cprx2->getImageData();
            colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(
                                          imageData2->description->format);

            if (!fmt)
               throw "Format not supported";

            cv::Mat image2;
            image2.create(cv::Size(imageData2->description->width, imageData2->description->height), CV_8UC3);

            memcpy((unsigned char *) image2.data ,&(imageData2->pixelData[0]), image2.cols*image2.rows * 3);
            
            char buff[30]; // enough to hold all numbers up to 64-bits
            sprintf(buff, "images/camera2_%ld.jpg", timeRelative);
            cv::imwrite(buff, image2);
            outfile << timeRelative << "\t"+robotName +":"+robotPort + ":Camera2:\t" << buff  << std::endl;
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
         
         
         if(avKinect){
            kinectData = prx->getKinectData();
            outfile << timeRelative << "\t"+robotName +":"+robotPort + ":KinectData:\t"<< kinectData->points.size() << "\t";
            for(int i = 0; i < kinectData->points.size(); i++){
               outfile << (float)kinectData->points[i].x <<"\t" << (float) kinectData->points[i].y <<"\t" << (float)kinectData->points[i].z <<"\t"<< (float)kinectData->points[i].r <<"\t"<< (float)kinectData->points[i].g <<"\t"<< (float)kinectData->points[i].b <<"\t";
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
