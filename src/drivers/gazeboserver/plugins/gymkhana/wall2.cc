#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <stdio.h>
#include <iostream>

namespace gazebo
{
	class Wall2 : public ModelPlugin {

		private: math::Pose pose;
		private: bool flag;
		private: double vel1; 
		private: double vel2; 

		public: void Load(physics::ModelPtr _parent, sdf::ElementPtr ) {
			this->model = _parent;
			flag = true;
			this->updateConnection = event::Events::ConnectWorldUpdateBegin(
				boost::bind(&Wall2::OnUpdate, this, _1));
			std::cout << "Loading wall 2" << std::endl;
			this->vel1 = 0.5 + double(rand())/RAND_MAX*1.5;
			this->vel2 = (0.5 + double(rand())/RAND_MAX*1.5)*-1;
		}

		public: void OnUpdate(const common::UpdateInfo & ) {
			pose = this->model->GetWorldPose();

			if  (flag) {
				this->model->SetLinearVel(math::Vector3(0, 0, this->vel1));
			}
			if ( pose.pos.z >= 2.8 ) {
				pose.pos.z = 2.8;
				this->model->SetWorldPose(pose);
				flag = false;
			}
			if (!flag) {
				this->model->SetLinearVel(math::Vector3(0, 0, this->vel2));
			}	
			if ( pose.pos.z <= 0) {
				pose.pos.z = 0;
				this->model->SetWorldPose(pose);
				flag = true;
			}

		}
		private: physics::ModelPtr model;
		private: event::ConnectionPtr updateConnection;
	};
	GZ_REGISTER_MODEL_PLUGIN(Wall2)
}
