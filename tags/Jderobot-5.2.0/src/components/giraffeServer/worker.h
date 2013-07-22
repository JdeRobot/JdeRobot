/*
 *    Copyright (C) 2009-2010 by RoboLab - University of Extremadura
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
#ifndef WORKER_H
#define WORKER_H

// #include <ipp.h>
#include <QtCore>
#include "handler.h"
#include "dynamixel.h"
//#include "fakehandler.h"
#include <JointMotor.h>
//#include <megaroboticshandler.h>
#include <servo.h>

#define CHECK_PERIOD 5000
#define BASIC_PERIOD 50

/**
       \brief
       @author Robolab
*/

class Worker : public QThread
{
  Q_OBJECT
  public:
	Worker( RoboCompJointMotor::MotorParamsList *params, RoboCompJointMotor::BusParams  *busParams, QWaitCondition *PARAMETERS_SET_WAIT_CONDITION, 
			QObject *parent = 0);
	~Worker();
	QMutex *mutex;                //Shared mutex with servant
	QMutex mutexMon; 
	void setParams( );
	QWaitCondition *PARAMETERS_SET_WAIT_CONDITION;
	
	void getState(const QString & motor, RoboCompJointMotor::MotorState & state);
	void setPosition( const RoboCompJointMotor::MotorGoalPosition & goalPosition);
	void setVelocity( const RoboCompJointMotor::MotorGoalVelocity & goalVelocity);
	void setSyncPosition( const RoboCompJointMotor::MotorGoalPositionList & goalPosList );
	RoboCompJointMotor::BusParams getBusParams();
	void getMotorParams( const QString & motor, RoboCompJointMotor::MotorParams & mp);
	RoboCompJointMotor::MotorParamsList getAllMotorParams();
	RoboCompJointMotor::MotorStateMap getAllMotorState();
	void run();
	
  private:
	QTimer timer;
	RoboCompJointMotor::MotorParamsList *params;
	RoboCompJointMotor::BusParams *busParams;
	Handler *handler;

  public slots:
	 
};

#endif
