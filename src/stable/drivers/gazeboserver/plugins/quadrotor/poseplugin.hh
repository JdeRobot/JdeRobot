#ifndef POSEPLUGIN_HH
#define POSEPLUGIN_HH

#include <boost/bind.hpp>
#include "gazebo/gazebo.hh"
#include "gazebo/physics/physics.hh"
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>

namespace gazebo {

//void* thread_PosePluginICE ( void* v );

class PosePlugin : public ModelPlugin {
public:
    PosePlugin();
    ~PosePlugin();

    mutable pthread_mutex_t pose_mtx;

protected:
    virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);
    virtual void Init();
    virtual void OnUpdate();

private:
    event::ConnectionPtr updateConnection;
    physics::WorldPtr world;
    physics::ModelPtr model;
    sdf::ElementPtr sdf;
    physics::LinkPtr link;
    transport::PublisherPtr pub_;
    std::string pose_topic_;
};

} //gazebo
#endif // POSEPLUGIN_H
