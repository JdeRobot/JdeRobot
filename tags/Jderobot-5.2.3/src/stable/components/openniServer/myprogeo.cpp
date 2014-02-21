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

#include "myprogeo.h"


namespace openniServer {
myprogeo::myprogeo(int nCam, int w, int h) {
	for (int i=0; i< nCam; i++){
		cameras[i]=new Progeo::Progeo();
	}
	this->w=w;
	this->h=h;
}

myprogeo::~myprogeo() {
}


/* gets the calibration of the camera from a file */
void myprogeo::load_cam(char *fich_in,int cam, int w, int h, bool fileFromCalibrator)
{
	std::cout << "Loading camera: " << cam << std::endl;

    if (fileFromCalibrator){
    	std::cout << "File loaded as extra calibration " << std::endl;
    	this->cameras[cam]->readFromFile(std::string(fich_in));

    	this->RT2=this->cameras[cam]->getRTMatrix();
    	Eigen::Vector4d pos;
		pos(0)=0;
		pos(1)=0;
		pos(2)=0;
		pos(3)=1;
		this->cameras[cam]->setPosition (pos);

		Eigen::Vector4d foa;
		foa(0)=0;
		foa(1)=1;
		foa(2)=0;
		foa(3)=1;
		this->cameras[cam]->setFoa(foa);
		this->cameras[cam]->setRoll(0);
		this->cameras[cam]->updateRTMatrix();

    }
    else{
		if (strlen(fich_in) ==0 ) {
			std::cout << "Setting standard calibration" << std::endl;

			Eigen::Matrix3d K;
			K(0,0) = 511;
			K(0,1) = 0;
			K(0,2) = w/2;

			K(1,0) = 0;
			K(1,1) = 511;
			K(1,2) = h/2;

			K(2,0) = 0;
			K(2,1) = 0;
			K(2,2) = 1;

			this->cameras[cam]->setKMatrix(K);

			Eigen::Vector4d pos;
			pos(0)=0;
			pos(1)=0;
			pos(2)=0;
			pos(3)=1;
			this->cameras[cam]->setPosition (pos);

			Eigen::Vector4d foa;
			foa(0)=0;
			foa(1)=1;
			foa(2)=0;
			foa(3)=1;
			this->cameras[cam]->setFoa(foa);
			this->cameras[cam]->setRoll(0);

			//this->cameras[cam]->updateKMatrix();
			this->cameras[cam]->updateRTMatrix();


		}
		else {
			std::cout << "Loading standard calibration file " << std::endl;
			this->cameras[cam]->readFromFile(std::string(fich_in));
			this->cameras[cam]->updateKMatrix();
			this->cameras[cam]->updateRTMatrix();
		}
    }


    this->cameras[cam]->displayCameraInfo();
}

void myprogeo::pixel2optical(float*x,float*y){
	int localX=*x;
	int localY=*y;

	*x= this->h-1-localY;
	*y= localX;
}

void myprogeo::optical2pixel(float*x,float*y){

	int localX=*x;
	int localY=*y;

	*x= localY;
	*y= this->h-1-localX;
}


void
myprogeo::mybackproject(float x, float y, float* xp, float* yp, float* zp, float* camx, float* camy, float* camz, int cam){

	Eigen::Vector3d p;
	Eigen::Vector4d pro;


	pixel2optical(&x, &y);
	p(0)=x;
	p(1)=y;
	p(2)=1;

	this->cameras[cam]->backproject(p,pro);
	*xp=pro(0);
	*yp=pro(1);
	*zp=pro(2);



	Eigen::Vector4d pos;

	pos=this->cameras[cam]->getPosition();

	*camx=pos(0);
	*camy=pos(1);
	*camz=pos(2);
}

void
myprogeo::myproject(float x, float y, float z, float* xp, float* yp, int cam){
	Eigen::Vector3d p;
	Eigen::Vector4d p3;

	p3(0)=x;
	p3(1)=y;
	p3(2)=z;
	p3(3)=1;

	this->cameras[cam]->project(p3,p);
	*xp=p(0);
	*yp=p(1);
}

void
myprogeo::mygetcameraposition(float *x, float *y, float *z, int cam){
	Eigen::Vector4d pos;

	pos=this->cameras[cam]->getPosition();

	*x=pos(0);
	*y=pos(1);
	*z=pos(2);
}

void
myprogeo::mygetcamerafoa(float *x, float *y, float *z, int cam){
	Eigen::Vector4d foa;

	foa=this->cameras[cam]->getFoa();
	*x=foa(0);
	*y=foa(1);
	*z=foa(2);
}

void
myprogeo::mygetcamerasize(float *w, float *h, int cam){
	*w = this->w;
	*h = this->h;
}

void
myprogeo::applyExtraCalibration(float *x, float *y, float *z){
	Eigen::Vector4d pos;

	pos(0)=*x;
	pos(1)=*y;
	pos(2)=*z;
	pos(3)=1;

	//std::cout << "pre: " << pos << std::endl;

	pos=this->RT2*pos;
	//std::cout << "post: " << pos << std::endl;

	*x=pos(0);
	*y=pos(1);
	*z=pos(2);
}

Progeo::Progeo*
myprogeo::getCamera(int camera){
	return cameras[camera];
}

} //namespace
