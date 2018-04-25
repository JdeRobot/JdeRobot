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

#ifdef USE_REAL_WHEELS
        if (!_sdf->HasElement("front_right_joint"))
            gzerr << "motors plugin missing <front_right_joint> element\n";
        if (!_sdf->HasElement("front_left_joint"))
            gzerr << "motors plugin missing <front_left_joint> element\n";
        if (!_sdf->HasElement("front_right_steering_joint"))
            gzerr << "motors plugin missing <front_right_steering_joint> element\n";
        if (!_sdf->HasElement("front_left_steering_joint"))
            gzerr << "motors plugin missing <front_left_steering_joint> element\n";

        this->frontLeftJoint = _model->GetJoint(
            _sdf->GetElement("front_left_joint")->Get<std::string>());
        this->frontRightJoint = _model->GetJoint(
            _sdf->GetElement("front_right_joint")->Get<std::string>());
        this->steerLeftJoint = _model->GetJoint(
            _sdf->GetElement("front_right_steering_joint")->Get<std::string>());
        this->steerRightJoint = _model->GetJoint(
            _sdf->GetElement("front_left_steering_joint")->Get<std::string>());
/*
        if (_sdf->HasElement(" "))
            this->torque = _sdf->GetElement("torque")->Get<double>();
        else {
            gzwarn << "No torque value set for the motors plugin.\n";
            this->torque = 5.0;
        }
*/
        if (!this->frontLeftJoint)
            gzerr << "Unable to find front left joint["
                << _sdf->GetElement("front_left_joint")->Get<std::string>() << "]\n";
        if (!this->frontRightJoint)
            gzerr << "Unable to find front right joint["
                << _sdf->GetElement("front_right_joint")->Get<std::string>() << "]\n";
        if (!this->steerLeftJoint)
            gzerr << "Unable to find steering left joint["
                << _sdf->GetElement("front_left_steering_joint")->Get<std::string>() << "]\n";
        if (!this->steerRightJoint)
            gzerr << "Unable to find steering right joint["
                << _sdf->GetElement("front_right_steering_joint")->Get<std::string>() << "]\n";
#endif

        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                    boost::bind(&Motors::OnUpdate, this));
    }

    void Motors::Init() {
        //this->frontmotorseparation = this->frontLeftJoint->GetAnchor(0).Distance(this->frontRightJoint->GetAnchor(0));
        //std::cout << "Motors Separation:" << this->frontmotorseparation << std::endl;
        //physics::EntityPtr parent = boost::dynamic_pointer_cast<physics::Entity > (this->frontLeftJoint->GetChild());

        //math::Box bb = parent->GetBoundingBox();

        //this->frontmotorsRadius = bb.GetSize().GetMax() * 0.5;
        //std::cout << "motors Diameter:" << this->frontmotorsRadius * 2 << std::endl;
    }


    // Called by the world update start event
    void Motors::PID()
    {

    }

    void Motors::OnUpdate()
    {

        if (count == 0) {

            robotMotors.v = 0;
            robotMotors.w = 0;
            robotMotors.wheelMin= -0.52359;
            robotMotors.wheelMax= 0.52359;
            robotMotors.targetRightSteerPos=robotMotors.targetLeftSteerPos=0;
#ifdef USE_REAL_WHEELS

            this->steerRightJoint->SetHighStop(0,robotMotors.wheelMax);
            this->steerRightJoint->SetLowStop(0,robotMotors.wheelMin);
            this->steerLeftJoint->SetHighStop(0,robotMotors.wheelMax);
            this->steerLeftJoint->SetLowStop(0,robotMotors.wheelMin);
#endif
            count++;
            //std::string name = this->model->GetName();
            //std::cout << "Model name " << name << std::endl;

            //nameMotors = std::string("--Ice.Config=" + name +"motors.cfg");
            nameMotors = std::string("--Ice.Config=f1Motors.cfg");
            pthread_t thr_gui;
            pthread_create(&thr_gui, NULL, &motorsICE, (void*) this);

        }

#ifdef USE_REAL_WHEELS
        pthread_mutex_lock(&mutex);

        robotMotors.targetRightSteerPos=robotMotors.w*0.58-this->steerRightJoint->GetAngle(0).Radian();
        robotMotors.targetLeftSteerPos=robotMotors.w*0.58-this->steerLeftJoint->GetAngle(0).Radian();

        this->motorspeed = robotMotors.v * 25;
        pthread_mutex_unlock(&mutex);

        //this->motorspeed = vl;




        //double leftVelDesired = (this->motorspeed[FRONTLEFT]);
        //double rightVelDesired = (this->motorspeed[FRONTRIGHT]);

        /*
        std::cout << "leftVelDesired " << leftVelDesired << std::endl;
        std::cout << "rightVelDesired " << rightVelDesired << std::endl;
        std::cout << "torque " << torque << std::endl;
        */

        //DEPRECATED
        //this->frontLeftJoint->SetMaxForce(0,torque);
        //this->frontRightJoint->SetMaxForce(0,torque);
        this->frontLeftJoint->SetVelocity(0,motorspeed);
        this->frontRightJoint->SetVelocity(0,motorspeed);
        //std::cout << "Angle:" << this->steerRightJoint->GetAngle(0) << std::endl;
        this->steerRightJoint->SetForce(0,580*robotMotors.targetRightSteerPos);
        this->steerLeftJoint->SetForce(0,580*robotMotors.targetLeftSteerPos);
#else
        float z = model->GetRelativeLinearVel().z;
        math::Vector3 vel(0,-robotMotors.v/1.0,z);

        math::Quaternion rot = model->GetWorldPose().rot;
        vel = rot.GetAsMatrix3()*vel;

        this->model->SetLinearVel(vel);
        this->model->SetAngularVel(math::Vector3(0,0,robotMotors.w));
#endif

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
                pthread_mutex_unlock(&base->mutexMotor);
                return 0;
            };

            virtual Ice::Int setW(Ice::Float _w, const Ice::Current&) {
                pthread_mutex_lock(&base->mutexMotor);
                //base->w = _w;
                base->robotMotors.w = _w;
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
