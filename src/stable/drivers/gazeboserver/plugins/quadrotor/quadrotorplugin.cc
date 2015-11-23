//=================================================================================================
// Copyright (c) 2012, Johannes Meyer, TU Darmstadt
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Flight Systems and Automatic Control group,
//       TU Darmstadt, nor the names of its contributors may be used to
//       endorse or promote products derived from this software without
//       specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=================================================================================================

#include "quadrotorplugin.hh"

#define cycle_control 5 //miliseconds, 200 Hz

namespace gazebo {

GZ_REGISTER_MODEL_PLUGIN(QuadrotorPlugin)

QuadrotorPlugin::QuadrotorPlugin () {
    navi_state = Unknown;
    std::cout << "Contructor QuadrotorPlugin" << std::endl;
}

QuadrotorPlugin::~QuadrotorPlugin() {}

void QuadrotorPlugin::setVelocityCommand(const jderobot::CMDVelDataPtr& vel) {
    pthread_mutex_lock(&quadrotor_mtx);

    this->velocity_command_ = vel;

    if (fabs(vel->linearZ) > vzMax) {
        velocity_command_->linearZ = vel->linearZ > 0 ? vzMax : -1*vzMax;
    }
    if (fabs(vel->angularZ) > yawSpeed) {
        velocity_command_->angularZ = vel->angularZ > 0 ?
                                      yawSpeed : -1*yawSpeed;
    }

    pthread_mutex_unlock(&quadrotor_mtx);
}

common::Time QuadrotorPlugin::getSimTime() const {
    common::Time time;

    pthread_mutex_lock(&quadrotor_mtx);
    time = world->GetSimTime();
    pthread_mutex_unlock(&quadrotor_mtx);

    return time;
}

void QuadrotorPlugin::setConfiguration(QuadrotorConfig *conf) {
	this->conf = conf;
}

void QuadrotorPlugin::configureDrone() {
/*
    <max_bitrate>4000</max_bitrate>
*/
    pthread_mutex_lock(&quadrotor_mtx);

	eulerMax=conf->get_quadrotor_euler_angle_max();
	
	//set_navdata_demo_value = DEFAULT_NAVDATA_DEMO;
	cam_state=conf->get_quadrotor_default_camera();
	altMax=conf->get_quadrotor_altitude_max();
	altMin=conf->get_quadrotor_altitude_min();

	vzMax=conf->get_quadrotor_control_vz_max();
	yawSpeed=conf->get_quadrotor_control_yaw();
	isOutdoor=conf->get_quadrotor_outdoor();
	withoutShell=conf->get_quadrotor_flight_without_shell();
	frameSize=conf->get_quadrotor_bitrate();

    pthread_mutex_unlock(&quadrotor_mtx);
}

void QuadrotorPlugin::ImageCallback(const boost::shared_ptr<const msgs::Image> &_msg) {
	common::Image c_image;
	msgs::Set(c_image, *_msg);
    pthread_mutex_lock(&img_mutex);
	image->create(c_image.GetHeight(), c_image.GetWidth(), CV_8UC3);
    //image->resize(c_image.GetHeight() * c_image.GetWidth() * 3);
	unsigned int size = c_image.GetHeight()*c_image.GetWidth()*3;
    unsigned char * char_arr[size];
	c_image.GetData(char_arr, size);
    memcpy((unsigned char *)image->data, *char_arr, size);
    pthread_mutex_unlock(&img_mutex);
}

void QuadrotorPlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {
    this->model = _model;
    this->sdf = _sdf;
    this->world = _model->GetWorld();

    navi_state = Initialize;
    std::cout << "State: Initialize\n";

    if (!_sdf->HasElement("bodyName"))
        link = _model->GetLink();
    else {
        std::string link_name_ = _sdf->GetElement("bodyName")->GetValue()->GetAsString();
        link = boost::dynamic_pointer_cast<physics::Link>(world->GetEntity(link_name_));
    }
    if (!_sdf->HasElement("maxForce"))
        max_force_ = -1;
    else
        std::istringstream(_sdf->GetElement("maxForce")->GetValue()->GetAsString()) >> max_force_;
    if (!_sdf->HasElement("motionSmallNoise"))
        motion_small_noise_ = 0;
    else
        std::istringstream(_sdf->GetElement("motionSmallNoise")->GetValue()->GetAsString()) >> motion_small_noise_;
    if (!_sdf->HasElement("motionDriftNoise"))
        motion_drift_noise_ = 0;
    else
        std::istringstream(_sdf->GetElement("motionDriftNoise")->GetValue()->GetAsString()) >> motion_drift_noise_;
    if (!_sdf->HasElement("motionDriftNoiseTime"))
        motion_drift_noise_time_ = 1.0;
    else
        std::istringstream(_sdf->GetElement("motionDriftNoiseTime")->GetValue()->GetAsString()) >> motion_drift_noise_time_;
    if(!_sdf->HasElement("cfgFile"))
        this->cfgfile_QuadrotorPlugin = "--Ice.Config=quadrotorplugin.cfg";
    else
        this->cfgfile_QuadrotorPlugin = "--Ice.Config=" + _sdf->GetElement("cfgFile")->GetValue()->GetAsString();
    if(!_sdf->HasElement("imgTopic"))
        this->img_topic_ = "~/image_topic";
    else
        this->img_topic_ = _sdf->GetElement("imgTopic")->GetValue()->GetAsString();
    if(!_sdf->HasElement("toggleTopic"))
        this->toggle_topic_ = "~/toggle_topic";
    else
        this->toggle_topic_ = _sdf->GetElement("toggleTopic")->GetValue()->GetAsString();

    controllers_.roll.Load(_sdf, "rollpitch");
    controllers_.pitch.Load(_sdf, "rollpitch");
    controllers_.yaw.Load(_sdf, "yaw");
    controllers_.velocity_x.Load(_sdf, "velocityXY");
    controllers_.velocity_y.Load(_sdf, "velocityXY");
    controllers_.velocity_z.Load(_sdf, "velocityZ");

    inertia = link->GetInertial()->GetPrincipalMoments();
    mass = link->GetInertial()->GetMass();

    m_isFlying = m_takeOff = false;
    m_timeAfterTakeOff = 0;

    Reset();
    sc = new StateController(this);

    pthread_t thr_ice;
    pthread_create(&thr_ice, NULL, &thread_QuadrotorPluginICE, (void*) this);
    pthread_t thr_state;
    pthread_create(&thr_state, NULL, &thread_state_controller, (void*) this);

    image = new cv::Mat();

    transport::NodePtr sub_node(new transport::Node());
    sub_node->Init();
    sub_ = sub_node->Subscribe(this->img_topic_, &QuadrotorPlugin::ImageCallback, this);
}

void QuadrotorPlugin::Init() {
    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          boost::bind(&QuadrotorPlugin::OnUpdate, this));

