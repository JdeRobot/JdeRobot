#ifndef NAOSERVERPOSE3DMOTORS_H
#define NAOSERVERPOSE3DMOTORS_H

#include "Component.h"
#include "Singleton.h"
#include "Head.h"
#include <IceE/IceE.h>
#include <pose3dmotors.h>

#define DEG_TO_RAD (3.1415926/180.0)
#define RAD_TO_DEG (180.0/3.1415926)

using namespace jderobot;

class NaoServerPose3DMotors : public Component, public Singleton<NaoServerPose3DMotors>, public Pose3DMotors
{
public:

	NaoServerPose3DMotors();
	~NaoServerPose3DMotors();

	void init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker);

	void step();

	/*Pose3DMotors*/
	Ice::Int setPose3DMotorsData(const Pose3DMotorsDataPtr & data, const Ice::Current&);
	Pose3DMotorsDataPtr getPose3DMotorsData(const Ice::Current&);
	Pose3DMotorsParamsPtr getPose3DMotorsParams(const Ice::Current&);

private:

	Head * _Head;
};

#endif
