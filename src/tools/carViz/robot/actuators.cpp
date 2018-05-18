#include "actuators.h"

Actuators::Actuators(Comm::Communicator* jdrc)
{
    this->jdrc = jdrc;

    this->motorsClient = Comm::getMotorsClient(jdrc, "carViz.Motors");

}



///////////////// GETTER //////////////////
float Actuators::getMotorV()
{
	return 0;
}

float Actuators::getMotorW()
{
	return 0;
}

float Actuators::getMotorL()
{
    return 0;
}

///////////////// SETTER //////////////////
void Actuators::setMotorSTOP()
{
	JdeRobotTypes::CMDVel vel;
	if (this->motorsClient) {
		mutex.lock();
		this->motorsClient->sendVelocities(vel);
		mutex.unlock();
	}
}

void Actuators::setMotorV(float motorV)
{
	if (this->motorsClient) {
		mutex.lock();
		this->motorsClient->sendVX(motorV);
		mutex.unlock();
	}
}

void Actuators::setMotorW(float motorW)
{
	if (this->motorsClient) {
		mutex.lock();
		this->motorsClient->sendAZ(motorW);
		mutex.unlock();
	}
}

void Actuators::setMotorL(float motorL)
{
	if (this->motorsClient) {
		mutex.lock();
		this->motorsClient->sendVY(motorL);
		mutex.unlock();
	}

}

