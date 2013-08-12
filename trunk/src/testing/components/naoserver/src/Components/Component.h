/*
 * Name: Component.h
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

#ifndef Component_H
#define Component_H

#include <string>
#include <sstream>
#include <IceE/IceE.h>
#include "alcommon/alproxy.h"
#include "alcore/alptr.h"
#include "alcommon/albroker.h"
#include "Singleton.h"

using namespace std;
using namespace AL;

class Component
{
public:

	static const int DEFAULT_FREQ_TIME 	= 60; 	// Miliseconds
	static const int FRAME_RATE 		= 0; 	// Full speed

	static const int SHORT_RATE 		= 50; 	// Full speed
	static const int MEDIUM_RATE 		= 100; 	// Medium speed
	static const int LONG_RATE 			= 500; 	// Low speed

	Component();
	~Component();

	void init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker);
	AL::ALPtr<AL::ALBroker> getParentBroker(void);

	void initLog (const string filename );
	void writeLog (const string data);
	bool isTime2Run	();
	void setFreqTime (const int newFreqTime);
	void startDebugInfo();
	void endDebugInfo();
	void resetStopWatch();
	long getStopWatch();
	string getName();

	virtual void step() = 0;

	long getRealFreq();
	long getMeanCycleTime();
	long getMaxCycleTime();
	long getMinCycleTime();

	inline long getCurrentTime() {
		gettimeofday(&currentTime, NULL);
		return currentTime.tv_sec * 1000000 + currentTime.tv_usec;
	}

	inline long getCurrentTime2() {
		struct timespec begin;
		clock_gettime( CLOCK_MONOTONIC, &begin );

		return begin.tv_sec * 1000000000.0 + begin.tv_nsec;
	}

protected:
	struct timeval 	currentTime;
	int 			freq_time;

private:
	static const long INFO_TIME = 5000000; // 5 Seconds = 5000000 us

	AL::ALPtr<AL::ALBroker> parentBroker;
	int 		iters;
	bool 		starting;
	long 		beginStopWatch, beginStep, beginDebugStep, beginDebugCycle, meanCycle;
	string 		name;
	FILE 		*fdlog;
	long 		realfreq, mcycle, Mtcycle, mtcycle;
};

#endif
