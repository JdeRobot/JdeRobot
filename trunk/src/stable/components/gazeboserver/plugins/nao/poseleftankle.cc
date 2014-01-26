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

#include "poseleftankle.h"

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(PoseLeftAnkle)

    PoseLeftAnkle::PoseLeftAnkle () {
        pthread_mutex_init(&mutex_leftankleencoders, NULL);
        pthread_mutex_init(&mutex_leftanklemotors, NULL);
        this->cycle = 50;
        this->cfgfile_leftankle = std::string("--Ice.Config=poseleftankle.cfg");
        this->modelPitch = std::string("joint_poseleftankle_pitch");
        this->modelRoll = std::string("joint_poseleftankle_roll");

        std::cout << "Constructor PoseLeftAnkle" << std::endl;
    }

    void PoseLeftAnkle::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        if (!_sdf->HasElement(this->modelPitch))
            gzerr << "PoseLeftAnkle plugin missing <" << this->modelPitch << "> element\n";
        if (!_sdf->HasElement(this->modelRoll))
            gzerr << "PoseLeftAnkle plugin missing <" << this->modelRoll << "> element\n";
            
        std::string elemPitch = std::string(_sdf->GetElement(this->modelPitch)->GetValueString());
        std::string elemRoll = std::string(_sdf->GetElement(this->modelRoll)->GetValueString());
        
        if (!_sdf->HasElement(elemPitch))
            gzerr << "PoseLeftAnkle plugin missing <" << elemPitch << "> element\n";
        if (!_sdf->HasElement(elemRoll))
            gzerr << "PoseLeftAnkle plugin missing <" << elemRoll << "> element\n";
            
        this->leftankle.joint_pitch = _model->GetJoint(elemPitch);
        this->leftankle.joint_roll = _model->GetJoint(elemRoll);

        this->maxPitch = (float) this->leftankle.joint_pitch->GetUpperLimit(0).Radian();
        this->minPitch = (float) this->leftankle.joint_pitch->GetLowerLimit(0).Radian();
        this->maxRoll = (float) this->leftankle.joint_roll->GetUpperLimit(0).Radian();
        this->minRoll = (float) this->leftankle.joint_roll->GetLowerLimit(0).Radian();

        // Load torque
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the left ankle plugin.\n";
            this->stiffness = 5.0;
        }
        
        pthread_t thr_ice;
        pthread_create(&thr_ice, NULL, &thread_LeftAnkleICE, (void*) this);

        // Load OnUpdate method
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&PoseLeftAnkle::OnUpdate, this));
    }

    void PoseLeftAnkle::Init () {
        this->leftankle.encoders.tilt = 0.0;
        this->leftankle.encoders.roll = 0.0;
        
        this->leftankle.motorsdata.tilt = 0.0;
        this->leftankle.motorsdata.roll = 0.0;
    }

    void PoseLeftAnkle::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;

        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        //          ----------ENCODERS----------
        // GET pose3dencoders data from the left ankle (TILT&ROLL)
        pthread_mutex_lock(&this->mutex_leftankleencoders);
        
        this->leftankle.encoders.tilt = this->leftankle.joint_pitch->GetAngle(0).Radian();
        this->leftankle.encoders.roll = this->leftankle.joint_roll->GetAngle(0).Radian();   
        
        pthread_mutex_unlock(&this->mutex_leftankleencoders);

        //          ----------MOTORS----------
        this->leftankle.joint_pitch->SetMaxForce(0, this->stiffness);
        this->leftankle.joint_roll->SetMaxForce(0, this->stiffness);
        
        pthread_mutex_lock(&this->mutex_leftanklemotors);
        
        float tiltSpeed = - this->leftankle.motorsdata.tilt - this->leftankle.encoders.tilt;
        if ((std::abs(tiltSpeed) < 0.1) && (std::abs(tiltSpeed) > 0.001))
            tiltSpeed = 0.1;
        
        float rollSpeed = - this->leftankle.motorsdata.roll - this->leftankle.encoders.roll;
        if ((std::abs(rollSpeed) < 0.1) && (std::abs(rollSpeed) > 0.001))
            rollSpeed = 0.1;
        
        this->leftankle.joint_pitch->SetVelocity(0, tiltSpeed);
        this->leftankle.joint_roll->SetVelocity(0, rollSpeed);

        pthread_mutex_unlock(&this->mutex_leftanklemotors);

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;

        diff = (totalb - totala) / 1000;
        diff = cycle - diff;

        if (diff < 10)
            diff = 10;

        //usleep(diff*1000);
        sleep(diff / 1000);
    }
    
    class Pose3DEncodersLA : virtual public jderobot::Pose3DEncoders {
    public:

        Pose3DEncodersLA ( gazebo::PoseLeftAnkle* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncodersLA () {}

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftankleencoders);
            
            pose3DEncodersData->x = pose->leftankle.encoders.x;
            pose3DEncodersData->y = pose->leftankle.encoders.y;
            pose3DEncodersData->z = pose->leftankle.encoders.z;
            pose3DEncodersData->pan = pose->leftankle.encoders.pan;
            pose3DEncodersData->tilt = pose->leftankle.encoders.tilt;
            pose3DEncodersData->roll = pose->leftankle.encoders.roll;
            pose3DEncodersData->clock = pose->leftankle.encoders.clock;
            pose3DEncodersData->maxPan = pose->leftankle.encoders.maxPan;
            pose3DEncodersData->minPan = pose->leftankle.encoders.minPan;
            pose3DEncodersData->maxTilt = pose->leftankle.encoders.maxTilt;
            pose3DEncodersData->minTilt = pose->leftankle.encoders.minTilt;
            
            pthread_mutex_unlock(&pose->mutex_leftankleencoders);

            return pose3DEncodersData;
        }

        gazebo::PoseLeftAnkle* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;
    };

    class Pose3DMotorsLA : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotorsLA (gazebo::PoseLeftAnkle* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotorsLA () {}

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftanklemotors);
            
            pose3DMotorsData->x = pose->leftankle.motorsdata.x;
            pose3DMotorsData->y = pose->leftankle.motorsdata.y;
            pose3DMotorsData->z = pose->leftankle.motorsdata.z;
            pose3DMotorsData->pan = pose->leftankle.motorsdata.pan;
            pose3DMotorsData->tilt = pose->leftankle.motorsdata.tilt;
            pose3DMotorsData->roll = pose->leftankle.motorsdata.roll;
            pose3DMotorsData->panSpeed = pose->leftankle.motorsdata.panSpeed;
            pose3DMotorsData->tiltSpeed = pose->leftankle.motorsdata.tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftanklemotors);

            return pose3DMotorsData;
        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftanklemotors);
            
            pose3DMotorsParams->maxPan = pose->leftankle.motorsparams.maxPan;
            pose3DMotorsParams->minPan = pose->leftankle.motorsparams.minPan;
            pose3DMotorsParams->maxTilt = pose->leftankle.motorsparams.maxTilt;
            pose3DMotorsParams->minTilt = pose->leftankle.motorsparams.minTilt;
            pose3DMotorsParams->maxPanSpeed = pose->leftankle.motorsparams.maxPanSpeed;
            pose3DMotorsParams->maxTiltSpeed = pose->leftankle.motorsparams.maxTiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftanklemotors);
            
            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftanklemotors);
            
            pose->leftankle.motorsdata.x = data->x;
            pose->leftankle.motorsdata.y = data->y;
            pose->leftankle.motorsdata.z = data->z;
            pose->leftankle.motorsdata.pan = data->pan;
            pose->leftankle.motorsdata.tilt = data->tilt;
            pose->leftankle.motorsdata.roll = data->roll;
            pose->leftankle.motorsdata.panSpeed = data->panSpeed;
            pose->leftankle.motorsdata.tiltSpeed = data->tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftanklemotors);
        }

        gazebo::PoseLeftAnkle* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;
    };

    void* thread_LeftAnkleICE ( void* v ) {

        gazebo::PoseLeftAnkle* leftankle = (gazebo::PoseLeftAnkle*)v;
        char* name = (char*) leftankle->cfgfile_leftankle.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string EndpointsEncoders = prop->getProperty("PoseLeftAnkleEncoders.Endpoints");
            std::cout << "PoseLeftAnkleEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("PoseLeftAnkleMotors.Endpoints");
            std::cout << "PoseLeftAnkleMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterLeftAnkleEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterLeftAnkleMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncodersLA(leftankle);
            Ice::ObjectPtr motors = new Pose3DMotorsLA(leftankle);

            AdapterEncoders->add(encoders, ic->stringToIdentity("LeftAnkleEncoders"));
            AdapterMotors->add(motors, ic->stringToIdentity("LeftAnkleMotors"));

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
