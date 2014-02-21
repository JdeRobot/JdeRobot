/*
 *    Copyright (C) 2010 by RoboLab - University of Extremadura
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SERVO_H
#define SERVO_H

#include <jderobot/jointmotor.h>
#include <QtCore>
#include <math.h>

#define ERR_POSITION_STEPS 15
#define RAW_DEGREES_RANGE 300.
//Total range in radians assuming equal motors
#define RAW_RADIANS_RANGE ( RAW_DEGREES_RANGE * M_PI / 180. )
#define RAW_STEPS_RANGE 1024.

class Servo 
{
  public:
	struct TMotorData
	{
		//std::string name;
		//uchar busId;
		//int maxPos;
		//int minPos;
		//int zeroPos;	
		int targetVelocity;
		int currentVelocity;
		int antPos;
		int currentPos;
		float currentPosRads;
		//int PWMFreqDiv;
		//int deadBand;
		int currentPower;
		//int P;            //Para el microcontrolador de Openservo 256 equivale a una ganancia de 1
		//int D;
		//int I;
		//bool reverseSeek;
		//int maxVelocity;
		//bool NORMAL_ANGLES;
		bool isMoving;
	};
	
  public:
	Servo( RoboCompJointMotor::MotorParams params );
	~Servo();

	TMotorData data;
	void readIniData();
	bool pendAct;      //Pendiente para actualizar posicion
	bool newCommand;   //Petición de movimiento
	void setPendiente(int pos);
	bool pendiente();
	float steps2Rads(int p);
	int rads2Steps(float r);
	int radsPerSec2Steps(float rs);
	
	RoboCompJointMotor::MotorParams params;
	
};

#endif // SERVO_H
