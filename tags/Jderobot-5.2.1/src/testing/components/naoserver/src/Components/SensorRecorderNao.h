/*
 * Name: SensorRecorderNao.h
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

#ifndef SENSORRECORDERNAO_H
#define SENSORRECORDERNAO_H

#include "Component.h"
#include "Singleton.h"
#include "ImageInput.h"
#include "Body.h"
#include "Perception.h"
#include "Kinematics.h"

class SensorRecorderNao : public Component, public Singleton<SensorRecorderNao> {
public:

	SensorRecorderNao();
	~SensorRecorderNao();

	void init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker);

	void step();

private:

	Body * _Body;
	Perception * _Perception;
	Kinematics * _Kinematics;

	IplImage* src;
	string file_root;
	string file_name_image;
	string file_name_data;
	bool first_iteration;
	long init_time;
};

#endif
