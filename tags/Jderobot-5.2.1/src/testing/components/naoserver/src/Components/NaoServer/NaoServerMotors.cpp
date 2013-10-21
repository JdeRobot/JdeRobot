#include "NaoServerMotors.h"

NaoServerMotors::NaoServerMotors()
{
	_Body = Body::getInstance();
}

NaoServerMotors::~NaoServerMotors()
{
}

void
NaoServerMotors::init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker)
{
	Component::init(newName, parentBroker);
	this->setFreqTime(SHORT_RATE);
}


void
NaoServerMotors::step(void)
{
	if (!isTime2Run())
		return;
}

float
NaoServerMotors::getV(const Ice::Current&)
{
	return _Body->getV();
}

Ice::Int
NaoServerMotors::setV(Ice::Float v, const Ice::Current&)
{
	_Body->setVelV((float)v/250.0);

	return 0;
}

float
NaoServerMotors::getW(const Ice::Current&)
{
	return _Body->getW();
}

Ice::Int
NaoServerMotors::setW(Ice::Float w, const Ice::Current&)
{
	_Body->setVelW((float)w/20.0);

	return 0;
}

float
NaoServerMotors::getL(const Ice::Current&)
{
	return _Body->getL();
}

Ice::Int
NaoServerMotors::setL(Ice::Float l, const Ice::Current&)
{
	_Body->setVelL((float)l);

	return 0;
}
