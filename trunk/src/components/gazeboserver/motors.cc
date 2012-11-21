#include "motors.h"

enum {
    RIGHT,
    LEFT
};



namespace gazebo {

    //GLOBAL DATA
    

    void *motorsICE(void* v);

    GZ_REGISTER_MODEL_PLUGIN(Motors)

    Motors::Motors() {
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_init(&mutexMotor, NULL);
        count = 0;
        std::cout << "constructor Motors" << std::endl;
        this->wheelSpeed[LEFT] = this->wheelSpeed[RIGHT] = 0;
    }

    void Motors::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {
        // Get a pointer to the model
        this->model = _model;

        this->node = transport::NodePtr(new transport::Node());
        this->node->Init(this->model->GetWorld()->GetName());

        //this->velSub = this->node->Subscribe(std::string("~/") + this->model->GetName() + "/vel_cmd", &Motors::OnVelMsg, this);
        if (!_sdf->HasElement("left_joint"))
            gzerr << "Motors plugin missing <left_joint> element\n";
        if (!_sdf->HasElement("right_joint"))
            gzerr << "DiffDrive plugin missing <right_joint> element\n";

        this->leftJoint = _model->GetJoint(
                _sdf->GetElement("left_joint")->GetValueString());
        this->rightJoint = _model->GetJoint(
                _sdf->GetElement("right_joint")->GetValueString());

        if (_sdf->HasElement("torque"))
            this->torque = _sdf->GetElement("torque")->GetValueDouble();
        else {
            gzwarn << "No torque value set for the DiffDrive plugin.\n";
            this->torque = 5.0;
        }
        if (!this->leftJoint)
            gzerr << "Unable to find left joint["
                << _sdf->GetElement("left_joint")->GetValueString() << "]\n";
        if (!this->rightJoint)
            gzerr << "Unable to find right joint["
                << _sdf->GetElement("right_joint")->GetValueString() << "]\n";

        this->updateConnection = event::Events::ConnectWorldUpdateStart(
                boost::bind(&Motors::OnUpdate, this));
    }

    void Motors::Init() {
        this->wheelSeparation = this->leftJoint->GetAnchor(0).Distance(this->rightJoint->GetAnchor(0));
        std::cout << "Wheel Separation:" << this->wheelSeparation << std::endl;
        physics::EntityPtr parent = boost::shared_dynamic_cast<physics::Entity > (this->leftJoint->GetChild());

        math::Box bb = parent->GetBoundingBox();

        this->wheelRadius = bb.GetSize().GetMax() * 0.5;
        std::cout << "Wheel Diameter:" << this->wheelRadius * 2 << std::endl;
    }

    void Motors::OnUpdate() {

                gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;             
        cycle = 50;
        
        if (count == 0) {
            count++;
            std::string name = this->model->GetName();
            std::cout << "GetName() " << name << std::endl;
            nameMotors = std::string("--Ice.Config=" + name +"Motors.cfg");
            pthread_t thr_gui;
            pthread_create(&thr_gui, NULL, &motorsICE, (void*) this);
        }

        double vr, va; //vr -> velocidad lineal; va -> velocidad angular

        pthread_mutex_lock(&mutex);
        vr = robotMotors.v; //
        va = robotMotors.w; //
        pthread_mutex_unlock(&mutex);

        
        
        this->wheelSpeed[LEFT] = vr + va * this->wheelSeparation / 2.0;
        this->wheelSpeed[RIGHT] = vr - va * this->wheelSeparation / 2.0;

        double leftVelDesired = (this->wheelSpeed[LEFT] / this->wheelRadius);
        double rightVelDesired = (this->wheelSpeed[RIGHT] / this->wheelRadius);

        this->leftJoint->SetVelocity(0, leftVelDesired/2);
        this->rightJoint->SetVelocity(0, rightVelDesired/2);
        
        
        this->leftJoint->SetMaxForce(0, 1.4); //this->leftJoint->SetMaxForce(0, this->torque);
        this->rightJoint->SetMaxForce(0, 1.4); //this->rightJoint->SetMaxForce(0, this->torque);
        //this->leftJoint->SetForce(0,1.5);
        //        this->rightJoint->SetForce(0,1.5);

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;
        
        diff = (totalb - totala) / 1000;
        diff = cycle - diff;
        
        if (diff < 10)
            diff = 10;
        
        //usleep(diff*1000);
        sleep(diff/1000);        
        
    }

    class MotorsI : virtual public jderobot::Motors {
    public:

        MotorsI(gazebo::Motors* pose) {
            this->pose = pose;
        }

        virtual ~MotorsI() {
        };

        virtual float getV(const Ice::Current&) {

            float v_return;
            pthread_mutex_lock(&pose->mutex);
            v_return = pose->robotMotors.v;
            pthread_mutex_unlock(&pose->mutex);
            return v_return;
        };

        virtual float getW(const Ice::Current&) {
            float w_return;
            pthread_mutex_lock(&pose->mutex);
            w_return = pose->robotMotors.w;
            pthread_mutex_unlock(&pose->mutex);
            return w_return;
        };

        virtual float getL(const Ice::Current&) {
            return 0.;
        };

        virtual Ice::Int setV(Ice::Float v, const Ice::Current&) {
            pthread_mutex_lock(&pose->mutex);
            pose->robotMotors.v = v/1000;
            pthread_mutex_unlock(&pose->mutex);
            return 0;
        };

        virtual Ice::Int setW(Ice::Float _w, const Ice::Current&) {
            pthread_mutex_lock(&pose->mutex);
            pose->robotMotors.w = -_w/10;
            pthread_mutex_unlock(&pose->mutex);
            return 0;
        };

        virtual Ice::Int setL(Ice::Float l, const Ice::Current&) {
            return 0;
        };

    public:
        gazebo::Motors* pose;
    }; // end class MotorsI


    void *motorsICE(void* v) {

        gazebo::Motors* base = (gazebo::Motors*)v;

        Ice::CommunicatorPtr ic;
        int argc = 1;
        char* name = (char*) base->nameMotors.c_str();
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {

            ic = Ice::initialize(argc, argv);


            prop = ic->getProperties();
            std::string Endpoints = prop->getProperty("Motors.Endpoints");
            std::cout << "Motors Endpoints > " << Endpoints << std::endl;

            Ice::ObjectAdapterPtr adapter =
                    ic->createObjectAdapterWithEndpoints("Motors", Endpoints);
            Ice::ObjectPtr object = new MotorsI(base);
            
            adapter->add(object, ic->stringToIdentity("Motors"));
            
            adapter->activate();
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
