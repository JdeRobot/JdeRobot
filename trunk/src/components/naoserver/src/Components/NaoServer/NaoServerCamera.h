/*
 * Name: NaoServerCamera.h
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

#ifndef NAOSERVERCAMERA_H
#define NAOSERVERCAMERA_H

#include "Component.h"
#include "Singleton.h"
#include "Perception.h"
#include <IceE/IceE.h>
#include <camera.h>

using namespace jderobot;

class NaoServerCamera : public Component, public Singleton<NaoServerCamera>, public Camera
{
public:

	NaoServerCamera();
	~NaoServerCamera();

	void init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker);

	void step();

	/*Camera*/
	jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c);
	jderobot::CameraDescriptionPtr getCameraDescription(const Ice::Current& c);
	Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr &description, const Ice::Current& c);
	std::string startCameraStreaming(const Ice::Current&);
	void stopCameraStreaming(const Ice::Current&);
	jderobot::ImageDataPtr getImageData(const Ice::Current& c);

private:

	Perception * _Perception;

};

#endif
