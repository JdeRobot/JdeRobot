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

#include "poserightshoulder.h"

#define RADTODEG 57.29582790

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(PoseRightShoulder)

    PoseRightShoulder::PoseRightShoulder () {
        pthread_mutex_init(&this->mutex_rightshoulderencoders, NULL);
        pthread_mutex_init(&this->mutex_rightshouldermotors, NULL);
        this->count = 0;
        this->cycle = 50;
        this->cfgfile_rightshoulder = std::string("--Ice.Config=poserightshoulder.cfg");
        
        this->rightshoulder.motorsparams.maxPan = 1.57;
        this->rightshoulder.motorsparams.minPan = -1.57;          
        this->rightshoulder.motorsparams.maxTilt = 0.5;
        this->rightshoulder.motorsparams.minTilt = -0.5;

        std::cout << "Constructor PoseRightShoulder" << std::endl;
    }

    void PoseRightShoulder::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        // LOAD CAMERA LEFT
        if (!_sdf->HasElement("joint_pose3dencodersrightshoulder_pitch"))
            gzerr << "pose3dencodersrightshoulder plugin missing <joint_pose3dencodersrightshoulder_pitch> element\n";
        if (!_sdf->HasElement("joint_pose3dencodersrightshoulder_roll"))
            gzerr << "pose3dencodersrightshoulder plugin missing <joint_pose3dencodersrightshoulder_roll> element\n";

        this->rightshoulder.joint_roll = _model->GetJoint("rshoulder_roll");
        this->rightshoulder.joint_tilt = _model->GetJoint("rshoulder_pitch");

        if (!this->rightshoulder.joint_roll)
            gzerr << "Unable to find joint_pose3dencodersrightshoulder_roll["
                << _sdf->GetElement("joint_pose3dencodersrightshoulder_roll")->GetValueString() << "]\n";
        if (!this->rightshoulder.joint_tilt)
            gzerr << "Unable to find joint_pose3dencodersrightshoulder_pitch["
                << _sdf->GetElement("joint_pose3dencodersrightshoulder_pitch")->GetValueString() << "]\n"; 
                
        this->rightshoulder.link_roll = _model->GetLink("right_upper_arm");
        this->rightshoulder.link_tilt = _model->GetLink("rightshoulder_pitch");

        //LOAD TORQUE        
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the DiffDrive plugin.\n";
            this->stiffness = 5.0;
        }

        //LOAD POSE3DMOTORS
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
            boost::bind(&PoseRightShoulder::OnUpdate, this));

    }

    void PoseRightShoulder::Init () {}

    void PoseRightShoulder::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;
        
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        if (this->count == 0) {
            this->count++;
            pthread_t thr_ice;
            pthread_create(&thr_ice, NULL, &thread_RightShoulderICE, (void*) this);
        }

        //          ----------ENCODERS----------
        //GET pose3dencoders data from the right shoulder (PAN&TILT)
//        this->rightshoulder.encoder.roll = this->rightshoulder.rightshoulder_link_roll->GetRelativePose().rot.GetAsEuler().y;    
//        this->rightshoulder.encoder.tilt = this->rightshoulder.rightshoulder_link_tilt->GetRelativePose().rot.GetAsEuler().x;
        
        this->rightshoulder.encoders.tilt = - this->rightshoulder.joint_tilt->GetAngle(0).Radian();
        this->rightshoulder.encoders.roll = - this->rightshoulder.joint_roll->GetAngle(0).Radian();
        
        //          ----------MOTORS----------
        if (this->rightshoulder.motorsdata.roll >= 0) {
            if (this->rightshoulder.encoders.roll < this->rightshoulder.motorsdata.roll) {
                this->rightshoulder.joint_roll->SetVelocity(0, -0.1);
                this->rightshoulder.joint_roll->SetMaxForce(0, this->stiffness);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->rightshoulder.joint_roll->SetVelocity(0, 0.1);
                this->rightshoulder.joint_roll->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->rightshoulder.encoders.roll > this->rightshoulder.motorsdata.roll) {
                this->rightshoulder.joint_roll->SetVelocity(0, 0.1);
                this->rightshoulder.joint_roll->SetMaxForce(0, this->stiffness);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->rightshoulder.joint_roll->SetVelocity(0, -0.1);
                this->rightshoulder.joint_roll->SetMaxForce(0, this->stiffness);
            }            
        }
        
        if (this->rightshoulder.motorsdata.tilt >= 0) {
            if (this->rightshoulder.encoders.tilt < this->rightshoulder.motorsdata.tilt) {
                this->rightshoulder.joint_tilt->SetVelocity(0, -0.1);
                this->rightshoulder.joint_tilt->SetMaxForce(0, this->stiffness);
            } else {
                this->rightshoulder.joint_tilt->SetVelocity(0, 0.1);
                this->rightshoulder.joint_tilt->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->rightshoulder.encoders.tilt > this->rightshoulder.motorsdata.tilt) {
                this->rightshoulder.joint_tilt->SetVelocity(0, 0.1);
                this->rightshoulder.joint_tilt->SetMaxForce(0, this->stiffness);
            } else {
                this->rightshoulder.joint_tilt->SetVelocity(0, -0.1);
                this->rightshoulder.joint_tilt->SetMaxForce(0, this->stiffness);
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

        Pose3DEncoders ( gazebo::PoseRightShoulder* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncoders () {}

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

    class Pose3DMotors : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotors (gazebo::PoseRightShoulder* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotors() {}

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

            Ice::ObjectPtr encoders = new Pose3DEncoders(rightshoulder);
            Ice::ObjectPtr motors = new Pose3DMotors(rightshoulder);

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
