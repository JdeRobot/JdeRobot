/*
 *
 *  Copyright (C) 1997-2011 JDERobot Developers Team
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
 *  Authors : Maikel González <m.gonzalezbai@gmail.com>
 *
 */

#include "control.h"

namespace introrob {

    Control::~Control() {
    }

    void Control::UpdateSensorsICE(Api *api) {

        api->motorVin = this->mprx->getV();
        //      if(api->motorVin!=0)
        //            std::cout << "v: " << this->mprx->getV() <<std::endl;

        api->motorWin = this->mprx->getW();
        //     if(api->motorWin!=0)
        //	   std::cout << "w: " << this->mprx->getW() <<std::endl;


        api->motorLin = this->mprx->getL();
        api->encodersData = this->eprx->getEncodersData();

        api->laserData = this->lprx->getLaserData();

        pthread_mutex_lock(&api->controlGui);
        api->imagesReady = FALSE;
        api->imageData1 = this->cprx1->getImageData();
        api->imageData2 = this->cprx2->getImageData();
        if (api->guiReady) {
            createImage(api);
            createImage2(api);
        }
        api->imagesReady = TRUE;
        pthread_mutex_unlock(&api->controlGui);

        api->Pose3Dencoders1 = this->p3deprx1->getPose3DEncodersData();


    }

    void Control::createImage(Api *api) {
        cvReleaseImage(&image);
        this->image = cvCreateImage(cvSize(api->imageData1->description->width, api->imageData1->description->height), 8, 3);

        memcpy((unsigned char *) image->imageData, &(api->imageData1->pixelData[0]), image->width * image->height * 3);
/*
        colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(api->imageData1->description->format);

        if (!fmt)
            throw "Format not supported";
*/
        api->imgBuff =
                Gdk::Pixbuf::create_from_data((const guint8*) this->image->imageData,
                Gdk::COLORSPACE_RGB,
                false,
                this->image->depth,
                this->image->width,
                this->image->height,
                this->image->widthStep);

    }

    void Control::createImage2(Api *api) {
        cvReleaseImage(&image2);
        this->image2 = cvCreateImage(cvSize(api->imageData2->description->width, api->imageData2->description->height), 8, 3);

        memcpy((unsigned char *) image2->imageData, &(api->imageData2->pixelData[0]), image2->width * image2->height * 3);
/*
        colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat(api->imageData2->description->format);

        if (!fmt2)
            throw "Format not supported";
*/
        api->imgBuff2 =
                Gdk::Pixbuf::create_from_data((const guint8*) this->image2->imageData,
                Gdk::COLORSPACE_RGB,
                false,
                this->image2->depth,
                this->image2->width,
                this->image2->height,
                this->image2->widthStep);
    }

    // Send the actuators info to Gazebo with ICE

    void Control::SetActuatorsICE(Api *api) {

        if (api->motorWout < 5 && api->motorWout>-5)
            this->mprx->setW(0.);
        this->mprx->setW(api->motorWout);
        this->mprx->setL(api->motorLout);

        api->Pose3DmotorsData1 = new jderobot::Pose3DMotorsData();
        api->Pose3DmotorsData2 = new jderobot::Pose3DMotorsData();

        api->Pose3DmotorsData1->tilt = api->v_normalized;
        api->Pose3DmotorsData1->pan = api->w_normalized;
        api->Pose3DmotorsData2->tilt = api->v_normalized;
        api->Pose3DmotorsData2->pan = api->w_normalized;


        this->p3dmprx2->setPose3DMotorsData(api->Pose3DmotorsData2);
        this->p3dmprx1->setPose3DMotorsData(api->Pose3DmotorsData1);
        this->mprx->setV(api->motorVout);
    }

}

