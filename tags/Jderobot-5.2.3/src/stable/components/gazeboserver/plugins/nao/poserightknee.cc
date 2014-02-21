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

#include "poserightknee.h"

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(PoseRightKnee)

    PoseRightKnee::PoseRightKnee () {
        pthread_mutex_init(&this->mutex_rightkneeencoders, NULL);
        pthread_mutex_init(&this->mutex_rightkneemotors, NULL);
        this->cycle = 50;
        this->cfgfile_rightknee = std::string("--Ice.Config=poserightknee.cfg");
        this->modelPitch = std::string("joint_poserightknee_pitch");

        std::cout << "Constructor PoseRightKnee" << std::endl;
    }

    void PoseRightKnee::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        if (!_sdf->HasElement(this->modelPitch))
            gzerr << "PoseRightKnee plugin missing <" << this->modelPitch << "> element\n";
            
        std::string elemPitch = std::string(_sdf->GetElement(this->modelPitch)->GetValueString());
            
        if (!_sdf->HasElement(elemPitch))
            gzerr << "PoseRightKnee plugin missing <" << elemPitch << "> element\n";
            
        this->rightknee.joint_pitch = _model->GetJoint(elemPitch);

        this->maxPitch = (float) this->rightknee.joint_pitch->GetUpperLimit(0).Radian();
        this->minPitch = (float) this->rightknee.joint_pitch->GetLowerLimit(0).Radian();

        // Load torque
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the right knee plugin.\n";
            this->stiffness = 5.0;
        }
        
        pthread_t thr_ice;
        pthread_create(&thr_ice, NULL, &thread_RightKneeICE, (void*) this);

        // Load OnUpdate method
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&PoseRightKnee::OnUpdate, this));
    }

    void PoseRightKnee::Init () {
        this->rightknee.encoders.tilt = 0.0;
        
        this->rightknee.motorsdata.tilt = 0.0;
    }

    void PoseRightKnee::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;
        
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        //          ----------ENCODERS----------
        // GET pose3dencoders data from the right elbow (PAN&ROLL)
        pthread_mutex_lock(&this->mutex_rightkneeencoders);
        
        this->rightknee.encoders.tilt = this->rightknee.joint_pitch->GetAngle(0).Radian();
        
        pthread_mutex_unlock(&this->mutex_rightkneeencoders);

        //          ----------MOTORS----------
        this->rightknee.joint_pitch->SetMaxForce(0, this->stiffness);
        
        pthread_mutex_lock(&this->mutex_rightkneemotors);
        
        float pitchSpeed = - this->rightknee.motorsdata.tilt - this->rightknee.encoders.tilt;
        if ((std::abs(pitchSpeed) < 0.1) && (std::abs(pitchSpeed) > 0.001))
            pitchSpeed = 0.1;
        
        this->rightknee.joint_pitch->SetVelocity(0, pitchSpeed);

        pthread_mutex_unlock(&this->mutex_rightkneemotors);

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;

        diff = (totalb - totala) / 1000;
        diff = cycle - diff;

        if (diff < 10)
            diff = 10;

        sleep(diff / 1000);
    }
    
    class Pose3DEncodersRK : virtual public jderobot::Pose3DEncoders {
    public:

        Pose3DEncodersRK ( gazebo::PoseRightKnee* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncodersRK () {}

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_rightkneeencoders);
            
            pose3DEncodersData->x = pose->rightknee.encoders.x;
            pose3DEncodersData->y = pose->rightknee.encoders.y;
            pose3DEncodersData->z = pose->rightknee.encoders.z;
            pose3DEncodersData->pan = pose->rightknee.encoders.pan;
            pose3DEncodersData->tilt = pose->rightknee.encoders.tilt;
            pose3DEncodersData->roll = pose->rightknee.encoders.roll;
            pose3DEncodersData->clock = pose->rightknee.encoders.clock;
            pose3DEncodersData->maxPan = pose->rightknee.encoders.maxPan;
            pose3DEncodersData->minPan = pose->rightknee.encoders.minPan;
            pose3DEncodersData->maxTilt = pose->rightknee.encoders.maxTilt;
            pose3DEncodersData->minTilt = pose->rightknee.encoders.minTilt;
            
            pthread_mutex_unlock(&pose->mutex_rightkneeencoders);

            return pose3DEncodersData;
        }

        gazebo::PoseRightKnee* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;
    };

    class Pose3DMotorsRK : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotorsRK (gazebo::PoseRightKnee* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotorsRK () {}

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_rightkneemotors);
            
            pose3DMotorsData->x = pose->rightknee.motorsdata.x;
            pose3DMotorsData->y = pose->rightknee.motorsdata.y;
            pose3DMotorsData->z = pose->rightknee.motorsdata.z;
            pose3DMotorsData->pan = pose->rightknee.motorsdata.pan;
            pose3DMotorsData->tilt = pose->rightknee.motorsdata.tilt;
            pose3DMotorsData->roll = pose->rightknee.motorsdata.roll;
            pose3DMotorsData->panSpeed = pose->rightknee.motorsdata.panSpeed;
            pose3DMotorsData->tiltSpeed = pose->rightknee.motorsdata.tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_rightkneemotors);

            return pose3DMotorsData;
        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_rightkneemotors);
            
            pose3DMotorsParams->maxPan = pose->rightknee.motorsparams.maxPan;
            pose3DMotorsParams->minPan = pose->rightknee.motorsparams.minPan;
            pose3DMotorsParams->maxTilt = pose->rightknee.motorsparams.maxTilt;
            pose3DMotorsParams->minTilt = pose->rightknee.motorsparams.minTilt;
            pose3DMotorsParams->maxPanSpeed = pose->rightknee.motorsparams.maxPanSpeed;
            pose3DMotorsParams->maxTiltSpeed = pose->rightknee.motorsparams.maxTiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_rightkneemotors);
            
            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_rightkneemotors);
            
            pose->rightknee.motorsdata.x = data->x;
            pose->rightknee.motorsdata.y = data->y;
            pose->rightknee.motorsdata.z = data->z;
            pose->rightknee.motorsdata.pan = data->pan;
            pose->rightknee.motorsdata.tilt = data->tilt;
            pose->rightknee.motorsdata.roll = data->roll;
            pose->rightknee.motorsdata.panSpeed = data->panSpeed;
            pose->rightknee.motorsdata.tiltSpeed = data->tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_rightkneemotors);
        }

        gazebo::PoseRightKnee* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;
    };

    void* thread_RightKneeICE ( void* v ) {

        gazebo::PoseRightKnee* rightknee = (gazebo::PoseRightKnee*)v;
        char* name = (char*) rightknee->cfgfile_rightknee.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string EndpointsEncoders = prop->getProperty("PoseRightKneeEncoders.Endpoints");
            std::cout << "PoseRightKneeEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("PoseRightKneeMotors.Endpoints");
            std::cout << "PoseRightKneeMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterRightKneeEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterRightKneeMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncodersRK(rightknee);
            Ice::ObjectPtr motors = new Pose3DMotorsRK(rightknee);

            AdapterEncoders->add(encoders, ic->stringToIdentity("RightKneeEncoders"));
            AdapterMotors->add(motors, ic->stringToIdentity("RightKneeMotors"));

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
