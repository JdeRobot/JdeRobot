/*
 *  Copyright (C) 1997-2010 JDE Developers Team
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
 *  Authors : Jose María Cañas <jmplaza@gsyc.es>
 *            Eduardo Perdices <eperdes@gsyc.es>
 *            Francisco Miguel Rivas Montero <fm.rivas@alumnos.urjc.es>	
 */

/** \file bodyencodersAdapter.cpp
 * \brief bodyencodersAdapter definitios
 */

#include "bodyencodersAdapter.h"
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_multifit.h>

const float PI=3.1415926;


namespace NaoAdapter{ 
	/**
	* \brief bodyencodersAdapter class constructor
	* \param IP the ip of the robot
	* \param port the port of the robot
	*/
	bodyencoders::bodyencoders(char* IP, int port) {
		this->IP=IP;
		this->PORT = port;
	}

	/**
	* \brief Funtion that create the proxy to the robot ALMotion.
	* \return 0 if ok, otherwise -1
	*/
	int bodyencoders::init() {

			try {
				std::cout << "Trying to conect to bodyencoders in " << this->IP << ":" << this->PORT << std::endl;
				this->bodyencodersProxy = new AL::ALMotionProxy(this->IP, this->PORT);
			} catch(AL::ALError& e) {
				std::cerr << "NaoBody: exception connecting to NaoQi: "<<e.toString()<< std::endl;
				return -1;
			}
			this->bodyencodersProxy->stiffnessInterpolation("Body",1.0,1.0);
			this->leftLegClock=0;
			this->leftArmClock=0;
			this->rightLegClock=0;
			this->rightArmClock=0;
			return 0;	
	}
	/**
	* \brief bodyencodersAdapter class destructor
	*/
	void bodyencoders::terminate() {
		delete this->bodyencodersProxy;
	}

	/**
	* \brief Funtion that gets the arm position
	* \param arm a local reference to alocate the arm position
	* \param side param that defines the side of the motor (left-right)
	* \return 0 if ok, otherwise -1
	*/
	int bodyencoders::getArmPosition(jderobot::ArmEncodersDataPtr arm, jderobot::BodySide side){
		//std::cout << bodyencodersProxy->getSummary() << std::endl;

		std::vector<std::string> nombres;
		std::vector<std::string>::iterator e;
		AL::ALValue nombres_ok;
		AL::ALValue angles;
	
		nombres.push_back("ShoulderRoll");
		nombres.push_back("ShoulderPitch");
		nombres.push_back("ElbowYaw");
		nombres.push_back("ElbowRoll");

		if (side==jderobot::Left){
			for (e=nombres.begin(); e!=nombres.end(); e++){
				nombres_ok.arrayPush("L"+(*e));
			}
		}
		else{
			for (e=nombres.begin(); e!=nombres.end(); e++){
				nombres_ok.arrayPush("R"+(*e));
			}
		}
		try{
			angles=bodyencodersProxy->getAngles(nombres_ok,true);
		} catch(AL::ALError& e) {
				std::cerr << "NaoBody: exception while getting arm angles: "<<e.toString()<< std::endl;
				return -1;
		}
		arm->shoulder.roll=(float)angles[0]*180/PI;
		arm->shoulder.pitch=(float)angles[1]*180/PI;
		arm->elbow.yaw=(float)angles[2]*180/PI;
		arm->elbow.roll=(float)angles[3]*180/PI;
		if (side==jderobot::Left){
			if ((arm->shoulder != leftShoulder) || (arm->elbow != leftElbow)){
				leftShoulder=arm->shoulder;
				leftElbow=arm->elbow;
				if (leftArmClock > 9999999)
					leftArmClock=0;
				else
					leftArmClock++;
			}
			arm->clock=leftArmClock;
		}
		else{
			if ((arm->shoulder != rightShoulder) || (arm->elbow != rightElbow)){
				rightShoulder=arm->shoulder;
				rightElbow=arm->elbow;
				if (rightArmClock > 9999999)
					rightArmClock=0;
				else
					rightArmClock++;
			}
			arm->clock=rightArmClock;
		}
		return 0;
	}


