
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
                _sdf->GetElement("left_joint")->Get<std::string>());
        this->rightJoint = _model->GetJoint(
                _sdf->GetElement("right_joint")->Get<std::string>());

        if (_sdf->HasElement("torque"))
            this->torque = _sdf->GetElement("torque")->Get<double>();
        else {
            gzwarn << "No torque value set for the DiffDrive plugin.\n";
            this->torque = 5.0;
        }
        if (!this->leftJoint)
            gzerr << "Unable to find left joint["
                << _sdf->GetElement("left_joint")->Get<std::string>() << "]\n";
        if (!this->rightJoint)
            gzerr << "Unable to find right joint["
                << _sdf->GetElement("right_joint")->Get<std::string>() << "]\n";



        // Listen to the update event. This event is broadcast every
        // simulation iteration.
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                boost::bind(&Motors::OnUpdate, this));
    }

    // Called by the world update start event

    void Motors::Init() {
        this->wheelSeparation = this->leftJoint->GetAnchor(0).Distance(this->rightJoint->GetAnchor(0));
        std::cout << "Wheel Separation:" << this->wheelSeparation << std::endl;
        physics::EntityPtr parent = boost::dynamic_pointer_cast<physics::Entity > (this->leftJoint->GetChild());

        math::Box bb = parent->GetBoundingBox();

        this->wheelRadius = bb.GetSize().GetMax() * 0.5;
        std::cout << "Wheel Diameter:" << this->wheelRadius * 2 << std::endl;
    }

    /*
    void Motors::OnVelMsg(ConstPosePtr& _msg) {

        double vr, va;

        vr = _msg->position().x() + 5;
        std::cout << "vr:" << vr << std::endl;
        va = msgs::Convert(_msg->orientation()).GetAsEuler().z + 3;

        this->wheelSpeed[LEFT] = vr + va * this->wheelSeparation / 2.0;
        this->wheelSpeed[RIGHT] = vr - va * this->wheelSeparation / 2.0;



    }
    */
    
    void Motors::OnUpdate() {

        if (count == 0) {
        
	        robotMotors.v = 0;
	        robotMotors.w = 0;
        
            count++;
            std::string name = this->model->GetName();
            std::cout << "motors name " << name << std::endl;
            nameMotors = std::string("--Ice.Config=" + name +"Motors.cfg");
            pthread_t thr_gui;
            pthread_create(&thr_gui, NULL, &motorsICE, (void*) this);
        }

        double vr, va; //vr -> velocidad lineal; va -> velocidad angular

        pthread_mutex_lock(&mutex);

        vr = robotMotors.v;

        va = robotMotors.w;

        pthread_mutex_unlock(&mutex);


        this->wheelSpeed[LEFT] = vr + va * this->wheelSeparation / 2.0;
        this->wheelSpeed[RIGHT] = vr - va * this->wheelSeparation / 2.0;

        double leftVelDesired = (this->wheelSpeed[LEFT] / this->wheelRadius);
        double rightVelDesired = (this->wheelSpeed[RIGHT] / this->wheelRadius);
/*
		std::cout << "leftVelDesired " << leftVelDesired << std::endl;
		std::cout << "rightVelDesired " << rightVelDesired << std::endl;
		std::cout << "torque " << torque << std::endl;
*/
        this->leftJoint->SetParam("vel", 0, leftVelDesired);
        this->rightJoint->SetParam("vel", 0, rightVelDesired);

        this->leftJoint->SetParam("fmax", 0, this->torque);
        this->rightJoint->SetParam("fmax", 0, this->torque);
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
            pthread_mutex_lock(&pose->mutexMotor);
            //v_return = pose->vel;
            v_return = pose->robotMotors.v;
            pthread_mutex_unlock(&pose->mutexMotor);
            return v_return;
        };

        virtual float getW(const Ice::Current&) {
            float w_return;
            pthread_mutex_lock(&pose->mutexMotor);
            //w_return = pose->w;
            w_return = pose->robotMotors.w;
            pthread_mutex_unlock(&pose->mutexMotor);
            return w_return;
        };

        virtual float getL(const Ice::Current&) {
            return 0.;
        };

        virtual Ice::Int setV(Ice::Float v, const Ice::Current&) {
            pthread_mutex_lock(&pose->mutexMotor);
            //pose->vel = v;
            pose->robotMotors.v = v;
            pthread_mutex_unlock(&pose->mutexMotor);
            return 0;
        };

        virtual Ice::Int setW(Ice::Float _w, const Ice::Current&) {
            pthread_mutex_lock(&pose->mutexMotor);
            //pose->w = _w;
            pose->robotMotors.w = -_w;
            pthread_mutex_unlock(&pose->mutexMotor);
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
std::cout << argv[0] << " loaded" << std::endl;

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

        return NULL;
    }
}
