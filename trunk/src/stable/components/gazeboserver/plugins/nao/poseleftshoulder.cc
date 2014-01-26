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

#include "poseleftshoulder.h"

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(PoseLeftShoulder)

    PoseLeftShoulder::PoseLeftShoulder () {
        pthread_mutex_init(&this->mutex_leftshoulderencoders, NULL);
        pthread_mutex_init(&this->mutex_leftshouldermotors, NULL);
        this->cycle = 50;
        this->cfgfile_leftshoulder = std::string("--Ice.Config=poseleftshoulder.cfg");
        this->modelPitch = std::string("joint_poseleftshoulder_tilt");
        this->modelRoll = std::string("joint_poseleftshoulder_roll");

        std::cout << "Constructor PoseLeftShoulder" << std::endl;
    }

    void PoseLeftShoulder::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        if (!_sdf->HasElement(this->modelPitch))
            gzerr << "PoseLeftShoulder plugin missing <" << this->modelPitch << "> element\n";
        if (!_sdf->HasElement(this->modelRoll))
            gzerr << "PoseLeftShoulder plugin missing <" << this->modelRoll << "> element\n";
         
        std::string elemPitch = std::string(_sdf->GetElement(this->modelPitch)->GetValueString());
        std::string elemRoll = std::string(_sdf->GetElement(this->modelRoll)->GetValueString());
            
        if (!_sdf->HasElement(elemPitch))
            gzerr << "PoseLeftShoulder plugin missing <" << elemPitch << "> element\n";
        if (!_sdf->HasElement(elemRoll))
            gzerr << "PoseLeftShoulder plugin missing <" << elemRoll << "> element\n";
            
        this->leftshoulder.joint_pitch = _model->GetJoint(elemPitch);
        this->leftshoulder.joint_roll = _model->GetJoint(elemRoll);

        this->maxPitch = (float) this->leftshoulder.joint_pitch->GetUpperLimit(0).Radian();
        this->minPitch = (float) this->leftshoulder.joint_pitch->GetLowerLimit(0).Radian();
        this->maxRoll = (float) this->leftshoulder.joint_roll->GetUpperLimit(0).Radian();
        this->minRoll = (float) this->leftshoulder.joint_roll->GetLowerLimit(0).Radian();

        // Load torque
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for left shoulder.\n";
            this->stiffness = 5.0;
        }
        
        pthread_t thr_gui;
        pthread_create(&thr_gui, NULL, &thread_LeftShoulderICE, (void*) this);

        // Load OnUpdate method
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&PoseLeftShoulder::OnUpdate, this));
    }

    void PoseLeftShoulder::Init () {
        this->leftshoulder.encoders.tilt = 0.0;
        this->leftshoulder.encoders.roll = 0.0;
        
        this->leftshoulder.motorsdata.tilt = 0.0;
        this->leftshoulder.motorsdata.roll = 0.0;
    }

    void PoseLeftShoulder::OnUpdate() {
        long totalb, totala, diff;
        struct timeval a, b;

        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        //          ----------ENCODERS----------
        // GET pose3dencoders data from the left shoulder (ROLL&TILT)
        pthread_mutex_lock(&this->mutex_leftshoulderencoders);
        
        this->leftshoulder.encoders.tilt = this->leftshoulder.joint_pitch->GetAngle(0).Radian();
        this->leftshoulder.encoders.roll = this->leftshoulder.joint_roll->GetAngle(0).Radian();
        
        pthread_mutex_unlock(&this->mutex_leftshoulderencoders);

        //          ----------MOTORS----------
        this->leftshoulder.joint_pitch->SetMaxForce(0, this->stiffness);
        this->leftshoulder.joint_roll->SetMaxForce(0, this->stiffness);
        
        pthread_mutex_lock(&this->mutex_leftshouldermotors);
        
        float pitchSpeed = - this->leftshoulder.motorsdata.tilt - this->leftshoulder.encoders.tilt;
        if ((std::abs(pitchSpeed) < 0.1) && (std::abs(pitchSpeed) > 0.001))
            pitchSpeed = 0.1;
        
        float rollSpeed = - this->leftshoulder.motorsdata.roll - this->leftshoulder.encoders.roll;
        if ((std::abs(rollSpeed) < 0.1) && (std::abs(rollSpeed) > 0.001))
            rollSpeed = 0.1;
        
        this->leftshoulder.joint_pitch->SetVelocity(0, pitchSpeed);
        this->leftshoulder.joint_roll->SetVelocity(0, rollSpeed);

        pthread_mutex_unlock(&this->mutex_leftshouldermotors);

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;

        diff = (totalb - totala) / 1000;
        diff = cycle - diff;

        if (diff < 10)
            diff = 10;

        sleep(diff / 1000);
    }
    
    class Pose3DEncodersLS : virtual public jderobot::Pose3DEncoders {
    public:

        Pose3DEncodersLS ( gazebo::PoseLeftShoulder* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncodersLS () {}

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftshoulderencoders);
            
            pose3DEncodersData->x = pose->leftshoulder.encoders.x;
            pose3DEncodersData->y = pose->leftshoulder.encoders.y;
            pose3DEncodersData->z = pose->leftshoulder.encoders.z;
            pose3DEncodersData->pan = pose->leftshoulder.encoders.pan;
            pose3DEncodersData->tilt = pose->leftshoulder.encoders.tilt;
            pose3DEncodersData->roll = pose->leftshoulder.encoders.roll;
            pose3DEncodersData->clock = pose->leftshoulder.encoders.clock;
            pose3DEncodersData->maxPan = pose->leftshoulder.encoders.maxPan;
            pose3DEncodersData->minPan = pose->leftshoulder.encoders.minPan;
            pose3DEncodersData->maxTilt = pose->leftshoulder.encoders.maxTilt;
            pose3DEncodersData->minTilt = pose->leftshoulder.encoders.minTilt;
            
            pthread_mutex_unlock(&pose->mutex_leftshoulderencoders);

            return pose3DEncodersData;
        }

        gazebo::PoseLeftShoulder* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;
    };

    class Pose3DMotorsLS : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotorsLS (gazebo::PoseLeftShoulder* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotorsLS () {}

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftshouldermotors);
            
            pose3DMotorsData->x = pose->leftshoulder.motorsdata.x;
            pose3DMotorsData->y = pose->leftshoulder.motorsdata.y;
            pose3DMotorsData->z = pose->leftshoulder.motorsdata.z;
            pose3DMotorsData->pan = pose->leftshoulder.motorsdata.pan;
            pose3DMotorsData->tilt = pose->leftshoulder.motorsdata.tilt;
            pose3DMotorsData->roll = pose->leftshoulder.motorsdata.roll;
            pose3DMotorsData->panSpeed = pose->leftshoulder.motorsdata.panSpeed;
            pose3DMotorsData->tiltSpeed = pose->leftshoulder.motorsdata.tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftshouldermotors);

            return pose3DMotorsData;
        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftshouldermotors);
            
            pose3DMotorsParams->maxPan = pose->leftshoulder.motorsparams.maxPan;
            pose3DMotorsParams->minPan = pose->leftshoulder.motorsparams.minPan;
            pose3DMotorsParams->maxTilt = pose->leftshoulder.motorsparams.maxTilt;
            pose3DMotorsParams->minTilt = pose->leftshoulder.motorsparams.minTilt;
            pose3DMotorsParams->maxPanSpeed = pose->leftshoulder.motorsparams.maxPanSpeed;
            pose3DMotorsParams->maxTiltSpeed = pose->leftshoulder.motorsparams.maxTiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftshouldermotors);
            
            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftshouldermotors);
            
            pose->leftshoulder.motorsdata.x = data->x;
            pose->leftshoulder.motorsdata.y = data->y;
            pose->leftshoulder.motorsdata.z = data->z;
            pose->leftshoulder.motorsdata.pan = data->pan;
            pose->leftshoulder.motorsdata.tilt = data->tilt;
            pose->leftshoulder.motorsdata.roll = data->roll;
            pose->leftshoulder.motorsdata.panSpeed = data->panSpeed;
            pose->leftshoulder.motorsdata.tiltSpeed = data->tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftshouldermotors);
        }

        gazebo::PoseLeftShoulder* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;
    };

    void* thread_LeftShoulderICE ( void* v ) {
        gazebo::PoseLeftShoulder* leftshoulder = (gazebo::PoseLeftShoulder*)v;
        char* name = (char*) leftshoulder->cfgfile_leftshoulder.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string EndpointsEncoders = prop->getProperty("PoseLeftShoulderEncoders.Endpoints");
            std::cout << "PoseLeftShoulderEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("PoseLeftShoulderMotors.Endpoints");
            std::cout << "PoseLeftShoulderMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterLeftShoulderEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterLeftShoulderMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncodersLS(leftshoulder);
            Ice::ObjectPtr motors = new Pose3DMotorsLS(leftshoulder);

            AdapterEncoders->add(encoders, ic->stringToIdentity("LeftShoulderEncoders"));
            AdapterMotors->add(motors, ic->stringToIdentity("LeftShoulderMotors"));

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
