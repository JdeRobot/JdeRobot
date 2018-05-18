#include "formula1/interfaces/motorsi.h"

using namespace formula1::interfaces;
using namespace jderobot;

MotorsI::MotorsI(formula1::Formula1Control* const control):
    control(control)
{}

MotorsI::~MotorsI() {}

float
MotorsI::getV(const Ice::Current&) {

    return control->robotMotors.v;
}

float
MotorsI::getW(const Ice::Current&) {

    return control->robotMotors.w;
}

float
MotorsI::getL(const Ice::Current&) {
    return 0.;
}

Ice::Int
MotorsI::setV(Ice::Float v, const Ice::Current&) {

    control->robotMotors.v = v;
    return 0;
}

Ice::Int
MotorsI::setW(Ice::Float _w, const Ice::Current&) {

    control->robotMotors.w = -_w;
    return 0;
}

Ice::Int
MotorsI::setL(Ice::Float /*l*/, const Ice::Current&) {
    return 0;
}
