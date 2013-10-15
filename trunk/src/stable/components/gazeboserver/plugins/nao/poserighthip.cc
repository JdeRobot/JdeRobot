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
 *  Authors : Borja Menéndez <borjamonserrano@gmail.com>
 *
 */

#include "poserighthip.h"

#define RADTODEG 57.29582790

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(PoseRightHip)

    PoseRightHip::PoseRightHip () {
        pthread_mutex_init(&this->mutex_righthipencoders, NULL);
        pthread_mutex_init(&this->mutex_righthipmotors, NULL);
        this->countH = 0;
        this->cycle = 50;
        this->cfgfile_righthip = std::string("--Ice.Config=poserighthip.cfg");
                 
        this->righthip.motorsparams.maxPan = 1.57;
        this->righthip.motorsparams.minPan = -1.57;
        this->righthip.motorsparams.maxTilt = 0.5;
        this->righthip.motorsparams.minTilt = -0.5;

        std::cout << "Constructor PoseRightHip" << std::endl;
    }

    void PoseRightHip::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        // LOAD CAMERA LEFT
        if (!_sdf->HasElement("joint_pose3dencodersrhip_yawpitch"))
            gzerr << "pose3dencodership plugin missing <joint_pose3dencodersrhip_yawpitch> element\n";
        if (!_sdf->HasElement("joint_pose3dencodersrhip_roll"))
            gzerr << "pose3dencodership plugin missing <joint_pose3dencodersrhip_roll> element\n";
        if (!_sdf->HasElement("joint_pose3dencodersrhip_pitch"))
            gzerr << "pose3dencodership plugin missing <joint_pose3dencodersrhip_pitch> element\n";

        this->righthip.joint_pan = _model->GetJoint("rhip_yawpitch");
        this->righthip.joint_tilt = _model->GetJoint("rhip_pitch");
        this->righthip.joint_roll = _model->GetJoint("rhip_roll");

        if (!this->righthip.joint_pan)
            gzerr << "Unable to find joint_pose3dencodership_pan["
                << _sdf->GetElement("joint_pose3dencodership_pan")->GetValueString() << "]\n";
        if (!this->righthip.joint_tilt)
            gzerr << "Unable to find joint_pose3dencodership_tilt["
                << _sdf->GetElement("joint_pose3dencodership_tilt")->GetValueString() << "]\n";
        if (!this->righthip.joint_roll)
            gzerr << "Unable to find joint_pose3dencodership_roll["
                << _sdf->GetElement("joint_pose3dencodership_roll")->GetValueString() << "]\n";
                
        this->righthip.link_pan = _model->GetLink("righthip_yawpitch");
        this->righthip.link_tilt = _model->GetLink("right_thigh");
        this->righthip.link_roll = _model->GetLink("righthip_roll");

        //LOAD TORQUE        
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the right hip.\n";
            this->stiffness = 5.0;
        }

        //LOAD POSE3DMOTORS
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&PoseRightHip::OnUpdate, this));
    }

    void PoseRightHip::Init () {}

    void PoseRightHip::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;

        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        if (this->count == 0) {
            this->count++;
            pthread_t thr_ice;
            pthread_create(&thr_ice, NULL, &thread_RightHipICE, (void*) this);
        }
        
        //          ----------ENCODERS----------
        //GET pose3dencoders data from the hip (PAN&TILT)
        this->righthip.encoders.pan = this->righthip.link_pan->GetRelativePose().rot.GetAsEuler().z;
        this->righthip.encoders.tilt = this->righthip.link_tilt->GetRelativePose().rot.GetAsEuler().x;
        this->righthip.encoders.roll = this->righthip.link_roll->GetRelativePose().rot.GetAsEuler().y;
        
        if (this->righthip.motorsdata.pan >= 0) {
            if (this->righthip.encoders.pan < this->righthip.motorsdata.pan) {
                this->righthip.joint_pan->SetVelocity(0, -0.1);
                this->righthip.joint_pan->SetMaxForce(0, this->stiffness);
            } else {
                this->righthip.joint_pan->SetVelocity(0, 0.1);
                this->righthip.joint_pan->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->righthip.encoders.pan > this->righthip.motorsdata.pan) {
                this->righthip.joint_pan->SetVelocity(0, 0.1);
                this->righthip.joint_pan->SetMaxForce(0, this->stiffness);
            } else {
                this->righthip.joint_pan->SetVelocity(0, -0.1);
                this->righthip.joint_pan->SetMaxForce(0, this->stiffness);
            }
        }
        
        if (this->righthip.motorsdata.tilt >= 0) {
            if (this->righthip.encoders.tilt < this->righthip.motorsdata.tilt) {
                this->righthip.joint_tilt->SetVelocity(0, -0.1);
                this->righthip.joint_tilt->SetMaxForce(0, this->stiffness);
            } else {
                this->righthip.joint_tilt->SetVelocity(0, 0.1);
                this->righthip.joint_tilt->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->righthip.encoders.tilt > this->righthip.motorsdata.tilt) {
                this->righthip.joint_tilt->SetVelocity(0, 0.1);
                this->righthip.joint_tilt->SetMaxForce(0, this->stiffness);
            } else {
                this->righthip.joint_tilt->SetVelocity(0, -0.1);
                this->righthip.joint_tilt->SetMaxForce(0, this->stiffness);
            }
        }
        
        if (this->righthip.motorsdata.roll >= 0) {
            if (this->righthip.encoders.roll < this->righthip.motorsdata.roll) {
                this->righthip.joint_roll->SetVelocity(0, -0.1);
                this->righthip.joint_roll->SetMaxForce(0, this->stiffness);
            } else {
                this->righthip.joint_roll->SetVelocity(0, 0.1);
                this->righthip.joint_roll->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->righthip.encoders.roll > this->righthip.motorsdata.roll) {
                this->righthip.joint_roll->SetVelocity(0, 0.1);
                this->righthip.joint_roll->SetMaxForce(0, this->stiffness);
            } else {
                this->righthip.joint_roll->SetVelocity(0, -0.1);
                this->righthip.joint_roll->SetMaxForce(0, this->stiffness);
            }
        }

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

        Pose3DEncoders ( gazebo::PoseRightHip* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncoders () {}

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

    class Pose3DMotors : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotors (gazebo::PoseRightHip* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotors() {}

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

            Ice::ObjectPtr encoders = new Pose3DEncoders(righthip);
            Ice::ObjectPtr motors = new Pose3DMotors(righthip);

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
