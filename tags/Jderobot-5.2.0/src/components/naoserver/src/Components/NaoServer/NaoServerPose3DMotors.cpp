#include "NaoServerPose3DMotors.h"

NaoServerPose3DMotors::NaoServerPose3DMotors()
{
	_Head = Head::getInstance();
}

NaoServerPose3DMotors::~NaoServerPose3DMotors()
{
}

void
NaoServerPose3DMotors::init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker)
{
	Component::init(newName, parentBroker);
	this->setFreqTime(SHORT_RATE);
}


void
NaoServerPose3DMotors::step(void)
{
	if (!isTime2Run())
		return;
}

Ice::Int 
NaoServerPose3DMotors::setPose3DMotorsData(const Pose3DMotorsDataPtr & data, const Ice::Current&)
{
	_Head->setPanPos(data->pan*-DEG_TO_RAD, 1.0);
	_Head->setTiltPos(data->tilt*-DEG_TO_RAD, 1.0);

	return 0; 
};

Pose3DMotorsDataPtr 
NaoServerPose3DMotors::getPose3DMotorsData(const Ice::Current&)
{
	return NULL;
};

Pose3DMotorsParamsPtr 
NaoServerPose3DMotors::getPose3DMotorsParams(const Ice::Current&)
{
	return NULL;
};
