/*
 *
 *  Copyright (C) 1997-2011 JDERobot Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Authors : Julio Vega <julio.vega@urjc.es>
 *            Javier Vazquez Pereda <javiervazper@yahoo.es>
 *            
 *
 *  DATE					COMMENT
 *  04/11/2011   v 3.1: Fix bug on PTEncoders class
 *  01/03/2011   v 3.0: Modified to use PTEncoders y PTMotors
 *  11/02/2011   v 2.0: Modified to use Gazebo 0.9
 *  14/01/2011   v 1.0: Modified in order to use Gazebo 0.10
 *  21/03/2010   Initial version. It's an adaptation of cameraserver component
 *  						 It only supports 1 camera from gazebo interface.
 *  26/04/2010   Extension with laser interface
 *  15/5/2010    Extension with motors, sonaras, ptmotors, ptencoders, encoders.
 *
 */

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <gbxsickacfr/gbxiceutilacfr/safethread.h>
#include <jderobot/camera.h>
#include <colorspaces/colorspacesmm.h>
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <tr1/memory>
#include <list>
#include <sstream>
#include <jderobotice/exceptions.h>
#include <jderobot/motors.h>
#include <jderobot/laser.h>
#include <jderobot/encoders.h>
#include <jderobot/ptencoders.h>
#include <jderobot/ptmotors.h>
#include <jderobot/sonars.h>
#include <math.h>
#include <gazebo/gazebo.h>

#define NUM_LASER 180
#define DEGTORAD 0.01745327
#define RADTODEG 57.29582790
#define MAX_PAN 54 /** gazebo pantilt max pan, degrees.*/
#define MIN_PAN -54 /** gazebo pantilt min pan, degrees.*/
#define MAX_TILT 44 /** gazebo pantilt max tilt, degrees.*/
#define MIN_TILT -44 /** gazebo pantilt min tilt, degrees.*/

namespace gazeboserver {
	class CameraI: virtual public jderobot::Camera {
	public:
		CameraI(std::string& propertyPrefix, const jderobotice::Context& context)
		: prefix(propertyPrefix),context(context),
		imageFmt(),
		imageDescription(new jderobot::ImageDescription()),
		cameraDescription(new jderobot::CameraDescription()),

		replyTask() {
			Ice::PropertiesPtr prop = context.properties();

			//fill cameraDescription
			cameraDescription->name = prop->getProperty(prefix+"Name");
			if (cameraDescription->name.size() == 0)
			throw jderobotice::ConfigFileException(ERROR_INFO,"Camera name not configured");
			cameraDescription->shortDescription = prop->getProperty(prefix+"ShortDescription");

			//fill imageDescription
			imageDescription->width = prop->getPropertyAsIntWithDefault(prefix+"ImageWidth",340);
			imageDescription->height = prop->getPropertyAsIntWithDefault(prefix+"ImageHeight",280);

			//we use formats acording to colorspaces
			std::string fmtStr = prop->getPropertyWithDefault(prefix+"Format","YUY2");//default format YUY2
			imageFmt = colorspaces::Image::Format::searchFormat(fmtStr);
			if (!imageFmt)
			throw jderobotice::ConfigFileException(ERROR_INFO, "Format " + fmtStr + " unknown");
			imageDescription->size = imageDescription->width * imageDescription->height * CV_ELEM_SIZE(imageFmt->cvType);
			imageDescription->format = imageFmt->name;

			context.tracer().info("Starting thread for camera: " + cameraDescription->name);
			replyTask = new ReplyTask(this);

			replyTask->start(); // my own thread
		}

		std::string getName () {
			return (cameraDescription->name);
		}

		void prepare2draw (unsigned char** stream) {

		}

		std::string getRobotName () {
			return ((context.properties())->getProperty(context.tag()+".RobotName"));
		}

		virtual ~CameraI() {
			context.tracer().info("Stopping and joining thread for camera: " + cameraDescription->name);
			gbxiceutilacfr::stopAndJoin(replyTask);
		}

		virtual jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c){
			return imageDescription;
		}

		virtual jderobot::CameraDescriptionPtr getCameraDescription(const Ice::Current& c){
			return cameraDescription;
		}

		virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr &description, const Ice::Current& c) {
			return 0;
		}

		virtual void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb,const Ice::Current& c){
			replyTask->pushJob(cb);
		}

		virtual std::string startCameraStreaming(const Ice::Current&){
			context.tracer().info("Should be made anything to start camera streaming: " + cameraDescription->name);
		}

		virtual void stopCameraStreaming(const Ice::Current&) {
			context.tracer().info("Should be made anything to stop camera streaming: " + cameraDescription->name);
		}

	private:
		class ReplyTask: public gbxiceutilacfr::SafeThread {
			public:
				ReplyTask(CameraI* camera)
				: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycamera(camera) {
					gazeboserver_id=0;
					gazeboclient_id=0;
					// Create a client object
					gazeboclient = new gazebo::Client();

					/// Connect to the libgazebo server
					try {
						gazeboclient->ConnectWait(gazeboserver_id, gazeboclient_id);
					}	catch (std::string e) {
						std::cout << "Gazebo error: Unable to connect\n" << e << "\n";
						exit (-1);
					}

					gazebocameraSim = new gazebo::SimulationIface();

					/// Open the Simulation Interface
					try {
						gazebocameraSim->Open(gazeboclient, "default");
					}	catch (std::string e) {
						std::cout << "Gazebo error: Unable to connect to the sim interface\n" << e << "\n";
						exit (-1);
					}

					gazebocamera = new gazebo::CameraIface();
					std::string myRobot = camera->getRobotName();
					std::string myIface = "pioneer2dx_" + myRobot + "::sonyvid30_model_" + camera->getName () + "::camera_iface_0";

					/// Open the Camera interface
					do {
						try {
							printf("Connecting to camera device on server...\n");
							gazebocamera->Open (gazeboclient, myIface);
						} catch (std::string e) {
							std::cout << "Gazebo error: Unable to connect to the camera interface\n" << e << "\n";
							exit (-1);
						}
						if ((gazebocamera->data->width == 0) || (gazebocamera->data->height == 0))
							usleep(10000);
					} while ((gazebocamera->data->width == 0) || (gazebocamera->data->height == 0));

					gazeboCamData = gazebocamera->data;
					gazeboCamImage = gazeboCamData->image;

					printf("Image width= %d, height= %d\n", gazeboCamData->width, gazeboCamData->height);
				}

				void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
					IceUtil::Mutex::Lock sync(requestsMutex);
					requests.push_back(cb);
				}

				virtual void walk(){
					jderobot::ImageDataPtr reply(new jderobot::ImageData);
					reply->description = mycamera->imageDescription;
					struct timeval a, b;
					int cycle = 50;
					long totalb,totala;
					long diff;

					while(!isStopping()){
						gettimeofday(&a,NULL);
						totala=a.tv_sec*1000000+a.tv_usec;

						IceUtil::Time t = IceUtil::Time::now();
						reply->timeStamp.seconds = (long)t.toSeconds();
						reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
						reply->pixelData.resize(gazeboCamData->width*gazeboCamData->height*3);

						memmove (&(reply->pixelData[0]), gazeboCamImage, mycamera->imageDescription->size);

						int i;
						int pos;
						char temp;

						for (i=0; i<gazeboCamData->width*gazeboCamData->height; i++) {
							pos = i*3;
							temp = reply->pixelData[pos+2];
							reply->pixelData[pos+2]=reply->pixelData[pos];
							reply->pixelData[pos]=temp;
						}

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

						//std::cout << "Gazeboserver takes " << diff << " ms" << std::endl;

						if(diff < 10)
							diff = 10;

						/*Sleep Algorithm*/
						usleep(diff*1000);
					}
				}

				CameraI* mycamera;
				IceUtil::Mutex requestsMutex;
				std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
				gazebo::Client *gazeboclient;
				int gazeboserver_id;
				int gazeboclient_id;
				gazebo::CameraIface *gazebocamera;
			  gazebo::SimulationIface *gazebocameraSim;
				gazebo::CameraData *gazeboCamData;
				unsigned char *gazeboCamImage;
		};

		typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;
		std::string prefix;
		jderobotice::Context context;
		colorspaces::Image::FormatPtr imageFmt;
		jderobot::ImageDescriptionPtr imageDescription;
		jderobot::CameraDescriptionPtr cameraDescription;
		ReplyTaskPtr replyTask;
	}; // end class CameraI

	class MotorsI: virtual public jderobot::Motors {
		public:
			MotorsI(std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context) {
				Ice::PropertiesPtr prop = context.properties();

				gazeboserver_id=0;
				gazeboclient_id=0;

				// Create a client object
				gazeboclient = new gazebo::Client();

				/// Connect to the libgazebo server
				try {
					gazeboclient->ConnectWait(gazeboserver_id, gazeboclient_id);
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect\n" << e << "\n";
					exit (-1);
				}

				gazeboSim = new gazebo::SimulationIface();

				/// Open the Simulation Interface
				try {
					gazeboSim->Open(gazeboclient, "default");
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect to the sim interface\n" << e << "\n";
					exit (-1);
				}

				gazeboPosition = new gazebo::PositionIface ();
				std::string myRobot = prop->getProperty(context.tag()+".RobotName");
				std::string myIface = "pioneer2dx_" + myRobot + "::position_iface_0";

				/// Open the Position interface
				try {
					gazeboPosition->Open (gazeboclient, myIface);
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect to the position interface\n" << e << "\n";
					exit (-1);
				}
			}

			virtual ~MotorsI(){};

			virtual float getV(const Ice::Current&) {
				//waiting for next gazebo camera update
				gazeboclient->Wait ();

				if(!gazeboPosition){
					printf("Gazebo Position model not opened\n");
				}
				gazeboPositionData = gazeboPosition->data;

				double v_double;
				gazeboPosition->Lock(1);
				v_double = gazeboPositionData->velocity.pos.x;
				gazeboPosition->Unlock();
				return (float)v_double;
			};

			virtual float getW(const Ice::Current&) {
				//waiting for next gazebo camera update
				gazeboclient->Wait ();

				if(!gazeboPosition){
					printf("Gazebo Position model not opened\n");
				}
				gazeboPositionData = gazeboPosition->data;

				double w_double;
				gazeboPosition->Lock(1);
				w_double = gazeboPositionData->velocity.pos.y;
				gazeboPosition->Unlock();
				return (float)w_double;
			};

			virtual float getL(const Ice::Current&) {
				return 0.;
			};

			virtual  Ice::Int setV(Ice::Float v, const Ice::Current&) {
				gazeboclient->Wait ();

				if(!gazeboPosition) {
					printf("Gazebo Position model not opened\n");
				}
				gazeboPositionData = gazeboPosition->data;

				gazeboPosition->Lock(1);
				gazeboPositionData->cmdEnableMotors = 1;
				gazeboPosition->Unlock();

				gazeboPosition->Lock(1);
				// jderobot trabaja con mm/s, gazebo con m/s
				gazeboPositionData->cmdVelocity.pos.x = (v/1000);
				gazeboPosition->Unlock();

				return 0;
			};

			virtual  Ice::Int setW(Ice::Float w, const Ice::Current&) {
				gazeboclient->Wait ();

				if(!gazeboPosition) {
					printf("Gazebo Position model not opened\n");
				}
				gazeboPositionData = gazeboPosition->data;

				gazeboPosition->Lock(1);
				gazeboPositionData->cmdEnableMotors = 1;
				gazeboPosition->Unlock();

				gazeboPosition->Lock(1);
				gazeboPositionData->cmdVelocity.yaw=w*(3.14159265/180);
				gazeboPosition->Unlock();
				return 0;
			};

			virtual  Ice::Int setL(Ice::Float l, const Ice::Current&) {
				return 0;
			};

			std::string prefix;
			jderobotice::Context context;

			gazebo::Client *gazeboclient;
			int gazeboserver_id;
			int gazeboclient_id;
			gazebo::PositionIface * gazeboPosition;
		  gazebo::SimulationIface *gazeboSim;
			gazebo::PositionData * gazeboPositionData;
	}; // end class MotorsI


	class LaserI: virtual public jderobot::Laser {
		public:
			LaserI(std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context),
			laserData(new jderobot::LaserData()) {
				Ice::PropertiesPtr prop = context.properties();

				gazeboserver_id=0;
				gazeboclient_id=0;

				// Create a client object
				gazeboclient = new gazebo::Client();

				/// Connect to the libgazebo server
				try {
					gazeboclient->ConnectWait(gazeboserver_id, gazeboclient_id);
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect\n" << e << "\n";
					exit (-1);
				}

				gazeboLaserSim = new gazebo::SimulationIface();

				/// Open the Simulation Interface
				try {
					gazeboLaserSim->Open(gazeboclient, "default");
				}	catch (std::string e)	{
					std::cout << "Gazebo error: Unable to connect to the sim interface\n" << e << "\n";
					exit (-1);
				}

				gazeboLaser = new gazebo::LaserIface ();
				std::string myRobot = prop->getProperty(context.tag()+".RobotName");
				std::string myIface = "pioneer2dx_" + myRobot + "::laser::laser_iface_0";

				/// Open the Position interface
				try {
					gazeboLaser->Open (gazeboclient, myIface);
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect to the laser interface\n" << e << "\n";
					exit (-1);
				}

				laserData->numLaser=NUM_LASER;
				laserData->distanceData.resize(sizeof(int)*laserData->numLaser);
			}

			virtual ~LaserI(){};

			virtual jderobot::LaserDataPtr getLaserData(const Ice::Current&) {
				double relation; //(gazebo ray number / jde rays number)
				int cont2;
				int angle=0;
				int laserValue;

				gazeboclient->Wait ();

				if(!gazeboLaser){
					printf("Gazebo Laser model not opened\n");
				}

				gazeboLaser->Lock(1);
				relation=(float)gazeboLaser->data->range_count /(float)NUM_LASER;
				for (angle=0;angle<laserData->numLaser;angle++){
					cont2 = rint(relation * angle);
					laserData->distanceData[angle]=(int) (gazeboLaser->data->ranges[cont2] * 1000);
				}
				gazeboLaser->Unlock();
				return laserData;
			};

			private:
			std::string prefix;
			jderobotice::Context context;
			jderobot::LaserDataPtr laserData;

			gazebo::Client *gazeboclient;
			int gazeboserver_id;
			int gazeboclient_id;
			gazebo::LaserIface *gazeboLaser;
		  gazebo::SimulationIface *gazeboLaserSim;
	};

	class EncodersI: virtual public jderobot::Encoders {
		public:
			EncodersI(std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context),
			encodersData(new jderobot::EncodersData()) {
				Ice::PropertiesPtr prop = context.properties();

				gazeboserver_id=0;
				gazeboclient_id=0;

				// Create a client object
				gazeboclient = new gazebo::Client();

				/// Connect to the libgazebo server
				try {
					gazeboclient->ConnectWait(gazeboserver_id, gazeboclient_id);
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect\n" << e << "\n";
					exit (-1);
				}

				gazeboEncodersSim = new gazebo::SimulationIface();

				/// Open the Simulation Interface
				try {
					gazeboEncodersSim->Open(gazeboclient, "default");
				}	catch (std::string e)	{
					std::cout << "Gazebo error: Unable to connect to the sim interface\n" << e << "\n";
					exit (-1);
				}

				gazeboPosition = new gazebo::PositionIface ();
				std::string myRobot = prop->getProperty(context.tag()+".RobotName");
				std::string myIface = "pioneer2dx_" + myRobot + "::position_iface_0";

				/// Open the Position interface
				try {
					gazeboPosition->Open (gazeboclient, myIface);
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect to the position interface\n" << e << "\n";
					exit (-1);
				}

				correcting_x = 0.; /* mm */
				correcting_y = 0.; /* mm */
				correcting_theta = 0.; /* deg */
			}

			virtual ~EncodersI(){};

			virtual jderobot::EncodersDataPtr getEncodersData(const Ice::Current&) {
				gazeboclient->Wait ();

				if(!gazeboPosition){
					printf("Gazebo Position model not opened\n");
				}

				Ice::PropertiesPtr prop = context.properties();
                std::string myRobot = "pioneer2dx_" + prop->getProperty(context.tag()+".RobotName");

				gazebo::Pose myPose;
				gazeboEncodersSim->GetPose3d(myRobot.data(), myPose);

				float robotx, roboty, robottheta;
				gazeboPosition->Lock(1);
				encodersData->robotx =
				(myPose.pos.x) * 1000 * (float) cos (DEGTORAD * correcting_theta) -
				(myPose.pos.y) * 1000 * (float) sin (DEGTORAD * correcting_theta) +
				correcting_x;
				encodersData->roboty =
				(myPose.pos.y) * 1000 * (float) cos (DEGTORAD * correcting_theta) +
				(myPose.pos.x) * 1000 * (float) sin (DEGTORAD * correcting_theta) +
				correcting_y;
				encodersData->robottheta = (myPose.yaw * RADTODEG) + correcting_theta;
				if (encodersData->robottheta <= 0) encodersData->robottheta = encodersData->robottheta + 360;
				else if (encodersData->robottheta > 360) encodersData->robottheta = encodersData->robottheta - 360;
				gazeboPosition->Unlock();

				encodersData->robotcos=cos(myPose.yaw);
				encodersData->robotsin=sin(myPose.yaw);

				return encodersData;
			};

		private:
			std::string prefix;
			jderobotice::Context context;
			jderobot::EncodersDataPtr encodersData;
			gazebo::Client *gazeboclient;
			int gazeboserver_id;
			int gazeboclient_id;
			gazebo::PositionIface *gazeboPosition;
			/* Variables put to 0.0 and no change during the execution */
			float correcting_x; /* mm */
			float correcting_y; /* mm */
			float correcting_theta; /* deg */
			gazebo::SimulationIface *gazeboEncodersSim;
	};

	//PTMOTORS
	class PTMotorsI: virtual public jderobot::PTMotors {
		public:
			PTMotorsI(int index, std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context) {
				Ice::PropertiesPtr prop = context.properties();

				gazeboserver_id=0;
				gazeboclient_id=0;

				// Create a client object
				gazeboclient = new gazebo::Client();

				/// Connect to the libgazebo server
				try {
					gazeboclient->ConnectWait(gazeboserver_id, gazeboclient_id);
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect\n" << e << "\n";
					exit (-1);
				}

				gazeboSim = new gazebo::SimulationIface();

				/// Open the Simulation Interface
				try {
					gazeboSim->Open(gazeboclient, "default");
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect to the sim interface\n" << e << "\n";
					exit (-1);
				}

				gazeboPTZ1 = new gazebo::PTZIface();
				std::string myRobot1 = prop->getProperty(context.tag()+".RobotName");
								std::stringstream scamera;
				scamera << context.tag()+".Camera." << index << ".Name";
				std::string camera = prop->getProperty(scamera.str());
				std::string myIface1 = "pioneer2dx_" + myRobot1 + "::sonyvid30_model_"+ camera + "::ptz_iface_1";

				/// Open the Position interface
				try {
					gazeboPTZ1->Open (gazeboclient, myIface1);
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect to the position interface\n" << e << "\n";
					exit (-1);
				}
			}

			~PTMotorsI(){};

			virtual Ice::Int setPTMotorsData(const jderobot::PTMotorsDataPtr & data, const Ice::Current&) {
				//waiting for next gazebo camera update
				gazeboclient->Wait();

				if(!gazeboPTZ1){
					printf("Gazebo PTMOTORS model not opened\n");
				}

				gazeboPTZ1->Lock(1);
				gazeboPTZ1->data->control_mode = GAZEBO_PTZ_POSITION_CONTROL;
				gazeboPTZ1->data->cmd_pan = data->longitude * DEGTORAD;
				gazeboPTZ1->data->cmd_tilt = data->latitude * DEGTORAD;
				gazeboPTZ1->Unlock();

				return 0; 
			};

			virtual jderobot::PTMotorsDataPtr getPTMotorsData(const Ice::Current&) {
				return (NULL);
			};

			virtual jderobot::PTMotorsParamsPtr getPTMotorsParams(const Ice::Current&) {
				return (NULL);
			};

			std::string prefix;
			jderobotice::Context context;
			gazebo::Client *gazeboclient;
			int gazeboserver_id;
			int gazeboclient_id;
			gazebo::PTZIface * gazeboPTZ1;
		  gazebo::SimulationIface *gazeboSim;
	};

	class PTMotorsII: virtual public jderobot::PTMotors {
		public:
			PTMotorsII(int index, std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context) {
				Ice::PropertiesPtr prop = context.properties();

				gazeboserver_id=0;
				gazeboclient_id=0;

				// Create a client object
				gazeboclient = new gazebo::Client();

				/// Connect to the libgazebo server
				try {
					gazeboclient->ConnectWait(gazeboserver_id, gazeboclient_id);
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect\n" << e << "\n";
					exit (-1);
				}

				gazeboSim = new gazebo::SimulationIface();

				/// Open the Simulation Interface
				try {
					gazeboSim->Open(gazeboclient, "default");
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect to the sim interface\n" << e << "\n";
					exit (-1);
				}

				gazeboPTZ2 = new gazebo::PTZIface();
				std::string myRobot2 = prop->getProperty(context.tag()+".RobotName");
								std::stringstream scamera;
				scamera << context.tag()+".Camera." << index << ".Name";
				std::string camera = prop->getProperty(scamera.str());
				std::string myIface2 = "pioneer2dx_" + myRobot2 + "::sonyvid30_model_"+ camera +"::ptz_iface_1";

				/// Open the Position interface
				try {
					gazeboPTZ2->Open (gazeboclient, myIface2);
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect to the position interface\n" << e << "\n";
					exit (-1);
				}
			}

			virtual ~PTMotorsII(){};

			virtual Ice::Int setPTMotorsData(const jderobot::PTMotorsDataPtr & data, const Ice::Current&) {
				//waiting for next gazebo camera update
				gazeboclient->Wait();

				if(!gazeboPTZ2){
					printf("Gazebo PTMOTORS model not opened\n");
				}

				gazeboPTZ2->Lock(1);
				gazeboPTZ2->data->control_mode = GAZEBO_PTZ_POSITION_CONTROL;
				gazeboPTZ2->data->cmd_pan = data->longitude * DEGTORAD;
				gazeboPTZ2->data->cmd_tilt = data->latitude * DEGTORAD;
				gazeboPTZ2->Unlock();

				return 0; 
			};

			virtual jderobot::PTMotorsDataPtr getPTMotorsData(const Ice::Current&) {
				return (NULL);
			};

			virtual jderobot::PTMotorsParamsPtr getPTMotorsParams(const Ice::Current&) {
				return (NULL);
			};

			std::string prefix;
			jderobotice::Context context;
			gazebo::Client *gazeboclient;
			int gazeboserver_id;
			int gazeboclient_id;
			gazebo::PTZIface * gazeboPTZ2;
		  gazebo::SimulationIface *gazeboSim;
	};

	//PTENCODERSI
	class PTEncodersI: virtual public jderobot::PTEncoders {
		public:
			PTEncodersI(int index ,std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context),
			ptEncodersData1(new jderobot::PTEncodersData()) {
				Ice::PropertiesPtr prop = context.properties();

				gazeboserver_id=0;
				gazeboclient_id=0;

				// Create a client object
				gazeboclient = new gazebo::Client();

				/// Connect to the libgazebo server
				try {
					gazeboclient->ConnectWait(gazeboserver_id, gazeboclient_id);
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect\n" << e << "\n";
					exit (-1);
				}

				gazeboPTZ1 = new gazebo::PTZIface();
				std::string myRobot1 = prop->getProperty(context.tag()+".RobotName");
				std::stringstream scamera;
				scamera << context.tag()+".Camera." << index << ".Name";
				std::string camera = prop->getProperty(scamera.str());
				std::string myIface1 = "pioneer2dx_" + myRobot1 + "::sonyvid30_model_"+ camera +"::ptz_iface_1";

				/// Open the Position interface
				try {
					gazeboPTZ1->Open (gazeboclient, myIface1);
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect to the position interface\n" << e << "\n";
					exit (-1);
				}
			}

			virtual ~PTEncodersI(){};

			virtual jderobot::PTEncodersDataPtr getPTEncodersData(const Ice::Current&) {
				//waiting for next gazebo camera update
				gazeboclient->Wait ();

				if(!gazeboPTZ1){
					printf("Gazebo PTENCODERS model not opened\n");
				}

				gazeboPTZ1->Lock(1);
				ptEncodersData1->panAngle=gazeboPTZ1->data->pan * RADTODEG;
				ptEncodersData1->tiltAngle=gazeboPTZ1->data->tilt * RADTODEG;
				gazeboPTZ1->Unlock();

				return ptEncodersData1; 
			};

			std::string prefix;
			jderobotice::Context context;
			jderobot::PTEncodersDataPtr ptEncodersData1;
			gazebo::Client *gazeboclient;
			int gazeboserver_id;
			int gazeboclient_id;
			gazebo::PTZIface * gazeboPTZ1;
	};

	//PTENCODERSII
	class PTEncodersII: virtual public jderobot::PTEncoders {
		public:
			PTEncodersII(int index, std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context),
			ptEncodersData2(new jderobot::PTEncodersData()) {
				Ice::PropertiesPtr prop = context.properties();

				gazeboserver_id=0;
				gazeboclient_id=0;

				// Create a client object
				gazeboclient = new gazebo::Client();

				/// Connect to the libgazebo server
				try {
					gazeboclient->ConnectWait(gazeboserver_id, gazeboclient_id);
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect\n" << e << "\n";
					exit (-1);
				}

				gazeboPTZ2 = new gazebo::PTZIface();
				std::string myRobot2 = prop->getProperty(context.tag()+".RobotName");
				std::stringstream scamera;
				scamera << context.tag()+".Camera." << index << ".Name";
				std::string camera = prop->getProperty(scamera.str());
				std::string myIface2 = "pioneer2dx_" + myRobot2 + "::sonyvid30_model_" + camera + "::ptz_iface_1";

				/// Open the Position interface
				try {
					gazeboPTZ2->Open (gazeboclient, myIface2);
				} catch (std::string e) {
					std::cout << "Gazebo error: Unable to connect to the position interface\n" << e << "\n";
					exit (-1);
				}
			}

			virtual ~PTEncodersII(){};

			virtual jderobot::PTEncodersDataPtr getPTEncodersData(const Ice::Current&) {
				//waiting for next gazebo camera update
				gazeboclient->Wait ();

				if(!gazeboPTZ2){
					printf("Gazebo PTENCODERS model not opened\n");
				}
				gazeboPTZ2->Lock(1);
				ptEncodersData2->panAngle=gazeboPTZ2->data->pan * RADTODEG;
				ptEncodersData2->tiltAngle=gazeboPTZ2->data->tilt * RADTODEG;
				gazeboPTZ2->Unlock();

				return ptEncodersData2;
			};

			std::string prefix;
			jderobotice::Context context;
			jderobot::PTEncodersDataPtr ptEncodersData2;
			gazebo::Client *gazeboclient;
			int gazeboserver_id;
			int gazeboclient_id;
			gazebo::PTZIface * gazeboPTZ2;
	};
