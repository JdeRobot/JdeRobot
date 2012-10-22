#include <boost/bind.hpp>
#include "gazebo.hh"
#include "physics/physics.h"
#include "common/common.h"
#include "transport/transport.h"

#include "plugins/RayPlugin.hh"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <gbxsickacfr/gbxiceutilacfr/safethread.h>

// JDErobot general ice component includes
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <jderobotice/exceptions.h>

#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


float posx, posy, posz, postheta;
pthread_mutex_t mutex;

/*
namespace MotorEncoder{
	class Component: public jderobotice::Component {
		public:
			Component():jderobotice::Component("Encoder") 
			{
				std::cout << "Constructor" << std::endl;
			}
		virtual 
			void start()
			{
			}
	};
}


void *myMain2(void* v) 
{

	char* name = (char*)v;

	MotorEncoder::Component component;
	jderobotice::Application app(component);
	char* argv[] = {"--Ice.Config=MotorEncoder.cfg"};
	std::cout << "MyMain" << std::endl;
	int argc = 1;
	app.jderobotMain(argc,argv);
}
*/
namespace gazebo
{     
  class CameraMove : public ModelPlugin
  {     
    public: CameraMove() : ModelPlugin() 
    {
		pthread_mutex_init (&mutex, NULL);

		//pthread_t thr_gui;
		//pthread_create(&thr_gui, NULL, &myMain2, NULL);	
    }

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

		math::Pose pose = this->model->GetWorldPose();
		math::Vector3 v(0.01, 0, 0);
		v = pose.rot * v;

		pthread_mutex_lock (&mutex);
		posx = pose.pos.x;
		posy = pose.pos.y;
		posz = pose.pos.z;
		postheta = pose.rot.w;
		pthread_mutex_unlock (&mutex);

		//std::cout << pose << std::endl;
		this->model->SetLinearVel(v);
		this->model->SetAngularVel(math::Vector3(0, 0, 0.01));

    } 

    // Pointer to the model
    private: physics::ModelPtr model;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };
  
  GZ_REGISTER_MODEL_PLUGIN(CameraMove)
}
