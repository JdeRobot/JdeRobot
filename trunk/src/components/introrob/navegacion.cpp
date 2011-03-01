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
	/* color (R, G, B) */
	int Navegacion::pintaSegmento (CvPoint3D32f a, CvPoint3D32f b, CvPoint3D32f color) {
		glColor3f(color.x, color.y, color.z);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
			v3f(a.x, a.y, a.z);
			v3f(b.x, b.y, b.z);
		glEnd();
		return 1;
	}

	/* Calcula la posicion relativa respecto del robot de un punto absoluto. El robot se encuentra en robotx, roboty con orientacion robotheta respecto al sistema de referencia absoluto */
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

	/* Calcula la posicion absoluta de un punto expresado en el sistema de coordenadas solidario al robot. El robot se encuentra en robotx, roboty con orientacion robottheta respecto al sistema de referencia absoluto */
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

	void Navegacion::cogerImagen1(unsigned char* image) { // refresco el contenido de la imagen1
		image = &this->controller->data1->pixelData[0];
	}

	void Navegacion::cogerImagen2(unsigned char* image) { // refresco el contenido de la imagen1
		image = &this->controller->data2->pixelData[0];
	}

	void Navegacion::cogerPosicion(CvPoint3D32f* myPoint) { // refresco la posición del pioneer
		myPoint->x = this->controller->ed->robotx;
		myPoint->y = this->controller->ed->roboty;
		myPoint->z = this->controller->ed->robottheta;
	}

	void Navegacion::cogerLaser(std::vector<float>* laser, int *numLasers) { // refrescamos el vector de valores de láser
		int k;
		laser->clear();
		for (k = 0; k < this->controller->ld->numLaser; k++) {
			laser->push_back (this->controller->ld->distanceData[k]);
			*numLasers++;
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
		this->navega = new Navega (this->controller, this, this->view->world);
		this->running=false;
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
} // namespace
