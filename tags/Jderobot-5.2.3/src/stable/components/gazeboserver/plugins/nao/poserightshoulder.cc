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

#include "poserightshoulder.h"

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(PoseRightShoulder)

    PoseRightShoulder::PoseRightShoulder () {
        pthread_mutex_init(&this->mutex_rightshoulderencoders, NULL);
        pthread_mutex_init(&this->mutex_rightshouldermotors, NULL);
        this->cycle = 50;
        this->cfgfile_rightshoulder = std::string("--Ice.Config=poserightshoulder.cfg");
        
        this->modelPitch = std::string("joint_poserightshoulder_pitch");
        this->modelRoll = std::string("joint_poserightshoulder_roll");

        std::cout << "Constructor PoseRightShoulder" << std::endl;
    }

    void PoseRightShoulder::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        if (!_sdf->HasElement(this->modelPitch))
            gzerr << "PoseRightShoulder plugin missing <" << this->modelPitch << "> element\n";
        if (!_sdf->HasElement(this->modelRoll))
            gzerr << "PoseRightShoulder plugin missing <" << this->modelRoll << "> element\n";
        
        std::string elemPitch = std::string(_sdf->GetElement(this->modelPitch)->GetValueString());
        std::string elemRoll = std::string(_sdf->GetElement(this->modelRoll)->GetValueString());
            
        if (!_sdf->HasElement(elemPitch))
            gzerr << "PoseRightShoulder plugin missing <" << elemPitch << "> element\n";
        if (!_sdf->HasElement(elemRoll))
            gzerr << "PoseRightShoulder plugin missing <" << elemRoll << "> element\n";
            
        this->rightshoulder.joint_pitch = _model->GetJoint(elemPitch);
        this->rightshoulder.joint_roll = _model->GetJoint(elemRoll);

        this->maxPitch = (float) this->rightshoulder.joint_pitch->GetUpperLimit(0).Radian();
        this->minPitch = (float) this->rightshoulder.joint_pitch->GetLowerLimit(0).Radian();
        this->maxRoll = (float) this->rightshoulder.joint_roll->GetUpperLimit(0).Radian();
        this->minRoll = (float) this->rightshoulder.joint_roll->GetLowerLimit(0).Radian();

        // Load torque
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the DiffDrive plugin.\n";
            this->stiffness = 5.0;
        }
        
        pthread_t thr_ice;
        pthread_create(&thr_ice, NULL, &thread_RightShoulderICE, (void*) this);

        // Load OnUpdate method
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
            boost::bind(&PoseRightShoulder::OnUpdate, this));

    }

    void PoseRightShoulder::Init () {
        this->rightshoulder.encoders.tilt = 0.0;
        this->rightshoulder.encoders.roll = 0.0;
        
        this->rightshoulder.motorsdata.tilt = 0.0;
        this->rightshoulder.motorsdata.roll = 0.0;
    }

    void PoseRightShoulder::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;
        
        gettimeofday(&a, NULL);

        //          ----------ENCODERS----------
        // GET pose3dencoders data from the right shoulder (ROLL&TILT)
        pthread_mutex_lock(&this->mutex_rightshoulderencoders);
        
        this->rightshoulder.encoders.tilt = this->rightshoulder.joint_pitch->GetAngle(0).Radian();
        this->rightshoulder.encoders.roll = this->rightshoulder.joint_roll->GetAngle(0).Radian();
        
        pthread_mutex_unlock(&this->mutex_rightshoulderencoders);
        
        //          ----------MOTORS----------
        this->rightshoulder.joint_pitch->SetMaxForce(0, this->stiffness);
        this->rightshoulder.joint_roll->SetMaxForce(0, this->stiffness);
        
        pthread_mutex_lock(&this->mutex_rightshouldermotors);
        
        float pitchSpeed = - this->rightshoulder.motorsdata.tilt - this->rightshoulder.encoders.tilt;
        if ((std::abs(pitchSpeed) > 0.001) && (std::abs(pitchSpeed) < 0.1))
            pitchSpeed = 0.1;
        
        float rollSpeed = - this->rightshoulder.motorsdata.roll - this->rightshoulder.encoders.roll;
        if ((std::abs(rollSpeed) > 0.001) && (std::abs(rollSpeed) < 0.1))
            rollSpeed = 0.1;
        
        this->rightshoulder.joint_pitch->SetVelocity(0, pitchSpeed);
        this->rightshoulder.joint_roll->SetVelocity(0, rollSpeed);

        pthread_mutex_unlock(&this->mutex_rightshouldermotors);

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;

        diff = (totalb - totala) / 1000;
        diff = cycle - diff;

        if (diff < 10)
            diff = 10;

        sleep(diff / 1000);
    }
    
    class Pose3DEncodersRS : virtual public jderobot::Pose3DEncoders {
    public:

        Pose3DEncodersRS ( gazebo::PoseRightShoulder* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncodersRS () {}

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_rightshoulderencoders);
            
            pose3DEncodersData->x = pose->rightshoulder.encoders.x;
            pose3DEncodersData->y = pose->rightshoulder.encoders.y;
            pose3DEncodersData->z = pose->rightshoulder.encoders.z;
            pose3DEncodersData->pan = pose->rightshoulder.encoders.pan;
            pose3DEncodersData->tilt = pose->rightshoulder.encoders.tilt;
            pose3DEncodersData->roll = pose->rightshoulder.encoders.roll;
            pose3DEncodersData->clock = pose->rightshoulder.encoders.clock;
            pose3DEncodersData->maxPan = pose->rightshoulder.encoders.maxPan;
            pose3DEncodersData->minPan = pose->rightshoulder.encoders.minPan;
            pose3DEncodersData->maxTilt = pose->rightshoulder.encoders.maxTilt;
            pose3DEncodersData->minTilt = pose->rightshoulder.encoders.minTilt;
            
            pthread_mutex_unlock(&pose->mutex_rightshoulderencoders);

            return pose3DEncodersData;
        }

        gazebo::PoseRightShoulder* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;
    };

    class Pose3DMotorsRS : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotorsRS (gazebo::PoseRightShoulder* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotorsRS() {}

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_rightshouldermotors);
            
            pose3DMotorsData->x = pose->rightshoulder.motorsdata.x;
            pose3DMotorsData->y = pose->rightshoulder.motorsdata.y;
            pose3DMotorsData->z = pose->rightshoulder.motorsdata.z;
            pose3DMotorsData->pan = pose->rightshoulder.motorsdata.pan;
            pose3DMotorsData->tilt = pose->rightshoulder.motorsdata.tilt;
            pose3DMotorsData->roll = pose->rightshoulder.motorsdata.roll;
            pose3DMotorsData->panSpeed = pose->rightshoulder.motorsdata.panSpeed;
            pose3DMotorsData->tiltSpeed = pose->rightshoulder.motorsdata.tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_rightshouldermotors);

            return pose3DMotorsData;
        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_rightshouldermotors);
            
            pose3DMotorsParams->maxPan = pose->rightshoulder.motorsparams.maxPan;
            pose3DMotorsParams->minPan = pose->rightshoulder.motorsparams.minPan;
            pose3DMotorsParams->maxTilt = pose->rightshoulder.motorsparams.maxTilt;
            pose3DMotorsParams->minTilt = pose->rightshoulder.motorsparams.minTilt;
            pose3DMotorsParams->maxPanSpeed = pose->rightshoulder.motorsparams.maxPanSpeed;
            pose3DMotorsParams->maxTiltSpeed = pose->rightshoulder.motorsparams.maxTiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_rightshouldermotors);
            
            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_rightshouldermotors);
            
            pose->rightshoulder.motorsdata.x = data->x;
            pose->rightshoulder.motorsdata.y = data->y;
            pose->rightshoulder.motorsdata.z = data->z;
            pose->rightshoulder.motorsdata.pan = data->pan;
            pose->rightshoulder.motorsdata.tilt = data->tilt;
            pose->rightshoulder.motorsdata.roll = data->roll;
            pose->rightshoulder.motorsdata.panSpeed = data->panSpeed;
            pose->rightshoulder.motorsdata.tiltSpeed = data->tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_rightshouldermotors);
        }

        gazebo::PoseRightShoulder* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;
    };

    void* thread_RightShoulderICE ( void* v ) {
        gazebo::PoseRightShoulder* rightshoulder = (gazebo::PoseRightShoulder*)v;
        char* name = (char*) rightshoulder->cfgfile_rightshoulder.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string EndpointsEncoders = prop->getProperty("PoseRightShoulderEncoders.Endpoints");
            std::cout << "PoseRightShoulderEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("PoseRightShoulderMotors.Endpoints");
            std::cout << "PoseRightShoulderMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterRightShoulderEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterRightShoulderMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncodersRS(rightshoulder);
            Ice::ObjectPtr motors = new Pose3DMotorsRS(rightshoulder);

            AdapterEncoders->add(encoders, ic->stringToIdentity("RightShoulderEncoders"));
            AdapterMotors->add(motors, ic->stringToIdentity("RightShoulderMotors"));

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