	/**
	* \brief Funtion that gets the arm position
	* \param leg a local reference to alocate the leg position
	* \param side param that defines the side of the motor (left-right)
	* \return 0 if ok, otherwise -1
	*/
	int bodyencoders::getLegPosition(jderobot::LegEncodersDataPtr leg, jderobot::BodySide side){
		//std::cout << bodyencodersProxy->getSummary() << std::endl;

		std::vector<std::string> nombres;
		std::vector<std::string>::iterator e;
		AL::ALValue nombres_ok;
		AL::ALValue angles;
	
		nombres.push_back("HipYawPitch");
		nombres.push_back("HipRoll");
		nombres.push_back("HipPitch");
		nombres.push_back("KneePitch");
		nombres.push_back("AnklePitch");
		nombres.push_back("AnkleRoll");

		if (side==jderobot::Left){
			for (e=nombres.begin(); e!=nombres.end(); e++){
				nombres_ok.arrayPush("L"+(*e));
			}
		}
		else{
			for (e=nombres.begin(); e!=nombres.end(); e++){
				nombres_ok.arrayPush("R"+(*e));
			}
		}
		try{
			angles=bodyencodersProxy->getAngles(nombres_ok,true);
		} catch(AL::ALError& e) {
				std::cerr << "NaoBody: exception while getting leg angles: "<<e.toString()<< std::endl;
				return -1;
		}
		leg->hip.yaw=(float)angles[0]*180/PI;
		leg->hip.roll=(float)angles[1]*180/PI;
		leg->hip.pitch=(float)angles[2]*180/PI;
		leg->knee.pitch=(float)angles[3]*180/PI;
		leg->ankle.pitch=(float)angles[4]*180/PI;
		leg->ankle.roll=(float)angles[5]*180/PI;
		if (side==jderobot::Left){
			if ((leftHip != leg->hip) || (leftKnee != leg->knee) || (leftAnkle != leg->ankle)){
				leftHip=leg->hip;
				leftKnee=leg->knee;
				leftAnkle=leg->ankle;
				if (leftLegClock > 9999999)
					leftLegClock=0;
				else
					leftLegClock++;
			}
			leg->clock=leftLegClock;
		}
		else{
			if ((rightHip != leg->hip) || (rightKnee != leg->knee) || (rightAnkle != leg->ankle)){
				rightHip=leg->hip;
				rightKnee=leg->knee;
				rightAnkle=leg->ankle;
				if (rightLegClock > 9999999)
					rightLegClock=0;
				else
					rightLegClock++;
			}
			leg->clock=rightLegClock;
		}
		return 0;
	}

