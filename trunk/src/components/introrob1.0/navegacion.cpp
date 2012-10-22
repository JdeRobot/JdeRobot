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
 *  Author : Julio Vega <julio.vega@urjc.es>
 *
 */

#include "navegacion.h"

namespace introrob {

    bool boolView = false;

	
	void Navegacion::setView (View* view) {
		this->view = view;
		boolView=true;
	}

	int Navegacion::pintaSegmento (CvPoint3D32f a, CvPoint3D32f b, CvPoint3D32f color) {
		glColor3f(color.x, color.y, color.z);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
			v3f(a.x, a.y, a.z);
			v3f(b.x, b.y, b.z);
		glEnd();
		return 1;
	}

	int Navegacion::absolutas2relativas(CvPoint3D32f in, CvPoint3D32f *out)	{
		if (out!=NULL) {
			(*out).x = in.x*cos(this->robottheta*DEGTORAD) - in.y*sin(this->robottheta*DEGTORAD) +
				this->robotx*cos(this->robottheta*DEGTORAD) + this->roboty*sin(this->robottheta*DEGTORAD);
			(*out).y = in.y*cos(this->robottheta*DEGTORAD) + in.x*sin(this->robottheta*DEGTORAD) +
				this->roboty*sin(this->robottheta*DEGTORAD) + this->roboty*cos(this->robottheta*DEGTORAD);
			return 0;
		}
		return 1;
	}

	int Navegacion::relativas2absolutas(CvPoint3D32f in, CvPoint3D32f *out)	{
		if (out!=NULL){
			(*out).x = in.x*cos(this->robottheta*DEGTORAD) - in.y*sin(this->robottheta*DEGTORAD) + this->robotx;
			(*out).y = in.y*cos(this->robottheta*DEGTORAD) + in.x*sin(this->robottheta*DEGTORAD) + this->roboty;
			return 0;
		}
		return 1;
	}

	void* callback(void* obj) {
		static_cast<Navegacion*>(obj)->main();
		return(0);
	} // callback

	void Navegacion::cogerImagen1(unsigned char** image) { // refresco el contenido de la imagen1
		this->controller->getCameraData1 (&myImage1);
		*image = &myImage1[0];
	}

	void Navegacion::cogerImagen2(unsigned char** image) { // refresco el contenido de la imagen1
		this->controller->getCameraData2 (&myImage2);
		*image = &myImage2[0];
	}

	void Navegacion::cogerPosicion(CvPoint3D32f* myPoint) { // refresco la posición del pioneer
		this->controller->getPosition (myPoint);
	}

	int Navegacion::cogerLaser(std::vector<float>* laser) { // refrescamos el vector de valores de láser
		this->controller->getLaser (laser);
		return this->controller->getNumLasers();
	}

	void Navegacion::cogerDestino(CvPoint2D32f* destino) { // refresco la posición del pioneer
		this->view->setDestino ();
		if(boolView==true){
	        destino->x = this->view->destino.x;
	        destino->y = this->view->destino.y;
		}
	}

	int Navegacion::main() {
		struct timeval a, b;
		int cycle = 100;
		long totalb,totala;
		long diff;

		while(1) {
			gettimeofday(&a,NULL);
			totala=a.tv_sec*1000000+a.tv_usec;

		//	if (this->running) {
				this->navega->iteracionControl();
	//		}

			gettimeofday(&b,NULL);
			totalb=b.tv_sec*1000000+b.tv_usec;
			//std::cout << "Introrob takes " << (totalb-totala)/1000 << " ms" << std::endl;

			diff = (totalb-totala)/1000;
			if(diff < 0 || diff > cycle)
				diff = cycle;
			else
				diff = cycle-diff;

			/*Sleep Algorithm*/
			usleep(diff*1000);
			if(diff < 33)
				usleep(33*1000);
		}
	}

	void Navegacion::run(introrob::Controller * controller) {
		this->controller = controller;
		this->navega = new Navega (this->controller, this);
		this->running=false; //ALEX
		//this->myImage1 = (unsigned char*) calloc (this->controller->data1->description->width*this->controller->data1->description->height*3,sizeof(unsigned char));
		//this->myImage2 = (unsigned char*) calloc (this->controller->data1->description->width*this->controller->data1->description->height*3,sizeof(unsigned char));

		this->numlines=0;

       pthread_create(&thread, 0, &callback, this);
  }

	void Navegacion::start() {
    this->running=true;
  }

	void Navegacion::stop() {
		this->controller->stopMotors();
    this->running=false;
  }

  int Navegacion::join() {
    return pthread_join(thread, ret);
  }


	void Navegacion::initCameras () {

			// Init camera 1		
			camera *mycameraA = new camera("cameras/calibA");
			myCamA= mycameraA->readConfig();

			// Init camera 2
			camera *mycameraB = new camera("cameras/calibB");
			myCamB= mycameraB->readConfig();
		}

