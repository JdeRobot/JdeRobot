/*
*  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *   Authors : Eduardo Perdices <eperdices@gsyc.es>,
 *   			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#ifndef GIRAFFECLIENT_CONTROLLER_H
#define GIRAFFECLIENT_CONTROLLER_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <opencv/cv.h>
#include <progeo/progeo.h>
#include <colorspaces/colorspacesmm.h>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <opencv/cv.h>
#include "drawarea.h"
/*#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>*/


#define CALIBRATION_PI 3.1415926
/* GRAPHIC coordenates to OPTICAL coordenates */
#define GRAPHIC_TO_OPTICAL_X(x,y,h) (h-1-y)
#define GRAPHIC_TO_OPTICAL_Y(x,y,h) (x)
#define OPTICAL_TO_GRAPHIC_X(x,y,h) (y)
#define OPTICAL_TO_GRAPHIC_Y(x,y,h) (h-1-x)

using namespace std;

namespace rgbdManualCalibrator {
  class Controller {
  public:
    Controller(Ice::PropertiesPtr prop, int w, int h, int nCameras);
    virtual ~Controller();
    
    std::string getGladePath();

		void drawWorld(cv::Mat image,int cam);

		//Tamaño de la imagen
		int cWidth;
		int cHeight;

		HPoint3D * getPos(int cam);
		HPoint3D * getFoa(int cam);
		float getFdistX(int cam);
		float getFdistY(int cam);
		float getU0(int cam);
		float getV0(int cam);
		float getRoll(int cam);

		void setPos(float x, float y, float z,int cam);
		void setFoa(float x, float y, float z,int cam);
		void setFdistX(float value,int cam);
		void setFdistY(float value,int cam);
		void setU0(float value,int cam);
		void setV0(float value,int cam);	
		void setRoll(float value,int cam);
		void changeDrawCenter();
		void add_depth_pointsImage(cv::Mat distances, cv::Mat imageRGB, rgbdManualCalibrator::DrawArea* world,int cam, int scale, int colour);
		void saveCameras(int nCameras);

  private:

		void init(Ice::PropertiesPtr prop,int nCameras);
		int load_world_line(FILE *myfile);
		int load_camera_config(Ice::PropertyDict pd,int cam);
		int load_world();
		
		void drawLine(cv::Mat , HPoint3D pini, HPoint3D pend, int cam);

		std::string gladepath;
		std::string world;
		std::string camOut;

		float lastx, lasty, lastz;
		bool drawCenter;
		vector<HPoint3D> lines;
		std::vector<TPinHoleCamera> cameras;
		int nCameras;
  };

} /*namespace*/

#endif /*GIRAFFECLIENT_CONTROLLER_H*/