    pthread_mutex_lock(&quadrotor_mtx);
    navi_state = Landed;
    velocity_command_ = new jderobot::CMDVelData();
    velocity_command_->linearZ = velocity_command_->angularZ =
    velocity_command_->linearY = velocity_command_->angularY =
    velocity_command_->linearX = velocity_command_->angularX = 0;
    eulerMax = 0.7854; // rad
    vzMax = 2000; // mm/s
    yawSpeed = 1.75; // rad/s
    altMax = 100; // m
    pthread_mutex_unlock(&quadrotor_mtx);

    std::cout << "State: Landed\n";
}

void QuadrotorPlugin::OnUpdate() {
    math::Vector3 force, torque;

    // Get simulator time
    common::Time sim_time = world->GetSimTime();
    double dt = (sim_time - last_time).Double();
    if (dt == 0.0) return;

    pose = link->GetWorldPose();
    angular_velocity = link->GetWorldAngularVel();
    euler = pose.rot.GetAsEuler();
    acceleration = (link->GetWorldLinearVel() - velocity) / dt;
    velocity = link->GetWorldLinearVel();

    // Get gravity
    math::Vector3 gravity_body = pose.rot.RotateVector(
        world->GetPhysicsEngine()->GetGravity());
    double gravity = gravity_body.GetLength();
    double load_factor = gravity * gravity /
        world->GetPhysicsEngine()->GetGravity().Dot(gravity_body);

    // Rotate vectors to coordinate frames relevant for control
    math::Quaternion heading_quaternion(cos(euler.z/2),0,0,sin(euler.z/2));
    math::Vector3 velocity_xy =
        heading_quaternion.RotateVectorReverse(velocity);
    math::Vector3 acceleration_xy =
        heading_quaternion.RotateVectorReverse(acceleration);
    math::Vector3 angular_velocity_body =
        pose.rot.RotateVectorReverse(angular_velocity);

    // update controllers
    force.Set(0.0, 0.0, 0.0);
    torque.Set(0.0, 0.0, 0.0);

    pthread_mutex_lock(&quadrotor_mtx);

    // Add noise to velocity_command_
    AddNoiseToVel();
    double pitch_command =  controllers_.velocity_x.update(
        velocity_command_->linearX,
        velocity_xy.x,
        acceleration_xy.x, dt) / gravity;
    pitch_command = pitch_command > eulerMax ? eulerMax : pitch_command;
    pitch_command = pitch_command < -eulerMax ? -eulerMax : pitch_command;
    double roll_command  = -controllers_.velocity_y.update(
        velocity_command_->linearY,
        velocity_xy.y,
        acceleration_xy.y, dt) / gravity;
    roll_command = roll_command > eulerMax ? eulerMax : roll_command;
    roll_command = roll_command < -eulerMax ? -eulerMax : roll_command;
    torque.x = inertia.x * controllers_.roll.update(
        roll_command, euler.x, angular_velocity_body.x, dt);
    torque.y = inertia.y * controllers_.pitch.update(
        pitch_command, euler.y, angular_velocity_body.y, dt);
    torque.z = inertia.z * controllers_.yaw.update(
        velocity_command_->angularZ, angular_velocity.z, 0, dt);
    force.z = mass * (controllers_.velocity_z.update(
        velocity_command_->linearZ,
        velocity.z, acceleration.z, dt) +
        load_factor * gravity);
    if (max_force_ > 0.0 && force.z > max_force_) force.z = max_force_;
    if (force.z < 0.0) force.z = 0.0;

    // process robot state information
    if(navi_state == Landed) {
    } else if((navi_state == Flying) ||
        (navi_state == ToFixPoint)) {
        link->AddRelativeForce(force);
        link->AddRelativeTorque(torque);
    } else if(navi_state == TakingOff) {
        link->AddRelativeForce(force*1.5);
        link->AddRelativeTorque(torque*1.5);
    } else if(navi_state == Landing) {
        link->AddRelativeForce(force*0.8);
        link->AddRelativeTorque(torque*0.8);
    }
    pthread_mutex_unlock(&quadrotor_mtx);

    // save last time stamp
    last_time = sim_time;
}

