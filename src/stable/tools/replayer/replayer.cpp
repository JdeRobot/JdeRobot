/*
 *
 *  Copyright (C) 1997-2013 JDERobot Developers Team
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
 *  Authors :
 *						Francisco Rivas <franciscomiguel.rivas@urjc.es>
 *
 */


#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <tr1/memory>
#include <list>
#include <sstream>
#include <iostream>
#include <fstream>

// Library includes
#include <math.h>
#include <cv.h>
#include <highgui.h>

#include <visionlib/colorspaces/colorspacesmm.h>
#include <jderobot/camera.h>
#include <jderobot/pointcloud.h>
#include <jderobot/pose3dencoders.h>
#include <jderobot/encoders.h>
#include <jderobot/replayControl.h>
#include <jderobot/laser.h>
#include "replayergui.h"
#include "control.h"
#include <ns/ns.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>



bool componentAlive;

namespace replayer {



	control* controller;

	class CameraI: virtual public jderobot::Camera {
	public:
		CameraI(std::string& propertyPrefix, Ice::CommunicatorPtr ic, long long int initStateIN): prefix(propertyPrefix) {
			jderobot::Logger::getInstance()->info("Creating " + propertyPrefix );

		   imageDescription = (new jderobot::ImageDescription());
		   prop = ic->getProperties();
		   cameraDescription = (new jderobot::CameraDescription());
		   startThread = false;
		   this->width=prop->getPropertyAsIntWithDefault(propertyPrefix + "ImageWidth",320);
		   this->height=prop->getPropertyAsIntWithDefault(propertyPrefix + "ImageHeight",240);
		   this->dataPath=prop->getProperty(propertyPrefix+"Dir");
		   this->fileFormat=prop->getProperty(propertyPrefix+"FileFormat");
		   this->format = prop->getProperty(propertyPrefix+"Format");
		   jderobot::Logger::getInstance()->info("PATH " + this->dataPath );
		   jderobot::Logger::getInstance()->info("FORMAT: " + this->fileFormat );


		   this->initState=initStateIN;
		   //sync task
		   syncTask = new SyncTask(this,this->dataPath, this->fileFormat);
		   syncTask->start();
		   //reply task
		   replyTask = new ReplyTask(this);
		   replyTask->start(); // my own thread

		}

		std::string getName () {
			return (cameraDescription->name);
		}

		std::string getRobotName () {
			return (cameraDescription->name);

		}

		virtual ~CameraI() {
			jderobot::Logger::getInstance()->info("Stopping and joining thread for camera: " +  cameraDescription->name);
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

		virtual void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, const std::string& format, const Ice::Current& c){
			replyTask->pushJob(cb);
		}

		virtual jderobot::ImageFormat getImageFormat(const Ice::Current& c)
		{
			jderobot::ImageFormat formats;

			formats.push_back(colorspaces::ImageRGB8::FORMAT_RGB8.get()->name);

			return formats;
		}

		virtual std::string startCameraStreaming(const Ice::Current&){
			jderobot::Logger::getInstance()->info("Should be made anything to start camera streaming: " + cameraDescription->name);
			return std::string("");
		}

		virtual void stopCameraStreaming(const Ice::Current&) {
			jderobot::Logger::getInstance()->info("Should be made anything to stop camera streaming: " + cameraDescription->name);
		}

		virtual void reset(const Ice::Current&)
		{
		}

		/*virtual void update(cv::Mat imageIn){
		 imageIn.copyTo(this->image);
		 //std::cout << "update" << endl;

		 if(!startThread){
			startThread = true;
			replyTask = new ReplyTask(this);
			   replyTask->start(); // my own thread
		   }
	   }*/

