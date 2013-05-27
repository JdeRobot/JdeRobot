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

#include "myprogeo.h"


namespace kinectViewer {
myprogeo::myprogeo(){
	std::cout << "CREADO" << std::endl;
}

myprogeo::~myprogeo(){
}


/* gets the calibration of the camera from a file */
void myprogeo::load_cam(char *fich_in,int cam, int w, int h)
{
  FILE *entrada;
  int i;
	if (strlen(fich_in) ==0 ){
		std::cout << w << ", " << h << std::endl;
		this->cameras[cam].fdistx=515;
		this->cameras[cam].fdisty=515;
		this->cameras[cam].u0=h/2;
		this->cameras[cam].v0=w/2;
		this->cameras[cam].position.X=0;
		this->cameras[cam].position.Y=0;
		this->cameras[cam].position.Z=0;
		this->cameras[cam].foa.X=0;
		this->cameras[cam].foa.Y=1;
		this->cameras[cam].foa.Z=0;
		this->cameras[cam].skew=0;
		this->cameras[cam].roll=0;
		update_camera_matrix(&cameras[cam]);

		
	}
	else{
		xmlReader(&(this->cameras[cam]), fich_in);
		update_camera_matrix(&cameras[cam]);
	}
  
  display_camerainfo(cameras[cam]);
}


void
myprogeo::mybackproject(float x, float y, float* xp, float* yp, float* zp, float* camx, float* camy, float* camz, int cam){
	HPoint2D p;
	HPoint3D pro;
	



	p.x=GRAPHIC_TO_OPTICAL_X(x,y); 
	p.y=GRAPHIC_TO_OPTICAL_Y(x,y);
	p.h=1;
	backproject(&pro,p,cameras[cam]);
	*xp=pro.X;
	*yp=pro.Y;
	*zp=pro.Z;

	*camx=cameras[cam].position.X;
	*camy=cameras[cam].position.Y;
	*camz=cameras[cam].position.Z;
}

void 
myprogeo::myproject(float x, float y, float z, float* xp, float* yp, int cam){
	HPoint2D p;
	HPoint3D p3;

	p3.X=x;
	p3.Y=y;
	p3.Z=z;
	p3.H=1;
	
	project(p3, &p, cameras[cam]);
	*xp=p.x;
	*yp=p.y;
}

void
myprogeo::mygetcameraposition(float *x, float *y, float *z, int cam){
	*x=cameras[cam].position.X;
	*y=cameras[cam].position.Y;
	*z=cameras[cam].position.Z;
}

void 
myprogeo::mygetcamerafoa(float *x, float *y, float *z, int cam){
	*x=cameras[cam].foa.X;
	*y=cameras[cam].foa.Y;
	*z=cameras[cam].foa.Z;
}

void 
myprogeo::mygetcamerasize(float *w, float *h, int cam){
	*w = cameras[cam].columns;
	*h = cameras[cam].rows;
}

TPinHoleCamera 
myprogeo::getCamera(int camera){
	return cameras[camera];
}

} //namespace