	void Navegacion::calculate_projection_line(HPoint2D pix, int idcamera){

		HPoint3D pdraw3D;	

		if(idcamera==1){
			this->get3DPositionX(&myCamA,pdraw3D,pix,5000.0);// 5000 son los 5 metros a los que está la pared.
			this->navega->add_line(myCamA.position.X, myCamA.position.Y, myCamA.position.Z,(float)pdraw3D.X,(float)pdraw3D.Y, (float)pdraw3D.Z,idcamera);

		}else{
			this->get3DPositionX(&myCamB,pdraw3D,pix,5000.0);// 5000 son los 5 metros a los que está la pared.
			this->navega->add_line(myCamB.position.X, myCamB.position.Y, myCamB.position.Z,(float)pdraw3D.X,(float)pdraw3D.Y, (float)pdraw3D.Z,idcamera);

		}
	
	}


	void Navegacion::pixel2optical(TPinHoleCamera *cam, HPoint2D *p){
	
		float aux;
		int height;

		height=cam->rows;
	 	 aux=p->x;
	 	 p->x=height-1-p->y;
	 	 p->y=aux; 
	}

	void Navegacion::get3DPositionX(TPinHoleCamera * camera, HPoint3D &res, HPoint2D in, double X = 0.0){
		HPoint2D p2d;
		HPoint3D p3d;
		float x, y, z;
		float xfinal, yfinal, zfinal;

		x = camera->position.X;
		y = camera->position.Y;
		z = camera->position.Z;

		p2d.x = in.x;
		p2d.y = in.y;
		p2d.h = in.h;

		this->pixel2optical(camera, &p2d);
		backproject(&p3d, p2d, *camera);


		if((p3d.X-x) == 0.0) {
			res.H = 0.0;
			return;
		}

		xfinal = X;

		//Linear equation (X-x)/(p3d.X-x) = (Y-y)/(p3d.Y-y) = (Z-z)/(p3d.Z-z)
		yfinal = y + (p3d.Y - y)*(xfinal - x)/(p3d.X-x);	
		zfinal = z + (p3d.Z - z)*(xfinal - x)/(p3d.X-x);


		res.X = xfinal;
		res.Y = yfinal;
		res.Z = zfinal;
		res.H = 1.0;

	}

	void Navegacion::drawProjectionLines(){

		for(int i=0;i<numlines;i++) {

			CvPoint3D32f a,b,aa,ba;
			CvPoint3D32f color;
			a.x=extra_lines[i][0]/SCALE;
			a.y=extra_lines[i][1]/SCALE;
			a.z=extra_lines[i][2]/SCALE;
			b.x=extra_lines[i][4]/SCALE;
			b.y=extra_lines[i][5]/SCALE;
			b.z=extra_lines[i][6]/SCALE;

			if (extra_lines[i][8]==1){color.x = 1.;color.y = 0.;color.z = 0.;}else{color.x = 0.;color.y = 0.;color.z = 1.;}
			this->pintaSegmento (a, b, color);
		}
	}	

    HPoint3D Navegacion::calculate_intersection (HPoint3D posA, HPoint3D pA, HPoint3D posB, HPoint3D pB) {

	    HPoint3D vector, vector2, R, S, final;
	    float A, B, C, D, E, sigma, theta;
	    HPoint3D l1[2], l2[2];

	    l1[0] = posA;
	    l1[1] = pA;

	    l2[0] = posB;
	    l2[1] = pB;

	    /* Cálculo del Vector director de las rectas */
	    vector.X = l1[0].X - l1[1].X;
	    vector.Y = l1[0].Y - l1[1].Y;
	    vector.Z = l1[0].Z - l1[1].Z;
	    vector2.X = l2[0].X - l2[1].X;
	    vector2.Y = l2[0].Y - l2[1].Y;
	    vector2.Z = l2[0].Z - l2[1].Z;
	
	    /* Calculos para hallar sigma Y theta de las ecuaciones parametricas de las rectas */
	    A = (vector.X * vector2.X) + (vector.Y * vector2.Y) + (vector.Z * vector2.Z);
	    B = (vector.X * vector.X) + (vector.Y * vector.Y) + (vector.Z * vector.Z);
	    C = (vector2.X * vector2.X) + (vector2.Y * vector2.Y) + (vector2.Z * vector2.Z);
	    D = (vector.X * (l2[1].X - l1[1].X)) + (vector.Y * (l2[1].Y - l1[1].Y)) + (vector.Z * (l2[1].Z - l1[1].Z));
	    E = (vector2.X * (l2[1].X - l1[1].X)) + (vector2.Y * (l2[1].Y - l1[1].Y)) + (vector2.Z * (l2[1].Z - l1[1].Z));
	
	    theta = ((A * E) - (D * C)) / ((A * A) - (B * C));
	    sigma = ((A * theta) - E) / C;
	
	    /* Calculo de la interseccion de la perpendicular común con nuestras dos rectas */
	    R.X = l1[1].X + (vector.X * theta);
	    R.Y = l1[1].Y + (vector.Y * theta);
	    R.Z = l1[1].Z + (vector.Z * theta);
	
	    S.X = l2[1].X + (vector2.X * sigma);
	    S.Y = l2[1].Y + (vector2.Y * sigma);
	    S.Z = l2[1].Z + (vector2.Z * sigma);
	
	    final.X = (R.X + S.X) / 2; 
	    final.Y = (R.Y + S.Y) / 2;
	    final.Z = (R.Z + S.Z) / 2;
	    final.H = 1.;
	
	    return final;
    }


} // namespace