void QuadrotorPlugin::AddNoiseToVel() {

    static common::Time last_sim_time = world->GetSimTime();
    static double time_counter_for_drift_noise = 0;
    static double drift_noise[4] = {0.0, 0.0, 0.0, 0.0};
    // Get simulator time
    common::Time cur_sim_time = world->GetSimTime();
    double noise_dt = (cur_sim_time - last_sim_time).Double();
    // save last time stamp
    last_sim_time = cur_sim_time;

    // generate noise
    if(time_counter_for_drift_noise > motion_drift_noise_time_) {
        drift_noise[0] = 2*motion_drift_noise_*(drand48()-0.5);
        drift_noise[1] = 2*motion_drift_noise_*(drand48()-0.5);
        drift_noise[2] = 2*motion_drift_noise_*(drand48()-0.5);
        drift_noise[3] = 2*motion_drift_noise_*(drand48()-0.5);
        time_counter_for_drift_noise = 0.0;
    }
    time_counter_for_drift_noise += noise_dt;

    this->velocity_command_->linearZ += drift_noise[0] + 2*motion_small_noise_*(drand48()-0.5);
    this->velocity_command_->linearX += drift_noise[1] + 2*motion_small_noise_*(drand48()-0.5);
    this->velocity_command_->linearY += drift_noise[2] + 2*motion_small_noise_*(drand48()-0.5);
    this->velocity_command_->angularZ += drift_noise[3] + 2*motion_small_noise_*(drand48()-0.5);
}

