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

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <jderobotutil/observer.h>
#include <jderobot/camera.h>
#include <jderobot/recording.h>
#include "view.h"
#include "model.h"
#include "controller.h"

namespace motiondetection
{
  class Component: public jderobotice::Component
  {
  public:
    Component()
      : jderobotice::Component("Motiondetection"),
	model(),
	controller() {}

    virtual void start()
    {
      Ice::PropertiesPtr prop = context().properties();

      Ice::ObjectPrx base =
	context().communicator()->propertyToProxy("Motiondetection.Camera.Proxy");
      if (0==base)
	throw "Could not create proxy";

      /*cast to CameraPrx*/
      jderobot::CameraPrx cprx = jderobot::CameraPrx::checkedCast(base);
      if (0==cprx)
	throw "Invalid proxy";

      MotionItem2D motionValue;

      // Get Proxy to RecordingManager
      Ice::ObjectPrx base2 = context().communicator()->propertyToProxy("Motiondetection.RecordingManager.Proxy");
      if (0==base2)
	throw "Could not create proxy (recordingManager)";
	
      /*cast to RecordingManagerPrx*/
      jderobot::RecordingManagerPrx rm_prx = jderobot::RecordingManagerPrx::checkedCast(base2);
      if (0==rm_prx)
	throw "Invalid proxy (recordingManager)";

      
      jderobot::ImageDataPtr data = cprx->getImageData();
      colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(data->description->format);
      if (fmt == 0)
	throw "Unknown image format: " + data->description->format;
      
      colorspaces::Image image(data->description->width,
			       data->description->height,
			       fmt,
			       &(data->pixelData[0]));

      //build model and controller
      model.reset(new Model(context().tracer(),image));
      bool useUI = static_cast<bool>(prop->getPropertyAsIntWithDefault("Motiondetection.UseUI",1));
      controller.reset(new Controller(context().tracer(),model,useUI));
      
      while (controller->isRunning()){
	data = cprx->getImageData();//get image
	image = colorspaces::Image(data->description->width,
				   data->description->height,
				   fmt,
				   &(data->pixelData[0]));
	controller->setImage(image);
	if (controller->isMotionDetected(motionValue)){
 	  std::stringstream ss;
	  ss << "Motion detected: " << motionValue.motion;
	  context().tracer().info(ss.str());
	  jderobot::ByteSeq& imageVector = data->pixelData;//just a reference
	  
	  // Save std:vector <bytes> in file
	  char nameFile[] = "/tmp/imgXXXXXX";
	  int res;
	  std::ofstream imageFile;
	  
	  res = mkstemp(nameFile);
	  
	  if (res==-1)
	    {
	      context().tracer().error("Error in mktemp: Don't create fileName!");
	      continue;
	    }
	  
	  imageFile.open (nameFile, std::ios::out | std::ios::binary);
	  imageFile.write (reinterpret_cast<char *>(&(imageVector[0])), imageVector.size () * sizeof(Ice::Byte));
	  imageFile.close();
	  
	  // Build the Event
	  jderobot::RecordingEventPtr event = new jderobot::RecordingEvent();
	  
	  event->id = 12;
	  event->type = "1";
	  event->producer = "motiondetection";
	  event->resource = "cam1";
	  event->comment = "movement detected";
	  event->pathImage = nameFile;
	  
	  // Send event to RecordingManager
	  rm_prx->setEvent(event, 2);
	  
        }
      }
      context().communicator()->shutdown();
    }
  private:
    ModelPtr model;
    ControllerPtr controller;
  };
} //namespace

int main(int argc, char * argv[])
{
  motiondetection::Component component;

  jderobotice::Application app( component );
  return app.jderobotMain(argc, argv);
}
