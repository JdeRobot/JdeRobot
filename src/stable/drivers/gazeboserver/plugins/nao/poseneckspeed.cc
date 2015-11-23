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

#include "poseneckspeed.h"

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(PoseNeckSpeed)

    PoseNeckSpeed::PoseNeckSpeed () {
        pthread_mutex_init(&this->mutex_neckspeedencoders, NULL);
        pthread_mutex_init(&this->mutex_neckspeedmotors, NULL);
        this->cycle = 50;
        this->neckspeed.motorsdata.panSpeed = 0.0;
        this->neckspeed.motorsdata.tiltSpeed = 0.0;
        this->cfgfile_neckspeed = std::string("--Ice.Config=poseneckspeed.cfg");
        this->modelPan = std::string("joint_poseneckspeed_pan");
        this->modelTilt = std::string("joint_poseneckspeed_tilt");

        std::cout << "Constructor PoseNeckSpeed" << std::endl;
    }

    void PoseNeckSpeed::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        if (!_sdf->HasElement(this->modelPan))
            gzerr << "PoseNeckSpeed plugin missing <" << this->modelPan << "> element\n";
        if (!_sdf->HasElement(this->modelTilt))
            gzerr << "PoseNeckSpeed plugin missing <" << this->modelTilt << "> element\n";
            
        std::string elemPan = std::string(_sdf->GetElement(this->modelPan)->GetValueString());
        std::string elemTilt = std::string(_sdf->GetElement(this->modelTilt)->GetValueString());
            
        if (!_sdf->HasElement(elemPan))
            gzerr << "PoseNeck plugin missing <" << elemPan << "> element\n";
        if (!_sdf->HasElement(elemTilt))
            gzerr << "PoseNeck plugin missing <" << elemTilt << "> element\n";
            
        this->neckspeed.joint_pan = _model->GetJoint(elemPan);
        this->neckspeed.joint_tilt = _model->GetJoint(elemTilt);

        this->neckspeed.motorsparams.maxPan = (float) this->neckspeed.joint_pan->GetUpperLimit(0).Radian();
        this->neckspeed.motorsparams.minPan = (float) this->neckspeed.joint_pan->GetLowerLimit(0).Radian();
        this->neckspeed.motorsparams.maxTilt = (float) this->neckspeed.joint_tilt->GetUpperLimit(0).Radian();
        this->neckspeed.motorsparams.minTilt = (float) this->neckspeed.joint_tilt->GetLowerLimit(0).Radian();

        // Load torque        
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the neck speed plugin.\n";
            this->stiffness = 5.0;
        }
        
        pthread_t thr_ice;
        pthread_create(&thr_ice, NULL, &thread_NeckSpeedICE, (void*) this);

        // Load OnUpdate method
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&PoseNeckSpeed::OnUpdate, this));
    }

    void PoseNeckSpeed::Init () {
        this->neckspeed.joint_pan->SetVelocity(0, this->neckspeed.motorsdata.panSpeed);
        this->neckspeed.joint_pan->SetMaxForce(0, this->stiffness);
        
        this->neckspeed.joint_tilt->SetVelocity(0, this->neckspeed.motorsdata.tiltSpeed);
        this->neckspeed.joint_tilt->SetMaxForce(0, this->stiffness);
    }

    void PoseNeckSpeed::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;

        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;
        
        //          ----------ENCODERS----------
        // GET pose data from the neck (PAN&TILT)
        pthread_mutex_lock(&this->mutex_neckspeedencoders);

        this->neckspeed.encoders.pan = this->neckspeed.joint_pan->GetAngle(0).Radian();
        this->neckspeed.encoders.tilt = this->neckspeed.joint_tilt->GetAngle(0).Radian();
        
        pthread_mutex_unlock(&this->mutex_neckspeedencoders);
  
        //          ----------MOTORS----------
        this->neckspeed.joint_pan->SetMaxForce(0, this->stiffness);
        this->neckspeed.joint_tilt->SetMaxForce(0, this->stiffness);
        
        pthread_mutex_lock(&this->mutex_neckspeedmotors);
     
        this->neckspeed.joint_pan->SetVelocity(0, -this->neckspeed.motorsdata.panSpeed);
        this->neckspeed.joint_tilt->SetVelocity(0, this->neckspeed.motorsdata.tiltSpeed);

        pthread_mutex_unlock(&this->mutex_neckspeedmotors);

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;

        diff = (totalb - totala) / 1000;
        diff = cycle - diff;

        if (diff < 10)
            diff = 10;

        sleep(diff / 1000);
    }
    
    class Pose3DEncodersNS : virtual public jderobot::Pose3DEncoders {
    public:

        Pose3DEncodersNS ( gazebo::PoseNeckSpeed* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncodersNS () {}

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_neckspeedencoders);
            
            pose3DEncodersData->x = pose->neckspeed.encoders.x;
            pose3DEncodersData->y = pose->neckspeed.encoders.y;
            pose3DEncodersData->z = pose->neckspeed.encoders.z;
            pose3DEncodersData->pan = pose->neckspeed.encoders.pan;
            pose3DEncodersData->tilt = pose->neckspeed.encoders.tilt;
            pose3DEncodersData->roll = pose->neckspeed.encoders.roll;
            pose3DEncodersData->clock = pose->neckspeed.encoders.clock;
            pose3DEncodersData->maxPan = pose->neckspeed.encoders.maxPan;
            pose3DEncodersData->minPan = pose->neckspeed.encoders.minPan;
            pose3DEncodersData->maxTilt = pose->neckspeed.encoders.maxTilt;
            pose3DEncodersData->minTilt = pose->neckspeed.encoders.minTilt;
            
            pthread_mutex_unlock(&pose->mutex_neckspeedencoders);

            return pose3DEncodersData;
        }

        gazebo::PoseNeckSpeed* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;
    };

    class Pose3DMotorsNS : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotorsNS (gazebo::PoseNeckSpeed* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotorsNS () {}

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_neckspeedmotors);
            
            pose3DMotorsData->x = pose->neckspeed.motorsdata.x;
            pose3DMotorsData->y = pose->neckspeed.motorsdata.y;
            pose3DMotorsData->z = pose->neckspeed.motorsdata.z;
            pose3DMotorsData->pan = pose->neckspeed.motorsdata.pan;
            pose3DMotorsData->tilt = pose->neckspeed.motorsdata.tilt;
            pose3DMotorsData->roll = pose->neckspeed.motorsdata.roll;
            pose3DMotorsData->panSpeed = pose->neckspeed.motorsdata.panSpeed;
            pose3DMotorsData->tiltSpeed = pose->neckspeed.motorsdata.tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_neckspeedmotors);

            return pose3DMotorsData;
        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_neckspeedmotors);
            
            pose3DMotorsParams->maxPan = pose->neckspeed.motorsparams.maxPan;
            pose3DMotorsParams->minPan = pose->neckspeed.motorsparams.minPan;
            pose3DMotorsParams->maxTilt = pose->neckspeed.motorsparams.maxTilt;
            pose3DMotorsParams->minTilt = pose->neckspeed.motorsparams.minTilt;
            pose3DMotorsParams->maxPanSpeed = pose->neckspeed.motorsparams.maxPanSpeed;
            pose3DMotorsParams->maxTiltSpeed = pose->neckspeed.motorsparams.maxTiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_neckspeedmotors);
            
            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_neckspeedmotors);
            
            pose->neckspeed.motorsdata.x = data->x;
            pose->neckspeed.motorsdata.y = data->y;
            pose->neckspeed.motorsdata.z = data->z;
            pose->neckspeed.motorsdata.pan = data->pan;
            pose->neckspeed.motorsdata.tilt = data->tilt;
            pose->neckspeed.motorsdata.roll = data->roll;
            pose->neckspeed.motorsdata.panSpeed = data->panSpeed;
            pose->neckspeed.motorsdata.tiltSpeed = data->tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_neckspeedmotors);
        }

        gazebo::PoseNeckSpeed* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;
    };

    void* thread_NeckSpeedICE ( void* v ) {
        gazebo::PoseNeckSpeed* neckspeed = (gazebo::PoseNeckSpeed*)v;
        char* name = (char*) neckspeed->cfgfile_neckspeed.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string EndpointsEncoders = prop->getProperty("PoseNeckSpeedEncoders.Endpoints");
            std::cout << "PoseNeckSpeedEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("PoseNeckSpeedMotors.Endpoints");
            std::cout << "PoseNeckSpeedMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterNeckSpeedEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterNeckSpeedMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncodersNS(neckspeed);
            Ice::ObjectPtr motors = new Pose3DMotorsNS(neckspeed);

            AdapterEncoders->add(encoders, ic->stringToIdentity("NeckSpeedEncoders"));
            AdapterMotors->add(motors, ic->stringToIdentity("NeckSpeedMotors"));

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
