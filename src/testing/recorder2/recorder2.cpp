#include <iostream>
#include <fstream>
#include <pthread.h> 
#include <signal.h>
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
#include <jderobot/pose3d.h>
#include <jderobot/pointcloud.h>
#include <visionlib/colorspaces/colorspacesmm.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include "recordergui.h"
#include "poolWriteImages.h"
#include "poolWritePose3dEncoders.h"
#include "poolWritePose3d.h"
#include "poolWriteLasers.h"
#include "poolWritePointCloud.h"
#include "poolWriteEncoders.h"

bool recording=false;
struct timeval inicio;
bool globalActive=true;
Ice::CommunicatorPtr ic;
pthread_t consumerThreads[100];
pthread_t producerThreads[100];
int nCameras=0;
int nDepthSensors=0;
int nLasers=0;
int nPose3dEncoders=0;
int nEncoders=0;
int nPose3d=0;
int totalConsumers=0;
int totalProducers=0;
bool killed=false;

//how to include this threads under the class
//cameras
//thread for the camera consumer
void* camera_pool_consumer_thread(void* foo_ptr){
	recorder::poolWriteImages* pool =static_cast<recorder::poolWriteImages*>(foo_ptr);
	while (globalActive){
		if (recording)
			pool->consumer_thread();
		else
			usleep(1000);
	}

	pthread_exit(NULL);
	return NULL;
}
//thread for the camera producer
void* camera_pool_producer_thread(void* foo_ptr){
	recorder::poolWriteImages* pool =static_cast<recorder::poolWriteImages*>(foo_ptr);
	while (globalActive){
		if (recording)
			pool->producer_thread(inicio);
		else
			usleep(1000);
	}
	pthread_exit(NULL);
	return NULL;
}

//lasers
// thread for the laser consumer
void* laser_pool_consumer_thread(void* foo_ptr){
	recorder::poolWriteLasers* pool =static_cast<recorder::poolWriteLasers*>(foo_ptr);
	while (globalActive){
		if (recording)
			pool->consumer_thread();
		else
			usleep(1000);
	}
	pthread_exit(NULL);
	return NULL;
}
// thread for the laser producer
void* laser_pool_producer_thread(void* foo_ptr){
	recorder::poolWriteLasers* pool =static_cast<recorder::poolWriteLasers*>(foo_ptr);
	while (globalActive){
		if (recording)
			pool->producer_thread(inicio);
		else
			usleep(1000);
	}
	pthread_exit(NULL);
	return NULL;
}

//lasers
// thread for the pose3dencoders consumer
void* pose3dencoders_pool_consumer_thread(void* foo_ptr){
	recorder::poolWritePose3dEncoders* pool =static_cast<recorder::poolWritePose3dEncoders*>(foo_ptr);
	while (globalActive){
		if (recording)
			pool->consumer_thread();
		else
			usleep(1000);
	}
	pthread_exit(NULL);
	return NULL;
}
// thread for the pose3dencoders producer
void* pose3dencoders_pool_producer_thread(void* foo_ptr){
	recorder::poolWritePose3dEncoders* pool =static_cast<recorder::poolWritePose3dEncoders*>(foo_ptr);
	while (globalActive){
		if (recording)
			pool->producer_thread(inicio);
		else
			usleep(1000);
	}
	pthread_exit(NULL);
	return NULL;
}
// thread for the pose3d consumer
void* pose3d_pool_consumer_thread(void* foo_ptr){
	recorder::poolWritePose3d* pool =static_cast<recorder::poolWritePose3d*>(foo_ptr);
	while (globalActive){
		if (recording)
			pool->consumer_thread();
		else
			usleep(1000);
	}
	pthread_exit(NULL);
	return NULL;
}
// thread for the pose3d producer
void* pose3d_pool_producer_thread(void* foo_ptr){
	recorder::poolWritePose3d* pool =static_cast<recorder::poolWritePose3d*>(foo_ptr);
	while (globalActive){
		if (recording)
			pool->producer_thread(inicio);
		else
			usleep(1000);
	}
	pthread_exit(NULL);
	return NULL;
}
//pointcloud
// thread for the pointcloud consumer
void* pointcloud_pool_consumer_thread(void* foo_ptr){
	recorder::poolWritePointCloud* pool =static_cast<recorder::poolWritePointCloud*>(foo_ptr);
	while (globalActive){
		if (recording)
			pool->consumer_thread();
		else
			usleep(1000);
	}
	pthread_exit(NULL);
	return NULL;
}
// thread for the pointcloud producer
void* pointcloud_pool_producer_thread(void* foo_ptr){
	recorder::poolWritePointCloud* pool =static_cast<recorder::poolWritePointCloud*>(foo_ptr);
	while (globalActive){
		if (recording)
			pool->producer_thread(inicio);
		else
			usleep(1000);
	}
	pthread_exit(NULL);
	return NULL;
}


