#include <boost/bind.hpp>
#include "gazebo.hh"
#include "physics/physics.h"
#include "common/common.h"
#include "transport/transport.h"

namespace gazebo
{     
  class CameraMove : public ModelPlugin
  {     
    public: CameraMove() : ModelPlugin() {}

    public: void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
    {   
      // Get a pointer to the model
      this->model = _model;

      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateStart(
          boost::bind(&CameraMove::OnUpdate, this));
    }       

    // Called by the world update start event
    public: void OnUpdate()
    { 
      math::Vector3 v(0.03, 0, 0);
      math::Pose pose = this->model->GetWorldPose();
      v = pose.rot * v;

      // Apply a small linear velocity to the model. 
      this->model->SetLinearVel(v);
      this->model->SetAngularVel(math::Vector3(0, 0, 0.01));
    } 

    // Pointer to the model
    private: physics::ModelPtr model;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(CameraMove)
}
