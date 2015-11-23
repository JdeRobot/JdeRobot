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

#include "poselefthip.h"

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(PoseLeftHip)

    PoseLeftHip::PoseLeftHip () {
        pthread_mutex_init(&this->mutex_lefthipencoders, NULL);
        pthread_mutex_init(&this->mutex_lefthipmotors, NULL);
        this->cycle = 50;
        this->cfgfile_lefthip = std::string("--Ice.Config=poselefthip.cfg");
        this->modelYaw = std::string("joint_poselefthip_yaw");
        this->modelPitch = std::string("joint_poselefthip_pitch");
        this->modelRoll = std::string("joint_poselefthip_roll");

        std::cout << "Constructor PoseLeftHip" << std::endl;
    }

    void PoseLeftHip::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        if (!_sdf->HasElement(this->modelYaw))
            gzerr << "PoseRightElbow plugin missing <" << this->modelYaw << "> element\n";
        if (!_sdf->HasElement(this->modelPitch))
            gzerr << "PoseRightElbow plugin missing <" << this->modelPitch << "> element\n";
        if (!_sdf->HasElement(this->modelRoll))
            gzerr << "PoseRightElbow plugin missing <" << this->modelRoll << "> element\n";
            
        std::string elemYaw = std::string(_sdf->GetElement(this->modelYaw)->GetValueString());
        std::string elemPitch = std::string(_sdf->GetElement(this->modelPitch)->GetValueString());
        std::string elemRoll = std::string(_sdf->GetElement(this->modelRoll)->GetValueString());
            
        if (!_sdf->HasElement(elemYaw))
            gzerr << "PoseRightElbow plugin missing <" << elemYaw << "> element\n";
        if (!_sdf->HasElement(elemPitch))
            gzerr << "PoseRightElbow plugin missing <" << elemPitch << "> element\n";
        if (!_sdf->HasElement(elemRoll))
            gzerr << "PoseRightElbow plugin missing <" << elemRoll << "> element\n";
            
        this->lefthip.joint_yaw = _model->GetJoint(elemYaw);
        this->lefthip.joint_pitch = _model->GetJoint(elemPitch);
        this->lefthip.joint_roll = _model->GetJoint(elemRoll);

        this->maxYaw = (float) this->lefthip.joint_yaw->GetUpperLimit(0).Radian();
        this->minYaw = (float) this->lefthip.joint_yaw->GetLowerLimit(0).Radian();
        this->maxPitch = (float) this->lefthip.joint_pitch->GetUpperLimit(0).Radian();
        this->minPitch = (float) this->lefthip.joint_pitch->GetLowerLimit(0).Radian();
        this->maxRoll = (float) this->lefthip.joint_roll->GetUpperLimit(0).Radian();
        this->minRoll = (float) this->lefthip.joint_roll->GetLowerLimit(0).Radian();

        // Load torque       
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the right hip plugin.\n";
            this->stiffness = 5.0;
        }
        
        pthread_t thr_ice;
        pthread_create(&thr_ice, NULL, &thread_LeftHipICE, (void*) this);

        // Load OnUpdate method
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&PoseLeftHip::OnUpdate, this));
    }

    void PoseLeftHip::Init () {
        this->lefthip.encoders.pan = 0.0;
        this->lefthip.encoders.tilt = 0.0;
        this->lefthip.encoders.roll = 0.0;
        
        this->lefthip.motorsdata.pan = 0.0;
        this->lefthip.motorsdata.tilt = 0.0;
        this->lefthip.motorsdata.roll = 0.0;
    }

    void PoseLeftHip::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;

        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;
        
        //          ----------ENCODERS----------
        // GET pose3dencoders data from the right elbow (PAN&ROLL)
        pthread_mutex_lock(&this->mutex_lefthipencoders);
        
        this->lefthip.encoders.pan = this->lefthip.joint_yaw->GetAngle(0).Radian();
        this->lefthip.encoders.tilt = this->lefthip.joint_pitch->GetAngle(0).Radian();
        this->lefthip.encoders.roll = this->lefthip.joint_roll->GetAngle(0).Radian();
        
        pthread_mutex_unlock(&this->mutex_lefthipencoders);

        //          ----------MOTORS----------
        this->lefthip.joint_yaw->SetMaxForce(0, this->stiffness);
        this->lefthip.joint_pitch->SetMaxForce(0, this->stiffness);
        this->lefthip.joint_roll->SetMaxForce(0, this->stiffness);
        
        pthread_mutex_lock(&this->mutex_lefthipmotors);
        
        float yawSpeed = - this->lefthip.motorsdata.pan - this->lefthip.encoders.pan;
        if ((std::abs(yawSpeed) < 0.1) && (std::abs(yawSpeed) > 0.001))
            yawSpeed = 0.1;
        
        float pitchSpeed = - this->lefthip.motorsdata.tilt - this->lefthip.encoders.tilt;
        if ((std::abs(pitchSpeed) < 0.1) && (std::abs(pitchSpeed) > 0.001))
            pitchSpeed = 0.1;
            
        float rollSpeed = - this->lefthip.motorsdata.roll - this->lefthip.encoders.roll;
        if ((std::abs(rollSpeed) < 0.1) && (std::abs(rollSpeed) > 0.001))
            rollSpeed = 0.1;
        
        this->lefthip.joint_yaw->SetVelocity(0, yawSpeed);
        this->lefthip.joint_pitch->SetVelocity(0, pitchSpeed);
        this->lefthip.joint_roll->SetVelocity(0, rollSpeed);

        pthread_mutex_unlock(&this->mutex_lefthipmotors);

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;

        diff = (totalb - totala) / 1000;
        diff = cycle - diff;

        if (diff < 10)
            diff = 10;

        //usleep(diff*1000);
        sleep(diff / 1000);
    }
    
    class Pose3DEncodersRH : virtual public jderobot::Pose3DEncoders {
    public:

        Pose3DEncodersRH ( gazebo::PoseLeftHip* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncodersRH () {}

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_lefthipencoders);
            
            pose3DEncodersData->x = pose->lefthip.encoders.x;
            pose3DEncodersData->y = pose->lefthip.encoders.y;
            pose3DEncodersData->z = pose->lefthip.encoders.z;
            pose3DEncodersData->pan = pose->lefthip.encoders.pan;
            pose3DEncodersData->tilt = pose->lefthip.encoders.tilt;
            pose3DEncodersData->roll = pose->lefthip.encoders.roll;
            pose3DEncodersData->clock = pose->lefthip.encoders.clock;
            pose3DEncodersData->maxPan = pose->lefthip.encoders.maxPan;
            pose3DEncodersData->minPan = pose->lefthip.encoders.minPan;
            pose3DEncodersData->maxTilt = pose->lefthip.encoders.maxTilt;
            pose3DEncodersData->minTilt = pose->lefthip.encoders.minTilt;
            
            pthread_mutex_unlock(&pose->mutex_lefthipencoders);

            return pose3DEncodersData;
        }

        gazebo::PoseLeftHip* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;
    };

    class Pose3DMotorsRH : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotorsRH (gazebo::PoseLeftHip* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotorsRH () {}

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_lefthipmotors);
            
            pose3DMotorsData->x = pose->lefthip.motorsdata.x;
            pose3DMotorsData->y = pose->lefthip.motorsdata.y;
            pose3DMotorsData->z = pose->lefthip.motorsdata.z;
            pose3DMotorsData->pan = pose->lefthip.motorsdata.pan;
            pose3DMotorsData->tilt = pose->lefthip.motorsdata.tilt;
            pose3DMotorsData->roll = pose->lefthip.motorsdata.roll;
            pose3DMotorsData->panSpeed = pose->lefthip.motorsdata.panSpeed;
            pose3DMotorsData->tiltSpeed = pose->lefthip.motorsdata.tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_lefthipmotors);

            return pose3DMotorsData;
        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_lefthipmotors);
            
            pose3DMotorsParams->maxPan = pose->lefthip.motorsparams.maxPan;
            pose3DMotorsParams->minPan = pose->lefthip.motorsparams.minPan;
            pose3DMotorsParams->maxTilt = pose->lefthip.motorsparams.maxTilt;
            pose3DMotorsParams->minTilt = pose->lefthip.motorsparams.minTilt;
            pose3DMotorsParams->maxPanSpeed = pose->lefthip.motorsparams.maxPanSpeed;
            pose3DMotorsParams->maxTiltSpeed = pose->lefthip.motorsparams.maxTiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_lefthipmotors);
            
            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_lefthipmotors);
            
            pose->lefthip.motorsdata.x = data->x;
            pose->lefthip.motorsdata.y = data->y;
            pose->lefthip.motorsdata.z = data->z;
            pose->lefthip.motorsdata.pan = data->pan;
            pose->lefthip.motorsdata.tilt = data->tilt;
            pose->lefthip.motorsdata.roll = data->roll;
            pose->lefthip.motorsdata.panSpeed = data->panSpeed;
            pose->lefthip.motorsdata.tiltSpeed = data->tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_lefthipmotors);
        }

        gazebo::PoseLeftHip* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;
    };

    void* thread_LeftHipICE ( void* v ) {

        gazebo::PoseLeftHip* lefthip = (gazebo::PoseLeftHip*)v;
        char* name = (char*) lefthip->cfgfile_lefthip.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string EndpointsEncoders = prop->getProperty("PoseLeftHipEncoders.Endpoints");
            std::cout << "PoseLeftHipEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("PoseLeftHipkMotors.Endpoints");
            std::cout << "PoseLeftHipMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterLeftHipEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterLeftHipkMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncodersRH(lefthip);
            Ice::ObjectPtr motors = new Pose3DMotorsRH(lefthip);

            AdapterEncoders->add(encoders, ic->stringToIdentity("LeftHipEncoders"));
            AdapterMotors->add(motors, ic->stringToIdentity("LeftHipMotors"));

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
