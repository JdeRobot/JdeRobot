/*
 *  Copyright (C) 2014 JdeRobot developers
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
 *  Authors : Roberto Calvo Palomino <rocapal [at] gsyc [dot] urjc [dot] es>
 *
 */

#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include "easyiceconfig/EasyIce.h" 

#include "pthread.h"
#include <signal.h>
#include <logger/Logger.h>

#include "NamingServiceJdeRobot.h"

#include <pthread.h>

int status,i;
Ice::CommunicatorPtr ic;
int n_components=0;
pthread_t thread;
pthread_attr_t attr;
Ice::PropertiesPtr prop;
Ice::ObjectAdapterPtr adapter;

NamingService::NamingServiceJdeRobot* namingservice_prx;

void exitComponent(int s)
{
	ic->shutdown();
}

int main(int argc, char** argv){

	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = exitComponent;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);


	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	try{
		ic = EasyIce::initialize(argc,argv);
		prop = ic->getProperties();
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		return 1;
	}
	catch (const char* msg) {
		jderobot::Logger::getInstance()->error(msg);
		return 1;
	}


	try{

		std::string prefixComponent("NamingService");

		// Analyze LOG section

		std::string logFile = prop->getProperty(prefixComponent + ".Log.File.Name");
		if (logFile.size()==0)
			jderobot::Logger::getInstance()->warning("You didn't set log file!");
		else
			jderobot::Logger::getInstance()->setFileLog(logFile);

		std::string logLevel = prop->getProperty(prefixComponent + ".Log.File.Level");
		if (logLevel.size()==0)
			jderobot::Logger::getInstance()->warning("You didn't set *.Log.File.Level key!");
		else
			jderobot::Logger::getInstance()->setFileLevel(jderobot::Levels(boost::lexical_cast<int>(logLevel)));

		std::string screenLevel = prop->getProperty(prefixComponent + ".Log.Screen.Level");
		if (screenLevel.size()==0)
			jderobot::Logger::getInstance()->warning("You didn't set *.Log.Screen.Level key!");
		else
			jderobot::Logger::getInstance()->setScreenLevel(jderobot::Levels(boost::lexical_cast<int>(screenLevel)));

		jderobot::Logger::getInstance()->info("Logger:: screenLevel=" + screenLevel + " logLevel=" + logLevel + " LogFile=" + logFile);




		// Analyze EndPoint
		std::string Endpoints = prop->getProperty(prefixComponent + ".Endpoints");
		adapter =ic->createObjectAdapterWithEndpoints(prefixComponent, Endpoints);

		int communicator_active =prop->getPropertyAsIntWithDefault(prefixComponent + ".Active",0);
		if (communicator_active){

			std::string Name = prop->getProperty(prefixComponent + ".Name");
			jderobot::Logger::getInstance()->info("Creating Communicator: " + Name + " (" + Endpoints + ")");
			std::string objPrefix(prefixComponent);
			namingservice_prx = new NamingService::NamingServiceJdeRobot (objPrefix);

			adapter->add(namingservice_prx, ic->stringToIdentity(Name));
		}
		else
		{
			jderobot::Logger::getInstance()->warning("No NamingService Interface declared!");
			exit(-1);
		}

		adapter->activate();

		ic->waitForShutdown();
		adapter->destroy();

	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		return 1;
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
		return 1;
	}

	jderobot::Logger::getInstance()->info("Component finished correctly!");

	return 0;

}
