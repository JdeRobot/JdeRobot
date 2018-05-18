/*
 *  Copyright (C) 1997-2013 JDE Developers TeamkinectViewer.camRGB
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
 *  Author : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>

 */

#ifndef KINECTVIEWER_UTIL3D_H
#define KINECTVIEWER_UTIL3D_H

#include <cv.h>
#include "myprogeo.h"
#include <eigen3/Eigen/Dense>
#include <geometry/progeo/Progeo.h>





namespace rgbdViz {
class util3d {
	public:
		util3d(myprogeo* p);
		~util3d();
		void draw_room(cv::Mat image,int cam, float lines[][8], int n_lines);

	private:
		int cvDrawline(cv::Mat image,Eigen::Vector3d p1, Eigen::Vector3d p2, cv::Scalar color,int cam);
		myprogeo* mypro;
	};
}


#endif /*KINECTVIEWER_MUTIL3D_H*/
