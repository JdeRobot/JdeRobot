#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <stdio.h>
#include <iostream>

namespace gazebo
{
  	class ModelPush : public ModelPlugin {

		private: math::Pose pose;

  		public: void Load(physics::ModelPtr _parent, sdf::ElementPtr ) {
      			this->model = _parent;
	  		this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          		boost::bind(&ModelPush::OnUpdate, this, _1));
		}

    		public: void OnUpdate(const common::UpdateInfo & ) {
			pose = this->model->GetWorldPose();
			
			if (abs(pose.pos.y+20)<2) {
				this->model->SetLinearVel(math::Vector3(2, 0, 0));
    	  			if ( pose.pos.x >=50 ) {
					pose.pos.x = 10;
					this->model->SetWorldPose(pose);
				}
			}

			if (abs(pose.pos.y)<2) {
				this->model->SetLinearVel(math::Vector3(7, 0, 0));
    	  			if ( pose.pos.x >=30 ) {
					pose.pos.x = -50;
					this->model->SetWorldPose(pose);
				}	
			}

			if (abs(pose.pos.y-20)<2) {
				this->model->SetLinearVel(math::Vector3(10, 0, 0));
    	  			if ( pose.pos.x >=50 ) {
					pose.pos.x = -100;
					this->model->SetWorldPose(pose);
				}	
			}
		}
		private: physics::ModelPtr model;
		private: event::ConnectionPtr updateConnection;
  	};
  	GZ_REGISTER_MODEL_PLUGIN(ModelPush)
}
