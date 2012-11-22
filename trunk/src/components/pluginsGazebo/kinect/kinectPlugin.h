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

#include "common/Plugin.hh"
#include "sensors/DepthCameraSensor.hh"
#include "sensors/CameraSensor.hh"
#include "rendering/DepthCamera.hh"
#include "gazebo.hh"

#include <pcl/visualization/cloud_viewer.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>

// ICE utils includes
#include <gbxsickacfr/gbxiceutilacfr/safethread.h>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

// JDErobot general ice component includes
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <jderobotice/exceptions.h>
#include <jderobotice/context.h>

#include <colorspaces/colorspacesmm.h>

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

    protected: unsigned int width, height, depth;
    protected: std::string format;

    protected: sensors::DepthCameraSensorPtr parentSensor;
    protected: rendering::DepthCameraPtr depthCamera;

    private: event::ConnectionPtr newDepthFrameConnection;
    private: event::ConnectionPtr newRGBPointCloudConnection;
    private: event::ConnectionPtr newImageFrameConnection;
	//private: pcl::visualization::CloudViewer viewer;
	private: int count;
	public:
		std::string nameKinect;
	    pthread_mutex_t mutex;
	    pthread_mutex_t mutexRGB;
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud;
		cv::Mat imageRGB;
		cv::Mat imageDepth;
  };
}
#endif
