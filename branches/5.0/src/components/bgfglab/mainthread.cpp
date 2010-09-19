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
#include <algorithm>
#include <cctype>
#include "mainthread.h"
#include "viewgtk.h"
#include "viewtext.h"

namespace bgfglab {
  MainThread::MainThread(const jderobotice::Context &context)
    : jderobotice::SubsystemThread(context.tracer(),context.status(),"MainThread"),
      imagePrx(),fmt(),
      context(context),model(),controller(),
      stopAfterDumpFinished(false) {}

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

    //read algorithm config and set it if present
    std::string bgalgPropPrefix(prefix+"BGAlgorithm.");
    std::string bgalg(prop->getProperty(bgalgPropPrefix+"Name"));
    std::string bgalgFmtName(prop->getProperty(bgalgPropPrefix+"Fmt"));

    //init model. Road dimensions 
    model.reset(new Model(context.tracer(), initialImg,
			  colorspaces::Image::Format::searchFormat(bgalgFmtName)));
    
    //create controller
    controller.reset(new Controller(context.tracer(), *model));

    
    if (bgalg.length() > 0){
      ParamDict bgalgParams(prop->getPropertiesForPrefix(bgalgPropPrefix), bgalgPropPrefix);
      controller->setBGModel(bgalg,bgalgParams);
    }

    //start dumping?
    std::string dumpPropPrefix(prefix+"Dump.");
    std::string dumpfilename(prop->getPropertyWithDefault(dumpPropPrefix+"File","bgfglab.dump"));
    int dumpFrames = prop->getPropertyAsInt(dumpPropPrefix+"Frames");
    int delayFrames = prop->getPropertyAsInt(dumpPropPrefix+"DelayFrames");
    int dumpDataImg = prop->getPropertyAsIntWithDefault(dumpPropPrefix+"DumpIMG",1);
    int dumpDataBg = prop->getPropertyAsIntWithDefault(dumpPropPrefix+"DumpBG",1);
    int dumpDataFg = prop->getPropertyAsIntWithDefault(dumpPropPrefix+"DumpFG",0);
    if (dumpDataImg || dumpDataBg || dumpDataFg)
      controller->startDumpData(dumpfilename,dumpFrames,delayFrames,
				dumpDataImg,dumpDataBg,dumpDataFg);

    //create view: ui can be gui or text
    std::string uiPropPrefix(prefix+"UI.");
    std::string uiMode(prop->getPropertyWithDefault(uiPropPrefix+"Mode","gui"));
    std::transform(uiMode.begin(), uiMode.end(), uiMode.begin(), ::tolower);//to lower case

    ViewPtr vp;
    if (uiMode.compare("text") == 0){//text ui
      vp.reset(new ViewText(*controller));
      if (model->isDumpingData())
	stopAfterDumpFinished = true;
    }else//graphic ui. Default mode
      vp.reset(new ViewGtk(*controller));
    controller->addView(vp);
  }

  void MainThread::work(){
    //setMaxHeartbeatInterval( 1.0 );

    while(!isStopping()){
      getImage();
      if (stopAfterDumpFinished && !model->isDumpingData())
	stop();
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
    model->updateBGModel(cImg);
  }
}
