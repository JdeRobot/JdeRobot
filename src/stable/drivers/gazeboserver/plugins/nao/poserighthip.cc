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

#include "poserighthip.h"

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(PoseRightHip)

    PoseRightHip::PoseRightHip () {
        pthread_mutex_init(&this->mutex_righthipencoders, NULL);
        pthread_mutex_init(&this->mutex_righthipmotors, NULL);
        this->cycle = 50;
        this->cfgfile_righthip = std::string("--Ice.Config=poserighthip.cfg");
        this->modelYaw = std::string("joint_poserighthip_yaw");
        this->modelPitch = std::string("joint_poserighthip_pitch");
        this->modelRoll = std::string("joint_poserighthip_roll");

        std::cout << "Constructor PoseRightHip" << std::endl;
    }

    void PoseRightHip::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
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
            
        this->righthip.joint_yaw = _model->GetJoint(elemYaw);
        this->righthip.joint_pitch = _model->GetJoint(elemPitch);
        this->righthip.joint_roll = _model->GetJoint(elemRoll);

        this->maxYaw = (float) this->righthip.joint_yaw->GetUpperLimit(0).Radian();
        this->minYaw = (float) this->righthip.joint_yaw->GetLowerLimit(0).Radian();
        this->maxPitch = (float) this->righthip.joint_pitch->GetUpperLimit(0).Radian();
        this->minPitch = (float) this->righthip.joint_pitch->GetLowerLimit(0).Radian();
        this->maxRoll = (float) this->righthip.joint_roll->GetUpperLimit(0).Radian();
        this->minRoll = (float) this->righthip.joint_roll->GetLowerLimit(0).Radian();

        // Load torque       
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the right hip plugin.\n";
            this->stiffness = 5.0;
        }
        
        pthread_t thr_ice;
        pthread_create(&thr_ice, NULL, &thread_RightHipICE, (void*) this);

        // Load OnUpdate method
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&PoseRightHip::OnUpdate, this));
    }

    void PoseRightHip::Init () {
        this->righthip.encoders.pan = 0.0;
        this->righthip.encoders.tilt = 0.0;
        this->righthip.encoders.roll = 0.0;
        
        this->righthip.motorsdata.pan = 0.0;
        this->righthip.motorsdata.tilt = 0.0;
        this->righthip.motorsdata.roll = 0.0;
    }

    void PoseRightHip::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;

        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;
        
        //          ----------ENCODERS----------
        // GET pose3dencoders data from the right elbow (PAN&ROLL)
        pthread_mutex_lock(&this->mutex_righthipencoders);
        
        this->righthip.encoders.pan = this->righthip.joint_yaw->GetAngle(0).Radian();
        this->righthip.encoders.tilt = this->righthip.joint_pitch->GetAngle(0).Radian();
        this->righthip.encoders.roll = this->righthip.joint_roll->GetAngle(0).Radian();
        
        pthread_mutex_unlock(&this->mutex_righthipencoders);

        //          ----------MOTORS----------
        this->righthip.joint_yaw->SetMaxForce(0, this->stiffness);
        this->righthip.joint_pitch->SetMaxForce(0, this->stiffness);
        this->righthip.joint_roll->SetMaxForce(0, this->stiffness);
        
        pthread_mutex_lock(&this->mutex_righthipmotors);
        
        float yawSpeed = - this->righthip.motorsdata.pan - this->righthip.encoders.pan;
        if ((std::abs(yawSpeed) < 0.1) && (std::abs(yawSpeed) > 0.001))
            yawSpeed = 0.1;
        
        float pitchSpeed = - this->righthip.motorsdata.tilt - this->righthip.encoders.tilt;
        if ((std::abs(pitchSpeed) < 0.1) && (std::abs(pitchSpeed) > 0.001))
            pitchSpeed = 0.1;
            
        float rollSpeed = - this->righthip.motorsdata.roll - this->righthip.encoders.roll;
        if ((std::abs(rollSpeed) < 0.1) && (std::abs(rollSpeed) > 0.001))
            rollSpeed = 0.1;
        
        this->righthip.joint_yaw->SetVelocity(0, yawSpeed);
        this->righthip.joint_pitch->SetVelocity(0, pitchSpeed);
        this->righthip.joint_roll->SetVelocity(0, rollSpeed);

        pthread_mutex_unlock(&this->mutex_righthipmotors);

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

        Pose3DEncodersRH ( gazebo::PoseRightHip* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncodersRH () {}

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_righthipencoders);
            
            pose3DEncodersData->x = pose->righthip.encoders.x;
            pose3DEncodersData->y = pose->righthip.encoders.y;
            pose3DEncodersData->z = pose->righthip.encoders.z;
            pose3DEncodersData->pan = pose->righthip.encoders.pan;
            pose3DEncodersData->tilt = pose->righthip.encoders.tilt;
            pose3DEncodersData->roll = pose->righthip.encoders.roll;
            pose3DEncodersData->clock = pose->righthip.encoders.clock;
            pose3DEncodersData->maxPan = pose->righthip.encoders.maxPan;
            pose3DEncodersData->minPan = pose->righthip.encoders.minPan;
            pose3DEncodersData->maxTilt = pose->righthip.encoders.maxTilt;
            pose3DEncodersData->minTilt = pose->righthip.encoders.minTilt;
            
            pthread_mutex_unlock(&pose->mutex_righthipencoders);

            return pose3DEncodersData;
        }

        gazebo::PoseRightHip* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;
    };

    class Pose3DMotorsRH : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotorsRH (gazebo::PoseRightHip* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotorsRH () {}

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_righthipmotors);
            
            pose3DMotorsData->x = pose->righthip.motorsdata.x;
            pose3DMotorsData->y = pose->righthip.motorsdata.y;
            pose3DMotorsData->z = pose->righthip.motorsdata.z;
            pose3DMotorsData->pan = pose->righthip.motorsdata.pan;
            pose3DMotorsData->tilt = pose->righthip.motorsdata.tilt;
            pose3DMotorsData->roll = pose->righthip.motorsdata.roll;
            pose3DMotorsData->panSpeed = pose->righthip.motorsdata.panSpeed;
            pose3DMotorsData->tiltSpeed = pose->righthip.motorsdata.tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_righthipmotors);

            return pose3DMotorsData;
        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_righthipmotors);
            
            pose3DMotorsParams->maxPan = pose->righthip.motorsparams.maxPan;
            pose3DMotorsParams->minPan = pose->righthip.motorsparams.minPan;
            pose3DMotorsParams->maxTilt = pose->righthip.motorsparams.maxTilt;
            pose3DMotorsParams->minTilt = pose->righthip.motorsparams.minTilt;
            pose3DMotorsParams->maxPanSpeed = pose->righthip.motorsparams.maxPanSpeed;
            pose3DMotorsParams->maxTiltSpeed = pose->righthip.motorsparams.maxTiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_righthipmotors);
            
            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_righthipmotors);
            
            pose->righthip.motorsdata.x = data->x;
            pose->righthip.motorsdata.y = data->y;
            pose->righthip.motorsdata.z = data->z;
            pose->righthip.motorsdata.pan = data->pan;
            pose->righthip.motorsdata.tilt = data->tilt;
            pose->righthip.motorsdata.roll = data->roll;
            pose->righthip.motorsdata.panSpeed = data->panSpeed;
            pose->righthip.motorsdata.tiltSpeed = data->tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_righthipmotors);
        }

        gazebo::PoseRightHip* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;
    };

    void* thread_RightHipICE ( void* v ) {

        gazebo::PoseRightHip* righthip = (gazebo::PoseRightHip*)v;
        char* name = (char*) righthip->cfgfile_righthip.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string EndpointsEncoders = prop->getProperty("PoseRightHipEncoders.Endpoints");
            std::cout << "PoseRightHipEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("PoseRightHipkMotors.Endpoints");
            std::cout << "PoseRightHipMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterRightHipEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterRightHipkMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncodersRH(righthip);
            Ice::ObjectPtr motors = new Pose3DMotorsRH(righthip);

            AdapterEncoders->add(encoders, ic->stringToIdentity("RightHipEncoders"));
            AdapterMotors->add(motors, ic->stringToIdentity("RightHipMotors"));

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
