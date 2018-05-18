#ifndef MOTORSI_H
#define MOTORSI_H

#include <jderobot/motors.h>
#include <roomba/roombacontrol.hh>

namespace roomba{
namespace interfaces{

class MotorsI : virtual public jderobot::Motors {
public:

    MotorsI(roomba::RoombaControl* const control);
    virtual ~MotorsI();

    virtual float getV(const Ice::Current&);
    virtual float getW(const Ice::Current&);
    virtual float getL(const Ice::Current&);
    virtual Ice::Int setV(Ice::Float v, const Ice::Current&);
    virtual Ice::Int setW(Ice::Float _w, const Ice::Current&);
    virtual Ice::Int setL(Ice::Float l, const Ice::Current&);

private:
    roomba::RoombaControl* const control;
}; // end class MotorsI

}} //NS

#endif // MOTORSI_H
