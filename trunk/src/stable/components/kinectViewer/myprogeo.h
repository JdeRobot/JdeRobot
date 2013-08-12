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

#ifndef KINECTVIEWER_MYPROGEO_H
#define KINECTVIEWER_MYPROGEO_H

#include <progeo/progeo.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* GRAPHIC coordenates to OPTICAL coordenates */
#define WORKING_IMG_HEIGHT 480
#define GRAPHIC_TO_OPTICAL_X(x,y) (WORKING_IMG_HEIGHT-1-y)
#define GRAPHIC_TO_OPTICAL_Y(x,y) (x)
#define OPTICAL_TO_GRAPHIC_X(x,y) (y)
#define OPTICAL_TO_GRAPHIC_Y(x,y) (WORKING_IMG_HEIGHT-1-x)

#define MAX_CAMERAS 8
#define MAX_BUFFER 1024

namespace kinectViewer {
  class myprogeo {
	public:
	myprogeo();
	~myprogeo();
	void load_cam(char *fich_in,int cam, int w, int h);
	void mybackproject(float x, float y, float* xp, float* yp, float* zp, float* camx, float* camy, float* camz, int cam);
	void myproject(float x, float y, float z, float* xp, float* yp, int cam);
	void mygetcameraposition(float *x, float *y, float *z, int cam);
	void mygetcamerafoa(float *x, float *y, float *z, int cam);
	void mygetcamerasize(float *w, float *h, int cam);
	TPinHoleCamera getCamera(int camera);

	private:
		/* cameras */
		TPinHoleCamera cameras[MAX_CAMERAS];
  };
} // namespace

#endif /*KINECTVIEWER_MYPROGEO_H*/