// thread for the encoders consumer
void* encoders_pool_consumer_thread(void* foo_ptr){
	recorder::poolWriteEncoders* pool =static_cast<recorder::poolWriteEncoders*>(foo_ptr);
	while (globalActive){
		if (recording)
			pool->consumer_thread();
		else
			usleep(1000);
	}
	pthread_exit(NULL);
	return NULL;
}
// thread for the encoders producer
void* encoders_pool_producer_thread(void* foo_ptr){
	recorder::poolWriteEncoders* pool =static_cast<recorder::poolWriteEncoders*>(foo_ptr);
	while (globalActive){
		if (recording)
			pool->producer_thread(inicio);
		else
			usleep(1000);
	}
	pthread_exit(NULL);
	return NULL;
}


void exitApplication(int s){
	globalActive=false;
	killed=true;
	std::cout << totalConsumers << std::endl;
	for (int i = 0; i < totalConsumers; i++) {
		pthread_join(consumerThreads[i], NULL);
	}
	for (int i = 0; i < totalProducers; i++) {
		pthread_join(producerThreads[i], NULL);
	}
	//odenamdos los ficheros.
	for (int i=0; i< nCameras; i++){
		std::stringstream instruction1;
		instruction1 << "sort -n data/images/camera" << i+1 << "/cameraData.jde >tempSORT.temp";
		system(instruction1.str().c_str());
		std::stringstream instruction2;
		instruction2 << "mv tempSORT.temp data/images/camera" << i+1 << "/cameraData.jde";
		system(instruction2.str().c_str());
	}
	for (int i=0; i< nLasers; i++){
			std::stringstream instruction1;
			instruction1 << "sort -n data/lasers/laser" << i+1 << "/laserData.jde >tempSORT.temp";
			system(instruction1.str().c_str());
			std::stringstream instruction2;
			instruction2 << "mv tempSORT.temp data/lasers/laser" << i+1 << "/laserData.jde";
			system(instruction2.str().c_str());
	}
	for (int i=0; i< nPose3dEncoders; i++){
			std::stringstream instruction1;
			instruction1 << "sort -n data/pose3dencoders/pose3dencoder" << i+1 << "/pose3dencoderData.jde >tempSORT.temp";
			system(instruction1.str().c_str());
			std::stringstream instruction2;
			instruction2 << "mv tempSORT.temp data/pose3dencoders/pose3dencoder" << i+1 << "/pose3dencoderData.jde";
			system(instruction2.str().c_str());
	}
	for (int i=0; i< nDepthSensors; i++){
			std::stringstream instruction1;
			instruction1 << "sort -n data/pointClouds/pointCloud" << i+1 << "/pointCloudData.jde >tempSORT.temp";
			system(instruction1.str().c_str());
			std::stringstream instruction2;
			instruction2 << "mv tempSORT.temp data/pointClouds/pointCloud" << i+1 << "/pointCloudData.jde";
			system(instruction2.str().c_str());
	}
	for (int i=0; i< nEncoders; i++){
			std::stringstream instruction1;
			instruction1 << "sort -n data/encoders/encoder" << i+1 << "/encoderData.jde >tempSORT.temp";
			system(instruction1.str().c_str());
			std::stringstream instruction2;
			instruction2 << "mv tempSORT.temp data/encoders/encoder" << i+1 << "/encoderData.jde";
			system(instruction2.str().c_str());
	}
	for (int i=0; i< nPose3d; i++){
			std::stringstream instruction1;
			instruction1 << "sort -n data/pose3d/pose3d" << i+1 << "/pose3dData.jde >tempSORT.temp";
			system(instruction1.str().c_str());
			std::stringstream instruction2;
			instruction2 << "mv tempSORT.temp data/pose3d/pose3d" << i+1 << "/pose3dData.jde";
			system(instruction2.str().c_str());
	}
   if (ic)
	   ic->destroy();
}


