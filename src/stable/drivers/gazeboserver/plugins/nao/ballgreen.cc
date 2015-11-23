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
 *  Author:     Borja Menéndez Moreno <b.menendez.moreno@gmail.com>
 *  Co-author:  José María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include "ballencoder.h"

#define RADTODEG 57.29582790

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(BallGreen)

    BallGreen::BallGreen () {
        pthread_mutex_init(&mutex_ballgreenencoders, NULL);
        pthread_mutex_init(&mutex_ballgreenmotors, NULL);
        this->count = 0;
        this->cycle = 50;
        this->cfgfile_ballgreen = std::string("--Ice.Config=ballgreen.cfg");

        std::cout << "Constructor BallGreen" << std::endl;
    }

    void BallGreen::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        this->ballgreen.joint_pan = _model->GetJoint("jball_vertical");
        this->ballgreen.joint_tilt = _model->GetJoint("wmp2");
        
        this->stiffness = 40.0;

        //LOAD POSE3DMOTORS
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                        boost::bind(&BallGreen::OnUpdate, this));
    }

    void BallGreen::Init () {}

    void BallGreen::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;
        
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        if (this->count == 0) {
            this->count++;
            pthread_t thr_ice;
            pthread_create(&thr_ice, NULL, &thread_BallGreenICE, (void*) this);
            
            this->ballgreen.motorsdata.pan = 0.0;
            this->ballgreen.motorsdata.tilt = 0.0;
        }
        //          ----------ENCODERS----------
        //GET pose3dencoders data from the ball (PAN&TILT)
        pthread_mutex_lock(&this->mutex_ballgreenencoders);
        
        this->ballgreen.encoders.pan = - this->ballgreen.joint_pan->GetAngle(0).Radian();
        this->ballgreen.encoders.tilt = - this->ballgreen.joint_tilt->GetAngle(0).Radian();
        
        pthread_mutex_unlock(&this->mutex_ballgreenencoders);

        //          ----------MOTORS----------
        this->ballgreen.joint_pan->SetMaxForce(0, this->stiffness);
        this->ballgreen.joint_tilt->SetMaxForce(0, this->stiffness);
        
        pthread_mutex_lock(&this->mutex_ballgreenmotors);
        
        float panSpeed = this->ballgreen.encoders.pan - this->ballgreen.motorsdata.pan;
        if ((std::abs(panSpeed) < 0.1) && (std::abs(panSpeed) > 0.001))
            panSpeed = 0.1;
        
        float tiltSpeed = this->ballgreen.encoders.tilt - this->ballgreen.motorsdata.tilt;
        if ((std::abs(tiltSpeed) < 0.1) && (std::abs(tiltSpeed) > 0.001))
            tiltSpeed = 0.1;
        
        this->ballgreen.joint_pan->SetVelocity(0, panSpeed);
        this->ballgreen.joint_tilt->SetVelocity(0, tiltSpeed);

        pthread_mutex_unlock(&this->mutex_ballgreenmotors);
