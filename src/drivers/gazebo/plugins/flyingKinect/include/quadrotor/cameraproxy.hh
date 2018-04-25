/*
 *  Copyright (C) 1997-2015 JDE Developers Team
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

#ifndef CAMERAPROXY
#define CAMERAPROXY

#include <boost/shared_ptr.hpp>
#include <gazebo/sensors/CameraSensor.hh>
#include <opencv2/core/core.hpp>

namespace quadrotor{

class ICameraConsumer{
public:
    virtual void onCameraSensorBoostrap(const cv::Mat, const gazebo::sensors::CameraSensorPtr)=0;
    virtual void onCameraSensorUpdate(const cv::Mat)=0;
};

typedef boost::shared_ptr<ICameraConsumer> ICameraConsumerPtr;

class CameraProxy{
public:
    CameraProxy();
    virtual ~CameraProxy();

    void registerCamera(gazebo::sensors::CameraSensorPtr cam);
    void setActive(int id);
    int getActive();
    void next();

    void registerConsumer(ICameraConsumerPtr consumer);

protected:
    /// overrideable method that acts as pasarell between gazebo's raw image format and cv::Mat
    virtual cv::Mat _createImageWrapper(const gazebo::sensors::CameraSensorPtr cam, const int id) const;

private:
    void _on_cam_bootstrap();
    void _on_cam_update();


private:
    boost::mutex lock;
    int active_camera = -1;
    gazebo::event::ConnectionPtr active_sub;

    cv::Mat img;

    std::vector<gazebo::sensors::CameraSensorPtr> cameras;
    std::vector<ICameraConsumerPtr> consumers;
};

}//NS

#endif // CAMERAPROXY

