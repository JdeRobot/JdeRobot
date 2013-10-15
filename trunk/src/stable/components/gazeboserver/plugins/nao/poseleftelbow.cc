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

#include "poseleftelbow.h"

#define RADTODEG 57.29582790

namespace gazebo {

    GZ_REGISTER_MODEL_PLUGIN(PoseLeftElbow)

    PoseLeftElbow::PoseLeftElbow () {
        pthread_mutex_init(&this->mutex_leftelbowencoders, NULL);
        pthread_mutex_init(&this->mutex_leftelbowmotors, NULL);
        this->count = 0;
        this->cycle = 50;
        this->cfgfile_leftelbow = std::string("--Ice.Config=poseleftelbow.cfg");
        
        this->leftelbow.motorsparams.maxPan = 1.57;
        this->leftelbow.motorsparams.minPan = -1.57;          
        this->leftelbow.motorsparams.maxTilt = 0.5;
        this->leftelbow.motorsparams.minTilt = -0.5;

        std::cout << "Constructor PoseLeftElbow" << std::endl;
    }

    void PoseLeftElbow::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        // LOAD CAMERA LEFT
        if (!_sdf->HasElement("joint_pose3dencodersleftelbow_yaw"))
            gzerr << "pose3dencodersleftelbow plugin missing <joint_pose3dencodersleftelbow_yaw> element\n";
        if (!_sdf->HasElement("joint_pose3dencodersleftelbow_roll"))
            gzerr << "pose3dencodersleftelbow plugin missing <joint_pose3dencodersleftelbow_roll> element\n";

        this->leftelbow.joint_pan = _model->GetJoint("lelbow_yaw");
        this->leftelbow.joint_roll = _model->GetJoint("lelbow_roll");

        if (!this->leftelbow.joint_pan)
            gzerr << "Unable to find joint_pose3dencodersleftelbow_yaw["
                << _sdf->GetElement("joint_pose3dencodersleftelbow_yaw")->GetValueString() << "]\n"; 
        if (!this->leftelbow.joint_roll)
            gzerr << "Unable to find joint_pose3dencodersleftelbow_roll["
                << _sdf->GetElement("joint_pose3dencodersleftelbow_roll")->GetValueString() << "]\n";
                
        this->leftelbow.link_pan = _model->GetLink("leftelbow_yaw");
        this->leftelbow.link_roll = _model->GetLink("left_lower_arm");

        //LOAD TORQUE        
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the DiffDrive plugin.\n";
            this->stiffness = 5.0;
        }

        //LOAD POSE3DMOTORS
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&PoseLeftElbow::OnUpdate, this));
    }

    void PoseLeftElbow::Init () {}

    void PoseLeftElbow::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;

        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        if (this->count == 0) {
            this->count++;
            pthread_t thr_ice;
            pthread_create(&thr_ice, NULL, &thread_LeftElbowICE, (void*) this);
        }

        //          ----------ENCODERS----------
        //GET pose3dencoders data from the left elbow (PAN&TILT)
