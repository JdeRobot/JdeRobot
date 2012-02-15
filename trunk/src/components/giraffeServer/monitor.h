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
#ifndef MONITOR_H
#define MONITOR_H

#include <JointMotor.h>
#include <Ice/Ice.h>
#include <QtCore>

class Monitor : public QThread
{
  Q_OBJECT
  
  public:
	Monitor( RoboCompJointMotor::MotorParamsList *params, RoboCompJointMotor::BusParams *busParams , Ice::CommunicatorPtr _communicator,
			 QWaitCondition *PARAMETERS_SET_WAIT_CONDITION);
	~Monitor();

	bool initialize( );
	QWaitCondition *PARAMETERS_SET_WAIT_CONDITION;

  public:
	void run();
	
  private:
	Ice::CommunicatorPtr communicator;
	bool configGetString( const std::string name, std::string &value,  const std::string default_value, QStringList *list = NULL);
	bool configGetInt( const std::string name, int &value, const int default_value, QList< int > *list = NULL);
	bool configGetBool (const std::string name, bool &value, const bool default_value);
	bool configGetFloat( const std::string name, float &value, const float default_value, QList< int > *list = NULL);
	RoboCompJointMotor::MotorParamsList *params;
	RoboCompJointMotor::BusParams *busParams;
	bool initializedOk;
};

#endif // MONITOR_H