void QuadrotorPlugin::Reset() {
  controllers_.roll.reset();
  controllers_.pitch.reset();
  controllers_.yaw.reset();
  controllers_.velocity_x.reset();
  controllers_.velocity_y.reset();
  controllers_.velocity_z.reset();
  link->SetForce(math::Vector3(0,0,0));
  link->SetTorque(math::Vector3(0,0,0));

  // reset state
  pose.Reset();
  velocity.Set();
  angular_velocity.Set();
  acceleration.Set();
  euler.Set();
}

void QuadrotorPlugin::RunController() {
    //StateController* sc = new StateController(qp);
    struct timeval a, b;
    long diff;
    long totalb, totala;

    while(true) {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        sc->updateController();

        //Sleep Algorithm
        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;
        diff = (totalb - totala) / 1000;
        if (diff < 0 || diff > cycle_control)
            diff = cycle_control;
        else
            diff = cycle_control - diff;

        /*Sleep Algorithm*/
        usleep(diff * 1000);
        if (diff < 33)
            usleep(33 * 1000);
    }
}

class ArDroneExtraI : virtual public jderobot::ArDroneExtra {
public:
    ArDroneExtraI(QuadrotorPlugin *qp) {
        this->qp = qp;
        transport::NodePtr node(new transport::Node());
        node->Init();
        pub_ = node->Advertise<msgs::Vector2d>(qp->toggle_topic_);
    }

	virtual void recordOnUsb(bool  record, Ice::Current const & c) {}
	
	virtual void ledAnimation(Ice::Int type, Ice::Float duration,
                              Ice::Float freq, Ice::Current const & c) {}
	
	virtual void flightAnimation(Ice::Int type, Ice::Float duration,
                                 Ice::Current const & c) {}
	
	virtual void flatTrim(Ice::Current const & c) {}

    virtual void takeoff(const Ice::Current&) {
        pthread_mutex_lock(&qp->quadrotor_mtx);
        if (qp->navi_state == Landed ||
            qp->navi_state == Landing)
        {
            qp->m_isFlying = true;
            qp->m_takeOff = true;
        }
        pthread_mutex_unlock(&qp->quadrotor_mtx);
    }

    virtual void land(const Ice::Current&) {
        pthread_mutex_lock(&qp->quadrotor_mtx);
        if (qp->navi_state == Flying ||
            qp->navi_state == ToFixPoint ||
            qp->navi_state == TakingOff)
        {
            qp->m_isFlying = false;
            qp->m_takeOff = false;
        }
        pthread_mutex_unlock(&qp->quadrotor_mtx);
    }

    virtual void toggleCam(const Ice::Current&) {
	    msgs::Vector2d msg;
        math::Vector2d v2d(0, 0);
	    msgs::Set(&msg, v2d);
	    pub_->Publish(msg);
	}

    virtual void reset(const Ice::Current&) {}

private:
    QuadrotorPlugin *qp;
    transport::PublisherPtr pub_;
};


class CMDVelI: virtual public jderobot::CMDVel {
public:
    CMDVelI(QuadrotorPlugin *qp) {
        this->qp=qp;
    }
    virtual ~CMDVelI() {
        delete qp;
    }
    virtual Ice::Int setCMDVelData(const jderobot::CMDVelDataPtr& vel,
                                   const Ice::Current&) {
        qp->setVelocityCommand(vel);
    }
private:
    QuadrotorPlugin *qp;
};


class RemoteConfigI: virtual public jderobot::remoteConfig {
public:
	RemoteConfigI(QuadrotorPlugin *qp) {
        this->qp=qp;
        idLocal=0;
    }

	virtual ~RemoteConfigI() {
	    delete qp;
    }

	virtual Ice::Int initConfiguration(const Ice::Current&) {
	    std::cout << "inicializado" << std::endl;
	    if (idLocal==0)
        {
		    /* initialize random seed: */
		    srand ( time(NULL) );

		    /* generate secret number: */
		    idLocal = rand() + 1;

		    std::stringstream ss;//create a stringstream
		    ss << idLocal << ".xml";//add number to the stream
	
		    path=ss.str();
		    f2.open(ss.str().c_str(), std::ofstream::out);
		    std::cout << "-----------------" <<  idLocal << std::endl;
		    return idLocal;
	    }
	    else
		    return 0;
    }

