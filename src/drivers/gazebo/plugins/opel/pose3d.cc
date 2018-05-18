#include "pose3d.h"

namespace gazebo {

    void *Pose3DICE(void* v);

    GZ_REGISTER_MODEL_PLUGIN(Pose3D)

    Pose3D::Pose3D() {
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_init(&mutexPose3D, NULL);
        count = 0;
        //exit(1);
        std::cout << "-----------------constructor Pose3D" << std::endl;
    }

    void Pose3D::Load(physics::ModelPtr _parent, sdf::ElementPtr) {

        model = _parent;
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                boost::bind(&Pose3D::OnUpdate, this));
    }
    
    physics::ModelPtr Pose3D::getModel()
    {
    	return model;
    }

    void Pose3D::OnUpdate() {

        if (count == 0) {
            count++;
            std::string name = this->model->GetName();
            std::cout << "GetName() Pose3D: " << name << std::endl;
            namePose3D = std::string("--Ice.Config=" + name + "Pose3D.cfg");
            pthread_t thr_gui;
            pthread_create(&thr_gui, NULL, &Pose3DICE, (void*) this);
        }

        position = model->GetWorldPose();
        this->initial_q = position.rot;


        pthread_mutex_lock(&mutex);
        robotPose3D.x = position.pos.x;
        robotPose3D.y = position.pos.y;
        robotPose3D.q0 = position.rot.w;
        robotPose3D.q1 = position.rot.x;
        robotPose3D.q2 = position.rot.y;
        robotPose3D.q3 = position.rot.z;
        pthread_mutex_unlock(&mutex);

    }

    class Pose3DI : virtual public jderobot::Pose3D {
    public:

        Pose3DI(gazebo::Pose3D* pose) : Pose3DData(new jderobot::Pose3DData()) {
            this->pose = pose;
        }

        virtual ~Pose3DI() {
        };
        
        virtual int setPose3DData(const jderobot::Pose3DDataPtr&  Pose3DData,
        						     const Ice::Current&) {
             math::Pose position = this->pose->getModel()->GetWorldPose();
             


             position.pos.x = Pose3DData->x / Pose3DData->h;
             position.pos.y = Pose3DData->y / Pose3DData->h;
             position.pos.z = Pose3DData->z / Pose3DData->h;

             position.rot.w = Pose3DData->q0;
             position.rot.x = Pose3DData->q1;
             position.rot.y = Pose3DData->q2;
             position.rot.z = Pose3DData->q3;

             this->pose->getModel()->SetWorldPose(position);

             return 0;
			//this->getModel();
        }


        virtual jderobot::Pose3DDataPtr getPose3DData(const Ice::Current&) {
            pthread_mutex_lock(&pose->mutex);

            //std::cout << "theta: " << pose->robotPose3D.theta << std::endl;

            Pose3DData->x = pose->robotPose3D.x;
            Pose3DData->y = pose->robotPose3D.y;
            Pose3DData->z = 0.0;
            Pose3DData->h = 1.0;

            Pose3DData->q0 = pose->robotPose3D.q0;
            Pose3DData->q1 = pose->robotPose3D.q1;
            Pose3DData->q2 = pose->robotPose3D.q2;
            Pose3DData->q3 = pose->robotPose3D.q3;


            pthread_mutex_unlock(&pose->mutex);

            return Pose3DData;
        };

    public:
        gazebo::Pose3D* pose;
    private:
        jderobot::Pose3DDataPtr Pose3DData;
    };

    void *Pose3DICE(void* v) {
        gazebo::Pose3D* base = (gazebo::Pose3D*)v;
        char* name = (char*) base->namePose3D.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {

            ic = EasyIce::initialize(argc, argv);


            prop = ic->getProperties();
            std::string Endpoints = prop->getProperty("Pose3D.Endpoints");
            std::cout << "Pose3D Endpoints " << Endpoints << std::endl;

            Ice::ObjectAdapterPtr adapter =
                    ic->createObjectAdapterWithEndpoints("Pose3D", Endpoints);
            Ice::ObjectPtr object = new Pose3DI(base);

            adapter->add(object, ic->stringToIdentity("Pose3D"));

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
