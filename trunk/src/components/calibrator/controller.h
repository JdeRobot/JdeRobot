/*
*  Copyright (C) 1997-2010 JDERobot Developers Team
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
/*#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>*/


#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 240
#define CALIBRATION_PI 3.1415926

using namespace std;

namespace calibrator {
  class Controller {
  public:
    Controller(Ice::PropertiesPtr prop);
    virtual ~Controller();
    
    std::string getGladePath();

		void drawWorld(const colorspaces::Image& image);

		HPoint3D * getPos();
		HPoint3D * getFoa();
		float getFdistX();
		float getFdistY();
		float getU0();
		float getV0();
		float getRoll();

		void setPos(float x, float y, float z);
		void setFoa(float x, float y, float z);
		void setFdistX(float value);
		void setFdistY(float value);
		void setU0(float value);
		void setV0(float value);	
		void setRoll(float value);
		void changeDrawCenter();

  private:

		void init(Ice::PropertiesPtr prop);
		int load_world_line(FILE *myfile);
		int load_camera_config(Ice::PropertyDict pd);
		int load_world();
		
		void drawLine(IplImage * src, HPoint3D pini, HPoint3D pend);

		std::string gladepath;
		std::string world;
		std::string camOut;

		float lastx, lasty, lastz;
		bool drawCenter;
		vector<HPoint3D> lines;
		TPinHoleCamera camera;
  };

} /*namespace*/

#endif /*GIRAFFECLIENT_CONTROLLER_H*/
