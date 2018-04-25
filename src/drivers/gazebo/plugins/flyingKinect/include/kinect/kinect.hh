/*
 *  Copyright (C) 2015-2016 JDE Developers Team
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
 *  Authors :
 *       Victor Arribas Raigadas <v.arribas.urjc@gmai.com>
 */


// Gazebo
#include <gazebo/common/common.hh>
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/Link.hh>

#include <sensors/DepthCameraSensor.hh>
#include <rendering/DepthCamera.hh>

// Ice
#include <Ice/Ice.h>

// Jderobot
#include <jderobot/pose3d.h>
#include <quadrotor/interfaces/pushcamerai.h>
#include <kinect/pointcloudi.hpp>



namespace kinect{

#define PARENT_SENSOR_GETS_UPDATES

class KinectPlugin :
        public gazebo::ModelPlugin,
        public jderobot::Pose3D
{
public:
  	KinectPlugin();
	virtual ~KinectPlugin();

    std::string _log_prefix;

private:


/// Gazebo
protected:
    void Load(gazebo::physics::ModelPtr _model, sdf::ElementPtr _sdf);
    void Init();
	void OnUpdate(const gazebo::common::UpdateInfo & /*_info*/);
	void Reset();
	void OnSigInt();
	void LoadSensors(gazebo::physics::ModelPtr _model);

private:
    gazebo::physics::ModelPtr model;
    gazebo::event::ConnectionPtr sigintConnection;
    uint32_t base_link_id;

/// Gazebo (pose)
    void _on_pose_update();

/// Gazebo (cameras)
#ifdef PARENT_SENSOR_GETS_UPDATES
private:
    void _on_cam_bootstrap();
    void _on_cam_update();
#else
    void _on_cam_bootstrap_depth_data(const float *_data, unsigned int _width, unsigned int _height,
                                    unsigned int, const std::string &);
    void _on_cam_update_depth_data(const float *, unsigned int, unsigned int,
                                   unsigned int, const std::string &);
    void _on_cam_bootstrap_rgb_data(const unsigned char * _data, unsigned int _width, unsigned int _height,
                                      unsigned int, const std::string &);
    void _on_cam_update_rgb_data(const unsigned char *, unsigned int, unsigned int,
                                   unsigned int, const std::string &);
#endif

private:
    gazebo::sensors::DepthCameraSensorPtr camera_sensor;
    gazebo::rendering::DepthCameraPtr camera_impl;

#ifdef PARENT_SENSOR_GETS_UPDATES
    gazebo::event::ConnectionPtr sub_camera;
#else
    gazebo::event::ConnectionPtr sub_cam_depth;
    gazebo::event::ConnectionPtr sub_cam_rgb;
    gazebo::event::ConnectionPtr sub_cam_pcd;
#endif

    cv::Mat img_depth;
    cv::Mat img_depth_raw;
    cv::Mat img_rgb;

/// Ice
    void InitializeIce(sdf::ElementPtr _sdf);

protected:
    Ice::CommunicatorPtr ic;


/// Ice (Pose3D)
private:
    jderobot::Pose3DDataPtr ice_pose3ddata;
public:
    jderobot::Pose3DDataPtr getPose3DData ( const Ice::Current& );

    Ice::Int setPose3DData ( const jderobot::Pose3DDataPtr & pose3dData,
                                     const Ice::Current& );

/// Ice (PCL)
private:
    jderobot::interfaces::PointCloudI point_cloudI;

/// Ice (Camera)
private:
    quadrotor::interfaces::PushCameraI cam_depthI;
    quadrotor::interfaces::PushCameraI cam_rgbI;

};

}//NS