	virtual std::string read(Ice::Int id, const Ice::Current&) {
        return "";
    }

	virtual Ice::Int write(const std::string& data,
                           Ice::Int id, const Ice::Current&) {
	    if (id == idLocal) {
		    f2 << data << std::endl;
		    return 1;
	    } else
		    return 0;
    }

	virtual Ice::Int setConfiguration(Ice::Int id, const Ice::Current&) {
	    if (id == idLocal) {
		    id=0;
		    idLocal=0;
		    f2.close();
		    std::cout << "file completed" << std::endl;
		    // aqui tienes que llamar a tu parser de xml.
		    QuadrotorParser parser=QuadrotorParser(5);
		    QuadrotorConfig *conf=new QuadrotorConfig();
		    parser.readFile(path,conf);
            // CONCURRENCIA!!!
		    qp->setConfiguration(conf);
		    qp->configureDrone();

		    if(remove(path.c_str())!=0) {
			    std::cout << "Error deleting file" << std::endl;
		    }
		    return 1;
	    }
	    return 0;
    }

private:
	std::ofstream f2;
	int idLocal;
	std::string path;
	QuadrotorPlugin *qp;
};

class CameraI: virtual public jderobot::Camera {
public:
	CameraI(std::string propertyPrefix, QuadrotorPlugin* qp)
		   : prefix(propertyPrefix) {
	    this->qp = qp;
		std::cout << "Constructor CameraI" << std::endl;

        mFormats.push_back(colorspaces::ImageRGB8::FORMAT_RGB8.get()->name);
		imageDescription = (new jderobot::ImageDescription());

	    replyTask = new ReplyTask(this);
	    replyTask->start(); // my own thread
	}

	std::string getName () {
		return (cameraDescription->name);
	}

	std::string getRobotName () {
		return "RobotName";
	}

	virtual ~CameraI() {}

	virtual jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c){
		return imageDescription;
	}

	virtual jderobot::CameraDescriptionPtr getCameraDescription(const Ice::Current& c){
		return cameraDescription;
	}

	virtual Ice::Int setCameraDescription(
            const jderobot::CameraDescriptionPtr &description,
            const Ice::Current& c) {
		return 0;
	}

	virtual void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb,const std::string& format, const Ice::Current& c){
		replyTask->pushJob(cb, format);
	}

	virtual jderobot::ImageFormat getImageFormat(const Ice::Current& c)
	{
		jderobot::ImageFormat mFormats;
		mFormats.push_back(colorspaces::ImageRGB8::FORMAT_RGB8.get()->name);

		return mFormats;
	}

	virtual std::string startCameraStreaming(const Ice::Current&) {}

	virtual void stopCameraStreaming(const Ice::Current&) {}

	virtual void reset(const Ice::Current&) {}

