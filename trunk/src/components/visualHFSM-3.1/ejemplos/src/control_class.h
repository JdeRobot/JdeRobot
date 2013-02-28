#ifndef MYCOMPONENT_CONTROL_H
#define MYCOMPONENT_CONTROL_H


#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <jderobot/motors.h>
#include <jderobot/ptmotors.h>
#include <jderobot/laser.h>
#include <jderobot/encoders.h>
#include <jderobot/ptencoders.h>
#include <colorspaces/colorspacesmm.h>
#include <pthread.h>


namespace mycomponent {
		class Gui;
		class Control {
		public:

		virtual ~Control();

			//FUNCTIONS
			void handleCameras();
			void resetControl();
			//HANDLE THREADS
			pthread_t thr_gui;
			pthread_t thr_sub_1;
			pthread_t thr_sub_2;
			pthread_t thr_sub_3;
			pthread_mutex_t controlGui;

			// INTERFACES DATA
			jderobot::EncodersDataPtr ed;
			jderobot::LaserDataPtr ld;
			jderobot::ImageDataPtr data1; // Contains the image info
			jderobot::ImageDataPtr data2; // Contains the image info
			jderobot::PTEncodersDataPtr pted1;
			jderobot::PTEncodersDataPtr pted2;

			// INTERFACES
			jderobot::MotorsPrx mprx;
			jderobot::EncodersPrx eprx;
			jderobot::LaserPrx lprx;
			jderobot::CameraPrx cprx1;
			jderobot::CameraPrx cprx2;
			jderobot::PTMotorsPrx ptmprx1;
			jderobot::PTEncodersPrx pteprx1;
			jderobot::PTMotorsPrx ptmprx2;
			jderobot::PTEncodersPrx pteprx2;

			colorspaces::Image* image1;	// Prepare the image to use with openCV
			colorspaces::Image* image2;

		};//class
	} // namespace
#endif /*MYCOMPONENT_Control_H*/
