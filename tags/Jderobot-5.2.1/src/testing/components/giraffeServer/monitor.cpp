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
#include "monitor.h"

Monitor::Monitor( RoboCompJointMotor::MotorParamsList *params, RoboCompJointMotor::BusParams *busParams ,  Ice::CommunicatorPtr _communicator, 
				  QWaitCondition * PARAMETERS_SET_WAIT_CONDITION   )
{
  this->communicator = _communicator;
  this->params = params;
  this->busParams = busParams;
  this->PARAMETERS_SET_WAIT_CONDITION = PARAMETERS_SET_WAIT_CONDITION;
  initializedOk= false;

}

Monitor::~Monitor()
{

}

void Monitor::run()
{
  forever
  {
	if ( initializedOk == false)
	{
	  if (initialize( ) == true)
	  {
		initializedOk = true;
		PARAMETERS_SET_WAIT_CONDITION->wakeAll();
	  }
	  else 
	  {
		std::cout << "JointMotor::Monitor::run - Configuration failure. Exiting";
		this->exit();
	  }
	}
	else
	  this->sleep(500);
  }
}

/**
 * \brief Reads components parameters and checks set integrity before signaling the Worker thread to start runningç
 * There can be four (4) types of parameteres:
 *		(1) Ice parameters
 *		(2) Nexus (configuration) parameters	
 *		(3) Local component parameters read at start
 *		(4) Local parameters read from other running component
 *
 */
bool Monitor::initialize( )
{
  
//    Ice::PropertiesPtr props = communicator->getProperties();
//    std::string name = props->getProperty("Ice.Warn.Connections");
 	
  ///Local Component parameters read at start
	
	std::cout << "JointMotor::Monitor::Initialize ..." << std::endl;
	
  /// Reading parameters from config file or passed in command line, with Ice machinery
  /// We need to supply a list of accepted values to each call
	configGetInt( "JointMotor.NumMotors", busParams->numMotors, 0 );
	if (busParams->numMotors <= 0) 
	  qFatal("Monitor::initialize - Zero motors found. Exiting..." );
	configGetString( "JointMotor.Device", busParams->device, "" );  
	configGetString( "JointMotor.Handler", busParams->handler, "" );  
	configGetInt( "JointMotor.BaudRate", busParams->baudRate, 115200);  
	configGetInt( "JointMotor.BasicPeriod", busParams->basicPeriod, 100);  
	
	std::string paramsStr;
	for (int i=0; i<busParams->numMotors; i++)
	{
	  std::string s= QString::number(i).toStdString();
	  configGetString( "JointMotor.Params_" + s, paramsStr , "");  
	  QStringList list = QString::fromStdString(paramsStr).split(",");
	  if (list.size() != 7) qFatal("Error reading motor %d", i);
	  RoboCompJointMotor::MotorParams mpar;
	  mpar.name = list[0].toStdString();
	  mpar.busId = (uchar)list[1].toUShort();
	  mpar.invertedSign = list[2].contains("true");
	  mpar.minPos = list[3].toFloat();
	  mpar.maxPos = list[4].toFloat();
	  mpar.zeroPos = list[5].toFloat();
	  mpar.maxVelocity = list[6].toFloat();
	  params->push_back(mpar);
	}
	//Analize parameters now
	
	//Check if device exits
	if (QFile::exists(QString::fromStdString(busParams->device)) == false)
	{
	  std::cout << "Device " << busParams->device << " does not exits. Please check if required hardware is installed" << std::endl;
	  return false;
	}
	  
  return true;
}

//Ice Methods to read from file

bool Monitor::configGetString( const std::string name, std::string &value,  const std::string default_value, QStringList *list)
	{
		value = communicator->getProperties()->getProperty( name );
		if ( value.length() == 0)
		{
			value = default_value;
			return true;
		}
		if(list != NULL)
		{
			if (list->contains(QString::fromStdString(value)) == false)
			{
				qFatal("Reading config file: %s is not a valid string", name.c_str());
			}
		}
		std::cout << name << " " << value << std::endl;
		return true;
	}

bool Monitor::configGetInt( const std::string name, int & value, const int default_value, QList< int > *list )
{
	std::string tmp;

	tmp = this->communicator->getProperties()->getProperty( name );
	if ( tmp.length() == 0)
	{
		value = default_value;
		return true;
	}
	value = std::atoi( tmp.c_str() );
	if(list != NULL)
	{
		if (list->contains(value) == false)
		{
			qFatal("Reading config file: %s is not a valid integer", name.c_str());
		}
	}
	std::cout << name << " " << value << std::endl;
	return true;
}

bool Monitor::configGetBool (const std::string name, bool &value, const bool default_value)
{
	std::string tmp;
	tmp = communicator->getProperties()->getProperty( name );
	if ( tmp.length() == 0 )
	{
		value = default_value;
		return true;
	}
	if(tmp == "true") value = true;
	else 
	if(tmp == "false") value = false;
	else qFatal("Reading config file: '%s' is not a valid boolean", name.c_str());
	std::cout << name << " " << value << std::endl;
	return true;
}

bool Monitor::configGetFloat( const std::string name, float &value, const float default_value, QList< int > *list )
{
	std::string tmp;

	tmp = communicator->getProperties()->getProperty( name );
	if ( tmp.length() == 0)
	{
		value = default_value;
		return true;
	}
	value = std::atof( tmp.c_str() );
	if(list != NULL)
	{
		if (list->contains(value) == false)
		{
			qFatal("Reading config file: '%s' is not a valid float", name.c_str());
		}
	}
	std::cout << name << " " << value << std::endl;
	return true;
}