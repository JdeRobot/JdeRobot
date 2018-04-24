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
 *  CLONE of https://github.com/jderobot-varribas/gazeboplugin-quadrotor2/blob/2.1.0/src/interfaces/cameraibase.cpp
 *  Victor Arribas Raigadas <v.arribas.urjc@gmai.com>
 *  
 */

#include <turtlebot/interfaces/cameraibase.h>

using namespace turtlebot::interfaces;
using namespace jderobot;


CameraIBase::CameraIBase():
	imageDescription(new ImageDescription()),
	cameraDescription(new CameraDescription())
{
    ONDEBUG_INFO(std::cout << "Constructor CameraIBase" << std::endl;)
}

CameraIBase::~CameraIBase(){
}


ImageDescriptionPtr
CameraIBase::getImageDescription(const Ice::Current& /*c*/){
    ONDEBUG_VERBOSE(std::cout<<"CameraIBase::getImageDescription()"<<std::endl;)
    lock_guard_t RAII_lock(mutex);
    if (!imageDescription)
        throw JderobotException("not initialized");

	return imageDescription;
}


ImageFormats
CameraIBase::getImageFormat(const Ice::Current& /*c*/) {
    ONDEBUG_VERBOSE(std::cout<<"CameraIBase::getImageFormat()"<<std::endl;)
    lock_guard_t RAII_lock(mutex);
    if (imageFormats.empty())
        throw JderobotException("not initialized");

	return imageFormats;
}


void
CameraIBase::getImageData_async (const jderobot::AMD_ImageProvider_getImageDataPtr& cb,const std::string& format, const Ice::Current& c){
    ONDEBUG_VERBOSE(std::cout<<"CameraIBase::getImageData_async()"<<std::endl;)
    lock_guard_t RAII_lock(mutex);
    if (std::find(imageFormats.begin(), imageFormats.end(), format) == imageFormats.end())
        throw JderobotException("format not supported"); // UnknownUserException: unknown = jderobot::JderobotException because it is not declared into .ice

    _getImageData_async(cb, format, c);
}

void
CameraIBase::_getImageData_async (const jderobot::AMD_ImageProvider_getImageDataPtr& cb,const std::string& /*format*/, const Ice::Current& /*c*/){
    //Non async response
    if (!imageData)
        throw DataNotExistException();

    cb->ice_response(imageData);
}


CameraDescriptionPtr
CameraIBase::getCameraDescription(const Ice::Current& /*c*/){
    ONDEBUG_VERBOSE(std::cout<<"CameraIBase::getCameraDescription()"<<std::endl;)
    lock_guard_t RAII_lock(mutex);
    if (!cameraDescription)
        throw new JderobotException("not initialized");

    return cameraDescription;
}
