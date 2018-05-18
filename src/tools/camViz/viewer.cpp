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
#include <string>
#include <resourcelocator/gladelocator.hpp> 

namespace camViz {
  const std::string gladepath = resourcelocator::findGladeFile("camViz.glade");

  Viewer::Viewer()
    : gtkmain(0, 0), frameCount(0) {

    std::cout << "Loading glade\n";
    refXml = Gnome::Glade::Xml::create(gladepath);
    refXml->get_widget("image", gtkimage);
    refXml->get_widget("mainwindow",mainwindow);
    refXml->get_widget("fpslabel",fpslabel);
        
    // start the timer for calculating the number of frames per second
    // the images are being displayed at
    oldFrameTime = IceUtil::Time::now();
  }
    

  Viewer::~Viewer() {}

  bool Viewer::isVisible(){
    return mainwindow->is_visible();
  }


  void Viewer::display(cv::Mat imageRGB)
  {

	  if (!imageRGB.empty()){


		  Glib::RefPtr<Gdk::Pixbuf> imgBuff =
				  Gdk::Pixbuf::create_from_data((const guint8*) imageRGB.data,Gdk::COLORSPACE_RGB,false,8,imageRGB.cols,imageRGB.rows,imageRGB.step);

		  gtkimage->clear();
		  gtkimage->set(imgBuff);

	  }
	  mainwindow->resize(1,1);
	  while (gtkmain.events_pending())
		  gtkmain.iteration();

	  /*
    colorspaces::ImageRGB8 img_rgb8(image);//conversion will happen if needed
    Glib::RefPtr<Gdk::Pixbuf> imgBuff = 
      Gdk::Pixbuf::create_from_data((const guint8*)img_rgb8.data,
				    Gdk::COLORSPACE_RGB,
				    false,
				    8,
				    img_rgb8.width,
				    img_rgb8.height,
				    img_rgb8.step);
    
    gtkimage->clear();
    gtkimage->set(imgBuff);
    displayFrameRate();
    mainwindow->resize(1,1);
    while (gtkmain.events_pending())
      gtkmain.iteration();

      */
  }
    
  void
  Viewer::displayFrameRate(int rate)
  {
      std::stringstream fpsString;
      fpsString << "fps = " << rate;
      fpslabel->set_label(fpsString.str());
  }

}//namespace
