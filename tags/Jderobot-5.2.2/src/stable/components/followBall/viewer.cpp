/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
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
 *  Authors : David Lobato Bravo <dav.lobato@gmail.com>
 *
 */

#include "viewer.h" 
#include <iostream>
#include <cmath>

  const std::string gladepath = std::string("./cameraview.glade");

  Viewer::Viewer() 
    : gtkmain(0,0),frameCount(0) {

    std::cout << "Loading glade\n";
    refXml = Gnome::Glade::Xml::create(gladepath);
    refXml->get_widget("image", gtkimage);
    refXml->get_widget("mainwindow", mainwindow);
    refXml->get_widget("fpslabel", fpslabel);

    this->mainwindow->add_events(Gdk::BUTTON_PRESS_MASK);
    this->mainwindow->signal_event().connect(
                sigc::mem_fun(*this, &Viewer::on_window_event));
        
    this->mainwindow->show_all_children();

    // start the timer for calculating the number of frames per second
    // the images are being displayed at
    oldFrameTime = IceUtil::Time::now();
  }
    

  Viewer::~Viewer() {}
  
  bool Viewer::on_window_event ( GdkEvent* event ) {
    if (event->button.button == 1) {
        int indice = event->button.y * this->myImage.step + event->button.x * this->myImage.channels();
        this->r = this->myImage.data[indice];
        this->g = this->myImage.data[indice+1];
        this->b = this->myImage.data[indice+2];
    }
  }

  bool Viewer::isVisible(){
    return mainwindow->is_visible();
  }

  void Viewer::display( cv::Mat& image, int rf, int gf, int bf )
  {
    this->rF = rf;
    this->gF = gf;
    this->bF = bf;
    this->myImage = image;
    Glib::RefPtr<Gdk::Pixbuf> imgBuff = 
            Gdk::Pixbuf::create_from_data((const guint8*)image.data,
				                          Gdk::COLORSPACE_RGB,
				                          false,
				                          8,
				                          image.cols,
				                          image.rows,
				                          image.step); 
    
    gtkimage->clear();
    gtkimage->set(imgBuff);
    displayFrameRate();
    mainwindow->resize(1,1);
    while (gtkmain.events_pending())
      gtkmain.iteration();
  }
    
  void
  Viewer::displayFrameRate()
  {
    double diff;
    IceUtil::Time diffT;

    currentFrameTime = IceUtil::Time::now();
    diff = (currentFrameTime - oldFrameTime).toMilliSecondsDouble();
    if (diff < 1000.0)
      frameCount++;
    else{
      oldFrameTime = currentFrameTime;
      fps = frameCount*1000.0/diff;
      frameCount=0;
      // Display the frame rate
      std::stringstream fpsString;
      fpsString << "fps = " << int(fps) << "; R/H = " << this->r << "; G/S = " << this->g << "; B/V = " << this->b << std::endl;
      fpsString << "filtering: R/H = " << this->rF << "; G/S = " << this->gF << "; B/V = " << this->bF << std::endl;
      fpslabel->set_label(fpsString.str());
    }
  }
