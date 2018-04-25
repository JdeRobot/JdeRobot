/*
 * Copyright 2011 Nate Koenig & Andrew Howard
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
//#include "plugins/DepthCameraPlugin.hh"
#include "kinectPlugin.h"

#include <stdio.h>

#include <pcl/io/pcd_io.h>

using namespace gazebo;

gazebo::DepthCameraPlugin* kinect;

GZ_REGISTER_SENSOR_PLUGIN(DepthCameraPlugin)

void *mainKinect(void* v);

/////////////////////////////////////////////////
DepthCameraPlugin::DepthCameraPlugin() : SensorPlugin()/*,viewer ("PCL OpenNI Viewer")*/,cloud((new pcl::PointCloud <pcl::PointXYZRGBA>))
{

	count = 0;
	pthread_mutex_init (&mutex, NULL);	
	pthread_mutex_init (&mutexRGB, NULL);
	pthread_mutex_init (&mutexDepth, NULL);	

}

/////////////////////////////////////////////////
void DepthCameraPlugin::Load(sensors::SensorPtr _sensor,
                              sdf::ElementPtr /*_sdf*/)
{
  this->parentSensor =
    boost::dynamic_pointer_cast<sensors::DepthCameraSensor>(_sensor);
  this->depthCamera = this->parentSensor->GetDepthCamera();

  if (!this->parentSensor)
  {
    gzerr << "DepthCameraPlugin not attached to a depthCamera sensor\n";
    return;
  }

  this->width = this->depthCamera->GetImageWidth();
  this->height = this->depthCamera->GetImageHeight();
  this->depth = this->depthCamera->GetImageDepth();
  this->format = this->depthCamera->GetImageFormat();

  this->newDepthFrameConnection = this->depthCamera->ConnectNewDepthFrame(
      boost::bind(&DepthCameraPlugin::OnNewDepthFrame,
        this, _1, _2, _3, _4, _5));

  this->newImageFrameConnection = this->depthCamera->ConnectNewImageFrame(
      boost::bind(&DepthCameraPlugin::OnNewImageFrame,
        this, _1, _2, _3, _4, _5));


  /*
  this->newRGBPointCloudConnection = this->depthCamera->ConnectNewRGBPointCloud(
      boost::bind(&DepthCameraPlugin::OnNewRGBPointCloud,
        this, _1, _2, _3, _4, _5));
*/


  this->parentSensor->SetActive(true);
}

	void DepthCameraPlugin::depth2rgb(cv::Mat image){
	   //const unsigned short *disparity = Xn_disparity;
	   imageDepth.create(image.rows,image.cols,CV_8UC3);
	   float * data= (float*)image.data;
	   for (int i=0; i<image.rows* image.cols; i++) {
		  //std::cout << i << std::endl;
		  int val = (int)(data[i]*1000);
			  imageDepth.data[3*i+0] = (float)val/10000*255;;
			  imageDepth.data[3*i+1] = val>>8;
			  imageDepth.data[3*i+2] = val&0xff;
			  
				if(imageDepth.data[i*3]!=0)
					imageDepth.data[i*3]=255-imageDepth.data[i*3];
				imageDepth.data[i*3+1]=imageDepth.data[i*3];
				imageDepth.data[i*3+2]=imageDepth.data[i*3];
			
	   }
	}


