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
 *  Authors : Rubén González Barriada <ruben.gbarriada@gmail.com>
 *						Alejandro Hernández Cordero <ahcorde@gmail.com> 
 *            Óscar Javier García Baudet <oscar.robotica@linaresdigital.com>
 *
 */

#include <Ice/Ice.h>

#include <jderobot/camera.h>
#include <visionlib/colorspaces/colorspacesmm.h>
#include "viewer.h"

int main(int argc, char** argv) {
  opencvdemo::Viewer viewer;
  Ice::CommunicatorPtr ice_communicator;
  Ice::ObjectPrx base;
  jderobot::CameraPrx camera_proxy = 0;
  jderobot::ImageDataPtr image_data;
  cv::Mat image;

  /* Initialize Ice and load parameters from command line */
  try {
    ice_communicator = Ice::initialize(argc, argv);
    base = ice_communicator->propertyToProxy("Opencvdemo.Camera.Proxy");
    if (0 == base)
      throw "Could not create proxy";
  } catch (const Ice::Exception& ex) {
    /* Show Ice generated error */
    std::cerr << "ERROR: " << ex << std::endl;
    return 1;
  } catch (const char* msg) {
    std::cerr << msg << std::endl;
    return 1;
  }

  /* While GUI is visible (user didn't close it) */
  while (viewer.isVisible()) {
    /* Try to get next frame */
    try {
      /* Get casted proxy to access all Camera methods */
      if (camera_proxy == 0) {
        camera_proxy = jderobot::CameraPrx::checkedCast(base);
        if (camera_proxy == 0) {
          std::cerr << "ERROR: Invalid proxy" << std::endl;
          return 1;
        }
      }
      /* Get next frame */
      image_data = camera_proxy->getImageData(colorspaces::ImageRGB8::FORMAT_RGB8.get()->name);
    } catch (const Ice::Exception& ex) {
      /* Show an error in stderr */
      std::cerr << "ICE error: " << ex << std::endl;
      /* Show the default "Missing Image" icon instead input image */
      if (viewer.isVisible()) {
        viewer.DisplayError();
      }
      /* Wait 250ms before trying again */
      usleep(250000);
      continue;
    }

    /* Check if format is supported by colorspaces */
    colorspaces::Image::FormatPtr format_string =
        colorspaces::Image::Format::searchFormat(
            image_data->description->format);
    if (!format_string)
      throw "Format not supported";

    /* Get image with same format as origin */
    colorspaces::Image frame(image_data->description->width,
                             image_data->description->height, format_string,
                             &(image_data->pixelData[0]));
    /* Conversion to RGB8 will happen only if needed */
    colorspaces::ImageRGB8 frame_rgb8(frame);
    /* Convert old OpenCV image container, IplImage, to newer cv::Mat */
    image = cv::Mat(frame_rgb8);

    /* Display and process input image */
    viewer.Display(image);
  }

  /* Free ICE resources */
  if (ice_communicator)
    ice_communicator->destroy();

  return 0;
}

/*! \mainpage JdeRobot's OpenCV demo
 *
 * File opencvdemo.cpp: function main(int argc, char** argv).
 *
 * File viewer.cpp: Class opencvdemo.Viewer.
 */
