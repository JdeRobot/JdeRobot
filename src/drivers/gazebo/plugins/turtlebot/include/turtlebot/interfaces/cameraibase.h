/*
 *  Copyright (C) 1997-2017 JDE Developers Team
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
 *  CLONE of https://github.com/jderobot-varribas/gazeboplugin-quadrotor2/blob/2.1.0/include/quadrotor/interfaces/cameraibase.h
 *  Victor Arribas Raigadas <v.arribas.urjc@gmai.com>
 *  
 */

#ifndef CAMERAIBASE_H
#define CAMERAIBASE_H

#include <list>
#include <iostream>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

#include <jderobot/camera.h>

#include <turtlebot/debugtools.h>


// Fix name
namespace jderobot{ typedef ImageFormat ImageFormats; }


namespace turtlebot{
namespace interfaces{


class CameraIBase: virtual public jderobot::Camera {
public:
    CameraIBase();
    virtual ~CameraIBase();

    /// jderobot::ImageProvider
    jderobot::ImageFormats getImageFormat(const Ice::Current& c); //bad name... getSupportedFormats please
    jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c);
    void getImageData_async (const jderobot::AMD_ImageProvider_getImageDataPtr& cb,const std::string& format, const Ice::Current& c);
    virtual void _getImageData_async (const jderobot::AMD_ImageProvider_getImageDataPtr& cb,const std::string& format, const Ice::Current& c);


    /// jderobot::Camera
    jderobot::CameraDescriptionPtr getCameraDescription(const Ice::Current& c);
    virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr &/*description*/, const Ice::Current& /*c*/) { return 0; }


    /// jderobot::StreamableCamera
    virtual std::string startCameraStreaming(const Ice::Current&){ return ""; }
    virtual void stopCameraStreaming(const Ice::Current&) {}
    virtual void reset(const Ice::Current&){}

protected:
    boost::mutex mutex;
    typedef boost::lock_guard<boost::mutex> lock_guard_t;

protected:
    /// Ice
    jderobot::ImageDataPtr imageData;
    jderobot::ImageFormats imageFormats;
    jderobot::ImageDescriptionPtr imageDescription;
    jderobot::CameraDescriptionPtr cameraDescription;
};


}}//NS

#endif // CAMERAIBASE_H

