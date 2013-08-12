/*
 * Name: NaoServerEncoders.h
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

#ifndef NAOSERVERENCODERS_H
#define NAOSERVERENCODERS_H

#include "Component.h"
#include "Singleton.h"
#include "Body.h"
#include <IceE/IceE.h>
#include <encoders.h>

#define DEG_TO_RAD (3.1415926/180.0)
#define RAD_TO_DEG (180.0/3.1415926)

using namespace jderobot;

class NaoServerEncoders : public Component, public Singleton<NaoServerEncoders>, public Encoders
{
public:

	NaoServerEncoders();
	~NaoServerEncoders();

	void init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker);

	void step();

	/*Encoders*/
	EncodersDataPtr getEncodersData(const Ice::Current&);

private:

	EncodersDataPtr encodersData;

	Body * _Body;

};

#endif
