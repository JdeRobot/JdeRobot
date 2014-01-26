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

#include "ballred.h"

#define RADTODEG 57.29582790

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(BallRed)

    BallRed::BallRed () {
        pthread_mutex_init(&mutex_ballredencoders, NULL);
        pthread_mutex_init(&mutex_ballredmotors, NULL);
        this->count = 0;
        this->cycle = 50;
        this->cfgfile_ballred = std::string("--Ice.Config=ballred.cfg");

        std::cout << "Constructor BallRed" << std::endl;
    }

    void BallRed::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        this->ballred.joint_pan = _model->GetJoint("jball_vertical");
        this->ballred.joint_tilt = _model->GetJoint("wmp2");
        
        this->stiffness = 40.0;

        //LOAD POSE3DMOTORS
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                        boost::bind(&BallRed::OnUpdate, this));
    }

    void BallRed::Init () {}

    void BallRed::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;
        
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        if (this->count == 0) {
            this->count++;
            pthread_t thr_ice;
            pthread_create(&thr_ice, NULL, &thread_BallRedICE, (void*) this);
            
            this->ballred.motorsdata.pan = 0.0;
            this->ballred.motorsdata.tilt = 0.0;
        }
        //          ----------ENCODERS----------
        //GET pose3dencoders data from the ball (PAN&TILT)
        this->ballred.encoders.pan = - this->ballred.joint_pan->GetAngle(0).Radian();
        this->ballred.encoders.tilt = - this->ballred.joint_tilt->GetAngle(0).Radian();

        //          ----------ENCODERS----------
        pthread_mutex_lock(&this->mutex_ballredencoders);
        
        this->ballred.encoders.pan = - this->ballred.joint_pan->GetAngle(0).Radian();
        this->ballred.encoders.tilt = - this->ballred.joint_tilt->GetAngle(0).Radian();
        
        pthread_mutex_unlock(&this->mutex_ballredencoders);

        //          ----------MOTORS----------
        this->ballred.joint_pan->SetMaxForce(0, this->stiffness);
        this->ballred.joint_tilt->SetMaxForce(0, this->stiffness);
        
        pthread_mutex_lock(&this->mutex_ballredmotors);
        
        float panSpeed = this->ballred.encoders.pan - this->ballred.motorsdata.pan;
        if ((std::abs(panSpeed) < 0.1) && (std::abs(panSpeed) > 0.001))
            panSpeed = 0.1;
        
        float tiltSpeed = this->ballred.encoders.tilt - this->ballred.motorsdata.tilt;
        if ((std::abs(tiltSpeed) < 0.1) && (std::abs(tiltSpeed) > 0.001))
            tiltSpeed = 0.1;
        
        this->ballred.joint_pan->SetVelocity(0, panSpeed);
        this->ballred.joint_tilt->SetVelocity(0, tiltSpeed);

        pthread_mutex_unlock(&this->mutex_ballredmotors);

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

        Pose3DEncoders ( gazebo::BallRed* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncoders () {}

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_ballredencoders);
            
            pose3DEncodersData->x = pose->ballred.encoders.x;
            pose3DEncodersData->y = pose->ballred.encoders.y;
            pose3DEncodersData->z = pose->ballred.encoders.z;
            pose3DEncodersData->pan = pose->ballred.encoders.pan;
            pose3DEncodersData->tilt = pose->ballred.encoders.tilt;
            pose3DEncodersData->roll = pose->ballred.encoders.roll;
            pose3DEncodersData->clock = pose->ballred.encoders.clock;
            pose3DEncodersData->maxPan = pose->ballred.encoders.maxPan;
            pose3DEncodersData->minPan = pose->ballred.encoders.minPan;
            pose3DEncodersData->maxTilt = pose->ballred.encoders.maxTilt;
            pose3DEncodersData->minTilt = pose->ballred.encoders.minTilt;
            
            pthread_mutex_unlock(&pose->mutex_ballredencoders);

            return pose3DEncodersData;
        }

        gazebo::BallRed* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;
    };

    class Pose3DMotors : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotors (gazebo::BallRed* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotors() {}

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_ballredmotors);
            
            pose3DMotorsData->x = pose->ballred.motorsdata.x;
            pose3DMotorsData->y = pose->ballred.motorsdata.y;
            pose3DMotorsData->z = pose->ballred.motorsdata.z;
            pose3DMotorsData->pan = pose->ballred.motorsdata.pan;
            pose3DMotorsData->tilt = pose->ballred.motorsdata.tilt;
            pose3DMotorsData->roll = pose->ballred.motorsdata.roll;
            pose3DMotorsData->panSpeed = pose->ballred.motorsdata.panSpeed;
            pose3DMotorsData->tiltSpeed = pose->ballred.motorsdata.tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_ballredmotors);

            return pose3DMotorsData;
        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_ballredmotors);
            
            pose3DMotorsParams->maxPan = pose->ballred.motorsparams.maxPan;
            pose3DMotorsParams->minPan = pose->ballred.motorsparams.minPan;
            pose3DMotorsParams->maxTilt = pose->ballred.motorsparams.maxTilt;
            pose3DMotorsParams->minTilt = pose->ballred.motorsparams.minTilt;
            pose3DMotorsParams->maxPanSpeed = pose->ballred.motorsparams.maxPanSpeed;
            pose3DMotorsParams->maxTiltSpeed = pose->ballred.motorsparams.maxTiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_ballredmotors);
            
            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_ballredmotors);
            
            pose->ballred.motorsdata.x = data->x;
            pose->ballred.motorsdata.y = data->y;
            pose->ballred.motorsdata.z = data->z;
            pose->ballred.motorsdata.pan = data->pan;
            pose->ballred.motorsdata.tilt = data->tilt;
            pose->ballred.motorsdata.roll = data->roll;
            pose->ballred.motorsdata.panSpeed = data->panSpeed;
            pose->ballred.motorsdata.tiltSpeed = data->tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_ballredmotors);
        }

        gazebo::BallRed* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;
    };

    void* thread_BallRedICE ( void* v ) {

        gazebo::BallRed* ballred = (gazebo::BallRed*)v;
        char* name = (char*) ballred->cfgfile_ballred.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string EndpointsEncoders = prop->getProperty("BallRedEncoders.Endpoints");
            std::cout << "BallRedEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("BallRedMotors.Endpoints");
            std::cout << "BallRedMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterBallRedEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterBallRedMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncoders(ballred);
            Ice::ObjectPtr motors = new Pose3DMotors(ballred);

            AdapterEncoders->add(encoders, ic->stringToIdentity("BallRedEncoders"));
            AdapterMotors->add(motors, ic->stringToIdentity("BallRedMotors"));

            AdapterEncoders->activate();
            AdapterMotors->activate();

            ic->waitForShutdown();
        } catch (const Ice::Exception& e) {
            std::cerr << e << std::endl;
        } catch (const char* msg) {
            std::cerr << msg << std::endl;
        }
        if (ic) {
            try {
                ic->destroy();
            } catch (const Ice::Exception& e) {
                std::cerr << e << std::endl;
            }
        }
    }
}
