/*
 *
 *  Copyright (C) 1997-2010 JDERobot Developers Team
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
 *  Authors : Julio Vega  <julio.vega@urjc.es>
 *						Eduardo Perdices  <eperdices@gsyc.es>
 *
 */

// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <gbxsickacfr/gbxiceutilacfr/safethread.h>

// JDErobot general ice component includes
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <tr1/memory>
#include <list>
#include <sstream>
#include <jderobotice/exceptions.h>

// JDErobot specific ice component includes
#include <jderobot/laser.h>
#include <jderobot/encoders.h>
#include <jderobot/motors.h>
#include <jderobot/camera.h>
#include <jderobot/ptencoders.h>
#include <jderobot/ptmotors.h>
#include <jderobot/pose3dmotors.h>
#include <jderobot/pose3dencoders.h>
#include <gazebo/gazebo.h>

#include <colorspaces/colorspacesmm.h>

#include <iostream>
#include <fstream>

// Library includes
#include <math.h>

// opencv Libraries
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


// Constants
#define DEGTORAD 0.01745327
#define RADTODEG 57.29582790


using namespace std;

// Global variables
string robotName;
string robotPort;

namespace playerserver {

		class CameraI: virtual public jderobot::Camera {
	public:
		CameraI(std::string& propertyPrefix, const jderobotice::Context& context)
		: prefix(propertyPrefix),context(context),

		replyTask() {
		
		   std::cout << "Constructor" << endl;
		   
		   imageDescription = (new jderobot::ImageDescription());
		   cameraDescription = (new jderobot::CameraDescription());
		
			replyTask = new ReplyTask(this);
		
         startThread = false;
         fileName.clear();
		}

		std::string getName () {
			return (cameraDescription->name);
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
		virtual void update(std::string s){
         fileName = s;
         //std::cout << "update" << endl;
         
         if(!startThread){
            startThread = true;
			   replyTask->start(); // my own thread
		   }
	   }	

	private:
		class ReplyTask: public gbxiceutilacfr::SafeThread {
			public:
				ReplyTask(CameraI* camera)
				: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycamera(camera) {
				   cout << "safeThread" <<endl;
				}

				void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
					IceUtil::Mutex::Lock sync(requestsMutex);
					requests.push_back(cb);
				}

				virtual void walk(){
               cv::Mat image = cv::imread(mycamera->fileName);
               mycamera->imageDescription->width = image.cols;
               mycamera->imageDescription->height = image.rows;
               mycamera->imageDescription->size = image.cols*image.rows*3;
               mycamera->imageDescription->format = "RGB8";
				
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
                  
					   reply->pixelData.resize(image.rows*image.cols*3);

					   memcpy( &(reply->pixelData[0]), (unsigned char *) image.data, image.rows*image.cols*3);

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

						//sstd::cout << "Gazeboserver takes " << diff << " ms" << std::endl;

						if(diff < 33)
							diff = 33;


						/*Sleep Algorithm*/
						usleep(diff*1000);
					}
				}

				CameraI* mycamera;
				IceUtil::Mutex requestsMutex;
				std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
		};

		typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;
		std::string prefix;
		jderobotice::Context context;
		colorspaces::Image::FormatPtr imageFmt;
		jderobot::ImageDescriptionPtr imageDescription;
		jderobot::CameraDescriptionPtr cameraDescription;
		ReplyTaskPtr replyTask;
		std::string fileName;
		bool startThread;
	}; // end class CameraI
	
	




   /////////////////////////// MOTORS ////////////////////////////////

	class MotorsI: virtual public jderobot::Motors {
		public:
			MotorsI (std::string& propertyPrefix, const jderobotice::Context& context):
			prefix(propertyPrefix),context(context) {

			}

			virtual ~MotorsI(){};

			virtual float getV (const Ice::Current&) {

			};

			virtual float getW (const Ice::Current&) {
			};

			virtual Ice::Int setV (Ice::Float v, const Ice::Current&) {
				return 0;
			};

			virtual Ice::Int setW (Ice::Float w, const Ice::Current&) {
				return 0;
			};

			virtual float getL(const Ice::Current&) {

			};

			virtual  Ice::Int setL(Ice::Float w, const Ice::Current&) {
			};

			virtual void update() {
			};

			std::string prefix;
			jderobotice::Context context;
	};
	
	//////////////////////////// LASER //////////////////////////////////////////

	class LaserI: virtual public jderobot::Laser {
		public:
			LaserI (std::string& propertyPrefix, const jderobotice::Context& context):
			prefix(propertyPrefix),context(context),laserData(new jderobot::LaserData()) {
				Ice::PropertiesPtr prop = context.properties();

				//Get laser resolution
				laser_num_readings = prop->getPropertyAsIntWithDefault("PlayerServer.Laser.Num_readings", 180);

				laserData->numLaser = laser_num_readings;
				laserData->distanceData.resize(sizeof(int)*laserData->numLaser);
			}

			virtual ~LaserI(){};

			virtual jderobot::LaserDataPtr getLaserData(const Ice::Current&) {
				return laserData;
			};

			virtual void update(std::vector<float> distanceData) {
				//Update laser values
				for(int i = 0 ; i < laserData->numLaser; i++){
				   laserData->distanceData[i] = distanceData[i];
				}

			};

		private:
			std::string prefix;
			jderobotice::Context context;
			jderobot::LaserDataPtr laserData;

			int laser_num_readings;
	};
	
	////////////////////////// Encoders ///////////////////////////////////

  class EncodersI: virtual public jderobot::Encoders {
		public:
			EncodersI (std::string& propertyPrefix, const jderobotice::Context& context):
			prefix(propertyPrefix),context(context),encodersData(new jderobot::EncodersData()) {
				Ice::PropertiesPtr prop = context.properties();


				correcting_x = prop->getPropertyAsIntWithDefault("PlayerServer.Initial_position.X", 0); /* mm */
				correcting_y = prop->getPropertyAsIntWithDefault("PlayerServer.Initial_position.Y", 0); /* mm */
				correcting_theta = prop->getPropertyAsIntWithDefault("PlayerServer.Initial_position.Theta", 0); /* deg */
			}

			virtual ~EncodersI(){};

			virtual jderobot::EncodersDataPtr getEncodersData(const Ice::Current&){
				return encodersData;
			};

			virtual void update(float robotx, float roboty, float robottheta) {
            encodersData->robotx = robotx;
				encodersData->roboty = roboty;
				encodersData->robottheta = robottheta;
				encodersData->robotcos = cos(robottheta);
				encodersData->robotsin = sin(robottheta);

			};

		private:
			std::string prefix;
			jderobotice::Context context;
			jderobot::EncodersDataPtr encodersData;
			
			float correcting_x; /* mm */
			float correcting_y; /* mm */
			float correcting_theta; /* deg */
	};


	//PTENCODERSI
	class PTEncodersI: virtual public jderobot::PTEncoders {
		public:
			PTEncodersI(int index ,std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context) {
				Ice::PropertiesPtr prop = context.properties();
			}

			virtual ~PTEncodersI(){};

			virtual jderobot::PTEncodersDataPtr getPTEncodersData(const Ice::Current&) {
				return ptEncodersData1; 
			};

			std::string prefix;
			jderobotice::Context context;
			jderobot::PTEncodersDataPtr ptEncodersData1;
	};
	
		//PTENCODERSII
	class PTEncodersII: virtual public jderobot::PTEncoders {
		public:
			PTEncodersII(int index ,std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context) {
				Ice::PropertiesPtr prop = context.properties();
			}

			virtual ~PTEncodersII(){};

			virtual jderobot::PTEncodersDataPtr getPTEncodersData(const Ice::Current&) {
				return ptEncodersData1; 
			};

			std::string prefix;
			jderobotice::Context context;
			jderobot::PTEncodersDataPtr ptEncodersData1;
	};
	
		//PTMOTORS
	class PTMotorsI: virtual public jderobot::PTMotors {
		public:
			PTMotorsI(int index, std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context) {
				Ice::PropertiesPtr prop = context.properties();
			}

			~PTMotorsI(){};

			virtual Ice::Int setPTMotorsData(const jderobot::PTMotorsDataPtr & data, const Ice::Current&) {
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
	};
	
	class PTMotorsII: virtual public jderobot::PTMotors {
		public:
			PTMotorsII(int index, std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context) {
				Ice::PropertiesPtr prop = context.properties();
			}

			~PTMotorsII(){};

			virtual Ice::Int setPTMotorsData(const jderobot::PTMotorsDataPtr & data, const Ice::Current&) {
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
	};
	
	//////////////////////////////////////////////////////////////////////
		//POSE3DMOTORSI
	class Pose3DMotorsI: virtual public jderobot::Pose3DMotors {
		public:
			Pose3DMotorsI(int index, std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context) {
				Ice::PropertiesPtr prop = context.properties();
			}

			~Pose3DMotorsI(){};

			virtual Ice::Int setPose3DMotorsData(const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current&) {
				//waiting for next gazebo camera update
				gazeboPTZ1->Lock(1);
				/*
				gazeboPTZ1->data->control_mode = GAZEBO_PTZ_POSITION_CONTROL;
				gazeboPTZ1->data->cmd_pan = data->pan * DEGTORAD;
				gazeboPTZ1->data->cmd_tilt = data->tilt * DEGTORAD;
				gazeboPTZ1->data->cmd_pan_speed = data->panSpeed;
				gazeboPTZ1->data->cmd_tilt_speed = data->tiltSpeed;
				*/
				gazeboPTZ1->Unlock();

				return 0; 
			};

			virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData(const Ice::Current&) {
				return (NULL);
			};

			virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const Ice::Current&) {
				return (NULL);
			};

			std::string prefix;
			jderobotice::Context context;
			gazebo::PTZIface * gazeboPTZ1;

	};

	//POSE3DMOTORSII
	class Pose3DMotorsII: virtual public jderobot::Pose3DMotors {
		public:
			Pose3DMotorsII(int index, std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context) {
				Ice::PropertiesPtr prop = context.properties();
			}

			virtual ~Pose3DMotorsII(){};

			virtual Ice::Int setPose3DMotorsData(const jderobot::Pose3DMotorsDataPtr & data, const Ice::Current&) {
				//waiting for next gazebo camera update

				gazeboPTZ2->Lock(1);
				/*
				gazeboPTZ2->data->control_mode = GAZEBO_PTZ_POSITION_CONTROL;
				gazeboPTZ2->data->cmd_pan = data->pan * DEGTORAD;
				gazeboPTZ2->data->cmd_tilt = data->tilt * DEGTORAD;
				gazeboPTZ2->data->cmd_pan_speed = data->panSpeed;
				gazeboPTZ2->data->cmd_tilt_speed = data->tiltSpeed;
            */
				gazeboPTZ2->Unlock();

				return 0; 
			};

			virtual jderobot::Pose3DMotorsDataPtr getPose3DMotorsData(const Ice::Current&) {
				return (NULL);
			};

			virtual jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const Ice::Current&) {
				return (NULL);
			};

			std::string prefix;
			jderobotice::Context context;
			gazebo::PTZIface * gazeboPTZ2;

	};

	//POSE3DENCODERSI
	class Pose3DEncodersI: virtual public jderobot::Pose3DEncoders {
		public:
			Pose3DEncodersI(int index ,std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context),
			ptEncodersData1(new jderobot::Pose3DEncodersData()) {
				Ice::PropertiesPtr prop = context.properties();
				
				pan = 0.0;
				tilt= 0.0;
				
			}

			virtual ~Pose3DEncodersI(){};

			virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData(const Ice::Current&) {
				//waiting for next gazebo camera update
				gazeboPTZ1->Lock(1);
				ptEncodersData1->pan = this->pan;
				ptEncodersData1->tilt = this->tilt;
				gazeboPTZ1->Unlock();
				return ptEncodersData1; 
			};
			
			void update(float x, float y, float z, float pan, float tilt, float roll){
			   this->x = x;
			   this->y = y;
			   this->z = z;
			   this->pan = pan;
			   this->tilt = tilt;
			   this->roll = roll;  
			}
         float x, y, z;
         float pan;
         float tilt;
         float roll;
			std::string prefix;
			jderobotice::Context context;
			jderobot::Pose3DEncodersDataPtr ptEncodersData1;
			gazebo::PTZIface * gazeboPTZ1;
	};

	//POSEENCODERSII
	class Pose3DEncodersII: virtual public jderobot::Pose3DEncoders {
		public:
			Pose3DEncodersII(int index, std::string& propertyPrefix, const jderobotice::Context& context)
			: prefix(propertyPrefix),context(context),
			ptEncodersData2(new jderobot::Pose3DEncodersData()) {
				Ice::PropertiesPtr prop = context.properties();
				
				pan = 0.0;
				tilt= 0.0;
			}

			virtual ~Pose3DEncodersII(){};

			virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData(const Ice::Current&) {
				//waiting for next gazebo camera update
				gazeboPTZ2->Lock(1);
				ptEncodersData2->pan = this->pan;
				ptEncodersData2->tilt = this->tilt;
				gazeboPTZ2->Unlock();

				return ptEncodersData2;
			};
			
			void update(float x, float y, float z, float pan, float tilt, float roll){
			   this->x = x;
			   this->y = y;
			   this->z = z;
			   this->pan = pan;
			   this->tilt = tilt;
			   this->roll = roll;  
			}
			
         float x, y, z;
         float pan;
         float tilt;
         float roll;
			std::string prefix;
			jderobotice::Context context;
			jderobot::Pose3DEncodersDataPtr ptEncodersData2;
			gazebo::PTZIface * gazeboPTZ2;
	};
	
	
	
	//////////////////////////////////////////////////////////////////////////


	class Component: public jderobotice::Component {
		public:
			Component():jderobotice::Component("Replayer"),
			laser1(0), encoders1(0) {}
			
			FILE* readLine(FILE* fp, int n){
			   int i=0;   
			   char* str;
			   size_t nSize = 0;
			   
			   while(! feof(fp)){
               getline(&str, &nSize, fp);
               if(i==(n-1)) break;
               i++;

            }
            return fp;
			} 

			virtual void start() {		
				int avLaser, avEncoders, avMotors;
				int avPTMotorsI, avPTMotorsII;
				int avPTEncodersI, avPTEncodersII;
				int avPose3dmotors1, avPose3dmotors2;
				int avPose3dencoders1, avPose3dencoders2;
				
				int Nread = 0;

				Ice::PropertiesPtr prop = context().properties();
								
				robotName = prop->getPropertyWithDefault(context().tag() + ".Hostname","localhost");
				robotPort = prop->getPropertyWithDefault(context().tag() + ".Port","9999");
            
            std::string fileName;
            fileName = prop->getProperty(context().tag() + ".FileName");
            cout << "........" << fileName << " ..... " << endl;

				int nCameras = prop->getPropertyAsInt(context().tag() + ".NCameras");				
				cameras.resize(nCameras);
				for (int i=0; i<nCameras; i++) {//build camera objects
				   cameras[i]=NULL;
				}
				
            usleep(100*1000);  
				
				cout << "DEBUG: Entramos en camera con " << nCameras << " cameras" << endl;
				//Camera
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
					cout << "1" << endl;
					context().tracer().info("Creating camera " + cameraName);
				   cout << "2" << endl;
				   cout << objPrefix << " " << context().tag() << endl; 
				   CameraI* cam = new CameraI(objPrefix, context());
				   cout << "3" << endl;
               if(cam!=NULL){
                  cout << "NULL " << i << endl;
               }  
					cameras[i] = cam;
				   cout << "4" << endl;
					context().createInterfaceWithString(cameras[i],cameraName);
				   cout << "5" << endl;
				   Nread++;
				}
				cout << "DEBUG: Salidos de camera" << endl;

				//Laser
				avLaser = prop->getPropertyAsInt(context().tag() + ".Laser");
				if (avLaser == 1) {
					cout << "DEBUG: Entramos en laser" << endl;
					std::string objPrefix3(context().tag() + ".Laser");
					std::string laserName = "laser1";
					context().tracer().info("Creating laser1 " + laserName);
					laser1 = new LaserI(objPrefix3,context());
					context().createInterfaceWithString(laser1,laserName);
					Nread++;
				}
				cout << "DEBUG: Salidos de laser" << endl;

				//Encoders
				avEncoders = prop->getPropertyAsInt(context().tag() + ".Encoders");
				if (avEncoders == 1) {
					cout << "DEBUG: Entramos en encoders" << endl;
					std::string objPrefix4(context().tag() + ".Encoders");
					std::string encodersName = "encoders1";
					context().tracer().info("Creating encoders1 " + encodersName);
					encoders1 = new EncodersI(objPrefix4,context());
					context().createInterfaceWithString(encoders1,encodersName);
					Nread++;
				}
				
				//Motors (vacia)
				avMotors = prop->getPropertyAsInt(context().tag() + ".Motors");
				if (avMotors == 1) {
					cout << "DEBUG: Entramos en motors" << endl;
					std::string objPrefix2(context().tag() + ".Motors");
					std::string motorsName = "motors1";
					context().tracer().info("Creating motors1 " + motorsName);
					motors1 = new MotorsI(objPrefix2,context());
					context().createInterfaceWithString(motors1,motorsName);
				}
				
             //PTMotorsI
             avPTMotorsI = prop->getPropertyAsInt(context().tag() + ".PTMotorsI");
             if(avPTMotorsI){
				    std::string objPrefix5="ptmotors1";
				    std::string ptmotorsName1 = "ptmotors1";
				    context().tracer().info("Creating ptmotors1 " + ptmotorsName1);
				    ptmotors1 = new PTMotorsI( 0, objPrefix5,context());
				    context().createInterfaceWithString(ptmotors1,ptmotorsName1);
			    }

             //PTMotorsII
             avPTMotorsII = prop->getPropertyAsInt(context().tag() + ".PTMotorsII");
             if(avPTMotorsII){
				    std::string objPrefix6="ptmotors2";
				    std::string ptmotorsName2 = "ptmotors2";
				    context().tracer().info("Creating ptmotors2 " + ptmotorsName2);
				    ptmotors2 = new PTMotorsII(1, objPrefix6, context());
				    context().createInterfaceWithString(ptmotors2,ptmotorsName2);
                }
              
                
              //PTEncodersI
             avPTEncodersI = prop->getPropertyAsInt(context().tag() + ".PTEncodersI");
             if(avPTEncodersI){
				    std::string objPrefix7="ptencoders1";
				    std::string ptencodersName1 = "ptencoders1";
				    context().tracer().info("Creating ptencoders1 " + ptencodersName1);
				    ptencoders1 = new PTEncodersI(0, objPrefix7, context());
				    context().createInterfaceWithString(ptencoders1,ptencodersName1);
			    }
			    
			    //PTEncodersII
             avPTEncodersII = prop->getPropertyAsInt(context().tag() + ".PTEncodersII");
             if(avPTEncodersII){
				    std::string objPrefix8="ptencoders2";
				    std::string ptencodersName2 = "ptencoders2";
				    context().tracer().info("Creating ptencoders2 " + ptencodersName2);
				    ptencoders2 = new PTEncodersII(1, objPrefix8, context());
				    context().createInterfaceWithString(ptencoders2,ptencodersName2);
			    }
			    
 			    //PTMotorsI
             avPose3dmotors1 = prop->getPropertyAsInt(context().tag() + ".pose3dmotors1");
             if(avPose3dmotors1){
                  std::string objPrefix10="pose3dmotors1";
                  std::string pose3dmotorsName1 = "pose3dmotors1";
                  context().tracer().info("Creating pose3dmotors1 " + pose3dmotorsName1);
                  pose3dmotors1 = new Pose3DMotorsI( 0, objPrefix10,context());
                  context().createInterfaceWithString(pose3dmotors1,pose3dmotorsName1);
                  
			    }	    
 			    //PTMotorsII
             avPose3dmotors2 = prop->getPropertyAsInt(context().tag() + ".pose3dmotors2");
             if(avPose3dmotors2){
                  std::string objPrefix12="pose3dmotors2";
						std::string pose3dmotorsName2 = "pose3dmotors2";
						context().tracer().info("Creating pose3dmotors2 " + pose3dmotorsName2);
						pose3dmotors2 = new Pose3DMotorsII(1, objPrefix12, context());
						context().createInterfaceWithString(pose3dmotors2,pose3dmotorsName2);
			    }
			    
 				 avPose3dencoders1 = prop->getPropertyAsInt(context().tag() + ".pose3dencoders1");
				 if(avPose3dencoders1){
						std::string objPrefix11="pose3dencoders1";
					   std::string pose3dencodersName1 = "pose3dencoders1";
					   context().tracer().info("Creating pose3dencoders1 " + pose3dencodersName1);
					   pose3dencoders1 = new Pose3DEncodersI(0, objPrefix11, context());
					   context().createInterfaceWithString(pose3dencoders1,pose3dencodersName1);
					   Nread++;
            }
			    
			    avPose3dencoders2 = prop->getPropertyAsInt(context().tag() + ".pose3dencoders2");
			    if(avPose3dencoders2){
 						std::string objPrefix13="pose3dencoders2";
						std::string pose3dencodersName2 = "pose3dencoders2";
						context().tracer().info("Creating pose3dencoders2 " + pose3dencodersName2);
						pose3dencoders2 = new Pose3DEncodersII(1, objPrefix13, context());
						context().createInterfaceWithString(pose3dencoders2,pose3dencodersName2);
						Nread++;
				 }
				 

			
				//Get subclasses
				MotorsI * motors = NULL;
				LaserI * laser = NULL;
				EncodersI * encoders = NULL;
				
				PTMotorsI* ptmotors_1 = NULL;
			   PTMotorsII* ptmotors_2 = NULL ;
			   
   			PTEncodersI *ptencoders_1 = NULL;
			   PTEncodersII* ptencoders_2 = NULL;

			   Pose3DMotorsI*  pose3dmotors_1  =NULL;
			   Pose3DMotorsII* pose3dmotors_2 = NULL;
			   
			   Pose3DEncodersI*  pose3dencoders_1 = NULL;
			   Pose3DEncodersII* pose3dencoders_2 = NULL;
			   
			   std::vector<CameraI*> camera;


            if(avMotors)
					motors = dynamic_cast<MotorsI*>(&(*motors1));
				if(avLaser)
				    laser = dynamic_cast<LaserI*>(&(*laser1));
				if(avEncoders)
					encoders = dynamic_cast<EncodersI*>(&(*encoders1));
				if(avPTMotorsI)
					ptmotors_1 = dynamic_cast<PTMotorsI*>(&(*ptmotors1));
				if(avPTMotorsII)
					ptmotors_2 = dynamic_cast<PTMotorsII*>(&(*ptmotors2));
				if(avPTEncodersI)
					ptencoders_1 = dynamic_cast<PTEncodersI*>(&(*ptencoders1));	
				if(avPTEncodersII)
					ptencoders_2 = dynamic_cast<PTEncodersII*>(&(*ptencoders2));	
				if(avPose3dencoders1)
				   pose3dencoders_1 = dynamic_cast<Pose3DEncodersI*>(&(*pose3dencoders1));
				if(avPose3dencoders2)
				   pose3dencoders_2 = dynamic_cast<Pose3DEncodersII*>(&(*pose3dencoders2));
			   if(avPose3dmotors1)
				   pose3dmotors_1 = dynamic_cast<Pose3DMotorsI*>(&(*pose3dmotors1));
			   if(avPose3dmotors2)
				   pose3dmotors_2 = dynamic_cast<Pose3DMotorsII*>(&(*pose3dmotors2));
				   
				camera.resize(nCameras);
            for(int i = 0; i< cameras.size(); i++){
               camera[i] = dynamic_cast<CameraI*>(&(*cameras[i]));	
            }
					
					
            struct timeval a, b;
            int cycle = 100;
            long totalb,totala;
            long diff;
            
            int speed = prop->getPropertyAsInt(context().tag() + ".Speed");;   
            
            std::string fileSensors;
            fileSensors = prop->getProperty(context().tag() + ".Sensors");
            cout << "........" << fileSensors << " ..... " << endl;
            
            FILE * pFile;
            pFile = fopen (fileSensors.c_str() ,"r");
            if (pFile == NULL)
                  cout << "Error al leer el fichero" << endl;
            char buff[30];
            long timeRelative = 0;	
            long timeRelativeInicial;	
            long timeRelativeInicial0;	
            long timeToSleep = 0;
            fscanf (pFile, "%ld", &timeRelativeInicial);
            timeRelativeInicial0 = timeRelativeInicial;
            fseek ( pFile , 0 , SEEK_SET );
            
				//Update values
            while ( true ) {
               gettimeofday(&a,NULL);
               totala=a.tv_sec*1000000+a.tv_usec;
					
					while(true){
					
                  if(feof(pFile)){
                     fseek ( pFile , 0 , SEEK_SET );
                     timeRelativeInicial = timeRelativeInicial0;
					      cout << "FIN" <<endl;
					   }
                  fscanf (pFile, "%ld", &timeRelative);
                  //cout << "time:" <<  timeRelative <<endl;
                  
                  if(timeRelative - timeRelativeInicial > 0){
                     timeToSleep = timeRelative - timeRelativeInicial;
                     timeRelativeInicial = timeRelative;
                     fseek ( pFile , -1 , SEEK_CUR );
                     break;
                  }
                  
                  fscanf (pFile, "%s", buff);

                  std:: stringstream streamLaser;
                  streamLaser << robotName+":"+robotPort + ":Laser:";
                  std::string sLaser = streamLaser.str();
					   if(avLaser && laser && !strcmp(buff, sLaser.c_str()) ){  
					        //cout << "Laser" << endl; 
                       float dist;
                       std::vector<float> distanceData;
                       distanceData.resize(180);
                       for(int i = 0; i < 180 ; i++){
                           fscanf (pFile, "%f", &dist);
                           distanceData[i] = dist;
                           //cout << dist << " ";
                       } 
                       //cout << endl;
     						  laser->update(distanceData);
     						  continue;
                  }


                  std:: stringstream streamEncoders;
                  streamEncoders << robotName+":"+robotPort + ":Encoders:";
                  std::string sEncoders = streamEncoders.str();
					   if(avEncoders && encoders && !strcmp(buff, sEncoders.c_str())){
					      //cout << "Encoders" << endl;
                     float robotx, roboty, robottheta;
                     fscanf (pFile, "%f", &robotx);
                     fscanf (pFile, "%f", &roboty);
                     fscanf (pFile, "%f", &robottheta);
                     
                     std::cout << robotx << " y: " << roboty << " z: " << robottheta << endl;
                     
						   encoders->update( robotx, roboty, robottheta);
						   continue;
                  }
                  
                  for(int i = 0; i < camera.size(); i++){ 
                     
                     std:: stringstream stream;
                     stream << robotName +":"+robotPort + ":Camera" << i+1 << ":";
                     std::string s = stream.str();
			            //cout << s << "  " << buff <<endl;
			            
				         if(!strcmp(buff, s.c_str()) ){

                        fscanf (pFile, "%s", buff);
                        
                        FILE* fpImage = fopen(buff,"r");
                        if(fpImage==NULL){
                           cout << "No exite la imagen: Descartando odometría" << endl;
                           pFile = readLine(pFile, Nread-i+1);
                           break;
                        }                     
				            camera[i]->update(buff);
				            //cout << "Dentro:" << i  << buff <<endl;
						      //encoders->update( robotx, roboty, robottheta);
						      break;
                     }
                  }
                  
                  std:: stringstream streamPose3DEncoders1;
                  streamPose3DEncoders1 << robotName+":"+robotPort + ":Pose3DEncoders1:";
                  std::string sPose3DEncoders1 = streamPose3DEncoders1.str();
					   if(avPose3dencoders1 && pose3dencoders_1 && !strcmp(buff, sPose3DEncoders1.c_str())){
                     float x, y, z, pan, tilt, roll;
                     fscanf (pFile, "%f", &x);
                     fscanf (pFile, "%f", &y);
                     fscanf (pFile, "%f", &z);
                     fscanf (pFile, "%f", &pan);
                     fscanf (pFile, "%f", &tilt);
                     fscanf (pFile, "%f", &roll);
                     //std::cout << "pan" << pan << "tilt" << tilt << endl;
                     pose3dencoders_2->update(x, y, z, pan, tilt, roll);
						   continue;
                  }
                  
                  std:: stringstream streamPose3DEncoders2;
                  streamPose3DEncoders2 << robotName+":"+robotPort + ":Pose3DEncoders1:";
                  std::string sPose3DEncoders2 = streamPose3DEncoders2.str();
					   if(avPose3dencoders2 && pose3dencoders_2 && !strcmp(buff, sPose3DEncoders2.c_str())){
                     float x, y, z, pan, tilt, roll;
                     fscanf (pFile, "%f", &x);
                     fscanf (pFile, "%f", &y);
                     fscanf (pFile, "%f", &z);
                     fscanf (pFile, "%f", &pan);
                     fscanf (pFile, "%f", &tilt);
                     fscanf (pFile, "%f", &roll);
                     //std::cout << "pan" << pan << "tilt" << tilt << endl;
                     pose3dencoders_2->update(x, y, z, pan, tilt, roll);
						   continue;
                  }
                  
                  
                  
               }
               

               gettimeofday(&b,NULL);
               totalb=b.tv_sec*1000000+b.tv_usec;
               
               //std::cout << "Introrob takes " << (totalb-totala)/1000 << " ms" << std::endl;
               
               //cout << "Time to sleep: " << (timeToSleep/speed)-(totalb-totala)/1000 << endl;
               
               timeToSleep = (timeToSleep/speed)-(totalb-totala)/1000;
               
               //Sleep Algorithm
               usleep(timeToSleep*1000);
                  
               //std::cout << "->" << diff << std::endl;      
               
               if(feof(pFile))
                  fseek ( pFile , 0 , SEEK_SET );

               
				}
				
            fclose(pFile);
            
				
			}

			virtual ~Component() {}

			private:
			   std::vector<Ice::ObjectPtr> cameras;
				Ice::ObjectPtr motors1;
				Ice::ObjectPtr laser1;
				Ice::ObjectPtr encoders1;
				
				Ice::ObjectPtr ptmotors1;
			   Ice::ObjectPtr ptmotors2;
			   
   			Ice::ObjectPtr ptencoders1;
			   Ice::ObjectPtr ptencoders2;
			   
   			Ice::ObjectPtr pose3dmotors1;
			   Ice::ObjectPtr pose3dmotors2;
			
				Ice::ObjectPtr pose3dencoders1;
			   Ice::ObjectPtr pose3dencoders2;
	};
}


int main(int argc, char** argv) {


	playerserver::Component component;
	jderobotice::Application app(component);
	return app.jderobotMain(argc,argv);
}
