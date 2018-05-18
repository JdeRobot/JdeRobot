/*
 *  Copyright (C) 1997-2013 JDE Developers TeamrgbdViz.camRGB
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

#ifndef rgbdViz_MYPROGEO_H
#define rgbdViz_MYPROGEO_H

#include <geometry/progeo/Progeo.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>

/* GRAPHIC coordenates to OPTICAL coordenates */

#define MAX_CAMERAS 8
#define MAX_BUFFER 1024

namespace rgbdViz {
  class myprogeo {
	public:
	myprogeo(int nCam, int w, int h);
	~myprogeo();
	void load_cam(char *fich_in,int cam, int w, int h);
	void mybackproject(float x, float y, float* xp, float* yp, float* zp, float* camx, float* camy, float* camz, int cam);
	void myproject(float x, float y, float z, float* xp, float* yp, int cam);
	void mygetcameraposition(float *x, float *y, float *z, int cam);
	void mygetcamerafoa(float *x, float *y, float *z, int cam);
	void mygetcamerasize(float *w, float *h, int cam);
	Progeo::Progeo* getCamera(int camera);
	void new_camera();

	private:
		void pixel2optical(float*x,float*y);
		void optical2pixel(float* x, float* y); 
		
		/* cameras */
		Progeo::Progeo* cameras[MAX_CAMERAS];
		int w,h;
  };
} // namespace

#endif /*rgbdViz_MYPROGEO_H*/
