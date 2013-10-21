/*
 *  Copyright (C) 2010 Julio Vega Pérez
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
 * 	Author : Julio Vega Pérez (julio [dot] vega [at] urjc [dot] es)
 *
 *  This library was programed for RobotVision Project http://jde.gsyc.es/index.php/robotvision
 *
 */

#ifndef VISUALMEMORY_LINESDETECTION_H
#define VISUALMEMORY_LINESDETECTION_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <visionlib/colorspaces/colorspacesmm.h>
#include <sys/time.h>
#include "image.h"

using namespace visionLibrary;
using namespace std;

namespace visionLibrary {
  class linesDetection {
		public:
			static void solisAlgorithm (cv::Mat &image, std::vector<Segment2D> *segments);
		
		private:
			static const int solis_cases[][11];
			static const int CASES_OFFSET;
	};
}

#endif