/*
	//SONARS
	class SonarsI: virtual public jderobot::Sonars {
		public:
			SonarsI(std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context) {
				Ice::PropertiesPtr prop = context.properties();

				gazeboserver_id=0;
				gazeboclient_id=0;

				// Create a client object
				gazeboclient = new gazebo::Client();

				// Connect to the server
				gazeboclient->ConnectWait(gazeboserver_id, gazeboclient_id);
				gazeboSonar = new gazebo::FiducialIface();
				gazeboSonar->Open (gazeboclient, "robot1");

				sonarsData->numSonars=NUM_SONARS;
				sonarsData->us.resize(sizeof(int)*sonarsData->numSonars);
			}

			virtual ~SonarsI(){};

			virtual jderobot::SonarsDataPtr getSonarsData(const Ice::Current&) {
				//waiting for next gazebo camera update
				int j;
				gazeboclient->Wait();

				if(!gazeboSonar){
					printf("Gazebo SONARS model not opened\n");
				}
				gazeboSonar->Lock(1);
				for (j = 0; j < NUM_SONARS; j++) {
					sonarsData->us[j] = (int) gazeboSonar->data->fids[j].pose.pos.x*1000;
				}
				gazeboSonar->Unlock();
				return sonarsData; 
			};

			std::string prefix;
			jderobotice::Context context;
			jderobot::SonarsDataPtr sonarsData;
			gazebo::Client *gazeboclient;
			int gazeboserver_id;
			int gazeboclient_id;
			gazebo::FiducialIface * gazeboSonar;
	};
*/
	//COMPONENT
	class Component: public jderobotice::Component{
		public:
			Component()
			:jderobotice::Component("GazeboServer"), cameras(0), motors1(0), laser1(0), encoders1(0), ptmotors1(0), ptmotors2(0), ptencoders1(0), ptencoders2(0), sonars1(0) {}

			virtual void start() {
				//Cameras
				Ice::PropertiesPtr prop = context().properties();
				int nCameras = prop->getPropertyAsInt(context().tag() + ".NCameras");
				cameras.resize(nCameras);
				for (int i=0; i<nCameras; i++) {//build camera objects
					std::stringstream objIdS;
					objIdS <<  i;
					std::string objId = objIdS.str();
					std::string objPrefix(context().tag() + ".Camera." + objId + ".");
					std::string cameraName = prop->getProperty(objPrefix + "Name");

					if (cameraName.size() == 0) { //no name specified, we create one using the index
						cameraName = "camera" + objId;
						prop->setProperty(objPrefix + "Name",cameraName);//set the value
					}
					context().tracer().info("Creating camera " + cameraName);
					cameras[i] = new CameraI(objPrefix,context());
					context().createInterfaceWithString(cameras[i],cameraName);
				}

				//Motors
				std::string objPrefix2="motors1";
				std::string gazeboactName = "motors1";
				context().tracer().info("Creating motors1 " + gazeboactName);
				motors1 = new MotorsI(objPrefix2,context());
				context().createInterfaceWithString(motors1,gazeboactName);

				//Laser
				std::string objPrefix3="laser1";
				std::string laserName = "laser1";
				context().tracer().info("Creating laser1 " + laserName);
				laser1 = new LaserI(objPrefix3,context());
				context().createInterfaceWithString(laser1,laserName);
				
				//Encoders
				std::string objPrefix4="encoders1";
				std::string encodersName = "encoders1";
				context().tracer().info("Creating encoders1 " + encodersName);
				encoders1 = new EncodersI(objPrefix4,context());
				context().createInterfaceWithString(encoders1,encodersName);

				//PTMotorsI
				std::string objPrefix5="ptmotors1";
				std::string ptmotorsName1 = "ptmotors1";
				context().tracer().info("Creating ptmotors1 " + ptmotorsName1);
				ptmotors1 = new PTMotorsI( 0, objPrefix5,context());
				context().createInterfaceWithString(ptmotors1,ptmotorsName1);

				//PTMotorsII
				std::string objPrefix6="ptmotors2";
				std::string ptmotorsName2 = "ptmotors2";
				context().tracer().info("Creating ptmotors2 " + ptmotorsName2);
				ptmotors2 = new PTMotorsII(1, objPrefix6, context());
				context().createInterfaceWithString(ptmotors2,ptmotorsName2);

				//PTEncodersI
				std::string objPrefix7="ptencoders1";
				std::string ptencodersName1 = "ptencoders1";
				context().tracer().info("Creating ptencoders1 " + ptencodersName1);
				ptencoders1 = new PTEncodersI(0, objPrefix7, context());
				context().createInterfaceWithString(ptencoders1,ptencodersName1);

				//PTEncodersII
				std::string objPrefix8="ptencoders2";
				std::string ptencodersName2 = "ptencoders2";
				context().tracer().info("Creating ptencoders2 " + ptencodersName2);
				ptencoders2 = new PTEncodersII(1, objPrefix8, context());
				context().createInterfaceWithString(ptencoders2,ptencodersName2);
/*
				//Sonars
				std::string objPrefix7="sonars1";
				std::string sonarsName = "sonars1";
				context().tracer().info("Creating sonars1 " + sonarsName);
				sonars1 = new SonarsI(objPrefix7,context());
				context().createInterfaceWithString(sonars1,sonarsName);
			*/
				usleep(1000); // para que pille los valores, que parece que le cuesta un tiempo...
			}

			virtual ~Component(){}

		private:
			std::vector<Ice::ObjectPtr> cameras;
			Ice::ObjectPtr motors1;
			Ice::ObjectPtr laser1;
			Ice::ObjectPtr encoders1;
			Ice::ObjectPtr ptmotors1;
			Ice::ObjectPtr ptmotors2;
			Ice::ObjectPtr ptencoders1;
			Ice::ObjectPtr ptencoders2;
			Ice::ObjectPtr sonars1;
	};
} // end namespace

int main(int argc, char** argv) {
	gazeboserver::Component component;
	jderobotice::Application app(component);
	return app.jderobotMain(argc,argv);
}
