#include "pose3dencoders.h"
//#include "pose3dmotors.h"

#define RADTODEG 57.29582790


namespace gazebo {

    void *pose3dencodersICE(void* v);

    GZ_REGISTER_MODEL_PLUGIN(Pose3DEncoders)

    Pose3DEncoders::Pose3DEncoders() {
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_init(&mutexPose3DEncoders, NULL);
        count = 0;
        //Init right joint properties defined in models/pioneer2dx.model
        this->cameraRight.motor.maxPan = 1.57;
        this->cameraRight.motor.minPan = -1.57;
        this->cameraRight.motor.maxTilt = 0.4;
        this->cameraRight.motor.minTilt = -0.4;
        //Init left joint properties defined in models/pioneer2dx.model
        this->cameraLeft.motor.maxPan = 1.57;
        this->cameraLeft.motor.minPan = -1.57;
        this->cameraLeft.motor.maxTilt = 0.4;
        this->cameraLeft.motor.minTilt = -0.4;


        std::cout << "constructor pose3dencoders" << std::endl;
    }

    void Pose3DEncoders::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {

        this->model = _model;

        // LOAD CAMERA LEFT        
        if (!_sdf->HasElement("left_joint_pose3dencoders_pan"))
            gzerr << "pose3dencoders plugin missing <left_joint_pose3dencoders_pan> element\n";
        if (!_sdf->HasElement("left_joint_pose3dencoders_tilt"))
            gzerr << "pose3dencoders plugin missing <left_joint_pose3dencoders_tilt> element\n";

        this->cameraLeft.joint_pose3dencoders_pan = this->model->GetJoint("pan_joint_left");
        this->cameraLeft.joint_pose3dencoders_tilt = this->model->GetJoint("tilt_joint_left");



        if (!this->cameraLeft.joint_pose3dencoders_pan)
            gzerr << "Unable to find left joint pose3dencoders_pan["
                << _sdf->GetElement("left_joint_pose3dencoders_pan")->GetValueString() << "]\n";
        if (!this->cameraLeft.joint_pose3dencoders_tilt)
            gzerr << "Unable to find left joint pose3dencoders_tilt["
                << _sdf->GetElement("left_joint_pose3dencoders_tilt")->GetValueString() << "]\n";
        this->cameraLeft.camera_link_pan = this->model->GetLink("camera_column_body_left");
        this->cameraLeft.camera_link_tilt = this->model->GetLink("camera_top_body_left");

        //LOAD CAMERA RIGHT
        if (!_sdf->HasElement("right_joint_pose3dencoders_pan"))
            gzerr << "Motors plugin missing <right_joint_pose3dencoders_pan> element\n";
        if (!_sdf->HasElement("right_joint_pose3dencoders_tilt"))
            gzerr << "Motors plugin missing <right_joint_pose3dencoders_tilt> element\n";


        this->cameraRight.joint_pose3dencoders_pan = this->model->GetJoint("pan_joint_right");
        this->cameraRight.joint_pose3dencoders_tilt = this->model->GetJoint("tilt_joint_right");


        if (!this->cameraRight.joint_pose3dencoders_pan)
            gzerr << "Unable to find right joint pose3dencoders_pan["
                << _sdf->GetElement("right_joint_pose3dencoders_pan")->GetValueString() << "]\n";
        if (!this->cameraRight.joint_pose3dencoders_tilt)
            gzerr << "Unable to find right joint pose3dencoders_tilt["
                << _sdf->GetElement("right_joint_pose3dencoders_tilt")->GetValueString() << "]\n";
        this->cameraRight.camera_link_pan = this->model->GetLink("camera_column_body_right");
        this->cameraRight.camera_link_tilt = this->model->GetLink("camera_top_body_right");


        //LOAD TORQUE        
        if (_sdf->HasElement("torque"))
            this->torque = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the DiffDrive plugin.\n";
            this->torque = 5.0;
        }

        //LOAD POSE3DMOTORS

