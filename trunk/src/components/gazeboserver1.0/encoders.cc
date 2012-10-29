#include "encoders.h"

namespace gazebo {
    
    void *encodersICE(void* v);
    int32_t secs;
    GZ_REGISTER_MODEL_PLUGIN(Encoders)

    Encoders::Encoders() {
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_init(&mutexEncoders, NULL);
        count = 0;
        common::Time time;
        time = time.GetWallTime();
        secs = time.sec;
        std::cout << "constructor Encoders" << std::endl;
    }

    void Encoders::Load(physics::ModelPtr _parent, sdf::ElementPtr) {

        model = _parent;
        this->updateConnection = event::Events::ConnectWorldUpdateStart(
                boost::bind(&Encoders::OnUpdate, this));
    }

    void Encoders::OnUpdate() {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;             
        cycle = 50;
        
        
        if (count == 0) {
            count++;
            std::string name = this->model->GetName();
            std::cout << "GetName() encoders: " << name << std::endl;
            nameEncoders = std::string("--Ice.Config=" + name + "Encoders.cfg");
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
        //std::cout << "x: " << robotEncoders.x << std::endl;
        //std::cout << "y: " << robotEncoders.y << std::endl;
        pthread_mutex_unlock(&mutex);
        
        common::Time time;
        
        //std::cout << time.GetWallTime() << std::endl;
        
        time = time.GetWallTime();
        //std::cout << time.sec - secs << std::endl;
        //std::cout << "Vel: " << robotEncoders.x/(time.sec - secs) << "m/s" << std::endl;
        if((time.sec - secs) % 10 == 0){
            //std::cout << "x: " << robotEncoders.x << std::endl;
        }
        
        
        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;
        
        diff = (totalb - totala) / 1000;
        diff = cycle - diff;
        
        if (diff < 10)
            diff = 10;
        
        sleep(diff/1000);        
        //MSleep(diff*1000);

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

            //std::cout << "theta: " << pose->robotEncoders.theta << std::endl;

            encodersData->robotx = pose->robotEncoders.x ;
            encodersData->roboty = pose->robotEncoders.y ;
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
