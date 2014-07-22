/*
 *  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors : Borja Menéndez <borjamonserrano@gmail.com>
 *
 */

#include "control.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
Control::Control () {}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
Control::~Control () {
    if (ic)
        ic->destroy();
}

/*************************************************************
 * SETTERS
 *************************************************************/
void Control::setArgs ( int argc, char** argv) {
    this->argc = argc;
    this->argv = argv;
}

float Control::normalizeMe ( float number, float negBound, float posBound ) {
    float n = number - 100.0;
    if (n > 100.0)
        n = 100.0;
        
    return (posBound * n / 100.0);
}

void Control::setMovement ( Motion m, float yaw_v, float pitch_w, float roll_l ) {
    bool send = true;
    if (m == MOTORS) {
        if ( (yaw_v == 0.0) && (pitch_w == 0.0) && (roll_l == 0.0) ) {
            this->motors_v = 0.0;
            this->motors_w = 0.0;
            this->motors_l = 0.0;
        } else {
            if (directMovement) {
                this->motors_v = - this->normalizeMe(yaw_v, -1.0, 1.0);
                this->motors_w = - this->normalizeMe(pitch_w, -1.0, 1.0);
            } else if ((this->motors_v + yaw_v <= 1.0) || (this->motors_w + pitch_w <= 1.0)) {
                this->motors_v += yaw_v;
                this->motors_w += pitch_w;
                this->motors_l += roll_l;
            } else {
                send = false;
            }
        }
        if (send) {
            this->walk->setV(this->motors_v);
            this->walk->setW(this->motors_w);
            this->walk->setL(this->motors_l);
        }
    } else if (m == HEADMOTORS) {
        jderobot::Pose3DMotorsDataPtr pose(new jderobot::Pose3DMotorsData);
        if ( (yaw_v == 0.0) && (pitch_w == 0.0) && (roll_l == 0.0) ) {
            this->head_pan = 0.0;
            this->head_tilt = 0.0;
        } else {
            if (directHead) {
                this->head_pan = - this->normalizeMe(yaw_v, -2.0858, 2.0858);
                this->head_tilt = this->normalizeMe(pitch_w, -0.6721, 0.515);
            } else if (((this->head_pan + yaw_v < 2.0858 ) && (this->head_pan - yaw_v > -2.0858 )) ||
                       ((this->head_tilt + pitch_w < 0.515) && (this->head_tilt - pitch_w > -0.6721))) {
                this->head_pan += yaw_v;
                this->head_tilt += pitch_w;
            } else {
                send = false;
            }
        }
        if (send) {
            pose->pan = this->head_pan;
            pose->tilt = this->head_tilt;
            pose->roll = 0.0;
            this->mapMotors[m]->setPose3DMotorsData(pose);
        }
    } else if (m == HEADSPEED) {
        jderobot::Pose3DMotorsDataPtr pose(new jderobot::Pose3DMotorsData);
        pose->panSpeed = yaw_v;
        pose->tiltSpeed = pitch_w;
        this->mapMotors[m]->setPose3DMotorsData(pose);
    } else {
        jderobot::Pose3DMotorsDataPtr pose(new jderobot::Pose3DMotorsData);
        pose->pan = yaw_v * PI / 180.0;
        pose->tilt = pitch_w * PI / 180.0;
        pose->roll = roll_l * PI / 180.0;
        this->mapMotors[m]->setPose3DMotorsData(pose);
    }
}

void Control::setAction ( Action a ) {
    this->motions->setMotion((jderobot::MotionType) a);
}

void Control::setDirectMovement ( bool b ) {
    this->directMovement = b;
}

void Control::setDirectHead ( bool b ) {
    this->directHead = b;
}

/*************************************************************
 * GETTERS
 *************************************************************/
int Control::getArgc () {
    return this->argc;
}

char** Control::getArgv () {
    return this->argv;
}

jderobot::ImageDataPtr Control::getImage () {
    return this->cameraprx->getImageData();
}

