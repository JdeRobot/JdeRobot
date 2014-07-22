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

#include "poseleftelbow.h"

namespace gazebo {

    GZ_REGISTER_MODEL_PLUGIN(PoseLeftElbow)

    PoseLeftElbow::PoseLeftElbow () {
        pthread_mutex_init(&this->mutex_leftelbowencoders, NULL);
        pthread_mutex_init(&this->mutex_leftelbowmotors, NULL);
        this->cycle = 50;
        this->cfgfile_leftelbow = std::string("--Ice.Config=poseleftelbow.cfg");
        this->modelYaw = std::string("joint_poseleftelbow_yaw");
        this->modelRoll = std::string("joint_poseleftelbow_roll");

        std::cout << "Constructor PoseLeftElbow" << std::endl;
    }

    void PoseLeftElbow::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        if (!_sdf->HasElement(this->modelYaw))
            gzerr << "PoseLeftElbow plugin missing <" << this->modelYaw << "> element\n";
        if (!_sdf->HasElement(this->modelRoll))
            gzerr << "PoseLeftElbow plugin missing <" << this->modelRoll << "> element\n";
        
        std::string elemYaw = std::string(_sdf->GetElement(this->modelYaw)->GetValueString());
        std::string elemRoll = std::string(_sdf->GetElement(this->modelRoll)->GetValueString());
            
        if (!_sdf->HasElement(elemYaw))
            gzerr << "PoseLeftElbow plugin missing <" << elemYaw << "> element\n";
        if (!_sdf->HasElement(elemRoll))
            gzerr << "PoseLeftElbow plugin missing <" << elemRoll << "> element\n";
            
        this->leftelbow.joint_yaw = _model->GetJoint(elemYaw);
        this->leftelbow.joint_roll = _model->GetJoint(elemRoll);

        this->maxYaw = (float) this->leftelbow.joint_yaw->GetUpperLimit(0).Radian();
        this->minYaw = (float) this->leftelbow.joint_yaw->GetLowerLimit(0).Radian();
        this->maxRoll = (float) this->leftelbow.joint_roll->GetUpperLimit(0).Radian();
        this->minRoll = (float) this->leftelbow.joint_roll->GetLowerLimit(0).Radian();

        // Load torque
        if (_sdf->HasElement("torque"))
            this->stiffness = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the left elbow plugin.\n";
            this->stiffness = 5.0;
        }
        
        pthread_t thr_ice;
        pthread_create(&thr_ice, NULL, &thread_LeftElbowICE, (void*) this);

        // Load OnUpdate method
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&PoseLeftElbow::OnUpdate, this));
    }

    void PoseLeftElbow::Init () {
        this->leftelbow.encoders.pan = 0.0;
        this->leftelbow.encoders.roll = 0.0;
        
        this->leftelbow.motorsdata.pan = 0.0;
        this->leftelbow.motorsdata.roll = 0.0;
    }

    void PoseLeftElbow::OnUpdate () {
        long totalb, totala, diff;
        struct timeval a, b;

        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        //          ----------ENCODERS----------
        //GET pose3dencoders data from the left elbow (PAN&ROLL)
        pthread_mutex_lock(&this->mutex_leftelbowencoders);
        
        this->leftelbow.encoders.pan = this->leftelbow.joint_yaw->GetAngle(0).Radian();
        this->leftelbow.encoders.roll = this->leftelbow.joint_roll->GetAngle(0).Radian();
        
        pthread_mutex_unlock(&this->mutex_leftelbowencoders);

        //          ----------MOTORS----------
        this->leftelbow.joint_yaw->SetMaxForce(0, this->stiffness);
        this->leftelbow.joint_roll->SetMaxForce(0, this->stiffness);
        
        pthread_mutex_lock(&this->mutex_leftelbowmotors);
        
        float yawSpeed = - this->leftelbow.motorsdata.pan - this->leftelbow.encoders.pan;
        if ((std::abs(yawSpeed) < 0.1) && (std::abs(yawSpeed) > 0.001))
            yawSpeed = 0.1;
        
        float rollSpeed = - this->leftelbow.motorsdata.roll - this->leftelbow.encoders.roll;
        if ((std::abs(rollSpeed) < 0.1) && (std::abs(rollSpeed) > 0.001))
            rollSpeed = 0.1;
        
        this->leftelbow.joint_yaw->SetVelocity(0, yawSpeed);
        this->leftelbow.joint_roll->SetVelocity(0, rollSpeed);

        pthread_mutex_unlock(&this->mutex_leftelbowmotors);

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;

        diff = (totalb - totala) / 1000;
        diff = cycle - diff;

        if (diff < 10)
            diff = 10;

        sleep(diff / 1000);
    }
    
    class Pose3DEncodersLE : virtual public jderobot::Pose3DEncoders {
    public:

        Pose3DEncodersLE ( gazebo::PoseLeftElbow* pose ) : pose3DEncodersData ( new jderobot::Pose3DEncodersData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DEncodersLE () {}

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

    class Pose3DMotorsLE : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotorsLE (gazebo::PoseLeftElbow* pose) : pose3DMotorsData ( new jderobot::Pose3DMotorsData() ) {
            this->pose = pose;
        }

        virtual ~Pose3DMotorsLE () {}

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
            std::cout << "PoseLeftElbowEncoders Endpoints > " << EndpointsEncoders << std::endl;
            std::string EndpointsMotors = prop->getProperty("PoseLeftElbowMotors.Endpoints");
            std::cout << "PoseLeftElbowMotors Endpoints > " << EndpointsMotors << std::endl;

            Ice::ObjectAdapterPtr AdapterEncoders =
                    ic->createObjectAdapterWithEndpoints("AdapterLeftElbowEncoders", EndpointsEncoders);
            Ice::ObjectAdapterPtr AdapterMotors =
                    ic->createObjectAdapterWithEndpoints("AdapterLeftElbowMotors", EndpointsMotors);

            Ice::ObjectPtr encoders = new Pose3DEncodersLE(leftelbow);
            Ice::ObjectPtr motors = new Pose3DMotorsLE(leftelbow);

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