	private:
		class SyncTask: public IceUtil::Thread{
		public:
			SyncTask(CameraI* camera, std::string pathIn, std::string fileFormatIN){
				this->mycamera=camera;
				this->path=pathIn;
				this->initiated=false;
				this->fileFormat=fileFormatIN;
				this->onPause=false;
			}
			~SyncTask(){
			}
			virtual void run(){
				std::string line;
				std::string fileName(this->path + "cameraData.jde");
				std::ifstream myfile(fileName.c_str());
				if (!myfile.is_open())
					jderobot::Logger::getInstance()->error("Error while trying to open: " + fileName);
				while(this->isAlive()){
					while ( myfile.good() ){
						bool playing=controller->getPlay();

						this->onPause=!playing;
						while (!playing){
							playing=controller->getPlay();
							long long int pauseStatus= controller->getSyncTime();
							if (pauseStatus != this->mycamera->initState){
								this->mycamera->initState=pauseStatus;
								break;
							}
							//check if w
							//std::cout << "not playing" << std::endl;
							usleep(10000);
							continue;
						}

						if (this->onPause){
							this->mycamera->initState=controller->getSyncTime();
							myfile.close();
							myfile.open(fileName.c_str());
						}

						getline (myfile,line);
						std::istringstream sTemp(line);
						long long int relative;
						sTemp >> relative;
						lastRelative=relative;


						//tiempo para comprobar si vamos muy desacompasados y para rewind - forward
						IceUtil::Time pretime = IceUtil::Time::now();
						long long int checkState=(pretime.toMicroSeconds())/1000;



						while((((relative) - (checkState - this->mycamera->initState ))<0)&&(myfile.good())){
							//no hacemos nada, estamos fuera de tiempo tenemos que avanzar al siguiente frame
							getline (myfile,line);
							std::istringstream sTemp(line);
							sTemp >> relative;
						}
						if (!myfile.good()){
							if (this->onPause)
								continue;
							else
								break;
						}


						cv::Mat tempImage=cv::imread(this->path + line + "." + this->fileFormat);

						IceUtil::Time a = IceUtil::Time::now();
						long long int actualState=(a.toMicroSeconds())/1000;
						/*std::cout << "Relativo fichero: " << relative << std::endl;
						std::cout << "Relativo global: " << (actualState - this->mycamera->initState ) << std::endl;
						std::cout << "Duermo: " << (relative) - (actualState - this->mycamera->initState ) << std::endl;*/
						if ((actualState - this->mycamera->initState ) < relative ){
							usleep(((relative) - (actualState - this->mycamera->initState ))*1000);
						}


						this->mycamera->dataMutex.lock();

						tempImage.copyTo(this->mycamera->image);



						/*cv::imshow("lector", this->mycamera->image);
						cv::waitKey(0);*/
						this->mycamera->dataMutex.unlock();

					}
					myfile.close();
					//control.controlMutex.lock();
					this->mycamera->initState=controller->wait();
					myfile.open(fileName.c_str());

				}

			}
		private:
			std::string path;
			CameraI* mycamera;
			bool initiated;
			std::string fileFormat;
			bool onPause;
			long long int lastRelative;
		};


		class ReplyTask: public IceUtil::Thread{
			public:
				ReplyTask(CameraI* camera){
				   this->mycamera=camera;
				}

				void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
					IceUtil::Mutex::Lock sync(requestsMutex);
					requests.push_back(cb);
				}

