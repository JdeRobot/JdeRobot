/*
 *
 *  Copyright (C) 1997-2013 JDERobot Developers Team
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
 *  Authors : Roberto Calvo <rocapal [at] gsyc [dot] urjc [dot] es>
 *
 */

#ifndef RGBDCALIBRATOR_VIEWER_H
#define RGBDCALIBRATOR_VIEWER_H


#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <string>
#include <visionlib/colorspaces/colorspacesmm.h>
#include <cmath>
#include <jderobot/camera.h>
#include <visionlib/colorspaces/colorspaces.h>
#include "calibration.h"

using namespace cv;

namespace rgbdCalibrator{


  class Viewer
  {
  public:
    Viewer();
    ~Viewer();
  
    bool isVisible();

    //! function that actually displays the image in a window
    void display( const colorspaces::Image& imageColor, const colorspaces::Image& imageDepth );
    
    void setDepth(const jderobot::ImageDataPtr depth);

  private:
    Glib::RefPtr<Gnome::Glade::Xml> refXml;
    Gtk::Image* gtkimage_color;
    Gtk::Image* gtkimage_color2;
    Gtk::Image* gtkimage_color2zoom;

    Gtk::Image* gtkimage_depth;
    Gtk::Image* gtkimage_hsv;
    Gtk::Image* gtkimage_blob;
    Gtk::Window* mainwindow;
    Gtk::Label* fpsLabel;
    Gtk::Label* lbSleepPhoto;
    Gtk::Button* btTakePhoto;
    Gtk::Button* btIntrinsic;
    Gtk::Button* btSave;
    Gtk::Entry* etSleepPhoto;
    Gtk::Entry* etNumPhoto;
    Gtk::TextView* tvStatus;
    Gtk::EventBox* ebImage;
    Gtk::EventBox* ebImageExtrinsics;
    Gtk::EventBox* ebImageExtrinsicsZoom;

    Gtk::Main gtkmain;
    Gtk::Entry* etOffsetX;
    Gtk::Entry* etOffsetY;
    Gtk::Entry* etOffsetZ;
  
    //! display the frame rate of the received images
    void displayFrameRate();
      
    //! time variables for calculating number of frames per second 
    IceUtil::Time currentFrameTime,oldFrameTime;
    double fps;
    int frameCount;

    // Intrinsics variables
    Calibration* mCalibration;
    int intrinsicsEnable;
    IceUtil::Time lastTimePhoto;
    int delayPhoto;
    int numPhoto;
    int contPhoto;

    cv::Mat imgOrig;
    cv::Mat imgHSV;
    colorspaces::Image mImageDepth;
    IplImage* mFrameBlob;
    pthread_mutex_t mutex;
    const HSV* hsvFilter;
    double hmin, hmax, smin, smax, vmin, vmax;

    std::vector<colorspaces::Image> mDepthVector;
    const unsigned int MAX_MAPS;
    bool handlerDepth;

    // Extrinsics variables

    // onclicks
    bool on_eventbox_clicked(GdkEventButton * event);
    bool on_eventbox_extrinsics_clicked(GdkEventButton * event);
    bool on_eventbox_extrinsics_moved(GdkEventMotion * event);

    void on_bt_save_clicked();
    void on_bt_take_photo_clicked ();
    void on_bt_intrinsic();
    
    void saveImage(const colorspaces::Image& imageColor);
    void beep();
    void createImageHSV(const colorspaces::Image& imageDepth);

    int x_pos_zoom;
    int y_pos_zoom;

    
  
  };

}//namespace

#endif //RGBDCALIBRATOR_VIEWER_H