	/**
	* \brief Funtion that calulaes the head position of the robot
	* \param camera defines which camera is in use
	* \return a 9 position vector with the robot position: x, y, z, pan, tilt, roll, foax, foay, foaz 
	*/
	std::vector<float> bodyencoders::getOdometry(jderobot::CameraBody camera) {

		gsl_matrix * RTneck, * RTdiff, * RTcam, * foarel, * foa;
		std::vector<float> neckpos;
		std::vector<float> neckRT;
		std::vector<float> footpos;
		float yangle = -40.0*PI/180.0;
		int support;
		std::vector<float> odometry;

		RTneck = gsl_matrix_alloc(3,4);
		RTdiff = gsl_matrix_alloc(4,4);
		RTcam = gsl_matrix_alloc(3,4);
		foarel = gsl_matrix_alloc(4,1);
		foa = gsl_matrix_alloc(3,1);

		/*Obtetemos la posición y orientación del cuello*/
		neckpos = this->bodyencodersProxy->getPosition("Head", 2,true);

		/*Obtenemos la transformada homogenea, que nos permite calcular la traslación desde el cuello a la cámara teniendo en cuenta la inclinación de la cámara*/
		neckRT = this->bodyencodersProxy->getTransform ("Head", 2,true);

		/*RT del cuello*/
		gsl_matrix_set(RTneck,0,0,neckRT[0]);
		gsl_matrix_set(RTneck,0,1,neckRT[1]);
		gsl_matrix_set(RTneck,0,2,neckRT[2]);
		gsl_matrix_set(RTneck,0,3,neckRT[3]);
		gsl_matrix_set(RTneck,1,0,neckRT[4]);
		gsl_matrix_set(RTneck,1,1,neckRT[5]);
		gsl_matrix_set(RTneck,1,2,neckRT[6]);
		gsl_matrix_set(RTneck,1,3,neckRT[7]);
		gsl_matrix_set(RTneck,2,0,neckRT[8]);
		gsl_matrix_set(RTneck,2,1,neckRT[9]);
		gsl_matrix_set(RTneck,2,2,neckRT[10]);
		gsl_matrix_set(RTneck,2,3,neckRT[11]);

		/*RT de la camara respecto al cuello*/
		if(camera==jderobot::Top) {	/*Top camera*/
			gsl_matrix_set(RTdiff,0,0,cos(0.0));
			gsl_matrix_set(RTdiff,0,1,0.0);
			gsl_matrix_set(RTdiff,0,2,-sin(0.0));
			gsl_matrix_set(RTdiff,0,3,0.0539);				/*Profundidad*/
			gsl_matrix_set(RTdiff,1,0,0.0);
			gsl_matrix_set(RTdiff,1,1,1.0);
			gsl_matrix_set(RTdiff,1,2,0.0);
			gsl_matrix_set(RTdiff,1,3,0.0);			/*Lateral*/
			gsl_matrix_set(RTdiff,2,0,sin(0.0));
			gsl_matrix_set(RTdiff,2,1,0.0);
			gsl_matrix_set(RTdiff,2,2,cos(0.0));
			gsl_matrix_set(RTdiff,2,3,0.0679);				/*Altura*/
			gsl_matrix_set(RTdiff,3,0,0.0);
			gsl_matrix_set(RTdiff,3,1,0.0);
			gsl_matrix_set(RTdiff,3,2,0.0);
			gsl_matrix_set(RTdiff,3,3,1.0);	
		} else {	/*Bottom camera*/
			gsl_matrix_set(RTdiff,0,0,cos(yangle));
			gsl_matrix_set(RTdiff,0,1,0.0);
			gsl_matrix_set(RTdiff,0,2,-sin(yangle));
			gsl_matrix_set(RTdiff,0,3,0.0488);				/*Profundidad*/
			gsl_matrix_set(RTdiff,1,0,0.0);
			gsl_matrix_set(RTdiff,1,1,1.0);
			gsl_matrix_set(RTdiff,1,2,0.0);
			gsl_matrix_set(RTdiff,1,3,0.0);			/*Lateral*/
			gsl_matrix_set(RTdiff,2,0,sin(yangle));
			gsl_matrix_set(RTdiff,2,1,0.0);
			gsl_matrix_set(RTdiff,2,2,cos(yangle));
			gsl_matrix_set(RTdiff,2,3,0.02381);				/*Altura*/
			gsl_matrix_set(RTdiff,3,0,0.0);
			gsl_matrix_set(RTdiff,3,1,0.0);
			gsl_matrix_set(RTdiff,3,2,0.0);
			gsl_matrix_set(RTdiff,3,3,1.0);	
		}

		/*RT de la camara*/
		gsl_linalg_matmult(RTneck,RTdiff,RTcam);

		/*Foa en relativas, mirando 1 metro hacia adelante*/
		gsl_matrix_set(foarel,0,0,1.0);
		gsl_matrix_set(foarel,1,0,0.0);
		gsl_matrix_set(foarel,2,0,0.0);
		gsl_matrix_set(foarel,3,0,1.0);

		/*Foa en absolutas*/
		gsl_linalg_matmult(RTcam,foarel,foa);

		/*Guardamos la odometría*/
		odometry[0] = (float)gsl_matrix_get(RTcam,0,3);		/*X*/
		odometry[1] = (float)gsl_matrix_get(RTcam,1,3);		/*Y*/
		odometry[2] = (float)gsl_matrix_get(RTcam,2,3);		/*Z*/
		odometry[3] = -neckpos[5];							/*Pan*/
		if(camera==jderobot::Top)
			odometry[4] = -neckpos[4];							/*Tilt*/
		else
			odometry[4] = -(neckpos[4] - yangle);				/*Tilt*/
		odometry[5] = neckpos[3];							/*Roll*/
		odometry[6] = (float)gsl_matrix_get(foa,0,0);		/*FoaX*/
		odometry[7] = (float)gsl_matrix_get(foa,1,0);		/*FoaY*/
		odometry[8] = (float)gsl_matrix_get(foa,2,0);		/*FoaZ*/

		gsl_matrix_free(RTneck);
		gsl_matrix_free(RTdiff);
		gsl_matrix_free(RTcam);
		gsl_matrix_free(foarel);
		gsl_matrix_free(foa);
		return odometry;
	}

	
}
