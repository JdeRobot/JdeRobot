/*
*  Copyright (C) 1997-2011 JDERobot Developers Team
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
 *   Authors : Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#ifndef VISORNECT_UTIL3D_H
#define VISORNECT_UTIL3D_H

#include "cv.h"
#include "myprogeo.h"




namespace visornect {
  class util3d {
	public:
		util3d(myprogeo* p);
		~util3d();
		void draw_room(IplImage *image,int cam, float lines[][8], int n_lines);

	private:
		int cvDrawline(IplImage* image,HPoint2D p1, HPoint2D p2, CvScalar color,int cam);
		myprogeo* mypro;
	};
}


#endif /*VISORNECT_MUTIL3D_H*/
