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

#ifndef CAMVIZ_VIEWER_H
#define CAMVIZ_VIEWER_H

#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <string>
#include <visionlib/colorspaces/colorspacesmm.h>

namespace camViz{

  class Viewer
  {
  public:
    Viewer();
    ~Viewer();
  
    bool isVisible();

    //! function that actually displays the image in a window
    void display( cv::Mat imageRGB);
    void displayFrameRate(int rate);

  private:
    Glib::RefPtr<Gnome::Glade::Xml> refXml;
    Gtk::Image* gtkimage;
    Gtk::Window* mainwindow;
    Gtk::Label* fpslabel;
    Gtk::Main gtkmain;
  
    //! display the frame rate of the received images

      
    //! time variables for calculating number of frames per second 
    IceUtil::Time currentFrameTime,oldFrameTime;
    double fps;
    int frameCount;
  };

}//namespace

#endif //CAMVIZ_VIEWER_H
