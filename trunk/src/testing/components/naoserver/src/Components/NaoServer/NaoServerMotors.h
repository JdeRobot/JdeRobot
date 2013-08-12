/*
 * Name: NaoServerMotors.h
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

#ifndef NAOSERVERMOTORS_H
#define NAOSERVERMOTORS_H

#include "Component.h"
#include "Singleton.h"
#include "Body.h"
#include <IceE/IceE.h>
#include <motors.h>

using namespace jderobot;

class NaoServerMotors : public Component, public Singleton<NaoServerMotors>, public Motors
{
public:

	NaoServerMotors();
	~NaoServerMotors();

	void init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker);

	void step();

	/*Motors*/
	float getV(const Ice::Current&);
	Ice::Int setV(Ice::Float v, const Ice::Current&);
	float getW(const Ice::Current&);
	Ice::Int setW(Ice::Float w, const Ice::Current&);
	float getL(const Ice::Current&);
	Ice::Int setL(Ice::Float l, const Ice::Current&);

private:

	Body * _Body;

};

#endif
