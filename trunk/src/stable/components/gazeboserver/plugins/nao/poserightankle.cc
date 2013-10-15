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

#include "poserightankle.h"

#define RADTODEG 57.29582790

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(PoseRightAnkle)

    PoseRightAnkle::PoseRightAnkle () {
        pthread_mutex_init(&this->mutex_rightankleencoders, NULL);
        pthread_mutex_init(&this->mutex_rightanklemotors, NULL);
        this->count = 0;
        this->cycle = 50;
        this->cfgfile_rightshoulder = std::string("--Ice.Config=poserightankle.cfg");
        
        this->rightankle.motorsparams.maxPan = 1.57;
        this->rightankle.motorsparams.minPan = -1.57;          
        this->rightankle.motorsparams.maxTilt = 0.5;
        this->rightankle.motorsparams.minTilt = -0.5;

        std::cout << "Constructor PoseRightAnkle" << std::endl;
    }

    void PoseRightAnkle::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        // LOAD CAMERA LEFT
        if (!_sdf->HasElement("joint_pose3dencodersrightankle_pitch"))
            gzerr << "pose3dencodersrightankle plugin missing <joint_pose3dencodersrightankle_pitch> element\n";
        if (!_sdf->HasElement("joint_pose3dencodersrightankle_roll"))
            gzerr << "pose3dencodersrightankle plugin missing <joint_pose3dencodersrightankle_roll> element\n";

        this->rightankle.joint_roll = _model->GetJoint("rankle_roll");
        this->rightankle.joint_tilt = _model->GetJoint("rankle_pitch");

        if (!this->rightankle.joint_pose3dencoders_roll)
            gzerr << "Unable to find joint_pose3dencodersrightankle_roll["
                << _sdf->GetElement("joint_pose3dencodersrightankle_roll")->GetValueString() << "]\n";
        if (!this->rightankle.joint_pose3dencoders_tilt)
            gzerr << "Unable to find joint_pose3dencodersrightankle_pitch["
                << _sdf->GetElement("joint_pose3dencodersrightankle_pitch")->GetValueString() << "]\n"; 
                
        this->rightankle.link_roll = _model->GetLink("right_foot");
        this->rightankle.link_tilt = _model->GetLink("rightankle_pitch");

        //LOAD TORQUE        
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the DiffDrive plugin.\n";
            this->stiffness = 5.0;
        }

        //LOAD POSE3DMOTORS
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&PoseRightAnkle::OnUpdate, this));
    }

    void PoseRightAnkle::Init () {}

    void PoseRightAnkle::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;
        
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        if (this->count == 0) {
            this->count++;
            pthread_t thr_ice;
            pthread_create(&thr_ice, NULL, &thread_RightAnkleICE, (void*) this);
            
            this->rightankle.encoders.roll = 0;    
            this->rightankle.encoders.tilt = 0;
        } else {
            //          ----------ENCODERS----------
            //GET pose3dencoders data from the right ankle (PAN&TILT)
            this->rightankle.encoders.roll = this->rightankle.link_roll->GetRelativePose().rot.GetAsEuler().y;    
            this->rightankle.encoders.tilt = this->rightankle.link_tilt->GetRelativePose().rot.GetAsEuler().x;
        }

        //          ----------MOTORS----------
        if (this->rightankle.motorsdata.roll >= 0) {
            if (this->rightankle.encoders.roll < this->rightankle.motorsdata.roll) {
                this->rightankle.joint_roll->SetVelocity(0, -0.1);
                this->rightankle.joint_roll->SetMaxForce(0, this->stiffness);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->rightankle.joint_roll->SetVelocity(0, 0.1);
                this->rightankle.joint_roll->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->rightankle.encoders.roll > this->rightankle.motorsdata.roll) {
                this->rightankle.joint_roll->SetVelocity(0, 0.1);
                this->rightankle.joint_roll->SetMaxForce(0, this->stiffness);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->rightankle.joint_roll->SetVelocity(0, -0.1);
                this->rightankle.joint_roll->SetMaxForce(0, this->stiffness);
            }            
        }
        
        if (this->rightankle.motorsdata.tilt >= 0) {
            if (this->rightankle.encoders.tilt < this->rightankle.motorsdata.tilt) {
                this->rightankle.joint_tilt->SetVelocity(0, -0.1);
                this->rightankle.joint_tilt->SetMaxForce(0, this->stiffness);
            } else {
                this->rightankle.joint_tilt->SetVelocity(0, 0.1);
                this->rightankle.joint_tilt->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->rightankle.encoders.tilt > this->rightankle.motorsdata.tilt) {
                this->rightankle.joint_tilt->SetVelocity(0, 0.1);
                this->rightankle.joint_tilt->SetMaxForce(0, this->stiffness);
            } else {
                this->rightankle.joint_tilt->SetVelocity(0, -0.1);
                this->rightankle.joint_tilt->SetMaxForce(0, this->stiffness);
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

        Pose3DEncoders ( gazebo::PoseRightAnkle* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncoders () {}

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

    class Pose3DMotors : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotors (gazebo::PoseRightAnkle* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotors() {}

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

            Ice::ObjectPtr encoders = new Pose3DEncoders(rightankle);
            Ice::ObjectPtr motors = new Pose3DMotors(rightankle);

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