private:
	class ReplyTask: public IceUtil::Thread {
	public:
		ReplyTask(CameraI* camera){
			mycamera = camera;
		   	std::cout << "safeThread" << std::endl;
		}

		void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, std::string format){
			mFormat = format;
			IceUtil::Mutex::Lock sync(requestsMutex);
			requests.push_back(cb);
		}

		virtual void run(){
			jderobot::ImageDataPtr reply(new jderobot::ImageData);
			struct timeval a, b;
			int cycle = 48;
			long totalb,totala;
			long diff;
			int count =0 ;

			while(1){
                pthread_mutex_lock(&mycamera->qp->img_mutex);
				if(!mycamera->qp->image){
                    pthread_mutex_unlock(&mycamera->qp->img_mutex);
					usleep(100);
					continue;
				}
                //pthread_mutex_unlock(&img_mutex);
				//if(count==0){
					//pthread_mutex_lock (&img_mutex);
					mycamera->imageDescription->width = mycamera->qp->image->cols;
					mycamera->imageDescription->height = mycamera->qp->image->rows;
					mycamera->imageDescription->size = mycamera->qp->image->cols*mycamera->qp->image->rows*3;
					pthread_mutex_unlock (&mycamera->qp->img_mutex);

					mycamera->imageDescription->format = "RGB8";

					reply->description = mycamera->imageDescription;
					//count++;
				//}
				gettimeofday(&a,NULL);
				totala=a.tv_sec*1000000+a.tv_usec;

				IceUtil::Time t = IceUtil::Time::now();
				reply->timeStamp.seconds = (long)t.toSeconds();
				reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
			    
		    	pthread_mutex_lock (&mycamera->qp->img_mutex);
			    reply->pixelData.resize(mycamera->qp->image->rows*mycamera->qp->image->cols*3);
			    //Copiamos en reply la imagen que se devolverá
			    memcpy( &(reply->pixelData[0]), (unsigned char *) mycamera->qp->image->data,
                                                mycamera->qp->image->rows*mycamera->qp->image->cols*3);
				pthread_mutex_unlock (&mycamera->qp->img_mutex);

			    { //critical region start
				    IceUtil::Mutex::Lock sync(requestsMutex);
				    while(!requests.empty()) {
                        jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front();
                        requests.pop_front();
                        cb->ice_response(reply);
                    }
			    } //critical region end
				gettimeofday(&b,NULL);
				totalb=b.tv_sec*1000000+b.tv_usec;

				diff = (totalb-totala)/1000;
				diff = cycle-diff;

				//std::cout << "Gazeboserver takes " << diff << " ms " << mycamera->fileName << std::endl;
				if(diff < 33)
					diff = 33;

				/*Sleep Algorithm*/
				usleep(diff*1000);
			}
		}
		std::string mFormat;
		CameraI* mycamera;
		IceUtil::Mutex requestsMutex;
		std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
	};
	typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;
	std::string prefix;

	colorspaces::Image::FormatPtr imageFmt;
	jderobot::ImageDescriptionPtr imageDescription;
	jderobot::CameraDescriptionPtr cameraDescription;
	ReplyTaskPtr replyTask;
    QuadrotorPlugin* qp;
    jderobot::ImageFormat mFormats;	
}; // end class CameraI

