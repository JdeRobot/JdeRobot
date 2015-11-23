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

#include "poseleftknee.h"

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(PoseLeftKnee)

    PoseLeftKnee::PoseLeftKnee () {
        pthread_mutex_init(&this->mutex_leftkneeencoders, NULL);
        pthread_mutex_init(&this->mutex_leftkneemotors, NULL);
        this->cycle = 50;
        this->cfgfile_leftknee = std::string("--Ice.Config=poseleftknee.cfg");
        this->modelPitch = std::string("joint_poseleftknee_pitch");

        std::cout << "Constructor PoseLeftKnee" << std::endl;
    }

    void PoseLeftKnee::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        if (!_sdf->HasElement(this->modelPitch))
            gzerr << "PoseLeftKnee plugin missing <" << this->modelPitch << "> element\n";
            
        std::string elemPitch = std::string(_sdf->GetElement(this->modelPitch)->GetValueString());
            
        if (!_sdf->HasElement(elemPitch))
            gzerr << "PoseLeftKnee plugin missing <" << elemPitch << "> element\n";
            
        this->leftknee.joint_pitch = _model->GetJoint(elemPitch);

        this->maxPitch = (float) this->leftknee.joint_pitch->GetUpperLimit(0).Radian();
        this->minPitch = (float) this->leftknee.joint_pitch->GetLowerLimit(0).Radian();

        // Load torque
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the left knee plugin.\n";
            this->stiffness = 5.0;
        }
        
        pthread_t thr_ice;
        pthread_create(&thr_ice, NULL, &thread_LeftKneeICE, (void*) this);

        // Load OnUpdate method
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&PoseLeftKnee::OnUpdate, this));
    }

    void PoseLeftKnee::Init () {
        this->leftknee.encoders.tilt = 0.0;
        
        this->leftknee.motorsdata.tilt = 0.0;
    }

    void PoseLeftKnee::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;
        
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        //          ----------ENCODERS----------
        // GET pose3dencoders data from the right elbow (PAN&ROLL)
        pthread_mutex_lock(&this->mutex_leftkneeencoders);
        
        this->leftknee.encoders.tilt = this->leftknee.joint_pitch->GetAngle(0).Radian();
        
        pthread_mutex_unlock(&this->mutex_leftkneeencoders);

        //          ----------MOTORS----------
        this->leftknee.joint_pitch->SetMaxForce(0, this->stiffness);
        
        pthread_mutex_lock(&this->mutex_leftkneemotors);
        
        float pitchSpeed = - this->leftknee.motorsdata.tilt - this->leftknee.encoders.tilt;
        if ((std::abs(pitchSpeed) < 0.1) && (std::abs(pitchSpeed) > 0.001))
            pitchSpeed = 0.1;
        
        this->leftknee.joint_pitch->SetVelocity(0, pitchSpeed);

        pthread_mutex_unlock(&this->mutex_leftkneemotors);

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;

        diff = (totalb - totala) / 1000;
        diff = cycle - diff;

        if (diff < 10)
            diff = 10;

        sleep(diff / 1000);
    }
    
    class Pose3DEncodersLK : virtual public jderobot::Pose3DEncoders {
    public:

        Pose3DEncodersLK ( gazebo::PoseLeftKnee* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncodersLK () {}

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftkneeencoders);
            
            pose3DEncodersData->x = pose->leftknee.encoders.x;
            pose3DEncodersData->y = pose->leftknee.encoders.y;
            pose3DEncodersData->z = pose->leftknee.encoders.z;
            pose3DEncodersData->pan = pose->leftknee.encoders.pan;
            pose3DEncodersData->tilt = pose->leftknee.encoders.tilt;
            pose3DEncodersData->roll = pose->leftknee.encoders.roll;
            pose3DEncodersData->clock = pose->leftknee.encoders.clock;
            pose3DEncodersData->maxPan = pose->leftknee.encoders.maxPan;
            pose3DEncodersData->minPan = pose->leftknee.encoders.minPan;
            pose3DEncodersData->maxTilt = pose->leftknee.encoders.maxTilt;
            pose3DEncodersData->minTilt = pose->leftknee.encoders.minTilt;
            
            pthread_mutex_unlock(&pose->mutex_leftkneeencoders);

            return pose3DEncodersData;
        }

        gazebo::PoseLeftKnee* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;
    };

    class Pose3DMotorsLK : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotorsLK (gazebo::PoseLeftKnee* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotorsLK () {}

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftkneemotors);
            
            pose3DMotorsData->x = pose->leftknee.motorsdata.x;
            pose3DMotorsData->y = pose->leftknee.motorsdata.y;
            pose3DMotorsData->z = pose->leftknee.motorsdata.z;
            pose3DMotorsData->pan = pose->leftknee.motorsdata.pan;
            pose3DMotorsData->tilt = pose->leftknee.motorsdata.tilt;
            pose3DMotorsData->roll = pose->leftknee.motorsdata.roll;
            pose3DMotorsData->panSpeed = pose->leftknee.motorsdata.panSpeed;
            pose3DMotorsData->tiltSpeed = pose->leftknee.motorsdata.tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftkneemotors);

            return pose3DMotorsData;
        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftkneemotors);
            
            pose3DMotorsParams->maxPan = pose->leftknee.motorsparams.maxPan;
            pose3DMotorsParams->minPan = pose->leftknee.motorsparams.minPan;
            pose3DMotorsParams->maxTilt = pose->leftknee.motorsparams.maxTilt;
            pose3DMotorsParams->minTilt = pose->leftknee.motorsparams.minTilt;
            pose3DMotorsParams->maxPanSpeed = pose->leftknee.motorsparams.maxPanSpeed;
            pose3DMotorsParams->maxTiltSpeed = pose->leftknee.motorsparams.maxTiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftkneemotors);
            
            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftkneemotors);
            
            pose->leftknee.motorsdata.x = data->x;
            pose->leftknee.motorsdata.y = data->y;
            pose->leftknee.motorsdata.z = data->z;
            pose->leftknee.motorsdata.pan = data->pan;
            pose->leftknee.motorsdata.tilt = data->tilt;
            pose->leftknee.motorsdata.roll = data->roll;
            pose->leftknee.motorsdata.panSpeed = data->panSpeed;
            pose->leftknee.motorsdata.tiltSpeed = data->tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftkneemotors);
        }

        gazebo::PoseLeftKnee* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;
    };

    void* thread_LeftKneeICE ( void* v ) {

        gazebo::PoseLeftKnee* leftknee = (gazebo::PoseLeftKnee*)v;
        char* name = (char*) leftknee->cfgfile_leftknee.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string EndpointsEncoders = prop->getProperty("PoseLeftKneeEncoders.Endpoints");
            std::cout << "PoseLeftKneeEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("PoseLeftKneeMotors.Endpoints");
            std::cout << "PoseLeftKneeMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterLeftKneeEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterLeftKneeMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncodersLK(leftknee);
            Ice::ObjectPtr motors = new Pose3DMotorsLK(leftknee);

            AdapterEncoders->add(encoders, ic->stringToIdentity("LeftKneeEncoders"));
            AdapterMotors->add(motors, ic->stringToIdentity("LeftKneeMotors"));

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
