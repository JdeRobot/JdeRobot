#include "NaoServerPose3DEncoders.h"

NaoServerPose3DEncoders::NaoServerPose3DEncoders()
{
	_Kinematics = Kinematics::getInstance();

	this->pose3D = new Pose3DEncodersData();
}

NaoServerPose3DEncoders::~NaoServerPose3DEncoders()
{
}

void
NaoServerPose3DEncoders::init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker)
{
	Component::init(newName, parentBroker);
	this->setFreqTime(SHORT_RATE);
}


void
NaoServerPose3DEncoders::step(void)
{
	if (!isTime2Run())
		return;
}

Pose3DEncodersDataPtr
NaoServerPose3DEncoders::getPose3DEncodersData(const Ice::Current&)
{
	TKinematics * data;

	data = _Kinematics->getKinematics();

	this->pose3D->x = data->x;
	this->pose3D->y = data->y;
	this->pose3D->z = data->z;
	this->pose3D->pan = data->pan * RAD_TO_DEG;
	this->pose3D->tilt = data->tilt * RAD_TO_DEG;
	this->pose3D->roll = data->roll * RAD_TO_DEG;

	return this->pose3D;
}
