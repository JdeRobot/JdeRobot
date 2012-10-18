#include "ptencoders.h"

namespace gazebo {

    //void *ptencodersICE(void* v);

    GZ_REGISTER_MODEL_PLUGIN(PTEncoders)

    PTEncoders::PTEncoders() {
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_init(&mutexPTEncoders, NULL);
        count = 0;
        std::cout << "constructor PTEncoders" << std::endl;
    }

    void PTEncoders::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {

        this->model = _model;

        // LOAD CAMERA LEFT        
        if (!_sdf->HasElement("left_joint_ptencoders"))
            gzerr << "Motors plugin missing <left_joint_ptencoders> element\n";

        this->cameraLeft.joint_ptencoders = _model->GetJoint(
                _sdf->GetElement("left_joint_ptencoders")->GetValueString());

        if (!this->cameraLeft.joint_ptencoders)
            gzerr << "Unable to find left joint ptencoders["
                << _sdf->GetElement("left_joint_ptencoders")->GetValueString() << "]\n";


        //LOAD CAMERA RIGHT
        if (!_sdf->HasElement("right_joint_ptencoders"))
            gzerr << "DiffDrive plugin missing <right_joint_ptencoders> element\n";

        this->cameraRight.joint_ptencoders = _model->GetJoint(
                _sdf->GetElement("right_joint_ptencoders")->GetValueString());

        if (!this->cameraRight.joint_ptencoders)
            gzerr << "Unable to find right joint ptencoders["
                << _sdf->GetElement("right_joint_ptencoders")->GetValueString() << "]\n";

        //LOAD TORQUE        
        if (_sdf->HasElement("torque"))
            this->torque = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the DiffDrive plugin.\n";
            this->torque = 5.0;
        }


        this->updateConnection = event::Events::ConnectWorldUpdateStart(
                boost::bind(&PTEncoders::OnUpdate, this));


    }

    void PTEncoders::Init() {



    }

    void PTEncoders::OnUpdate() {

        if (count == 0) {
            count++;
            std::string name = this->model->GetName();
            namePTEncoders = std::string("--Ice.Config="+ name + ".cfg");
            pthread_t thr_gui;
            //pthread_create(&thr_gui, NULL, &ptencodersICE, (void*) this);
        }


        //GET PTEncoders data from left_camera
        this->cameraLeft.camera_link = this->cameraLeft.joint_ptencoders->GetChild();
        this->cameraLeft.position = this->cameraLeft.camera_link->GetRelativePose();
        this->cameraLeft.initial_q = this->cameraLeft.position.rot;
        this->cameraLeft.initial_rpy = this->cameraLeft.initial_q.GetAsEuler();
        this->cameraLeft.degreesX = this->cameraLeft.initial_rpy.x * 180.0 / M_PI;
        this->cameraLeft.degreesZ = this->cameraLeft.initial_rpy.z * 180.0 / M_PI;
        if (this->cameraLeft.degreesX < 0) {
            this->cameraLeft.degreesX = 360 + this->cameraLeft.degreesX;
        }
        if (this->cameraLeft.degreesZ < 0) {
            this->cameraLeft.degreesZ = 360 + this->cameraLeft.degreesZ;
        }
        //std::cout << this->cameraLeft.degreesX << std::endl;


        //GET PTEncoders data from right_camera
        this->cameraRight.camera_link = this->cameraRight.joint_ptencoders->GetChild();
        this->cameraRight.position = this->cameraRight.camera_link->GetRelativePose();
        this->cameraRight.initial_q = this->cameraRight.position.rot;
        this->cameraRight.initial_rpy = this->cameraRight.initial_q.GetAsEuler();
        this->cameraRight.degreesX = this->cameraRight.initial_rpy.x * 180.0 / M_PI;
        this->cameraRight.degreesZ = this->cameraRight.initial_rpy.z * 180.0 / M_PI;
        if (this->cameraRight.degreesX < 0) {
            this->cameraRight.degreesX = 360 + this->cameraRight.degreesX;
        }
        if (this->cameraRight.degreesZ < 0) {
            this->cameraRight.degreesZ = 360 + this->cameraRight.degreesZ;
        }

        /*      PTMOTORS
        std::cout << gradesZ << std::endl;
        
        if(gradesX_left < 90){
                this->left_joint_ptencoders->SetVelocity(0, 0.01); //Set move X -> vertically
                this->left_joint_ptencoders->SetMaxForce(0, this->torque);            
        }
        else{
                this->left_joint_ptencoders->SetVelocity(0, -0); //Set move X -> vertically
                this->left_joint_ptencoders->SetMaxForce(0, this->torque);            
            
        }
        this->left_joint_ptencoders->SetVelocity(0, 0.1); //Set move X -> vertically
        this->left_joint_ptencoders->SetMaxForce(0, this->torque);
        this->left_joint_ptencoders->SetVelocity(1, 0.1); //Set move Z -> horizontally
        
        this->left_joint_ptencoders->SetMaxForce(1, this->torque);
         */


    }



}