/////////////////////////////////////////////////
void DepthCameraPlugin::OnNewDepthFrame(const float *_image,
    unsigned int _width, unsigned int _height,
    unsigned int _depth, const std::string &_format)
{

	if(count == 0){
		count++;
		std::string name = this->parentSensor->GetName();
		nameKinect = std::string("--Ice.Config=" + name + ".cfg");
		pthread_t thr_gui;
		pthread_create(&thr_gui, NULL, &mainKinect, (void*)this);
        cloud->points.resize(_width*_height);
    	imageDepth.create(_height, _width, CV_8UC3);
	}

	double hfov = 1.04719755;
	double fl = ((double)_width) / (2.0 *tan(hfov/2.0));
	double pointCloudCutoff = 0.001;

	//std::cout << "depth: " << (int)(_image[0]*1000) << std::endl;

	cv::Mat image;

	image.create(_height, _width, CV_32FC1);

    memcpy( (float *)image.data,(float *) _image, _width*_height*4 );




	double pAngle;
    double yAngle;


	pthread_mutex_lock (&kinect->mutex);
    cloud->points.resize(_width*_height);
    
    int indicePunto = 0;
	pcl::PointXYZRGBA point;
	point.r      = 255;
	point.g      = 0;
	point.b      = 0;
    
	for(unsigned int x = 0 ; x < _width ; x++){
		for(unsigned int y = 0; y < _height; y++){
			unsigned int indice = y*_width + x;

		  	if (_height>1) 
				yAngle = atan2( (double)x - 0.5*(double)(width-1), fl);
		  	else            
				yAngle = 0.0;

			if (_width>1) 
				pAngle = atan2( (double)y - 0.5*(double)(height-1), fl);
			else            
				pAngle = 0.0;

			float d = _image[indice];


			point.x      = d * tan(yAngle);
			point.y      = d * tan(pAngle);
			if(!imageRGB.data){
				point.r      = 255;
				point.g      = 0;
				point.b      = 0;
			}else{
				unsigned int indice = y*imageRGB.step + x*imageRGB.channels();
				point.r      = imageRGB.data[indice];
				point.g      = imageRGB.data[indice+1];
				point.b      = imageRGB.data[indice+2];
			}
			if(d > pointCloudCutoff){
				point.z    = _image[indice];
			}else{ //point in the unseeable range
			
				point.x = point.y = point.z = std::numeric_limits<float>::quiet_NaN ();
				cloud->is_dense = false;
			}

			cloud->points[indicePunto++] = point;

		}
	}
	
	pcl::VoxelGrid<pcl::PointXYZRGBA> sor;
	sor.setInputCloud (cloud);
	sor.setLeafSize (this->leafSize, this->leafSize, this->leafSize);

	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud2(new pcl::PointCloud<pcl::PointXYZRGBA>);

	sor.filter (*cloud2);

	*cloud = *cloud2;
	
	pthread_mutex_unlock (&kinect->mutex);
		
  pthread_mutex_lock(&mutexDepth);            
  depth2rgb(image);
  pthread_mutex_unlock(&mutexDepth);
		
	//pthread_mutex_unlock (&mutex);

	//std::cout << "cloud: " << cloud->points.size() << std::endl;
	//if (!viewer.wasStopped())
	//  viewer.showCloud(cloud);

	
}

/////////////////////////////////////////////////
void DepthCameraPlugin::OnNewRGBPointCloud(const float * _image,
                unsigned int _width, unsigned int _height,
                unsigned int _depth, const std::string &/*_format*/)
{
 
    
}

/////////////////////////////////////////////////
void DepthCameraPlugin::OnNewImageFrame(const unsigned char * _image,
                              unsigned int _width,
                              unsigned int _height,
                              unsigned int _depth,
                              const std::string &_format)
{


  	//std::cout << "OnNewImageFrame Format: " << format << " Depth: " << _depth << std::endl;
	pthread_mutex_lock (&mutexRGB);
	imageRGB.create(_height, _width, CV_8UC3);
    memcpy((unsigned char *) imageRGB.data, &(_image[0]), _width*_height * 3);
	pthread_mutex_unlock (&mutexRGB);

}

void DepthCameraPlugin::SetLeafSize(const float size)
{
	pthread_mutex_lock (&kinect->mutex);
    this->leafSize = size;
	pthread_mutex_unlock (&kinect->mutex);
}

   class KinectI: virtual public jderobot::pointCloud{
   public:
		KinectI (std::string propertyPrefix, gazebo::DepthCameraPlugin* kinect):prefix(propertyPrefix)
		{

			  this->kinect = kinect;
		}
		
		virtual jderobot::pointCloudDataPtr getCloudData(const Ice::Current&){
		      
			pthread_mutex_lock (&kinect->mutex);
			jderobot::pointCloudDataPtr KData = new jderobot::pointCloudData();
            pcl::PointCloud<pcl::PointXYZRGBA> cloud = *kinect->cloud;
			pthread_mutex_unlock (&kinect->mutex);
			
            if(cloud.points.size()){
               KData->p.resize(cloud.points.size());
               unsigned int index = 0;
               for(unsigned int i = 0; i < cloud.points.size(); i++){
                  KData->p[index].x = cloud.points[i].x;
                  KData->p[index].y = cloud.points[i].y;
                  KData->p[index].z = cloud.points[i].z;
                  KData->p[index].r = cloud.points[i].r;
                  KData->p[index].g = cloud.points[i].g;
                  KData->p[index].b = cloud.points[i].b;    
                  index++;
               }
            }
			return KData;
		};
	     gazebo::DepthCameraPlugin* kinect;
		 std::string prefix;

 }; //end class KinectI