void* thread_QuadrotorPluginICE ( void* v ) {

    QuadrotorPlugin* quadrotorPlugin = (QuadrotorPlugin*)v;
    char* name = (char*) quadrotorPlugin->cfgfile_QuadrotorPlugin.c_str();
    Ice::CommunicatorPtr ic;
    int argc = 1;
    Ice::PropertiesPtr prop;
    char* argv[] = {name};

    try {
        ic = Ice::initialize(argc, argv);
        prop = ic->getProperties();

        //Interface CMDVel
        std::string cmdvel_name = prop->getProperty("Quadrotor.CMDVel.Name");
        std::string cmdvel_endpoints =
                prop->getProperty("Quadrotor.CMDVel.Endpoints");
        std::cout << "Quadrotor CMDVel Endpoints > " <<
                cmdvel_endpoints << std::endl;
        Ice::ObjectAdapterPtr cmdvel_adapter =
                ic->createObjectAdapterWithEndpoints(
                                "QuadrotorCMDVel",
                                cmdvel_endpoints
                                );
        Ice::ObjectPtr quadrotor = new CMDVelI(quadrotorPlugin);
        cmdvel_adapter->add(quadrotor, ic->stringToIdentity(cmdvel_name));

        //Interface RemoteConfig
        std::string conf_name = prop->getProperty("Quadrotor.Conf.Name");
        std::string conf_endpoints =
                prop->getProperty("Quadrotor.Conf.Endpoints");
        std::cout << "Quadrotor Configuration Endpoints > " <<
                conf_endpoints << std::endl;
        Ice::ObjectAdapterPtr conf_adapter =
                ic->createObjectAdapterWithEndpoints(
                                "QuadrotorConf", conf_endpoints);
        Ice::ObjectPtr quadrotorConf = new RemoteConfigI(quadrotorPlugin);
        conf_adapter->add(quadrotorConf, ic->stringToIdentity(conf_name));

	    //Interface Camera
        std::string camera_name = prop->getProperty("Quadrotor.Camera.Name");
        std::string camera_endpoints =
                prop->getProperty("Quadrotor.Camera.Endpoints");
        std::cout << "Quadrotor Camera Endpoints > " <<
                camera_endpoints << std::endl;
        Ice::ObjectAdapterPtr camera_adapter =
                ic->createObjectAdapterWithEndpoints(
                                "QuadrotorCamera", camera_endpoints);
        Ice::ObjectPtr quadrotorCamera = new CameraI(std::string("CameraGazebo"), quadrotorPlugin);
        camera_adapter->add(quadrotorCamera, ic->stringToIdentity(camera_name));

        //Interface Navdata
		std::string nav_name = prop->getProperty("Quadrotor.Navdata.Name");	
		std::string nav_endpoints =
                prop->getProperty("Quadrotor.Navdata.Endpoints");
        std::cout << "Quadrotor Navdata Endpoints > " <<
                nav_endpoints << std::endl;
		Ice::ObjectAdapterPtr nav_adapter =
                ic->createObjectAdapterWithEndpoints(
                                "QuadrotorNavdata", nav_endpoints);
		Ice::ObjectPtr navO = new navdata::NavdataI();
		nav_adapter->add(navO,ic->stringToIdentity(nav_name));

		//Interface Extra
		std::string extra_name = prop->getProperty("Quadrotor.Extra.Name");		
		std::string extra_endpoints =
                prop->getProperty("Quadrotor.Extra.Endpoints");
        std::cout << "Quadrotor Extra Endpoints > " <<
                extra_endpoints << std::endl;
		Ice::ObjectAdapterPtr extra_adapter =
                ic->createObjectAdapterWithEndpoints(
                                "ArDroneExtra", extra_endpoints);
		Ice::ObjectPtr extraO = new ArDroneExtraI(quadrotorPlugin);
		extra_adapter->add(extraO,ic->stringToIdentity(extra_name));

		nav_adapter->activate();	
		extra_adapter->activate();
        cmdvel_adapter->activate();
        conf_adapter->activate();
        camera_adapter->activate();

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

void *thread_state_controller( void* v ) {
    QuadrotorPlugin *qp = (QuadrotorPlugin*)v;
    qp->RunController();
}

////////////////////////////////////////////////////////////////////////////////
// PID controller implementation
QuadrotorPlugin::PIDController::PIDController() {}

QuadrotorPlugin::PIDController::~PIDController() {}

void QuadrotorPlugin::PIDController::Load(sdf::ElementPtr _sdf,
                                        const std::string& prefix) {
  gain_p = 0.0;
  gain_d = 0.0;
  gain_i = 0.0;
  time_constant = 0.0;
  limit = -1.0;

  if (!_sdf) return;
  // _sdf->PrintDescription(_sdf->GetName());
  if (_sdf->HasElement(prefix + "ProportionalGain"))
    std::istringstream(_sdf->GetElement(prefix + "ProportionalGain")->GetValue()->GetAsString()) >> gain_p;
  if (_sdf->HasElement(prefix + "DifferentialGain"))
    std::istringstream(_sdf->GetElement(prefix + "DifferentialGain")->GetValue()->GetAsString()) >> gain_d;
  if (_sdf->HasElement(prefix + "IntegralGain"))
    std::istringstream(_sdf->GetElement(prefix + "IntegralGain")->GetValue()->GetAsString()) >> gain_i;
  if (_sdf->HasElement(prefix + "TimeConstant"))
    std::istringstream(_sdf->GetElement(
                    prefix + "TimeConstant")->GetValue()->GetAsString()) >> time_constant;
  if (_sdf->HasElement(prefix + "Limit"))
    std::istringstream(_sdf->GetElement(prefix + "Limit")->GetValue()->GetAsString()) >> limit;
}

double QuadrotorPlugin::PIDController::update(double new_input,
                                            double x,
                                            double dx,
                                            double dt) {
  // limit command
  if (limit > 0.0 && fabs(new_input) > limit)
    new_input = (new_input < 0 ? -1.0 : 1.0) * limit;

  // filter command
  if (dt + time_constant > 0.0) {
    dinput = (new_input - input) / (dt + time_constant);
    input  = (dt * new_input + time_constant * input) / (dt + time_constant);
  }
  // update proportional, differential and integral errors
  p = input - x;
  d = dinput - dx;
  i = i + dt * p;

  // update control output
  output = gain_p * p + gain_d * d + gain_i * i;
  return output;
}

void QuadrotorPlugin::PIDController::reset() {
  input = dinput = 0;
  p = i = d = output = 0;
}

} //gazebo
