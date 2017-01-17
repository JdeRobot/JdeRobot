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
 *           Eduardo Perdices (eperdices [at] gsyc [dot] es)
 *
 *  This library was programed for RobotVision Project http://jderobot.org/index.php/robotvision
 *
 */

#ifndef VISIONLIBRARY_IMAGE_H
#define VISIONLIBRARY_IMAGE_H

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "structs.h"
#include <visionlib/cvfast/cvfast.h>
#include "geometry.h"

#ifndef I_SQUARE
#define I_SQUARE(a) ( (a) * (a) )
#endif

namespace visionLibrary {
  class image {

		public:
			image ();
		  virtual ~image ();

			/*Get segments from the image src. Return corners*/
			static std::vector<HPoint2D> * getSegments(cv::Mat &src, std::vector<Segment2D> * segments, double threshold_fast = 50.0, double threshold_sobel = 100.0);

			/*Multiply fast fourier transform*/
			//static int multiplyFFT (const CvArr* srcAarr, const CvArr* srcBarr, CvArr* dstarr);

			/*Debug function*/
			static void getSegmentsDebug(cv::Mat &src);
		
		private:

			static const int IMAGE_WIDTH;
			static const int IMAGE_HEIGHT;
			static const double MIN_DISTANCE_CORNERS;
			static const double MIN_PERCENTAGE_VALID;
			static const double MIN_DISTANCE_UNIQUE_CORNERS;

			/*Check line fast*/
			static bool isLineFast(int x1, int y1, int x2, int y2);
			/*Check line slow*/
			static bool isLineSlow(int x1, int y1, int x2, int y2);
			/*Check edge filter*/
			static bool isEdge(int x, int y);
			/*Calculate edge in a concrete pixel*/
			static bool getEdge(int x, int y);
			/*Calculate edge in a concrete pixel with a threshold*/
			static bool getEdgeThresholed(int x, int y);
			/*Check if a corner is unique*/
			static bool is_unique_corner(int x, int y, int score);

			static cv::Mat *current_src;
			static double sobel_threshold;
			static bool * calThreshold;
			static bool * thresholds;
			static bool * calPixel;
			static bool * pixels;
			static std::vector<HPoint2D> * unique_corners;
	};
}

#endif
