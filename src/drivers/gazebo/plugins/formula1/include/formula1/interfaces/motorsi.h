#ifndef MOTORSI_H
#define MOTORSI_H

#include <jderobot/motors.h>
#include <formula1/formula1control.hh>

namespace formula1{
namespace interfaces{

class MotorsI : virtual public jderobot::Motors {
public:

    MotorsI(formula1::Formula1Control* const control);
    virtual ~MotorsI();

    virtual float getV(const Ice::Current&);
    virtual float getW(const Ice::Current&);
    virtual float getL(const Ice::Current&);
    virtual Ice::Int setV(Ice::Float v, const Ice::Current&);
    virtual Ice::Int setW(Ice::Float _w, const Ice::Current&);
    virtual Ice::Int setL(Ice::Float l, const Ice::Current&);

private:
    formula1::Formula1Control* const control;
}; // end class MotorsI

}} //NS

#endif // MOTORSI_H
