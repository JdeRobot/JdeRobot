#include "motors.h"

/*
enum {
    FRONTRIGHT,
    FRONTLEFT
};
*/

namespace gazebo
{
    void *motorsICE(void* v);


    Motors::Motors(){
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_init(&mutexMotor, NULL);
        count = 0;
        std::cout << "constructor motors" << std::endl;
        //this->motorspeed[FRONTLEFT] = this->motorspeed[FRONTRIGHT] =  0;
        this->motorsteer = 0;
        this->motorspeed = 0;
    }
    void Motors::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
    {
        this->model = _model;
        this->node = transport::NodePtr(new transport::Node());
        this->node->Init(this->model->GetWorld()->GetName());

        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&Motors::OnUpdate, this));
    }

    void Motors::Init() {

    }


    // Called by the world update start event
    void Motors::PID()
    {

    }

    void Motors::OnUpdate()
    {
        float v=0, w=0;
        std::time_t now = std::time(nullptr);
        if (count == 0) {

            robotMotors.v = 0;
            robotMotors.w = 0;
            robotMotors.wheelMin= -0.52359;
            robotMotors.wheelMax= 0.52359;
            robotMotors.targetRightSteerPos=robotMotors.targetLeftSteerPos=0;

            count++;
            std::string name = this->model->GetName();
            //std::cout << "Model name " << name << std::endl;

            nameMotors = std::string("--Ice.Config=" + name +"Motors.cfg");
            //nameMotors = std::string("--Ice.Config=holoCarMotors.cfg");
            pthread_t thr_gui;
            pthread_create(&thr_gui, NULL, &motorsICE, (void*) this);

        }

        float z = model->GetRelativeLinearVel().z;
        pthread_mutex_lock(&mutexMotor);
        if ((now - robotMotors.lastVtime) < 2){
            v = robotMotors.v;
        }
        if ((now - robotMotors.lastWtime) < 2){
            w = robotMotors.w;
        }
        pthread_mutex_unlock(&mutexMotor);
        math::Vector3 vel(v,0,z);

        math::Quaternion rot = model->GetWorldPose().rot;
        vel = rot.GetAsMatrix3()*vel;

        this->model->SetLinearVel(vel);
        this->model->SetAngularVel(math::Vector3(0,0,w));
    }

    class MotorsI : virtual public jderobot::Motors {
        public:

            MotorsI(gazebo::Motors* base) {
                this->base = base;
            }

            virtual ~MotorsI() {
            };

            virtual float getV(const Ice::Current&) {

                float v_return;
                pthread_mutex_lock(&base->mutexMotor);
                //v_return = base->vel;
                v_return = base->robotMotors.v;
                pthread_mutex_unlock(&base->mutexMotor);
                return v_return;
            };

            virtual float getW(const Ice::Current&) {
                float w_return;
                pthread_mutex_lock(&base->mutexMotor);
                //w_return = base->w;
                w_return = base->robotMotors.w;
                pthread_mutex_unlock(&base->mutexMotor);
                return w_return;
            };

            virtual float getL(const Ice::Current&) {
                return 0.;
            };

            virtual Ice::Int setV(Ice::Float v, const Ice::Current&) {
                pthread_mutex_lock(&base->mutexMotor);
                //base->vel = v;
                base->robotMotors.v = v;
                base->robotMotors.lastVtime = std::time(nullptr);
                pthread_mutex_unlock(&base->mutexMotor);
                return 0;
            };

            virtual Ice::Int setW(Ice::Float _w, const Ice::Current&) {
                pthread_mutex_lock(&base->mutexMotor);
                //base->w = _w;
                base->robotMotors.w = _w;
                base->robotMotors.lastWtime = std::time(nullptr);
                pthread_mutex_unlock(&base->mutexMotor);
                return 0;
            };

            virtual Ice::Int setL(Ice::Float l, const Ice::Current&) {
                return 0;
            };

        public:
            gazebo::Motors* base;
        }; // end class MotorsI


    void *motorsICE(void* v) {

        gazebo::Motors* base = (gazebo::Motors*)v;

        Ice::CommunicatorPtr ic;
        int argc = 1;
        char* name = (char*) base->nameMotors.c_str();
        std::cout << name << "\n";
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {

            ic = EasyIce::initialize(argc, argv);

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
    // Register this plugin with the simulator
    GZ_REGISTER_MODEL_PLUGIN(Motors)

}
