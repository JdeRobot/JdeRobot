#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <stdio.h>
#include <iostream>

namespace gazebo
{
  	class Wall1 : public ModelPlugin {

		private: math::Pose pose;
  		private: bool flag;

  		public: void Load(physics::ModelPtr _parent, sdf::ElementPtr ) {
      			this->model = _parent;
			flag = true;
	  		this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          		boost::bind(&Wall1::OnUpdate, this, _1));
			std::cout << "Loading wall 1" << std::endl;
			
		}

    		public: void OnUpdate(const common::UpdateInfo & ) {
			pose = this->model->GetWorldPose();

			if  (flag) {
				this->model->SetLinearVel(math::Vector3(1, 0, 0));
			}
			if ( pose.pos.x >=4 ) {
			        pose.pos.x = 4;
			        this->model->SetWorldPose(pose);
				flag = false;
			}
			if (!flag) {
				this->model->SetLinearVel(math::Vector3(-1, 0, 0));
			}	
			if ( pose.pos.x <=-4 ) {
			        pose.pos.x = -4;
			        this->model->SetWorldPose(pose);
				flag = true;
			}

		}
		private: physics::ModelPtr model;
		private: event::ConnectionPtr updateConnection;
  	};
  	GZ_REGISTER_MODEL_PLUGIN(Wall1)
}