int main(int argc, char** argv){



	struct sigaction sigIntHandler;

   sigIntHandler.sa_handler = exitApplication;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = 0;

   sigaction(SIGINT, &sigIntHandler, NULL);

	int accion=0;
	int sentido=10;

	int status;

	struct timeval a, b, t2;
	int cycle = 100;
	long totalb,totala;
	long diff;

	// INTERFACE
	std::vector<jderobot::LaserPrx> lprx;
	std::vector<jderobot::CameraPrx> cprx;
	std::vector<jderobot::Pose3DEncodersPrx> pose3dencoders;
	std::vector<jderobot::Pose3DPrx> pose3dprx;
	std::vector<jderobot::EncodersPrx> encoders;
	std::vector <jderobot::pointCloudPrx> prx;


	//INTERFACE DATA
	jderobot::EncodersDataPtr ed;
	jderobot::LaserDataPtr ld;
	
	jderobot::ImageDataPtr imageData;

	//pools

	pthread_attr_t attr;
	//images
	std::vector<recorder::poolWriteImages*> poolImages;
	int nConsumidores;
	int poolSize;
	//lasers
	std::vector<recorder::poolWriteLasers*> poolLasers;
	//pose3dencoders
	std::vector<recorder::poolWritePose3dEncoders*> poolPose3dEncoders;
	//pose3d
	std::vector<recorder::poolWritePose3d*> poolPose3d;
	//encoders
	std::vector<recorder::poolWriteEncoders*> poolEncoders;
	//pointClouds
	std::vector<recorder::poolWritePointCloud*> poolPointClouds;

	//numero de lasers

	int Hz = 10;
	int muestrasLaser = 180;
	int pngCompressRatio;
	int jpgQuality;
	std::string imageFormat;
	std::vector<int> compression_params;
    
   
	//---------------- INPUT ARGUMENTS ---------------//

	if (argc<2){
		std::cout << std::endl << "USE: ./mycomponent --Ice.Config=mycomponent.cfg" << std::endl;
	}

	//---------------- INPUT ARGUMENTS -----------------//

	try{
		//creamos el directorio principal
		struct stat buf;
		char dire[]="./data/";
		if( stat( dire, &buf ) == -1 )
		{
			system("mkdir data");
		}
   
		Ice::PropertiesPtr prop;

		ic = Ice::initialize(argc,argv);

		prop = ic->getProperties();

		Hz = prop->getPropertyAsInt("Recorder.Hz");
		cycle = 1000.0/Hz;
      
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
      

		nCameras = prop->getPropertyAsIntWithDefault("Recorder.nCameras",0);
		if (nCameras > 0 ){
			struct stat buf;
			char dire[]="./data/images/";
			if( stat( dire, &buf ) == -1 )
			{
				system("mkdir ./data/images/");
			}
			imageFormat=prop->getProperty("Recorder.Format");
			if (imageFormat.compare(std::string("png"))==0){
				pngCompressRatio=prop->getPropertyAsIntWithDefault("Recorder.PngCompression",3);
				compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
				compression_params.push_back(pngCompressRatio);
			}
			else if (imageFormat.compare(std::string("jpg"))==0){
				jpgQuality=prop->getPropertyAsIntWithDefault("Recorder.JpgQuality",95);
				compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
				compression_params.push_back(jpgQuality);
			}
			else{
				throw "Image format is not valid";
			}
			nConsumidores=prop->getPropertyAsIntWithDefault("Recorder.nConsumers",2);
			poolSize=prop->getPropertyAsIntWithDefault("Recorder.poolSize",10);


		}

		for (int i=0; i< nCameras; i++){
			struct stat buf;
			std::stringstream cameraPath;
			cameraPath << "./data/images/camera" << i+1;
			if( stat( cameraPath.str().c_str(), &buf ) == -1 )
			{
				std::stringstream instruction;
				instruction << "mkdir " << cameraPath.str();
				system(instruction.str().c_str());
			}


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

			//pool
			recorder::poolWriteImages *temp = new recorder::poolWriteImages(cprxAux, Hz,poolSize,i+1,imageFormat,compression_params);
			poolImages.push_back(temp);


			for (int j=i*nConsumidores; j< i*nConsumidores+nConsumidores; j++){
				pthread_create(&consumerThreads[j], &attr, camera_pool_consumer_thread,temp);
				totalConsumers++;
			}
			pthread_create(&producerThreads[i], &attr, camera_pool_producer_thread,temp);
			totalProducers++;
		}



		nLasers= prop->getPropertyAsInt("Recorder.nLasers");
		if (nLasers > 0){
			struct stat buf;
			char dire[]="./data/lasers/";
			if( stat( dire, &buf ) == -1 )
			{
				system("mkdir data/lasers/");
			}
		}
		for (int i=0; i< nLasers; i++){
			struct stat buf;
			std::stringstream claserPath;
			claserPath << "./data/lasers/laser" << i+1;
			if( stat( claserPath.str().c_str(), &buf ) == -1 )
			{
				std::stringstream instruction;
				instruction << "mkdir " << claserPath.str();
				system(instruction.str().c_str());
			}

			// Contact to LASER interface
			std::stringstream sProxy;
			sProxy << "Recorder.Laser" << i+1 << ".Proxy";

			Ice::ObjectPrx baseLaser = ic->propertyToProxy(sProxy.str());
			if (0==baseLaser)
				throw "Could not create proxy with laser";

			// Cast to laser
			jderobot::LaserPrx laserPrx = jderobot::LaserPrx::checkedCast(baseLaser);
			if (0== laserPrx)
				throw "Invalid proxy Mycomponent.Laser.Proxy";
			lprx.push_back(laserPrx);
			recorder::poolWriteLasers *temp = new recorder::poolWriteLasers(laserPrx, Hz,poolSize,i+1);

			poolLasers.push_back(temp);
			pthread_create(&consumerThreads[totalConsumers], &attr, laser_pool_consumer_thread,temp);
			totalConsumers++;
			pthread_create(&producerThreads[totalProducers], &attr, laser_pool_producer_thread,temp);
			totalProducers++;
		}


		nPose3dEncoders= prop->getPropertyAsInt("Recorder.nPose3dEncoders");
		if (nPose3dEncoders > 0){
			struct stat buf;
			char dire[]="./data/pose3dencoders/";
			if( stat( dire, &buf ) == -1 )
			{
				system("mkdir data/pose3dencoders/");
			}
		}
		for (int i=0; i< nPose3dEncoders; i++){
			struct stat buf;
			std::stringstream claserPath;
			claserPath << "./data/pose3dencoders/pose3dencoder" << i+1;
			if( stat( claserPath.str().c_str(), &buf ) == -1 )
			{
				std::stringstream instruction;
				instruction << "mkdir " << claserPath.str();
				system(instruction.str().c_str());
			}

			// Contact to POSE3DENCODERS interface
			std::stringstream sProxy;
			sProxy << "Recorder.Pose3DEncoders" << i+1 << ".Proxy";

			Ice::ObjectPrx base = ic->propertyToProxy(sProxy.str());
			if (0==base)
				throw "Could not create proxy with pose3dencoders";

			// Cast to Pose3DEncodersPrx
			jderobot::Pose3DEncodersPrx prx = jderobot::Pose3DEncodersPrx::checkedCast(base);
			if (0== prx)
				throw "Invalid proxy Mycomponent.pose3dencoders.Proxy";
			pose3dencoders.push_back(prx);
			recorder::poolWritePose3dEncoders*temp = new recorder::poolWritePose3dEncoders(prx, Hz,poolSize,i+1);

			poolPose3dEncoders.push_back(temp);
			pthread_create(&consumerThreads[totalConsumers], &attr, pose3dencoders_pool_consumer_thread,temp);
			totalConsumers++;
			pthread_create(&producerThreads[totalProducers], &attr, pose3dencoders_pool_producer_thread,temp);
			totalProducers++;
		}

		nPose3d= prop->getPropertyAsInt("Recorder.nPose3d");
		if (nPose3d > 0){
			struct stat buf;
			char dire[]="./data/pose3d/";
			if( stat( dire, &buf ) == -1 )
			{
				system("mkdir data/pose3d/");
			}
		}
		for (int i=0; i< nPose3d; i++){
			struct stat buf;
			std::stringstream claserPath;
			claserPath << "./data/pose3d/pose3d" << i+1;
			if( stat( claserPath.str().c_str(), &buf ) == -1 )
			{
				std::stringstream instruction;
				instruction << "mkdir " << claserPath.str();
				system(instruction.str().c_str());
			}

			// Contact to POSE3D interface
			std::stringstream sProxy;
			sProxy << "Recorder.Pose3D" << i+1 << ".Proxy";

			Ice::ObjectPrx base = ic->propertyToProxy(sProxy.str());
			if (0==base)
				throw "Could not create proxy with pose3d";

			// Cast to Pose3DPrx
			jderobot::Pose3DPrx prx = jderobot::Pose3DPrx::checkedCast(base);
			if (0== prx)
				throw "Invalid proxy Mycomponent.pose3d.Proxy";
			pose3dprx.push_back(prx);
			recorder::poolWritePose3d*temp = new recorder::poolWritePose3d(prx, Hz,poolSize,i+1);

			poolPose3d.push_back(temp);
			pthread_create(&consumerThreads[totalConsumers], &attr, pose3d_pool_consumer_thread,temp);
			totalConsumers++;
			pthread_create(&producerThreads[totalProducers], &attr, pose3d_pool_producer_thread,temp);
			totalProducers++;
		}

		nEncoders= prop->getPropertyAsInt("Recorder.nEncoders");
		if (nEncoders > 0){
			struct stat buf;
			char dire[]="./data/encoders/";
			if( stat( dire, &buf ) == -1 )
			{
				system("mkdir data/encoders/");
			}
		}
		for (int i=0; i< nEncoders; i++){
			struct stat buf;
			std::stringstream claserPath;
			claserPath << "./data/encoders/encoder" << i+1;
			if( stat( claserPath.str().c_str(), &buf ) == -1 )
			{
				std::stringstream instruction;
				instruction << "mkdir " << claserPath.str();
				system(instruction.str().c_str());
			}

			// Contact to ENCODERS interface
			std::stringstream sProxy;
			sProxy << "Recorder.Encoders" << i+1 << ".Proxy";

			Ice::ObjectPrx base = ic->propertyToProxy(sProxy.str());
			if (0==base)
				throw "Could not create proxy with encoders";

			// Cast to EncodersPrx
			jderobot::EncodersPrx prx = jderobot::EncodersPrx::checkedCast(base);
			if (0== prx)
				throw "Invalid proxy Mycomponent.encoders.Proxy";
			encoders.push_back(prx);
			recorder::poolWriteEncoders*temp = new recorder::poolWriteEncoders(prx, Hz,poolSize,i+1);

			poolEncoders.push_back(temp);
			pthread_create(&consumerThreads[totalConsumers], &attr, encoders_pool_consumer_thread,temp);
			totalConsumers++;
			pthread_create(&producerThreads[totalProducers], &attr, encoders_pool_producer_thread,temp);
			totalProducers++;
		}

		nDepthSensors = prop->getPropertyAsIntWithDefault("Recorder.nDethSensors",0);
		if (nDepthSensors){
			struct stat buf;
			char dire[]="./data/pointClouds/";
			if( stat( dire, &buf ) == -1 )
			{
				system("mkdir data/pointClouds/");
			}
		}
		for (int i=0; i< nDepthSensors; i++){
			struct stat buf;
			std::stringstream claserPath;
			claserPath << "./data/pointClouds/pointCloud" << i+1;
			if( stat( claserPath.str().c_str(), &buf ) == -1 )
			{
				std::stringstream instruction;
				instruction << "mkdir " << claserPath.str();
				system(instruction.str().c_str());
			}


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
			prx.push_back(prxAux);
			recorder::poolWritePointCloud* temp = new recorder::poolWritePointCloud(prxAux, Hz,poolSize,i+1);
			poolPointClouds.push_back(temp);
			pthread_create(&consumerThreads[totalConsumers], &attr, pointcloud_pool_consumer_thread,temp);
			totalConsumers++;
			pthread_create(&producerThreads[totalProducers], &attr, pointcloud_pool_producer_thread,temp);
			totalProducers++;
		}

		//****************************** Processing the Control ******************************///


		//---------------- ITERATIONS CONTROL -----------//
		//muestreo para el laser
		muestrasLaser = prop->getPropertyAsInt("Recorder.Laser.Samples");
		std::string robotName = prop->getPropertyWithDefault("Recorder.Hostname","localhost");
		std::string robotPort = prop->getPropertyWithDefault("Recorder.Port","9999");
      
		long timeRelative = 0;


		int guiActive=prop->getPropertyAsIntWithDefault("Recorder.GUI",0);
		recorder::recordergui *gui;

		if (guiActive){
	      gui = new recorder::recordergui();
		}
		long long int iteration=0;
		
            
		while(globalActive){
			//gui activado
			if (guiActive){
				gui->set_iteration(iteration);
				gui->update();
				globalActive=gui->get_active();
				recording=gui->get_recording();
			}
			else{
				recording=true;
			}

			if (recording){
				if (iteration==0){
					gettimeofday(&inicio,NULL);
				}
				iteration++;
				gettimeofday(&b,NULL);
				totalb=b.tv_sec*1000000+b.tv_usec;
				//calculamos la velocidad de grabación actual
				float myfps=1000000./((float)totalb-(float)totala);
				if (guiActive){
					gui->set_fps((int)myfps);
				}
				gettimeofday(&a,NULL);
				totala=a.tv_sec*1000000+a.tv_usec;

				gettimeofday(&b,NULL);
				totalb=b.tv_sec*1000000+b.tv_usec;
				std::cout << "Recorder takes " << (totalb-totala)/1000 << " ms" << std::endl;

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
			else{
				usleep(10*1000);
			}

		}

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

	if (!killed)
		exitApplication(1);
   
   return 0;
}
