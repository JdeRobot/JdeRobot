#include "encoders.h"

namespace gazebo {

    void *encodersICE(void* v);

    GZ_REGISTER_MODEL_PLUGIN(Encoders)

    Encoders::Encoders() {
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_init(&mutexEncoders, NULL);
        count = 0;
        std::cout << "constructor Encoders" << std::endl;
    }

    void Encoders::Load(physics::ModelPtr _parent, sdf::ElementPtr) {

        model = _parent;
        this->updateConnection = event::Events::ConnectWorldUpdateStart(
                boost::bind(&Encoders::OnUpdate, this));
    }

    void Encoders::OnUpdate() {

        if (count == 0) {
            count++;
            std::string name = this->model->GetName();
            std::cout << "GetName() encoders" << name << std::endl;
            nameEncoders = std::string("--Ice.Config=Encoders.cfg");
            pthread_t thr_gui;
            pthread_create(&thr_gui, NULL, &encodersICE, (void*) this);
        }

        position = model->GetWorldPose();
        this->initial_q = position.rot;

        math::Vector3 initial_rpy = initial_q.GetAsEuler();

        double degrees = initial_rpy.z * 180.0 / M_PI;
        if (degrees < 0) {
            degrees = 360 + degrees;
        }


        pthread_mutex_lock(&mutex);
        robotEncoders.x = position.pos.x;
        robotEncoders.y = position.pos.y;
        robotEncoders.theta = degrees;
        pthread_mutex_unlock(&mutex);

    }

    class EncodersI : virtual public jderobot::Encoders {
    public:

        EncodersI(gazebo::Encoders* pose) : encodersData(new jderobot::EncodersData()) {
            this->pose = pose;
        }

        virtual ~EncodersI() {
        };

        virtual jderobot::EncodersDataPtr getEncodersData(const Ice::Current&) {
            pthread_mutex_lock(&pose->mutex);

            std::cout << "theta: " << pose->robotEncoders.theta << std::endl;

            encodersData->robotx = pose->robotEncoders.x * 1000;
            encodersData->roboty = pose->robotEncoders.y * 1000;
            encodersData->robottheta = pose->robotEncoders.theta;
            encodersData->robotcos = cos(pose->robotEncoders.theta);
            encodersData->robotsin = sin(pose->robotEncoders.theta);

            pthread_mutex_unlock(&pose->mutex);

            return encodersData;
        };

    public:
        gazebo::Encoders* pose;
    private:
        jderobot::EncodersDataPtr encodersData;
    };

    void *encodersICE(void* v) {
        gazebo::Encoders* base = (gazebo::Encoders*)v;
        char* name = (char*) base->nameEncoders.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {

            ic = Ice::initialize(argc, argv);


            prop = ic->getProperties();
            std::string Endpoints = prop->getProperty("Encoders.Endpoints");
            std::cout << "Encoders Endpoints > " << Endpoints << std::endl;

            Ice::ObjectAdapterPtr adapter =
                    ic->createObjectAdapterWithEndpoints("Encoders", Endpoints);
            Ice::ObjectPtr object = new EncodersI(base);

            adapter->add(object, ic->stringToIdentity("Encoders"));

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