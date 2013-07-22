#include "actuators.h"

Actuators::Actuators(Ice::CommunicatorPtr ic)
{
    this->ic = ic;

    // Contact to MOTORS interface
    Ice::ObjectPrx baseMotors = ic->propertyToProxy("introrob.Motors.Proxy");
    if (0 == baseMotors)
        throw "Could not create proxy with motors";
    // Cast to motors
    mprx = jderobot::MotorsPrx::checkedCast(baseMotors);
    if (0 == mprx)
        throw "Invalid proxy introrob.Motors.Proxy";

    motorVout= 0;
    motorWout = 0;
    motorLout= 0;

}

void Actuators::update()
{
    mutex.lock();

    motorVin = this->mprx->getV();
    motorWin = this->mprx->getW();
    motorLin = this->mprx->getL();

    mutex.unlock();
}

void Actuators::setActuators()
{
    mutex.lock();

    if (motorWout < 5 && motorWout>-5)
        this->mprx->setW(0.);

    this->mprx->setW(motorWout);
    this->mprx->setL(motorLout);
    this->mprx->setV(motorVout);

    mutex.unlock();
}


///////////////// GETTER //////////////////
float Actuators::getMotorV()
{
    mutex.lock();
    float v = motorVin;
    mutex.unlock();

    return v;
}

float Actuators::getMotorW()
{
    mutex.lock();
    float w = motorWin;
    mutex.unlock();

    return w;
}

float Actuators::getMotorL()
{
    mutex.lock();
    float l = motorLin;
    mutex.unlock();
    return l;
}

///////////////// SETTER //////////////////
void Actuators::setMotorSTOP()
{
    mutex.lock();
    this->motorVout = 0;
    this->motorWout = 0;
    this->motorLout = 0;
    mutex.unlock();
}

void Actuators::setMotorV(float motorV)
{
    mutex.lock();
    this->motorVout = motorV;
    mutex.unlock();
}

void Actuators::setMotorW(float motorW)
{
    mutex.lock();
    this->motorWout = motorW;
    mutex.unlock();

}

void Actuators::setMotorL(float motorL)
{
    mutex.lock();
    this->motorLout = motorL;
    mutex.unlock();

}

