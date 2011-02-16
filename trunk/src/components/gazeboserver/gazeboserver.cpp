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
 *  11/02/2011   Modified to use Gazebo 0.9
 *  14/01/2011   Modified in order to use Gazebo 0.10
 *  21/03/2010				Initial version. It's an adaptation of cameraserver component
 *  						It only supports 1 camera from gazebo interface.
 *  26/04/2010				Extension with laser interface
 *  15/5/2010				Extension with motors, sonaras, ptmotors, ptencoders, encoders.
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

			replyTask->start();//my own thread
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

					/// Open the Camera interface
					try {
						printf("Connecting to camera device on server...\n");
						gazebocamera->Open (gazeboclient, "pioneer2dx_model1::sonyvid30_model::camera_iface_0");
					} catch (std::string e) {
						std::cout << "Gazebo error: Unable to connect to the camera interface\n" << e << "\n";
						exit (-1);
					}

					gazeboCamData = gazebocamera->data;
					gazeboCamImage = gazeboCamData->image;

					usleep(100000); // para que pille los valores, que parece que le cuesta un tiempo...

					printf("Image width= %d, height= %d\n", gazeboCamData->width, gazeboCamData->height);
				}

				void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
					IceUtil::Mutex::Lock sync(requestsMutex);
					requests.push_back(cb);
				}

				virtual void walk(){
					jderobot::ImageDataPtr reply(new jderobot::ImageData);
					reply->description = mycamera->imageDescription;

					while(!isStopping()){
						IceUtil::Time t = IceUtil::Time::now();
						reply->timeStamp.seconds = (long)t.toSeconds();
						reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
						reply->pixelData.resize(gazeboCamData->width*gazeboCamData->height*3);

						memmove( &(reply->pixelData[0]), gazeboCamImage, mycamera->imageDescription->size);

						{ //critical region start
							IceUtil::Mutex::Lock sync(requestsMutex);
							while(!requests.empty()) {
								jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front();
								requests.pop_front();
								cb->ice_response(reply);
							}
						} //critical region end
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

				/// Open the Position interface
				try {
					gazeboPosition->Open (gazeboclient, "pioneer2dx_model1::position_iface_0");
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

			virtual float getL(const Ice::Current&) {
				return (float)0.0;
			};

			virtual  Ice::Int setL(Ice::Float w, const Ice::Current&) {
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

				/// Open the Simulation Interface
				try {
					gazeboLaser->Open(gazeboclient, "pioneer2dx_model1::laser::laser_iface_0");
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

				gazeboPosition = new gazebo::PositionIface ();

				/// Open the Position interface
				try {
					gazeboPosition->Open (gazeboclient, "pioneer2dx_model1::position_iface_0");
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
				gazeboPositionData = gazeboPosition->data;
				float robotx, roboty, robottheta;
				gazeboPosition->Lock(1);
				encodersData->robotx =
				(gazeboPositionData->pose.pos.x) * 1000 * (float) cos (DEGTORAD * correcting_theta) -
				(gazeboPositionData->pose.pos.y) * 1000 * (float) sin (DEGTORAD * correcting_theta) +
				correcting_x;
				encodersData->roboty =
				(gazeboPositionData->pose.pos.y) * 1000 * (float) cos (DEGTORAD * correcting_theta) +
				(gazeboPositionData->pose.pos.x) * 1000 * (float) sin (DEGTORAD * correcting_theta) +
				correcting_y;
				encodersData->robottheta = (gazeboPositionData->pose.yaw * RADTODEG) + correcting_theta;
				if (encodersData->robottheta <= 0) encodersData->robottheta = encodersData->robottheta + 360;
				else if (encodersData->robottheta > 360) encodersData->robottheta = encodersData->robottheta - 360;
				gazeboPosition->Unlock();

				encodersData->robotcos=cos(gazeboPositionData->pose.yaw);
				encodersData->robotsin=sin(gazeboPositionData->pose.yaw);
				return encodersData;
			};

		private:
			std::string prefix;
			jderobotice::Context context;
			jderobot::EncodersDataPtr encodersData;
			gazebo::Client *gazeboclient;
			int gazeboserver_id;
			int gazeboclient_id;
			gazebo::PositionIface * gazeboPosition;
			gazebo::PositionData * gazeboPositionData;
			/* Variables put to 0.0 and no change during the execution */
			float correcting_x; /* mm */
			float correcting_y; /* mm */
			float correcting_theta; /* deg */
	};
/*
	//PTMOTORS
	class PTMotorsI: virtual public jderobot::PTMotors {
		public:
			PTMotorsI(std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context) {
				Ice::PropertiesPtr prop = context.properties();

				gazeboserver_id=0;
				gazeboclient_id=0;

				// Create a client object
				gazeboclient = new gazebo::Client();

				// Connect to the server
				gazeboclient->ConnectWait(gazeboserver_id, gazeboclient_id);
				gazeboPTZ = new gazebo::PTZIface();
				gazeboPTZ->Open (gazeboclient, "pioneer2dx_model1::sonyvid30_model::ptz_iface_1");
			}

			virtual ~PTMotorsI(){};

			virtual Ice::Int setPTMotorsData(const jderobot::PTMotorsDataPtr & data, const Ice::Current&) {
				//waiting for next gazebo camera update
				gazeboclient->Wait();

				if(!gazeboPTZ){
					printf("Gazebo PTMOTORS model not opened\n");
				}
				gazeboPTZ->Lock(1);
				ptMotorsData->longitude=data->longitude;
				if (data->longitude > MAX_PAN)
					ptMotorsData->longitude=MAX_PAN;
				else if (data->longitude < -54)
					ptMotorsData->longitude= MIN_PAN;

				ptMotorsData->latitude=data->latitude;
				if (data->latitude > MAX_TILT)
					ptMotorsData->latitude= MAX_TILT;
				else if (data->latitude < MIN_TILT)
					ptMotorsData->latitude= MIN_TILT;

				gazeboPTZ->data->cmd_pan=-ptMotorsData->longitude * DEGTORAD;
				gazeboPTZ->data->cmd_tilt=-ptMotorsData->latitude * DEGTORAD;
				gazeboPTZ->Unlock();

				return 0; 
			};

			std::string prefix;
			jderobotice::Context context;
			jderobot::PTMotorsDataPtr ptMotorsData;
			gazebo::Client *gazeboclient;
			int gazeboserver_id;
			int gazeboclient_id;
			gazebo::PTZIface * gazeboPTZ;
	};

	//PTENCODERS
	class PTEncodersI: virtual public jderobot::PTEncoders {
		public:
			PTEncodersI(std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context) {
				Ice::PropertiesPtr prop = context.properties();

				gazeboserver_id=0;
				gazeboclient_id=0;

				// Create a client object
				gazeboclient = new gazebo::Client();

				// Connect to the server
				gazeboclient->ConnectWait(gazeboserver_id, gazeboclient_id);
				gazeboPTZ = new gazebo::PTZIface();
				gazeboPTZ->Open (gazeboclient, "pioneer2dx_model1::sonyvid30_model::ptz_iface_1");
			}

			virtual ~PTEncodersI(){};

			virtual jderobot::PTEncodersDataPtr getPTEncodersData(const Ice::Current&) {
				//waiting for next gazebo camera update
				gazeboclient->Wait ();

				if(!gazeboPTZ){
					printf("Gazebo PTENCODERS model not opened\n");
				}
				gazeboPTZ->Lock(1);
				ptEncodersData->panAngle=-1 * gazeboPTZ->data->pan * RADTODEG;
				ptEncodersData->tiltAngle= -1 * gazeboPTZ->data->tilt * RADTODEG;
				gazeboPTZ->Unlock();

				return ptEncodersData; 
			};

			std::string prefix;
			jderobotice::Context context;
			jderobot::PTEncodersDataPtr ptEncodersData;
			gazebo::Client *gazeboclient;
			int gazeboserver_id;
			int gazeboclient_id;
			gazebo::PTZIface * gazeboPTZ;
	};

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
			:jderobotice::Component("GazeboServer"), cameras(0), motors1(0), laser1(0), encoders1(0), ptmotors1(0), ptencoders1(0), sonars1(0) {}

			virtual void start() {
				Ice::PropertiesPtr prop = context().properties();
				int nCameras = prop->getPropertyAsInt(context().tag() + ".NCameras");
				cameras.resize(nCameras);
				for (int i=0; i<nCameras; i++) {//build camera objects
					std::stringstream objIdS;
					objIdS <<  i;
					std::string objId = objIdS.str();// should this be something unique??
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
/*
				//PTMotors
				std::string objPrefix5="ptmotors1";
				std::string ptmotorsName = "ptmotors1";
				context().tracer().info("Creating ptmotors1 " + ptmotorsName);
				ptmotors1 = new PTMotorsI(objPrefix5,context());
				context().createInterfaceWithString(ptmotors1,ptmotorsName);

				//PTEncoders
				std::string objPrefix6="ptencoders1";
				std::string ptencodersName = "ptencoders1";
				context().tracer().info("Creating ptencoders1 " + ptencodersName);
				ptencoders1 = new PTEncodersI(objPrefix6,context());
				context().createInterfaceWithString(ptencoders1,ptencodersName);

				//Sonars
				std::string objPrefix7="sonars1";
				std::string sonarsName = "sonars1";
				context().tracer().info("Creating sonars1 " + sonarsName);
				sonars1 = new SonarsI(objPrefix7,context());
				context().createInterfaceWithString(sonars1,sonarsName);
			*/

			}

			virtual ~Component(){}

		private:
			std::vector<Ice::ObjectPtr> cameras;
			Ice::ObjectPtr motors1;
			Ice::ObjectPtr laser1;
			Ice::ObjectPtr encoders1;
			Ice::ObjectPtr ptmotors1;
			Ice::ObjectPtr ptencoders1;
			Ice::ObjectPtr sonars1;
	};
} // end namespace

int main(int argc, char** argv) {
	gazeboserver::Component component;
	jderobotice::Application app(component);
	return app.jderobotMain(argc,argv);
}
