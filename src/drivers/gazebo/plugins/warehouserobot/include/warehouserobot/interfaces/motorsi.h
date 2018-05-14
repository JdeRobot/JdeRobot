#ifndef MOTORSI_H
#define MOTORSI_H

#include <jderobot/motors.h>
#include <warehouserobot/warehouserobotcontrol.hh>

namespace warehouserobot{
namespace interfaces{

class MotorsI : virtual public jderobot::Motors {
public:

    MotorsI(warehouserobot::WarehouserobotControl* const control);
    virtual ~MotorsI();

    virtual float getV(const Ice::Current&);
    virtual float getW(const Ice::Current&);
    virtual float getL(const Ice::Current&);
    virtual Ice::Int setV(Ice::Float v, const Ice::Current&);
    virtual Ice::Int setW(Ice::Float _w, const Ice::Current&);
    virtual Ice::Int setL(Ice::Float l, const Ice::Current&);

private:
    warehouserobot::WarehouserobotControl* const control;
}; // end class MotorsI

}} //NS

#endif // MOTORSI_H