        this->updateConnection = event::Events::ConnectWorldUpdateStart(
                boost::bind(&Pose3DEncoders::OnUpdate, this));


    }

    void Pose3DEncoders::Init() {



    }

    void Pose3DEncoders::OnUpdate() {

        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;
        cycle = 50;

        if (count == 0) {
            count++;
            namePose3DEncoders = std::string("--Ice.Config=pose3dencoders.cfg");
            pthread_t thr_gui;
            pthread_create(&thr_gui, NULL, &pose3dencodersICE, (void*) this);
        }

        //          ----------ENCODERS----------
        //GET pose3dencoders data from left_camera (PAN&TILT)

        if (this->cameraLeft.camera_link_pan->GetRelativePose().rot.GetAsEuler().z < 0) {
            this->cameraLeft.encoder.pan = -(3.14146 + this->cameraLeft.camera_link_pan->GetRelativePose().rot.GetAsEuler().z);
        } else {
            this->cameraLeft.encoder.pan = 3.14146 - this->cameraLeft.camera_link_pan->GetRelativePose().rot.GetAsEuler().z;
        }

        this->cameraLeft.encoder.tilt = this->cameraLeft.camera_link_tilt->GetRelativePose().rot.GetAsEuler().y;

        //GET pose3dencoders data from left_camera (PAN&TILT)
        if (this->cameraRight.camera_link_pan->GetRelativePose().rot.GetAsEuler().z < 0) {
            this->cameraRight.encoder.pan = -(3.14146 + this->cameraRight.camera_link_pan->GetRelativePose().rot.GetAsEuler().z);
        } else {
            this->cameraRight.encoder.pan = 3.14146 - this->cameraRight.camera_link_pan->GetRelativePose().rot.GetAsEuler().z;
        }

        this->cameraRight.encoder.tilt = this->cameraRight.camera_link_tilt->GetRelativePose().rot.GetAsEuler().y;

        //          ----------MOTORS----------
        if (this->cameraLeft.motor.pan >= 0) {
            if (this->cameraLeft.encoder.pan < this->cameraLeft.motor.pan) {
                this->cameraLeft.joint_pose3dencoders_pan->SetVelocity(0, -0.1);
                this->cameraLeft.joint_pose3dencoders_pan->SetMaxForce(0, this->torque);
            } else {
                this->cameraLeft.joint_pose3dencoders_pan->SetVelocity(0, 0.1);
                this->cameraLeft.joint_pose3dencoders_pan->SetMaxForce(0, this->torque);
            }
        } else {
            if (this->cameraLeft.encoder.pan > this->cameraLeft.motor.pan) {
                this->cameraLeft.joint_pose3dencoders_pan->SetVelocity(0, 0.1);
                this->cameraLeft.joint_pose3dencoders_pan->SetMaxForce(0, this->torque);
            } else {
                this->cameraLeft.joint_pose3dencoders_pan->SetVelocity(0, -0.1);
                this->cameraLeft.joint_pose3dencoders_pan->SetMaxForce(0, this->torque);
            }
        }
        if (this->cameraRight.motor.pan >= 0) {
            if (this->cameraRight.encoder.pan < this->cameraRight.motor.pan) {
                this->cameraRight.joint_pose3dencoders_pan->SetVelocity(0, -0.1);
                this->cameraRight.joint_pose3dencoders_pan->SetMaxForce(0, this->torque);
            } else {
                this->cameraRight.joint_pose3dencoders_pan->SetVelocity(0, 0.1);
                this->cameraRight.joint_pose3dencoders_pan->SetMaxForce(0, this->torque);
            }
        } else {
            if (this->cameraRight.encoder.pan > this->cameraRight.motor.pan) {
                this->cameraRight.joint_pose3dencoders_pan->SetVelocity(0, 0.1);
                this->cameraRight.joint_pose3dencoders_pan->SetMaxForce(0, this->torque);
            } else {
                this->cameraRight.joint_pose3dencoders_pan->SetVelocity(0, -0.1);
                this->cameraRight.joint_pose3dencoders_pan->SetMaxForce(0, this->torque);
            }
        }



        if (this->cameraLeft.motor.tilt >= 0) {
            if (this->cameraLeft.encoder.tilt < this->cameraLeft.motor.tilt) {
                this->cameraLeft.joint_pose3dencoders_tilt->SetVelocity(0, -0.1);
                this->cameraLeft.joint_pose3dencoders_tilt->SetMaxForce(0, this->torque);
            } else {
                this->cameraLeft.joint_pose3dencoders_tilt->SetVelocity(0, 0.1);
                this->cameraLeft.joint_pose3dencoders_tilt->SetMaxForce(0, this->torque);
            }
        } else {
            if (this->cameraLeft.encoder.tilt > this->cameraLeft.motor.tilt) {
                this->cameraLeft.joint_pose3dencoders_tilt->SetVelocity(0, 0.1);
                this->cameraLeft.joint_pose3dencoders_tilt->SetMaxForce(0, this->torque);
            } else {
                this->cameraLeft.joint_pose3dencoders_tilt->SetVelocity(0, -0.1);
                this->cameraLeft.joint_pose3dencoders_tilt->SetMaxForce(0, this->torque);
            }
        }
        if (this->cameraRight.motor.tilt >= 0) {
            if (this->cameraRight.encoder.tilt < this->cameraRight.motor.tilt) {
                this->cameraRight.joint_pose3dencoders_tilt->SetVelocity(0, -0.1);
                this->cameraRight.joint_pose3dencoders_tilt->SetMaxForce(0, this->torque);
            } else {
                this->cameraRight.joint_pose3dencoders_tilt->SetVelocity(0, 0.1);
                this->cameraRight.joint_pose3dencoders_tilt->SetMaxForce(0, this->torque);
            }
        } else {
            if (this->cameraRight.encoder.tilt > this->cameraRight.motor.tilt) {
                this->cameraRight.joint_pose3dencoders_tilt->SetVelocity(0, 0.1);
                this->cameraRight.joint_pose3dencoders_tilt->SetMaxForce(0, this->torque);
            } else {
                this->cameraRight.joint_pose3dencoders_tilt->SetVelocity(0, -0.1);
                this->cameraRight.joint_pose3dencoders_tilt->SetMaxForce(0, this->torque);
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

    class Pose3DEncodersI : virtual public jderobot::Pose3DEncoders {
    public:

        Pose3DEncodersI(gazebo::Pose3DEncoders* pose) : pose3DEncodersData(new jderobot::Pose3DEncodersData()) {

            this->pose = pose;

        }

        virtual ~Pose3DEncodersI() {

        }

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData(const Ice::Current&) {

            pose3DEncodersData->x = 0;
            pose3DEncodersData->y = 0;
            pose3DEncodersData->z = 0;
            pose3DEncodersData->pan = pose->cameraLeft.encoder.pan;
            pose3DEncodersData->tilt = pose->cameraLeft.encoder.tilt;
            pose3DEncodersData->clock = 0;
            pose3DEncodersData->roll = 0;
            pose3DEncodersData->maxPan = pose->cameraLeft.motor.maxPan;
            pose3DEncodersData->minPan = pose->cameraLeft.motor.minPan;
            pose3DEncodersData->maxTilt = pose->cameraLeft.motor.maxTilt;
            pose3DEncodersData->minTilt = pose->cameraLeft.motor.minTilt;

            return pose3DEncodersData;

        }

        gazebo::Pose3DEncoders* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;

    };

    class Pose3DEncodersII : virtual public jderobot::Pose3DEncoders {
    public:

        Pose3DEncodersII(gazebo::Pose3DEncoders* pose) : pose3DEncodersData(new jderobot::Pose3DEncodersData()) {

            this->pose = pose;

        }

        virtual ~Pose3DEncodersII() {

        }

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData(const Ice::Current&) {

            pose3DEncodersData->x = 0;
            pose3DEncodersData->y = 0;
            pose3DEncodersData->z = 0;
            pose3DEncodersData->pan = pose->cameraRight.encoder.pan;
            pose3DEncodersData->tilt = pose->cameraRight.encoder.tilt;
            pose3DEncodersData->clock = 0;
            pose3DEncodersData->roll = 0;
            pose3DEncodersData->maxPan = pose->cameraLeft.motor.maxPan;
            pose3DEncodersData->minPan = pose->cameraLeft.motor.minPan;
            pose3DEncodersData->maxTilt = pose->cameraLeft.motor.maxTilt;
            pose3DEncodersData->minTilt = pose->cameraLeft.motor.minTilt;

            return pose3DEncodersData;

        }

        gazebo::Pose3DEncoders* pose;

    private:
        jderobot::Pose3DEncodersDataPtr pose3DEncodersData;

    };

    class Pose3DMotorsI : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotorsI(gazebo::Pose3DEncoders* pose) : pose3DMotorsData(new jderobot::Pose3DMotorsData()) {

            this->pose = pose;

        }

        virtual ~Pose3DMotorsI() {

        }

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData(const Ice::Current&) {

            pose3DMotorsData->x = 0;
            pose3DMotorsData->y = 0;
            pose3DMotorsData->z = 0;
            pose3DMotorsData->pan = pose->cameraLeft.encoder.pan;
            pose3DMotorsData->tilt = pose->cameraLeft.encoder.tilt;
            pose3DMotorsData->roll = 0;
            pose3DMotorsData->panSpeed = 0;
            pose3DMotorsData->tiltSpeed = 0;

            return pose3DMotorsData;

        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const Ice::Current&) {

            pose3DMotorsParams->maxPan = pose->cameraLeft.motor.maxPan;
            pose3DMotorsParams->minPan = pose->cameraLeft.motor.minPan;
            pose3DMotorsParams->maxTilt = pose->cameraLeft.motor.maxTilt;
            pose3DMotorsParams->minTilt = pose->cameraLeft.motor.minTilt;
            pose3DMotorsParams->maxPanSpeed = 0;
            pose3DMotorsParams->maxTiltSpeed = 0;

            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData(const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current&) {

            pose->cameraLeft.motor.x = data->x;
            pose->cameraLeft.motor.y = data->y;
            pose->cameraLeft.motor.z = data->z;
            pose->cameraLeft.motor.pan = data->pan;
            pose->cameraLeft.motor.tilt = data->tilt;
            pose->cameraLeft.motor.roll = data->roll;
            pose->cameraLeft.motor.panSpeed = data->panSpeed;
            pose->cameraLeft.motor.tiltSpeed = data->tiltSpeed;

        }

        gazebo::Pose3DEncoders* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;


    };

    class Pose3DMotorsII : virtual public jderobot::Pose3DMotors {
    public:

        Pose3DMotorsII(gazebo::Pose3DEncoders* pose) : pose3DMotorsData(new jderobot::Pose3DMotorsData()) {

            this->pose = pose;

        }

        virtual ~Pose3DMotorsII() {

        }

        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData(const Ice::Current&) {

            pose3DMotorsData->x = 0;
            pose3DMotorsData->y = 0;
            pose3DMotorsData->z = 0;
            pose3DMotorsData->pan = pose->cameraRight.encoder.pan;
            pose3DMotorsData->tilt = pose->cameraRight.encoder.tilt;
            pose3DMotorsData->roll = 0;
            pose3DMotorsData->panSpeed = 0;
            pose3DMotorsData->tiltSpeed = 0;

            return pose3DMotorsData;

        }

        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const Ice::Current&) {

            pose3DMotorsParams->maxPan = pose->cameraRight.motor.maxPan;
            pose3DMotorsParams->minPan = pose->cameraRight.motor.minPan;
            pose3DMotorsParams->maxTilt = pose->cameraRight.motor.maxTilt;
            pose3DMotorsParams->minTilt = pose->cameraRight.motor.minTilt;
            pose3DMotorsParams->maxPanSpeed = 0;
            pose3DMotorsParams->maxTiltSpeed = 0;

            return pose3DMotorsParams;
        }

        virtual Ice::Int setPose3DMotorsData(const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current&) {

            pose->cameraRight.motor.x = data->x;
            pose->cameraRight.motor.y = data->y;
            pose->cameraRight.motor.z = data->z;
            pose->cameraRight.motor.pan = data->pan;
            pose->cameraRight.motor.tilt = data->tilt;
            pose->cameraRight.motor.roll = data->roll;
            pose->cameraRight.motor.panSpeed = data->panSpeed;
            pose->cameraRight.motor.tiltSpeed = data->tiltSpeed;

        }

        gazebo::Pose3DEncoders* pose;

    private:
        jderobot::Pose3DMotorsDataPtr pose3DMotorsData;
        jderobot::Pose3DMotorsParamsPtr pose3DMotorsParams;

    };

    void *pose3dencodersICE(void* v) {

        gazebo::Pose3DEncoders* base = (gazebo::Pose3DEncoders*)v;
        char* name = (char*) base->namePose3DEncoders.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {

            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();
            std::string Endpoints1 = prop->getProperty("Pose3DEncodersLeft.Endpoints");
            std::cout << "Pose3DEncodersLeft Endpoints > " << Endpoints1 << std::endl;
            std::string Endpoints2 = prop->getProperty("Pose3DEncodersRight.Endpoints");
            std::cout << "Pose3DEncodersRight Endpoints > " << Endpoints2 << std::endl;
            std::string Endpoints3 = prop->getProperty("Pose3DMotorsLeft.Endpoints");
            std::cout << "Pose3DMotorsLeft Endpoints > " << Endpoints3 << std::endl;
            std::string Endpoints4 = prop->getProperty("Pose3DMotorsRight.Endpoints");
            std::cout << "Pose3DMotorsRight Endpoints > " << Endpoints4 << std::endl;

            Ice::ObjectAdapterPtr adapter1 =
                    ic->createObjectAdapterWithEndpoints("Pose3DEncodersLeft", Endpoints1);
            Ice::ObjectAdapterPtr adapter2 =
                    ic->createObjectAdapterWithEndpoints("Pose3DEncodersRight", Endpoints2);
            Ice::ObjectAdapterPtr adapter3 =
                    ic->createObjectAdapterWithEndpoints("Pose3DMotorsLeft", Endpoints3);
            Ice::ObjectAdapterPtr adapter4 =
                    ic->createObjectAdapterWithEndpoints("Pose3DMotorsRight", Endpoints4);

            Ice::ObjectPtr object1 = new Pose3DEncodersI(base);
            Ice::ObjectPtr object2 = new Pose3DEncodersII(base);
            Ice::ObjectPtr object3 = new Pose3DMotorsI(base);
            Ice::ObjectPtr object4 = new Pose3DMotorsII(base);

            adapter1->add(object1, ic->stringToIdentity("Pose3DEncodersLeft"));
            adapter2->add(object2, ic->stringToIdentity("Pose3DEncodersRight"));
            adapter3->add(object3, ic->stringToIdentity("Pose3DMotorsLeft"));
            adapter4->add(object4, ic->stringToIdentity("Pose3DMotorsRight"));

            adapter1->activate();
            adapter2->activate();
            adapter3->activate();
            adapter4->activate();

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