class CameraI: virtual public jderobot::Camera {
	public:
		CameraI(std::string propertyPrefix, gazebo::DepthCameraPlugin* camera, std::string format)
			   : prefix(propertyPrefix), cameraI(camera) {
		
			std::cout << "Constructor CameraRGB" << std::endl;

			imageDescription = (new jderobot::ImageDescription());
			mFormats.push_back(format);

            cameraDescription = (new jderobot::CameraDescription ());
            cameraDescription->name = propertyPrefix;

        replyTask = new ReplyTask(this);
		    replyTask->start(); // my own thread
		  
		}

		std::string getName () {
			return (cameraDescription->name);
		}

		std::string getRobotName () {
			return ("RobotName");
		}

		virtual ~CameraI() {

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

		virtual jderobot::ImageFormat getImageFormat(const Ice::Current& c){
		  return(mFormats);
		}

		virtual void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, const std::string& format, const Ice::Current& c){
			replyTask->pushJob(cb);
		}



		virtual std::string startCameraStreaming(const Ice::Current&){
            return ("N/A");
		}

		virtual void stopCameraStreaming(const Ice::Current&) {

		}

		virtual void reset(const Ice::Current&)
		{
		}

	private:
		class ReplyTask: public IceUtil::Thread  {
			public:
				ReplyTask(CameraI* camera)
				: mycamera(camera) {
				   	std::cout << "safeThread" << std::endl;
				}

				void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
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
						
						if(!mycamera->cameraI->imageRGB.data){
							usleep(100);
							continue;
						}
						if(count==0){
							pthread_mutex_lock (&mycamera->cameraI->mutexRGB);
							mycamera->imageDescription->width = mycamera->cameraI->imageRGB.cols;
							mycamera->imageDescription->height = mycamera->cameraI->imageRGB.rows;
							mycamera->imageDescription->size = mycamera->cameraI->imageRGB.cols*mycamera->cameraI->imageRGB.rows*3;
							pthread_mutex_unlock (&mycamera->cameraI->mutexRGB);

							mycamera->imageDescription->format = "RGB8";

							reply->description = mycamera->imageDescription;
							count++;
						}

						//std::cout << nameGlobal<< std::endl;
									
						gettimeofday(&a,NULL);
						totala=a.tv_sec*1000000+a.tv_usec;


						IceUtil::Time t = IceUtil::Time::now();
						reply->timeStamp.seconds = (long)t.toSeconds();
						reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
          				
          				pthread_mutex_lock (&mycamera->cameraI->mutexRGB);
					    reply->pixelData.resize(mycamera->cameraI->imageRGB.rows*mycamera->cameraI->imageRGB.cols*3);
					    
					    memcpy( &(reply->pixelData[0]), (unsigned char *) mycamera->cameraI->imageRGB.data, mycamera->cameraI->imageRGB.rows*mycamera->cameraI->imageRGB.cols*3);
						pthread_mutex_unlock (&mycamera->cameraI->mutexRGB);

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
		gazebo::DepthCameraPlugin* cameraI;	
		std::vector<std::string> mFormats;
		
}; // end class CameraI


class CameraII: virtual public jderobot::Camera {
	public:
		CameraII(std::string propertyPrefix, gazebo::DepthCameraPlugin* camera, std::string format)
			   : prefix(propertyPrefix), cameraI(camera) {
		
			std::cout << "Constructor CameraDepth" << std::endl;

			imageDescription = (new jderobot::ImageDescription());
			mFormats.push_back(format);

            cameraDescription = (new jderobot::CameraDescription ());
            cameraDescription->name = propertyPrefix;

        	replyTask = new ReplyTask(this);
		    replyTask->start(); // my own thread
		  
		}

		std::string getName () {
			return (cameraDescription->name);
		}

		std::string getRobotName () {
			return ("RobotName");
		}

		virtual ~CameraII() {

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

    virtual jderobot::ImageFormat getImageFormat(const Ice::Current& c){
      return(mFormats);
    }

    virtual void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, const std::string& format, const Ice::Current& c){
			replyTask->pushJob(cb);
		}

		virtual std::string startCameraStreaming(const Ice::Current&){
            return ("N/A");
		}

