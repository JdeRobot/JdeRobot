#include <boost/bind.hpp>
#include "gazebo/gazebo.hh"
#include "gazebo/common/common.hh"
#include <gazebo/sensors/sensors.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <pthread.h>
#include <pose3d.h>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#ifndef IMUPLUGIN_H
#define IMUPLUGIN_H

namespace gazebo {

void* thread_Pose3DICE ( void* v );

class ImuPlugin : public SensorPlugin {
public:

    ImuPlugin ();
    ~ImuPlugin();
    virtual void Load (sensors::SensorPtr _sensor, sdf::ElementPtr _sdf);
    virtual void Init ();

    struct imuplugin_t {
        math::Pose pose;
        math::Quaternion orien;
    };

    imuplugin_t imuplugin;
    pthread_mutex_t mutex_imuplugin;
    math::Pose pose;
    //pthread_mutex_t pose_mutex;
//    pthread_mutex_t mutex_imupluginmotors;
    std::string cfgfile_imuplugin;
private:
    virtual void OnUpdate ();

    //int cycle;
    event::ConnectionPtr updateConnection;
    sensors::ImuSensorPtr parentSensor;
    transport::SubscriberPtr sub_;
    std::string pose_topic_;
    void PoseCallback(const boost::shared_ptr<const msgs::Pose> &_msg);
};

} //gazebo

#endif // IMUPLUGIN_H
