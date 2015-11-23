#ifndef SENSORS_H
#define SENSORS_H
//ICE
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
//QT
#include <QtGui>
//INTERFACES
#include <jderobot/camera.h>
#include <jderobot/pose3d.h>
#include <jderobot/navdata.h>
#include <jderobot/cmdvel.h>
#include <jderobot/ardroneextra.h>


#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define LINX 0.3
#define LINY 0.3
#define LINZ 0.8
#define ANGZ 1.0
#define ANGY 0.0
#define ANGX 0.0
#define IMAGE_COLS_MAX 640
#define IMAGE_ROWS_MAX 360
#define ARDRONE1 0
#define ARDRONE2 1
#define ARDRONESIM 10

class Sensors
{
	public:
		Sensors(Ice::CommunicatorPtr ic);
		virtual ~Sensors();
		cv::Mat getImage();

		void update();

		jderobot::NavdataDataPtr getNavdata();	
		jderobot::Pose3DDataPtr getPose3DData();
		void takeOff();
		void land();
		void reset();
		void toggleCam();
		void sendVelocitiesToUAV(float vx,float vy,float vz,float roll,float pitch,float yaw);

	private:
		QMutex mutex;
		QMutex mutexDrone;
		cv::Mat image;
        jderobot::NavdataDataPtr navdata;
        jderobot::Pose3DDataPtr pose3DDataPtr;
		Ice::CommunicatorPtr ic;
		jderobot::CameraPrx cprx;
		jderobot::NavdataPrx navprx;	
		jderobot::CMDVelPrx cmdprx;
		jderobot::Pose3DPrx poseprx;	
        jderobot::ArDroneExtraPrx arextraprx;
		bool tracking;
		bool flying;	
		bool rst; 

};

#endif // SENSORS_H