				virtual void run(){
				   mycamera->imageDescription->width = this->mycamera->width;
				   mycamera->imageDescription->height = this->mycamera->height;
				   mycamera->imageDescription->size = this->mycamera->width*this->mycamera->height*3;
				   mycamera->imageDescription->format = this->mycamera->format;

					jderobot::ImageDataPtr reply(new jderobot::ImageData);
					reply->description = mycamera->imageDescription;
					IceUtil::Time a, b;
					int cycle = 48;
					long totalb,totala;
					long diff;

					while(this->isAlive()){
						a = IceUtil::Time::now();
						totala=a.toMicroSeconds();


						IceUtil::Time t = IceUtil::Time::now();
						reply->timeStamp.seconds = (long)t.toSeconds();
						reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;

						reply->pixelData.resize(mycamera->imageDescription->size);

						//image = cv::imread(mycamera->fileName);
						this->mycamera->dataMutex.lock();
						if (this->mycamera->image.rows != 0){
							memcpy( &(reply->pixelData[0]), (unsigned char *) this->mycamera->image.data, this->mycamera->image.rows*this->mycamera->image.cols*3);
						}

						this->mycamera->dataMutex.unlock();
						{ //critical region start
							IceUtil::Mutex::Lock sync(requestsMutex);
							while(!requests.empty()) {
								jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front();
								requests.pop_front();
								cb->ice_response(reply);
							}
						} //critical region end

						b = IceUtil::Time::now();
						totalb=b.toMicroSeconds();

						diff = (totalb-totala)/1000;
						diff = cycle-diff;

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
		colorspaces::Image::FormatPtr imageFmt;
		jderobot::ImageDescriptionPtr imageDescription;
		jderobot::CameraDescriptionPtr cameraDescription;
		ReplyTaskPtr replyTask;
		bool startThread;
		Ice::PropertiesPtr prop;
		cv::Mat image;
		IceUtil::Mutex dataMutex;
		IceUtil::Time ref;
		int width;
		int height;
		typedef IceUtil::Handle<SyncTask> SyncTaskPtr;
		SyncTaskPtr syncTask;
		long long int initState;
		std::string fileFormat;
		std::string dataPath;
		std::string format;

	}; // end class CameraI

	//BEGIN pointCloudI
	class PointCloudI: virtual public jderobot::pointCloud{
	   public:
		PointCloudI (std::string& propertyPrefix, Ice::CommunicatorPtr ic, long long int initStateIN):prefix(propertyPrefix),KData(new jderobot::pointCloudData()) {
					this->prop = ic->getProperties();
					this->dataPath=prop->getProperty(propertyPrefix+"Dir");
					this->initState=initStateIN;
					v  = new SyncTask(this,this->dataPath);
					v->start();
				}

			virtual jderobot::pointCloudDataPtr getCloudData(const Ice::Current&){
				//check si los dos son iguales
				this->m.lock();
				jderobot::pointCloudDataPtr localData(KData);
				this->m.unlock();
				return localData;
			};

		   private:

		      class SyncTask :public IceUtil::Thread{
	            public:
		    	  SyncTask(PointCloudI* cloud, std::string pathIn){
		    		  this->myPointCloud=cloud;
		    		  this->path=pathIn;
		    		  this->initiated=false;
		    		  this->onPause=false;
		    	  }

		    	  virtual void run(){
					std::string line;
					std::string fileName(this->path + "pointCloudData.jde");
					std::ifstream myfile(fileName.c_str());
					if (!myfile.is_open())
						jderobot::Logger::getInstance()->error("-----Error while trying to open: " + fileName);
					while(this->isAlive()){
						while ( myfile.good() ){
							bool playing=controller->getPlay();
							this->onPause=!playing;
							while (!playing){
								playing=controller->getPlay();
								long long int pauseStatus= controller->getSyncTime();
								if (pauseStatus != this->myPointCloud->initState){
									this->myPointCloud->initState=pauseStatus;
									break;
								}
								//check if w
								usleep(10000);
								continue;
							}

							if (this->onPause){
								this->myPointCloud->initState=controller->getSyncTime();
								myfile.close();
								myfile.open(fileName.c_str());
							}


							getline (myfile,line);
							std::istringstream sTemp(line);
							long long int relative;
							sTemp >> relative;
							int sizeVector;
							sTemp >> sizeVector;

							//tiempo para comprobar si vamos muy desacompasados y para rewind - forward
							IceUtil::Time pretime = IceUtil::Time::now();
							long long int checkState=(pretime.toMicroSeconds())/1000;

							while((((relative) - (checkState - this->myPointCloud->initState ))<0)&&(myfile.good())){
								//no hacemos nada, estamos fuera de tiempo tenemos que avanzar al siguiente frame
								getline (myfile,line);
								std::istringstream sTemp(line);
								sTemp >> relative;
								sTemp >> sizeVector;

							}
							if (!myfile.good()){
								if (this->onPause)
									continue;
								else
									break;
							}
							std::ostringstream relativeFile;
							relativeFile << relative;
							std::string localFile(this->path + relativeFile.str());

							std::ifstream infile(localFile.c_str(), std::ios::in | std::ios::binary);


							IceUtil::Time a = IceUtil::Time::now();
							long long int actualState=(a.toMicroSeconds())/1000;
							/*std::cout << "Relativo fichero: " << relative << std::endl;
							std::cout << "Relativo global: " << (actualState - this->myPointCloud->initState ) << std::endl;
							std::cout << "Duermo: " << (relative) - (actualState - this->myPointCloud->initState ) << std::endl;*/
							if ((actualState - this->myPointCloud->initState ) < relative ){
								usleep(((relative) - (actualState - this->myPointCloud->initState ))*1000);
							}

							this->myPointCloud->m.lock();
							this->myPointCloud->KData->p.resize(sizeVector);
							infile.read((char *)&this->myPointCloud->KData->p.front(), this->myPointCloud->KData->p.size()*sizeof(jderobot::RGBPoint));
							this->myPointCloud->m.unlock();
						}
						myfile.close();
						//control.controlMutex.lock();
						this->myPointCloud->initState=controller->wait();
						myfile.open(fileName.c_str());

					}

				}
		    	  PointCloudI* myPointCloud;
		    	  std::string path;
		    	  bool initiated;
		    	  bool onPause;

	         };
	         typedef IceUtil::Handle<SyncTask> SyncTaskPtr;
	         SyncTaskPtr v;
	    	 std::string prefix;
	         jderobot::pointCloudDataPtr KData;
	         Ice::PropertiesPtr prop;
	         std::string dataPath;
	         IceUtil::Mutex m;
	         long long int initState;


	     };




	//BEGIN LaserI
	class LaserI: virtual public jderobot::Laser{
	   public:
		LaserI (std::string& propertyPrefix, Ice::CommunicatorPtr ic, long long int initStateIN):prefix(propertyPrefix),KData(new jderobot::LaserData()) {
					this->prop = ic->getProperties();
					this->dataPath=prop->getProperty(propertyPrefix+"Dir");
					this->initState=initStateIN;
					v  = new SyncTask(this,this->dataPath);
					v->start();
				}

			virtual jderobot::LaserDataPtr getLaserData(const Ice::Current&){
				//check si los dos son iguales
				this->m.lock();
				jderobot::LaserDataPtr localData(KData);
				//solucion??
				localData->numLaser=180;
				this->m.unlock();
				return localData;
			};

		   private:

			  class SyncTask :public IceUtil::Thread{
				public:
				  SyncTask(LaserI* laser, std::string pathIn){
					  this->myLaser=laser;
					  this->path=pathIn;
					  this->initiated=false;
					  this->onPause=false;
				  }

				  virtual void run(){
					std::string line;
					std::string fileName(this->path + "laserData.jde");
					std::ifstream myfile(fileName.c_str());
					if (!myfile.is_open())
						jderobot::Logger::getInstance()->error("-----Error while trying to open: " + fileName);
					while(this->isAlive()){
						while ( myfile.good() ){
							bool playing=controller->getPlay();
							this->onPause=!playing;
							while (!playing){
								playing=controller->getPlay();
								long long int pauseStatus= controller->getSyncTime();
								if (pauseStatus != this->myLaser->initState){
									this->myLaser->initState=pauseStatus;
									break;
								}
								//check if w
								usleep(10000);
								continue;
							}

							if (this->onPause){
								this->myLaser->initState=controller->getSyncTime();
								myfile.close();
								myfile.open(fileName.c_str());
							}


							getline (myfile,line);
							std::istringstream sTemp(line);
							long long int relative;
							sTemp >> relative;
							int sizeVector;
							sTemp >> sizeVector;

							//tiempo para comprobar si vamos muy desacompasados y para rewind - forward
							IceUtil::Time pretime = IceUtil::Time::now();
							long long int checkState=(pretime.toMicroSeconds())/1000;

							while((((relative) - (checkState - this->myLaser->initState ))<0)&&(myfile.good())){
								//no hacemos nada, estamos fuera de tiempo tenemos que avanzar al siguiente frame
								getline (myfile,line);
								std::istringstream sTemp(line);
								sTemp >> relative;
								sTemp >> sizeVector;

							}
							if (!myfile.good()){
								if (this->onPause)
									continue;
								else
									break;
							}
							std::ostringstream relativeFile;
							relativeFile << relative;
							std::string localFile(this->path + relativeFile.str());

							std::ifstream infile(localFile.c_str(), std::ios::in | std::ios::binary);


							IceUtil::Time a = IceUtil::Time::now();
							long long int actualState=(a.toMicroSeconds())/1000;
							/*std::cout << "Relativo fichero: " << relative << std::endl;
							std::cout << "Relativo global: " << (actualState - this->myPointCloud->initState ) << std::endl;
							std::cout << "Duermo: " << (relative) - (actualState - this->myPointCloud->initState ) << std::endl;*/
							if ((actualState - this->myLaser->initState ) < relative ){
								usleep(((relative) - (actualState - this->myLaser->initState ))*1000);
							}
							this->myLaser->m.lock();
							this->myLaser->KData->distanceData.resize(sizeVector);
							infile.read((char *)&this->myLaser->KData->distanceData.front(), this->myLaser->KData->distanceData.size()*sizeof(int));
							this->myLaser->m.unlock();
						}
						myfile.close();
						//control.controlMutex.lock();
						this->myLaser->initState=controller->wait();
						myfile.open(fileName.c_str());

					}

				}
				  LaserI* myLaser;
				  std::string path;
				  bool initiated;
				  bool onPause;

			 };
			 typedef IceUtil::Handle<SyncTask> SyncTaskPtr;
			 SyncTaskPtr v;
			 std::string prefix;
			 jderobot::LaserDataPtr KData;
			 Ice::PropertiesPtr prop;
			 std::string dataPath;
			 IceUtil::Mutex m;
			 long long int initState;


		 };






	//BEGIN Pose3DEncodersI
		class Pose3DEncodersI: virtual public jderobot::Pose3DEncoders{
		   public:
			Pose3DEncodersI (std::string& propertyPrefix, Ice::CommunicatorPtr ic, long long int initStateIN):prefix(propertyPrefix),encData(new jderobot::Pose3DEncodersData()) {
						this->prop = ic->getProperties();
						this->dataPath=prop->getProperty(propertyPrefix+"Dir");
						this->initState=initStateIN;
						v  = new SyncTask(this,this->dataPath);
						v->start();
					}

				virtual jderobot::Pose3DEncodersDataPtr getPose3DEncodersData(const Ice::Current&){
					//check si los dos son iguales
					this->m.lock();
					jderobot::Pose3DEncodersDataPtr localData(encData);
					this->m.unlock();
					return localData;
				};

			   private:

			      class SyncTask :public IceUtil::Thread{
		            public:
			    	  SyncTask(Pose3DEncodersI* enc, std::string pathIn){
			    		  this->myPose3d=enc;
			    		  this->path=pathIn;
			    		  this->initiated=false;
			    		  this->onPause=false;
			    	  }

			    	  virtual void run(){
						std::string line;
						std::string fileName(this->path + "pose3dencoderData.jde");
						std::ifstream myfile(fileName.c_str());
						if (!myfile.is_open())
							jderobot::Logger::getInstance()->error("-----Error while trying to open: " + fileName);
						while(this->isAlive()){
							while ( myfile.good() ){
								bool playing=controller->getPlay();
								this->onPause=!playing;
								while (!playing){
									playing=controller->getPlay();
									long long int pauseStatus= controller->getSyncTime();
									if (pauseStatus != this->myPose3d->initState){
										this->myPose3d->initState=pauseStatus;
										break;
									}
									//check if w
									usleep(10000);
									continue;
								}

								if (this->onPause){
									this->myPose3d->initState=controller->getSyncTime();
									myfile.close();
									myfile.open(fileName.c_str());
								}


								getline (myfile,line);
								std::istringstream sTemp(line);
								long long int relative;
								sTemp >> relative;
								int sizeVector;
								sTemp >> sizeVector;

								//tiempo para comprobar si vamos muy desacompasados y para rewind - forward
								IceUtil::Time pretime = IceUtil::Time::now();
								long long int checkState=(1000000+pretime.toMicroSeconds())/1000;

								while((((relative) - (checkState - this->myPose3d->initState ))<0)&&(myfile.good())){
									//no hacemos nada, estamos fuera de tiempo tenemos que avanzar al siguiente frame
									getline (myfile,line);
									std::istringstream sTemp(line);
									sTemp >> relative;
									sTemp >> sizeVector;

								}
								if (!myfile.good()){
									if (this->onPause)
										continue;
									else
										break;
								}
								std::ostringstream relativeFile;
								relativeFile << relative;
								std::string localFile(this->path + relativeFile.str());

								std::ifstream infile(localFile.c_str(), std::ios::in | std::ios::binary);


								IceUtil::Time a = IceUtil::Time::now();
								long long int actualState=(a.toMicroSeconds())/1000;
								/*std::cout << "Relativo fichero: " << relative << std::endl;
								std::cout << "Relativo global: " << (actualState - this->myPointCloud->initState ) << std::endl;
								std::cout << "Duermo: " << (relative) - (actualState - this->myPointCloud->initState ) << std::endl;*/
								if ((actualState - this->myPose3d->initState ) < relative ){
									usleep(((relative) - (actualState - this->myPose3d->initState ))*1000);
								}

								this->myPose3d->m.lock();
								infile.read((char *)&this->myPose3d->tempData, sizeof(pose3dencoders));
								//hago la copia al interfaz....
								this->myPose3d->encData->x=this->myPose3d->tempData.x;
								this->myPose3d->encData->y=this->myPose3d->tempData.y;
								this->myPose3d->encData->z=this->myPose3d->tempData.z;
								this->myPose3d->encData->pan=this->myPose3d->tempData.pan;
								this->myPose3d->encData->tilt=this->myPose3d->tempData.tilt;
								this->myPose3d->encData->roll=this->myPose3d->tempData.roll;
								this->myPose3d->encData->maxPan=this->myPose3d->tempData.maxPan;
								this->myPose3d->encData->maxTilt=this->myPose3d->tempData.maxTilt;
								this->myPose3d->encData->minPan=this->myPose3d->tempData.minPan;
								this->myPose3d->encData->minTilt=this->myPose3d->tempData.minTilt;
								this->myPose3d->m.unlock();
							}
							myfile.close();
							//control.controlMutex.lock();
							this->myPose3d->initState=controller->wait();
							myfile.open(fileName.c_str());

						}

					}
			    	  Pose3DEncodersI* myPose3d;
			    	  std::string path;
			    	  bool initiated;
			    	  bool onPause;

		         };

			      struct pose3dencoders{
					float x;
					float y;
					float z;
					float pan;
					float tilt;
					float roll;
					int clock;
					float maxPan;
					float maxTilt;
					float minPan;
					float minTilt;
				 };

		         typedef IceUtil::Handle<SyncTask> SyncTaskPtr;
		         SyncTaskPtr v;
		    	 std::string prefix;
		         jderobot::Pose3DEncodersDataPtr encData;
		         Ice::PropertiesPtr prop;
		         std::string dataPath;
		         IceUtil::Mutex m;
		         long long int initState;
		         pose3dencoders tempData;




		     };



		//BEGIN Pose3DEncodersI
		class EncodersI: virtual public jderobot::Encoders{
		   public:
			EncodersI (std::string& propertyPrefix, Ice::CommunicatorPtr ic, long long int initStateIN):prefix(propertyPrefix),encData(new jderobot::EncodersData()) {
						this->prop = ic->getProperties();
						this->dataPath=prop->getProperty(propertyPrefix+"Dir");
						this->initState=initStateIN;
						v  = new SyncTask(this,this->dataPath);
						v->start();
					}

				virtual jderobot::EncodersDataPtr getEncodersData(const Ice::Current&){
					//check si los dos son iguales
					this->m.lock();
					jderobot::EncodersDataPtr localData(encData);
					this->m.unlock();
					return localData;
				};

			   private:

				  class SyncTask :public IceUtil::Thread{
					public:
					  SyncTask(EncodersI* enc, std::string pathIn){
						  this->myEncoder=enc;
						  this->path=pathIn;
						  this->initiated=false;
						  this->onPause=false;
					  }

					  virtual void run(){
						std::string line;
						std::string fileName(this->path + "encoderData.jde");
						std::ifstream myfile(fileName.c_str());
						if (!myfile.is_open())
							jderobot::Logger::getInstance()->error("-----Error while trying to open: " + fileName);
						while(this->isAlive()){
							while ( myfile.good() ){
								bool playing=controller->getPlay();
								this->onPause=!playing;
								while (!playing){
									playing=controller->getPlay();
									long long int pauseStatus= controller->getSyncTime();
									if (pauseStatus != this->myEncoder->initState){
										this->myEncoder->initState=pauseStatus;
										break;
									}
									//check if w
									usleep(10000);
									continue;
								}

								if (this->onPause){
									this->myEncoder->initState=controller->getSyncTime();
									myfile.close();
									myfile.open(fileName.c_str());
								}


								getline (myfile,line);
								std::istringstream sTemp(line);
								long long int relative;
								sTemp >> relative;
								int sizeVector;
								sTemp >> sizeVector;

								//tiempo para comprobar si vamos muy desacompasados y para rewind - forward
								IceUtil::Time pretime = IceUtil::Time::now();
								long long int checkState=(pretime.toMicroSeconds())/1000;

								while((((relative) - (checkState - this->myEncoder->initState ))<0)&&(myfile.good())){
									//no hacemos nada, estamos fuera de tiempo tenemos que avanzar al siguiente frame
									getline (myfile,line);
									std::istringstream sTemp(line);
									sTemp >> relative;
									sTemp >> sizeVector;

								}
								if (!myfile.good()){
									if (this->onPause)
										continue;
									else
										break;
								}
								std::ostringstream relativeFile;
								relativeFile << relative;
								std::string localFile(this->path + relativeFile.str());

								std::ifstream infile(localFile.c_str(), std::ios::in | std::ios::binary);


								IceUtil::Time a = IceUtil::Time::now();
								long long int actualState=(a.toMicroSeconds())/1000;
								/*std::cout << "Relativo fichero: " << relative << std::endl;
								std::cout << "Relativo global: " << (actualState - this->myPointCloud->initState ) << std::endl;
								std::cout << "Duermo: " << (relative) - (actualState - this->myPointCloud->initState ) << std::endl;*/
								if ((actualState - this->myEncoder->initState ) < relative ){
									usleep(((relative) - (actualState - this->myEncoder->initState ))*1000);
								}

								this->myEncoder->m.lock();
								infile.read((char *)&this->myEncoder->tempData, sizeof(encoders));
								//hago la copia al interfaz....
								this->myEncoder->encData->robotx=this->myEncoder->tempData.robotx;
								this->myEncoder->encData->roboty=this->myEncoder->tempData.roboty;
								this->myEncoder->encData->robottheta=this->myEncoder->tempData.robottheta;
								this->myEncoder->encData->robotcos=this->myEncoder->tempData.robotcos;
								this->myEncoder->encData->robotsin=this->myEncoder->tempData.robotsin;
								this->myEncoder->m.unlock();
							}
							myfile.close();
							//control.controlMutex.lock();
							this->myEncoder->initState=controller->wait();
							myfile.open(fileName.c_str());

						}

					}
					  EncodersI* myEncoder;
					  std::string path;
					  bool initiated;
					  bool onPause;

				 };

				  struct encoders{
				  		float robotx;
				  		float roboty;
				  		float robottheta;
				  		float robotcos;
				  		float robotsin;
				  	};

				 typedef IceUtil::Handle<SyncTask> SyncTaskPtr;
				 SyncTaskPtr v;
				 std::string prefix;
				 jderobot::EncodersDataPtr encData;
				 Ice::PropertiesPtr prop;
				 std::string dataPath;
				 IceUtil::Mutex m;
				 long long int initState;
				 encoders tempData;




			 };





	//BEGIN replayControllerI
		class replayControllerI: virtual public jderobot::replayControl{
		public:
			replayControllerI(){

			}
			~replayControllerI(){

			}

			virtual bool pause(const Ice::Current&){
				controller->stop();
				return true;
			}
			virtual bool resume(const Ice::Current&){
				controller->resume();
				return true;
			}
			virtual void setReplay( bool replay, const Ice::Current&){
				controller->setRepeat(replay);
			}
			virtual bool setStep( Ice::Int step, const Ice::Current&){
				controller->setStep(step);
				return true;
			}
			virtual Ice::Long getTime(const Ice::Current&){
				return controller->getRelativeTime();
			}
			virtual bool goTo(Ice::Long seek, const Ice::Current&){
				//set the current position to seek
				return 0;
			}
		private:

		};



} //namespace

jderobot::ns* namingService = NULL;
bool killed;
Ice::CommunicatorPtr ic;

void exitApplication(int s){


	killed=true;
	componentAlive=false;

	// NamingService
	if (namingService != NULL)
	{
		namingService->unbindAll();

		delete(namingService);
	}

	ic->shutdown();
	exit(0);

}


int main(int argc, char** argv) {

	std::vector<Ice::ObjectPtr> cameras;
	IceUtil::Time a = IceUtil::Time::now();
	long long int initState=(a.toMicroSeconds())/1000;
	int nProcs=0;
	std::string componentPrefix("Replayer");


	componentAlive=true;
	killed=false;
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = exitApplication;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);


