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

#include "poseleftshoulder.h"

#define RADTODEG 57.29582790

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(PoseLeftShoulder)

    PoseLeftShoulder::PoseLeftShoulder () {
        pthread_mutex_init(&this->mutex_leftshoulderencoders, NULL);
        pthread_mutex_init(&this->mutex_leftshouldermotors, NULL);
        this->count = 0;
        this->cycle = 50;
        this->cfgfile_leftshoulder = std::string("--Ice.Config=poseleftshoulder.cfg");
        
        this->leftshoulder.motorsparams.maxPan = 1.57;
        this->leftshoulder.motorsparams.minPan = -1.57;          
        this->leftshoulder.motorsparams.maxTilt = 0.5;
        this->leftshoulder.motorsparams.minTilt = -0.5;

        std::cout << "Constructor PoseLeftShoulder" << std::endl;
    }

    void PoseLeftShoulder::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        // LOAD CAMERA LEFT
        if (!_sdf->HasElement("joint_pose3dencodersleftshoulder_pitch"))
            gzerr << "pose3dencodersleftshoulder plugin missing <joint_pose3dencodersleftshoulder_pitch> element\n";
        if (!_sdf->HasElement("joint_pose3dencodersleftshoulder_roll"))
            gzerr << "pose3dencodersleftshoulder plugin missing <joint_pose3dencodersleftshoulder_roll> element\n";

        this->leftshoulder.joint_roll = _model->GetJoint("lshoulder_roll");
        this->leftshoulder.joint_tilt = _model->GetJoint("lshoulder_pitch");

        if (!this->leftshoulder.joint_roll)
            gzerr << "Unable to find joint_pose3dencodersleftshoulder_roll["
                << _sdf->GetElement("joint_pose3dencodersleftshoulder_roll")->GetValueString() << "]\n";
        if (!this->leftshoulder.joint_tilt)
            gzerr << "Unable to find joint_pose3dencodersleftshoulder_pitch["
                << _sdf->GetElement("joint_pose3dencodersleftshoulder_pitch")->GetValueString() << "]\n"; 
                
        this->leftshoulder.link_roll = _model->GetLink("left_upper_arm");
        this->leftshoulder.link_tilt = _model->GetLink("leftshoulder_pitch");

        //LOAD TORQUE        
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for left shoulder.\n";
            this->stiffness = 5.0;
        }

        //LOAD POSE3DMOTORS
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&PoseLeftShoulder::OnUpdate, this));
    }

    void PoseLeftShoulder::Init () {}

    void PoseLeftShoulder::OnUpdate() {
        long totalb, totala, diff;
        struct timeval a, b;

        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        if (this->count == 0) {
            this->count++;
            pthread_t thr_gui;
            pthread_create(&thr_gui, NULL, &thread_LeftShoulderICE, (void*) this);
        }

        //          ----------ENCODERS----------
        //GET pose3dencoders data from the left shoulder (PAN&TILT)
//        this->leftshoulder.encoder.roll = this->leftshoulder.leftshoulder_link_roll->GetRelativePose().rot.GetAsEuler().y;    
//        this->leftshoulder.encoder.tilt = this->leftshoulder.leftshoulder_link_tilt->GetRelativePose().rot.GetAsEuler().x;
        
        this->leftshoulder.encoders.tilt = - this->leftshoulder.joint_tilt->GetAngle(0).Radian();
        this->leftshoulder.encoders.roll = - this->leftshoulder.joint_roll->GetAngle(0).Radian();

        //          ----------MOTORS----------
        if (this->leftshoulder.motorsdata.roll >= 0) {
            if (this->leftshoulder.encoders.roll < this->leftshoulder.motorsdata.roll) {
                this->leftshoulder.joint_roll->SetVelocity(0, -0.1);
                this->leftshoulder.joint_roll->SetMaxForce(0, this->stiffness);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->leftshoulder.joint_roll->SetVelocity(0, 0.1);
                this->leftshoulder.joint_roll->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->leftshoulder.encoders.roll > this->leftshoulder.motorsdata.roll) {
                this->leftshoulder.joint_roll->SetVelocity(0, 0.1);
                this->leftshoulder.joint_roll->SetMaxForce(0, this->stiffness);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->leftshoulder.joint_roll->SetVelocity(0, -0.1);
                this->leftshoulder.joint_roll->SetMaxForce(0, this->stiffness);
            }            
        }
        
        if (this->leftshoulder.motorsdata.tilt >= 0) {
            if (this->leftshoulder.encoders.tilt < this->leftshoulder.motorsdata.tilt) {
                this->leftshoulder.joint_tilt->SetVelocity(0, -0.1);
                this->leftshoulder.joint_tilt->SetMaxForce(0, this->stiffness);
            } else {
                this->leftshoulder.joint_tilt->SetVelocity(0, 0.1);
                this->leftshoulder.joint_tilt->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->leftshoulder.encoders.tilt > this->leftshoulder.motorsdata.tilt) {
                this->leftshoulder.joint_tilt->SetVelocity(0, 0.1);
                this->leftshoulder.joint_tilt->SetMaxForce(0, this->stiffness);
            } else {
                this->leftshoulder.joint_tilt->SetVelocity(0, -0.1);
                this->leftshoulder.joint_tilt->SetMaxForce(0, this->stiffness);
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

        Pose3DEncoders ( gazebo::PoseLeftShoulder* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncoders () {}

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

    class Pose3DMotors : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotors (gazebo::PoseLeftShoulder* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotors() {}

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

            Ice::ObjectPtr encoders = new Pose3DEncoders(leftshoulder);
            Ice::ObjectPtr motors = new Pose3DMotors(leftshoulder);

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
