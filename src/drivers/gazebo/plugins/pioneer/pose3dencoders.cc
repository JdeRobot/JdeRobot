#include "pose3dencoders.h"
//#include "pose3dmotors.h"



namespace gazebo {

    void *pose3dencodersICE(void* v);

    GZ_REGISTER_MODEL_PLUGIN(Pose3DEncoders)

    Pose3DEncoders::Pose3DEncoders() {
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_init(&mutexPose3DEncoders, NULL);
        count = 0;
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
                << _sdf->GetElement("left_joint_pose3dencoders_pan")->Get<std::string>() << "]\n";
        if (!this->cameraLeft.joint_pose3dencoders_tilt)
            gzerr << "Unable to find left joint pose3dencoders_tilt["
                << _sdf->GetElement("left_joint_pose3dencoders_tilt")->Get<std::string>() << "]\n";
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
                << _sdf->GetElement("right_joint_pose3dencoders_pan")->Get<std::string>() << "]\n";
        if (!this->cameraRight.joint_pose3dencoders_tilt)
            gzerr << "Unable to find right joint pose3dencoders_tilt["
                << _sdf->GetElement("right_joint_pose3dencoders_tilt")->Get<std::string>() << "]\n";
        this->cameraRight.camera_link_pan = this->model->GetLink("camera_column_body_right");
        this->cameraRight.camera_link_tilt = this->model->GetLink("camera_top_body_right");


        //LOAD TORQUE        
        if (_sdf->HasElement("torque"))
            this->torque = _sdf->GetElement("torque")->Get<double>();
        else {
            gzwarn << "No torque value set for the DiffDrive plugin.\n";
            this->torque = 5.0;
        }

        //LOAD POSE3DMOTORS

        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                boost::bind(&Pose3DEncoders::OnUpdate, this));


    }

    void Pose3DEncoders::Init() {



    }

    void Pose3DEncoders::OnUpdate() {

        if (count == 0) {
            count++;

            std::string name = this->model->GetName();

            namePose3DEncoders = std::string("--Ice.Config=" + name + "_pose3dencoders.cfg");
            pthread_t thr_gui;
            pthread_create(&thr_gui, NULL, &pose3dencodersICE, (void*) this);
        }

        //          ----------ENCODERS----------
        //GET pose3dencoders data from left_camera (PAN&TILT)
        this->cameraLeft.encoder.pan = this->cameraLeft.camera_link_pan->GetRelativePose().rot.GetAsEuler().z * 180.0 / M_PI;
        if (this->cameraLeft.encoder.pan > 0) {
            this->cameraLeft.encoder.pan = 180 - this->cameraLeft.encoder.pan;
        }
        if (this->cameraLeft.encoder.pan < 0) {
            this->cameraLeft.encoder.pan = -(180 + this->cameraLeft.encoder.pan);
        }

        //std::cout << this->cameraLeft.encoder.pan << std::endl;
        this->cameraLeft.encoder.tilt = this->cameraLeft.camera_link_tilt->GetRelativePose().rot.GetAsEuler().y * 180.0 / M_PI;
        //std::cout << this->cameraLeft.encoder.tilt << std::endl;


        //GET pose3dencoders data from left_camera (PAN&TILT)

        this->cameraRight.encoder.pan = this->cameraRight.camera_link_pan->GetRelativePose().rot.GetAsEuler().z * 180.0 / M_PI;
        if (this->cameraRight.encoder.pan > 0) {
            this->cameraRight.encoder.pan = 180 - this->cameraRight.encoder.pan;
        }
        if (this->cameraRight.encoder.pan < 0) {
            this->cameraRight.encoder.pan = -(180 + this->cameraRight.encoder.pan);
        }

        //std::cout << this->cameraRight.pan << std::endl;
        this->cameraRight.encoder.tilt = this->cameraRight.camera_link_tilt->GetRelativePose().rot.GetAsEuler().y * 180.0 / M_PI;
        //std::cout << this->cameraRight.encoder.tilt << std::endl;

        // double setPanRight = -50;
        // double setPanLeft = -50;
        // double setTiltRight = -10;
        // double setTiltLeft = -10;
        //          ----------MOTORS----------

        
        
        if (this->cameraLeft.motor.pan >= 0) {
            if (this->cameraLeft.encoder.pan < this->cameraLeft.motor.pan) {
                this->cameraLeft.joint_pose3dencoders_pan->SetParam("vel", 0, -0.1);
                this->cameraLeft.joint_pose3dencoders_pan->SetParam("fmax", 0, this->torque);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->cameraLeft.joint_pose3dencoders_pan->SetParam("vel", 0, 0);
                this->cameraLeft.joint_pose3dencoders_pan->SetParam("fmax", 0, this->torque);
            }
        } else {
            if (this->cameraLeft.encoder.pan > this->cameraLeft.motor.pan) {
                this->cameraLeft.joint_pose3dencoders_pan->SetParam("vel", 0, 0.1);
                this->cameraLeft.joint_pose3dencoders_pan->SetParam("fmax", 0, this->torque);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->cameraLeft.joint_pose3dencoders_pan->SetParam("vel", 0, 0);
                this->cameraLeft.joint_pose3dencoders_pan->SetParam("fmax", 0, this->torque);
            }            
        }
        if (this->cameraRight.motor.pan >= 0) {
            if (this->cameraRight.encoder.pan < this->cameraRight.motor.pan) {
                this->cameraRight.joint_pose3dencoders_pan->SetParam("vel",0, -0.1);
                this->cameraRight.joint_pose3dencoders_pan->SetParam("fmax",0, this->torque);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->cameraRight.joint_pose3dencoders_pan->SetParam("vel",0, 0);
                this->cameraRight.joint_pose3dencoders_pan->SetParam("fmax",0, this->torque);
            }
        } else {
            if (this->cameraRight.encoder.pan > this->cameraRight.motor.pan) {
                this->cameraRight.joint_pose3dencoders_pan->SetParam("vel",0, 0.1);
                this->cameraRight.joint_pose3dencoders_pan->SetParam("fmax",0, this->torque);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->cameraRight.joint_pose3dencoders_pan->SetParam("vel",0, 0);
                this->cameraRight.joint_pose3dencoders_pan->SetParam("fmax",0, this->torque);
            }            
        }

        if (this->cameraLeft.motor.tilt >= 0) {
            if (this->cameraLeft.encoder.tilt < this->cameraLeft.motor.tilt) {
                this->cameraLeft.joint_pose3dencoders_tilt->SetParam("vel", 0, -0.1);
                this->cameraLeft.joint_pose3dencoders_tilt->SetParam("fmax", 0, this->torque);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->cameraLeft.joint_pose3dencoders_tilt->SetParam("vel", 0, 0);
                this->cameraLeft.joint_pose3dencoders_tilt->SetParam("fmax", 0, this->torque);
            }
        } else {
            if (this->cameraLeft.encoder.tilt > this->cameraLeft.motor.tilt) {
                this->cameraLeft.joint_pose3dencoders_tilt->SetParam("vel", 0, 0.1);
                this->cameraLeft.joint_pose3dencoders_tilt->SetParam("fmax",0, this->torque);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->cameraLeft.joint_pose3dencoders_tilt->SetParam("vel", 0, 0);
                this->cameraLeft.joint_pose3dencoders_tilt->SetParam("fmax",0, this->torque);
            }            
        }
        if (this->cameraRight.motor.tilt >= 0) {
            if (this->cameraRight.encoder.tilt < this->cameraRight.motor.tilt) {
                this->cameraRight.joint_pose3dencoders_tilt->SetParam("vel", 0, -0.1);
                this->cameraRight.joint_pose3dencoders_tilt->SetParam("fmax",0, this->torque);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->cameraRight.joint_pose3dencoders_tilt->SetParam("vel", 0, 0);
                this->cameraRight.joint_pose3dencoders_tilt->SetParam("fmax",0, this->torque);
            }
        } else {
            if (this->cameraRight.encoder.tilt > this->cameraRight.motor.tilt) {
                this->cameraRight.joint_pose3dencoders_tilt->SetParam("vel", 0, 0.1);
                this->cameraRight.joint_pose3dencoders_tilt->SetParam("fmax",0, this->torque);
                //std::cout << "AQUI" << std::endl;
            } else {
                this->cameraRight.joint_pose3dencoders_tilt->SetParam("vel", 0, 0);
                this->cameraRight.joint_pose3dencoders_tilt->SetParam("fmax",0, this->torque);
            }            
        }        
        




    }

    class Pose3DEncodersI : virtual public jderobot::Pose3DEncoders {
    public:

        Pose3DEncodersI(gazebo::Pose3DEncoders* pose) : pose3DEncodersData(new jderobot::Pose3DEncodersData()) {

            this->pose = pose;

        }

        virtual ~Pose3DEncodersI() {

        }

        virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData(const Ice::Current&) {

            pthread_mutex_lock(&pose->mutex);
            pose3DEncodersData->x = 0;
            pose3DEncodersData->y = 0;
            pose3DEncodersData->z = 0;
            pose3DEncodersData->pan = pose->cameraLeft.encoder.pan;
            pose3DEncodersData->tilt = pose->cameraLeft.encoder.tilt;
            pose3DEncodersData->clock = 0;
            pose3DEncodersData->roll = 0;
            pthread_mutex_unlock(&pose->mutex);

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

            pthread_mutex_lock(&pose->mutex);
            pose3DEncodersData->x = 0;
            pose3DEncodersData->y = 0;
            pose3DEncodersData->z = 0;
            pose3DEncodersData->pan = pose->cameraRight.encoder.pan;
            pose3DEncodersData->tilt = pose->cameraRight.encoder.tilt;
            pose3DEncodersData->clock = 0;
            pose3DEncodersData->roll = 0;
            pthread_mutex_unlock(&pose->mutex);

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
        
        virtual ~Pose3DMotorsI(){
            
        }
        
        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData(const Ice::Current&){
            
            pthread_mutex_lock(&pose->mutex);
            pose3DMotorsData->x = 0;
            pose3DMotorsData->y = 0;
            pose3DMotorsData->z = 0;
            pose3DMotorsData->pan = pose->cameraLeft.encoder.pan;
            pose3DMotorsData->tilt = pose->cameraLeft.encoder.tilt;
            pose3DMotorsData->roll = 0;
            pose3DMotorsData->panSpeed = 0;
            pose3DMotorsData->tiltSpeed = 0;
            pthread_mutex_unlock(&pose->mutex);

            return pose3DMotorsData;
            
            
        }
        
        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const Ice::Current&) {
            pthread_mutex_lock(&pose->mutex);
            pose3DMotorsParams->maxPan = 0;
            pose3DMotorsParams->minPan = 0;
            pose3DMotorsParams->maxTilt = 0;
            pose3DMotorsParams->minTilt = 0;
            pose3DMotorsParams->maxPanSpeed = 0;
            pose3DMotorsParams->maxTiltSpeed = 0;
            pthread_mutex_unlock(&pose->mutex);

            return pose3DMotorsParams;
        }
        
        virtual Ice::Int setPose3DMotorsData(const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current&) {
            pthread_mutex_lock(&pose->mutex);
            pose->cameraLeft.motor.x = data->x;
            pose->cameraLeft.motor.y = data->y;
            pose->cameraLeft.motor.z = data->z;
            pose->cameraLeft.motor.pan = data->pan;
            pose->cameraLeft.motor.tilt = data->tilt;
            pose->cameraLeft.motor.roll = data->roll;
            pose->cameraLeft.motor.panSpeed = data->panSpeed;
            pose->cameraLeft.motor.tiltSpeed = data->tiltSpeed;            
            pthread_mutex_unlock(&pose->mutex);

            return 0; // Remove return warning
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
        
        virtual ~Pose3DMotorsII(){
            
        }
        
        virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData(const Ice::Current&){
            
            pthread_mutex_lock(&pose->mutex);
            pose3DMotorsData->x = 0;
            pose3DMotorsData->y = 0;
            pose3DMotorsData->z = 0;
            pose3DMotorsData->pan = pose->cameraRight.encoder.pan;
            pose3DMotorsData->tilt = pose->cameraRight.encoder.tilt;
            pose3DMotorsData->roll = 0;
            pose3DMotorsData->panSpeed = 0;
            pose3DMotorsData->tiltSpeed = 0;
            pthread_mutex_unlock(&pose->mutex);

            return pose3DMotorsData;
            
            
        }
        
        virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const Ice::Current&) {
            pthread_mutex_lock(&pose->mutex);
            pose3DMotorsParams->maxPan = 0;
            pose3DMotorsParams->minPan = 0;
            pose3DMotorsParams->maxTilt = 0;
            pose3DMotorsParams->minTilt = 0;
            pose3DMotorsParams->maxPanSpeed = 0;
            pose3DMotorsParams->maxTiltSpeed = 0;
            pthread_mutex_unlock(&pose->mutex);

            return pose3DMotorsParams;
        }
        
        virtual Ice::Int setPose3DMotorsData(const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current&) {
            pthread_mutex_lock(&pose->mutex);
            pose->cameraRight.motor.x = data->x;
            pose->cameraRight.motor.y = data->y;
            pose->cameraRight.motor.z = data->z;
            pose->cameraRight.motor.pan = data->pan;
            pose->cameraRight.motor.tilt = data->tilt;
            pose->cameraRight.motor.roll = data->roll;
            pose->cameraRight.motor.panSpeed = data->panSpeed;
            pose->cameraRight.motor.tiltSpeed = data->tiltSpeed;            
            pthread_mutex_unlock(&pose->mutex);

            return 0; // Remove return warning
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
            std::string Endpoints1 = prop->getProperty("Pose3DEncoders1.Endpoints");
            std::cout << "Pose3DEncoders1 Endpoints > " << Endpoints1 << std::endl;
            std::string Endpoints2 = prop->getProperty("Pose3DEncoders2.Endpoints");
            std::cout << "Pose3DEncoders2 Endpoints > " << Endpoints2 << std::endl;
            std::string Endpoints3 = prop->getProperty("Pose3DMotors1.Endpoints");
            std::cout << "Pose3DMotors1 Endpoints > " << Endpoints3 << std::endl;
            std::string Endpoints4 = prop->getProperty("Pose3DMotors2.Endpoints");
            std::cout << "Pose3DMotors2 Endpoints > " << Endpoints4 << std::endl;


            Ice::ObjectAdapterPtr adapter1 =
                    ic->createObjectAdapterWithEndpoints("Pose3DEncoders1", Endpoints1);
            Ice::ObjectAdapterPtr adapter2 =
                    ic->createObjectAdapterWithEndpoints("Pose3DEncoders2", Endpoints2);
            Ice::ObjectAdapterPtr adapter3 =
                    ic->createObjectAdapterWithEndpoints("Pose3DMotors1", Endpoints3);
            Ice::ObjectAdapterPtr adapter4 =
                    ic->createObjectAdapterWithEndpoints("Pose3DMotors2", Endpoints4);



            Ice::ObjectPtr object1 = new Pose3DEncodersI(base);
            Ice::ObjectPtr object2 = new Pose3DEncodersII(base);
            Ice::ObjectPtr object3 = new Pose3DMotorsI(base);
            Ice::ObjectPtr object4 = new Pose3DMotorsII(base);



            adapter1->add(object1, ic->stringToIdentity("Pose3DEncoders1"));
            adapter2->add(object2, ic->stringToIdentity("Pose3DEncoders2"));
            adapter3->add(object3, ic->stringToIdentity("Pose3DMotors1"));
            adapter4->add(object4, ic->stringToIdentity("Pose3DMotors2"));

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
        
        return NULL;
    }


}
