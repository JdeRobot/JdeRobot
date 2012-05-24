/*
 * Name: Head.cpp
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

#include "Head.h"

const float	Head::VEL_PAN_MIN		=	0.02;
const float	Head::VEL_PAN_MAX		=	0.3;		// Real robot
const float	Head::VEL_TILT_MIN	=	0.02;
const float	Head::VEL_TILT_MAX	=	0.2;		// Real robot

const float Head::MAXTILT[MAX_TILT_VECTOR] = {14.0f, 14.0f, 14.0f, 14.0f, 14.0f, 16.0f, 18.0f, 20.0f, 22.0f, 26.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f,
		26.0f, 22.0f, 20.0f, 18.0f, 16.0f, 14.0f, 14.0f, 14.0f, 14.0f, 14.0f};

Head::Head()
{
	pan = new JointControl("HeadYaw", MIN_PAN_ANGLE / MAX_PANU, MAX_PAN_ANGLE / MAX_PANU, VEL_PAN_MIN, VEL_PAN_MAX);
	tilt = new JointControl("HeadPitch", MIN_TILT_ANGLE / MAX_TILT, MAX_TILT_ANGLE / MAX_TILT, VEL_TILT_MIN, VEL_TILT_MAX);

	setFreqTime(SHORT_RATE);
}

Head::~Head()
{
	stop();
}

void
Head::init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker)
{
	Component::init(newName, parentBroker);
	pan->init(parentBroker);
	tilt->init(parentBroker);

	this->setFreqTime(SHORT_RATE);
}

float
Head::getTiltLimit(float currentYaw, float currentVy)
{
	if (currentVy < 0)
		return 0.0;

	int idxm, idxM;

	idxm = (toDegrees(currentYaw) / 10) + (MAX_TILT_VECTOR / 2);

	if (currentYaw < 0){
		if (idxm > 0)
			idxM = idxm - 1;
		else
			idxM = idxm;
	}else{
		if (idxm < MAX_TILT_VECTOR - 1)
			idxM = idxm + 1;
		else
			idxM = idxm;
	}

	return toRadians((MAXTILT[idxM] + MAXTILT[idxm]) / 2.0);
}


void
Head::step(void)
{
	if (!isTime2Run())
		return;

	startDebugInfo();

	// --- Head Control ---
	pan->move();

	// --- Tilt control ---
	if(tilt->getOutput() > 0.0)
		target = getTiltLimit(pan->getJointValue(), tilt->getOutput());
	else
		target = toRadians(MIN_TILT);
	tilt->setLimitedTarget(target);
	tilt->move();

	endDebugInfo();
}

//BUILDER COMMENT. DO NOT REMOVE. auxcode begin
void
Head::setPan(float newPanVel, const Ice::Current& c)
{
	setPan(newPanVel);
}

void
Head::setPanPos (float rads, float vel, const Ice::Current& c)
{
	setPanPos(rads, vel);
}

void
Head::setTilt(float tiltVel, const Ice::Current& c)
{
	setTilt(tiltVel);
}

void
Head::setTiltPos(float rads, float vel, const Ice::Current& c)
{
	setTiltPos(rads, vel);
}

void
Head::stop(const Ice::Current& c)
{
	stop();
}
//BUILDER COMMENT. DO NOT REMOVE. auxcode end
