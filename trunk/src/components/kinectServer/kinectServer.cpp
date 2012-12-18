#define  EIGEN_DONT_VECTORIZE 
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <jderobotice/component.h>
#include <jderobotice/application.h>

#include <jderobot/pointcloud.h>
#include <jderobot/camera.h>

#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/common/time.h> //fps calculations 
#include <pcl/io/openni_grabber.h>
#include <pcl/io/openni_camera/openni_driver.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/voxel_grid.h>

#include <gbxsickacfr/gbxiceutilacfr/safethread.h>

#include <iostream>
#include <pthread.h>

// opencv Libraries
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//#include <cv.h>
//#include <highgui.h>

#define NUM_THREADS     5
#define MAX_DEPTH 10000


namespace kinect{

int avRGB = 0;
int avDepth = 0;
int avCloud = 0;

int leafSize = 0.1f;

cv::Mat imageRGB;
cv::Mat imageDepth;
pthread_mutex_t mutexRGB;
pthread_mutex_t mutexDepth;
pthread_mutex_t mutex;
//pcl::visualization::CloudViewer viewer("Cloud RGB"); 
pcl::PointCloud<pcl::PointXYZRGBA>cloud2;

unsigned short depth[10000];

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

void depth2rgb( const XnDepthPixel*  Xn_disparity, cv::Mat image){
   int i;
   //const unsigned short *disparity = Xn_disparity;
    
   for (i=0; i<image.rows* image.cols; i++) {
      //std::cout << i << std::endl;
      int pval = depth[Xn_disparity[i]];
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
	      case 3:
		      image.data[3*i+0] = 0;
		      image.data[3*i+1] = 255;
		      image.data[3*i+2] = lb;
		      break;
	      case 4:
		      image.data[3*i+0] = 0;
		      image.data[3*i+1] = 255-lb;
		      image.data[3*i+2] = 255;
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

void rgb_image_cb (const boost::shared_ptr<openni_wrapper::Image>& image)
{
   //cout << "Callback RGB--" <<endl;
   boost::shared_ptr<openni_wrapper::Image> image_w;
   image_w = image;

   pthread_mutex_lock(&mutexRGB);
   imageRGB.create(image_w->getHeight(), image_w->getWidth(), CV_8UC3);

   image_w->fillRGB( image_w->getWidth(), image_w->getHeight(),  (unsigned char *) imageRGB.data, imageRGB.step);

   //cv::cvtColor(imageRGB, imageRGB, CV_RGB2BGR);
   pthread_mutex_unlock(&mutexRGB);


}

void depth_image_cb (const boost::shared_ptr<openni_wrapper::DepthImage>& depth_image)
{

   boost::shared_ptr<openni_wrapper::DepthImage> image_w;
   image_w = depth_image;
   pthread_mutex_lock(&mutexDepth);
   imageDepth.create(image_w->getHeight(), image_w->getWidth(), CV_8UC3);

   depth2rgb(image_w->getDepthMetaData ().Data(), imageDepth);
   pthread_mutex_unlock(&mutexDepth);
   
}

pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloudaux(new pcl::PointCloud<pcl::PointXYZRGBA>);
void cloud_cb_ (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloud)
{ 

   pthread_mutex_lock(&mutex);
   
   float size = 1./leafSize;

   if(cloud->points.size()>0){
      //viewer.showCloud (cloud); 
      pcl::VoxelGrid<pcl::PointXYZRGBA> sor;
      sor.setInputCloud (cloud);
      sor.setLeafSize (size, size, size);
      sor.filter (*cloudaux);
      //std::cout << "Reducidad: "<< cloudaux->points.size() << std::endl;
      
      cloud2 = *cloudaux;
   }
   pthread_mutex_unlock(&mutex);
   
}


void* kinectThread(void*)
{
   pcl::Grabber* interface = new pcl::OpenNIGrabber(); 
   
   if(avRGB){
   
      boost::function<void (const boost::shared_ptr<openni_wrapper::Image>&)> f =
                              boost::bind (rgb_image_cb, _1);
      interface->registerCallback (f); 
   }
   
   if(avDepth){                        
      boost::function<void (const boost::shared_ptr<openni_wrapper::DepthImage>&)> f_depth =
                              boost::bind (depth_image_cb, _1);
      interface->registerCallback (f_depth);  
   }    
   
   if(avCloud){                    
      boost::function<void (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr&)> f_cloud =
                                 boost::bind (cloud_cb_, _1);
                                 
      interface->registerCallback (f_cloud);                                  
   } 

   pthread_mutex_init(&mutexRGB, NULL);
   pthread_mutex_init(&mutex, NULL);
   pthread_mutex_init(&mutexDepth, NULL);
   
   interface->start();
   
   
   
   while(1){
   
      /*pthread_mutex_lock(&mutexRGB);
      if(imageRGB.data)
         cv::imshow("RGB", imageRGB);
      pthread_mutex_unlock(&mutexRGB);
      
      pthread_mutex_lock(&mutexDepth);
      if(imageDepth.data)
         cv::imshow("Depth", imageDepth);
      pthread_mutex_unlock(&mutexDepth);
      
      cv::waitKey(10);
      */
      usleep(10);
   }
   
}

class CamaraDepth: virtual public jderobot::Camera {
	public:
		CamaraDepth(std::string& propertyPrefix, const jderobotice::Context& context)
		: prefix(propertyPrefix),context(context) {
		   
		   imageDescription = (new jderobot::ImageDescription());
		   //cameraDescription = (new jderobot::CameraDescription());
		
         replyTask = new ReplyTask(this);
		   replyTask->start(); // my own thread
		}

		std::string getName () {
			return (cameraDescription->name);
		}

		std::string getRobotName () {
			return ((context.properties())->getProperty(context.tag()+".RobotName"));
		}

		virtual ~CamaraDepth() {
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
				ReplyTask(CamaraDepth* camera)
				: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycamera(camera) {
				}

				void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
					IceUtil::Mutex::Lock sync(requestsMutex);
					requests.push_back(cb);
				}
             

				virtual void walk(){
 				   cv::Mat image;
 				   
 				   while(!imageDepth.data)
 				      usleep(1000);
 				      
 				   if(imageDepth.data){
		            pthread_mutex_lock(&mutexRGB);
                  image = imageDepth.clone();
                  pthread_mutex_unlock(&mutexRGB);
                  mycamera->imageDescription->width = image.cols;
                  mycamera->imageDescription->height = image.rows;
                  mycamera->imageDescription->size = image.cols*image.rows*3;
                  mycamera->imageDescription->format = "RGB8";
 				   }
               
				
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

	               pthread_mutex_lock(&mutexRGB);
                  cv::Mat image = imageDepth.clone();
                  pthread_mutex_unlock(&mutexRGB);

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

						//std::cout << "Gazeboserver takes " << diff << " ms " << mycamera->fileName << std::endl;

						if(diff < 33)
							diff = 33;


						//Sleep Algorithm//
						usleep(diff*1000);
					}
					
				}
				
				
				CamaraDepth* mycamera;
				IceUtil::Mutex requestsMutex;
				std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
		};

		typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;
		std::string prefix;
		jderobotice::Context context;
		jderobot::ImageDescriptionPtr imageDescription;
		jderobot::CameraDescriptionPtr cameraDescription;
		ReplyTaskPtr replyTask;

	}; // end class CamaraDepth



class CamaraRGB: virtual public jderobot::Camera {
	public:
		CamaraRGB(std::string& propertyPrefix, const jderobotice::Context& context)
		: prefix(propertyPrefix),context(context) {
		   
		   imageDescription = (new jderobot::ImageDescription());
		   //cameraDescription = (new jderobot::CameraDescription());
		
         replyTask = new ReplyTask(this);
		   replyTask->start(); // my own thread
		}

