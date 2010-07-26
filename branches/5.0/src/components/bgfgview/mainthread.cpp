/*
 *
 *  Copyright (C) 1997-2010 JDERobot Developers Team
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

#include <jderobotice/interfaceconnect.h>
#include "mainthread.h"
#include "viewgtk.h"

namespace carspeed {
  MainThread::MainThread(const jderobotice::Context &context)
    : jderobotice::SubsystemThread(context.tracer(),context.status(),"MainThread"),
      imagePrx(),fmt(),
      context(context),model(),controller() {}

  void MainThread::initialize(){
    //read configuration
    Ice::PropertiesPtr prop = context.properties();
    std::string prefix = context.tag() + ".Config.";

    health().ok( "Initialized" );

    if ( isStopping() )
      return;
    
    //connect to required interfaces
    //read image provider proxy address from config: XX.Config.ImageProvide.Proxy = <address>
    jderobotice::connectToInterfaceWithString(context,imagePrx,prop->getProperty(prefix+"ImageProvider.Proxy"));

    //read first image to build model
    jderobot::ImageDataPtr img = imagePrx->getImageData();
    //we try to know if we understand this format
    fmt = colorspaces::Image::Format::searchFormat(img->description->format);
    if (!fmt)
      throw colorspaces::Image::FormatMismatch("Format not supported:" + img->description->format);

    std::stringstream ss;
    ss << "Received fmt " << *fmt;
    context.tracer().info(ss.str());
    colorspaces::Image initialImg(img->description->width,
				  img->description->height,
				  fmt,
				  &(img->pixelData[0]));//data will be available until img is destroyed

    //init model. Road dimensions 
    model.reset(new Model(context.tracer(), initialImg.clone()));
    
    //create controller
    controller.reset(new Controller(context.tracer(), *model));

    //read properties to get road points and size
    std::vector<cv::Point> roadPoints;
    cv::Point p;
    double width, length;
    
    //check if A.x is set. if that fails we suppose there is no road
    //parameters. FIXME
    if (prop->getProperty(prefix+"Road.A.x").length() > 0){
      context.tracer().info("Initializing algorithm with config file data");
      p = cv::Point(prop->getPropertyAsInt(prefix+"Road.A.x"),
		    prop->getPropertyAsInt(prefix+"Road.A.y"));
      roadPoints.push_back(p);
      p = cv::Point(prop->getPropertyAsInt(prefix+"Road.B.x"),
		    prop->getPropertyAsInt(prefix+"Road.B.y"));
      roadPoints.push_back(p);
      p = cv::Point(prop->getPropertyAsInt(prefix+"Road.C.x"),
		    prop->getPropertyAsInt(prefix+"Road.C.y"));
      roadPoints.push_back(p);
      p = cv::Point(prop->getPropertyAsInt(prefix+"Road.D.x"),
		    prop->getPropertyAsInt(prefix+"Road.D.y"));
      roadPoints.push_back(p);
      ss.str(prop->getProperty(prefix+"Road.Width"));
      ss >> width;
      ss.clear();
      ss.str(prop->getProperty(prefix+"Road.Length"));
      ss >> length;
      ss.clear();
      ss.str("");
      ss << "Setting new algorithm: width=" << width << ",length=" << length;
      context.tracer().info(ss.str());
      CarspeedAlgorithmConfig cfg(width,length,roadPoints);
      CarspeedAlgorithmPtr newAlg(new CarspeedAlgorithm(controller->model().getImage().clone(), cfg));
      controller->setAlgorithm(newAlg);
    }
    

    //create view: maybe we could activate/deactivate gui through config
    ViewPtr vp(new ViewGtk(*controller));
    controller->addView(vp);
  }

  void MainThread::work(){
    //setMaxHeartbeatInterval( 1.0 );

    while(!isStopping()){
      getImage();
    }

  }

  void MainThread::finalize(){
    context.communicator()->shutdown();
  }
  
  //copy image to local variable
  void MainThread::getImage(){
    //read from interface and place data in model
    jderobot::ImageDataPtr img = imagePrx->getImageData();
    //we suppose fmt is not going to change so fmt is not checked again
    colorspaces::Image cImg(img->description->width,
			    img->description->height,
			    fmt,
			    &(img->pixelData[0]));//data will be available until img is destroyed
    model->setImage(cImg.clone());//FIXME: find a way to avoid this copy
  }
}
