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

#include "poseleftankle.h"

#define RADTODEG 57.29582790

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(Pose3DEncodersLeftAnkle)

    PoseLeftAnkle::PoseLeftAnkle () {
        pthread_mutex_init(&mutex_lefankleencoders, NULL);
        pthread_mutex_init(&mutex_lefanklemotors, NULL);
        this->count = 0;
        this->cycle = 50;
        this->cfgfile_leftankle = std::string("--Ice.Config=poseleftankle.cfg");
        
        this->leftankle.motorsparams.maxPan = 1.57;
        this->leftankle.motorsparams.minPan = -1.57;          
        this->leftankle.motorsparams.maxTilt = 0.5;
        this->leftankle.motorsparams.minTilt = -0.5;

        std::cout << "Constructor PoseLeftAnkle" << std::endl;
    }

    void PoseLeftAnkle::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        // LOAD CAMERA LEFT
        if (!_sdf->HasElement("joint_pose3dencodersleftankle_pitch"))
            gzerr << "pose3dencodersleftankle plugin missing <joint_pose3dencodersleftankle_pitch> element\n";
        if (!_sdf->HasElement("joint_pose3dencodersleftankle_roll"))
            gzerr << "pose3dencodersleftankle plugin missing <joint_pose3dencodersleftankle_roll> element\n";

        this->leftankle.joint_roll = this->model->GetJoint("lankle_roll");
        this->leftankle.joint_tilt = this->model->GetJoint("lankle_pitch");

        if (!this->leftankle.joint_roll)
            gzerr << "Unable to find joint_pose3dencodersleftankle_roll["
                << _sdf->GetElement("joint_pose3dencodersleftankle_roll")->GetValueString() << "]\n";
        if (!this->leftankle.joint_tilt)
            gzerr << "Unable to find joint_pose3dencodersleftankle_pitch["
                << _sdf->GetElement("joint_pose3dencodersleftankle_pitch")->GetValueString() << "]\n"; 
                
        this->leftankle.link_roll = this->model->GetLink("left_foot");
        this->leftankle.link_tilt = this->model->GetLink("leftankle_pitch");

        //LOAD TORQUE        
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the DiffDrive plugin.\n";
            this->stiffness = 5.0;
        }

        //LOAD POSE3DMOTORS
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&PoseLeftAnkle::OnUpdate, this));
    }

    void PoseLeftAnkle::Init () {}

    void PoseLeftAnkle::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;

        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        if (count == 0) {
            count++;
            pthread_t thr_ice;
            pthread_create(&thr_ice, NULL, &thread_LeftAnkleICE, (void*) this);
            
            this->leftankle.encoders.roll = 0;    
            this->leftankle.encoders.tilt = 0;
        } else {
            //          ----------ENCODERS----------
            //GET pose3dencoders data from the left ankle (PAN&TILT)
            this->leftankle.encoders.roll = this->leftankle.link_roll->GetRelativePose().rot.GetAsEuler().y;    
            this->leftankle.encoders.tilt = this->leftankle.link_tilt->GetRelativePose().rot.GetAsEuler().x;
        }

        //          ----------MOTORS----------
        if (this->leftankle.motorsdata.roll >= 0) {
            if (this->leftankle.encoders.roll < this->leftankle.motorsdata.roll) {
                this->leftankle.joint_roll->SetVelocity(0, -0.1);
                this->leftankle.joint_roll->SetMaxForce(0, this->stiffness);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->leftankle.joint_roll->SetVelocity(0, 0.1);
                this->leftankle.joint_roll->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->leftankle.encoders.roll > this->leftankle.motorsdata.roll) {
                this->leftankle.joint_roll->SetVelocity(0, 0.1);
                this->leftankle.joint_roll->SetMaxForce(0, this->stiffness);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->leftankle.joint_roll->SetVelocity(0, -0.1);
                this->leftankle.joint_roll->SetMaxForce(0, this->stiffness);
            }            
        }
        
        if (this->leftankle.motorsdata.tilt >= 0) {
            if (this->leftankle.encoders.tilt < this->leftankle.motorsdata.tilt) {
                this->leftankle.joint_tilt->SetVelocity(0, -0.1);
                this->leftankle.joint_tilt->SetMaxForce(0, this->stiffness);
            } else {
                this->leftankle.joint_tilt->SetVelocity(0, 0.1);
                this->leftankle.joint_tilt->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->leftankle.encoders.tilt > this->leftankle.motorsdata.tilt) {
                this->leftankle.joint_tilt->SetVelocity(0, 0.1);
                this->leftankle.joint_tilt->SetMaxForce(0, this->stiffness);
            } else {
                this->leftankle.joint_tilt->SetVelocity(0, -0.1);
                this->leftankle.joint_tilt->SetMaxForce(0, this->stiffness);
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

        Pose3DEncoders ( gazebo::PoseLeftAnkle* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncoders () {}

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftankleencoders);
            
            pose3DEncodersData->x = pose->leftankle.encoders.x;
            pose3DEncodersData->y = pose->leftankle.encoders.y;
            pose3DEncodersData->z = pose->leftankle.encoders.z;
            pose3DEncodersData->pan = pose->leftankle.encoders.pan;
            pose3DEncodersData->tilt = pose->leftankle.encoders.tilt;
            pose3DEncodersData->roll = pose->leftankle.encoders.roll;
            pose3DEncodersData->clock = pose->leftankle.encoders.clock;
            pose3DEncodersData->maxPan = pose->leftankle.encoders.maxPan;
            pose3DEncodersData->minPan = pose->leftankle.encoders.minPan;
            pose3DEncodersData->maxTilt = pose->leftankle.encoders.maxTilt;
            pose3DEncodersData->minTilt = pose->leftankle.encoders.minTilt;
            
            pthread_mutex_unlock(&pose->mutex_leftankleencoders);

            return pose3DEncodersData;
        }

        gazebo::PoseLeftAnkle* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;
    };

    class Pose3DMotors : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotors (gazebo::PoseLeftAnkle* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotors() {}

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftanklemotors);
            
            pose3DMotorsData->x = pose->leftankle.motorsdata.x;
            pose3DMotorsData->y = pose->leftankle.motorsdata.y;
            pose3DMotorsData->z = pose->leftankle.motorsdata.z;
            pose3DMotorsData->pan = pose->leftankle.motorsdata.pan;
            pose3DMotorsData->tilt = pose->leftankle.motorsdata.tilt;
            pose3DMotorsData->roll = pose->leftankle.motorsdata.roll;
            pose3DMotorsData->panSpeed = pose->leftankle.motorsdata.panSpeed;
            pose3DMotorsData->tiltSpeed = pose->leftankle.motorsdata.tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftanklemotors);

            return pose3DMotorsData;
        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftanklemotors);
            
            pose3DMotorsParams->maxPan = pose->leftankle.motorsparams.maxPan;
            pose3DMotorsParams->minPan = pose->leftankle.motorsparams.minPan;
            pose3DMotorsParams->maxTilt = pose->leftankle.motorsparams.maxTilt;
            pose3DMotorsParams->minTilt = pose->leftankle.motorsparams.minTilt;
            pose3DMotorsParams->maxPanSpeed = pose->leftankle.motorsparams.maxPanSpeed;
            pose3DMotorsParams->maxTiltSpeed = pose->leftankle.motorsparams.maxTiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftanklemotors);
            
            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftanklemotors);
            
            pose->leftankle.motorsdata.x = data->x;
            pose->leftankle.motorsdata.y = data->y;
            pose->leftankle.motorsdata.z = data->z;
            pose->leftankle.motorsdata.pan = data->pan;
            pose->leftankle.motorsdata.tilt = data->tilt;
            pose->leftankle.motorsdata.roll = data->roll;
            pose->leftankle.motorsdata.panSpeed = data->panSpeed;
            pose->leftankle.motorsdata.tiltSpeed = data->tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftanklemotors);
        }

        gazebo::PoseLeftAnkle* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;
    };

    void* thread_LeftAnkleICE ( void* v ) {

        gazebo::PoseLeftAnkle* leftankle = (gazebo::PoseLeftAnkle*)v;
        char* name = (char*) leftankle->cfgfile_leftankle.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string EndpointsEncoders = prop->getProperty("PoseLeftAnkleEncoders.Endpoints");
            std::cout << "PoseLeftAnkleEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("PoseLeftAnkleMotors.Endpoints");
            std::cout << "PoseLeftAnkleMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterLeftAnkleEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterLeftAnkleMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncoders(leftankle);
            Ice::ObjectPtr motors = new Pose3DMotors(leftankle);

            AdapterEncoders->add(encoders, ic->stringToIdentity("LeftAnkleEncoders"));
            AdapterMotors->add(motors, ic->stringToIdentity("LeftAnkleMotors"));

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
