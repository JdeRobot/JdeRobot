#ifndef NAOSERVERPOSE3DENCODERS_H
#define NAOSERVERPOSE3DENCODERS_H

#include "Component.h"
#include "Singleton.h"
#include "Kinematics.h"
#include <IceE/IceE.h>
#include <pose3dencoders.h>

#define DEG_TO_RAD (3.1415926/180.0)
#define RAD_TO_DEG (180.0/3.1415926)

using namespace jderobot;

class NaoServerPose3DEncoders : public Component, public Singleton<NaoServerPose3DEncoders>, public Pose3DEncoders
{
public:

	NaoServerPose3DEncoders();
	~NaoServerPose3DEncoders();

	void init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker);

	void step();

	/*Pose3DEncoders*/
	Pose3DEncodersDataPtr getPose3DEncodersData(const Ice::Current&);

private:

	Pose3DEncodersDataPtr pose3D;

	Kinematics * _Kinematics;
};

#endif
