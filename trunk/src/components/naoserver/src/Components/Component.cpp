/*
 * Name: Component.cpp
 * @Author: Carlos Agüero (caguero@gsyc.es)
 *
 * Description: Abstract class that defines some data structures and methods contained
 * in every component. It has an abstract method step() that should implement each
 * specific component.
 *
 * All components work in the same way and defines how BICA architecture operates. A
 * component is executed in a iterative way calling its step() method. In order to not
 * overhead the system, a call to step() could be ignored by the component itself. Every
 * component should specify a minimum amount of time between several step() calls. If
 * the elapsed time between the last real step() and the current time is not enough,
 * the call will be ignored.
 *
 * Another important feature is that components are designed as singletons. So, there is
 * only one instance of every component present in the system. In order to use the behavior
 * of a component, a client should declare a pointer to the component and calls to
 * getInstance() method to initialize it. After that, the client can use the component as
 * it has been created with the usual new().
 *
 * The component has also some debug utilities to calculate the cycle time consumption,
 * the real frequency operation, etc.
 *
 * The common use of a component is for getting some information (position of the ball,
 * position of the blueNet, etc.) or for unleash some behavior (turn left the head, walk
 * straight, ...). In order to get information a client should call step() and the ask for
 * the information. In order to unleash some behavior a client should modulate the component
 * with some of its method and then call step().
 *
 * Created on: 2009
 *
 * Copyright (C) Universidad Rey Juan Carlos
 * All Rights Reserved.
 *
 */

#include "Component.h"
#include "limits.h"
/**
 * Class constructor that initializes the controller with a default minimum frequency time.
 */
Component::Component()
{
	this->setFreqTime(DEFAULT_FREQ_TIME);
	beginStep = getCurrentTime();
	iters = meanCycle = realfreq = mcycle = Mtcycle = 0;
	mtcycle = LONG_MAX;
	fdlog = NULL;
}

/**
 * Class destructor.
 **/
Component::~Component()
{
	fclose(fdlog);
}

void
Component::initLog(const string filename)
{
	fdlog = fopen((string("/tmp/") + filename+string(".log")).c_str(), "w");
}

/**
 * writeLog. Method that writes a new message in the file log.
 * @param data New message to be written in the file log.
 **/
void
Component::writeLog(const string data)
{
	if(fdlog != NULL)
	{
		fprintf(fdlog, "%s", data.c_str());
		fflush(fdlog);
	}
	else
		cerr << "[Component (" << name << ")::writeLog()] Log no iniciado\n";
}

/**
 * init. Method that stores the parentBroker.
 * @param parentBroker Parent broker of the component.
 **/
void
Component::init(const string newName, const AL::ALPtr<AL::ALBroker> parentBroker)
{
	this->parentBroker 	= parentBroker;
	name = newName;
	initLog(name);
	//Register::getInstance()->regist(name, this);
}

/**
 * getParentBroker. Method that returns the parentBroker.
 **/
AL::ALPtr<AL::ALBroker>
Component::getParentBroker(void)
{
	return this->parentBroker;
}

/**
 * startDebugInfo. Method that starts the timing debug information. There are two kinds
 * of parameteres measured: cycle time and iterations between an specific INFO_TIME interval.
 **/
void
Component::startDebugInfo()
{
	return;
	beginDebugCycle = getCurrentTime();
	if (iters == 0)
		beginDebugStep = beginDebugCycle;
}

/**
 * endDebugInfo. Method that calculate the cycle time and if it is time to show the real
 * frequency of the component (if the elapsed time is greater than INFO_TIME).
 **/
void
Component::endDebugInfo()
{
	return;
	ostringstream s;

	long now = getCurrentTime();
	long cycleTime = now - beginDebugCycle;
	s << cycleTime << endl;
	meanCycle += cycleTime;

	if(cycleTime > Mtcycle)
		Mtcycle = cycleTime;
	if(cycleTime < mtcycle)
		mtcycle = cycleTime;

	iters++;

	long dif = now - beginDebugStep;

	if (dif >= INFO_TIME){

		realfreq = iters / ((double)(dif / 1000000));
		mcycle = (meanCycle / iters) / 1000.0;

		s << "[" << name << "] Running at " << iters / ((double)(dif / 1000000)) <<
				"Hz with mean cycle time of " << (meanCycle / iters) / 1000.0 << " ms.\n";
		iters = 0;
		meanCycle = 0;
	}

	writeLog(s.str());
}

long
Component::getRealFreq()
{
	return realfreq;
}

long
Component::getMeanCycleTime()
{
	return mcycle;
}

long
Component::getMaxCycleTime()
{
	return Mtcycle;
}

long
Component::getMinCycleTime()
{
	return mtcycle;
}

/**
 * setFreqTime. Method that sets the minimum elapsed time between two real step() calls.
 * @param newFreqTime Minimal number of milliseconds between two real step() calls.
 **/
void
Component::setFreqTime (const int newFreqTime)
{
	freq_time = newFreqTime * 1000;
}

/**
 * isTime2Run. Method that checks if at this moment the component should execute a real
 * step() call or should return without executing it.
 **/
bool
Component::isTime2Run()
{
	long ctime = getCurrentTime();

	starting = (ctime - beginStep >= freq_time);
	if (starting)
		beginStep = ctime;  

	return starting;
}

/**
 * resetStopWatch. Internal method for measure the time elapsed into an specific state.
 * This method reset the stopwatch.
 **/
void
Component::resetStopWatch()
{
	beginStopWatch = getCurrentTime();
}

/**
 * getStopWatch. Method that returns the elapsed time from the last reset of the stopwatch.
 * It is internally used by the time based transitions.
 **/
long int
Component::getStopWatch()
{
	return (getCurrentTime() - beginStopWatch) / 1000;
}

string
Component::getName()
{
	return name;
}
