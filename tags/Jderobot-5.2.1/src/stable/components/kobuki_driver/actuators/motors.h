#ifndef MOTORS_H
#define MOTORS_H

#include <jderobot/motors.h>

#include "../kobukimanager.h"
#include <iostream>

class Motors : virtual public jderobot::Motors {
 public:

    /** Construtor
      * @brief MotorsI
      * @param propertyPrefix name of the server
      * @param sharer Pointer to share memory
      */
     Motors(KobukiManager* kobuki_manager);

     /** Destructor
      * @brief ~MotorsI Destructor
      */
     virtual ~Motors();

     /**
      * @brief getV
      * @return Return the velocity of the robot
      */
     virtual float getV(const Ice::Current&);

     /**
      * @brief getW
      * @return return the angle of the wheel
      */
     virtual float getW(const Ice::Current&);

     /**
      * @brief getL
      * @return Not used
      */
     virtual float getL(const Ice::Current&);

     /**
      * @brief setV
      * @param v velocity in m/s
      * @return return the velocity
      */
     virtual Ice::Int setV(Ice::Float v, const Ice::Current&);

     /**
      * @brief setW
      * @param _w angle of the wheel in radians
      * @return return the angle of the robot
      */
     virtual Ice::Int setW(Ice::Float _w, const Ice::Current&) ;

     /**
      * @brief setL
      * @param l Not used
      * @return Not used
      */
     virtual Ice::Int setL(Ice::Float l, const Ice::Current&);

private:
     KobukiManager* kobuki_manager;
 };

#endif // MOTORS_H
