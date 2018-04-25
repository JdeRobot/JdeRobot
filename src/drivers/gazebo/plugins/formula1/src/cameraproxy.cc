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



#include <formula1/cameraproxy.hh>

using namespace formula1;


CameraProxy::CameraProxy(){

}

CameraProxy::~CameraProxy(){

}

void
CameraProxy::registerCamera(gazebo::sensors::CameraSensorPtr cam){
    cameras.push_back(cam);
}

void
CameraProxy::registerConsumer(ICameraConsumerPtr consumer){
    consumers.push_back(consumer);
}

void
CameraProxy::setActive(int id){
    lock.lock();

    /*
    /// deactivate previous
    if (active_camera != -1){
        cameras[active_camera]->SetActive(false);
        cameras[active_camera]->DisconnectUpdated(active_sub);
    }
    */

    /// active new
    if (id != -1){
        active_camera = id;
        active_sub = cameras[active_camera]->ConnectUpdated(
            boost::bind(&CameraProxy::_on_cam_bootstrap, this));
        cameras[0]->SetActive(true);
        cameras[1]->SetActive(true);
    }

    lock.unlock();
}

int
CameraProxy::getActive(){
    return active_camera;
}

void
CameraProxy::next(){
    int next = (active_camera+1)%cameras.size();
    setActive(next);
}

void
CameraProxy::_on_cam_bootstrap(){
    lock.lock();

    cameras[active_camera]->DisconnectUpdated(active_sub);

    img = _createImageWrapper(cameras[active_camera], active_camera);

    for (ICameraConsumerPtr consumer : consumers){
        consumer->onCameraSensorBoostrap(img, cameras[active_camera]);
    }

    active_sub = cameras[active_camera]->ConnectUpdated(
        boost::bind(&CameraProxy::_on_cam_update, this));
    lock.unlock();
}

void
CameraProxy::_on_cam_update(){
    lock.lock();

    for (ICameraConsumerPtr consumer : consumers){
        consumer->onCameraSensorUpdate(img);
    }

    lock.unlock();
}


cv::Mat
CameraProxy::_createImageWrapper(const gazebo::sensors::CameraSensorPtr cam, const int /*id*/) const{
    const unsigned char *data = cam->ImageData();
    uint32_t h = cam->ImageHeight();
    uint32_t w = cam->ImageWidth();
    return cv::Mat(h, w, CV_8UC3, (uint8_t*)data);
}
