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
	void Navegacion::setView (View* view) {
		this->view = view;
	}

	int Navegacion::pintaSegmento (CvPoint3D32f a, CvPoint3D32f b, CvPoint3D32f color) {
	  /* OJO mundo de coordenadas OpenGL está en decímetros, por compatibilidad con la plantilla OpenGL 
	     del robotPioneer. El factor SCALE marca la relación entre las coordenadas en milímetros de a,b
	     y los homólogos en el mundo OpenGL */
		glColor3f(color.x, color.y, color.z);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
			v3f(a.x/SCALE, a.y/SCALE, a.z/SCALE);
			v3f(b.x/SCALE, b.y/SCALE, b.z/SCALE);
		glEnd();
		return 1;
	}

	int Navegacion::absolutas2relativas(CvPoint3D32f in, CvPoint3D32f *out)	{
		if (out!=NULL) {
		  CvPoint3D32f myPoint;
		  
		  this->controller->getPosition(&myPoint);
		  this->robotx = myPoint.x;
		  this->roboty = myPoint.y;
		  this->robottheta = myPoint.z;
		  
		  (*out).x = in.x*cos(this->robottheta*DEGTORAD) + in.y*sin(this->robottheta*DEGTORAD) -
		    this->robotx*cos(this->robottheta*DEGTORAD) - this->roboty*sin(this->robottheta*DEGTORAD);
		  (*out).y = in.y*cos(this->robottheta*DEGTORAD) - in.x*sin(this->robottheta*DEGTORAD) -
		    this->roboty*cos(this->robottheta*DEGTORAD) + this->robotx*sin(this->robottheta*DEGTORAD);
		  return 0;
		}
		return 1;
	}

	int Navegacion::relativas2absolutas(CvPoint3D32f in, CvPoint3D32f *out)	{
		if (out!=NULL){
		  CvPoint3D32f myPoint;
		  
		  this->controller->getPosition(&myPoint);
		  this->robotx = myPoint.x;
		  this->roboty = myPoint.y;
		  this->robottheta = myPoint.z;
		  
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
		destino->x = this->view->destino.x;
		destino->y = this->view->destino.y;
	}

	void Navegacion::getMyLeftCam (TPinHoleCamera **myCam) {
		*myCam = &myCamA;
	}

	int Navegacion::main() {
		struct timeval a, b;
		int cycle = 100;
		long totalb,totala;
		long diff;

		while(1) {
			gettimeofday(&a,NULL);
			totala=a.tv_sec*1000000+a.tv_usec;

			if (this->running) {
				this->navega->iteracionControl();
			}

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
		this->running=false;
		this->myImage1 = (unsigned char*) calloc (this->controller->data1->description->width*this->controller->data1->description->height*3,sizeof(unsigned char));
		this->myImage2 = (unsigned char*) calloc (this->controller->data1->description->width*this->controller->data1->description->height*3,sizeof(unsigned char));

		this->numlines=0;
		this->panA=0.;
		this->tiltA=0.;
		this->panB=0.;
		this->tiltB=0.;

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

		if (idcamera==1) {
			this->get3DPositionZ(&myCamA,pdraw3D,pix,0.);// 5000 son los 5 metros a los que está la pared.
			this->add_line(myCamA.position.X, myCamA.position.Y, myCamA.position.Z,(float)pdraw3D.X,(float)pdraw3D.Y, (float)pdraw3D.Z,idcamera);
		} else {
			this->get3DPositionZ(&myCamB,pdraw3D,pix,0.);// 5000 son los 5 metros a los que está la pared.
			this->add_line(myCamB.position.X, myCamB.position.Y, myCamB.position.Z,(float)pdraw3D.X,(float)pdraw3D.Y, (float)pdraw3D.Z,idcamera);
		}
	}

	void Navegacion::pixel2optical(TPinHoleCamera *cam, HPoint2D *p) {
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

	void Navegacion::get3DPositionZ(TPinHoleCamera * camera, HPoint3D &res, HPoint2D in, float Z = 0.0) {
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

		/*Check division by zero*/
		if((p3d.Z-z) == 0.0) {
			res.H = 0.0;
			return;
		}

		zfinal = Z;

		/*Linear equation (X-x)/(p3d.X-x) = (Y-y)/(p3d.Y-y) = (Z-z)/(p3d.Z-z)*/
		xfinal = x + (p3d.X - x)*(zfinal - z)/(p3d.Z-z);
		yfinal = y + (p3d.Y - y)*(zfinal - z)/(p3d.Z-z);

		res.X = xfinal;
		res.Y = yfinal;
		res.Z = zfinal;
		res.H = 1.0;
	} 

	void Navegacion::add_line(float x0,float y0, float z0, float x1, float y1, float z1,int color){
		if (numlines < MAX_LINES) {
			extra_lines[numlines][0] = x0;
			extra_lines[numlines][1] = y0;
			extra_lines[numlines][2] = z0;
			extra_lines[numlines][3] = 0;
			extra_lines[numlines][4] = x1;
			extra_lines[numlines][5] = y1;
			extra_lines[numlines][6] = z1;
			extra_lines[numlines][7] = 0;
			extra_lines[numlines][8] = color;
			numlines ++;
		}	else {
			printf("error, too many lines in the world\n");
		}
	}

	void Navegacion::drawProjectionLines(){
		for(int i=0;i<numlines;i++) {
			CvPoint3D32f a,b,aa,ba;
			CvPoint3D32f color;
			a.x=extra_lines[i][0];
			a.y=extra_lines[i][1];
			a.z=extra_lines[i][2];
			b.x=extra_lines[i][4];
			b.y=extra_lines[i][5];
			b.z=extra_lines[i][6];

			if (extra_lines[i][8]==1){color.x = 1.;color.y = 0.;color.z = 0.;}else{color.x = 0.;color.y = 0.;color.z = 1.;}
			this->pintaSegmento (a, b, color);
		}
	}	

	void printRT(gsl_matrix *m){
		printf("---------- \n");
		for(int i=0;i<4;i++){for(int j=0;j<4;j++){printf("%f ",gsl_matrix_get(m,i,j));}printf("\n");}
	}

	void Navegacion::updateCamerasPos() {
		gsl_matrix *robotRT,*centroART, *panART, *tiltART, *temp1A,*temp2A, *temp3A, *foaRel, *foaAbsA,*centroBRT, *panBRT, *tiltBRT, *temp1B,*temp2B, *temp3B,*foaAbsB;	
		CvPoint3D32f myPoint;
		this->cogerPosicion (&myPoint);
		this->robotx=myPoint.x;
		this->roboty=myPoint.y;
		this->robottheta=myPoint.z;

		robotRT = gsl_matrix_calloc(4,4);
		centroART = gsl_matrix_calloc(4,4);
		panART= gsl_matrix_calloc(4,4);
		tiltART= gsl_matrix_calloc(4,4);
		temp1A= gsl_matrix_calloc(4,4);
		temp2A= gsl_matrix_calloc(4,4);
		temp3A= gsl_matrix_calloc(4,4);
		foaRel= gsl_matrix_calloc(4,1);
		foaAbsA=gsl_matrix_calloc(4,1);
		centroBRT = gsl_matrix_calloc(4,4);
		panBRT= gsl_matrix_calloc(4,4);
		tiltBRT= gsl_matrix_calloc(4,4);
		temp1B= gsl_matrix_calloc(4,4);
		temp2B= gsl_matrix_calloc(4,4);
		temp3B= gsl_matrix_calloc(4,4);
		foaAbsB=gsl_matrix_calloc(4,1);

		gsl_matrix_set(robotRT,0,0,cos(this->robottheta*DEGTORAD));
		gsl_matrix_set(robotRT,0,1,-sin(this->robottheta*DEGTORAD));
		gsl_matrix_set(robotRT,0,2,0.);
		gsl_matrix_set(robotRT,0,3,this->robotx);
		gsl_matrix_set(robotRT,1,0,sin(this->robottheta*DEGTORAD));
		gsl_matrix_set(robotRT,1,1,cos(this->robottheta*DEGTORAD));
		gsl_matrix_set(robotRT,1,2,0.);
		gsl_matrix_set(robotRT,1,3,this->roboty);
		gsl_matrix_set(robotRT,2,0,0.);
		gsl_matrix_set(robotRT,2,1,0.);
		gsl_matrix_set(robotRT,2,2,1.);
		gsl_matrix_set(robotRT,2,3,0.); // Z de la base robot la considero 0
		gsl_matrix_set(robotRT,3,0,0.);
		gsl_matrix_set(robotRT,3,1,0.);
		gsl_matrix_set(robotRT,3,2,0.);
		gsl_matrix_set(robotRT,3,3,1.0);

		//CAMARA A
		gsl_matrix_set(centroART,0,0,1.);
		gsl_matrix_set(centroART,0,1,0.);
		gsl_matrix_set(centroART,0,2,0.);
		gsl_matrix_set(centroART,0,3,215.);//215
		gsl_matrix_set(centroART,1,0,0.);
		gsl_matrix_set(centroART,1,1,1.);
		gsl_matrix_set(centroART,1,2,0.);
		gsl_matrix_set(centroART,1,3,110.);//110
		gsl_matrix_set(centroART,2,0,0.);
		gsl_matrix_set(centroART,2,1,0.);
		gsl_matrix_set(centroART,2,2,1.);
		gsl_matrix_set(centroART,2,3,294.); // 294
		gsl_matrix_set(centroART,3,0,0.);
		gsl_matrix_set(centroART,3,1,0.);
		gsl_matrix_set(centroART,3,2,0.);
		gsl_matrix_set(centroART,3,3,1.);

		//pan
		gsl_matrix_set(panART,0,0,cos(this->panA*DEGTORAD));
		gsl_matrix_set(panART,0,1,sin(this->panA*DEGTORAD));
		gsl_matrix_set(panART,0,2,0.);
		gsl_matrix_set(panART,0,3,0.); // translacion en x
		gsl_matrix_set(panART,1,0,-sin(this->panA*DEGTORAD));
		gsl_matrix_set(panART,1,1,cos(this->panA*DEGTORAD));
		gsl_matrix_set(panART,1,2,0.);
		gsl_matrix_set(panART,1,3,0.);// translacion en y
		gsl_matrix_set(panART,2,0,0.);
		gsl_matrix_set(panART,2,1,0.);
		gsl_matrix_set(panART,2,2,1.);
		gsl_matrix_set(panART,2,3,0.); // Altura del centro optico de la camara respecto del suelo. 
		gsl_matrix_set(panART,3,0,0.);
		gsl_matrix_set(panART,3,1,0.);
		gsl_matrix_set(panART,3,2,0.);
		gsl_matrix_set(panART,3,3,1.0);

		//tilt
		gsl_matrix_set(tiltART,0,0,cos(this->tiltA*DEGTORAD));
		gsl_matrix_set(tiltART,0,1,0.);
		gsl_matrix_set(tiltART,0,2,-sin(this->tiltA*DEGTORAD));
		gsl_matrix_set(tiltART,0,3,0.); 
		gsl_matrix_set(tiltART,1,0,0.);
		gsl_matrix_set(tiltART,1,1,1.);
		gsl_matrix_set(tiltART,1,2,0.);
		gsl_matrix_set(tiltART,1,3,0.);
		gsl_matrix_set(tiltART,2,0,sin(this->tiltA*DEGTORAD));
		gsl_matrix_set(tiltART,2,1,0.);
		gsl_matrix_set(tiltART,2,2,cos(this->tiltA*DEGTORAD));
		gsl_matrix_set(tiltART,2,3,0.); 
		gsl_matrix_set(tiltART,3,0,0.);
		gsl_matrix_set(tiltART,3,1,0.);
		gsl_matrix_set(tiltART,3,2,0.);
		gsl_matrix_set(tiltART,3,3,1.0);

		//CAMARA B
		gsl_matrix_set(centroBRT,0,0,1.);
		gsl_matrix_set(centroBRT,0,1,0.);
		gsl_matrix_set(centroBRT,0,2,0.);
		gsl_matrix_set(centroBRT,0,3,215.);//215
		gsl_matrix_set(centroBRT,1,0,0.);
		gsl_matrix_set(centroBRT,1,1,1.);
		gsl_matrix_set(centroBRT,1,2,0.);
		gsl_matrix_set(centroBRT,1,3,-110.);//110
		gsl_matrix_set(centroBRT,2,0,0.);
		gsl_matrix_set(centroBRT,2,1,0.);
		gsl_matrix_set(centroBRT,2,2,1.);
		gsl_matrix_set(centroBRT,2,3,294.); // 294
		gsl_matrix_set(centroBRT,3,0,0.);
		gsl_matrix_set(centroBRT,3,1,0.);
		gsl_matrix_set(centroBRT,3,2,0.);
		gsl_matrix_set(centroBRT,3,3,1.);

		//pan
		gsl_matrix_set(panBRT,0,0,cos(this->panB*DEGTORAD));
		gsl_matrix_set(panBRT,0,1,sin(this->panB*DEGTORAD));
		gsl_matrix_set(panBRT,0,2,0.);
		gsl_matrix_set(panBRT,0,3,0.); // translacion en x
		gsl_matrix_set(panBRT,1,0,-sin(this->panB*DEGTORAD));
		gsl_matrix_set(panBRT,1,1,cos(this->panB*DEGTORAD));
		gsl_matrix_set(panBRT,1,2,0.);
		gsl_matrix_set(panBRT,1,3,0.);// translacion en y
		gsl_matrix_set(panBRT,2,0,0.);
		gsl_matrix_set(panBRT,2,1,0.);
		gsl_matrix_set(panBRT,2,2,1.);
		gsl_matrix_set(panBRT,2,3,0.); // Altura del centro optico de la camara respecto del suelo. 
		gsl_matrix_set(panBRT,3,0,0.);
		gsl_matrix_set(panBRT,3,1,0.);
		gsl_matrix_set(panBRT,3,2,0.);
		gsl_matrix_set(panBRT,3,3,1.0);

		//tilt
		gsl_matrix_set(tiltBRT,0,0,cos(this->tiltB*DEGTORAD));
		gsl_matrix_set(tiltBRT,0,1,0.);
		gsl_matrix_set(tiltBRT,0,2,-sin(this->tiltB*DEGTORAD));
		gsl_matrix_set(tiltBRT,0,3,0.); 
		gsl_matrix_set(tiltBRT,1,0,0.);
		gsl_matrix_set(tiltBRT,1,1,1.);
		gsl_matrix_set(tiltBRT,1,2,0.);
		gsl_matrix_set(tiltBRT,1,3,0.);
		gsl_matrix_set(tiltBRT,2,0,sin(this->tiltB*DEGTORAD));
		gsl_matrix_set(tiltBRT,2,1,0.);
		gsl_matrix_set(tiltBRT,2,2,cos(this->tiltB*DEGTORAD));
		gsl_matrix_set(tiltBRT,2,3,0.); 
		gsl_matrix_set(tiltBRT,3,0,0.);
		gsl_matrix_set(tiltBRT,3,1,0.);
		gsl_matrix_set(tiltBRT,3,2,0.);
		gsl_matrix_set(tiltBRT,3,3,1.0);

		gsl_linalg_matmult (robotRT, centroART, temp1A);
		gsl_linalg_matmult (temp1A,panART,temp2A);
		gsl_linalg_matmult (temp2A,tiltART, temp3A);

		gsl_linalg_matmult (robotRT, centroBRT, temp1B);
		gsl_linalg_matmult (temp1B,panBRT,temp2B);
		gsl_linalg_matmult (temp2B,tiltBRT, temp3B);

		gsl_matrix_set(foaRel,0,0,1000.0);
		gsl_matrix_set(foaRel,1,0,0.0);
		gsl_matrix_set(foaRel,2,0,0.0);
		gsl_matrix_set(foaRel,3,0,1.0);

		gsl_linalg_matmult(temp3A,foaRel,foaAbsA);
		gsl_linalg_matmult(temp3B,foaRel,foaAbsB);	

		myCamA.position.X = gsl_matrix_get (temp3A, 0, 3);
		myCamA.position.Y = gsl_matrix_get (temp3A, 1, 3);
		myCamA.position.Z = gsl_matrix_get (temp3A, 2, 3);
		myCamA.foa.X=(float)gsl_matrix_get(foaAbsA,0,0);
		myCamA.foa.Y=(float)gsl_matrix_get(foaAbsA,1,0);
		myCamA.foa.Z=(float)gsl_matrix_get(foaAbsA,2,0);

		myCamB.position.X = gsl_matrix_get (temp3B, 0, 3);
		myCamB.position.Y = gsl_matrix_get (temp3B, 1, 3);
		myCamB.position.Z = gsl_matrix_get (temp3B, 2, 3);
		myCamB.foa.X=(float)gsl_matrix_get(foaAbsB,0,0);
		myCamB.foa.Y=(float)gsl_matrix_get(foaAbsB,1,0);
		myCamB.foa.Z=(float)gsl_matrix_get(foaAbsB,2,0);

		update_camera_matrix (&myCamA);
		//printCameraInformation (&myCamA);
		update_camera_matrix (&myCamB);
		//printCameraInformation (&myCamB);

		gsl_matrix_free(foaRel);
		gsl_matrix_free(robotRT);

		gsl_matrix_free(centroART);
		gsl_matrix_free(panART);
		gsl_matrix_free(tiltART);
		gsl_matrix_free(temp1A);
		gsl_matrix_free(temp2A);
		gsl_matrix_free(temp3A);
		gsl_matrix_free(foaAbsA);

		gsl_matrix_free(centroBRT);
		gsl_matrix_free(panBRT);
		gsl_matrix_free(tiltBRT);
		gsl_matrix_free(temp1B);
		gsl_matrix_free(temp2B);
		gsl_matrix_free(temp3B);
		gsl_matrix_free(foaAbsB);
	}

	void Navegacion::resetLines(){
		numlines = 0;
	}

	void Navegacion::printCameraInformation (TPinHoleCamera* actualCamera) {
		printf ("\n");
		printf ("CAMERA INFORMATION\n");
		printf ("==================\n");
		printf ("Position = [%f, %f, %f]\n", actualCamera->position.X, actualCamera->position.Y, actualCamera->position.Z);
		printf ("FOA = [%f, %f, %f]\n", actualCamera->foa.X, actualCamera->foa.Y, actualCamera->foa.Z);
		printf ("fdist = [%f, %f], u0 = %f, v0 = %f, roll = %f\n", actualCamera->fdistx, actualCamera->fdisty, actualCamera->u0, actualCamera->v0, actualCamera->roll);
		printf ("K matrix:\n");
		printf ("%f %f %f %f\n", actualCamera->k11, actualCamera->k12, actualCamera->k13, actualCamera->k14);
		printf ("%f %f %f %f\n", actualCamera->k21, actualCamera->k22, actualCamera->k23, actualCamera->k24);
		printf ("%f %f %f %f\n", actualCamera->k31, actualCamera->k32, actualCamera->k33, actualCamera->k34);
		printf ("RT matrix:\n");
		printf ("%f %f %f %f\n", actualCamera->rt11, actualCamera->rt12, actualCamera->rt13, actualCamera->rt14);
		printf ("%f %f %f %f\n", actualCamera->rt21, actualCamera->rt22, actualCamera->rt23, actualCamera->rt24);
		printf ("%f %f %f %f\n", actualCamera->rt31, actualCamera->rt32, actualCamera->rt33, actualCamera->rt34);
		printf ("%f %f %f %f\n", actualCamera->rt41, actualCamera->rt42, actualCamera->rt43, actualCamera->rt44);
	}

	void Navegacion::updatePT1(float pan, float tilt){
		this->panA=pan;
		this->tiltA=tilt;
	}

	void Navegacion::updatePT2(float pan, float tilt){
		this->panB=pan;
		this->tiltB=tilt;
	}
} // namespace
