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
#include "servo.h"

Servo::Servo( RoboCompJointMotor::MotorParams params )
{
  this->params = params;
  //data.name = params.name;
  //data.busId = (uchar)params.busId;
  //data.maxPos = params.maxPos;
  //data.minPos = params.minPos;
  //data.zeroPos = params.zeroPos;	
  data.targetVelocity = 0;
  data.currentVelocity = 0;
  data.antPos = 0;
  data.currentPos = 0;
  data.currentPosRads = 0;
  //data.deadBand = 0;
  data.currentPower = 0;
  //data.P = 0;
  //data.D = 0;
  //data.I = 0;
  //data.maxVelocity = params.maxVelocity;
  //data.NORMAL_ANGLES = params.normalAngles;
  data.isMoving = false;
  pendAct = true;
  
}

Servo::~Servo()
{}

void Servo::setPendiente(int pos)
{
	this->pendAct = true;
	if (fabs(pos - data.currentPos)>ERR_POSITION_STEPS)
		this->newCommand=true;
	else
		this->newCommand=false;
}

bool Servo::pendiente()
{ 
  return pendAct; 
}

float Servo::steps2Rads(int p)
{
	if (  params.invertedSign )
		 return (p-params.zeroPos) * (RAW_RADIANS_RANGE/RAW_STEPS_RANGE);
	else
		return (p-params.zeroPos) * (-RAW_RADIANS_RANGE/RAW_STEPS_RANGE);
	
}

int Servo::rads2Steps(float r)
{
	if (  params.invertedSign)
		return( (int)rint((RAW_STEPS_RANGE / RAW_RADIANS_RANGE) * r + params.zeroPos)) ;
	else 
		return( (int)rint(-(RAW_STEPS_RANGE / RAW_RADIANS_RANGE) * r + params.zeroPos)) ;
}

 int Servo::radsPerSec2Steps(float rs)
 {
	//Max speed
	return 0;
 }