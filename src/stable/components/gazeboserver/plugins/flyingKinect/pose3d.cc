#include "pose3d.h"

namespace gazebo
{

	void *mainPose3d(void* v);

	GZ_REGISTER_MODEL_PLUGIN(Pose3d)

	Pose3d::Pose3d()
	{
		pthread_mutex_init(&mutex, NULL);
		count = 0;
	}

	void Pose3d::Load(physics::ModelPtr _parent, sdf::ElementPtr)
	{
		model = _parent;
		this->updateConnection = event::Events::ConnectWorldUpdateBegin(
				boost::bind(&Pose3d::OnUpdate, this));
	}

	physics::ModelPtr Pose3d::getModel()
	{
		return model;
	}

	void Pose3d::OnUpdate()
	{
		if (count == 0)
		{
			Sharer::getInstance()->setGzModel(this->getModel());

			count++;
			std::string name = this->model->GetLinks()[0]->GetName();
			namePose3d = std::string("--Ice.Config=" + name + "Pose3d.cfg");
			pthread_t thr_pose3d;
			pthread_create(&thr_pose3d, NULL, &mainPose3d, (void*) this);
		}
	}


	class Pose3DI : virtual public jderobot::Pose3D
	{

		public:

			Pose3DI(gazebo::Pose3d* pose) : pose3dData(new jderobot::Pose3DData())
			{
				this->pose = pose;
			}

			virtual ~Pose3DI()
			{
			}

			virtual Ice::Int setPose3DData(const jderobot::Pose3DDataPtr& pose3dData,
					const Ice::Current&)
			{
				math::Pose position = this->pose->getModel()->GetWorldPose();

				position.Set(math::Vector3(pose3dData->x, pose3dData->y, pose3dData->z),
						math::Quaternion(pose3dData->q0, pose3dData->q1,
							pose3dData->q2, pose3dData->q3)
						);

				this->pose->getModel()->SetWorldPose(position);

				return 0;
			}

			virtual jderobot::Pose3DDataPtr getPose3DData(const Ice::Current&)
			{
				math::Pose position = Sharer::getInstance()->getCurrentPose3d();

				pthread_mutex_lock(&pose->mutex);
					pose3dData->x = position.pos.x;
					pose3dData->y = position.pos.y;
					pose3dData->z = position.pos.z;
					pose3dData->h = 1; // position in meters
					pose3dData->q0 = position.rot.w;
					pose3dData->q1 = position.rot.x;
					pose3dData->q2 = position.rot.y;
					pose3dData->q3 = position.rot.z;
				pthread_mutex_unlock(&pose->mutex);

				return pose3dData;
			}

		private:

			gazebo::Pose3d* pose;
			jderobot::Pose3DDataPtr pose3dData;

	}; // end class Pose3DI


	void *mainPose3d(void* v)
	{
		gazebo::Pose3d* base = (gazebo::Pose3d*)v;

		char* name = (char*) base->namePose3d.c_str();

		Ice::CommunicatorPtr ic;
		int argc = 1;

		Ice::PropertiesPtr prop;
		char* argv[] = {name};

		try
		{
			ic = Ice::initialize(argc, argv);
			prop = ic->getProperties();

			std::vector<std::string> endpoints;        
			std::string property = "";
			for (int i=0; i<6; i++) {
				property = "Pose3d"+boost::to_string (i)+".Endpoints";
	        		std::string Endpoints = prop->getProperty(property);
        			//std::cout << "Pose3d Endpoints > " << Endpoints << std::endl;
				endpoints.push_back(Endpoints);
        		}

			bool connected = false;
			int cont = 0;
			Ice::ObjectAdapterPtr adapter;	

			while (!connected) {
				try {
					adapter = ic->createObjectAdapterWithEndpoints("Pose3d", endpoints[cont]);
					connected = true;
					std::cout << "Connected to free endpoint: " << endpoints[cont] << std::endl;
				}catch (Ice::SocketException& ss) {
					cont++;
				}
			}

			Ice::ObjectPtr object = new Pose3DI(base);

			adapter->add(object, ic->stringToIdentity("Pose3d"));
			adapter->activate();
			ic->waitForShutdown();
		} catch (const Ice::Exception& e) {
			std::cerr << e << std::endl;
		} catch (const char* msg) {
			std::cerr << msg << std::endl;
		}

		if (ic)
		{
			try
			{
				ic->destroy();
			} catch (const Ice::Exception& e) {
				std::cerr << e << std::endl;
			}
		}
	}

} /* gazebo */
