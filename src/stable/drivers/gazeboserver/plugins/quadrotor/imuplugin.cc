#include "imuplugin.h"

namespace gazebo {

GZ_REGISTER_SENSOR_PLUGIN(ImuPlugin);

ImuPlugin::ImuPlugin() {
    pthread_mutex_init(&mutex_imuplugin, NULL);
	std::cout << "Constructor ImuPlugin\n";
}

ImuPlugin::~ImuPlugin() {}

void ImuPlugin::PoseCallback(const boost::shared_ptr<const msgs::Pose> &_msg) {
    pthread_mutex_lock(&this->mutex_imuplugin);
    pose = msgs::Convert(*_msg);
    pthread_mutex_unlock(&this->mutex_imuplugin);
}

void ImuPlugin::Load(sensors::SensorPtr _sensor, sdf::ElementPtr _sdf) {
    this->parentSensor =
        boost::dynamic_pointer_cast<sensors::ImuSensor>(_sensor);

    if (!parentSensor)
        gzerr << "No ImuSensor detected.\n";

    this->parentSensor->SetActive(true);
    if(!_sdf->HasElement("cfgFile"))
        this->cfgfile_imuplugin = "--Ice.Config=imuplugin.cfg";
    else
        this->cfgfile_imuplugin = "--Ice.Config=" + _sdf->GetElement("cfgFile")->GetValue()->GetAsString();
    if(!_sdf->HasElement("poseTopic"))
        this->pose_topic_ = "~/pose_topic";
    else
        this->pose_topic_ = _sdf->GetElement("poseTopic")->GetValue()->GetAsString();
    transport::NodePtr node(new transport::Node());
    node->Init();
    sub_ = node->Subscribe(this->pose_topic_, &ImuPlugin::PoseCallback, this);

    pthread_t thr_ice;
    pthread_create(&thr_ice, NULL, &thread_Pose3DICE, (void*) this);
    this->updateConnection = this->parentSensor->ConnectUpdated(
        boost::bind(&ImuPlugin::OnUpdate, this));
}

void ImuPlugin::Init() {
    /*parentSensor->SetReferencePose();
    pthread_mutex_lock(&this->mutex_imuplugin);
    this->imuplugin.pose.Set(0, 0, 0, 0, 0, 0);
    this->imuplugin.orien.SetFromEuler(0, 0, 0);
    pthread_mutex_unlock(&this->mutex_imuplugin);*/
}

void ImuPlugin::OnUpdate() {
    pthread_mutex_lock(&this->mutex_imuplugin);
    math::Quaternion aux(this->parentSensor->GetOrientation().GetRoll(),
                         this->parentSensor->GetOrientation().GetPitch(),
                         pose.rot.GetYaw());
    imuplugin.pose.pos = pose.pos;
    imuplugin.orien = aux;
    pthread_mutex_unlock(&this->mutex_imuplugin);
}

class Pose3DI : virtual public jderobot::Pose3D {
public:

    Pose3DI (ImuPlugin* imu_p) : pose3DData ( new jderobot::Pose3DData() ) {
        this->imu_p = imu_p;
    }

    virtual ~Pose3DI () {}

    virtual jderobot::Pose3DDataPtr getPose3DData ( const Ice::Current& ) {
        pthread_mutex_lock(&imu_p->mutex_imuplugin);
        
        pose3DData->x = imu_p->imuplugin.pose.pos.x;
        pose3DData->y = imu_p->imuplugin.pose.pos.y;
        pose3DData->z = imu_p->imuplugin.pose.pos.z;
        pose3DData->h = 0;
        pose3DData->q0 = imu_p->imuplugin.orien.w;
        pose3DData->q1 = imu_p->imuplugin.orien.x;
        pose3DData->q2 = imu_p->imuplugin.orien.y;
        pose3DData->q3 = imu_p->imuplugin.orien.z;
        
        pthread_mutex_unlock(&imu_p->mutex_imuplugin);

        return pose3DData;
    }

    virtual Ice::Int setPose3DData ( const jderobot::Pose3DDataPtr & data,
                                     const Ice::Current& ) {
        pthread_mutex_lock(&imu_p->mutex_imuplugin);
        
        imu_p->imuplugin.pose.pos.x = data->x;
        imu_p->imuplugin.pose.pos.y = data->y;
        imu_p->imuplugin.pose.pos.z = data->z;
        imu_p->imuplugin.orien.w = data->q0;
        imu_p->imuplugin.orien.x = data->q1;
        imu_p->imuplugin.orien.y = data->q2;
        imu_p->imuplugin.orien.z = data->q3;

        pthread_mutex_unlock(&imu_p->mutex_imuplugin);
    }

    ImuPlugin* imu_p;

private:

    jderobot::Pose3DDataPtr pose3DData;
};

void* thread_Pose3DICE ( void* v )
{
    ImuPlugin* imuplugin = (ImuPlugin*)v;
    char* name = (char*) imuplugin->cfgfile_imuplugin.c_str();
    Ice::CommunicatorPtr ic;
    int argc = 1;
    Ice::PropertiesPtr prop;
    char* argv[] = {name};

    try {
        ic = Ice::initialize(argc, argv);

        prop = ic->getProperties();
        std::string imu_name = prop->getProperty("ImuPlugin.Name");
        std::string endpoints = prop->getProperty("ImuPlugin.Endpoints");
        std::cout << "ImuPlugin Endpoints > " << endpoints << std::endl;

        Ice::ObjectAdapterPtr Adapter =
                ic->createObjectAdapterWithEndpoints("AdapterImuPlugin", endpoints);

        Ice::ObjectPtr pose3d = new Pose3DI(imuplugin);
        Adapter->add(pose3d, ic->stringToIdentity(imu_name));
        
        Adapter->activate();

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

} //gazebo