/*        if (this->ballgreen.motorsdata.pan > 0) {
            if (this->ballgreen.encoders.pan < this->ballgreen.motorsdata.pan) {
                this->ballgreen.joint_pan->SetVelocity(0, -0.3);
                this->ballgreen.joint_pan->SetMaxForce(0, this->stiffness);
            } else if (this->ballgreen.encoders.pan > this->ballgreen.motorsdata.pan) {
                this->ballgreen.joint_pan->SetVelocity(0, 0.3);
                this->ballgreen.joint_pan->SetMaxForce(0, this->stiffness);
            }
        } else if (this->ballgreen.motorsdata.pan < 0) {
            if (this->ballgreen.encoders.pan > this->ballgreen.motorsdata.pan) {
                this->ballgreen.joint_pan->SetVelocity(0, 0.3);
                this->ballgreen.joint_pan->SetMaxForce(0, this->stiffness);
            } else if (this->ballgreen.encoders.pan < this->ballgreen.motorsdata.pan) {
                this->ballgreen.joint_pan->SetVelocity(0, -0.3);
                this->ballgreen.joint_pan->SetMaxForce(0, this->stiffness);
            }            
        }
        

        if (this->ballgreen.motorsdata.tilt > 0) {
            if (this->ballgreen.encoders.tilt < this->ballgreen.motorsdata.tilt) {
                this->ballgreen.joint_tilt->SetVelocity(0, -0.3);
                this->ballgreen.joint_tilt->SetMaxForce(0, this->stiffness);
            } else if (this->ballgreen.encoders.tilt > this->ballgreen.motorsdata.tilt) {
                this->ballgreen.joint_tilt->SetVelocity(0, 0.3);
                this->ballgreen.joint_tilt->SetMaxForce(0, this->stiffness);
            }
        } else if (this->ballgreen.motorsdata.tilt < 0) {
            if (this->ballgreen.encoders.tilt > this->ballgreen.motorsdata.tilt) {
                this->ballgreen.joint_tilt->SetVelocity(0, 0.3);
                this->ballgreen.joint_tilt->SetMaxForce(0, this->stiffness);
            } else if (this->ballgreen.encoders.tilt < this->ballgreen.motorsdata.tilt) {
                this->ballgreen.joint_tilt->SetVelocity(0, -0.3);
                this->ballgreen.joint_tilt->SetMaxForce(0, this->stiffness);
            }
        }
*/
        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;

        diff = (totalb - totala) / 1000;
        diff = cycle - diff;

        if (diff < 10)
            diff = 10;

        //usleep(diff*1000);
        sleep(diff / 1000);
    }
    
    class Pose3DEncoders : virtual public jderobot::Pose3DEncoders {
    public:

        Pose3DEncoders ( gazebo::PoseGreenBall* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncoders () {}

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_greenballencoders);
            
            pose3DEncodersData->x = pose->ballgreen.encoders.x;
            pose3DEncodersData->y = pose->ballgreen.encoders.y;
            pose3DEncodersData->z = pose->ballgreen.encoders.z;
            pose3DEncodersData->pan = pose->ballgreen.encoders.pan;
            pose3DEncodersData->tilt = pose->ballgreen.encoders.tilt;
            pose3DEncodersData->roll = pose->ballgreen.encoders.roll;
            pose3DEncodersData->clock = pose->ballgreen.encoders.clock;
            pose3DEncodersData->maxPan = pose->ballgreen.encoders.maxPan;
            pose3DEncodersData->minPan = pose->ballgreen.encoders.minPan;
            pose3DEncodersData->maxTilt = pose->ballgreen.encoders.maxTilt;
            pose3DEncodersData->minTilt = pose->ballgreen.encoders.minTilt;
            
            pthread_mutex_unlock(&pose->mutex_greenballencoders);

            return pose3DEncodersData;
        }

        gazebo::PoseNeck* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;
    };

    class Pose3DMotors : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotors (gazebo::PoseGreenBall* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotors() {}

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_greenballmotors);
            
            pose3DMotorsData->x = pose->ballgreen.motorsdata.x;
            pose3DMotorsData->y = pose->ballgreen.motorsdata.y;
            pose3DMotorsData->z = pose->ballgreen.motorsdata.z;
            pose3DMotorsData->pan = pose->ballgreen.motorsdata.pan;
            pose3DMotorsData->tilt = pose->ballgreen.motorsdata.tilt;
            pose3DMotorsData->roll = pose->ballgreen.motorsdata.roll;
            pose3DMotorsData->panSpeed = pose->ballgreen.motorsdata.panSpeed;
            pose3DMotorsData->tiltSpeed = pose->ballgreen.motorsdata.tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_greenballmotors);

            return pose3DMotorsData;
        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_greenballmotors);
            
            pose3DMotorsParams->maxPan = pose->ballgreen.motorsparams.maxPan;
            pose3DMotorsParams->minPan = pose->ballgreen.motorsparams.minPan;
            pose3DMotorsParams->maxTilt = pose->ballgreen.motorsparams.maxTilt;
            pose3DMotorsParams->minTilt = pose->ballgreen.motorsparams.minTilt;
            pose3DMotorsParams->maxPanSpeed = pose->ballgreen.motorsparams.maxPanSpeed;
            pose3DMotorsParams->maxTiltSpeed = pose->ballgreen.motorsparams.maxTiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_greenballmotors);
            
            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_greenballmotors);
            
            pose->ballgreen.motorsdata.x = data->x;
            pose->ballgreen.motorsdata.y = data->y;
            pose->ballgreen.motorsdata.z = data->z;
            pose->ballgreen.motorsdata.pan = data->pan;
            pose->ballgreen.motorsdata.tilt = data->tilt;
            pose->ballgreen.motorsdata.roll = data->roll;
            pose->ballgreen.motorsdata.panSpeed = data->panSpeed;
            pose->ballgreen.motorsdata.tiltSpeed = data->tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_greenballmotors);
        }

        gazebo::PoseGreenBall* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;
    };

    void* thread_BallGreenICE ( void* v ) {

        gazebo::BallGreen* ballgreen = (gazebo::BallGreen*)v;
        char* name = (char*) ballgreen->cfgfile_ballgreen.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string EndpointsEncoders = prop->getProperty("BallGreenEncoders.Endpoints");
            std::cout << "BallGreenEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("BallGreenMotors.Endpoints");
            std::cout << "BallGreenMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterBallGreenEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterBallGreenMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncoders(ballgreen);
            Ice::ObjectPtr motors = new Pose3DMotors(ballgreen);

            AdapterEncoders->add(encoders, ic->stringToIdentity("BallGreenEncoders"));
            AdapterMotors->add(motors, ic->stringToIdentity("BallGreenMotors"));

            AdapterEncoders->activate();
            AdapterMotors->activate();

            ic->waitForShutdown();
        } catch (const Ice::Exception& e) {
            std::cerr << e << std::endl;
        } catch (const char* msg) {
            std::cerr << msg << std::endl;
        }
        if (icB) {
            try {
                icB->destroy();
            } catch (const Ice::Exception& e) {
                std::cerr << e << std::endl;
            }
        }
    }
}
