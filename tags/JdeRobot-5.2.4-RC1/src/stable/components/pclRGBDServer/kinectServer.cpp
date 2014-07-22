#define  EIGEN_DONT_VECTORIZE 
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

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

#include "cameradepth.h"
#include "cameraRGB.h"
#include "pointcloud.h"

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



int avRGB = 0;
int avDepth = 0;
int avCloud = 0;

int leafSize = 0.1f;

namespace kinect{

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

   depth2rgb(image_w->getDepthMetaData().Data(), imageDepth);
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
        usleep(100);
    }  
}

}

int main(int argc, char** argv) 
{
    kinect::raw2depth();

    Ice::ObjectPtr kinect_RGB;
    Ice::ObjectPtr kinect_Depth;
    Ice::ObjectPtr kinect_Cloud;
    
    Ice::CommunicatorPtr ic;
    Ice::PropertiesPtr prop;
    try {
        //-----------------ICE----------------//
        ic = Ice::initialize(argc, argv);

        Ice::PropertiesPtr prop = ic->getProperties();
			
        avRGB = prop->getPropertyAsInt("KinectServer.CameraRGB");

        std::string Endpoints = prop->getProperty("KinectServer.Endpoints");

        Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints("kinect", Endpoints);

        if(avRGB){
            std::string kinectRGB = "kinectRGB";
            std::cout << "Creating Camera " + kinectRGB << std::endl;
            kinect_RGB = new kinect::CamaraRGB(kinectRGB);
            adapter->add(kinect_RGB, ic->stringToIdentity(kinectRGB));
        }
        
        avDepth = prop->getPropertyAsInt("KinectServer.CameraDepth");
        if(avDepth){
            std::string kinectDepth= "kinectDepth";
            std::cout << "Creating Camera " + kinectDepth << std::endl;
            kinect_Depth = new kinect::CamaraDepth(kinectDepth);
            adapter->add(kinect_Depth, ic->stringToIdentity(kinectDepth));
        }
        
        avCloud = prop->getPropertyAsInt("KinectServer.CloudPoints");
        if(avCloud){
            std::string kinectCloud = "kinect1";
            std::cout << "Creating Camera " + kinectCloud << std::endl;
            kinect_Cloud = new kinect::KinectI(kinectCloud);
            adapter->add(kinect_Cloud, ic->stringToIdentity(kinectCloud));
        }
        
        adapter->activate();   
        
        std::string s = prop->getPropertyWithDefault("KinectServer.LeafSize", "0.1");
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
        rc = pthread_join(threads[0], &status);

    
    } catch (const Ice::Exception& ex) {
        std::cerr <<  ex << std::endl;
        exit(-1);
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        exit(-1);
    }
/*
    avCloud = prop->getPropertyAsInt("KinectServer.CloudPoints");
    if(avCloud){
        std::string kinectCloud = "kinect1";
        std::cout << "Creating Camera " + kinectCloud << std::endl;
        kinect_Cloud = new KinectI(kinectCloud, context());
        context().createInterfaceWithString(kinect_Cloud, kinectCloud);
    }
*/

}
