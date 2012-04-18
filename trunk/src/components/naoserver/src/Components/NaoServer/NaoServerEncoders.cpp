#include "NaoServerEncoders.h"

NaoServerEncoders::NaoServerEncoders()
{
	_Body = Body::getInstance();

	this->encodersData = new jderobot::EncodersData();
}

NaoServerEncoders::~NaoServerEncoders()
{
}

void
NaoServerEncoders::init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker)
{
	Component::init(newName, parentBroker);
	this->setFreqTime(SHORT_RATE);
}


void
NaoServerEncoders::step(void)
{
	if (!isTime2Run())
		return;
}

EncodersDataPtr
NaoServerEncoders::getEncodersData(const Ice::Current&)
{
	float x, y, theta;

	_Body->getGlobalMovement(x, y, theta);

	this->encodersData->robotx = x;
	this->encodersData->roboty = y;
	this->encodersData->robottheta = theta * RAD_TO_DEG;
	this->encodersData->robotcos=cos(theta);
	this->encodersData->robotsin=sin(theta);
	return this->encodersData;
}