		std::string getName () {
			return (cameraDescription->name);
		}

		std::string getRobotName () {
			return ((context.properties())->getProperty(context.tag()+".RobotName"));
		}

		virtual ~CamaraRGB() {
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
				ReplyTask(CamaraRGB* camera)
				: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycamera(camera) {
				}

				void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
					IceUtil::Mutex::Lock sync(requestsMutex);
					requests.push_back(cb);
				}
             

				virtual void walk(){
 				   cv::Mat image;
 				   
 				   while(!imageRGB.data)
 				      usleep(1000);
 				      
 				   if(imageRGB.data){
		            pthread_mutex_lock(&mutexRGB);
                  image = imageRGB.clone();
                  pthread_mutex_unlock(&mutexRGB);
                  mycamera->imageDescription->width = image.cols;
                  mycamera->imageDescription->height = image.rows;
                  mycamera->imageDescription->size = image.cols*image.rows*3;
                  mycamera->imageDescription->format = "RGB8";
 				   }
               
				
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

	               pthread_mutex_lock(&mutexRGB);
                  cv::Mat image = imageRGB.clone();
                  pthread_mutex_unlock(&mutexRGB);

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

						//std::cout << "RGB takes " << diff << " ms " << std::endl;

						if(diff < 33)
							diff = 33;


						//Sleep Algorithm//
						usleep(diff*1000);
					}
					
				}
				
				
				CamaraRGB* mycamera;
				IceUtil::Mutex requestsMutex;
				std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
		};

		typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;
		std::string prefix;
		jderobotice::Context context;
		jderobot::ImageDescriptionPtr imageDescription;
		jderobot::CameraDescriptionPtr cameraDescription;
		ReplyTaskPtr replyTask;

	}; // end class CamaraRGB
	
   class KinectI: virtual public jderobot::pointCloud{
   public:
		KinectI (std::string& propertyPrefix, const jderobotice::Context& context):
			prefix(propertyPrefix),context(context) {
				
			  KData = new jderobot::pointCloudData();
			  Ice::PropertiesPtr prop = context.properties();
              v=NULL;
              v  = new CloudViewer(this);
              v->start();
			}
		
		virtual jderobot::pointCloudDataPtr getCloudData(const Ice::Current&){
		      
            pcl::PointCloud<pcl::PointXYZRGBA> cloud = v->getCloud();

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


	   
	   private:
	      class CloudViewer :public gbxiceutilacfr::SafeThread{ 
            public: 
            CloudViewer (KinectI* kinect) : gbxiceutilacfr::SafeThread(kinect->context.tracer()) 
             {}
             
            
             void walk()
             { 
             }
             
             
            pcl::PointCloud<pcl::PointXYZRGBA> getCloud()
            {
               pcl::PointCloud<pcl::PointXYZRGBA>cloud;
               pthread_mutex_lock(&mutex);
               cloud = cloud2;
               pthread_mutex_unlock(&mutex);
               return cloud;
            }
           
         };
         typedef IceUtil::Handle<CloudViewer> CloudViewerPtr;
         CloudViewerPtr v; 
    	 std::string prefix;
	     jderobotice::Context context;
         jderobot::pointCloudDataPtr KData;

     }; //end class KinectI


	class Component: public jderobotice::Component {
		public:
		Component():jderobotice::Component("KinectServer"){}

      virtual void start() {
 
		 Ice::PropertiesPtr prop = context().properties();
			
         avRGB = prop->getPropertyAsInt(context().tag() + ".CameraRGB");

         if(avRGB){
			   std::string kinectRGB = "cameraRGB";
			   context().tracer().info("Creating Camera " + kinectRGB);
			   kinect_RGB = new CamaraRGB(kinectRGB, context());
			   context().createInterfaceWithString(kinect_RGB, kinectRGB);
         }
         
         avDepth = prop->getPropertyAsInt(context().tag() + ".CameraDepth");

         if(avDepth){
			   std::string kinectDepth= "cameraDepth";
			   context().tracer().info("Creating Camera " + kinectDepth);
			   kinect_Depth = new CamaraDepth(kinectDepth, context());
			   context().createInterfaceWithString(kinect_Depth, kinectDepth);
		   }
         
         avCloud = prop->getPropertyAsInt(context().tag() + ".CloudPoints");
         if(avCloud){
			   std::string kinectCloud = "pointcloud1";
			   context().tracer().info("Creating Cloud " + kinectCloud);
			   kinect_Cloud = new KinectI(kinectCloud, context());
			   context().createInterfaceWithString(kinect_Cloud, kinectCloud);
         }
         
         std::string s = prop->getPropertyWithDefault(context().tag() + ".LeafSize", "0.1");
         leafSize = (float)atof(s.c_str());
         
         cout << "leafSize: " << leafSize << endl;
         
         int rc;
         void *status;

         pthread_t threads[NUM_THREADS];
         rc =  pthread_create(&threads[0], NULL, &kinect::kinectThread, NULL);
         if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
         }
   
         
      }
      virtual ~Component() {}
      
      private:
         Ice::ObjectPtr kinect_RGB;
         Ice::ObjectPtr kinect_Depth;
         Ice::ObjectPtr kinect_Cloud;
   };

}
int main(int argc, char** argv) 
{

	kinect::Component component;
	jderobotice::Application app(component);
   kinect::raw2depth();
   
	return app.jderobotMain(argc,argv);
}
