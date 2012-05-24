#include "NaoServerCamera.h"

NaoServerCamera::NaoServerCamera()
{
	_Perception = Perception::getInstance();
}

NaoServerCamera::~NaoServerCamera()
{
}

void
NaoServerCamera::init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker)
{
	Component::init(newName, parentBroker);
	this->setFreqTime(SHORT_RATE);
}


void
NaoServerCamera::step(void)
{
	if (!isTime2Run())
		return;
}

jderobot::ImageDescriptionPtr
NaoServerCamera::getImageDescription(const Ice::Current& c)
{
	return NULL;
}

jderobot::CameraDescriptionPtr
NaoServerCamera::getCameraDescription(const Ice::Current& c)
{
	return NULL;
}

Ice::Int
NaoServerCamera::setCameraDescription(const jderobot::CameraDescriptionPtr &description, const Ice::Current& c)
{
	return 0;
}

std::string
NaoServerCamera::startCameraStreaming(const Ice::Current&)
{
	return "";
}

void
NaoServerCamera::stopCameraStreaming(const Ice::Current&)
{
}

jderobot::ImageDataPtr
NaoServerCamera::getImageData(const Ice::Current& c)
{
	return _Perception->getImageData(c);
}