		virtual void stopCameraStreaming(const Ice::Current&) {

		}

		virtual void reset(const Ice::Current&)
		{
		}

	private:
		class ReplyTask: public IceUtil::Thread {
			public:
				ReplyTask(CameraII* camera)
				:  mycamera(camera) {
				   	std::cout << "safeThread" << std::endl;
				}

				void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
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
						
						if(!mycamera->cameraI->imageDepth.data){
							usleep(100);
							continue;
						}
						if(count==0){
							pthread_mutex_lock (&mycamera->cameraI->mutexDepth);
							mycamera->imageDescription->width = mycamera->cameraI->imageDepth.cols;
							mycamera->imageDescription->height = mycamera->cameraI->imageDepth.rows;
							mycamera->imageDescription->size = mycamera->cameraI->imageDepth.cols*mycamera->cameraI->imageDepth.rows*3;
							pthread_mutex_unlock (&mycamera->cameraI->mutexDepth);

							mycamera->imageDescription->format = "RGB8";

							reply->description = mycamera->imageDescription;
							count++;
						}

						//std::cout << nameGlobal<< std::endl;
									
						gettimeofday(&a,NULL);
						totala=a.tv_sec*1000000+a.tv_usec;


						IceUtil::Time t = IceUtil::Time::now();
						reply->timeStamp.seconds = (long)t.toSeconds();
						reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
          				
          				pthread_mutex_lock (&mycamera->cameraI->mutexDepth);
					    reply->pixelData.resize(mycamera->cameraI->imageDepth.rows*mycamera->cameraI->imageDepth.cols*3);
					    
					    memcpy( &(reply->pixelData[0]), (unsigned char *) mycamera->cameraI->imageDepth.data, mycamera->cameraI->imageDepth.rows*mycamera->cameraI->imageDepth.cols*3);
						pthread_mutex_unlock (&mycamera->cameraI->mutexDepth);

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

				CameraII* mycamera;
				IceUtil::Mutex requestsMutex;
				std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
		};

		typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;
		std::string prefix;
		colorspaces::Image::FormatPtr imageFmt;
		jderobot::ImageDescriptionPtr imageDescription;
		jderobot::CameraDescriptionPtr cameraDescription;
		ReplyTaskPtr replyTask;
		gazebo::DepthCameraPlugin* cameraI;	
		std::vector<std::string> mFormats;
		
}; // end class CameraII

void *mainKinect(void* v) 
{

	kinect = (gazebo::DepthCameraPlugin*)v;
	
	char* name = (char*)kinect->nameKinect.c_str();

    Ice::CommunicatorPtr ic;
    int argc = 1;

    Ice::PropertiesPtr prop;
	char* argv[] = {name};
    try {
        
        ic = EasyIce::initialize(argc, argv);
        prop = ic->getProperties();
        
        std::string Endpoints = prop->getProperty("Kinect.Endpoints");
        std::cout << "Kinect Endpoints > " << Endpoints << std::endl;
        

        std::stringstream cnvrt(prop->getPropertyWithDefault("PointCloud.LeafSize","100"));
        float size;
        cnvrt >> size;
        if (cnvrt.fail()) {
         std::cout << "Couldn't read PointCloud.LeafSize property, setting to default" << std::endl;
         size=100;
        }else if(size <= 0.001){
         std::cout << "PointCloud.LeafSize property out of bounds, setting to default" << std::endl;
         size=100;
        }
        kinect->SetLeafSize(1./size);

        Ice::ObjectAdapterPtr adapter =
            ic->createObjectAdapterWithEndpoints("Kinect", Endpoints);

        Ice::ObjectPtr object = new KinectI(std::string("pointcloud1"),  kinect);
        Ice::ObjectPtr object2 = new CameraI(std::string("cameraRGB"),  kinect,colorspaces::ImageRGB8::FORMAT_RGB8.get()->name);
        Ice::ObjectPtr object3 = new CameraII(std::string("cameraDepth"),  kinect, colorspaces::ImageRGB8::FORMAT_DEPTH8_16.get()->name);

        adapter->add(object, ic->stringToIdentity("pointcloud1"));
        adapter->add(object2, ic->stringToIdentity("cameraRGB"));
        adapter->add(object3, ic->stringToIdentity("cameraDepth"));
        std::cout << "        adapter->add(object, ic->stringToIdentity(Kinect)); "  << std::endl;
        adapter->activate();
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

    return NULL;
}