	try{
		ic = Ice::initialize(argc, argv);
		Ice::PropertiesPtr prop = ic->getProperties();

		replayer::controller= new replayer::control(initState);


		// Naming Service
		int nsActive = prop->getPropertyAsIntWithDefault("NamingService.Enabled", 0);

		if (nsActive)
		{
			std::string ns_proxy = prop->getProperty("NamingService.Proxy");
			try
			{
				namingService = new jderobot::ns(ic, ns_proxy);
			}
			catch (Ice::ConnectionRefusedException& ex)
			{
				jderobot::Logger::getInstance()->error("Impossible to connect with NameService!");
				exit(-1);
			}
		}

		// Analyze LOG section

		std::string logFile = prop->getProperty(componentPrefix + ".Log.File.Name");
		if (logFile.size()==0)
			jderobot::Logger::getInstance()->warning("You didn't set log file!");
		else
			jderobot::Logger::getInstance()->setFileLog(logFile);

		std::string logLevel = prop->getProperty(componentPrefix + ".Log.File.Level");
		if (logLevel.size()==0)
			jderobot::Logger::getInstance()->warning("You didn't set *.Log.File.Level key!");
		else
			jderobot::Logger::getInstance()->setFileLevel(jderobot::Levels(boost::lexical_cast<int>(logLevel)));

		std::string screenLevel = prop->getProperty(componentPrefix + ".Log.Screen.Level");
		if (screenLevel.size()==0)
			jderobot::Logger::getInstance()->warning("You didn't set *.Log.Screen.Level key!");
		else
			jderobot::Logger::getInstance()->setScreenLevel(jderobot::Levels(boost::lexical_cast<int>(screenLevel)));

		jderobot::Logger::getInstance()->info("Logger:: screenLevel=" + screenLevel + " logLevel=" + logLevel + " LogFile=" + logFile);




		int nCameras = prop->getPropertyAsIntWithDefault(componentPrefix+".nCameras",0);
		jderobot::Logger::getInstance()->info( "Cameras to load: " + boost::lexical_cast<std::string>(nCameras));
		std::string Endpoints = prop->getProperty(componentPrefix+".Endpoints");
		cameras.resize(nCameras);
		Ice::ObjectAdapterPtr adapter =ic->createObjectAdapterWithEndpoints(componentPrefix+"", Endpoints);
		for (int i=0; i<nCameras; i++) {//build camera objects
			std::stringstream objIdS;
			objIdS <<  i;
			std::string objId = objIdS.str();
			std::string objPrefix(componentPrefix+".Camera." + objId + ".");
			std::string cameraName = prop->getProperty(objPrefix + "Name");
			jderobot::Logger::getInstance()->info( "Creating camera: " + cameraName);

			if (cameraName.size() == 0) { //no name specified, we create one using the index
				cameraName = "camera" + objId;
				prop->setProperty(objPrefix + "Name",cameraName);//set the value
			}




			cameras[i] = new replayer::CameraI(objPrefix,ic,initState);



			adapter->add(cameras[i], ic->stringToIdentity(cameraName));
			if (namingService)
						namingService->bind(cameraName, Endpoints, "::jderobot::Camera");
			nProcs++;
		}
		int nPointClouds = prop->getPropertyAsIntWithDefault(componentPrefix+".nPointClouds",0);
		for (int i=0; i<nPointClouds; i++) {//build camera objects
			std::stringstream objIdS;
			objIdS <<  i;
			std::string objId = objIdS.str();
			std::string objPrefix(componentPrefix+".PointCloud." + objId + ".");
			std::string Name = prop->getProperty(objPrefix + "Name");
			jderobot::Logger::getInstance()->info("Creating pointCloud " + Name );


			if (Name.size() == 0) { //no name specified, we create one using the index
				Name = "pointcloud" + objId;
				prop->setProperty(objPrefix + "Name",Name);//set the value
			}




			Ice::ObjectPtr  object= new replayer::PointCloudI(objPrefix,ic,initState);



			adapter->add(object, ic->stringToIdentity(Name));
			if (namingService)
				namingService->bind(Name, Endpoints, object->ice_staticId());
			nProcs++;
		}

		int nLasers = prop->getPropertyAsIntWithDefault(componentPrefix+".nLasers",0);
		for (int i=0; i<nLasers; i++) {//build camera objects
			std::stringstream objIdS;
			objIdS <<  i;
			std::string objId = objIdS.str();
			std::string objPrefix(componentPrefix+".laser." + objId + ".");
			std::string Name = prop->getProperty(objPrefix + "Name");
			jderobot::Logger::getInstance()->info("Creating laser " + Name );

			if (Name.size() == 0) { //no name specified, we create one using the index
				Name = "laser" + objId;
				prop->setProperty(objPrefix + "Name",Name);//set the value
			}



			Ice::ObjectPtr  object= new replayer::LaserI(objPrefix,ic,initState);



			adapter->add(object, ic->stringToIdentity(Name));
			if (namingService)
				namingService->bind(Name, Endpoints, object->ice_staticId());
			nProcs++;
		}



		int nPose3dEncoders = prop->getPropertyAsIntWithDefault(componentPrefix+".nPose3dEncoders",0);
		for (int i=0; i<nPose3dEncoders; i++) {//build camera objects
			std::stringstream objIdS;
			objIdS <<  i;
			std::string objId = objIdS.str();
			std::string objPrefix(componentPrefix+".pose3dencoder." + objId + ".");
			std::string Name = prop->getProperty(objPrefix + "Name");
			jderobot::Logger::getInstance()->info("Creating pose3dencoders " + Name );

			if (Name.size() == 0) { //no name specified, we create one using the index
				Name = "pose3dencoders" + objId;
				prop->setProperty(objPrefix + "Name",Name);//set the value
			}

			Ice::ObjectPtr  object= new replayer::Pose3DEncodersI(objPrefix,ic,initState);



			adapter->add(object, ic->stringToIdentity(Name));
			if (namingService)
				namingService->bind(Name, Endpoints, object->ice_staticId());
			nProcs++;
		}

		int nEncoders = prop->getPropertyAsIntWithDefault(componentPrefix+".nEncoders",0);
		for (int i=0; i<nEncoders; i++) {//build camera objects
			std::stringstream objIdS;
			objIdS <<  i;
			std::string objId = objIdS.str();
			std::string objPrefix(componentPrefix+".encoder." + objId + ".");
			std::string Name = prop->getProperty(objPrefix + "Name");
			jderobot::Logger::getInstance()->info("Creating encoders " + Name );


			if (Name.size() == 0) { //no name specified, we create one using the index
				Name = "encoders" + objId;
				prop->setProperty(objPrefix + "Name",Name);//set the value
			}

			Ice::ObjectPtr  object= new replayer::EncodersI(objPrefix,ic,initState);



			adapter->add(object, ic->stringToIdentity(Name));
			if (namingService)
				namingService->bind(Name, Endpoints, object->ice_staticId());
			nProcs++;
		}


		int controllerActive = prop->getPropertyAsIntWithDefault(componentPrefix+".replayControl.Active",0);
		if (controllerActive){
			Ice::ObjectPtr rc= new replayer::replayControllerI();
			std::string objPrefix(componentPrefix+".replayControl.");
			std::string Name = prop->getProperty(objPrefix + "Name");

			adapter->add(rc, ic->stringToIdentity(Name));
			if (namingService)
				namingService->bind(Name, Endpoints, rc->ice_staticId());
		}

		adapter->activate();
		replayer::controller->setProcesses(nProcs);

		//replayer::replayergui* gui = new replayer::replayergui(replayer::controller);

		//do it beeter
		while (1){
			//gui->update();
			replayer::controller->checkStatus();
		}



		ic->waitForShutdown();
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		exit(-1);
	} catch (const char* msg) {
		std::cerr << msg << std::endl;
		exit(-1);
	}
	return 0;
}
