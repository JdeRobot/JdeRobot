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
#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <jderobotutil/observer.h>
#include <jderobot/varcolor.h>
#include <jderobot/recording.h>
#include <colorspacesice/image.h>
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
	model(new Model()),
	controller(new Controller(model)) {}

    virtual void start()
    {
      Ice::ObjectPrx base =
	context().communicator()->propertyToProxy("Motiondetection.VarColor.Proxy");
      if (0==base)
	throw "Could not create proxy";

      /*cast to VarColorPrx*/
      jderobot::VarColorPrx vprx = jderobot::VarColorPrx::checkedCast(base);
      if (0==vprx)
	throw "Invalid proxy";

      CvRect areaWithMotion;
      Model::MotionGridItem2D motionValue;

      // // Get Proxy to RecordingManager
//       Ice::ObjectPrx base2 = context().communicator()->propertyToProxy("Motiondetection.RecordingManager.Proxy");
//       if (0==base2)
// 	throw "Could not create proxy (recordingManager)";
	
//       /*cast to RecordingManagerPrx*/
//       jderobot::RecordingManagerPrx rm_prx = jderobot::RecordingManagerPrx::checkedCast(base2);
//       if (0==rm_prx)
// 	throw "Invalid proxy (recordingManager)";


      //int i = 0;
      while (controller->isRunning())
        {
	  colorspacesice::ImagePtr image(new colorspacesice::Image(vprx->getData()));
	  //colorspaces::ImageppPtr image = colorspaces::Imagepp::createTestSquare(320,280,10,10,i,0,0);

	  controller->setImage(image);
	  if (controller->isMotionDetected(&areaWithMotion,&motionValue))
            {
	      std::cout << "Motion detected: " << motionValue.motion << std::endl;

	      jderobot::ByteSeq imageVector = vprx->getData()->pixelData;

	      // Save std:vector <bytes> in file
	      char nameFile[] = "/tmp/imgXXXXXX";
	      int res;
	      std::ofstream imageFile;

	      res = mkstemp(nameFile);

	      if (res==-1)
		{
		  std::cerr << "Error in mktemp: Don't create fileName!" << std::endl;
		  continue;
		}

	      imageFile.open (nameFile, std::ios::out | std::ios::binary);
	      imageFile.write (reinterpret_cast<char *>(&imageVector[0]), imageVector.size () * sizeof(Ice::Byte));
	      imageFile.close();

// 	      // Build the Event
// 	      jderobot::RecordingEventPtr event = new jderobot::RecordingEvent();

// 	      event->id = 12;
// 	      event->type = "1";
// 	      event->producer = "motiondetection";
// 	      event->resource = "cam1";
// 	      event->comment = "movement detected";
// 	      event->pathImage = nameFile;

// 	      // Send event to RecordingManager
// 	      rm_prx->setEvent(event, 2);

            }

	  //i++;
	  //if (i>=image->description.height)
	  //  i = 0;
	  //IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(66));/*15fps*/
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
