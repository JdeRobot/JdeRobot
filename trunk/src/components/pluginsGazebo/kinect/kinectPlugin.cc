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
    gzerr << "Kinect Construtor\n";

}

/////////////////////////////////////////////////
void DepthCameraPlugin::Load(sensors::SensorPtr _sensor,
                              sdf::ElementPtr /*_sdf*/)
{
  this->parentSensor =
    boost::shared_dynamic_cast<sensors::DepthCameraSensor>(_sensor);
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

	void raw2depth(){
	   int i;
	   for ( i=0; i<MAX_DEPTH; i++) {
		    float v = (float)i/MAX_DEPTH;//for visualization purposes only
		    v = v*v;
		    v = v*36*256;
		    depth[i] = v;
		    //std::cout << "depth[]:" << depth[i] << std::endl;
	   } 
	}

	void depth2rgb( const float* disparity , cv::Mat image){
	   int i;
		
	   for (i=0; i<image.rows* image.cols; i++) {
		  //std::cout << i << std::endl;
		  int pval = depth[(int)(disparity[i]*10000)];
		  int lb = pval & 0xff;
		  switch (pval>>8) {
			  case 0:
				  image.data[3*i+0] = 255;
				  image.data[3*i+1] = 255-lb;
				  image.data[3*i+2] = 255-lb;
				  break;
			  case 1:
				  image.data[3*i+0] = 255;
				  image.data[3*i+1] = lb;
				  image.data[3*i+2] = 0;
				  break;
			  case 2:
				  image.data[3*i+0] = 255-lb;
				  image.data[3*i+1] = 255;
				  image.data[3*i+2] = 0;
				  break;
			  case 5:
				  image.data[3*i+0] = 0;
				  image.data[3*i+1] = 0;
				  image.data[3*i+2] = 255-lb;
				  break;
			  default:
				  image.data[3*i+0] = 0;
				  image.data[3*i+1] = 0;
				  image.data[3*i+2] = 0;
				  break;
		  }
	   }
}

/////////////////////////////////////////////////
void DepthCameraPlugin::OnNewDepthFrame(const float *_image,
    unsigned int _width, unsigned int _height,
    unsigned int _depth, const std::string &_format)
{
    cycle = 100;
    gettimeofday(&a, NULL);
    totala = a.tv_sec * 1000000 + a.tv_usec;             


	if(count == 0){
		count++;
		std::string name = this->parentSensor->GetName();
		nameKinect = std::string("--Ice.Config=" + name + ".cfg");
		pthread_t thr_gui;
		pthread_create(&thr_gui, NULL, &mainKinect, (void*)this);
	    gzerr << "Creating Kinect\n";
        cloud->points.resize(_width*_height);
	}

	double hfov = 1.04719755;
	double fl = ((double)_width) / (2.0 *tan(hfov/2.0));
	double pointCloudCutoff = 0.001;

	//std::cout << "depth: " << (int)(_image[0]*1000) << std::endl;
/*
	cv::Mat image;

	image.create(_height, _width, CV_32FC1);
	imageDepth.create(_height, _width, CV_8UC1);
	
	pthread_mutex_lock (&mutex);
    memcpy( (float *)image.data,(float *) _image, _width*_height*4 );
	image.convertTo(imageDepth, CV_8UC1, 255, 0);
	cv::cvtColor(imageDepth, imageDepth, CV_GRAY2RGB);
	//cv::imshow("profundidad", imageDepth);
	//cv::waitKey(10);
*/

	double pAngle;
    double yAngle;


	pthread_mutex_lock (&kinect->mutex);
    //cloud->points.resize(_width*_height);
    
    int indicePunto = 0;
	pcl::PointXYZRGBA point;
	point.r      = 255;
	point.g      = 0;
	point.b      = 0;
    
	for(int x = 0 ; x < _width ; x++){
		for(int y = 0; y < _height; y++){
			int indice = y*_width + x;

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
	sor.setLeafSize (0.05, 0.05, 0.05);

	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud2(new pcl::PointCloud<pcl::PointXYZRGBA>);

	sor.filter (*cloud2);

	*cloud = *cloud2;
	
	pthread_mutex_unlock (&kinect->mutex);
		
	//pthread_mutex_unlock (&mutex);

	//std::cout << "cloud: " << cloud->points.size() << std::endl;
	//if (!viewer.wasStopped())
	//  viewer.showCloud(cloud);
	
	gettimeofday(&b, NULL);
	totalb = b.tv_sec * 1000000 + b.tv_usec;

	diff = (totalb - totala) / 1000;
	diff = cycle - diff;

	if (diff < 10)
		diff = 10;

	sleep(diff/1000);  
	
}

/////////////////////////////////////////////////
void DepthCameraPlugin::OnNewRGBPointCloud(const float * _image,
                unsigned int _width, unsigned int _height,
                unsigned int _depth, const std::string &/*_format*/)
{
    cycle3 = 100;
    gettimeofday(&a3, NULL);
    totala3 = a3.tv_sec * 1000000 + a3.tv_usec; 
    
	gettimeofday(&b3, NULL);
	totalb3 = b3.tv_sec * 1000000 + b3.tv_usec;

	diff3 = (totalb3 - totala3) / 1000;
	diff3 = cycle3 - diff3;

	if (diff3 < 10)
		diff3 = 10;

	sleep(diff3/1000); 
    
    
}

/////////////////////////////////////////////////
void DepthCameraPlugin::OnNewImageFrame(const unsigned char * _image,
                              unsigned int _width,
                              unsigned int _height,
                              unsigned int _depth,
                              const std::string &_format)
{

    cycle2 = 100;
    gettimeofday(&a2, NULL);
    totala2 = a2.tv_sec * 1000000 + a2.tv_usec;  

  	//std::cout << "OnNewImageFrame Format: " << format << " Depth: " << _depth << std::endl;
	pthread_mutex_lock (&mutexRGB);
	imageRGB.create(_height, _width, CV_8UC3);
    memcpy((unsigned char *) imageRGB.data, &(_image[0]), _width*_height * 3);
	//cv::imshow("Color", imageRGB);
	//cv::waitKey(10);
	pthread_mutex_unlock (&mutexRGB);
	
	gettimeofday(&b2, NULL);
	totalb2 = b2.tv_sec * 1000000 + b2.tv_usec;

	diff2 = (totalb2 - totala2) / 1000;
	diff2 = cycle2 - diff2;

	if (diff2 < 10)
		diff2 = 10;

	sleep(diff2/1000); 

}

   class KinectI: virtual public jderobot::pointCloud{
   public:
		KinectI (std::string propertyPrefix, const jderobotice::Context& context, gazebo::DepthCameraPlugin* kinect):prefix(propertyPrefix),context(context) 
		{
			  KData = new jderobot::pointCloudData();
			  this->kinect = kinect;
		}
		
		virtual jderobot::pointCloudDataPtr getCloudData(const Ice::Current&){
		      
			pthread_mutex_lock (&kinect->mutex);
            pcl::PointCloud<pcl::PointXYZRGBA> cloud = *kinect->cloud;
			pthread_mutex_unlock (&kinect->mutex);
			
            if(cloud.points.size()){
               KData->p.resize(cloud.points.size());
               int index = 0;
               for(int i = 0; i < cloud.points.size(); i++){
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
         jderobot::pointCloudDataPtr KData;
	     gazebo::DepthCameraPlugin* kinect;
		 std::string prefix;
		 jderobotice::Context context;

 }; //end class KinectI

class CameraI: virtual public jderobot::Camera {
	public:
		CameraI(std::string propertyPrefix, const jderobotice::Context& context, gazebo::DepthCameraPlugin* camera)
			   : prefix(propertyPrefix),context(context), cameraI(camera) {
		
			std::cout << "Constructor CameraI" << std::endl;

			imageDescription = (new jderobot::ImageDescription());

        	replyTask = new ReplyTask(this);
		    replyTask->start(); // my own thread
		  
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

	private:
		class ReplyTask: public gbxiceutilacfr::SafeThread {
			public:
				ReplyTask(CameraI* camera)
				: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycamera(camera) {
				   	std::cout << "safeThread" << std::endl;
				}

				void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
					IceUtil::Mutex::Lock sync(requestsMutex);
					requests.push_back(cb);
				}

				virtual void walk(){
					jderobot::ImageDataPtr reply(new jderobot::ImageData);
					struct timeval a, b;
					int cycle = 48;
					long totalb,totala;
					long diff;
					
					int count =0 ;

					while(!isStopping()){
						
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
		jderobotice::Context context;
		colorspaces::Image::FormatPtr imageFmt;
		jderobot::ImageDescriptionPtr imageDescription;
		jderobot::CameraDescriptionPtr cameraDescription;
		ReplyTaskPtr replyTask;
		gazebo::DepthCameraPlugin* cameraI;	
		
}; // end class CameraI


class CameraII: virtual public jderobot::Camera {
	public:
		CameraII(std::string propertyPrefix, const jderobotice::Context& context, gazebo::DepthCameraPlugin* camera)
			   : prefix(propertyPrefix),context(context), cameraI(camera) {
		
			std::cout << "Constructor CameraDepth" << std::endl;

			imageDescription = (new jderobot::ImageDescription());

        	replyTask = new ReplyTask(this);
		    replyTask->start(); // my own thread
		  
		}

		std::string getName () {
			return (cameraDescription->name);
		}

		std::string getRobotName () {
			return ((context.properties())->getProperty(context.tag()+".RobotName"));
		}

		virtual ~CameraII() {
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
				ReplyTask(CameraII* camera)
				: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycamera(camera) {
				   	std::cout << "safeThread" << std::endl;
				}

				void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
					IceUtil::Mutex::Lock sync(requestsMutex);
					requests.push_back(cb);
				}

				virtual void walk(){
					jderobot::ImageDataPtr reply(new jderobot::ImageData);
					struct timeval a, b;
					int cycle = 48;
					long totalb,totala;
					long diff;
					
					int count =0 ;

					while(!isStopping()){
						
						if(!mycamera->cameraI->imageDepth.data){
							usleep(100);
							continue;
						}
						if(count==0){
							pthread_mutex_lock (&mycamera->cameraI->mutex);
							mycamera->imageDescription->width = mycamera->cameraI->imageDepth.cols;
							mycamera->imageDescription->height = mycamera->cameraI->imageDepth.rows;
							mycamera->imageDescription->size = mycamera->cameraI->imageDepth.cols*mycamera->cameraI->imageDepth.rows*3;
							pthread_mutex_unlock (&mycamera->cameraI->mutex);

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
          				
          				pthread_mutex_lock (&mycamera->cameraI->mutex);
					    reply->pixelData.resize(mycamera->cameraI->imageDepth.rows*mycamera->cameraI->imageDepth.cols*3);
					    
					    memcpy( &(reply->pixelData[0]), (unsigned char *) mycamera->cameraI->imageDepth.data, mycamera->cameraI->imageDepth.rows*mycamera->cameraI->imageDepth.cols*3);
						pthread_mutex_unlock (&mycamera->cameraI->mutex);

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
		jderobotice::Context context;
		colorspaces::Image::FormatPtr imageFmt;
		jderobot::ImageDescriptionPtr imageDescription;
		jderobot::CameraDescriptionPtr cameraDescription;
		ReplyTaskPtr replyTask;
		gazebo::DepthCameraPlugin* cameraI;	
		
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
        
        ic = Ice::initialize(argc, argv);
        prop = ic->getProperties();
        
        std::string Endpoints = prop->getProperty("Kinect.Endpoints");
        std::cout << "Kinect Endpoints > " << Endpoints << std::endl;
        
        Ice::ObjectAdapterPtr adapter =
            ic->createObjectAdapterWithEndpoints("Kinect", Endpoints);

		jderobotice::Context context;

        Ice::ObjectPtr object = new KinectI(std::string("KinectGazebo"), context, kinect);
        //Ice::ObjectPtr object2 = new CameraI(std::string("KinectGazebo"), context, kinect);
        //Ice::ObjectPtr object3 = new CameraII(std::string("KinectGazebo"), context, kinect);

        adapter->add(object, ic->stringToIdentity("Kinect"));
        //adapter->add(object2, ic->stringToIdentity("KinectRGB"));
        //adapter->add(object3, ic->stringToIdentity("KinectDepth"));
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

}

