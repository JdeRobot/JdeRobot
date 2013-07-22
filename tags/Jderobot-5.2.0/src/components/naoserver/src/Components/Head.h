/*
 * Name: Head.h
 * @Author: Carlos Agüero (caguero@gsyc.es)
 *
 * Description:
 *
 * Created on: 17/03/2010
 *
 * Copyright (C) Universidad Rey Juan Carlos
 * All Rights Reserved.
 *
 */

#ifndef HEAD_H
#define HEAD_H

#include "Component.h"
#include "Singleton.h"
#include "JointControl.h"
#include "Common.h"

//BUILDER COMMENT. DO NOT REMOVE. auxinclude begin
#include <IceE/IceE.h>

//BUILDER COMMENT. DO NOT REMOVE. auxinclude end

class Head : public Component, public Singleton<Head>
{
public:

	Head();
	~Head();

	inline float getPan() { return pan->getJointValue(); };
	inline float getTilt() { return tilt->getJointValue(); };

	inline void stop() { setPan(0.0); setTilt(0.0); };
	void step();
	void init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker);

	// Velocity control
	inline void setPan(float newPan)
	{
		pan->set(-newPan);
		pan->setLimitedTarget(toRadians(-sign(newPan) * MAX_PANL));
	}
	inline void setTilt(float newTilt) { tilt->set(newTilt); };

	// Position control
	inline void setPanPos(float rads, float speed) { pan->setPos(rads, speed); };
	inline void setTiltPos(float rads, float speed) { tilt->setPos(rads, speed); };



private:
	float getTiltLimit(float currentYaw, float currentVy);

	static const float	MAX_PANU = 80.0f;
	static const float	MAX_PANL = 40.0f;
	static const float 	MAX_TILT = 45.0f;
	static const float 	MIN_TILT = -39.0f;
	static const float	MIN_PAN_ANGLE = 5.0f;
	static const float	MAX_PAN_ANGLE = 70.0f;
	static const float  MIN_TILT_ANGLE = 5.0f;
	static const float 	MAX_TILT_ANGLE = 30.0f;

	static const int    MAX_TILT_VECTOR =	25;
	static const float	MAXTILT[MAX_TILT_VECTOR];

	//Velocities are expressed in %  (18,12 is ok)
	static const float	VEL_PAN_MIN;
	static const float	VEL_PAN_MAX;		// Real robot
	static const float	VEL_TILT_MIN;
	static const float	VEL_TILT_MAX;		// Real robot

	float target;
	JointControl *pan, *tilt;

//BUILDER COMMENT. DO NOT REMOVE. auxcode begin
public:
	virtual void setPan(float newPanVel, const Ice::Current& c);
	virtual void setPanPos (float rads, float vel, const Ice::Current& c);
	virtual void setTilt(float tiltVel, const Ice::Current& c);
	virtual void setTiltPos(float rads, float vel, const Ice::Current& c);
	virtual void stop(const Ice::Current& c);
//BUILDER COMMENT. DO NOT REMOVE. auxcode end
};

#endif
