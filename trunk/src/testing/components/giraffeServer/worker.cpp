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
#include "worker.h"

Worker::Worker( RoboCompJointMotor::MotorParamsList *params, RoboCompJointMotor::BusParams *busParams,  QWaitCondition *PARAMETERS_SET_WAIT_CONDITION, QObject *parent) : QThread(parent)
{
  this->params = params;
  this->busParams = busParams;
  this->PARAMETERS_SET_WAIT_CONDITION = PARAMETERS_SET_WAIT_CONDITION;
  
/*  if (busParams->handler != "Fake")
  {
	connect(&timer, SIGNAL(timeout()), this, SLOT(compute()));
	if (busParams->handler != "Gazebo")
		timer.start(70);
	else
		timer.start(BASIC_PERIOD);
  }*/ 
  mutex = new QMutex();
  qDebug() << "JointMotor::Worker::Worker() - constructor Ok";
}

Worker::~Worker()
{

}

void Worker::setParams()
{
  //qDebug() << "JointMotor::Worker::setParams() with Handler = " << busParams->handler.c_str();

/*  if (busParams->handler == "Fake")
	handler = new FakeHandler();*/
  
 /* else*/ 
	if (busParams->handler == "Dynamixel")
	{
	  handler = new Dynamixel(busParams, params);
	  handler->tipo = "Dynamixel";
	  try
	  {
		handler->initialize();
	  }
	  catch(QString &s) 
	  { 
		throw s;
	  }
	}
  
//   else if (busParams->handler == "Megarobotics")
// 	handler = new MegaroboticsHandler(busParams->device);
//   
// #if COMPILE_GAZEBO==1
//   else if (busParams->handler == "Gazebo")
// 	handler = new GazeboHandler(busParams->device);
// #endif


}

void Worker::run( )
{
  //qDebug() << "Waiting for condition";
  PARAMETERS_SET_WAIT_CONDITION->wait(mutex);

  //qDebug() << "Setting Parameters ";
  try
  {
	setParams();
  }
  catch(QString & s) 
  { 
	std::cout << s.toStdString() << std::endl; 
	exit(1);
  }
	
  std::cout << "JointMotor::run() - Handler initialized correctly. Starting update loop." << std::endl;
  
  forever
  {
	try
	{
	  handler->update( mutex );
	}
	catch( QString &s)
	{
	  std::cout << "JointMotor::run() - Exception caught in handler->update() " << s.toStdString();
	}
	this->usleep(this->busParams->basicPeriod*1000);
  }
}

///
/// Servant methods
///

void Worker::setPosition( const RoboCompJointMotor::MotorGoalPosition & goalPosition )
{
 QString motorName = QString::fromStdString( goalPosition.name);
  if ( handler->motors.contains( motorName ) )
  {
	Servo *servo = handler->motors[motorName];
	try
	{
	  handler->setPosition( servo->params.busId , servo->rads2Steps( goalPosition.position));
	}
	catch( QString &s )
	{
	  std::string se = "JointMotorComp::Worker::" + s.toStdString();
	  std::cout << se;
	  RoboCompJointMotor::HardwareFailedException ex("Exception raised :"+se);
	  throw ex;
	}
  }
  else 
  {
	RoboCompJointMotor::UnknownMotorException ex(goalPosition.name);
	throw ex;
  }
}

void Worker::setVelocity( const RoboCompJointMotor::MotorGoalVelocity & goalVelocity )
{
  QString motorName = QString::fromStdString( goalVelocity.name);
  if ( handler->motors.contains( motorName ) )
  {
	Servo *servo = handler->motors[motorName];
	if( handler->setVelocity( servo->params.busId , servo->radsPerSec2Steps( goalVelocity.velocity)) == false )
	{
	  RoboCompJointMotor::HardwareFailedException ex(goalVelocity.name);
	  throw ex;
	}
  }
  else 
  {
	RoboCompJointMotor::UnknownMotorException ex(goalVelocity.name);
	throw ex;
  }
}

void Worker::setSyncPosition( const RoboCompJointMotor::MotorGoalPositionList & goalPosList)
{
  QVector<Handler::GoalPosition> hPositionList;
  
  for(uint i=0; i< goalPosList.size() ; i++)
  {
	QString motorName = QString::fromStdString( goalPosList[i].name);
	//qDebug() << motorName << goalPosList[i].position;
	if ( handler->motors.contains( motorName )  == false )
	{
	  RoboCompJointMotor::UnknownMotorException ex(goalPosList[i].name);
	  throw ex;  
	}
	else
	{
	  Servo *servo = handler->motors[motorName];
	  Handler::GoalPosition gp(motorName, servo->params.busId, servo->rads2Steps(goalPosList[i].position), servo->radsPerSec2Steps(goalPosList[i].maxSpeed));
	  hPositionList.append( gp );
	}
  }
  try
  {
	handler->setSyncPosition( hPositionList );
  }
  catch( QString &s)
  {
	std::string se = "JointMotorComp::Worker::" + s.toStdString();
	std::cout << se;
	RoboCompJointMotor::HardwareFailedException ex("Exception raised :" + se);
	throw ex;
  }
}
 
RoboCompJointMotor::BusParams Worker::getBusParams()
{
	return *busParams;
}

void Worker::getMotorParams( const QString & motor , RoboCompJointMotor::MotorParams & mp)
{
  if ( handler->motors.contains( motor ) )
  {
	mp = handler->motors[motor]->params;
  }
  else
  {
	RoboCompJointMotor::UnknownMotorException ex(motor.toStdString());
	throw ex;  
  }
}

RoboCompJointMotor::MotorParamsList Worker::getAllMotorParams( )
{
  RoboCompJointMotor::MotorParamsList list;
  
  foreach( Servo *s, handler->motors)
  {
	list.push_back( s->params );
  }
  return list;
}

RoboCompJointMotor::MotorStateMap Worker::getAllMotorState( )
{
  RoboCompJointMotor::MotorStateMap map;
  RoboCompJointMotor::MotorState state;

  //printf("devolviendo el estado de los motores\n");
  foreach( Servo *s, handler->motors)
  {
	state.pos = s->data.currentPosRads;
	state.v = s->data.currentVelocity;
	state.p = s->data.currentPos;
	state.isMoving = s->data.isMoving;
	map[s->params.name] = state ;
	qDebug()<<"motor"<<QString::fromStdString( s->params.name )<<"position"<<state.pos<<"(in steps)"<<state.p;	

  }
  //printf("hecho\n");
  return map;
}

void Worker::getState(const QString & motor, RoboCompJointMotor::MotorState & state)
{
  if ( handler->motors.contains( motor ) )
  {
	state.pos = handler->motors[motor]->data.currentPosRads;
	state.v = handler->motors[motor]->data.currentVelocity;
	state.p = handler->motors[motor]->data.currentPos;
  }
  else 
  {
	RoboCompJointMotor::UnknownMotorException ex(motor.toStdString());
	throw ex;
  }
}