
namespace kinect{
    pthread_mutex_t mutex;
    pcl::PointCloud<pcl::PointXYZRGBA>cloud2;

   class KinectI: virtual public jderobot::pointCloud{
   public:
		KinectI (std::string& propertyPrefix): prefix(propertyPrefix) {
				
			  KData = new jderobot::pointCloudData();

              v=NULL;
              v  = new CloudViewer(this);
              v->start();
			}
		
		virtual jderobot::pointCloudDataPtr getCloudData(const Ice::Current&){
		      
            pcl::PointCloud<pcl::PointXYZRGBA> cloud = v->getCloud();
            std::cout << "Cloud size: " << cloud.points.size() << std::endl;

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
	      class CloudViewer: public IceUtil::Thread{ 
            public: 
            CloudViewer (KinectI* kinect)
             {}
             
            
             virtual void run()
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
         jderobot::pointCloudDataPtr KData;

     }; //end class KinectI

}