//        this->leftelbow.encoder.pan = this->leftelbow.leftelbow_link_pan->GetRelativePose().rot.GetAsEuler().z;    
//        this->leftelbow.encoder.roll = this->leftelbow.leftelbow_link_roll->GetRelativePose().rot.GetAsEuler().y;
        
        this->leftelbow.encoders.pan = - this->leftelbow.joint_pan->GetAngle(0).Radian();
        this->leftelbow.encoders.roll = - this->leftelbow.joint_roll->GetAngle(0).Radian();

        //          ----------MOTORS----------
        if (this->leftelbow.motorsdata.pan >= 0) {
            if (this->leftelbow.encoders.pan < this->leftelbow.motorsdata.pan) {
                this->leftelbow.joint_pan->SetVelocity(0, -0.1);
                this->leftelbow.joint_pan->SetMaxForce(0, this->stiffness);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->leftelbow.joint_pan->SetVelocity(0, 0.1);
                this->leftelbow.joint_pan->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->leftelbow.encoders.pan > this->leftelbow.motorsdata.pan) {
                this->leftelbow.joint_pan->SetVelocity(0, 0.1);
                this->leftelbow.joint_pan->SetMaxForce(0, this->stiffness);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->leftelbow.joint_pan->SetVelocity(0, -0.1);
                this->leftelbow.joint_pan->SetMaxForce(0, this->stiffness);
            }            
        }
        
        if (this->leftelbow.motorsdata.roll >= 0) {
            if (this->leftelbow.encoders.roll < this->leftelbow.motorsdata.roll) {
                this->leftelbow.joint_roll->SetVelocity(0, -0.1);
                this->leftelbow.joint_roll->SetMaxForce(0, this->stiffness);
            } else {
                this->leftelbow.joint_roll->SetVelocity(0, 0.1);
                this->leftelbow.joint_roll->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->leftelbow.encoders.roll > this->leftelbow.motorsdata.roll) {
                this->leftelbow.joint_roll->SetVelocity(0, 0.1);
                this->leftelbow.joint_roll->SetMaxForce(0, this->stiffness);
            } else {
                this->leftelbow.joint_roll->SetVelocity(0, -0.1);
                this->leftelbow.joint_roll->SetMaxForce(0, this->stiffness);
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

        Pose3DEncoders ( gazebo::PoseLeftElbow* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncoders () {}

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftelbowencoders);
            
            pose3DEncodersData->x = pose->leftelbow.encoders.x;
            pose3DEncodersData->y = pose->leftelbow.encoders.y;
            pose3DEncodersData->z = pose->leftelbow.encoders.z;
            pose3DEncodersData->pan = pose->leftelbow.encoders.pan;
            pose3DEncodersData->tilt = pose->leftelbow.encoders.tilt;
            pose3DEncodersData->roll = pose->leftelbow.encoders.roll;
            pose3DEncodersData->clock = pose->leftelbow.encoders.clock;
            pose3DEncodersData->maxPan = pose->leftelbow.encoders.maxPan;
            pose3DEncodersData->minPan = pose->leftelbow.encoders.minPan;
            pose3DEncodersData->maxTilt = pose->leftelbow.encoders.maxTilt;
            pose3DEncodersData->minTilt = pose->leftelbow.encoders.minTilt;
            
            pthread_mutex_unlock(&pose->mutex_leftelbowencoders);

            return pose3DEncodersData;
        }

        gazebo::PoseLeftElbow* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;
    };

    class Pose3DMotors : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotors (gazebo::PoseLeftElbow* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotors() {}

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftelbowmotors);
            
            pose3DMotorsData->x = pose->leftelbow.motorsdata.x;
            pose3DMotorsData->y = pose->leftelbow.motorsdata.y;
            pose3DMotorsData->z = pose->leftelbow.motorsdata.z;
            pose3DMotorsData->pan = pose->leftelbow.motorsdata.pan;
            pose3DMotorsData->tilt = pose->leftelbow.motorsdata.tilt;
            pose3DMotorsData->roll = pose->leftelbow.motorsdata.roll;
            pose3DMotorsData->panSpeed = pose->leftelbow.motorsdata.panSpeed;
            pose3DMotorsData->tiltSpeed = pose->leftelbow.motorsdata.tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftelbowmotors);

            return pose3DMotorsData;
        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftelbowmotors);
            
            pose3DMotorsParams->maxPan = pose->leftelbow.motorsparams.maxPan;
            pose3DMotorsParams->minPan = pose->leftelbow.motorsparams.minPan;
            pose3DMotorsParams->maxTilt = pose->leftelbow.motorsparams.maxTilt;
            pose3DMotorsParams->minTilt = pose->leftelbow.motorsparams.minTilt;
            pose3DMotorsParams->maxPanSpeed = pose->leftelbow.motorsparams.maxPanSpeed;
            pose3DMotorsParams->maxTiltSpeed = pose->leftelbow.motorsparams.maxTiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftelbowmotors);
            
            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_leftelbowmotors);
            
            pose->leftelbow.motorsdata.x = data->x;
            pose->leftelbow.motorsdata.y = data->y;
            pose->leftelbow.motorsdata.z = data->z;
            pose->leftelbow.motorsdata.pan = data->pan;
            pose->leftelbow.motorsdata.tilt = data->tilt;
            pose->leftelbow.motorsdata.roll = data->roll;
            pose->leftelbow.motorsdata.panSpeed = data->panSpeed;
            pose->leftelbow.motorsdata.tiltSpeed = data->tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_leftelbowmotors);
        }

        gazebo::PoseLeftElbow* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;
    };

    void* thread_LeftElbowICE ( void* v ) {

        gazebo::PoseLeftElbow* leftelbow = (gazebo::PoseLeftElbow*)v;
        char* name = (char*) leftelbow->cfgfile_leftelbow.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string EndpointsEncoders = prop->getProperty("PoseLeftElbowEncoders.Endpoints");
            std::cout << "PoseNeckEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("PoseLeftElbowMotors.Endpoints");
            std::cout << "PoseNeckMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterLeftElbowEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterLeftElbowMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncoders(leftelbow);
            Ice::ObjectPtr motors = new Pose3DMotors(leftelbow);

            AdapterEncoders->add(encoders, ic->stringToIdentity("LeftElbowEncoders"));
            AdapterMotors->add(motors, ic->stringToIdentity("LeftElbowMotors"));

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
