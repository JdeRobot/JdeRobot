/*
 * Name: JointControl.h
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

#ifndef JOINTCONTROL_H
#define JOINTCONTROL_H

#include <alcore/alptr.h>
#include <alcommon/alproxy.h>
#include "alcommon/albroker.h"
#include "alproxies/almotionproxy.h"
#include "PIDController.h"

class JointControl
{
public:

	JointControl(string joint, float minRef, float maxRef, float minOutput, float maxOutput);
	~JointControl();

	inline void stop() {if (currentCmd != UNKNOWN) pmotion->stop(currentCmd); };
	float getJointValue();
	inline float getOutput() { return vel; };
	inline void setLimitedTarget(float newTarget) { last_target = target; target = newTarget; };

	void init(AL::ALPtr<AL::ALBroker> parentBroker);
	void move(void);
	void set(float newValue);				// Velocity control
	void setPos(float rads, float time);	// Position control

private:

	static const int UNKNOWN 	= 99999;
	static const int MODE_VEL 	= 0;
	static const int MODE_POS 	= 1;

	AL::ALPtr<AL::ALMotionProxy> pmotion;
	float newVelValue, newPosValue, newPosSpeed, last_target, target;
	int currentCmd, ctrl_mode;
	float vel, last_vel, max_vel;
	PIDController *ctrl;
	bool cmd_pending;
	AL::ALValue joint;
};

#endif
