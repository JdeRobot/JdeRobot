#ifndef __GAZEBO_POSE3D_H__
#define __GAZEBO_POSE3D_H__

#include <gazebo.hh>
#include <physics/physics.hh>
#include <common/common.hh>
#include <transport/transport.hh>

// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <jderobot/pose3d.h>

#include "sharer.h"

namespace gazebo
{

	class Pose3d : public ModelPlugin
	{
		public:

			Pose3d();

			virtual void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/);

			physics::ModelPtr getModel();

			pthread_mutex_t mutex;
			int count;
			std::string namePose3d;

		private:

			void OnUpdate();

			physics::ModelPtr model;
			event::ConnectionPtr updateConnection;

	};

} /* gazebo */
#endif /* __GAZEBO_POSE3D_H__ */
