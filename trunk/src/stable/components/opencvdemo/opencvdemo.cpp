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
 *
 */

#include <Ice/Ice.h>
#include <jderobot/camera.h>
#include <visionlib/colorspaces/colorspacesmm.h>
#include "viewer.h"

int main(int argc, char** argv){
  int status;
  opencvdemo::Viewer viewer;
  Ice::CommunicatorPtr ic;

  try{
    ic = Ice::initialize(argc,argv);
    Ice::ObjectPrx base = ic->propertyToProxy("Opencvdemo.Camera.Proxy");
    if (0==base)
      throw "Could not create proxy";

    /*cast to CameraPrx*/
    jderobot::CameraPrx cprx = jderobot::CameraPrx::checkedCast(base);
    if (0==cprx)
      throw "Invalid proxy";

    while(viewer.isVisible()){
      
			jderobot::ImageDataPtr data = cprx->getImageData();
      colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(data->description->format);

      if (!fmt)
				throw "Format not supported";

			 char * data1 = new char[data->description->width*data->description->height*3];

			memcpy((unsigned char *)data1, &(data->pixelData[0]), data->description->width*data->description->height*3);

			// Get input image 
      /*colorspaces::Image image(data->description->width,
			       data->description->height,
			       fmt,
			       &(data->pixelData[0]));*/
	cv::Mat image (data->description->height, 
		   data->description->width,
		   CV_8UC3,
		   &(data->pixelData[0]));
	
			// Get output image
     /*colorspaces::Image image2(data->description->width,
			       data->description->height,
			       fmt,
			       data1);*/

      cv::Mat image2 (data->description->height,
		 data->description->width,
		 CV_8UC3,
		 data1);

			//std::cout << data->description->width << std::endl;
			//std::cout << data->description->height << std::endl;
			//cv::Size s=image2.size();
			//std::cout << s.width << std::endl;
			//std::cout << s.height << std::endl;
			//std::cout << "El step de image es:\n";
			//std::cout << image.step << std::endl;
			//std::cout << image.data << std::endl;

			// Selecting the operation
			viewer.selection(image2);
      
			// Displaying the images
			viewer.display(image,image2);
			delete data1;
	
    }
  }catch (const Ice::Exception& ex) {
    std::cerr << ex << std::endl;
    status = 1;
  } catch (const char* msg) {
    std::cerr << msg << std::endl;
    status = 1;
  }

  if (ic)
    ic->destroy();
  return status;
}
