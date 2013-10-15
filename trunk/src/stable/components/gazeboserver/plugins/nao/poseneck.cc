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

#include "poseneck.h"

#define RADTODEG 57.29582790

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(PoseNeck)

    PoseNeck::PoseNeck () {
        pthread_mutex_init(&this->mutex_neckencoders, NULL);
        pthread_mutex_init(&this->mutex_neckmotors, NULL);
        this->count = 0;
        this->cycle = 50;
        this->cfgfile_neck = std::string("--Ice.Config=poseneck.cfg");
        
        this->neck.motorsparams.maxPan = 1.57;
        this->neck.motorsparams.minPan = -1.57;          
        this->neck.motorsparams.maxTilt = 0.5;
        this->neck.motorsparams.minTilt = -0.5;

        std::cout << "Constructor PoseNeck" << std::endl;
    }

    void PoseNeck::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        // LOAD CAMERA LEFT
        if (!_sdf->HasElement("bottom_joint_pose3dencodersneck_pan"))
            gzerr << "pose3dencodersneck plugin missing <bottom_joint_pose3dencodersneck_pan> element\n";
        if (!_sdf->HasElement("top_joint_pose3dencodersneck_tilt"))
            gzerr << "pose3dencodersneck plugin missing <top_joint_pose3dencodersneck_tilt> element\n";

        this->neck.joint_pan = _model->GetJoint("neck_pan");
        this->neck.joint_tilt = _model->GetJoint("neck_tilt");

        if (!this->neck.joint_pan)
            gzerr << "Unable to find bottom_joint_pose3dencodersneck_pan["
                << _sdf->GetElement("bottom_joint_pose3dencodersneck_pan")->GetValueString() << "]\n"; 
        if (!this->neck.joint_tilt)
            gzerr << "Unable to find top_joint_pose3dencodersneck_tilt["
                << _sdf->GetElement("top_joint_pose3dencodersneck_tilt")->GetValueString() << "]\n";
                
        this->neck.link_pan = _model->GetLink("head_pan");
        this->neck.link_tilt = _model->GetLink("head_tilt");

        //LOAD TORQUE        
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the DiffDrive plugin.\n";
            this->stiffness = 5.0;
        }

        //LOAD POSE3DMOTORS
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
            boost::bind(&PoseNeck::OnUpdate, this));

    }

    void PoseNeck::Init () {}

    void PoseNeck::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;

        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        if (this->count == 0) {
            this->count++;
            pthread_t thr_ice;
            pthread_create(&thr_ice, NULL, &thread_NeckICE, (void*) this);
        }

        //          ----------ENCODERS----------
        //GET pose3dencoders data from the neck (PAN&TILT)
        this->neck.encoders.pan = this->neck.link_pan->GetRelativePose().rot.GetAsEuler().z;    
        this->neck.encoders.tilt = this->neck.link_tilt->GetRelativePose().rot.GetAsEuler().x;
        
        //          ----------MOTORS----------
        if (this->neck.motorsdata.pan >= 0) {
            if (this->neck.encoders.pan < this->neck.motorsdata.pan) {
                this->neck.joint_pan->SetVelocity(0, -0.1);
                this->neck.joint_pan->SetMaxForce(0, this->stiffness);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->neck.joint_pan->SetVelocity(0, 0.1);
                this->neck.joint_pan->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->neck.encoders.pan > this->neck.motorsdata.pan) {
                this->neck.joint_pan->SetVelocity(0, 0.1);
                this->neck.joint_pan->SetMaxForce(0, this->stiffness);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->neck.joint_pan->SetVelocity(0, -0.1);
                this->neck.joint_pan->SetMaxForce(0, this->stiffness);
            }            
        }
        
        if (this->neck.motorsdata.tilt >= 0) {
            if (this->neck.encoders.tilt < this->neck.motorsdata.tilt) {
                this->neck.joint_tilt->SetVelocity(0, -0.1);
                this->neck.joint_tilt->SetMaxForce(0, this->stiffness);
            } else {
                this->neck.joint_tilt->SetVelocity(0, 0.1);
                this->neck.joint_tilt->SetMaxForce(0, this->stiffness);
            }
        } else {
            if (this->neck.encoders.tilt > this->neck.motorsdata.tilt) {
                this->neck.joint_tilt->SetVelocity(0, 0.1);
                this->neck.joint_tilt->SetMaxForce(0, this->stiffness);
            } else {
                this->neck.joint_tilt->SetVelocity(0, -0.1);
                this->neck.joint_tilt->SetMaxForce(0, this->stiffness);
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

        Pose3DEncoders ( gazebo::PoseNeck* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncoders () {}

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_neckencoders);
            
            pose3DEncodersData->x = pose->neck.encoders.x;
            pose3DEncodersData->y = pose->neck.encoders.y;
            pose3DEncodersData->z = pose->neck.encoders.z;
            pose3DEncodersData->pan = pose->neck.encoders.pan;
            pose3DEncodersData->tilt = pose->neck.encoders.tilt;
            pose3DEncodersData->roll = pose->neck.encoders.roll;
            pose3DEncodersData->clock = pose->neck.encoders.clock;
            pose3DEncodersData->maxPan = pose->neck.encoders.maxPan;
            pose3DEncodersData->minPan = pose->neck.encoders.minPan;
            pose3DEncodersData->maxTilt = pose->neck.encoders.maxTilt;
            pose3DEncodersData->minTilt = pose->neck.encoders.minTilt;
            
            pthread_mutex_unlock(&pose->mutex_neckencoders);

            return pose3DEncodersData;
        }

        gazebo::PoseNeck* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;
    };

    class Pose3DMotors : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotors (gazebo::PoseNeck* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotors() {}

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_neckmotors);
            
            pose3DMotorsData->x = pose->neck.motorsdata.x;
            pose3DMotorsData->y = pose->neck.motorsdata.y;
            pose3DMotorsData->z = pose->neck.motorsdata.z;
            pose3DMotorsData->pan = pose->neck.motorsdata.pan;
            pose3DMotorsData->tilt = pose->neck.motorsdata.tilt;
            pose3DMotorsData->roll = pose->neck.motorsdata.roll;
            pose3DMotorsData->panSpeed = pose->neck.motorsdata.panSpeed;
            pose3DMotorsData->tiltSpeed = pose->neck.motorsdata.tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_neckmotors);

            return pose3DMotorsData;

        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams ( const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_neckmotors);
            
            pose3DMotorsParams->maxPan = pose->neck.motorsparams.maxPan;
            pose3DMotorsParams->minPan = pose->neck.motorsparams.minPan;
            pose3DMotorsParams->maxTilt = pose->neck.motorsparams.maxTilt;
            pose3DMotorsParams->minTilt = pose->neck.motorsparams.minTilt;
            pose3DMotorsParams->maxPanSpeed = pose->neck.motorsparams.maxPanSpeed;
            pose3DMotorsParams->maxTiltSpeed = pose->neck.motorsparams.maxTiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_neckmotors);
            
            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData ( const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current& ) {
            pthread_mutex_lock(&pose->mutex_neckmotors);
            
            pose->neck.motorsdata.x = data->x;
            pose->neck.motorsdata.y = data->y;
            pose->neck.motorsdata.z = data->z;
            pose->neck.motorsdata.pan = data->pan;
            pose->neck.motorsdata.tilt = data->tilt;
            pose->neck.motorsdata.roll = data->roll;
            pose->neck.motorsdata.panSpeed = data->panSpeed;
            pose->neck.motorsdata.tiltSpeed = data->tiltSpeed;
            
            pthread_mutex_unlock(&pose->mutex_neckmotors);

        }

        gazebo::PoseNeck* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;
    };

    void* thread_NeckICE ( void* v ) {

        gazebo::PoseNeck* neck = (gazebo::PoseNeck*)v;
        char* name = (char*) neck->cfgfile_neck.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string EndpointsEncoders = prop->getProperty("PoseNeckEncoders.Endpoints");
            std::cout << "PoseNeckEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("PoseNeckMotors.Endpoints");
            std::cout << "PoseNeckMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterNeckEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterNeckMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncoders(neck);
            Ice::ObjectPtr motors = new Pose3DMotors(neck);

            AdapterEncoders->add(encoders, ic->stringToIdentity("NeckEncoders"));
            AdapterMotors->add(motors, ic->stringToIdentity("NeckMotors"));

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