int Control::connect () { 
    try {
        // ICE
        char* name = (char*) "--Ice.Config=naooperator.cfg";
        int argc = 1;
        char* argv[] = {name};
        std::cout << "Initializing..." << std::endl;
        ic = Ice::initialize(argc, argv);
//        ic = Ice::initialize(control->getArgc(), control->getArgv());
        
        // Contact to head motors
        Ice::ObjectPrx headmotors = ic->propertyToProxy("naooperator.HeadMotors.Proxy");
        if (headmotors == 0)
            throw "Could not create proxy with head motors";
        this->mapMotors[HEADMOTORS] = jderobot::Pose3DMotorsPrx::checkedCast(headmotors);
        if (this->mapMotors[HEADMOTORS] == 0)
            throw "Invalid proxy naooperator.HeadMotors.Proxy";
        std::cout << "Head motors connected" << std::endl;
        
        // Contact to head motors (speed commands)
/*        Ice::ObjectPrx headspeed = ic->propertyToProxy("naooperator.HeadSpeed.Proxy");
        if (headspeed == 0)
            throw "Could not create proxy with head speed";
        this->mapMotors[HEADSPEED] = jderobot::Pose3DMotorsPrx::checkedCast(headspeed);
        if (this->mapMotors[HEADSPEED] == 0)
            throw "Invalid proxy naooperator.HeadSpeed.Proxy";
        std::cout << "Head speed connected" << std::endl;
  */          
        // Contact to shoulder motors
        Ice::ObjectPrx leftshouldermotors = ic->propertyToProxy("naooperator.LeftShoulderMotors.Proxy");
        if (leftshouldermotors == 0)
            throw "Could not create proxy with left shoulder motors";
        this->mapMotors[LEFTSHOULDERMOTORS] = jderobot::Pose3DMotorsPrx::checkedCast(leftshouldermotors);
        if (this->mapMotors[LEFTSHOULDERMOTORS] == 0)
            throw "Invalid proxy naooperator.LeftShoulderMotors.Proxy";
        std::cout << "Left shoulder motors connected" << std::endl;
            
        Ice::ObjectPrx rightshouldermotors = ic->propertyToProxy("naooperator.RightShoulderMotors.Proxy");
        if (rightshouldermotors == 0)
            throw "Could not create proxy with right shoulder motors";
        this->mapMotors[RIGHTSHOULDERMOTORS] = jderobot::Pose3DMotorsPrx::checkedCast(rightshouldermotors);
        if (this->mapMotors[RIGHTSHOULDERMOTORS] == 0)
            throw "Invalid proxy naooperator.RightShoulderMotors.Proxy";
        std::cout << "Right shoulder motors connected" << std::endl;
            
        // Contact to elbow motors
        Ice::ObjectPrx leftelbowmotors = ic->propertyToProxy("naooperator.LeftElbowMotors.Proxy");
        if (leftelbowmotors == 0)
            throw "Could not create proxy with left elbow motors";
        this->mapMotors[LEFTELBOWMOTORS] = jderobot::Pose3DMotorsPrx::checkedCast(leftelbowmotors);
        if (this->mapMotors[LEFTELBOWMOTORS] == 0)
            throw "Invalid proxy naooperator.LeftElbowMotors.Proxy";
        std::cout << "Left elbow motors connected" << std::endl;
            
        Ice::ObjectPrx rightelbowmotors = ic->propertyToProxy("naooperator.RightElbowMotors.Proxy");
        if (rightelbowmotors == 0)
            throw "Could not create proxy with right elbow motors";
        this->mapMotors[RIGHTELBOWMOTORS] = jderobot::Pose3DMotorsPrx::checkedCast(rightelbowmotors);
        if (this->mapMotors[RIGHTELBOWMOTORS] == 0)
            throw "Invalid proxy naooperator.RightElbowMotors.Proxy";
        std::cout << "Right elbow motors connected" << std::endl;
/*            
        // Contact to hip motors
        Ice::ObjectPrx lefthipmotors = ic->propertyToProxy("naooperator.LeftHipMotors.Proxy");
        if (lefthipmotors == 0)
            throw "Could not create proxy with left hip motors";
        this->mapMotors[LEFTHIPMOTORS] = jderobot::Pose3DMotorsPrx::checkedCast(lefthipmotors);
        if (this->mapMotors[LEFTHIPMOTORS] == 0)
            throw "Invalid proxy naooperator.LeftHipMotors.Proxy";
        std::cout << "Left hip motors connected" << std::endl;
            
        Ice::ObjectPrx righthipmotors = ic->propertyToProxy("naooperator.RightHipMotors.Proxy");
        if (righthipmotors == 0)
            throw "Could not create proxy with right hip motors";
        this->mapMotors[RIGHTHIPMOTORS] = jderobot::Pose3DMotorsPrx::checkedCast(righthipmotors);
        if (this->mapMotors[RIGHTHIPMOTORS] == 0)
            throw "Invalid proxy naooperator.RightHipMotors.Proxy";
        std::cout << "Right hip motors connected" << std::endl;
*/            
        // Contact to knee motors
        Ice::ObjectPrx leftkneemotors = ic->propertyToProxy("naooperator.LeftKneeMotors.Proxy");
        if (leftkneemotors == 0)
            throw "Could not create proxy with left knee motors";
        this->mapMotors[LEFTKNEEMOTORS] = jderobot::Pose3DMotorsPrx::checkedCast(leftkneemotors);
        if (this->mapMotors[LEFTKNEEMOTORS] == 0)
            throw "Invalid proxy naooperator.LeftKneeMotors.Proxy";
        std::cout << "Left knee motors connected" << std::endl;
            
        Ice::ObjectPrx rightkneemotors = ic->propertyToProxy("naooperator.RightKneeMotors.Proxy");
        if (rightkneemotors == 0)
            throw "Could not create proxy with right knee motors";
        this->mapMotors[RIGHTKNEEMOTORS] = jderobot::Pose3DMotorsPrx::checkedCast(rightkneemotors);
        if (this->mapMotors[RIGHTKNEEMOTORS] == 0)
            throw "Invalid proxy naooperator.RightKneeMotors.Proxy";
        std::cout << "Right knee motors connected" << std::endl;
            
        // Contact to ankle motors
        Ice::ObjectPrx leftanklemotors = ic->propertyToProxy("naooperator.LeftAnkleMotors.Proxy");
        if (leftanklemotors == 0)
            throw "Could not create proxy with left ankle motors";
        this->mapMotors[LEFTANKLEMOTORS] = jderobot::Pose3DMotorsPrx::checkedCast(leftanklemotors);
        if (this->mapMotors[LEFTANKLEMOTORS] == 0)
            throw "Invalid proxy naooperator.LeftAnkleMotors.Proxy";
        std::cout << "Left ankle motors connected" << std::endl;
            
        Ice::ObjectPrx rightanklemotors = ic->propertyToProxy("naooperator.RightAnkleMotors.Proxy");
        if (rightanklemotors == 0)
            throw "Could not create proxy with right ankle motors";
        this->mapMotors[RIGHTANKLEMOTORS] = jderobot::Pose3DMotorsPrx::checkedCast(rightanklemotors);
        if (this->mapMotors[RIGHTANKLEMOTORS] == 0)
            throw "Invalid proxy naooperator.RightAnkleMotors.Proxy";
        std::cout << "Right ankle motors connected" << std::endl;
            
        std::cout << "All hinge motors connected!" << std::endl;
        
        // Contact to camera
/*        Ice::ObjectPrx camera = ic->propertyToProxy("naooperator.Camera.Proxy");
        if (camera == 0)
            throw "Could not create proxy with camera";
        this->cameraprx = jderobot::CameraPrx::checkedCast(camera);
        if (this->cameraprx == 0)
            throw "Invalid proxy naooperator.Camera.Proxy";
        std::cout << "Camera connected" << std::endl;

        // Contact to motors (for walking)
        Ice::ObjectPrx motors = ic->propertyToProxy("naooperator.Motors.Proxy");
        if (motors == 0)
            throw "Could not create proxy with motors";
        this->walk = jderobot::MotorsPrx::checkedCast(motors);
        if (this->walk == 0)
            throw "Invalid proxy naooperator.Motors.Proxy";
        std::cout << "Motors connected" << std::endl;
        
        // Contact to motors (for different actions)
        Ice::ObjectPrx motionsPrx = ic->propertyToProxy("naooperator.Motions.Proxy");
        if (motionsPrx == 0)
            throw "Could not create proxy with motions";
        this->motions = jderobot::NaoMotionsPrx::checkedCast(motionsPrx);
        if (this->motions == 0)
            throw "Invalid proxy naooperator.Motions.Proxy";
        std::cout << "Motions connected" << std::endl;
*/
        std::cout << "All ICE interfaces connected!" << std::endl;

    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        return -1;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        return -1;
    }
        
    return 0;
}
