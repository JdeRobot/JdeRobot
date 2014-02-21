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

#include "poserightankle.h"

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(PoseRightAnkle)

    PoseRightAnkle::PoseRightAnkle () {
        pthread_mutex_init(&this->mutex_rightankleencoders, NULL);
        pthread_mutex_init(&this->mutex_rightanklemotors, NULL);
        this->cycle = 50;
        this->cfgfile_rightankle = std::string("--Ice.Config=poserightankle.cfg");
        this->modelPitch = std::string("joint_poserightankle_pitch");
        this->modelRoll = std::string("joint_poserightankle_roll");

        std::cout << "Constructor PoseRightAnkle" << std::endl;
    }

    void PoseRightAnkle::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        if (!_sdf->HasElement(this->modelPitch))
            gzerr << "PoseRightAnkle plugin missing <" << this->modelPitch << "> element\n";
        if (!_sdf->HasElement(this->modelRoll))
            gzerr << "PoseRightAnkle plugin missing <" << this->modelRoll << "> element\n";
            
        std::string elemPitch = std::string(_sdf->GetElement(this->modelPitch)->GetValueString());
        std::string elemRoll = std::string(_sdf->GetElement(this->modelRoll)->GetValueString());
        
        if (!_sdf->HasElement(elemPitch))
            gzerr << "PoseRightAnkle plugin missing <" << elemPitch << "> element\n";
        if (!_sdf->HasElement(elemRoll))
            gzerr << "PoseRightAnkle plugin missing <" << elemRoll << "> element\n";
            
        this->rightankle.joint_pitch = _model->GetJoint(elemPitch);
        this->rightankle.joint_roll = _model->GetJoint(elemRoll);

        this->maxPitch = (float) this->rightankle.joint_pitch->GetUpperLimit(0).Radian();
        this->minPitch = (float) this->rightankle.joint_pitch->GetLowerLimit(0).Radian();
        this->maxRoll = (float) this->rightankle.joint_roll->GetUpperLimit(0).Radian();
        this->minRoll = (float) this->rightankle.joint_roll->GetLowerLimit(0).Radian();

        // Load torque
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the left ankle plugin.\n";
            this->stiffness = 5.0;
        }
        
        pthread_t thr_ice;
        pthread_create(&thr_ice, NULL, &thread_RightAnkleICE, (void*) this);

        // Load OnUpdate method
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&PoseRightAnkle::OnUpdate, this));
    }

    void PoseRightAnkle::Init () {
        this->rightankle.encoders.tilt = 0.0;
        this->rightankle.encoders.roll = 0.0;
        
        this->rightankle.motorsdata.tilt = 0.0;
        this->rightankle.motorsdata.roll = 0.0;
    }

    void PoseRightAnkle::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;
        
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        //          ----------ENCODERS----------
        // GET pose3dencoders data from the left ankle (TILT&ROLL)
        pthread_mutex_lock(&this->mutex_rightankleencoders);
        
        this->rightankle.encoders.tilt = this->rightankle.joint_pitch->GetAngle(0).Radian();
        this->rightankle.encoders.roll = this->rightankle.joint_roll->GetAngle(0).Radian();   
        
        pthread_mutex_unlock(&this->mutex_rightankleencoders);

        //          ----------MOTORS----------
        this->rightankle.joint_pitch->SetMaxForce(0, this->stiffness);
        this->rightankle.joint_roll->SetMaxForce(0, this->stiffness);
        
        pthread_mutex_lock(&this->mutex_rightanklemotors);
        
        float pitchSpeed = - this->rightankle.motorsdata.tilt - this->rightankle.encoders.tilt;
        if ((std::abs(pitchSpeed) < 0.1) && (std::abs(pitchSpeed) > 0.001))
            pitchSpeed = 0.1;
        
        float rollSpeed = - this->rightankle.motorsdata.roll - this->rightankle.encoders.roll;
        if ((std::abs(rollSpeed) < 0.1) && (std::abs(rollSpeed) > 0.001))
            rollSpeed = 0.1;
        
        this->rightankle.joint_pitch->SetVelocity(0, pitchSpeed);
        this->rightankle.joint_roll->SetVelocity(0, rollSpeed);

        pthread_mutex_unlock(&this->mutex_rightanklemotors);

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;

        diff = (totalb - totala) / 1000;
        diff = cycle - diff;

        if (diff < 10)
            diff = 10;

        //usleep(diff*1000);
        sleep(diff / 1000);
    }
    
    class Pose3DEncodersRA : virtual public jderobot::Pose3DEncoders {
    public:

        Pose3DEncodersRA ( gazebo::PoseRightAnkle* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncodersRA () {}

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_rightankleencoders);
            
            pose3DEncodersData->x = pose->rightankle.encoders.x;
            pose3DEncodersData->y = pose->rightankle.encoders.y;
            pose3DEncodersData->z = pose->rightankle.encoders.z;
            pose3DEncodersData->pan = pose->rightankle.encoders.pan;
            pose3DEncodersData->tilt = pose->rightankle.encoders.tilt;
            pose3DEncodersData->roll = pose->rightankle.encoders.roll;
            pose3DEncodersData->clock = pose->rightankle.encoders.clock;
            pose3DEncodersData->maxPan = pose->rightankle.encoders.maxPan;
            pose3DEncodersData->minPan = pose->rightankle.encoders.minPan;
            pose3DEncodersData->maxTilt = pose->rightankle.encoders.maxTilt;
            pose3DEncodersData->minTilt = pose->rightankle.encoders.minTilt;
            
            pthread_mutex_unlock(&pose->mutex_rightankleencoders);

            return pose3DEncodersData;
        }

        gazebo::PoseRightAnkle* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;
    };

    class Pose3DMotorsRA : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotorsRA (gazebo::PoseRightAnkle* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotorsRA () {}

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_rightanklemotors);
            
            pose3DMotorsData->x = pose->rightankle.motorsdata.x;
            pose3DMotorsData->y = pose->rightankle.motorsdata.y;
            pose3DMotorsData->z = pose->rightankle.motorsdata.z;
            pose3DMotorsData->pan = pose->rightankle.motorsdata.pan;
            pose3DMotorsData->tilt = pose->rightankle.motorsdata.tilt;
            pose3DMotorsData->roll = pose->rightankle.motorsdata.roll;
            pose3DMotorsData->panSpeed = pose->rightankle.motorsdata.panSpeed;
            pose3DMotorsData->tiltSpeed = pose->rightankle.motorsdata.tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_rightanklemotors);

            return pose3DMotorsData;
        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_rightanklemotors);
            
            pose3DMotorsParams->maxPan = pose->rightankle.motorsparams.maxPan;
            pose3DMotorsParams->minPan = pose->rightankle.motorsparams.minPan;
            pose3DMotorsParams->maxTilt = pose->rightankle.motorsparams.maxTilt;
            pose3DMotorsParams->minTilt = pose->rightankle.motorsparams.minTilt;
            pose3DMotorsParams->maxPanSpeed = pose->rightankle.motorsparams.maxPanSpeed;
            pose3DMotorsParams->maxTiltSpeed = pose->rightankle.motorsparams.maxTiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_rightanklemotors);
            
            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_rightanklemotors);
            
            pose->rightankle.motorsdata.x = data->x;
            pose->rightankle.motorsdata.y = data->y;
            pose->rightankle.motorsdata.z = data->z;
            pose->rightankle.motorsdata.pan = data->pan;
            pose->rightankle.motorsdata.tilt = data->tilt;
            pose->rightankle.motorsdata.roll = data->roll;
            pose->rightankle.motorsdata.panSpeed = data->panSpeed;
            pose->rightankle.motorsdata.tiltSpeed = data->tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_rightanklemotors);
        }

        gazebo::PoseRightAnkle* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;
    };

    void* thread_RightAnkleICE ( void* v ) {

        gazebo::PoseRightAnkle* rightankle = (gazebo::PoseRightAnkle*)v;
        char* name = (char*) rightankle->cfgfile_rightankle.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string EndpointsEncoders = prop->getProperty("PoseRightAnkleEncoders.Endpoints");
            std::cout << "PoseRightAnkleEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("PoseRightAnkleMotors.Endpoints");
            std::cout << "PoseRightAnkleMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterRightAnkleEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterRightAnkleMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncodersRA(rightankle);
            Ice::ObjectPtr motors = new Pose3DMotorsRA(rightankle);

            AdapterEncoders->add(encoders, ic->stringToIdentity("RightAnkleEncoders"));
            AdapterMotors->add(motors, ic->stringToIdentity("RightAnkleMotors"));

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
