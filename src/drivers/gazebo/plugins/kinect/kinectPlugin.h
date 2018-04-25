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

#ifndef __GAZEBO_DEPTH_CAMERA_PLUGIN_HH__
#define __GAZEBO_DEPTH_CAMERA_PLUGIN_HH__

#include <string>

#include <common/Plugin.hh>
#include <sensors/DepthCameraSensor.hh>
#include <sensors/CameraSensor.hh>
#include <rendering/DepthCamera.hh>
#include <gazebo.hh>

#include <pcl/visualization/cloud_viewer.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>

// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <easyiceconfig/EasyIce.h> 

#include <visionlib/colorspaces/colorspacesmm.h>

#include <jderobot/pointcloud.h>
#include <jderobot/camera.h>

//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define NUM_THREADS     5
#define MAX_DEPTH 10000

namespace gazebo
{
	unsigned short depth[MAX_DEPTH];


  class DepthCameraPlugin : public SensorPlugin
  {
    public: DepthCameraPlugin();

    public: void Load(sensors::SensorPtr _sensor, sdf::ElementPtr _sdf);

    public: virtual void OnNewDepthFrame(const float *_image,
                unsigned int _width, unsigned int _height,
                unsigned int _depth, const std::string &_format);

    /// \brief Update the controller
    public: virtual void OnNewRGBPointCloud(const float *_pcd,
                unsigned int _width, unsigned int _height,
                unsigned int _depth, const std::string &_format);

    public: virtual void OnNewImageFrame(const unsigned char *_image,
                              unsigned int _width, unsigned int _height,
                              unsigned int _depth, const std::string &_format);

    public: virtual void SetLeafSize(const float size);

    protected: unsigned int width, height, depth;
    protected: float leafSize;
    protected: std::string format;

    protected: sensors::DepthCameraSensorPtr parentSensor;
    protected: rendering::DepthCameraPtr depthCamera;

    private: event::ConnectionPtr newDepthFrameConnection;
    private: event::ConnectionPtr newRGBPointCloudConnection;
    private: event::ConnectionPtr newImageFrameConnection;
	//private: pcl::visualization::CloudViewer viewer;
	
	void depth2rgb(cv::Mat image);
	private: int count;
	public:
		std::string nameKinect;
	    pthread_mutex_t mutex;
	    pthread_mutex_t mutexRGB;
	    pthread_mutex_t mutexDepth;
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud;
		cv::Mat imageRGB;
		cv::Mat imageDepth;
		
        int cycle;
        long totalb, totala, diff;
        struct timeval a, b;   
        
        int cycle2;
        long totalb2, totala2, diff2;
        struct timeval a2, b2;  
        
        int cycle3;
        long totalb3, totala3, diff3;
        struct timeval a3, b3;   
  };
}
#endif
