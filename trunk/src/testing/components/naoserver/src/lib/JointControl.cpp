/*
 * Name: JointControl.cpp
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

#include "JointControl.h"
#include "Common.h"

JointControl::JointControl(string joint, float minRef, float maxRef, float minOutput, float maxOutput)
{
	this->joint.arraySetSize(1);
	this->joint[0] = joint;
	currentCmd = newPosValue = last_vel = last_target = UNKNOWN;
	ctrl = new PIDController(joint, minRef, maxRef, minOutput, maxOutput);
	max_vel = maxOutput;
	cmd_pending = false;
	ctrl_mode = MODE_VEL;
	target = 0.0;
	vel = 0.0;
	last_target = -1000.0;
	last_vel = -1000.0;
}

JointControl::~JointControl()
{
	stop();
}

void
JointControl::init(AL::ALPtr<AL::ALBroker> parentBroker)
{
	try{
		pmotion = parentBroker->getMotionProxy();
		pmotion->setStiffnesses(joint, 1.0);
	}catch( AL::ALError& e) {
		cerr << "[JointControl ()::init(): " << e.toString() << endl;
	}
}

void
JointControl::set(float newValue)
{
	ctrl_mode = MODE_VEL;
	newVelValue = newValue;
	ctrl->setReference(newVelValue);
	vel = ctrl->getOutput();
	newPosValue = UNKNOWN;
}

void
JointControl::setPos(float rads, float speed)
{
	ctrl_mode = MODE_POS;
	if (rads != newPosValue) {
		cmd_pending = true;
		newPosValue = rads;
		newPosSpeed = speed;
	}
}

void
JointControl::move(void)
{
	float realvel;

	if (ctrl_mode == MODE_VEL) {

		realvel = 0.0001 + fabs(vel);

		float pos = vel/2.0;


		if(fabs(vel)>0.001)
		{
			//cerr<<"V ["<<(string)joint[0]<<"]\tt: "<<realvel<<"\tv: "<<realvel<<endl;

			float actual;

			actual = getJointValue();

			if(((actual < toRadians(-65.0)) && (pos<0.0)) ||
			   ((actual > toRadians(65.0)) && (pos>0.0)))
				return;

			try{
				pmotion->changeAngles(joint, pos, realvel);
				last_vel = realvel;
				last_target = target;
			}catch( AL::ALError& e) {
				cerr << "[JointControl ( )::step()]: V error : " << e.toString() << endl;
			}
		}
	}else if (cmd_pending) {	// Position control
		//cerr<<"P ["<<(string)joint[0]<<"]\tt: "<<newPosValue<<"\tv: "<<newPosSpeed<<endl;

		try{
			pmotion->setAngles(joint, newPosValue, newPosSpeed);
		}catch( AL::ALError& e) {
			cerr << "[JointControl ( )::step()]: P error : " << e.toString() << endl;
		}

		cmd_pending = false;
	}

}

float
JointControl::getJointValue()
{
	return (pmotion->getAngles(joint, USE_SENSOR)[0]);
};
